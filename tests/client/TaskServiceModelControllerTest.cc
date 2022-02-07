#include <gtest/gtest.h>

#include "TaskService_mock.grpc.pb.h"
#include "client/TaskServiceModelController.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/operators.h"

namespace task_manager
{
bool operator==(const TaskAndTaskIdRequest& lhs,
                const TaskAndTaskIdRequest& rhs)
{
    return lhs.task_id() == rhs.task_id() and lhs.task() == rhs.task();
}

bool operator==(const TaskIdAndLabelRequest& lhs,
                const TaskIdAndLabelRequest& rhs)
{
    return lhs.task_id() == rhs.task_id() and lhs.label().name() == rhs.label().name();
}

bool operator==(const Label& lhs, const Label& rhs)
{
    return lhs.name() == rhs.name();
}

TaskIdResponse CreateTaskIdResponseWithOk(google::protobuf::int32 id)
{
    TaskIdResponse ret;
    ret.set_allocated_task_id(new TaskId(CreateTaskId(id)));
    ret.set_status(kOk);
    return ret;
}

PlainResponse CreatePlainResponseWithOk()
{
    PlainResponse ret;
    ret.set_status(kOk);
    return ret;
}

template<typename Response>
Response CreateFaultyResponse(TaskServiceStatus status)
{
    Response response;
    response.set_status(status);
    return response;
}

template<typename Request, typename Response>
auto InvokeResultFactory(Response expected_response)
{
    return [expected_response = std::move(expected_response)](
        ::grpc::ClientContext*, const Request&, Response* response)
    {
        *response = expected_response;
        return grpc::Status::OK;
    };
}
}  // namespace task_manager

class TaskServiceModelControllerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        auto stub = std::make_unique<MockTaskServiceStub>();
        mock_task_service_stub = stub.get();
        task_service_model_controller =
            std::make_unique<TaskServiceModelController>(std::move(stub));
    }

    std::unique_ptr<TaskServiceModelController>
        task_service_model_controller;            // NOLINT
    MockTaskServiceStub* mock_task_service_stub;  // NOLINT
    TaskFactory task_factory;                     // NOLINT
};

TEST_F(TaskServiceModelControllerTest, MustAddTask)
{
    using testing::_;
    auto task = task_factory.GetNextTask();

    EXPECT_CALL(*mock_task_service_stub, AddTask(_, task, _))
        .WillOnce(testing::Invoke(InvokeResultFactory<Task, TaskIdResponse>(
            CreateTaskIdResponseWithOk(0))));

    auto result = task_service_model_controller->AddTask(task);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kOk);
    EXPECT_EQ(result.AccessResult().id(), 0);
}

TEST_F(TaskServiceModelControllerTest, FaultyAddTask)
{
    using testing::_;
    auto task = task_factory.GetNextTask();

    EXPECT_CALL(*mock_task_service_stub, AddTask(_, task, _))
        .WillOnce(testing::Invoke(InvokeResultFactory<Task, TaskIdResponse>(
            CreateFaultyResponse<TaskIdResponse>(task_manager::kNotPresentId))));

    auto result = task_service_model_controller->AddTask(task);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kNotPresentId);
}

TEST_F(TaskServiceModelControllerTest, MustAddSubtask)
{
    using testing::_;
    auto task = task_factory.GetNextTask();
    auto id = CreateTaskId(0);
    TaskAndTaskIdRequest request;
    request.set_allocated_task_id(new TaskId(id));
    request.set_allocated_task(new Task(task));

    EXPECT_CALL(*mock_task_service_stub, AddSubtask(_, request, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<TaskAndTaskIdRequest, TaskIdResponse>(
                CreateTaskIdResponseWithOk(1))));

    auto result = task_service_model_controller->AddSubtask(id, task);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kOk);
    EXPECT_EQ(result.AccessResult().id(), 1);
}

TEST_F(TaskServiceModelControllerTest, FaultyAddSubtask)
{
    using testing::_;
    auto task = task_factory.GetNextTask();
    auto id = CreateTaskId(0);
    TaskAndTaskIdRequest request;
    request.set_allocated_task_id(new TaskId(id));
    request.set_allocated_task(new Task(task));

    EXPECT_CALL(*mock_task_service_stub, AddSubtask(_, request, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<TaskAndTaskIdRequest, TaskIdResponse>(
                CreateFaultyResponse<TaskIdResponse>(
                    task_manager::kNotPresentId))));

    auto result = task_service_model_controller->AddSubtask(id, task);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kNotPresentId);
}

