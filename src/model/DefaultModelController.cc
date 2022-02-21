#include "DefaultModelController.h"

#include <google/protobuf/util/time_util.h>

#include <boost/log/attributes/named_scope.hpp>

#include "logging/DefaultLogFacility.h"
#include "model/task_manager/TaskManager.h"
#include "persistence/Persistence.h"
#include "utils/TaskIdUtils.h"

namespace task_manager
{
using MCStatus = ModelController::Status;

MCStatus TMStatusToMCStatus(TaskManager::Status tmstatus)
{
    switch (tmstatus)
    {
        case TaskManager::Status::kNotPresentId:return MCStatus::kNotPresentId;

        case TaskManager::Status::kOk:return MCStatus::kOk;

        case TaskManager::Status::kNotPresentLabel:return MCStatus::kNotPresentLabel;
    }
}

DefaultModelController::DefaultModelController(
    std::unique_ptr<TaskManager> task_manager,
    std::unique_ptr<Persistence> persistence)
    : task_manager_(std::move(task_manager)),
      persistence_(std::move(persistence)) {}

OperationResult<MCStatus, TaskId> DefaultModelController::Add(Task task)
{
    BOOST_LOG_NAMED_SCOPE("DefaultModelController::Add(Task task)");
    auto& logger = logging::GetDefaultLogger();

    auto to_log = task;

    OperationResult<TaskManager::Status, TaskId> result;
    {
        std::lock_guard<std::mutex> lock(task_manager_mutex_);
        result = task_manager_->Add(std::move(task));
    }

    if (result)
    {
        BOOST_LOG_SEV(logger, logging::severinity::info)
            << "Added task: "
            << "TITLE:'" << to_log.title() << "' DUEDATE:" << to_log.due_date()
            << " PRIORITY:" << Task_Priority_Name(to_log.priority())
            << " PROGRESS:" << Task_Progress_Name(to_log.progress())
            << " with id:" << result.AccessResult().id();
        return OperationResult<Status, TaskId>::Ok(result.AccessResult());
    }

    BOOST_LOG_SEV(logger, logging::severinity::info)
        << "Failed to add task: "
        << "TITLE:'" << to_log.title() << "' DUEDATE:" << to_log.due_date()
        << " PRIORITY:" << Task_Priority_Name(to_log.priority())
        << " PROGRESS:" << Task_Progress_Name(to_log.progress());

    return OperationResult<Status, TaskId>::Error(
        TMStatusToMCStatus(result.GetStatus()));
}

OperationResult<MCStatus, TaskId> DefaultModelController::Add(TaskId id,
                                                              Task task)
{
    OperationResult<TaskManager::Status, TaskId> result;
    {
        std::lock_guard<std::mutex> lock(task_manager_mutex_);
        result = task_manager_->Add(std::move(id), std::move(task));
    }

    if (result)
    {
        return OperationResult<Status, TaskId>::Ok(result.AccessResult());
    }
    return OperationResult<Status, TaskId>::Error(
        TMStatusToMCStatus(result.GetStatus()));
}

OperationResult<MCStatus> DefaultModelController::Edit(TaskId id, Task task)
{
    OperationResult<TaskManager::Status> result;
    {
        std::lock_guard<std::mutex> lock(task_manager_mutex_);
        result = task_manager_->Edit(std::move(id), std::move(task));
    }

    if (result)
    {
        return OperationResult<Status>::Ok();
    }
    return OperationResult<Status>::Error(TMStatusToMCStatus(result.GetStatus()));
}

OperationResult<MCStatus> DefaultModelController::Complete(TaskId id)
{
    OperationResult<TaskManager::Status> result;
    {
        std::lock_guard<std::mutex> lock(task_manager_mutex_);
        result = task_manager_->Complete(std::move(id));
    }

    if (result)
    {
        return OperationResult<Status>::Ok();
    }
    return OperationResult<Status>::Error(TMStatusToMCStatus(result.GetStatus()));
}

OperationResult<MCStatus> DefaultModelController::Delete(TaskId id)
{
    OperationResult<TaskManager::Status> result;
    {
        std::lock_guard<std::mutex> lock(task_manager_mutex_);
        result = task_manager_->Delete(std::move(id));
    }

    if (result)
    {
        return OperationResult<Status>::Ok();
    }
    return OperationResult<Status>::Error(TMStatusToMCStatus(result.GetStatus()));
}

// Precondition: All ids must be present in store
SolidTasks GetSolidTasksSorted(TaskManager::Storage storage)
{
    if (storage.roots.empty())
    {
        return {};
    }
    auto& roots = storage.roots;
    auto& parents = storage.parents;
    auto& tasks = storage.tasks;
    SolidTasks result;
    std::vector<TaskId> to_traverse{storage.roots.back()};
    storage.roots.pop_back();
    bool is_growing = true;
    for (; !to_traverse.empty() ;)
    {
        if (is_growing and tasks.find(to_traverse.back()) != tasks.end())
        {
            auto id = to_traverse.back();
            SolidTask solid_task;
            solid_task.set_allocated_task_id(new TaskId(id));
            auto found_task = tasks.find(id);
            solid_task.set_allocated_task(new Task(std::move(found_task->second)));
            tasks.erase(found_task);
            if (to_traverse.size() > 1)
            {
                auto parent_id = to_traverse[to_traverse.size() - 2];
                solid_task.set_allocated_parent_id(new TaskId(std::move(parent_id)));
            }
            result.push_back(std::move(solid_task));
        }

        if (auto it = parents.find(to_traverse.back()); it != parents.end())
        {
            if (!it->second.empty())
            {
                to_traverse.push_back(it->second.back());
                it->second.pop_back();
                is_growing = true;
            }
            else
            {
                parents.erase(it);
            }
            continue;
        }
        is_growing = false;
        to_traverse.pop_back();
        if (!roots.empty() && to_traverse.empty())
        {
            to_traverse.push_back(roots.back());
            roots.pop_back();
            is_growing = true;
        }
    }
    return result;
}

OperationResult<MCStatus, SolidTasks>
DefaultModelController::GetAllSolidTasks()
{
    OperationResult<TaskManager::Status, TaskManager::Storage> result;
    {
        std::lock_guard<std::mutex> lock(task_manager_mutex_);
        result = task_manager_->Show();
    }
    return OperationResult<Status, SolidTasks>::Ok(
        GetSolidTasksSorted(result.AccessResult()));
}

OperationResult<MCStatus, SolidTasks>
DefaultModelController::GetSpecificSolidTasks(std::vector<TaskId> ids)
{
    BOOST_LOG_NAMED_SCOPE("DefaultModelController::GetSpecificSolidTasks");

    auto& logger = logging::GetDefaultLogger();

    {
        boost::log::record rec = logger.open_record(boost::log::keywords::severity =
                                                        logging::severinity::info);
        if (rec)
        {
            boost::log::record_ostream strm(rec);
            strm << "getting tasks with ids: ";
            for (const auto& i : ids)
            {
                strm << i.id() << " ";
            }
            strm.flush();
            logger.push_record(std::move(rec));
        }
    }

    TaskManager::Storage storage;
    {
        std::lock_guard<std::mutex> lock(task_manager_mutex_);
        storage = task_manager_->Show().AccessResult();
    }

    for (const auto& i : ids)
    {
        if (storage.tasks.find(i) == storage.tasks.end())
        {
            BOOST_LOG_SEV(logger, logging::severinity::info)
                << "id " << i.id() << " was not found";
            return OperationResult<Status, SolidTasks>::Error(Status::kNotPresentId);
        }
    }
    std::sort(ids.begin(), ids.end(), std::greater<TaskId>());
    storage.roots = std::move(ids);
    return OperationResult<Status, SolidTasks>::Ok(
        GetSolidTasksSorted(std::move(storage)));
}

OperationResult<MCStatus> DefaultModelController::Load()
{
    OperationResult<Persistence::Status, SolidTasks> result;
    {
        std::lock_guard<std::mutex> lock(persistence_mutex_);
        result = persistence_->Load();
    }
    if (!result)
    {
        return OperationResult<Status>::Error(Status::kLoadFailure);
    }
    if (result.AccessResult().empty())
    {
        auto task_id_producer = std::make_unique<TaskIdProducer>();
        std::lock_guard<std::mutex> lock(task_manager_mutex_);
        task_manager_ = std::make_unique<TaskManager>(std::move(task_id_producer));
        return OperationResult<Status>::Ok();
    }
    TaskManager::Parents parents;
    TaskManager::Tasks tasks;
    TaskManager::Roots roots;
    TaskId max_task_id = result.AccessResult().back().task_id();
    for (auto& i : result.AccessResult())
    {
        if (i.task_id() > max_task_id)
        {
            max_task_id = i.task_id();
        }
        if (!i.has_parent_id())
        {
            roots.push_back(i.task_id());
            tasks.insert({i.task_id(), i.task()});
            parents.insert({i.task_id(), {}});
        }
        else
        {
            parents[i.parent_id()].push_back(i.task_id());
            parents.insert({i.task_id(), {}});
            tasks.insert({i.task_id(), i.task()});
        }
    }
    TaskManager::Storage storage;
    storage.roots = std::move(roots);
    storage.parents = std::move(parents);
    storage.tasks = std::move(tasks);
    auto task_id_producer =
        std::make_unique<TaskIdProducer>(std::move(max_task_id));
    task_id_producer->GetNextId();

    std::lock_guard<std::mutex> lock(task_manager_mutex_);
    task_manager_ = std::make_unique<TaskManager>(std::move(task_id_producer),
                                                  std::move(storage));
    return OperationResult<Status>::Ok();
}

OperationResult<MCStatus> DefaultModelController::Save()
{
    auto solid_tasks = GetAllSolidTasks();

    OperationResult<Persistence::Status> result;
    {
        std::lock_guard<std::mutex> lock(persistence_mutex_);
        result = persistence_->Save(solid_tasks.AccessResult());
    }
    if (!result)
    {
        return OperationResult<MCStatus>::Error(Status::kSaveFailure);
    }
    return OperationResult<Status>::Ok();
}

OperationResult<MCStatus> DefaultModelController::AddLabel(TaskId task_id,
                                                           Label label)
{
    OperationResult<TaskManager::Status> result;
    {
        std::lock_guard<std::mutex> lock(task_manager_mutex_);
        result = task_manager_->AddLabel(std::move(task_id), std::move(label));
    }

    if (result)
    {
        return OperationResult<Status>::Ok();
    }
    return OperationResult<MCStatus>::Error(
        TMStatusToMCStatus(result.GetStatus()));
}
OperationResult<MCStatus> DefaultModelController::DeleteLabel(TaskId task_id,
                                                              Label label)
{
    OperationResult<TaskManager::Status> result;
    {
        std::lock_guard<std::mutex> lock(task_manager_mutex_);
        result = task_manager_->DeleteLabel(std::move(task_id), std::move(label));
    }

    if (result)
    {
        return OperationResult<Status>::Ok();
    }
    return OperationResult<MCStatus>::Error(
        TMStatusToMCStatus(result.GetStatus()));
}

std::optional<std::pair<TaskId, TaskId>> HasParentChildRelationship(
    const SolidTasks& tasks, const std::vector<TaskId>& ids)
{
    std::unordered_map<TaskId, std::vector<TaskId>> visited;
    TaskId current_root_id;
    for (const auto& i : tasks)
    {
        if (i.has_parent_id())
        {
            visited[current_root_id].push_back(i.task_id());
        }
        else
        {
            current_root_id = i.task_id();
        }
    }
    for (const auto& i : ids)
    {
        auto it = std::find_if(visited.begin(), visited.end(), [&i](auto it)
        {
            return std::find(it.second.begin(), it.second.end(), i) !=
                it.second.end();
        });
        if (it != visited.end())
        {
            return std::make_pair(it->first, i);
        }
    }
    return {};
}

}  // namespace task_manager
