#ifndef TASKMANAGER_TESTS_TEST_UTILS_MOCKIOFACILITY_H_
#define TASKMANAGER_TESTS_TEST_UTILS_MOCKIOFACILITY_H_

#include <vector>

#include "interactor/io_facility/IIoFacility.h"
#include "test_utils/common.h"

class MockIoFacility : public IIoFacility {
 public:
  void SetInput(std::vector<std::string> input) { input_ = std::move(input); }

  std::vector<std::string> GetOutput() { return output_; }

  std::string GetLine() override {
    auto to_return = input_.back();
    input_.pop_back();
    return to_return;
  }

  void Print(const std::string& str) override { output_.push_back(str); }

 private:
  std::vector<std::string> input_;
  std::vector<std::string> output_;
};

#endif  // TASKMANAGER_TESTS_TEST_UTILS_MOCKIOFACILITY_H_
