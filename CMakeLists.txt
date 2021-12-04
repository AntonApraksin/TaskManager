cmake_minimum_required(VERSION 3.14)

project(TaskManager)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

add_compile_options(-Werror -Wno-language-extension-token)

add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++)

enable_testing()

set(TaskManager_SRCS
    src/model/id/TaskId.cc
    src/model/id/TaskIdProducer.cc
    src/model/task/Task.cc
    src/model/task_manager/TaskManager.cc
    src/model/id/TaskId.h
    src/model/id/TaskIdProducer.h
    src/model/task/Task.h
    src/model/task/TaskDate.h
    src/model/task_manager/TaskManager.h
    src/model/task_wrapper/TaskWrapper.cc
    src/model/task_wrapper/TaskWrapper.h
    src/model/task_wrapper/TaskStorage.h
    src/repl/view/steps/ISmallStepFactory.h
    src/repl/validator/IValidator.h
    src/repl/validator/DefaultValidator.cc
    src/repl/validator/DefaultValidator.h
    src/repl/view/steps/iostream/small_step/IostreamSmallStepFactory.cc
    src/repl/view/steps/iostream/small_step/IostreamSmallStepFactory.h
    src/repl/view/steps/iostream/small_step/IostreamSmallStep.h
    src/repl/view/steps/iostream/small_step/IostreamReadTitleSmallStep.cc
    src/repl/view/steps/iostream/small_step/IostreamReadDateSmallStep.cc
    src/repl/view/steps/iostream/small_step/IostreamReadPrioritySmallStep.cc
    src/repl/TaskBuilder.cc
    src/repl/TaskBuilder.h
    src/repl/view/StateEnum.h
    src/repl/view/steps/iostream/small_step/IostreamSmallStepEnum.h
    src/repl/view/steps/ISmallStep.h
    src/repl/view/steps/TaskContext.cc
    src/repl/view/steps/TaskContext.h
    src/repl/view/View.cc
    src/repl/view/View.h
    src/repl/Controller.cc
    src/repl/Controller.h
    src/repl/view/steps/IStep.h
    src/repl/view/steps/StepResult.h
    src/repl/view/steps/iostream/step/IostreamStep.h
    src/repl/view/steps/iostream/step/IostreamAddTaskREPLState.cc
    src/repl/view/steps/iostream/step/IostreamEditTaskREPLState.cc
    src/repl/view/steps/iostream/step/IostreamDeleteTaskREPLState.cc
    src/repl/view/steps/iostream/step/IostreamCompleteTaskREPLState.cc
    src/repl/view/steps/iostream/step/IostreamShowAllTasksREPLState.cc
    src/repl/view/steps/iostream/step/IostreamShowNTasksREPLState.cc
    src/repl/view/steps/iostream/step/IostreamShowSortedTasksREPLState.cc
    src/repl/view/steps/IStepFactory.h
    src/repl/view/steps/iostream/step/IostreamStepFactory.h
    src/repl/view/steps/iostream/step/IostreamStepFactory.cc
    src/repl/view/steps/iostream/IostreamGeneralFunctional.cc
    src/repl/view/steps/iostream/IostreamGeneralFunctional.h
    src/repl/validator/DateFormat.h
    src/repl/view/steps/DefaultTaskInitializerSmallStep.cc
    src/repl/view/steps/TaskInitializerSmallStep.h
    src/repl/view/steps/iostream/small_step/IostreamReadStateSmallStep.cc
)

add_library(TaskManager
  STATIC ${TaskManager_SRCS}
)

add_executable(main
        src/main.cc)

target_link_libraries(main TaskManager)

target_include_directories(TaskManager
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory(tests)
