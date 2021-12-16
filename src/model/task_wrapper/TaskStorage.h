#ifndef TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKSTORAGE_H_
#define TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKSTORAGE_H_

#include <map>

#include "model/id/TaskId.h"
#include "model/task/Task.h"

template <typename T>
class _TaskStorageImpl {  // NOLINT
 public:
  using Storage = std::map<TaskId, T>;

 public:
  void Add(TaskId task_id, Task task);
  void Delete(typename Storage::iterator to_delete);

  const Storage& ShowStorage() const;
  typename Storage::const_iterator Find(TaskId) const;
  typename Storage::iterator Find(TaskId);

  std::optional<std::reference_wrapper<const _TaskStorageImpl>>
      FindStorageContaining(TaskId) const;
  std::optional<std::reference_wrapper<_TaskStorageImpl>> FindStorageContaining(
      TaskId);

  typename Storage::iterator begin();
  typename Storage::iterator end();

  typename Storage::const_iterator begin() const;
  typename Storage::const_iterator end() const;

  typename Storage::const_iterator cbegin() const;
  typename Storage::const_iterator cend() const;

 protected:
  Storage children_;
};

template <typename T>
void _TaskStorageImpl<T>::Add(TaskId task_id, Task task) {
  children_.insert(std::make_pair(task_id, std::move(task)));
}

template <typename T>
void _TaskStorageImpl<T>::Delete(
    typename _TaskStorageImpl<T>::Storage::iterator to_delete) {
  children_.erase(to_delete);
}

template <typename T>
typename _TaskStorageImpl<T>::Storage::const_iterator _TaskStorageImpl<T>::Find(
    TaskId task_id) const {
  auto parent = FindStorageContaining(task_id);
  if (parent) {
    return parent->get().children_.find(task_id);
  }
  return cend();
}

template <typename T>
typename _TaskStorageImpl<T>::Storage::iterator _TaskStorageImpl<T>::Find(
    TaskId task_id) {
  auto parent = FindStorageContaining(task_id);
  if (parent) {
    return parent->get().children_.find(task_id);
  }
  return end();
}

template <typename T>
std::optional<std::reference_wrapper<_TaskStorageImpl<T>>>
_TaskStorageImpl<T>::FindStorageContaining(TaskId task_id) {
  if (auto it = children_.find(task_id); it != children_.end()) {
    return std::ref(*this);
  }
  for (auto& i : children_) {
    if (auto it = i.second.FindStorageContaining(task_id); it) {
      return it;
    }
  }
  return {};
}

template <typename T>
std::optional<std::reference_wrapper<const _TaskStorageImpl<T>>>
_TaskStorageImpl<T>::FindStorageContaining(TaskId task_id) const {
  if (auto it = children_.find(task_id); it != children_.end()) {
    return std::cref(*this);
  }
  for (auto& i : children_) {
    if (auto it = i.second.FindStorageContaining(task_id); it) {
      return it;
    }
  }
  return {};
}

template <typename T>
const typename _TaskStorageImpl<T>::Storage& _TaskStorageImpl<T>::ShowStorage()
    const {
  return children_;
}

template <typename T>
typename _TaskStorageImpl<T>::Storage::iterator _TaskStorageImpl<T>::begin() {
  return children_.begin();
}

template <typename T>
typename _TaskStorageImpl<T>::Storage::iterator _TaskStorageImpl<T>::end() {
  return children_.end();
}

template <typename T>
typename _TaskStorageImpl<T>::Storage::const_iterator
_TaskStorageImpl<T>::begin() const {
  return children_.cbegin();
}

template <typename T>
typename _TaskStorageImpl<T>::Storage::const_iterator _TaskStorageImpl<T>::end()
    const {
  return children_.cend();
}

template <typename T>
typename _TaskStorageImpl<T>::Storage::const_iterator
_TaskStorageImpl<T>::cbegin() const {
  return children_.cbegin();
}

template <typename T>
typename _TaskStorageImpl<T>::Storage::const_iterator
_TaskStorageImpl<T>::cend() const {
  return children_.cend();
}

#endif  // TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKSTORAGE_H_