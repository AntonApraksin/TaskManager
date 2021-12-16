#ifndef TASKMANAGER_SRC_REPL_VIEW_STEPS_MESSAGEENUM_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEPS_MESSAGEENUM_H_

enum class MessageEnum {
  kInvalidId,
  kMultipleId,
  kRequiredId,
  kNotPresentId,
  kShowId,
  kUnknownCommand,
  kRepeatedId,
};

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEPS_MESSAGEENUM_H_
