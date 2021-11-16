#include "model/TaskIdProducer.h"

TaskIdProducer::TaskIdProducer() : current_id_{0} {}

TaskId TaskIdProducer::GetNextId() { return TaskId::Create(current_id_++); }
