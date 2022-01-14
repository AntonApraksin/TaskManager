#ifndef TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_
#define TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_

#include <istream>
#include <memory>
#include <ostream>

#include "OperationResult.h"
#include "model/SolidTask.h"

namespace task_manager {
class Persistence {
 public:
  enum class Status {
    kOk,
    kFailureReading,
    kFailureWriting,
    kFailureDuringEstablishing,
  };

 public:
  virtual OperationResult<Status> Save(SolidTasks to_save) const = 0;
  virtual OperationResult<Status, SolidTasks> Load() const = 0;

  virtual ~Persistence() {}
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_
