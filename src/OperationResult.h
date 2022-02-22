#ifndef TASKMANAGER_SRC_MODEL_TASK_MANAGER_OPERATIONRESULT_H_
#define TASKMANAGER_SRC_MODEL_TASK_MANAGER_OPERATIONRESULT_H_

#include <optional>
#include <type_traits>

template<typename StatusEnum, typename Result = std::nullopt_t>
class OperationResult
{
    static_assert(std::is_enum_v<StatusEnum>,
                  "StatusEnum must be an enumeration type");
    static_assert(
        static_cast<std::underlying_type_t<StatusEnum>>(StatusEnum::kOk) == 0,
        "StatusEnum must contain kOk status with value equals to 0");

public:
    StatusEnum GetStatus() { return status_; }

    const Result& AccessResult() { return *result_; }

    explicit operator bool() { return status_ == StatusEnum::kOk; }

    static OperationResult Ok(Result result)
    {
        OperationResult to_return;
        to_return.result_ = std::move(result);
        to_return.status_ = StatusEnum::kOk;
        return to_return;
    }

    static OperationResult Error(StatusEnum status)
    {
        OperationResult to_return;
        to_return.status_ = status;
        return to_return;
    }

private:
    std::optional<Result> result_;
    StatusEnum status_;
};

template<typename StatusEnum>
class OperationResult<StatusEnum, std::nullopt_t>
{
    static_assert(std::is_enum_v<StatusEnum>,
                  "StatusEnum must be an enumeration type");
    static_assert(
        static_cast<std::underlying_type_t<StatusEnum>>(StatusEnum::kOk) == 0,
        "StatusEnum must contain kOk status with value equals to 0");

public:
    StatusEnum GetStatus() { return status_; }

    explicit operator bool() { return status_ == StatusEnum::kOk; }

    static OperationResult Ok()
    {
        OperationResult to_return;
        to_return.status_ = StatusEnum::kOk;
        return to_return;
    }

    static OperationResult Error(StatusEnum status)
    {
        OperationResult to_return;
        to_return.status_ = status;
        return to_return;
    }

private:
    StatusEnum status_;
};

#endif  // TASKMANAGER_SRC_MODEL_TASK_MANAGER_OPERATIONRESULT_H_
