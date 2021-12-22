#ifndef TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_
#define TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_

#include "repl/io_facility/IIoFacility.h"
#include "repl/validator/IValidator.h"
#include "repl/view/steps/ISmallStep.h"

class IostreamSmallStep : public ISmallStep {
 public:
  explicit IostreamSmallStep(const std::shared_ptr<IIoFacility>& io_facility,
                             const std::shared_ptr<IValidator>& validator)
      : validator_(validator), io_facility_(io_facility) {}

 protected:
  std::shared_ptr<IValidator> validator_;
  std::shared_ptr<IIoFacility> io_facility_;
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

class IostreamReadStateSmallStep : public IostreamSmallStep {
 public:
  using IostreamSmallStep::IostreamSmallStep;
  void Execute(TaskContext&) override;
};

#endif  // TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_