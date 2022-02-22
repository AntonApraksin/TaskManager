#include "FilePersistence.h"

#include <fstream>
#include <type_traits>

#include "model/SolidTask.h"

namespace task_manager
{
template<typename T>
OperationResult<Persistence::Status, T> ConsumeOneMessageFromIstream(
    std::istream& is)
{
    static_assert(std::is_base_of_v<google::protobuf::MessageLite, T>,
                  "T must implement google::protobuf::MessageLite");
    google::protobuf::int64 nbytes;
    is >> nbytes;
    if (is.fail())
    {
        return OperationResult<Persistence::Status, T>::Error(
            Persistence::Status::kFailureReading);
    }
    std::string str(nbytes, 0);
    try
    {
        is.read(&str[0], nbytes);
    } catch (const std::ios_base::failure& e)
    {
        return OperationResult<Persistence::Status, T>::Error(
            Persistence::Status::kFailureReading);
    }
    T message;
    if (!message.ParseFromString(str))
    {
        return OperationResult<Persistence::Status, T>::Error(
            Persistence::Status::kFailureReading);
    }
    return OperationResult<Persistence::Status, T>::Ok(std::move(message));
}

OperationResult<Persistence::Status> ProduceOneMessageToOstream(
    std::ostream& os, const google::protobuf::MessageLite& message)
{
    google::protobuf::int64 nbytes =
        message.ByteSizeLong();  // NOLINT: Narrowing conversion?
    os << nbytes;
    if (os.fail())
    {
        return OperationResult<Persistence::Status>::Error(
            Persistence::Status::kFailureWriting);
    }
    os << message.SerializeAsString();
    if (os.fail())
    {
        return OperationResult<Persistence::Status>::Error(
            Persistence::Status::kFailureWriting);
    }
    return OperationResult<Persistence::Status>::Ok();
}

FilePersistence::FilePersistence(std::string filename)
    : filename_(std::move(filename)) {}

OperationResult<Persistence::Status, SolidTasks> FilePersistence::Load() const
{
    SolidTasks solid_tasks;
    std::ifstream file{filename_};
    if (!file.is_open())
    {
        return OperationResult<Persistence::Status, SolidTasks>::Error(
            Persistence::Status::kFailureDuringEstablishing);
    }
    for (; file.peek() != EOF ;)
    {
        auto value = ConsumeOneMessageFromIstream<SolidTask>(file);
        if (value)
        {
            solid_tasks.push_back(value.AccessResult());
        }
        else
        {
            return OperationResult<Persistence::Status, SolidTasks>::Error(
                value.GetStatus());
        }
    }
    return OperationResult<Persistence::Status, SolidTasks>::Ok(
        std::move(solid_tasks));
}

OperationResult<Persistence::Status> FilePersistence::Save(
    SolidTasks solid_tasks) const
{
    std::ofstream file{filename_};
    if (!file.is_open())
    {
        return OperationResult<Persistence::Status>::Error(
            Persistence::Status::kFailureDuringEstablishing);
    }
    for (const auto& i : solid_tasks)
    {
        auto result = ProduceOneMessageToOstream(file, i);
        if (!result)
        {
            return OperationResult<Persistence::Status>::Error(result.GetStatus());
        }
    }
    return OperationResult<Persistence::Status>::Ok();
}
}  // namespace task_manager
