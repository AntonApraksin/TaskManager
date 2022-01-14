#ifndef TASKMANAGER_SRC_REPL_STATE_STATEENUM_H_
#define TASKMANAGER_SRC_REPL_STATE_STATEENUM_H_

namespace task_manager {
enum class CommandEnum {
  kQuit,
  kAdd,
  kEdit,
  kComplete,
  kDelete,
  kShow,
  kHelp,
  kUnknown,
  kSave,
  kLoad,
};
}

#endif  // TASKMANAGER_SRC_REPL_STATE_STATEENUM_H_
