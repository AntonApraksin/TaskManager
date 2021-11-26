#include "TaskStorage.h"

template<typename T>
void _TaskStorageImpl<T>::Add(TaskId task_id, Task task) {
  children_.insert({task_id, {task}});
}

template<typename T>
void _TaskStorageImpl<T>::Delete(TaskId task_id) {
  if (auto it = children_.find(task_id); it != children_.end()) {
    children_.erase(it);
    return;
  }
  throw std::runtime_error("Given Id does not exist");
}

template<typename T>
const T& _TaskStorageImpl<T>::Find(TaskId task_id) const {
  _TaskStorageImpl<T>& parent = FindParentOf(task_id);
  return parent.children_.find(task_id).second;
}

template<typename T>
T& _TaskStorageImpl<T>::Find(TaskId task_id) {
  _TaskStorageImpl<T>& parent = FindParentOf(task_id);
  return parent.children_.find(task_id)->second;
}

template<typename T>
_TaskStorageImpl<T> &_TaskStorageImpl<T>::FindParentOf(TaskId task_id) {
  _TaskStorageImpl* parent_of = FindParentOfImpl(task_id);
  if(parent_of == nullptr)
  {
    throw std::runtime_error("Given Id does not exist");
  }
  return *parent_of;
}

template<typename T>
const _TaskStorageImpl<T> &_TaskStorageImpl<T>::FindParentOf(TaskId task_id) const{
  _TaskStorageImpl* parent_of = FindParentOfImpl(task_id);
  if(parent_of == nullptr)
  {
    throw std::runtime_error("Given Id does not exist");
  }
  return *parent_of;
}

template<typename T>
const _TaskStorageImpl<T>* _TaskStorageImpl<T>::FindParentOfImpl(TaskId task_id) const {
  if (auto it = children_.find(task_id); it != children_.end()) {
    return this;
  }
  for (auto& i : children_) {
    if (auto it = i.second.FindParentOfImpl(task_id); it != nullptr) {
      return it;
    }
  }
  return nullptr;
}

template<typename T>
_TaskStorageImpl<T>* _TaskStorageImpl<T>::FindParentOfImpl(TaskId task_id) {
  if (auto it = children_.find(task_id); it != children_.end()) {
    return this;
  }
  for (auto& i : children_) {
    if (auto it = i.second.FindParentOfImpl(task_id); it != nullptr) {
      return it;
    }
  }
  return nullptr;
}

template<typename T>
const typename _TaskStorageImpl<T>::Storage& _TaskStorageImpl<T>::ShowStorage() const { return children_; }