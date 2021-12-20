#ifndef TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_
#define TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_

#include <istream>
#include <memory>
#include <ostream>

#include "OperationResult.h"
#include "model/SolidTask.h"

class Persistence {
 public:
  enum class Status {
    kOk,
    kFailure,
  };

 public:
  OperationResult<Status> Save(std::ostream&, SolidTasks) const;
  OperationResult<Status, SolidTasks> Load(std::istream&) const;
};

#endif  // TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_
