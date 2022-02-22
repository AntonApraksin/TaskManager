#include "model/DefaultModelController.h"

#include <gtest/gtest.h>

#include "model/task_manager/TaskManager.h"
#include "persistence/Persistence.h"
#include "test_utils/MockPersistence.h"
#include "test_utils/MockTaskIdProducer.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/operators.h"
#include "test_utils/utils.h"

class DefaultModelControllerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        auto mtip = std::make_unique<MockTaskIdProducer>();
        auto tm = std::make_unique<TaskManager>(std::move(mtip));
        auto persistence = std::make_unique<MockPersistence>();
        persistence_ = persistence.get();
        model_controller_ = std::make_unique<DefaultModelController>(
            std::move(tm), std::move(persistence));
    }
    MockPersistence* persistence_;
    std::unique_ptr<DefaultModelController> model_controller_;
    TaskFactory task_factory_;
};

TEST_F(DefaultModelControllerTest, AddMustBePerformed)
{
    auto to_add = task_factory_.GetNextTask();
    ASSERT_EQ(model_controller_->AddTask(to_add).GetStatus(),
              ModelController::Status::kOk);
    auto solid_tasks = model_controller_->GetAllSolidTasks();
    ASSERT_EQ(solid_tasks.GetStatus(), ModelController::Status::kOk);
    EXPECT_EQ(solid_tasks.AccessResult().size(), 1);
    EXPECT_EQ(solid_tasks.AccessResult()[0], TaskToSolidTask(to_add, 0));
}

TEST_F(DefaultModelControllerTest, EditMustBePerformed)
{
    auto to_add = task_factory_.GetNextTask();
    auto edited = task_factory_.GetNextTask();
    ASSERT_EQ(model_controller_->AddTask(to_add).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->Edit(CreateTaskId(0), edited).GetStatus(),
              ModelController::Status::kOk);
    auto solid_tasks = model_controller_->GetAllSolidTasks();
    ASSERT_EQ(solid_tasks.GetStatus(), ModelController::Status::kOk);
    EXPECT_EQ(solid_tasks.AccessResult().size(), 1);
    EXPECT_EQ(solid_tasks.AccessResult()[0], TaskToSolidTask(edited, 0));
}

TEST_F(DefaultModelControllerTest, CompleteMustBePerformed)
{
    auto to_add = task_factory_.GetNextTask();
    ASSERT_EQ(model_controller_->AddTask(to_add).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->Complete(CreateTaskId(0)).GetStatus(),
              ModelController::Status::kOk);
    auto solid_tasks = model_controller_->GetAllSolidTasks();
    ASSERT_EQ(solid_tasks.GetStatus(), ModelController::Status::kOk);
    EXPECT_EQ(solid_tasks.AccessResult().size(), 1);
    EXPECT_EQ(solid_tasks.AccessResult()[0].task().progress(), Task::kCompleted);
}

TEST_F(DefaultModelControllerTest, DeleteMustBePerformed)
{
    auto to_add = task_factory_.GetNextTask();
    ASSERT_EQ(model_controller_->AddTask(to_add).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->Delete(CreateTaskId(0)).GetStatus(),
              ModelController::Status::kOk);
    auto solid_tasks = model_controller_->GetAllSolidTasks();
    ASSERT_EQ(solid_tasks.GetStatus(), ModelController::Status::kOk);
    EXPECT_EQ(solid_tasks.AccessResult().size(), 0);
}

TEST_F(DefaultModelControllerTest, AddSubtaskMustBePerformed)
{
    auto to_add = task_factory_.GetNextTask();
    auto to_sub_add = task_factory_.GetNextTask();
    ASSERT_EQ(model_controller_->AddTask(to_add).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(0), to_sub_add).GetStatus(),
              ModelController::Status::kOk);

    auto solid_tasks = model_controller_->GetAllSolidTasks();
    ASSERT_EQ(solid_tasks.GetStatus(), ModelController::Status::kOk);

    EXPECT_EQ(solid_tasks.AccessResult()[0], TaskToSolidTask(to_add, 0));
    EXPECT_EQ(solid_tasks.AccessResult()[1], TaskToSolidTask(to_sub_add, 1, 0));
}

TEST_F(DefaultModelControllerTest, EditOnUnexistingIdMustResultInNotPresentId)
{
    auto to_edit = task_factory_.GetNextTask();
    ASSERT_EQ(model_controller_->Edit(CreateTaskId(0), to_edit).GetStatus(),
              ModelController::Status::kNotPresentId);
    ASSERT_EQ(model_controller_->GetAllSolidTasks().AccessResult().size(), 0);
}

