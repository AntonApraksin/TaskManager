#ifndef TASKMANAGER_SRC_REPL_IO_FACILITY_IIOFACILITY_H_
#define TASKMANAGER_SRC_REPL_IO_FACILITY_IIOFACILITY_H_

#include <string>

class IIoFacility {
 public:
  virtual ~IIoFacility() {}

  virtual void Print(const std::string&) = 0;
  virtual std::string GetLine() = 0;
};

#endif  // TASKMANAGER_SRC_REPL_IO_FACILITY_IIOFACILITY_H_
