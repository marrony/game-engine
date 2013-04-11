/*
 * task.h
 *
 *  Created on: 10/04/2013
 *      Author: marrony
 */

#ifndef TASK_H_
#define TASK_H_

#include <pthread.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

typedef int32_t TaskId;

struct WorkItem {
	void (*do_task)(TaskId, WorkItem& item);
};

struct Task {
	TaskId id;
	TaskId parent;
	TaskId dependency;
	WorkItem work_item;
	uint32_t work_count;
};

const int32_t NUMBER_OF_TASKS = 32;
const int32_t NUMBER_OF_THREADS = 5;
const TaskId INVALID_ID = -1;

class TaskManager {
	struct Comp {
		TaskManager* task_manager;

		Comp(TaskManager* task_manager) : task_manager(task_manager) {
		}

		bool operator()(Task* a, Task* b) {
			bool a_dependency = task_manager->has_dependency(a);
			bool b_dependency = task_manager->has_dependency(b);

			if (!a_dependency && b_dependency)
				return true;

			if(a_dependency && !b_dependency)
				return false;

			return a->work_count > b->work_count;
		}
	};

	Task* tasks;
	std::vector<Task*> work_items;
	std::vector<Task*> open_list;
	std::vector<Task*> hold_area;
	std::stack<int16_t> free_tasks;

	int32_t last_id;

	pthread_t thread_ids[NUMBER_OF_THREADS];
	pthread_mutex_t mutex;
	pthread_cond_t task_reconfigure;

	static void* thread_function(void* ptr) {
		TaskManager* task_manager = (TaskManager*)ptr;

		while(true) {
			pthread_mutex_lock(&task_manager->mutex);
			task_manager->schedule_task();
			pthread_mutex_unlock(&task_manager->mutex);
		}

		return NULL;
	}

	void schedule_task() {
		while(work_items.empty())
			pthread_cond_wait(&task_reconfigure, &mutex);

		Task* task = work_items[0];
		std::pop_heap(work_items.begin(), work_items.end(), TaskManager::Comp(this));
		work_items.pop_back();

		pthread_mutex_unlock(&mutex);

		task->work_item.do_task(task->id, task->work_item);

		pthread_mutex_lock(&mutex);

		decrement_work_count(task);
	}

	bool has_dependency(Task* task) {
		Task* dependency = NULL;

		if(task->dependency != INVALID_ID)
			dependency = get_task(task->dependency);

		return dependency ? dependency->work_count > 0 : false;
	}

	Task* get_task(TaskId task_id) {
		for(size_t i = 0; i < open_list.size(); i++) {
			if(open_list[i]->id == task_id)
				return open_list[i];
		}
		return NULL;
	}

	void finalize_task(Task* task) {
		Task* parent = get_task(task->parent);
		if(parent)
			decrement_work_count(parent);

		std::vector<Task*> dependencies;
		for(size_t i = 0; i < hold_area.size(); i++) {
			Task* on_hold_task = hold_area[i];

			if(on_hold_task->dependency == task->id)
				dependencies.push_back(on_hold_task);
		}

		for(size_t i = 0; i < dependencies.size(); i++) {
			Task* dependency = dependencies[i];

			std::vector<Task*>::iterator position = std::find(hold_area.begin(), hold_area.end(), dependency);
			hold_area.erase(position);

			work_items.push_back(dependency);
			std::push_heap(work_items.begin(), work_items.end(), TaskManager::Comp(this));
		}

		std::vector<Task*>::iterator position = std::find(open_list.begin(), open_list.end(), task);
		open_list.erase(position);
		free_tasks.push(task - tasks);

		pthread_cond_broadcast(&task_reconfigure);
	}

	void decrement_work_count(Task* task) {
		task->work_count--;

		if(task->work_count == 0)
			finalize_task(task);
	}

