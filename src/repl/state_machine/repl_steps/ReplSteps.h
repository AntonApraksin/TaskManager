#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_REPLSTEPS_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_REPLSTEPS_H_

#include "repl/io_facility/IIoFacility.h"
#include "repl/state_machine/commands/Commands.h"
#include "repl/validator/IValidator.h"

namespace task_manager {

class ISmallStepFactory;

class ReplSteps {
 public:
  virtual std::unique_ptr<Command> execute(Context ctx) = 0;
  virtual void ChangeStep(std::shared_ptr<ReplSteps>&) = 0;
  virtual ~ReplSteps() {}
};

class ValidatorAndIoFacilityMixin {
 public:
  ValidatorAndIoFacilityMixin(std::shared_ptr<IValidator> validator,
                              std::shared_ptr<IIoFacility> io_facility)
      : validator_(std::move(validator)),
        io_facility_(std::move(io_facility)) {}

 protected:
  ~ValidatorAndIoFacilityMixin() {}

  std::shared_ptr<IValidator> validator_;
  std::shared_ptr<IIoFacility> io_facility_;
};

class SmallStepMixin {
 public:
  explicit SmallStepMixin(
      const std::shared_ptr<ISmallStepFactory>& small_step_factory)
      : small_step_factory_(small_step_factory) {}

 protected:
  ~SmallStepMixin() {}

  std::shared_ptr<ISmallStepFactory> small_step_factory_;
};

class PromptReplStep : public ReplSteps,
                       public ValidatorAndIoFacilityMixin,
                       public SmallStepMixin {
 public:
  PromptReplStep(std::shared_ptr<IValidator> validator,
                 std::shared_ptr<IIoFacility> io_facility,
                 std::shared_ptr<ISmallStepFactory> small_step_factory);

  std::unique_ptr<Command> execute(Context ctx) override;
  void ChangeStep(std::shared_ptr<ReplSteps>&) override;
};

class ArgumentMixin : public ValidatorAndIoFacilityMixin {
 public:
  ArgumentMixin(std::shared_ptr<IValidator> validator,
                std::shared_ptr<IIoFacility> io_facility, std::string arg)
      : ValidatorAndIoFacilityMixin(std::move(validator),
                                    std::move(io_facility)),
        arg_(std::move(arg)) {}

 protected:
  ~ArgumentMixin() {}

