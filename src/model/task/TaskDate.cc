#include "TaskDate.h"

Date_t parse_date(std::string date) {
  tm timeinfo;
  std::string pattern{"%d/%m/%y"};
  strptime(date.c_str(), pattern.c_str(), &timeinfo);
  return std::chrono::system_clock::from_time_t(std::mktime(&timeinfo));
}
