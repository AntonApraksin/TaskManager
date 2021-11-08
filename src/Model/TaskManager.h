#ifndef TASK_MANAGER_SRC_MODEL_TASK_MANAGER_H_
#define TASK_MANAGER_SRC_MODEL_TASK_MANAGER_H_
#pragma once

#include <map>
#include <stdexcept>

#include "Task.h"

class TaskManager {
 public:
  void Add(Task task) {
    if (tasks_.empty()) {
      tasks_.insert({0, std::move(task)});
    } else {
      tasks_.insert({tasks_.rbegin()->first + 1, std::move(task)});
    }
  }

  void Edit(int id, Task task) {
    if (auto to_edit = tasks_.find(id); to_edit == tasks_.end()) {
      throw std::runtime_error("Given id is invalid");
    } else {
      to_edit->second = std::move(task);
    }
  }

  void Complete(int id) {
    if (auto to_edit = tasks_.find(id); to_edit == tasks_.end()) {
      throw std::runtime_error("Given id is invalid");
    } else {
      tasks_.erase(to_edit);
    }
  }

  void Delete(int id) {
    if (auto to_edit = tasks_.find(id); to_edit == tasks_.end()) {
      throw std::runtime_error("Given id is invalid");
    } else {
      tasks_.erase(to_edit);
    }
  }

  std::map<int, Task> Show() { return tasks_; }

 private:
  std::map<int, Task> tasks_;
};

#endif  // TASK_MANAGER_SRC_MODEL_TASK_MANAGER_H_
