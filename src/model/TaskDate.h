#ifndef TASKMANAGER_SRC_MODEL_TASKDATE_H_
#define TASKMANAGER_SRC_MODEL_TASKDATE_H_

#include <ctime>
#include <string>

using Date_t = std::chrono::time_point<std::chrono::system_clock>;

Date_t parse_date(std::string date);

#endif  // TASKMANAGER_SRC_MODEL_TASKDATE_H_
