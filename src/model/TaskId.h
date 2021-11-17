#ifndef TASKMANAGER_SRC_MODEL_TASKID_H_
#define TASKMANAGER_SRC_MODEL_TASKID_H_
#pragma once

class TaskId final {
 public:
  int GetId() const;

  static TaskId Create(int id);

 private:
  explicit TaskId(int id);
  const int id_;
};

bool operator<(const TaskId&, const TaskId&);

bool operator==(const TaskId&, const TaskId&);

#endif  // TASKMANAGER_SRC_MODEL_TASKID_H_
