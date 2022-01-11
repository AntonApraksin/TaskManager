#ifndef TASKMANAGER_TESTS_REPL_UNIT_STATE_MACHINE_REPL_STEPS_REPLSTEPTEST_H_
#define TASKMANAGER_TESTS_REPL_UNIT_STATE_MACHINE_REPL_STEPS_REPLSTEPTEST_H_

#include <gtest/gtest.h>

#include "repl/small_steps/default/DefaultSmallStepFactory.h"
#include "repl/state_machine/repl_steps/ReplStep.h"
#include "repl/validator/DefaultValidator.h"
#include "test_utils/MockIoFacility.h"

class ReplStepTest : public ::testing::Test {
 public:
  void SetUp() override {
    validator_ = std::make_shared<DefaultValidator>();
    io_facility_ = std::make_shared<MockIoFacility>();
    small_step_factory_ =
        std::make_shared<DefaultSmallStepFactory>(io_facility_, validator_);
  }

  void SetInput(std::vector<std::string> input) {
    io_facility_->SetInput(
        std::vector<std::string>(input.rbegin(), input.rend()));
  }

 protected:
  std::shared_ptr<DefaultValidator> validator_;
  std::shared_ptr<MockIoFacility> io_facility_;
  std::shared_ptr<ISmallStepFactory> small_step_factory_;
};

#endif  // TASKMANAGER_TESTS_REPL_UNIT_STATE_MACHINE_REPL_STEPS_REPLSTEPTEST_H_
