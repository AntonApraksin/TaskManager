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
        src/model/task/TaskDate.cc
        src/model/task_manager/TaskManager.cc
        src/model/id/TaskId.h
        src/model/id/TaskIdProducer.h
        src/model/task/Task.h
        src/model/task/TaskDate.h
        src/model/task_manager/TaskManager.h
        src/model/task_wrapper/TaskWrapper.cc
        src/model/task_wrapper/TaskWrapper.h
        src/model/task_wrapper/TaskStorage.inl
        src/model/task_wrapper/TaskStorage.h
        src/repl/io_facility/IIOFacility.h
        src/repl/io_facility/DefaultIOFacility.cc
        src/repl/io_facility/DefaultIOFacility.h
        src/repl/state_factory/IStateFactory.h
        src/repl/io_facility/IValidator.h
        src/repl/io_facility/DefaultValidator.cc
        src/repl/io_facility/DefaultValidator.h
        src/repl/state_factory/DefaultStateFactory.cc
        src/repl/state_factory/DefaultStateFactory.h
        src/repl/substate/repl/IREPLSubState.h
        src/repl/substate/repl/ReadTitleREPLSubState.cc
        src/repl/substate/repl/ReadDateREPLSubState.cc
        src/repl/substate/repl/ReadPriorityREPLSubState.cc
        src/repl/TaskBuilder.cc
        src/repl/TaskBuilder.h
        src/repl/view/StateEnum.h
        src/repl/substate/repl/SubStateEnum.h
        src/repl/substate/ISubState.h
        src/repl/substate/TaskContext.cc
        src/repl/substate/TaskContext.h
        src/repl/view/View.cc
        src/repl/view/View.h
        src/repl/Controller.cc
        src/repl/Controller.h src/repl/view/step/IStep.h src/repl/view/step/StepResult.h src/repl/view/step/iostream_step/IostreamStep.h src/repl/view/step/iostream_step/IostreamAddTaskREPLState.cc src/repl/view/step/iostream_step/IostreamEditTaskREPLState.cc src/repl/view/step/iostream_step/IostreamDeleteTaskREPLState.cc src/repl/view/step/iostream_step/IostreamCompleteTaskREPLState.cc src/repl/view/step/iostream_step/IostreamShowAllTasksREPLState.cc src/repl/view/step/iostream_step/IostreamShowNTasksREPLState.cc src/repl/view/step/iostream_step/IostreamShowSortedTasksREPLState.cc src/repl/view/step/IStepFactory.h src/repl/view/step/iostream_step/IostreamStepFactory.h src/repl/view/step/iostream_step/IostreamStepFactory.cc)

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
