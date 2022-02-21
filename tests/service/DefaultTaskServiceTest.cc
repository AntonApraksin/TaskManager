#include <gtest/gtest.h>

#include "model/DefaultModelController.h"
#include "model/task_manager/TaskManager.h"
#include "service/DefaultTaskService.h"
#include "test_utils/MockPersistence.h"
#include "test_utils/MockTaskIdProducer.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/operators.h"

class DefaultTaskServiceTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        auto id_producer = std::make_unique<MockTaskIdProducer>();
        auto task_manager = std::make_unique<TaskManager>(std::move(id_producer));
        auto mock_persistence = std::make_unique<MockPersistence>();
        persistence = mock_persistence.get();
        auto made_model_controller = std::make_unique<DefaultModelController>(
            std::move(task_manager), std::move(mock_persistence));
        model_controller = made_model_controller.get();
        task_service =
            std::make_unique<DefaultTaskService>(std::move(made_model_controller));
    }

protected:
    std::unique_ptr<DefaultTaskService> task_service;  // NOLINT
    MockPersistence* persistence;                      // NOLINT
    ModelController* model_controller;                 // NOLINT
    TaskFactory task_factory;                          // NOLINT
};

TEST_F(DefaultTaskServiceTest, MustAddTask)
{
    auto request = task_factory.GetNextTask();
    TaskIdResponse response;
    task_service->AddTask(nullptr, &request, &response);
    auto storage = model_controller->GetAllSolidTasks().AccessResult();
    ASSERT_EQ(storage.size(), 1);
    EXPECT_EQ(storage[0].task(), request);
}

TEST_F(DefaultTaskServiceTest, MustAddSubtask)
{
    auto first = task_factory.GetNextTask();
    auto id = model_controller->Add(std::move(first)).AccessResult();
    auto second = task_factory.GetNextTask();
    TaskAndTaskIdRequest request;
    request.set_allocated_task(new Task(second));
    request.set_allocated_task_id(new TaskId(id));
    TaskIdResponse response;
    task_service->AddSubtask(nullptr, &request, &response);
    auto storage = model_controller->GetAllSolidTasks().AccessResult();
    ASSERT_EQ(storage.size(), 2);
    EXPECT_EQ(storage[1].task(), second);
}

TEST_F(DefaultTaskServiceTest, MustEditTask)
{
    auto task = task_factory.GetNextTask();
    auto id = model_controller->Add(std::move(task)).AccessResult();
    auto new_task = task_factory.GetNextTask();
    TaskAndTaskIdRequest request;
    request.set_allocated_task(new Task(new_task));
    request.set_allocated_task_id(new TaskId(id));
    PlainResponse response;
    task_service->Edit(nullptr, &request, &response);
    auto storage = model_controller->GetAllSolidTasks().AccessResult();
    ASSERT_EQ(storage.size(), 1);
    EXPECT_EQ(storage[0].task(), new_task);
}

TEST_F(DefaultTaskServiceTest, MustCompleteTask)
{
    auto task = task_factory.GetNextTask();
    auto id = model_controller->Add(std::move(task)).AccessResult();
    PlainResponse response;
    task_service->Complete(nullptr, &id, &response);
    auto storage = model_controller->GetAllSolidTasks().AccessResult();
    ASSERT_EQ(storage.size(), 1);
    EXPECT_EQ(storage[0].task().progress(), Task::kCompleted);
}

TEST_F(DefaultTaskServiceTest, MustDeleteTask)
{
    auto task = task_factory.GetNextTask();
    auto id = model_controller->Add(std::move(task)).AccessResult();
    PlainResponse response;
    task_service->Delete(nullptr, &id, &response);
    auto storage = model_controller->GetAllSolidTasks().AccessResult();
    ASSERT_TRUE(storage.empty());
}

TEST_F(DefaultTaskServiceTest, MustGetAllTasks)
{
    auto task1 = task_factory.GetNextTask();
    auto task2 = task_factory.GetNextTask();
    auto id1 = model_controller->Add(task1).AccessResult();
    auto id2 = model_controller->Add(id1, task1).AccessResult();
    google::protobuf::Empty request;
    SolidTasksResponse response;
    task_service->GetAllSolidTasks(nullptr, &request, &response);
    auto storage = model_controller->GetAllSolidTasks().AccessResult();
    ASSERT_EQ(storage.size(), response.solid_tasks().size());
    EXPECT_TRUE(std::equal(storage.cbegin(), storage.cend(),
                           response.solid_tasks().cbegin()));
}

TEST_F(DefaultTaskServiceTest, MustGetSpecificTasks)
{
    auto task1 = task_factory.GetNextTask();
    auto task2 = task_factory.GetNextTask();
    auto id1 = model_controller->Add(task1).AccessResult();
    auto id2 = model_controller->Add(id1, task1).AccessResult();
    TaskIdsRequest request;
    request.add_task_ids()->CopyFrom(id1);
    SolidTasksResponse response;
    task_service->GetSpecifiedSolidTasks(nullptr, &request, &response);
    EXPECT_EQ(response.solid_tasks().cbegin()->task(), task1);
}

TEST_F(DefaultTaskServiceTest,
       MustCallSaveOnPersistenceIndirectly)  // TODO: Fix this test
{
    auto task = task_factory.GetNextTask();
    auto id = model_controller->Add(std::move(task)).AccessResult();
    EXPECT_CALL(*persistence, Save(testing::_)).Times(1);
    google::protobuf::Empty request;
    PlainResponse response;
    task_service->Save(nullptr, &request, &response);
}

TEST_F(DefaultTaskServiceTest,
       MustCallLoadOnPersistenceIndirectly)  // TODO: Fix this test
{
    auto task = task_factory.GetNextTask();
    auto id = model_controller->Add(std::move(task)).AccessResult();
    auto solid_tasks = model_controller->GetAllSolidTasks().AccessResult();
    EXPECT_CALL(*persistence, Load())
        .WillOnce(testing::Return(
            OperationResult<Persistence::Status, SolidTasks>::Ok(solid_tasks)));
    google::protobuf::Empty request;
    PlainResponse response;
    task_service->Load(nullptr, &request, &response);
}

TEST_F(DefaultTaskServiceTest, MustAddLabelToTask)
{
    auto task = task_factory.GetNextTask();
    auto id = model_controller->Add(std::move(task)).AccessResult();
    Label label;
    label.set_name("label");
    TaskIdAndLabelRequest request;
    request.set_allocated_task_id(new TaskId(id));
    request.set_allocated_label(new Label(label));
    PlainResponse response;
    task_service->AddLabel(nullptr, &request, &response);
    auto storage = model_controller->GetAllSolidTasks().AccessResult();
    ASSERT_EQ(storage.size(), 1);
    EXPECT_EQ(storage[0].task().labels().size(), 1);
    EXPECT_EQ(storage[0].task().labels(0).name(), label.name());
}

TEST_F(DefaultTaskServiceTest, MustDeleteLabelFromTask)
{
    auto task = task_factory.GetNextTask();
    auto id = model_controller->Add(std::move(task)).AccessResult();
    Label label;
    label.set_name("label");
    model_controller->AddLabel(id, label);
    TaskIdAndLabelRequest request;
    request.set_allocated_task_id(new TaskId(id));
    request.set_allocated_label(new Label(label));
    PlainResponse response;
    task_service->DeleteLabel(nullptr, &request, &response);
    auto storage = model_controller->GetAllSolidTasks().AccessResult();
    ASSERT_EQ(storage.size(), 1);
    EXPECT_EQ(storage[0].task().labels().size(), 0);
}
