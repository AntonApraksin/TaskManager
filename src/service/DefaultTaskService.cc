#include "DefaultTaskService.h"

#include <google/protobuf/util/time_util.h>

#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

#include "logging/DefaultLogFacility.h"
#include "model/ModelController.h"

namespace task_manager
{

TaskServiceStatus ConvertModelControllerStatusToTaskServiceStatus(ModelController::Status status)
{
    switch (status)
    {
        case ModelController::Status::kNotPresentId:
        {
            return TaskServiceStatus::kNotPresentId;
        }
        case ModelController::Status::kOk:
        {
            return TaskServiceStatus::kOk;
        }
        case ModelController::Status::kLoadFailure:
        {
            return TaskServiceStatus::kLoadFailure;
        }
        case ModelController::Status::kSaveFailure:
        {
            return TaskServiceStatus::kSaveFailure;
        }
        case ModelController::Status::kNotPresentLabel:
        {
            return TaskServiceStatus::kNotPresentLabel;
        }
    }
}

DefaultTaskService::DefaultTaskService(std::unique_ptr<ModelController> model_controller)
    : model_controller_(std::move(model_controller)) {}

grpc::Status DefaultTaskService::AddTask(::grpc::ServerContext*, const Task* request,
                                         TaskIdResponse* response)
{
    BOOST_LOG_NAMED_SCOPE("DefaultTaskService::AddTask");
    auto& logger = logging::GetDefaultLogger();

    BOOST_LOG_SEV(logger, logging::severity::debug)
        << "request: " << request->DebugString();

    auto result = model_controller_->AddTask(*request);
    if (result)
    {
        response->set_allocated_task_id(new TaskId(result.AccessResult()));
    }
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));

    BOOST_LOG_SEV(logger, logging::severity::debug) << response->DebugString();

    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::AddSubtask(::grpc::ServerContext*,
                                            const TaskAndTaskIdRequest* request,
                                            TaskIdResponse* response)
{
    auto result = model_controller_->AddSubtask(request->task_id(), request->task());
    if (result)
    {
        response->set_allocated_task_id(new TaskId(result.AccessResult()));
    }
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Edit(::grpc::ServerContext*,
                                      const TaskAndTaskIdRequest* request,
                                      PlainResponse* response)
{
    auto result = model_controller_->Edit(request->task_id(), request->task());
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Complete(::grpc::ServerContext*, const TaskId* request,
                                          PlainResponse* response)
{
    auto result = model_controller_->Complete(*request);
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Delete(::grpc::ServerContext*, const TaskId* request,
                                        PlainResponse* response)
{
    auto result = model_controller_->Delete(*request);
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::GetAllSolidTasks(::grpc::ServerContext*, const ::google::protobuf::Empty*,
                                                  SolidTasksResponse* response)
{
    auto result = model_controller_->GetAllSolidTasks();
    if (result)
    {
        for (const auto& i : result.AccessResult())
        {
            response->add_solid_tasks()->CopyFrom(i);
        }
    }
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::GetSpecifiedSolidTasks(::grpc::ServerContext*, const TaskIdsRequest* request,
                                                        SolidTasksResponse* response)
{
    BOOST_LOG_NAMED_SCOPE("DefaultTaskService::GetSpecifiedSolidTasks");
    auto& logger = logging::GetDefaultLogger();

    BOOST_LOG_SEV(logger, logging::severity::debug)
        << "request: " << request->DebugString();

    std::vector<TaskId> ids;
    std::copy(request->task_ids().cbegin(), request->task_ids().cend(),
              std::back_inserter(ids));
    auto result = model_controller_->GetSpecificSolidTasks(std::move(ids));
    if (result)
    {
        for (const auto& i : result.AccessResult())
        {
            response->add_solid_tasks()->CopyFrom(i);
        }
    }
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));

    BOOST_LOG_SEV(logger, logging::severity::debug)
        << "response: " << TaskServiceStatus_Name(response->status());
    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Load(::grpc::ServerContext*,
                                      const ::google::protobuf::Empty*,
                                      PlainResponse* response)
{
    auto result = model_controller_->Load();
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}

grpc::Status DefaultTaskService::Save(::grpc::ServerContext*,
                                      const ::google::protobuf::Empty*,
                                      PlainResponse* response)
{
    auto result = model_controller_->Save();
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}
grpc::Status DefaultTaskService::AddLabel(::grpc::ServerContext*,
                                          const TaskIdAndLabelRequest* request,
                                          PlainResponse* response)
{
    auto result =
        model_controller_->AddLabel(request->task_id(), request->label());
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}
grpc::Status DefaultTaskService::DeleteLabel(::grpc::ServerContext*,
                                             const TaskIdAndLabelRequest* request,
                                             PlainResponse* response)
{
    auto result =
        model_controller_->DeleteLabel(request->task_id(), request->label());
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}
grpc::Status DefaultTaskService::GetTasksByLabel(::grpc::ServerContext*,
                                                 const Label* request,
                                                 SolidTasksResponse* response)
{
    auto result = model_controller_->GetTasksByLabel(*request);
    if (result)
    {
        for (const auto& i : result.AccessResult())
        {
            response->add_solid_tasks()->CopyFrom(i);
        }
    }
    response->set_status(
        ConvertModelControllerStatusToTaskServiceStatus(result.GetStatus()));
    return grpc::Status::OK;
}
}  // namespace task_manager