TEST_F(DefaultModelControllerTest,
       CompleteOnUnexistingIdMustResultInNotPresentId)
{
    ASSERT_EQ(model_controller_->Complete(CreateTaskId(0)).GetStatus(),
              ModelController::Status::kNotPresentId);
    ASSERT_EQ(model_controller_->GetAllSolidTasks().AccessResult().size(), 0);
}

TEST_F(DefaultModelControllerTest,
       DeleteOnUnexistingIdMustResultInNotPresentId)
{
    ASSERT_EQ(model_controller_->Delete(CreateTaskId(0)).GetStatus(),
              ModelController::Status::kNotPresentId);
    ASSERT_EQ(model_controller_->GetAllSolidTasks().AccessResult().size(), 0);
}

TEST_F(DefaultModelControllerTest,
       NestedAddOnUnexistingIdMustResultInNotPresentId)
{
    auto to_add = task_factory_.GetNextTask();
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(0), to_add).GetStatus(),
              ModelController::Status::kNotPresentId);
    ASSERT_EQ(model_controller_->GetAllSolidTasks().AccessResult().size(), 0);
}

TEST_F(DefaultModelControllerTest, CompleteMustCompleteNestedTasks)
{
    auto _0 = task_factory_.GetNextTask();     // NOLINT
    auto __1 = task_factory_.GetNextTask();    // NOLINT
    auto ___3 = task_factory_.GetNextTask();   // NOLINT
    auto ____5 = task_factory_.GetNextTask();  // NOLINT
    auto ___4 = task_factory_.GetNextTask();   // NOLINT
    auto __2 = task_factory_.GetNextTask();    // NOLINT
    ASSERT_EQ(model_controller_->AddTask(_0).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(0), __1).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(0), __2).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(1), ___3).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(1), ___4).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(3), ____5).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->Complete(CreateTaskId(0)).GetStatus(),
              ModelController::Status::kOk);

    auto result = model_controller_->GetAllSolidTasks();
    ASSERT_EQ(result.AccessResult().size(), 6);
    for (const auto& i : result.AccessResult())
    {
        EXPECT_EQ(i.task().progress(), Task::kCompleted);
    }
}

TEST_F(DefaultModelControllerTest, DeleteMustDeleteNestedTasks)
{
    auto _0 = task_factory_.GetNextTask();     // NOLINT
    auto __1 = task_factory_.GetNextTask();    // NOLINT
    auto ___3 = task_factory_.GetNextTask();   // NOLINT
    auto ____5 = task_factory_.GetNextTask();  // NOLINT
    auto ___4 = task_factory_.GetNextTask();   // NOLINT
    auto __2 = task_factory_.GetNextTask();    // NOLINT
    auto _6 = task_factory_.GetNextTask();     // NOLINT
    ASSERT_EQ(model_controller_->AddTask(_0).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(0), __1).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(0), __2).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(1), ___3).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(1), ___4).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(3), ____5).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddTask(_6).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->Delete(CreateTaskId(0)).GetStatus(),
              ModelController::Status::kOk);

    auto result = model_controller_->GetAllSolidTasks();
    ASSERT_EQ(result.AccessResult().size(), 1);
    EXPECT_EQ(result.AccessResult()[0], TaskToSolidTask(_6, 6));
}

TEST_F(DefaultModelControllerTest,
       GetAllSolidTasksMustReturnAllSolidTasksSorted)
{
    auto _0 = task_factory_.GetNextTask();     // NOLINT
    auto __1 = task_factory_.GetNextTask();    // NOLINT
    auto ___3 = task_factory_.GetNextTask();   // NOLINT
    auto ____7 = task_factory_.GetNextTask();  // NOLINT
    auto ___6 = task_factory_.GetNextTask();   // NOLINT
    auto __2 = task_factory_.GetNextTask();    // NOLINT
    auto _4 = task_factory_.GetNextTask();     // NOLINT
    auto __5 = task_factory_.GetNextTask();    // NOLINT
    auto _8 = task_factory_.GetNextTask();     // NOLINT
    ASSERT_EQ(model_controller_->AddTask(_0).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(0), __1).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(0), __2).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(1), ___3).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddTask(_4).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(4), __5).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(1), ___6).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(3), ____7).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddTask(_8).GetStatus(),
              ModelController::Status::kOk);

    SolidTasks expected{
        TaskToSolidTask(_8, 8), TaskToSolidTask(_4, 4),
        TaskToSolidTask(__5, 5, 4), TaskToSolidTask(_0, 0),
        TaskToSolidTask(__2, 2, 0), TaskToSolidTask(__1, 1, 0),
        TaskToSolidTask(___6, 6, 1), TaskToSolidTask(___3, 3, 1),
        TaskToSolidTask(____7, 7, 3),
    };
    auto result = model_controller_->GetAllSolidTasks();
    ASSERT_EQ(result.GetStatus(), ModelController::Status::kOk);
    EXPECT_EQ(result.AccessResult().size(), expected.size());
    for (size_t i{0}, sz{expected.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(result.AccessResult()[i], expected[i]);
    }
}

