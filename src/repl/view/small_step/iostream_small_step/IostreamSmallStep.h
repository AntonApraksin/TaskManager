#ifndef TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_
#define TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_

#include "repl/io_facility/IValidator.h"
#include "repl/view/small_step/ISmallStep.h"

class Context;

class IostreamSmallStep : public ISmallStep {
 public:
  explicit IostreamSmallStep(const std::shared_ptr<IValidator>& validator)
      : validator_(validator) {}

 protected:
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