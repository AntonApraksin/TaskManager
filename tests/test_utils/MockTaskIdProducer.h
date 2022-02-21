#ifndef TASKMANAGER_TESTS_TEST_UTILS_MOCKTASKIDPRODUCER_H_
#define TASKMANAGER_TESTS_TEST_UTILS_MOCKTASKIDPRODUCER_H_

#include "model/id/ITaskIdProducer.h"
#include "test_utils/common.h"
#include "utils/TaskIdUtils.h"

class MockTaskIdProducer : public ITaskIdProducer
{
public:
    TaskId GetNextId() override { return CreateTaskId(current_++); }
    TaskId GetCurrentId() override { return CreateTaskId(current_); }

private:
    int current_ = 0;
};

#endif  // TASKMANAGER_TESTS_TEST_UTILS_MOCKTASKIDPRODUCER_H_
