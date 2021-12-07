#ifndef TASKMANAGER_SRC_REPL_IO_FACILITY_IOSTREAM_IOSTREAMIOFACILITY_H_
#define TASKMANAGER_SRC_REPL_IO_FACILITY_IOSTREAM_IOSTREAMIOFACILITY_H_

#include "repl/io_facility/IIoFacility.h"

class IostreamIoFacility : public IIoFacility {
 public:
  void Print(const std::string &string) override;
  std::string GetLine() override;
};

#endif  // TASKMANAGER_SRC_REPL_IO_FACILITY_IOSTREAM_IOSTREAMIOFACILITY_H_
