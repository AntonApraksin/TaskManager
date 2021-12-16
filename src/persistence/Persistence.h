#ifndef TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_
#define TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_

#include "model/task_manager/TaskManager.h"

#include <memory>
#include <ostream>
#include <istream>

struct LoadResult
{
  enum class Status{
    kOk,
    kFailure,
  };
  Status status;
};

struct SaveResult
{
  enum class Status{
    kOk,
    kFailure,
  };
  Status status;
};
};

class Persistence {
 public:
  LoadResult Load(std::istream&, TaskManager&) const;
  SaveResult Save(std::ostream&, const TaskManager&) const;
};

#endif  // TASKMANAGER_SRC_PERSISTENCE_PERSISTENCE_H_
