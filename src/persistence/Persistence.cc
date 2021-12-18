#include "Persistence.h"

#include <type_traits>

#include "model/SolidTask.h"

template <typename T>
std::optional<T> ConsumeOneMessageFromIstream(std::istream& is) {
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
  google::protobuf::int64 nbytes =
      message.ByteSizeLong();  // NOLINT: Narrowing conversion?
  os << nbytes;
  os << message.SerializeAsString();
}

LoadResult Persistence::Load(std::istream& is) const {
  SolidTasks solid_tasks;
  for (; is.rdbuf()->in_avail() > 0;) {
    solid_tasks.push_back(
        *ConsumeOneMessageFromIstream<SolidTask>(is));  // TODO: Handle error
  }
  return LoadResult{LoadResult::Status::kOk, std::move(solid_tasks)};
}

SaveResult Persistence::Save(std::ostream& os, SolidTasks solid_tasks) const {
  for (const auto& i : solid_tasks) {
    ProduceOneMessageToOstream(os, i);
  }
  return SaveResult{SaveResult::Status::kOk};
}