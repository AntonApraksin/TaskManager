#include "model/TaskId.h"

int TaskId::GetId() const { return id_; }

TaskId TaskId::Create(int id) { return TaskId{id}; }

TaskId::TaskId(int id) : id_(id) {}

bool operator<(const TaskId& lhs, const TaskId& rhs) {
  return lhs.GetId() < rhs.GetId();
}

bool operator==(const TaskId& lhs, const TaskId& rhs) {
  return lhs.GetId() == rhs.GetId();
}
