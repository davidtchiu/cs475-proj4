/* newqueue.c - newqueue */

#include <xinu.h>
#include <stdlib.h>

/**
 * Allocate and initialize a queue
 * @return pointer to queue structure
 */
struct queue *newqueue(void)
{
	struct queue *newQueue = (struct queue*) malloc(sizeof(struct queue));

	newQueue->head = NULL;
	newQueue->tail = NULL;
	newQueue->size = 0;

	return newQueue;
}