TEST_F(TaskServiceModelControllerTest, MustEditTask)
{
    using testing::_;
    auto task = task_factory.GetNextTask();
    auto id = CreateTaskId(0);
    TaskAndTaskIdRequest request;
    request.set_allocated_task_id(new TaskId(id));
    request.set_allocated_task(new Task(task));

    EXPECT_CALL(*mock_task_service_stub, Edit(_, request, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<TaskAndTaskIdRequest, PlainResponse>(
                CreatePlainResponseWithOk())));

    auto result = task_service_model_controller->Edit(id, task);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kOk);
}

TEST_F(TaskServiceModelControllerTest, FaultyEditTask)
{
    using testing::_;
    auto task = task_factory.GetNextTask();
    auto id = CreateTaskId(0);
    TaskAndTaskIdRequest request;
    request.set_allocated_task_id(new TaskId(id));
    request.set_allocated_task(new Task(task));

    EXPECT_CALL(*mock_task_service_stub, Edit(_, request, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<TaskAndTaskIdRequest, PlainResponse>(
                CreateFaultyResponse<PlainResponse>(
                    task_manager::kNotPresentId))));

    auto result = task_service_model_controller->Edit(id, task);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kNotPresentId);
}

TEST_F(TaskServiceModelControllerTest, MustCompleteTask)
{
    using testing::_;
    auto request = CreateTaskId(0);
    auto id = request;

    EXPECT_CALL(*mock_task_service_stub, Complete(_, request, _))
        .WillOnce(testing::Invoke(InvokeResultFactory<TaskId, PlainResponse>(
            CreatePlainResponseWithOk())));

    auto result = task_service_model_controller->Complete(id);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kOk);
}

TEST_F(TaskServiceModelControllerTest, FaultyCompleteTask)
{
    using testing::_;
    auto request = CreateTaskId(0);
    auto id = request;

    EXPECT_CALL(*mock_task_service_stub, Complete(_, request, _))
        .WillOnce(testing::Invoke(InvokeResultFactory<TaskId, PlainResponse>(
            CreateFaultyResponse<PlainResponse>(task_manager::kNotPresentId))));

    auto result = task_service_model_controller->Complete(id);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kNotPresentId);
}

TEST_F(TaskServiceModelControllerTest, MustDeleteTask)
{
    using testing::_;
    auto request = CreateTaskId(0);
    auto id = request;

    EXPECT_CALL(*mock_task_service_stub, Delete(_, request, _))
        .WillOnce(testing::Invoke(InvokeResultFactory<TaskId, PlainResponse>(
            CreatePlainResponseWithOk())));

    auto result = task_service_model_controller->Delete(id);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kOk);
}

TEST_F(TaskServiceModelControllerTest, FaultyDeleteTask)
{
    using testing::_;
    auto request = CreateTaskId(0);
    auto id = request;

    EXPECT_CALL(*mock_task_service_stub, Delete(_, request, _))
        .WillOnce(testing::Invoke(InvokeResultFactory<TaskId, PlainResponse>(
            CreateFaultyResponse<PlainResponse>(task_manager::kNotPresentId))));

    auto result = task_service_model_controller->Delete(id);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kNotPresentId);
}

TEST_F(TaskServiceModelControllerTest, MustGetAllTasks)
{
    using testing::_;

    SolidTasksResponse response;
    SolidTask solid_task;
    SolidTasks solid_tasks;
    solid_task.set_allocated_task(new Task(task_factory.GetNextTask()));
    solid_task.set_allocated_task_id(new TaskId(CreateTaskId(0)));
    solid_tasks.push_back(solid_task);
    response.add_solid_tasks()->CopyFrom(solid_task);
    solid_task.set_allocated_task(new Task(task_factory.GetNextTask()));
    solid_task.set_allocated_task_id(new TaskId(CreateTaskId(1)));
    response.add_solid_tasks()->CopyFrom(solid_task);
    solid_tasks.push_back(solid_task);
    response.set_status(task_manager::kOk);

    EXPECT_CALL(*mock_task_service_stub, GetAllSolidTasks(_, _, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<google::protobuf::Empty, SolidTasksResponse>(
                response)));

    auto result = task_service_model_controller->GetAllSolidTasks();
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kOk);
    EXPECT_EQ(result.AccessResult()[0], solid_tasks[0]);
    EXPECT_EQ(result.AccessResult()[1], solid_tasks[1]);
}

