#include "repl/state_machine/repl_steps/ReplSteps.h"
#include "repl/view/steps/Strings.h"

namespace task_manager {
std::unique_ptr<Command> PromptReplStep::execute(Context ctx) {
  return std::make_unique<VoidCommand>();
}

void PromptReplStep::ChangeStep(std::shared_ptr<ReplSteps>& active_step) {
  std::string input = PrintAndGet(*io_facility_, Strings::GetPrompt(""));
  auto [command, arg] = validator_->MakeRequest(input);
  switch (command) {
    case CommandEnum::kAdd:
      active_step = std::make_shared<AddReplStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kEdit:
      active_step = std::make_shared<EditReplStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kComplete:
      active_step = std::make_shared<CompleteReplStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kDelete:
      active_step = std::make_shared<DeleteReplStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kHelp:
      active_step = std::make_shared<HelpReplStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kUnknown:
      active_step = std::make_shared<UnknownReplStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kShow:
      active_step = std::make_shared<ShowReplStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kSave:
      active_step = std::make_shared<SaveReplStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kLoad:
      active_step = std::make_shared<LoadReplStep>(
          validator_, io_facility_, small_step_factory_, std::move(arg));
      return;

    case CommandEnum::kQuit:
      active_step.reset();
      return;
  }
}
PromptReplStep::PromptReplStep(
    std::shared_ptr<IValidator> validator,
    std::shared_ptr<IIoFacility> io_facility,
    std::shared_ptr<ISmallStepFactory> small_step_factory)
    : ValidatorAndIoFacilityMixin(std::move(validator), std::move(io_facility)),
      SmallStepMixin(std::move(small_step_factory)) {}
}  // namespace task_manager