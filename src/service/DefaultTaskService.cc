#include "DefaultTaskService.h"

#include "model/ModelController.h"

namespace task_manager {

TaskServiceStatus ModelControllerStatusToTaskServiceStatus(
    ModelController::Status status) {
  switch (status) {
    case ModelController::Status::kNotPresentId:
      return TaskServiceStatus::kNotPresentId;
    case ModelController::Status::kOk:
      return TaskServiceStatus::kOk;
    case ModelController::Status::kLoadFailure:
      return TaskServiceStatus::kLoadFailure;
    case ModelController::Status::kSaveFailure:
      return TaskServiceStatus::kSaveFailure;
  }
}

DefaultTaskService::DefaultTaskService(
    std::unique_ptr<ModelController> model_controller)
    : model_controller_(std::move(model_controller)) {}

grpc::Status DefaultTaskService::AddTask(
    ::grpc::ServerContext *, const ::task_manager::Task *request,
    ::task_manager::TaskIdResponse *response) {
  auto result = model_controller_->Add(*request);
  if (result) {
    response->set_allocated_task_id(new TaskId(result.AccessResult()));
  }
  response->set_status(
      ModelControllerStatusToTaskServiceStatus(result.GetStatus()));
  return grpc::Status::OK;
}

grpc::Status DefaultTaskService::AddSubtask(
    ::grpc::ServerContext *,
    const ::task_manager::TaskAndTaskIdRequest *request,
    ::task_manager::TaskIdResponse *response) {
  auto result = model_controller_->Add(request->task_id(), request->task());
  if (result) {
    response->set_allocated_task_id(new TaskId(result.AccessResult()));
  }
  response->set_status(
      ModelControllerStatusToTaskServiceStatus(result.GetStatus()));
  return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Edit(
    ::grpc::ServerContext *,
    const ::task_manager::TaskAndTaskIdRequest *request,
    ::task_manager::PlainResponse *response) {
  auto result = model_controller_->Edit(request->task_id(), request->task());
  response->set_status(
      ModelControllerStatusToTaskServiceStatus(result.GetStatus()));
  return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Complete(
    ::grpc::ServerContext *, const ::task_manager::TaskId *request,
    ::task_manager::PlainResponse *response) {
  auto result = model_controller_->Complete(*request);
  response->set_status(
      ModelControllerStatusToTaskServiceStatus(result.GetStatus()));
  return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Delete(
    ::grpc::ServerContext *, const ::task_manager::TaskId *request,
    ::task_manager::PlainResponse *response) {
  auto result = model_controller_->Delete(*request);
  response->set_status(
      ModelControllerStatusToTaskServiceStatus(result.GetStatus()));
  return grpc::Status::OK;
}

grpc::Status DefaultTaskService::GetAllSolidTasks(
    ::grpc::ServerContext *, const ::google::protobuf::Empty *,
    ::task_manager::SolidTasksResponse *response) {
  auto result = model_controller_->GetAllSolidTasks();
  for (const auto &i : result.AccessResult()) {
    response->add_solid_tasks()->CopyFrom(i);
  }
  response->set_status(
      ModelControllerStatusToTaskServiceStatus(result.GetStatus()));
  return grpc::Status::OK;
}

grpc::Status DefaultTaskService::GetSpecifiedSolidTasks(
    ::grpc::ServerContext *, const TaskIdsRequest *request,
    ::task_manager::SolidTasksResponse *response) {
  std::vector<TaskId> ids;
  std::copy(request->task_ids().cbegin(), request->task_ids().cend(),
            std::back_inserter(ids));
  auto result = model_controller_->GetSpecificSolidTasks(std::move(ids));
  for (const auto &i : result.AccessResult()) {
    response->add_solid_tasks()->CopyFrom(i);
  }
  response->set_status(
      ModelControllerStatusToTaskServiceStatus(result.GetStatus()));
  return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Load(::grpc::ServerContext *,
                                      const ::google::protobuf::Empty *,
                                      ::task_manager::PlainResponse *response) {
  auto result = model_controller_->Save();
  response->set_status(
      ModelControllerStatusToTaskServiceStatus(result.GetStatus()));
  return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Save(::grpc::ServerContext *,
                                      const ::google::protobuf::Empty *,
                                      ::task_manager::PlainResponse *response) {
  auto result = model_controller_->Load();
  response->set_status(
      ModelControllerStatusToTaskServiceStatus(result.GetStatus()));
  return grpc::Status::OK;
}
}  // namespace task_manager