TEST_F(TaskServiceModelControllerTest, FaultyGetAllTasks)
{
    using testing::_;

    EXPECT_CALL(*mock_task_service_stub, GetAllSolidTasks(_, _, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<google::protobuf::Empty, SolidTasksResponse>(
                CreateFaultyResponse<SolidTasksResponse>(
                    task_manager::kNotPresentId))));

    auto result = task_service_model_controller->GetAllSolidTasks();
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kNotPresentId);
}

TEST_F(TaskServiceModelControllerTest, MustGetSpecifiedTasks)
{
    using testing::_;

    SolidTasksResponse response;
    SolidTask solid_task;
    SolidTasks solid_tasks;
    solid_task.set_allocated_task(new Task(task_factory.GetNextTask()));
    solid_task.set_allocated_task_id(new TaskId(CreateTaskId(0)));
    solid_tasks.push_back(solid_task);
    response.add_solid_tasks()->CopyFrom(solid_task);
    solid_task.set_allocated_task(new Task(task_factory.GetNextTask()));
    solid_task.set_allocated_task_id(new TaskId(CreateTaskId(1)));
    response.add_solid_tasks()->CopyFrom(solid_task);
    solid_tasks.push_back(solid_task);
    response.set_status(task_manager::kOk);

    std::vector<TaskId> task_ids;
    task_ids.push_back(CreateTaskId(0));
    task_ids.push_back(CreateTaskId(1));

    EXPECT_CALL(*mock_task_service_stub, GetSpecifiedSolidTasks(_, _, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<TaskIdsRequest, SolidTasksResponse>(response)));

    auto result = task_service_model_controller->GetSpecificSolidTasks(task_ids);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kOk);
    EXPECT_EQ(result.AccessResult()[0], solid_tasks[0]);
    EXPECT_EQ(result.AccessResult()[1], solid_tasks[1]);
}

TEST_F(TaskServiceModelControllerTest, FaultyGetSpecifiedTasks)
{
    using testing::_;

    std::vector<TaskId> task_ids;
    task_ids.push_back(CreateTaskId(0));
    task_ids.push_back(CreateTaskId(1));

    EXPECT_CALL(*mock_task_service_stub, GetSpecifiedSolidTasks(_, _, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<TaskIdsRequest, SolidTasksResponse>(
                CreateFaultyResponse<SolidTasksResponse>(
                    task_manager::kNotPresentId))));

    auto result = task_service_model_controller->GetSpecificSolidTasks(task_ids);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kNotPresentId);
}

TEST_F(TaskServiceModelControllerTest, MustSave)
{
    using testing::_;
    EXPECT_CALL(*mock_task_service_stub, Save(_, _, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<google::protobuf::Empty, PlainResponse>(
                CreatePlainResponseWithOk())));

    auto result = task_service_model_controller->Save();
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kOk);
}

TEST_F(TaskServiceModelControllerTest, FaultySave)
{
    using testing::_;
    EXPECT_CALL(*mock_task_service_stub, Save(_, _, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<google::protobuf::Empty, PlainResponse>(
                CreateFaultyResponse<PlainResponse>(
                    task_manager::kSaveFailure))));

    auto result = task_service_model_controller->Save();
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kSaveFailure);
}

TEST_F(TaskServiceModelControllerTest, MustLoad)
{
    using testing::_;
    EXPECT_CALL(*mock_task_service_stub, Load(_, _, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<google::protobuf::Empty, PlainResponse>(
                CreatePlainResponseWithOk())));

    auto result = task_service_model_controller->Load();
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kOk);
}

TEST_F(TaskServiceModelControllerTest, FaultyLoad)
{
    using testing::_;
    EXPECT_CALL(*mock_task_service_stub, Load(_, _, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<google::protobuf::Empty, PlainResponse>(
                CreateFaultyResponse<PlainResponse>(
                    task_manager::kLoadFailure))));

    auto result = task_service_model_controller->Load();
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kLoadFailure);
}

