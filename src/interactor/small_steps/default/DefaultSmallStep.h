#ifndef TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_
#define TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_

#include "interactor/io_facility/IIoFacility.h"
#include "interactor/small_steps/ISmallStep.h"
#include "interactor/validator/IValidator.h"

namespace task_manager {
class DefaultSmallStep : public ISmallStep {
 public:
  explicit DefaultSmallStep(const std::shared_ptr<IIoFacility>& io_facility,
                            const std::shared_ptr<IValidator>& validator)
      : validator_(validator), io_facility_(io_facility) {}

 protected:
  std::shared_ptr<IValidator> validator_;
  std::shared_ptr<IIoFacility> io_facility_;
};

class DefaultReadTitleSmallStep : public DefaultSmallStep {
 public:
  using DefaultSmallStep::DefaultSmallStep;
  void Execute(TaskContext&) override;
};

class DefaultReadDateSmallStep : public DefaultSmallStep {
 public:
  using DefaultSmallStep::DefaultSmallStep;
  void Execute(TaskContext&) override;
};

class DefaultReadPrioritySmallStep : public DefaultSmallStep {
 public:
  using DefaultSmallStep::DefaultSmallStep;
  void Execute(TaskContext&) override;
};

class DefaultReadStateSmallStep : public DefaultSmallStep {
 public:
  using DefaultSmallStep::DefaultSmallStep;
  void Execute(TaskContext&) override;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_