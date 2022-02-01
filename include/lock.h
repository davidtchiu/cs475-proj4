/* lock.h */
#define	NLOCK	10

/* Lock state definitions */
#define LOCK_FREE  0               /* lock table entry is available   */
#define LOCK_USED  1               /* lock table entry is in use      */

/* Lock table entry */
struct  lockentry
{
	byte	state;			/* whether entry is LOCK_FREE or LOCK_USED    */
	mutex_t	lock;			/* lock */
	struct	queue	*wait_queue;	/* queue of waiting processes */
};

extern	struct	lockentry locktab[];

#define isbadlock(m)     ((int32)(m) < 0 || (m) >= NLOCK)
