#include "logging/DefaultLogFacility.h"

#include <boost/log/attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <fstream>

namespace task_manager {
namespace logging {

namespace sources = boost::log::sources;
namespace trivial = boost::log::trivial;
namespace attributes = boost::log::attributes;
namespace sinks = boost::log::sinks;
namespace expressions = boost::log::expressions;
namespace keywords = boost::log::keywords;

static sources::severity_logger<trivial::severity_level> default_logger;

sources::severity_logger<trivial::severity_level>& GetDefaultLogger() {
  return default_logger;
}

void CreateFileLog(logging::trivial::severity_level level,
                   const std::string& format) {
  boost::log::add_file_log(
      keywords::file_name = "sample_%N.log",
      keywords::rotation_size = 10 * 1024 * 1024, keywords::auto_flush = true,
      keywords::time_based_rotation =
          sinks::file::rotation_at_time_point(0, 0, 0),
      keywords::filter = logging::trivial::severity >= level,
      keywords::format = format);
}

void SetUp() {
  boost::log::add_common_attributes();

  auto format = "[%TimeStamp%][%Severity%](%Scope%): %Message%";

  CreateFileLog(logging::trivial::trace, format);

  boost::log::add_console_log(
      std::clog, boost::log::keywords::format = format,
      keywords::filter = logging::trivial::severity >= logging::trivial::info);

  GetDefaultLogger().add_attribute("Scope", attributes::named_scope());
}
}  // namespace logging
}  // namespace task_manager
