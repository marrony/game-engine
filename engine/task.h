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
	void* data;
};

struct Task;

const int32_t NUMBER_OF_THREADS = 2;
const TaskId INVALID_ID = -1;
const pthread_t INVALID_THREAD = {0};

class TaskManager {
	struct Comp {
		TaskManager* task_manager;

		Comp(TaskManager* task_manager) : task_manager(task_manager) {
		}

		bool operator()(Task* a, Task* b) const;
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

	static void* thread_function(void* ptr);

	void schedule_task();
	bool has_dependency(Task* task);
	Task* get_task(TaskId task_id);
	void finalize_task(Task* task);
	void decrement_work_count(Task* task);
	TaskId _begin_add(const WorkItem& work, TaskId dependency, pthread_t thread);
	void _finish_add(TaskId task_id);
	void _add_child(TaskId parent_id, TaskId child_id);
public:
	TaskManager(int max_tasks);
	~TaskManager();

	TaskId add(size_t count, const WorkItem* items, TaskId dependency = INVALID_ID, pthread_t thread = INVALID_THREAD);
	TaskId add(const WorkItem& item, TaskId dependency = INVALID_ID, pthread_t thread = INVALID_THREAD);

	TaskId begin_add_empty(TaskId dependency = INVALID_ID, pthread_t thread = INVALID_THREAD);
	TaskId begin_add(const WorkItem& work, TaskId dependency = INVALID_ID, pthread_t thread = INVALID_THREAD);

	void add_child(TaskId parent_id, TaskId child_id);

	void finish_add(TaskId task_id);

	void wait(TaskId task_id);
};

#endif /* TASK_H_ */