TEST_F(DefaultModelControllerTest, GetSpecificSolidTasks)
{
    auto _0 = task_factory_.GetNextTask();     // NOLINT
    auto __1 = task_factory_.GetNextTask();    // NOLINT
    auto ___3 = task_factory_.GetNextTask();   // NOLINT
    auto ____7 = task_factory_.GetNextTask();  // NOLINT
    auto ___6 = task_factory_.GetNextTask();   // NOLINT
    auto __2 = task_factory_.GetNextTask();    // NOLINT
    auto _4 = task_factory_.GetNextTask();     // NOLINT
    auto __5 = task_factory_.GetNextTask();    // NOLINT
    auto _8 = task_factory_.GetNextTask();     // NOLINT
    ASSERT_EQ(model_controller_->AddTask(_0).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(0), __1).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(0), __2).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(1), ___3).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddTask(_4).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(4), __5).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(1), ___6).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddSubtask(CreateTaskId(3), ____7).GetStatus(),
              ModelController::Status::kOk);
    ASSERT_EQ(model_controller_->AddTask(_8).GetStatus(),
              ModelController::Status::kOk);

    SolidTasks expected{
        TaskToSolidTask(_0, 0), TaskToSolidTask(__2, 2, 0),
        TaskToSolidTask(__1, 1, 0), TaskToSolidTask(___6, 6, 1),
        TaskToSolidTask(___3, 3, 1), TaskToSolidTask(____7, 7, 3),
        TaskToSolidTask(_8, 8),
    };

    auto result = model_controller_->GetSpecificSolidTasks(
        {CreateTaskId(0), CreateTaskId(8)});
    ASSERT_EQ(result.GetStatus(), ModelController::Status::kOk);
    EXPECT_EQ(result.AccessResult().size(), expected.size());
    for (size_t i{0}, sz{expected.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(result.AccessResult()[i], expected[i]);
    }
}

TEST_F(DefaultModelControllerTest, MustCallSaveOnPersistence)
{
    auto _0 = task_factory_.GetNextTask();     // NOLINT
    auto __1 = task_factory_.GetNextTask();    // NOLINT
    auto ___3 = task_factory_.GetNextTask();   // NOLINT
    auto ____7 = task_factory_.GetNextTask();  // NOLINT
    auto ___6 = task_factory_.GetNextTask();   // NOLINT
    auto __2 = task_factory_.GetNextTask();    // NOLINT
    auto _4 = task_factory_.GetNextTask();     // NOLINT
    auto __5 = task_factory_.GetNextTask();    // NOLINT
    auto _8 = task_factory_.GetNextTask();     // NOLINT
    model_controller_->AddTask(_0);
    model_controller_->AddSubtask(CreateTaskId(0), __1);
    model_controller_->AddSubtask(CreateTaskId(0), __2);
    model_controller_->AddSubtask(CreateTaskId(1), ___3);
    model_controller_->AddTask(_4);
    model_controller_->AddSubtask(CreateTaskId(4), __5);
    model_controller_->AddSubtask(CreateTaskId(1), ___6);
    model_controller_->AddSubtask(CreateTaskId(3), ____7);
    model_controller_->AddTask(_8);

    auto expected = model_controller_->GetAllSolidTasks().AccessResult();

    EXPECT_CALL(*persistence_, Save(expected)).Times(1);
    auto result = model_controller_->Save();
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kOk);
}

TEST_F(DefaultModelControllerTest, MustCallLoadOnPersistence)
{
    auto _0 = task_factory_.GetNextTask();     // NOLINT
    auto __1 = task_factory_.GetNextTask();    // NOLINT
    auto ___3 = task_factory_.GetNextTask();   // NOLINT
    auto ____7 = task_factory_.GetNextTask();  // NOLINT
    auto ___6 = task_factory_.GetNextTask();   // NOLINT
    auto __2 = task_factory_.GetNextTask();    // NOLINT
    auto _4 = task_factory_.GetNextTask();     // NOLINT
    auto __5 = task_factory_.GetNextTask();    // NOLINT
    auto _8 = task_factory_.GetNextTask();     // NOLINT
    model_controller_->AddTask(_0);
    model_controller_->AddSubtask(CreateTaskId(0), __1);
    model_controller_->AddSubtask(CreateTaskId(0), __2);
    model_controller_->AddSubtask(CreateTaskId(1), ___3);
    model_controller_->AddTask(_4);
    model_controller_->AddSubtask(CreateTaskId(4), __5);
    model_controller_->AddSubtask(CreateTaskId(1), ___6);
    model_controller_->AddSubtask(CreateTaskId(3), ____7);
    model_controller_->AddTask(_8);

    auto to_load = OperationResult<Persistence::Status, SolidTasks>::Ok(
        model_controller_->GetAllSolidTasks().AccessResult());
    model_controller_->Delete(CreateTaskId(0));
    model_controller_->Delete(CreateTaskId(4));
    model_controller_->Delete(CreateTaskId(8));
    ASSERT_TRUE(model_controller_->GetAllSolidTasks().AccessResult().empty());

    EXPECT_CALL(*persistence_, Load()).WillOnce(testing::Return(to_load));
    auto result = model_controller_->Load();
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kOk);

    auto loaded_state = model_controller_->GetAllSolidTasks().AccessResult();
    ASSERT_EQ(to_load.AccessResult().size(), loaded_state.size());

    auto previous_state = to_load.AccessResult();
    std::sort(previous_state.begin(), previous_state.end(),
              [](auto& lhs, auto& rhs) { return lhs.task_id() < rhs.task_id(); });
    std::sort(loaded_state.begin(), loaded_state.end(),
              [](auto& lhs, auto& rhs) { return lhs.task_id() < rhs.task_id(); });

    ASSERT_TRUE(std::equal(previous_state.cbegin(), previous_state.cend(),
                           loaded_state.cbegin()));
}

