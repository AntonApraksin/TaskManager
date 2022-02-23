#ifndef TASKMANAGER_SRC_MODEL_SERVER_SERVER_H_
#define TASKMANAGER_SRC_MODEL_SERVER_SERVER_H_

#include <grpcpp/grpcpp.h>

#include <memory>

#include "TaskService.grpc.pb.h"

namespace task_manager
{
class ModelController;

class DefaultTaskService final : public TaskService::Service
{
public:
    explicit DefaultTaskService(
        std::unique_ptr<ModelController> model_controller);

    grpc::Status AddTask(::grpc::ServerContext* context,
                         const Task* request,
                         TaskIdResponse* response) override;

    grpc::Status AddSubtask(::grpc::ServerContext* context,
                            const TaskAndTaskIdRequest* request,
                            TaskIdResponse* response) override;

    grpc::Status Edit(::grpc::ServerContext* context,
                      const TaskAndTaskIdRequest* request,
                      PlainResponse* response) override;

    grpc::Status Complete(::grpc::ServerContext* context,
                          const TaskId* request,
                          PlainResponse* response) override;

    grpc::Status Delete(::grpc::ServerContext* context,
                        const TaskId* request,
                        PlainResponse* response) override;

    grpc::Status GetAllSolidTasks(::grpc::ServerContext* context,
                                  const ::google::protobuf::Empty* request,
                                  SolidTasksResponse* response) override;

    grpc::Status GetSpecifiedSolidTasks(::grpc::ServerContext* context,
                                        const TaskIdsRequest* request,
                                        SolidTasksResponse* response) override;

    grpc::Status Load(::grpc::ServerContext* context,
                      const ::google::protobuf::Empty* request,
                      PlainResponse* response) override;

    grpc::Status Save(::grpc::ServerContext* context,
                      const ::google::protobuf::Empty* request,
                      PlainResponse* response) override;

    grpc::Status AddLabel(::grpc::ServerContext* context,
                          const TaskIdAndLabelRequest* request,
                          PlainResponse* response) override;

    grpc::Status DeleteLabel(::grpc::ServerContext* context,
                             const TaskIdAndLabelRequest* request,
                             PlainResponse* response) override;

    grpc::Status GetTasksByLabel(::grpc::ServerContext* context,
                                 const Label* request,
                                 SolidTasksResponse* response) override;

private:
    std::unique_ptr<ModelController> model_controller_;
};

}  // namespace task_manager

#endif  // TASKMANAGER_SRC_MODEL_SERVER_SERVER_H_
