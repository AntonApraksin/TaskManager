#ifndef TASKMANAGER_SRC_REPL_IO_FACILITY_IOSTREAM_IOSTREAMIOFACILITY_H_
#define TASKMANAGER_SRC_REPL_IO_FACILITY_IOSTREAM_IOSTREAMIOFACILITY_H_

#include "interactor/io_facility/IIoFacility.h"

namespace task_manager {
class IostreamIoFacility : public IIoFacility {
 public:
  void Print(const std::string &string) override;
  std::string GetLine() override;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_IO_FACILITY_IOSTREAM_IOSTREAMIOFACILITY_H_
