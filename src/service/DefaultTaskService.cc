#include "DefaultTaskService.h"

#include <google/protobuf/util/time_util.h>

#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>

#include "logging/DefaultLogFacility.h"
#include "model/ModelController.h"

namespace task_manager {

TaskServiceStatus ConvertModelControllerStatusToTaskServiceStatus(
    ModelController::Status status) {
    switch (status) {
        case ModelController::Status::kNotPresentId:return TaskServiceStatus::kNotPresentId;
        case ModelController::Status::kOk:return TaskServiceStatus::kOk;
        case ModelController::Status::kLoadFailure:return TaskServiceStatus::kLoadFailure;
        case ModelController::Status::kSaveFailure:return TaskServiceStatus::kSaveFailure;
        case ModelController::Status::kNotPresentLabel:return TaskServiceStatus::kNotPresentLabel;
    }
}

DefaultTaskService::DefaultTaskService(
    std::unique_ptr<ModelController> model_controller)
    : model_controller_(std::move(model_controller)) {}

grpc::Status DefaultTaskService::AddTask(
    ::grpc::ServerContext *, const ::task_manager::Task *request,
    ::task_manager::TaskIdResponse *response) {
  BOOST_LOG_NAMED_SCOPE("DefaultTaskService::AddTask");
  auto &logger = logging::GetDefaultLogger();

  BOOST_LOG_SEV(logger, logging::severinity::debug)
      << "request: "
      << "TITLE:'" << request->title() << "' DUEDATE:" << request->due_date()
      << " PRIORITY:" << Task_Priority_Name(request->priority())
      << " PROGRESS:" << Task_Progress_Name(request->progress());

  auto result = model_controller_->Add(*request);
  if (result) {
    response->set_allocated_task_id(new TaskId(result.AccessResult()));
  }
  response->set_status(
      ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));

  if (response->has_task_id()) {
    BOOST_LOG_SEV(logger, logging::severinity::debug)
        << "response: "
        << "STATUS:" << TaskServiceStatus_Name(response->status())
        << " TASKID:" << response->task_id().id();
  } else {
    BOOST_LOG_SEV(logger, logging::severinity::debug)
        << "response: "
        << "STATUS:" << TaskServiceStatus_Name(response->status());
  }
  return grpc::Status::OK;
}

grpc::Status DefaultTaskService::AddSubtask(::grpc::ServerContext *,
                                            const ::task_manager::TaskAndTaskIdRequest *request,
                                            ::task_manager::TaskIdResponse *response) {
    auto result = model_controller_->Add(request->task_id(), request->task());
    if (result) {
        response->set_allocated_task_id(new TaskId(result.AccessResult()));
    }
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Edit(::grpc::ServerContext *,
                                      const ::task_manager::TaskAndTaskIdRequest *request,
                                      ::task_manager::PlainResponse *response) {
    auto result = model_controller_->Edit(request->task_id(), request->task());
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Complete(::grpc::ServerContext *,
                                          const ::task_manager::TaskId *request,
                                          ::task_manager::PlainResponse *response) {
    auto result = model_controller_->Complete(*request);
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Delete(::grpc::ServerContext *,
                                        const ::task_manager::TaskId *request,
                                        ::task_manager::PlainResponse *response) {
    auto result = model_controller_->Delete(*request);
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::GetAllSolidTasks(::grpc::ServerContext *,
                                                  const ::google::protobuf::Empty *,
                                                  ::task_manager::SolidTasksResponse *response) {
    auto result = model_controller_->GetAllSolidTasks();
    if (result) {
        for (const auto &i: result.AccessResult()) {
            response->add_solid_tasks()->CopyFrom(i);
        }
    }
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::GetSpecifiedSolidTasks(::grpc::ServerContext *,
                                                        const TaskIdsRequest *request,
                                                        ::task_manager::SolidTasksResponse *response) {
    std::vector<TaskId> ids;
    std::copy(request->task_ids().cbegin(), request->task_ids().cend(),
              std::back_inserter(ids));
    auto result = model_controller_->GetSpecificSolidTasks(std::move(ids));
    if (result) {
        for (const auto &i: result.AccessResult()) {
            response->add_solid_tasks()->CopyFrom(i);
        }
    }
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Load(::grpc::ServerContext *,
                                      const ::google::protobuf::Empty *,
                                      ::task_manager::PlainResponse *response) {
    auto result = model_controller_->Load();
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Save(::grpc::ServerContext *,
                                      const ::google::protobuf::Empty *,
                                      ::task_manager::PlainResponse *response) {
    auto result = model_controller_->Save();
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}
grpc::Status DefaultTaskService::AddLabel(::grpc::ServerContext *,
                                          const ::task_manager::TaskIdAndLabelRequest *request,
                                          ::task_manager::PlainResponse *response) {
    auto result =
        model_controller_->AddLabel(request->task_id(), request->label());
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}
grpc::Status DefaultTaskService::DeleteLabel(::grpc::ServerContext *,
                                             const ::task_manager::TaskIdAndLabelRequest *request,
                                             ::task_manager::PlainResponse *response) {
    auto result =
        model_controller_->DeleteLabel(request->task_id(), request->label());
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}
}  // namespace task_manager
