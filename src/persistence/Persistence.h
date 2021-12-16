#ifndef TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_
#define TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_

#include <istream>
#include <memory>
#include <ostream>

#include "model/id/TaskId.h"
#include "model/task_wrapper/TaskWrapper.h"

struct Payload {
  TaskId last_given_id;
  TaskStorage task_storage;
};

struct LoadResult {
  enum class Status {
    kOk,
    kFailure,
  };
  Status status;
  std::optional<Payload> payload;
};

struct SaveResult {
  enum class Status {
    kOk,
    kFailure,
  };
  Status status;
};

class Persistence {
 public:
  SaveResult Save(std::ostream&, Payload) const;
  LoadResult Load(std::istream&) const;
};

#endif  // TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_
