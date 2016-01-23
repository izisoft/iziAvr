#ifndef IZILIST_H_
#define IZILIST_H_

#include <IziAvr.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SIziListNode TIziListNode;

typedef struct SIziList TIziList;

struct SIziListNode
{
	TIziListNode* _prev;
	TIziListNode* _next;
	void* _value;
};

struct SIziList
{
	TIziListNode* _head;
	TIziListNode* _tail;
};

typedef TIziListNode* TIziListIter;

typedef IziBool_t (*IziListComparer_t)(void*, void*);

void iziListInsert(TIziList* list, TIziListIter iter, void* value);

void iziListRemove(TIziListIter iter);

IziSize_t iziListSize(TIziList* list);

#ifdef __cplusplus
}
#endif

#endif /* IZILIST_H_ */
