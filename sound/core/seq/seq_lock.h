#ifndef __SND_SEQ_LOCK_H
#define __SND_SEQ_LOCK_H

#include <linux/sched.h>

<<<<<<< HEAD
#if defined(CONFIG_SMP) || defined(CONFIG_SND_DEBUG)

=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
typedef atomic_t snd_use_lock_t;

/* initialize lock */
#define snd_use_lock_init(lockp) atomic_set(lockp, 0)

/* increment lock */
#define snd_use_lock_use(lockp) atomic_inc(lockp)

/* release lock */
#define snd_use_lock_free(lockp) atomic_dec(lockp)

/* wait until all locks are released */
void snd_use_lock_sync_helper(snd_use_lock_t *lock, const char *file, int line);
#define snd_use_lock_sync(lockp) snd_use_lock_sync_helper(lockp, __BASE_FILE__, __LINE__)

<<<<<<< HEAD
#else /* SMP || CONFIG_SND_DEBUG */

typedef spinlock_t snd_use_lock_t;	/* dummy */
#define snd_use_lock_init(lockp) /**/
#define snd_use_lock_use(lockp) /**/
#define snd_use_lock_free(lockp) /**/
#define snd_use_lock_sync(lockp) /**/

#endif /* SMP || CONFIG_SND_DEBUG */

=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#endif /* __SND_SEQ_LOCK_H */
