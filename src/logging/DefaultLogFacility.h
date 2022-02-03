#ifndef TASKMANAGER_SRC_LOGGING_DEFAULTLOGFACILITY_H_
#define TASKMANAGER_SRC_LOGGING_DEFAULTLOGFACILITY_H_

#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>

namespace task_manager {
namespace logging {
using severinity = boost::log::trivial::severity_level;

boost::log::sources::severity_logger<boost::log::trivial::severity_level>&
GetDefaultLogger();

void SetUp();

void CreateConsoleLog(const std::string& format, severinity level);

void CreateFileLog(const std::string& filename, const std::string& format,
                   severinity level);

std::optional<severinity> ConvertStringToLogLevel(const std::string& str);
}  // namespace logging
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_LOGGING_DEFAULTLOGFACILITY_H_
