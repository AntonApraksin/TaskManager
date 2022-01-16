#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_STATEMACHINECONTROLLER_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_STATEMACHINECONTROLLER_H_

#include "interactor/state_machine/StateMachine.h"
#include "model/ModelController.h"

namespace task_manager {
class StateMachineController {
 public:
  StateMachineController(std::shared_ptr<ModelController>,
                         std::unique_ptr<StateMachine>);

  void Run();

 private:
  std::shared_ptr<ModelController> model_controller_;
  std::unique_ptr<StateMachine> state_machine_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_STATEMACHINECONTROLLER_H_
