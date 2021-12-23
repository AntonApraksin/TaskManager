#ifndef TASKMANAGER_SRC_REPL_VIEW_VIEW_H_
#define TASKMANAGER_SRC_REPL_VIEW_VIEW_H_

#include <vector>

#include "repl/io_facility/IIoFacility.h"
#include "repl/validator/IValidator.h"
#include "repl/view/steps/CommandEnum.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/IStep.h"

namespace task_manager {
class View {
 public:
  View(const std::shared_ptr<IIoFacility>& io_facility,
       const std::shared_ptr<IValidator>& validator);

  void SetState(const std::shared_ptr<IStep>&);
  StepResult Run();
  std::pair<CommandEnum, std::string> GetNextCommand();

 private:
  std::shared_ptr<IValidator> validator_;
  std::shared_ptr<IIoFacility> io_facility_;
  std::shared_ptr<IStep> current_step_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_VIEW_VIEW_H_