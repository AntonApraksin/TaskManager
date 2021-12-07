#ifndef TASKMANAGER_SRC_REPL_STATE_STATEENUM_H_
#define TASKMANAGER_SRC_REPL_STATE_STATEENUM_H_

enum class CommandEnum {
  kMain,
  kAdd,
  kEdit,
  kComplete,
  kDelete,
  kShow,
  kHelp,
  kUnknown,
  kExit,

};

#endif  // TASKMANAGER_SRC_REPL_STATE_STATEENUM_H_
