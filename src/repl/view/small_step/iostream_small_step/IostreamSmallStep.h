#ifndef TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_
#define TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_

#include "repl/io_facility/IIOFacility.h"
#include "repl/io_facility/IValidator.h"
#include "repl/view/small_step/ISmallStep.h"

class Context;

class IostreamSmallStep : public ISmallStep {
 public:
  IostreamSmallStep(const std::shared_ptr<IIOFacility>& printer,
                    const std::shared_ptr<IValidator>& validator)
      : printer_(printer), validator_(validator) {}

 protected:
  std::shared_ptr<IIOFacility> printer_;
  std::shared_ptr<IValidator> validator_;
};

class IostreamReadTitleSmallStep : public IostreamSmallStep {
 public:
  using IostreamSmallStep::IostreamSmallStep;
  void Execute(TaskContext&) override;
};

class IostreamReadDateSmallStep : public IostreamSmallStep {
 public:
  using IostreamSmallStep::IostreamSmallStep;
  void Execute(TaskContext&) override;
};

class IostreamReadPrioritySmallStep : public IostreamSmallStep {
 public:
  using IostreamSmallStep::IostreamSmallStep;
  void Execute(TaskContext&) override;
};

#endif  // TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_