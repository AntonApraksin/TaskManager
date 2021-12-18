#ifndef TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_
#define TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_

#include <istream>
#include <memory>
#include <ostream>

#include "model/SolidTask.h"

struct LoadResult {
  enum class Status {
    kOk,
    kFailure,
  };
  Status status;
  std::optional<SolidTasks> solid_tasks;
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
  SaveResult Save(std::ostream&, SolidTasks) const;
  LoadResult Load(std::istream&) const;
};

#endif  // TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_