TEST_F(TaskServiceModelControllerTest, MustAddLabel)
{
    using testing::_;
    auto id = CreateTaskId(0);
    Label label;
    label.set_name("label");
    TaskIdAndLabelRequest request;
    request.set_allocated_task_id(new TaskId(id));
    request.set_allocated_label(new Label(label));

    EXPECT_CALL(*mock_task_service_stub, AddLabel(_, request, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<TaskIdAndLabelRequest, PlainResponse>(
                CreatePlainResponseWithOk())));

    auto result = task_service_model_controller->AddLabel(id, label);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kOk);
}

TEST_F(TaskServiceModelControllerTest, FaultyAddLabel)
{
    using testing::_;
    auto id = CreateTaskId(0);
    Label label;
    label.set_name("label");
    TaskIdAndLabelRequest request;
    request.set_allocated_task_id(new TaskId(id));
    request.set_allocated_label(new Label(label));

    EXPECT_CALL(*mock_task_service_stub, AddLabel(_, request, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<TaskIdAndLabelRequest, PlainResponse>(
                CreateFaultyResponse<PlainResponse>(task_manager::kNotPresentId))));

    auto result = task_service_model_controller->AddLabel(id, label);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kNotPresentId);
}

TEST_F(TaskServiceModelControllerTest, MustDeleteLabel)
{
    using testing::_;
    auto id = CreateTaskId(0);
    Label label;
    label.set_name("label");
    TaskIdAndLabelRequest request;
    request.set_allocated_task_id(new TaskId(id));
    request.set_allocated_label(new Label(label));

    EXPECT_CALL(*mock_task_service_stub, DeleteLabel(_, request, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<TaskIdAndLabelRequest, PlainResponse>(
                CreatePlainResponseWithOk())));

    auto result = task_service_model_controller->DeleteLabel(id, label);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kOk);
}

TEST_F(TaskServiceModelControllerTest, FaultyDeleteLabel)
{
    using testing::_;
    auto id = CreateTaskId(0);
    Label label;
    label.set_name("label");
    TaskIdAndLabelRequest request;
    request.set_allocated_task_id(new TaskId(id));
    request.set_allocated_label(new Label(label));

    EXPECT_CALL(*mock_task_service_stub, DeleteLabel(_, request, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<TaskIdAndLabelRequest, PlainResponse>(
                CreateFaultyResponse<PlainResponse>(task_manager::kNotPresentId))));

    auto result = task_service_model_controller->DeleteLabel(id, label);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kNotPresentId);
}

TEST_F(TaskServiceModelControllerTest, MustShowByLabel)
{
    using testing::_;

    SolidTasksResponse response;
    SolidTask solid_task;
    SolidTasks solid_tasks;
    solid_task.set_allocated_task(new Task(task_factory.GetNextTask()));
    solid_task.set_allocated_task_id(new TaskId(CreateTaskId(0)));
    solid_task.mutable_task()->add_labels()->set_name("label");
    solid_tasks.push_back(solid_task);
    response.add_solid_tasks()->CopyFrom(solid_task);
    solid_task.set_allocated_task(new Task(task_factory.GetNextTask()));
    solid_task.set_allocated_task_id(new TaskId(CreateTaskId(1)));
    solid_task.mutable_task()->add_labels()->set_name("label");
    response.add_solid_tasks()->CopyFrom(solid_task);
    solid_tasks.push_back(solid_task);
    response.set_status(task_manager::kOk);

    Label label;
    label.set_name("label");

    EXPECT_CALL(*mock_task_service_stub, GetTasksByLabel(_, label, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<Label, SolidTasksResponse>(
                response)));

    auto result = task_service_model_controller->GetTasksByLabel(label);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kOk);
    EXPECT_EQ(result.AccessResult()[0], solid_tasks[0]);
    EXPECT_EQ(result.AccessResult()[1], solid_tasks[1]);
}

TEST_F(TaskServiceModelControllerTest, FaultyShowByLabel)
{
    using testing::_;

    Label label;
    label.set_name("label");

    EXPECT_CALL(*mock_task_service_stub, GetTasksByLabel(_, label, _))
        .WillOnce(testing::Invoke(
            InvokeResultFactory<Label, SolidTasksResponse>(
                CreateFaultyResponse<SolidTasksResponse>(task_manager::kNotPresentLabel))));

    auto result = task_service_model_controller->GetTasksByLabel(label);
    EXPECT_EQ(result.GetStatus(), ModelController::Status::kNotPresentLabel);
}
