#include "interactor/state_machine/interactor_steps/PromptStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/AddLabelStep.h"
#include "interactor/state_machine/interactor_steps/AddStep.h"
#include "interactor/state_machine/interactor_steps/CompleteStep.h"
#include "interactor/state_machine/interactor_steps/DeleteLabelStep.h"
#include "interactor/state_machine/interactor_steps/DeleteStep.h"
#include "interactor/state_machine/interactor_steps/EditStep.h"
#include "interactor/state_machine/interactor_steps/HelpStep.h"
#include "interactor/state_machine/interactor_steps/LoadStep.h"
#include "interactor/state_machine/interactor_steps/SaveStep.h"
#include "interactor/state_machine/interactor_steps/ShowStep.h"
#include "interactor/state_machine/interactor_steps/UnknownStep.h"

namespace task_manager {
std::unique_ptr<Command> PromptStep::execute(StepParameter&) {
  return std::make_unique<VoidCommand>();
}

std::shared_ptr<Step> PromptStep::ChangeStep() {
  std::string input = PrintAndGet(*io_facility_, Strings::GetPrompt(""));
  auto [command, arg] = validator_->MakeRequest(input);
  switch (command) {
    case CommandEnum::kAdd:
      return std::make_shared<AddStep>(validator_, io_facility_,
                                       small_step_factory_, std::move(arg));

    case CommandEnum::kEdit:
      return std::make_shared<EditStep>(validator_, io_facility_,
                                        small_step_factory_, std::move(arg));

    case CommandEnum::kComplete:
      return std::make_shared<CompleteStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));

    case CommandEnum::kDelete:
      return std::make_shared<DeleteStep>(validator_, io_facility_,
                                          small_step_factory_, std::move(arg));

    case CommandEnum::kHelp:
      return std::make_shared<HelpStep>(validator_, io_facility_,
                                        small_step_factory_, std::move(arg));

    case CommandEnum::kAddLabel:
      active_step = std::make_shared<AddLabelStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kDeleteLabel:
      active_step = std::make_shared<DeleteLabelStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kUnknown:
      return std::make_shared<UnknownStep>(validator_, io_facility_,
                                           small_step_factory_, std::move(arg));

    case CommandEnum::kShow:
      return std::make_shared<ShowStep>(validator_, io_facility_,
                                        small_step_factory_, std::move(arg));

    case CommandEnum::kSave:
      return std::make_shared<SaveStep>(validator_, io_facility_,
                                        small_step_factory_, std::move(arg));

    case CommandEnum::kLoad:
      return std::make_shared<LoadStep>(validator_, io_facility_,
                                        small_step_factory_, std::move(arg));

    case CommandEnum::kQuit:
      return nullptr;
  }
}
}  // namespace task_manager