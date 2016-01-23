#ifndef TIZITASKPRIV_H_
#define TIZITASKPRIV_H_

#include <core/iziTask.h>

#ifdef __cplusplus
extern "C" {
#endif

struct SIziScoredTaskList
{
	TIziTask* _iter;
	uint8_t _taskCount;
	uint8_t _score;
};
typedef struct SIziScoredTaskList TIziScoredTaskList;

void iziActiveTaskListRemove(TIziTask* task);

void iziTaskEventListAdd(TIziTaskList* list, TIziTask* task);

void iziTaskActivate(TIziTask* task);

void iziTaskSuspend(IziDelay_t waitTicks);

void iziTaskWait();

EIziTaskPriority iziTaskWakeUpWithEvent(TIziTaskList* list);

void iziTaskFinalize();

#ifdef __cplusplus
}
#endif

#endif /* TIZITASKPRIV_H_ */
