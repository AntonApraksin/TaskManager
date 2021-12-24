#include "Persistence.h"

#include <type_traits>

#include "model/SolidTask.h"

namespace task_manager {
template <typename T>
OperationResult<Persistence::Status, T> ConsumeOneMessageFromIstream(
    std::istream& is) {
  static_assert(std::is_base_of_v<google::protobuf::MessageLite, T>,
                "T must implement google::protobuf::MessageLite");
  google::protobuf::int64 nbytes;
  is >> nbytes;
  if (is.fail()) {
    return OperationResult<Persistence::Status, T>::Error(
        Persistence::Status::kFailure);
  }
  std::string str(nbytes, 0);
  try {
    is.read(&str[0], nbytes);
  } catch (const std::ios_base::failure& e) {
    return OperationResult<Persistence::Status, T>::Error(
        Persistence::Status::kFailure);
  }
  T message;
  if (!message.ParseFromString(str)) {
    return OperationResult<Persistence::Status, T>::Error(
        Persistence::Status::kFailure);
  }
  return OperationResult<Persistence::Status, T>::Ok(std::move(message));
}

OperationResult<Persistence::Status> ProduceOneMessageToOstream(
    std::ostream& os, const google::protobuf::MessageLite& message) {
  google::protobuf::int64 nbytes =
      message.ByteSizeLong(); // NOLINT: Narrowing conversion?
  os << nbytes;
  if (os.fail()) {
    return OperationResult<Persistence::Status>::Error(
        Persistence::Status::kFailure);
  }
  os << message.SerializeAsString();
  if (os.fail()) {
    return OperationResult<Persistence::Status>::Error(
        Persistence::Status::kFailure);
  }
  return OperationResult<Persistence::Status>::Ok();
}

OperationResult<Persistence::Status, SolidTasks> Persistence::Load(
    std::istream& is) const {
  SolidTasks solid_tasks;
  for (; is.peek() != EOF;) {
    auto value = ConsumeOneMessageFromIstream<SolidTask>(is);
    if (value) {
      solid_tasks.push_back(value.AccessResult());
    } else {
      return OperationResult<Persistence::Status, SolidTasks>::Error(
          value.GetStatus());
    }
  }
  return OperationResult<Persistence::Status, SolidTasks>::Ok(
      std::move(solid_tasks));
}

OperationResult<Persistence::Status> Persistence::Save(
    std::ostream& os, SolidTasks solid_tasks) const {
  for (const auto& i : solid_tasks) {
    auto result = ProduceOneMessageToOstream(os, i);
    if (!result) {
      return OperationResult<Persistence::Status>::Error(result.GetStatus());
    }
  }
  return OperationResult<Persistence::Status>::Ok();
}
}  // namespace task_manager