TEST_F(DefaultModelControllerTest, MustAddLabel)
{
    auto task = task_factory_.GetNextTask();
    auto id = model_controller_->AddTask(task).AccessResult();
    Label label;
    label.set_name("label1");
    auto result = model_controller_->AddLabel(id, label);
    ASSERT_EQ(result.GetStatus(), ModelController::Status::kOk);
    auto storage = model_controller_->GetAllSolidTasks().AccessResult();
    auto got_task = storage.at(0).task();
    ASSERT_EQ(got_task.labels_size(), 1);
    EXPECT_EQ(got_task.labels()[0].name(), label.name());
}

TEST_F(DefaultModelControllerTest, MustDeleteLabel)
{
    auto task = task_factory_.GetNextTask();
    auto id = model_controller_->AddTask(task).AccessResult();
    Label label;
    label.set_name("label1");
    auto result = model_controller_->AddLabel(id, label);
    ASSERT_EQ(result.GetStatus(), ModelController::Status::kOk);
    result = model_controller_->DeleteLabel(id, label);
    ASSERT_EQ(result.GetStatus(), ModelController::Status::kOk);
    auto storage = model_controller_->GetAllSolidTasks().AccessResult();
    auto got_task = storage.at(0).task();
    ASSERT_EQ(got_task.labels_size(), 0);
}

TEST_F(DefaultModelControllerTest,
       AddLabelWithNotPresentIdMustResultInkNotPresentId)
{
    auto task = task_factory_.GetNextTask();
    auto id = model_controller_->AddTask(task).AccessResult();
    model_controller_->Delete(id);
    Label label;
    label.set_name("label1");
    auto result = model_controller_->AddLabel(id, label);
    ASSERT_EQ(result.GetStatus(), ModelController::Status::kNotPresentId);
}

TEST_F(DefaultModelControllerTest,
       AddLabelWithRepeatedLabelMustNotAddItAndReturnkOk)
{
    auto task = task_factory_.GetNextTask();
    auto id = model_controller_->AddTask(task).AccessResult();
    Label label;
    label.set_name("label1");
    auto result = model_controller_->AddLabel(id, label);
    ASSERT_EQ(result.GetStatus(), ModelController::Status::kOk);
    result = model_controller_->AddLabel(id, label);
    ASSERT_EQ(result.GetStatus(), ModelController::Status::kOk);
    auto storage = model_controller_->GetAllSolidTasks().AccessResult();
    auto got_task = storage.at(0).task();
    ASSERT_EQ(got_task.labels_size(), 1);
    EXPECT_EQ(got_task.labels()[0].name(), label.name());
}

TEST_F(DefaultModelControllerTest,
       DeleteLabelWithNotPresentIdMustResultInkNotPresentId)
{
    auto task = task_factory_.GetNextTask();
    auto id = model_controller_->AddTask(task).AccessResult();
    model_controller_->Delete(id);
    Label label;
    label.set_name("label1");
    auto result = model_controller_->DeleteLabel(id, label);
    ASSERT_EQ(result.GetStatus(), ModelController::Status::kNotPresentId);
}

TEST_F(DefaultModelControllerTest,
       DeleteLabelWithNotPresentLabelMustResultInkNotPresentLabel)
{
    auto task = task_factory_.GetNextTask();
    auto id = model_controller_->AddTask(task).AccessResult();
    Label label;
    label.set_name("label1");
    auto result = model_controller_->DeleteLabel(id, label);
    ASSERT_EQ(result.GetStatus(), ModelController::Status::kNotPresentLabel);
}
