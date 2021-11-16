#ifndef TASK_MANAGER_INCLUDE_TASK_DATE_H_
#define TASK_MANAGER_INCLUDE_TASK_DATE_H_

#include <ctime>
#include <string>

using Date_t = std::chrono::time_point<std::chrono::system_clock>;

Date_t parse_date(std::string date);

#endif  // TASK_MANAGER_INCLUDE_TASK_DATE_H_
