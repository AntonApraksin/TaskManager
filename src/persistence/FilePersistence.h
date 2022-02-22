#ifndef TASKMANAGER_SRC_PERSISTENCE_FILEPERSISTENCE_H_
#define TASKMANAGER_SRC_PERSISTENCE_FILEPERSISTENCE_H_

#include "persistence/Persistence.h"

namespace task_manager
{
class FilePersistence : public Persistence
{
public:
    explicit FilePersistence(std::string filename);

    OperationResult<Status> Save(SolidTasks) const override;
    OperationResult<Status, SolidTasks> Load() const override;

private:
    std::string filename_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_PERSISTENCE_FILEPERSISTENCE_H_
