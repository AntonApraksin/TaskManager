#ifndef TASKMANAGER_SRC_REPL_IO_FACILITY_DATEFORMAT_H_
#define TASKMANAGER_SRC_REPL_IO_FACILITY_DATEFORMAT_H_

namespace task_manager
{
constexpr const char* kDateRegex = "\\d\\d:\\d\\d \\d\\d/\\d\\d/\\d\\d\\d\\d";
constexpr const char* kDatePattern = "%H:%M %d/%m/%Y";
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_IO_FACILITY_DATEFORMAT_H_
