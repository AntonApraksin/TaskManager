#ifndef TASKMANAGER_SRC_REPL_STATE_STATEENUM_H_
#define TASKMANAGER_SRC_REPL_STATE_STATEENUM_H_

enum class StateEnum {
  kMain,
  kAdd,
  kEdit,
  kComplete,
  kDelete,

  kHelp,
  kShow,

  kExit,

  kUnknown,
};

#endif  // TASKMANAGER_SRC_REPL_STATE_STATEENUM_H_
