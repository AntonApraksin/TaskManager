#ifndef TASKMANAGER_TESTS_TEST_UTILS_TASKSTRINGEDDATA_H_
#define TASKMANAGER_TESTS_TEST_UTILS_TASKSTRINGEDDATA_H_

#include <google/protobuf/util/time_util.h>

#include <iomanip>

#include "interactor/io_facility/Strings.h"
#include "interactor/validator/DateFormat.h"
#include "test_utils/common.h"

struct TaskStringedData
{
    std::string title;
    std::string date;
    std::string priority;
    std::string state;
};

class TaskStringedDataProducer final
{
public:
    TaskStringedData GetData()
    {
        std::stringstream ss;
        ss << "Sample task #" << state_;
        TaskDate_t due_date =
            google::protobuf::util::TimeUtil::TimeTToTimestamp(std::time(nullptr));
        auto time = google::protobuf::util::TimeUtil::TimestampToTimeT(due_date);
        Task::Priority priority = static_cast<Task::Priority>(state_ % 3);
        Task::Progress progress = Task::kUncompleted;
        ++state_;
        std::stringstream ss_date;
        ss_date << std::put_time(std::localtime(&time), kDatePattern);
        return {ss.str(), ss_date.str(), Strings::to_string(priority),
                Strings::to_string(progress)};
    }

private:
    int state_ = 0;
};

#endif  // TASKMANAGER_TESTS_TEST_UTILS_TASKSTRINGEDDATA_H_
