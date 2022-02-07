#include "TaskServiceModelController.h"

namespace task_manager {
using MCStatus = ModelController::Status;

MCStatus ConvertTaskServiceStatusToModelControllerStatus(
    TaskServiceStatus status) {
    switch (status) {
        case TaskServiceStatus::kOk: { return MCStatus::kOk; }
        case TaskServiceStatus::kSaveFailure: { return MCStatus::kSaveFailure; }
        case TaskServiceStatus::kLoadFailure: { return MCStatus::kLoadFailure; }
        case TaskServiceStatus::kNotPresentId: { return MCStatus::kNotPresentId; }
        case TaskServiceStatus::kNotPresentLabel: { return MCStatus::kNotPresentLabel; }
        case TaskServiceStatus::TaskServiceStatus_INT_MAX_SENTINEL_DO_NOT_USE_: { assert(false); }
        case TaskServiceStatus::TaskServiceStatus_INT_MIN_SENTINEL_DO_NOT_USE_: { assert(false); }
    }
}

TaskServiceModelController::TaskServiceModelController(std::unique_ptr<task_manager::TaskService::StubInterface> stub)
    : stub_(std::move(stub)) {}

OperationResult<MCStatus, TaskId> task_manager::TaskServiceModelController::Add(task_manager::Task task) {
    grpc::ClientContext ctx;
    TaskIdResponse response;
    auto status = stub_->AddTask(&ctx, task, &response);
    if (response.status() == TaskServiceStatus::kOk) {
        return OperationResult<MCStatus, TaskId>::Ok(response.task_id());
    }
    return OperationResult<MCStatus, TaskId>::Error(
        ConvertTaskServiceStatusToModelControllerStatus(response.status()));
}

OperationResult<MCStatus, TaskId> task_manager::TaskServiceModelController::Add(task_manager::TaskId task_id,
                                                                                task_manager::Task task) {
    grpc::ClientContext ctx;
    TaskIdResponse response;
    TaskAndTaskIdRequest request;
    request.set_allocated_task_id(new TaskId(task_id));
    request.set_allocated_task(new Task(task));
    stub_->AddSubtask(&ctx, request, &response);
    if (response.status() == TaskServiceStatus::kOk) {
        return OperationResult<MCStatus, TaskId>::Ok(response.task_id());
    }
    return OperationResult<MCStatus, TaskId>::Error(
        ConvertTaskServiceStatusToModelControllerStatus(response.status()));
}

OperationResult<MCStatus> task_manager::TaskServiceModelController::Edit(task_manager::TaskId task_id,
                                                                         task_manager::Task task) {
    grpc::ClientContext ctx;
    PlainResponse response;
    TaskAndTaskIdRequest request;
    request.set_allocated_task_id(new TaskId(task_id));
    request.set_allocated_task(new Task(task));
    stub_->Edit(&ctx, request, &response);
    if (response.status() == TaskServiceStatus::kOk) {
        return OperationResult<MCStatus>::Ok();
    }
    return OperationResult<MCStatus>::Error(
        ConvertTaskServiceStatusToModelControllerStatus(response.status()));
}

OperationResult<MCStatus> task_manager::TaskServiceModelController::Complete(task_manager::TaskId task_id) {
    grpc::ClientContext ctx;
    PlainResponse response;
    stub_->Complete(&ctx, task_id, &response);
    if (response.status() == TaskServiceStatus::kOk) {
        return OperationResult<MCStatus>::Ok();
    }
    return OperationResult<MCStatus>::Error(
        ConvertTaskServiceStatusToModelControllerStatus(response.status()));
}

OperationResult<MCStatus> task_manager::TaskServiceModelController::Delete(task_manager::TaskId task_id) {
    grpc::ClientContext ctx;
    PlainResponse response;
    stub_->Delete(&ctx, task_id, &response);
    if (response.status() == TaskServiceStatus::kOk) {
        return OperationResult<MCStatus>::Ok();
    }
    return OperationResult<MCStatus>::Error(
        ConvertTaskServiceStatusToModelControllerStatus(response.status()));
}

OperationResult<MCStatus, SolidTasks>
task_manager::TaskServiceModelController::GetAllSolidTasks() {
    grpc::ClientContext ctx;
    SolidTasksResponse response;
    google::protobuf::Empty request;
    stub_->GetAllSolidTasks(&ctx, request, &response);
    if (response.status() == TaskServiceStatus::kOk) {
        return OperationResult<MCStatus, SolidTasks>::Ok(
            {response.solid_tasks().cbegin(), response.solid_tasks().cend()});
    }
    return OperationResult<MCStatus, SolidTasks>::Error(
        ConvertTaskServiceStatusToModelControllerStatus(response.status()));
}

OperationResult<MCStatus, SolidTasks>
task_manager::TaskServiceModelController::GetSpecificSolidTasks(
    std::vector<TaskId> task_ids) {
    grpc::ClientContext ctx;
    SolidTasksResponse response;
    TaskIdsRequest request;
    for (const auto &i: task_ids) {
        request.add_task_ids()->CopyFrom(i);
    }
    stub_->GetSpecifiedSolidTasks(&ctx, request, &response);
    if (response.status() == TaskServiceStatus::kOk) {
        return OperationResult<MCStatus, SolidTasks>::Ok(
            {response.solid_tasks().cbegin(), response.solid_tasks().cend()});
    }
    return OperationResult<MCStatus, SolidTasks>::Error(
        ConvertTaskServiceStatusToModelControllerStatus(response.status()));
}

OperationResult<MCStatus> task_manager::TaskServiceModelController::Load() {
    grpc::ClientContext ctx;
    PlainResponse response;
    google::protobuf::Empty request;
    stub_->Load(&ctx, request, &response);
    if (response.status() == TaskServiceStatus::kOk) {
        return OperationResult<MCStatus>::Ok();
    }
    return OperationResult<MCStatus>::Error(
        ConvertTaskServiceStatusToModelControllerStatus(response.status()));
}

OperationResult<MCStatus> task_manager::TaskServiceModelController::Save() {
    grpc::ClientContext ctx;
    PlainResponse response;
    google::protobuf::Empty request;
    stub_->Save(&ctx, request, &response);
    if (response.status() == TaskServiceStatus::kOk) {
        return OperationResult<MCStatus>::Ok();
    }
    return OperationResult<MCStatus>::Error(
        ConvertTaskServiceStatusToModelControllerStatus(response.status()));
}

}  // namespace task_manager
