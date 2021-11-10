#ifndef TASK_MANAGER_INCLUDE_MODEL_ID_H_
#define TASK_MANAGER_INCLUDE_MODEL_ID_H_
#pragma once

class TaskId {
 public:
  int GetId() const;

  static TaskId Create(int id);

 private:
  explicit TaskId(int id);
  const int id_;
};

bool operator<(const TaskId&, const TaskId&);

bool operator==(const TaskId&, const TaskId&);

class TaskIdProducer {
 public:
  TaskIdProducer();

  TaskId GetNextId();

 private:
  int current_id_;
};

#endif  // TASK_MANAGER_INCLUDE_MODEL_ID_H_
