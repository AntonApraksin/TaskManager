#ifndef TASKMANAGER_SRC_REPL_STATE_STATEENUM_H_
#define TASKMANAGER_SRC_REPL_STATE_STATEENUM_H_

enum class StateEnum {
  kMain,
  kAdd,
  kExit,
  kHelp,
  kComplete,
  kDelete,
  kEdit,

  kUnknown,
  kDefault,
};

#endif  // TASKMANAGER_SRC_REPL_STATE_STATEENUM_H_
