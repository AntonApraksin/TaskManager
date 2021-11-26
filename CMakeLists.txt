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
        src/model/task_wrapper/TaskStorage.inl src/model/task_wrapper/TaskStorage.h

        src/repl/state/IREPLState.h src/repl/state/MainREPLState.cc src/repl/state/AddNewREPLState.cc src/repl/context/Context.cc src/repl/context/Context.h src/repl/print/IPrinter.h src/repl/print/DefaultPrinter.cc src/repl/print/DefaultPrinter.h src/repl/state_factory/IStateFactory.h src/repl/print/IValidator.h src/repl/print/DefaultValidator.cc src/repl/print/DefaultValidator.h src/repl/state_factory/DefaultStateFactory.cc src/repl/state_factory/DefaultStateFactory.h src/repl/state/HelpREPLState.cc src/repl/state/UnknownREPLState.cc src/repl/substate/IREPLSubState.h src/repl/substate/ReadTitleREPLSubState.cc src/repl/substate/ReadDateREPLSubState.cc src/repl/substate/ReadPriorityREPLSubState.cc src/repl/substate/ReadConfirmationREPLSubState.cc src/repl/state_machine/StateMachine.cc src/repl/state_machine/StateMachine.h src/repl/substate/ReadIdREPLSubState.cc src/repl/TaskBuilder.cc src/repl/TaskBuilder.h src/repl/state/CompleteREPLState.cc src/repl/state/DeleteREPLState.cc src/repl/state/EditREPLState.cc src/repl/commit/ICommitState.h src/repl/commit/NothingCommitState.cc src/repl/commit/DeleteCommitState.cc src/repl/commit/AddCommitState.cc src/repl/commit/EditCommitState.cc src/repl/commit/CompleteCommitState.cc src/repl/state/StateEnum.h src/repl/commit/CommitStateEnum.h src/repl/substate/SubStateEnum.h)

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
