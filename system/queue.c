/* queue.c - enqueue, dequeue, isempty, nonempty, et al. */

#include <xinu.h>
#include <stdlib.h>

/**
 * Prints out contents of a queue
 * @param q	pointer to a queue
 */
void printqueue(struct queue *q)
{
	kprintf("[");
	if (nonempty(q))
	{
		struct qentry *curr = q->head;
		while (curr != NULL)
		{
			kprintf("(key=%d, pid=%u)", curr->key, curr->pid);
			curr = curr->next;
		}
		kprintf(" queuesize=%u", q->size);
	}
	kprintf("]\n");
}

/**
 * Checks whether queue is empty
 * @param q	Pointer to a queue
 * @return 1 if true, 0 otherwise
 */
bool8 isempty(struct queue *q)
{
	return (q->size == 0);
}

/**
 * Checks whether queue is nonempty
 * @param q	Pointer to a queue
 * @return 1 if true, 0 otherwise
 */
bool8 nonempty(struct queue *q)
{
	return (q == NULL || q->size > 0);
}

/**
 * Checks whether queue is full
 * @param q	Pointer to a queue
 * @return 1 if true, 0 otherwise
 */
bool8 isfull(struct queue *q)
{
	return (q->size >= NPROC);
}

/**
 * Insert a process into a queue in descending key order
 * @param pid	ID of process to insert
 * @param q	Pointer to the queue to use
 * @param key	key for the inserted process
 */
pid32 enqueue(pid32 pid, struct queue *q, int32 key)
{
	if (isfull(q))
	{
		return SYSERR;
	}

	// create entry
	struct qentry *newEntry = (struct qentry *)malloc(sizeof(struct qentry));
	newEntry->pid = pid;
	newEntry->key = key;
	newEntry->next = NULL;
	newEntry->prev = NULL;

	// find insert location
	struct qentry *currEntry; // runs through items in a queue
	struct qentry *prevEntry; // holds previous entry

	currEntry = q->head;
	while (currEntry != NULL && currEntry->key >= key)
		currEntry = currEntry->next;

	if (currEntry == NULL) // goes in the tail of the queue
		prevEntry = q->tail;
	else
		prevEntry = currEntry->prev;

	// insert process between curr node and previous node
	newEntry->prev = prevEntry;
	newEntry->next = currEntry;

	if (prevEntry != NULL)
		prevEntry->next = newEntry;
	if (currEntry != NULL)
		currEntry->prev = newEntry;

	// update queue
	if (NULL == prevEntry) // new head
		q->head = newEntry;
	if (NULL == currEntry) // new tail
		q->tail = newEntry;
	q->size++; // increment size

	return OK;
}

/**
 * Remove and return the first process on a list
 * @param q	Pointer to the queue to use
 * @return pid of the process removed, or EMPTY if queue is empty
 */
pid32 dequeue(struct queue *q)
{
	pid32 pid; // ID of process removed

	if (isempty(q))
	{
		return EMPTY;
	}

	// save pointer to head entry
	struct qentry *head = q->head;
	struct qentry *newHead = head->next;

	// save pid of head entry
	pid = head->pid;

	// unlink head from queue
	if (newHead != NULL)
		newHead->prev = NULL;
	else
		q->tail = NULL;

	// update queue to point head pointer at newhead
	q->head = newHead;

	// decrement size of queue
	q->size--;

	// deallocate head entry
	free(head, sizeof(struct qentry));

	return pid;
}

/**
 * Finds a queue entry given pid
 * @param pid	a process ID
 * @param q	a pointer to a queue
 * @return pointer to the entry if found, NULL otherwise
 */
struct qentry *getbypid(pid32 pid, struct queue *q)
{
	if (isempty(q))
		return NULL;

	struct qentry *currEntry = q->head;
	while (currEntry != NULL && currEntry->pid != pid)
		currEntry = currEntry->next;

	return currEntry;
}

/**
 * Remove a process from the front of a queue (pid assumed valid with no check)
 * @param q	pointer to a queue
 * @return pid on success, EMPTY if queue is empty
 */
pid32 getfirst(struct queue *q)
{
	if (isempty(q))
	{
		return EMPTY;
	}

	return dequeue(q);
}

/**
 * Remove a process from the end of a queue (pid assumed valid with no check)
 * @param q	Pointer to the queue
 * @return pid on success, EMPTY otherwise
 */
pid32 getlast(struct queue *q)
{
	if (isempty(q))
	{
		return EMPTY;
	}

	return remove(q->tail->pid, q);
}

/**
 * Remove a process from an arbitrary point in a queue
 * @param pid	ID of process to remove
 * @param q	Pointer to the queue
 * @return pid on success, SYSERR if pid is not found
 */
pid32 remove(pid32 pid, struct queue *q)
{
	// TODO - return EMPTY if queue is empty
	if (isempty(q))
	{
		return EMPTY;
	}

	// find the entry with pid
	struct qentry *currEntry = q->head;
	while (currEntry != NULL)
	{
		// found it!
		if (currEntry->pid == pid)
		{
			// unlink: find next and prev entries
			struct qentry *next = currEntry->next;
			struct qentry *prev = currEntry->prev;
			if (next != NULL)
				next->prev = prev;
			if (prev != NULL)
				prev->next = next;

			// update queue structure
			if (currEntry == q->head)
				q->head = next;
			if (currEntry == q->tail)
				q->tail = prev;
			q->size--;

			// deallocate current entry
			free(currEntry, sizeof(struct qentry));
			return pid;
		}
		currEntry = currEntry->next;
	}

	// TODO - if pid does not exist in the queue, return SYSERR
	return SYSERR;
}
