#include "Persistence.h"

#include <type_traits>

#include "SerializableTask.pb.h"

template <typename T>
std::optional<T> ConsumeOneMessageFromIstream(
    std::istream& is)  // TODO: do not reallocate new string on each read
{
  static_assert(std::is_base_of_v<google::protobuf::MessageLite, T>,
                "T must implement google::protobuf::MessageLite");
  google::protobuf::int64 nbytes;
  is >> nbytes;
  std::string str(nbytes, 0);
  is.read(&str[0], nbytes);
  // TODO: Handle possible error
  T message;
  message.ParseFromString(str);
  return message;
}

void ProduceOneMessageToOstream(std::ostream& os,
                                const google::protobuf::MessageLite& message) {
  google::protobuf::int64 nbytes = message.ByteSizeLong();
  os << nbytes;
  os << message.SerializeAsString();
}

void ProduceTaskWrapperToOstream(std::ostream& os,
                                 const TaskWrapper& task_wrapper,
                                 task_manager::TaskId task_id) {
  task_manager::SerializableTask serializable_task;
  serializable_task.set_allocated_parent_id(new TaskId(task_id));
  for (const auto& i : task_wrapper) {
    serializable_task.set_allocated_task_id(new TaskId(i.first));
    serializable_task.set_allocated_task(new Task(*i.second));
    ProduceOneMessageToOstream(os, serializable_task);
    ProduceTaskWrapperToOstream(os, i.second, i.first);
  }
}

LoadResult Persistence::Load(std::istream& is) const {
  Payload payload;
  payload.last_given_id =
      *ConsumeOneMessageFromIstream<TaskId>(is);  // TODO: Handle error here
  task_manager::SerializableTask serializable_task;
  for (; is.rdbuf()->in_avail() > 0;) {
    serializable_task =
        *ConsumeOneMessageFromIstream<task_manager::SerializableTask>(is);
    if (serializable_task.has_parent_id()) {
      payload.task_storage.Find(serializable_task.parent_id())
          ->second.Add(serializable_task.task_id(), serializable_task.task());
    } else {
      payload.task_storage.Add(serializable_task.task_id(),
                               serializable_task.task());
    }
  }
  return LoadResult{LoadResult::Status::kOk, std::move(payload)};
}

SaveResult Persistence::Save(std::ostream& os, Payload payload) const {
  ProduceOneMessageToOstream(os, payload.last_given_id);
  task_manager::SerializableTask serializable_task;
  for (const auto& i : payload.task_storage) {
    serializable_task.set_allocated_task_id(new task_manager::TaskId(i.first));
    serializable_task.set_allocated_task(new task_manager::Task(*i.second));
    ProduceOneMessageToOstream(os, serializable_task);
    ProduceTaskWrapperToOstream(os, i.second, i.first);
  }
  return SaveResult{SaveResult::Status::kOk};
}