  std::string arg_;
};

class AddReplStep : public ReplSteps,
                    public ArgumentMixin,
                    public SmallStepMixin {
 public:
  AddReplStep(std::shared_ptr<IValidator> validator,
              std::shared_ptr<IIoFacility> io_facility,
              std::shared_ptr<ISmallStepFactory> small_step_factory,
              std::string arg)
      : ArgumentMixin(std::move(validator), std::move(io_facility),
                      std::move(arg)),
        SmallStepMixin(std::move(small_step_factory)) {}

  std::unique_ptr<Command> execute(Context) override;
  void ChangeStep(std::shared_ptr<ReplSteps>&) override;

 private:
  template <int>
  std::unique_ptr<Command> HandleStage(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<2>(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<1>(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<0>(Context&);

  std::unique_ptr<Command> HandleAddTask(Context&);
  std::unique_ptr<Command> HandleAddSubTask(Context&);

  std::unique_ptr<Command> ReportError(std::string);

  int stage_ = 3;
  std::optional<TaskId> task_id_;
};

class EditReplStep : public ReplSteps,
                     public ArgumentMixin,
                     public SmallStepMixin {
 public:
  EditReplStep(std::shared_ptr<IValidator> validator,
               std::shared_ptr<IIoFacility> io_facility,
               std::shared_ptr<ISmallStepFactory> small_step_factory,
               std::string arg)
      : ArgumentMixin(std::move(validator), std::move(io_facility),
                      std::move(arg)),
        SmallStepMixin(std::move(small_step_factory)) {}

  std::unique_ptr<Command> execute(Context ctx) override;
  void ChangeStep(std::shared_ptr<ReplSteps>&) override;

 private:
  template <int>
  std::unique_ptr<Command> HandleStage(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<1>(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<0>(Context&);

  std::unique_ptr<Command> ReportError(std::string);

  TaskId task_id_;
  int stage_ = 2;
};

class CompleteReplStep : public ReplSteps,
                         public ArgumentMixin,
                         public SmallStepMixin {
 public:
  CompleteReplStep(std::shared_ptr<IValidator> validator,
                   std::shared_ptr<IIoFacility> io_facility,
                   std::shared_ptr<ISmallStepFactory> small_step_factory,
                   std::string arg)
      : ArgumentMixin(std::move(validator), std::move(io_facility),
                      std::move(arg)),
        SmallStepMixin(std::move(small_step_factory)) {}

  std::unique_ptr<Command> execute(Context ctx) override;
  void ChangeStep(std::shared_ptr<ReplSteps>&) override;

 private:
  template <int>
  std::unique_ptr<Command> HandleStage(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<1>(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<0>(Context&);

  std::unique_ptr<Command> ReportError(std::string);

  std::vector<TaskId> task_ids_;
  int stage_ = 2;
};

class DeleteReplStep : public ReplSteps,
                       public ArgumentMixin,
                       public SmallStepMixin {
 public:
  DeleteReplStep(std::shared_ptr<IValidator> validator,
                 std::shared_ptr<IIoFacility> io_facility,
                 std::shared_ptr<ISmallStepFactory> small_step_factory,
                 std::string arg)
      : ArgumentMixin(std::move(validator), std::move(io_facility),
                      std::move(arg)),
        SmallStepMixin(std::move(small_step_factory)) {}

  std::unique_ptr<Command> execute(Context ctx) override;
  void ChangeStep(std::shared_ptr<ReplSteps>&) override;

 private:
  template <int>
  std::unique_ptr<Command> HandleStage(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<1>(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<0>(Context&);

  std::unique_ptr<Command> ReportError(std::string);

  std::vector<TaskId> task_ids_;
  int stage_ = 2;
};

class ShowReplStep : public ReplSteps,
                     public ArgumentMixin,
                     public SmallStepMixin {
 public:
  ShowReplStep(std::shared_ptr<IValidator> validator,
               std::shared_ptr<IIoFacility> io_facility,
               std::shared_ptr<ISmallStepFactory> small_step_factory,
               std::string arg)
      : ArgumentMixin(std::move(validator), std::move(io_facility),
                      std::move(arg)),
        SmallStepMixin(std::move(small_step_factory)) {}

  std::unique_ptr<Command> execute(Context ctx) override;
  void ChangeStep(std::shared_ptr<ReplSteps>&) override;

 private:
  template <int>
  std::unique_ptr<Command> HandleStage(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<1>(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<0>(Context&);

  std::unique_ptr<Command> ReportError(std::string);

  std::vector<TaskId> task_ids_;
  int stage_ = 2;
};

class SaveReplStep : public ReplSteps,
                     public ArgumentMixin,
                     public SmallStepMixin {
 public:
  SaveReplStep(std::shared_ptr<IValidator> validator,
               std::shared_ptr<IIoFacility> io_facility,
               std::shared_ptr<ISmallStepFactory> small_step_factory,
               std::string arg)
      : ArgumentMixin(std::move(validator), std::move(io_facility),
                      std::move(arg)),
        SmallStepMixin(std::move(small_step_factory)) {}

  std::unique_ptr<Command> execute(Context ctx) override;
  void ChangeStep(std::shared_ptr<ReplSteps>&) override;

 private:
  template <int>
  std::unique_ptr<Command> HandleStage(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<1>(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<0>(Context&);

  std::unique_ptr<Command> ReportError(std::string);

  int stage_ = 2;
  std::string filename_;
};

class LoadReplStep : public ReplSteps,
                     public ArgumentMixin,
                     public SmallStepMixin {
 public:
  LoadReplStep(std::shared_ptr<IValidator> validator,
               std::shared_ptr<IIoFacility> io_facility,
               std::shared_ptr<ISmallStepFactory> small_step_factory,
               std::string arg)
      : ArgumentMixin(std::move(validator), std::move(io_facility),
                      std::move(arg)),
        SmallStepMixin(std::move(small_step_factory)) {}

  std::unique_ptr<Command> execute(Context ctx) override;
  void ChangeStep(std::shared_ptr<ReplSteps>&) override;

 private:
  template <int>
  std::unique_ptr<Command> HandleStage(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<1>(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<0>(Context&);

  std::unique_ptr<Command> ReportError(std::string);

  std::string filename_;
  int stage_ = 2;
};

class HelpReplStep : public ReplSteps,
                     public ArgumentMixin,
                     public SmallStepMixin {
 public:
  HelpReplStep(std::shared_ptr<IValidator> validator,
               std::shared_ptr<IIoFacility> io_facility,
               std::shared_ptr<ISmallStepFactory> small_step_factory,
               std::string arg)
      : ArgumentMixin(std::move(validator), std::move(io_facility),
                      std::move(arg)),
        SmallStepMixin(std::move(small_step_factory)) {}

  std::unique_ptr<Command> execute(Context ctx) override;
  void ChangeStep(std::shared_ptr<ReplSteps>&) override;
};

class UnknownReplStep : public ReplSteps,
                        public ArgumentMixin,
                        public SmallStepMixin {
 public:
  UnknownReplStep(std::shared_ptr<IValidator> validator,
                  std::shared_ptr<IIoFacility> io_facility,
                  std::shared_ptr<ISmallStepFactory> small_step_factory,
                  std::string arg)
      : ArgumentMixin(std::move(validator), std::move(io_facility),
                      std::move(arg)),
        SmallStepMixin(std::move(small_step_factory)) {}

  std::unique_ptr<Command> execute(Context ctx) override;
  void ChangeStep(std::shared_ptr<ReplSteps>&) override;
};

}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_REPLSTEPS_H_