	TaskId _begin_add(const WorkItem& work, TaskId dependency) {
		if(free_tasks.empty())
			return INVALID_ID;

		int32_t task_index = free_tasks.top();
		free_tasks.pop();

		TaskId id = last_id++;

		Task* task = &tasks[task_index];

		memset(task, 0, sizeof(Task));
		task->id = id;
		task->parent = INVALID_ID;
		task->dependency = dependency;
		task->work_count = 2;
		memcpy(&task->work_item, &work, sizeof(WorkItem));

		open_list.push_back(task);

		return id;
	}

	void _finish_add(TaskId task_id) {
		Task* task = get_task(task_id);

		task->work_count--;

		if(task->dependency == INVALID_ID) {
			work_items.push_back(task);
			std::push_heap(work_items.begin(), work_items.end(), TaskManager::Comp(this));
		} else
			hold_area.push_back(task);

		pthread_cond_broadcast(&task_reconfigure);
	}

	void _add_child(TaskId parent_id, TaskId child_id) {
		Task* parent = get_task(parent_id);
		Task* child = get_task(child_id);

		parent->work_count++;
		child->parent = parent_id;

		std::make_heap(work_items.begin(), work_items.end(), TaskManager::Comp(this));
	}

	static void empty_function(TaskId task_id, WorkItem& item) {
	}

public:
	TaskManager() {
		tasks = (Task*)malloc(sizeof(Task) * NUMBER_OF_TASKS);
		last_id = 0;

		for(int16_t i = 0; i < NUMBER_OF_TASKS; i++)
			free_tasks.push(NUMBER_OF_TASKS - i - 1);

		pthread_mutex_init(&mutex, NULL);
		pthread_cond_init(&task_reconfigure, NULL);

		for(int i = 0; i < NUMBER_OF_THREADS; i++) {
			pthread_create(&thread_ids[i], NULL, thread_function, this);
		}
	}

	~TaskManager() {
		for(int32_t i = 0; i < NUMBER_OF_THREADS; i++)
			pthread_cancel(thread_ids[i]);

		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&task_reconfigure);

		free(tasks);
	}

	TaskId add_empty() {
		WorkItem work = {empty_function};
		return begin_add(work, INVALID_ID);
	}

	TaskId add(size_t count, const WorkItem* items) {
		pthread_mutex_lock(&mutex);

		WorkItem empty_work = {empty_function};
		TaskId parent = _begin_add(empty_work, INVALID_ID);

		for(size_t i = 0; i < count; i++) {
			TaskId child = _begin_add(items[i], INVALID_ID);
			_add_child(parent, child);
			_finish_add(child);
		}

		pthread_mutex_unlock(&mutex);

		return parent;
	}

	TaskId begin_add(const WorkItem& work, TaskId dependency = INVALID_ID) {
		pthread_mutex_lock(&mutex);
		TaskId id = _begin_add(work, dependency);
		pthread_mutex_unlock(&mutex);

		return id;
	}

	void finish_add(TaskId task_id) {
		pthread_mutex_lock(&mutex);
		_finish_add(task_id);
		pthread_mutex_unlock(&mutex);
	}

	void add_child(TaskId parent_id, TaskId child_id) {
		pthread_mutex_lock(&mutex);
		_add_child(parent_id, child_id);
		pthread_mutex_unlock(&mutex);
	}

	void wait_scheduling(TaskId task_id) {
		while(true) {
			pthread_mutex_lock(&mutex);

			if(get_task(task_id) == NULL) {
				pthread_mutex_unlock(&mutex);
				break;
			}

			if(work_items.empty())
				pthread_cond_wait(&task_reconfigure, &mutex);
			else
				schedule_task();

			pthread_mutex_unlock(&mutex);
		}
	}

	void wait(TaskId task_id) {
		while(true) {
			pthread_mutex_lock(&mutex);

			if(get_task(task_id) == NULL) {
				pthread_mutex_unlock(&mutex);
				break;
			}

			if(work_items.empty())
				pthread_cond_wait(&task_reconfigure, &mutex);
			else
				schedule_task();

			pthread_mutex_unlock(&mutex);
		}
	}
};

#endif /* TASK_H_ */
