#include "interactor/state_machine/interactor_steps/PromptStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/AddStep.h"
#include "interactor/state_machine/interactor_steps/CompleteStep.h"
#include "interactor/state_machine/interactor_steps/DeleteStep.h"
#include "interactor/state_machine/interactor_steps/EditStep.h"
#include "interactor/state_machine/interactor_steps/HelpStep.h"
#include "interactor/state_machine/interactor_steps/LoadStep.h"
#include "interactor/state_machine/interactor_steps/SaveStep.h"
#include "interactor/state_machine/interactor_steps/ShowStep.h"
#include "interactor/state_machine/interactor_steps/UnknownStep.h"

namespace task_manager {
std::unique_ptr<Command> PromptStep::execute(Context) {
  return std::make_unique<VoidCommand>();
}

void PromptStep::ChangeStep(std::shared_ptr<Step>& active_step) {
  std::string input = PrintAndGet(*io_facility_, Strings::GetPrompt(""));
  auto [command, arg] = validator_->MakeRequest(input);
  switch (command) {
    case CommandEnum::kAdd:
      active_step = std::make_shared<AddStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kEdit:
      active_step = std::make_shared<EditStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kComplete:
      active_step = std::make_shared<CompleteStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kDelete:
      active_step = std::make_shared<DeleteStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kHelp:
      active_step = std::make_shared<HelpStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kAddLabel:

    case CommandEnum::kDeleteLabel:

    case CommandEnum::kUnknown:
      active_step = std::make_shared<UnknownStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kShow:
      active_step = std::make_shared<ShowStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kSave:
      active_step = std::make_shared<SaveStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kLoad:
      active_step = std::make_shared<LoadStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kQuit:
      active_step.reset();
      return;
  }
}
}  // namespace task_manager