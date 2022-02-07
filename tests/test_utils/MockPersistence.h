#ifndef TASKMANAGER_TESTS_TEST_UTILS_MOCKPERSISTENCE_H_
#define TASKMANAGER_TESTS_TEST_UTILS_MOCKPERSISTENCE_H_

#include <gmock/gmock.h>

#include "persistence/Persistence.h"
#include "test_utils/common.h"

class MockPersistence : public Persistence {
 public:
  MOCK_METHOD((OperationResult<Status>), Save, (SolidTasks to_save),
              (const, override));
  MOCK_METHOD((OperationResult<Status, SolidTasks>), Load, (),
              (const, override));
};

#endif  // TASKMANAGER_TESTS_TEST_UTILS_MOCKPERSISTENCE_H_
