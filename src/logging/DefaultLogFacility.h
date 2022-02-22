#ifndef TASKMANAGER_SRC_LOGGING_DEFAULTLOGFACILITY_H_
#define TASKMANAGER_SRC_LOGGING_DEFAULTLOGFACILITY_H_

#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>

namespace task_manager
{
namespace logging
{
using severity = boost::log::trivial::severity_level;

boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>&
GetDefaultLogger();

void SetUp();

void CreateFileLog(const std::string& filename, const std::string& format,
                   severity level);

std::optional<severity> ConvertStringToLogLevel(const std::string& str);
}  // namespace logging
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_LOGGING_DEFAULTLOGFACILITY_H_
