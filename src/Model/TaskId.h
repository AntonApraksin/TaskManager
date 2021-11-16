#ifndef TASK_MANAGER_INCLUDE_MODEL_TASK_ID_H_
#define TASK_MANAGER_INCLUDE_MODEL_TASK_ID_H_
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

#endif  // TASK_MANAGER_INCLUDE_MODEL_TASK_ID_H_
