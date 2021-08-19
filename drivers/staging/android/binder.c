/* binder.c
 *
 * Android IPC Subsystem
 *
 * Copyright (C) 2007-2008 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
<<<<<<< HEAD
#define DEBUG 1
=======

/*
 * Locking overview
 *
 * There are 3 main spinlocks which must be acquired in the
 * order shown:
 *
 * 1) proc->outer_lock : protects binder_ref
 *    binder_proc_lock() and binder_proc_unlock() are
 *    used to acq/rel.
 * 2) node->lock : protects most fields of binder_node.
 *    binder_node_lock() and binder_node_unlock() are
 *    used to acq/rel
 * 3) proc->inner_lock : protects the thread and node lists
 *    (proc->threads, proc->waiting_threads, proc->nodes)
 *    and all todo lists associated with the binder_proc
 *    (proc->todo, thread->todo, proc->delivered_death and
 *    node->async_todo), as well as thread->transaction_stack
 *    binder_inner_proc_lock() and binder_inner_proc_unlock()
 *    are used to acq/rel
 *
 * Any lock under procA must never be nested under any lock at the same
 * level or below on procB.
 *
 * Functions that require a lock held on entry indicate which lock
 * in the suffix of the function name:
 *
 * foo_olocked() : requires node->outer_lock
 * foo_nlocked() : requires node->lock
 * foo_ilocked() : requires proc->inner_lock
 * foo_oilocked(): requires proc->outer_lock and proc->inner_lock
 * foo_nilocked(): requires node->lock and proc->inner_lock
 * ...
 */

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <asm/cacheflush.h>
#include <linux/fdtable.h>
#include <linux/file.h>
#include <linux/freezer.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/miscdevice.h>
<<<<<<< HEAD
#include <linux/mm.h>
=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/nsproxy.h>
#include <linux/poll.h>
#include <linux/debugfs.h>
#include <linux/rbtree.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
<<<<<<< HEAD
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/pid_namespace.h>
#include <linux/security.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/rtc.h>
#include <mt-plat/aee.h>

#ifdef CONFIG_MT_PRIO_TRACER
#include <linux/prio_tracer.h>
#endif

#include "binder.h"
#include "binder_trace.h"

static DEFINE_MUTEX(binder_main_lock);
static DEFINE_MUTEX(binder_deferred_lock);
static DEFINE_MUTEX(binder_mmap_lock);

static HLIST_HEAD(binder_procs);
static HLIST_HEAD(binder_deferred_list);
static HLIST_HEAD(binder_dead_nodes);

static struct dentry *binder_debugfs_dir_entry_root;
static struct dentry *binder_debugfs_dir_entry_proc;
static struct binder_node *binder_context_mgr_node;
static kuid_t binder_context_mgr_uid = INVALID_UID;
static int binder_last_id;
static struct workqueue_struct *binder_deferred_workqueue;

#define RT_PRIO_INHERIT			"v1.7"
#ifdef RT_PRIO_INHERIT
#include <linux/sched/rt.h>
#endif

#define MTK_BINDER_DEBUG        "v0.1"	/* defined for mtk internal added debug code */

/*****************************************************************************************************/
/*	MTK Death Notify	|                                               */
/*	Debug Log Prefix	|	Description                                 */
/*	---------------------------------------------------------------------               */
/*	[DN #1]			|	Some one requests Death Notify from upper layer.                */
/*	[DN #2]			|	Some one cancels Death Notify from upper layer.                 */
/*	[DN #3]			|	Binder Driver sends Death Notify to all requesters' Binder Thread.      */
/*	[DN #4]			|	Some requester's binder_thread_read() handles Death Notify works.       */
/*	[DN #5]			|	Some requester sends confirmation to Binder Driver. (In IPCThreadState.cpp)*/
/*	[DN #6]			|	Finally receive requester's confirmation from upper layer.      */
/******************************************************************************************************/
#define MTK_DEATH_NOTIFY_MONITOR	"v0.1"

/**
 * Revision history of binder monitor
 *
 * v0.1   - enhance debug log
 * v0.2   - transaction timeout log
 * v0.2.1 - buffer allocation debug
 */
#ifdef CONFIG_MT_ENG_BUILD
#define BINDER_MONITOR			"v0.2.1"	/* BINDER_MONITOR only turn on for eng build */
#endif

#ifdef BINDER_MONITOR
#define MAX_SERVICE_NAME_LEN		32
/*******************************************************************************************************/
/*	Payload layout of addService():                                         */
/*		| Parcel header | IServiceManager.descriptor | Parcel header | Service name | ...               */
/*	(Please refer ServiceManagerNative.java:addService())                   */
/*	IServiceManager.descriptor is 'android.os.IServiceManager' interleaved with character '\0'.         */
/*	that is, 'a', '\0', 'n', '\0', 'd', '\0', 'r', '\0', 'o', ...           */
/*	so the offset of Service name = Parcel header x2 + strlen(android.os.IServiceManager) x2 = 8x2 + 26x2 = 68*/
/*******************************************************************************************************/
#define MAGIC_SERVICE_NAME_OFFSET	68

#define MAX_ENG_TRANS_LOG_BUFF_LEN	10240
static pid_t system_server_pid;
static int binder_check_buf_pid;
static int binder_check_buf_tid;
static unsigned long binder_log_level;
char aee_msg[512];
char aee_word[100];
#define TRANS_LOG_LEN 210
char large_msg[TRANS_LOG_LEN];

#define BINDER_PERF_EVAL		"V0.1"
#endif

=======
#include <linux/pid_namespace.h>
#include <linux/security.h>
#include <linux/spinlock.h>

#include "binder.h"
#include "binder_alloc.h"
#include "binder_trace.h"

static HLIST_HEAD(binder_deferred_list);
static DEFINE_MUTEX(binder_deferred_lock);

static HLIST_HEAD(binder_devices);
static HLIST_HEAD(binder_procs);
static DEFINE_MUTEX(binder_procs_lock);

static HLIST_HEAD(binder_dead_nodes);
static DEFINE_SPINLOCK(binder_dead_nodes_lock);

static struct dentry *binder_debugfs_dir_entry_root;
static struct dentry *binder_debugfs_dir_entry_proc;
static atomic_t binder_last_id;
static struct workqueue_struct *binder_deferred_workqueue;

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#define BINDER_DEBUG_ENTRY(name) \
static int binder_##name##_open(struct inode *inode, struct file *file) \
{ \
	return single_open(file, binder_##name##_show, inode->i_private); \
} \
\
static const struct file_operations binder_##name##_fops = { \
	.owner = THIS_MODULE, \
	.open = binder_##name##_open, \
	.read = seq_read, \
	.llseek = seq_lseek, \
	.release = single_release, \
}

<<<<<<< HEAD
#ifdef BINDER_MONITOR
#define BINDER_DEBUG_SETTING_ENTRY(name) \
static int binder_##name##_open(struct inode *inode, struct file *file) \
{ \
	return single_open(file, binder_##name##_show, inode->i_private); \
} \
\
static const struct file_operations binder_##name##_fops = { \
	.owner = THIS_MODULE, \
	.open = binder_##name##_open, \
	.read = seq_read, \
	.write = binder_##name##_write, \
	.llseek = seq_lseek, \
	.release = single_release, \
}
#endif

/*LCH add, for binder pages leakage debug*/
#ifdef CONFIG_MT_ENG_BUILD
#define MTK_BINDER_PAGE_USED_RECORD
#endif

#ifdef MTK_BINDER_PAGE_USED_RECORD
static unsigned int binder_page_used;
static unsigned int binder_page_used_peak;
#endif

=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
static int binder_proc_show(struct seq_file *m, void *unused);
BINDER_DEBUG_ENTRY(proc);

/* This is only defined in include/asm-arm/sizes.h */
#ifndef SZ_1K
#define SZ_1K                               0x400
#endif

#ifndef SZ_4M
#define SZ_4M                               0x400000
#endif

#define FORBIDDEN_MMAP_FLAGS                (VM_WRITE)

#define BINDER_SMALL_BUF_SIZE (PAGE_SIZE * 64)

enum {
<<<<<<< HEAD
	BINDER_DEBUG_USER_ERROR = 1U << 0,
	BINDER_DEBUG_FAILED_TRANSACTION = 1U << 1,
	BINDER_DEBUG_DEAD_TRANSACTION = 1U << 2,
	BINDER_DEBUG_OPEN_CLOSE = 1U << 3,
	BINDER_DEBUG_DEAD_BINDER = 1U << 4,
	BINDER_DEBUG_DEATH_NOTIFICATION = 1U << 5,
	BINDER_DEBUG_READ_WRITE = 1U << 6,
	BINDER_DEBUG_USER_REFS = 1U << 7,
	BINDER_DEBUG_THREADS = 1U << 8,
	BINDER_DEBUG_TRANSACTION = 1U << 9,
	BINDER_DEBUG_TRANSACTION_COMPLETE = 1U << 10,
	BINDER_DEBUG_FREE_BUFFER = 1U << 11,
	BINDER_DEBUG_INTERNAL_REFS = 1U << 12,
	BINDER_DEBUG_BUFFER_ALLOC = 1U << 13,
	BINDER_DEBUG_PRIORITY_CAP = 1U << 14,
	BINDER_DEBUG_BUFFER_ALLOC_ASYNC = 1U << 15,
};
static uint32_t binder_debug_mask = BINDER_DEBUG_USER_ERROR |
BINDER_DEBUG_FAILED_TRANSACTION | BINDER_DEBUG_DEAD_TRANSACTION;
module_param_named(debug_mask, binder_debug_mask, uint, S_IWUSR | S_IRUGO);

static bool binder_debug_no_lock;
module_param_named(proc_no_lock, binder_debug_no_lock, bool, S_IWUSR | S_IRUGO);
=======
	BINDER_DEBUG_USER_ERROR             = 1U << 0,
	BINDER_DEBUG_FAILED_TRANSACTION     = 1U << 1,
	BINDER_DEBUG_DEAD_TRANSACTION       = 1U << 2,
	BINDER_DEBUG_OPEN_CLOSE             = 1U << 3,
	BINDER_DEBUG_DEAD_BINDER            = 1U << 4,
	BINDER_DEBUG_DEATH_NOTIFICATION     = 1U << 5,
	BINDER_DEBUG_READ_WRITE             = 1U << 6,
	BINDER_DEBUG_USER_REFS              = 1U << 7,
	BINDER_DEBUG_THREADS                = 1U << 8,
	BINDER_DEBUG_TRANSACTION            = 1U << 9,
	BINDER_DEBUG_TRANSACTION_COMPLETE   = 1U << 10,
	BINDER_DEBUG_FREE_BUFFER            = 1U << 11,
	BINDER_DEBUG_INTERNAL_REFS          = 1U << 12,
	BINDER_DEBUG_PRIORITY_CAP           = 1U << 13,
	BINDER_DEBUG_SPINLOCKS              = 1U << 14,
};
static uint32_t binder_debug_mask = BINDER_DEBUG_USER_ERROR |
	BINDER_DEBUG_FAILED_TRANSACTION | BINDER_DEBUG_DEAD_TRANSACTION;
module_param_named(debug_mask, binder_debug_mask, uint, S_IWUSR | S_IRUGO);

static char *binder_devices_param = CONFIG_ANDROID_BINDER_DEVICES;
module_param_named(devices, binder_devices_param, charp, S_IRUGO);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

static DECLARE_WAIT_QUEUE_HEAD(binder_user_error_wait);
static int binder_stop_on_user_error;

<<<<<<< HEAD
static int binder_set_stop_on_user_error(const char *val, struct kernel_param *kp)
=======
static int binder_set_stop_on_user_error(const char *val,
					 struct kernel_param *kp)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	int ret;

	ret = param_set_int(val, kp);
<<<<<<< HEAD

=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	if (binder_stop_on_user_error < 2)
		wake_up(&binder_user_error_wait);
	return ret;
}
<<<<<<< HEAD

=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
module_param_call(stop_on_user_error, binder_set_stop_on_user_error,
	param_get_int, &binder_stop_on_user_error, S_IWUSR | S_IRUGO);

#define binder_debug(mask, x...) \
	do { \
		if (binder_debug_mask & mask) \
			pr_info(x); \
	} while (0)

<<<<<<< HEAD
#ifdef BINDER_MONITOR
#define binder_user_error(x...) \
	do { \
		if (binder_debug_mask & BINDER_DEBUG_USER_ERROR) \
			pr_err(x); \
		if (binder_stop_on_user_error) \
			binder_stop_on_user_error = 2; \
	} while (0)
#else
=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#define binder_user_error(x...) \
	do { \
		if (binder_debug_mask & BINDER_DEBUG_USER_ERROR) \
			pr_info(x); \
		if (binder_stop_on_user_error) \
			binder_stop_on_user_error = 2; \
	} while (0)
<<<<<<< HEAD
#endif
=======

#define to_flat_binder_object(hdr) \
	container_of(hdr, struct flat_binder_object, hdr)

#define to_binder_fd_object(hdr) container_of(hdr, struct binder_fd_object, hdr)

#define to_binder_buffer_object(hdr) \
	container_of(hdr, struct binder_buffer_object, hdr)

#define to_binder_fd_array_object(hdr) \
	container_of(hdr, struct binder_fd_array_object, hdr)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

enum binder_stat_types {
	BINDER_STAT_PROC,
	BINDER_STAT_THREAD,
	BINDER_STAT_NODE,
	BINDER_STAT_REF,
	BINDER_STAT_DEATH,
	BINDER_STAT_TRANSACTION,
	BINDER_STAT_TRANSACTION_COMPLETE,
	BINDER_STAT_COUNT
};

struct binder_stats {
<<<<<<< HEAD
	int br[_IOC_NR(BR_FAILED_REPLY) + 1];
	int bc[_IOC_NR(BC_DEAD_BINDER_DONE) + 1];
	int obj_created[BINDER_STAT_COUNT];
	int obj_deleted[BINDER_STAT_COUNT];
=======
	atomic_t br[_IOC_NR(BR_FAILED_REPLY) + 1];
	atomic_t bc[_IOC_NR(BC_REPLY_SG) + 1];
	atomic_t obj_created[BINDER_STAT_COUNT];
	atomic_t obj_deleted[BINDER_STAT_COUNT];
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
};

static struct binder_stats binder_stats;

static inline void binder_stats_deleted(enum binder_stat_types type)
{
<<<<<<< HEAD
	binder_stats.obj_deleted[type]++;
=======
	atomic_inc(&binder_stats.obj_deleted[type]);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
}

static inline void binder_stats_created(enum binder_stat_types type)
{
<<<<<<< HEAD
	binder_stats.obj_created[type]++;
=======
	atomic_inc(&binder_stats.obj_created[type]);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
}

struct binder_transaction_log_entry {
	int debug_id;
<<<<<<< HEAD
=======
	int debug_id_done;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	int call_type;
	int from_proc;
	int from_thread;
	int target_handle;
	int to_proc;
	int to_thread;
	int to_node;
	int data_size;
	int offsets_size;
<<<<<<< HEAD
#ifdef BINDER_MONITOR
	unsigned int code;
	struct timespec timestamp;
	char service[MAX_SERVICE_NAME_LEN];
	int fd;
	struct timeval tv;
	struct timespec readstamp;
	struct timespec endstamp;
#endif
};
struct binder_transaction_log {
	int next;
	int full;
#ifdef BINDER_MONITOR
	unsigned size;
	struct binder_transaction_log_entry *entry;
#else
	struct binder_transaction_log_entry entry[32];
#endif
=======
	int return_error_line;
	uint32_t return_error;
	uint32_t return_error_param;
	const char *context_name;
};
struct binder_transaction_log {
	atomic_t cur;
	bool full;
	struct binder_transaction_log_entry entry[32];
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
};
static struct binder_transaction_log binder_transaction_log;
static struct binder_transaction_log binder_transaction_log_failed;

static struct binder_transaction_log_entry *binder_transaction_log_add(
	struct binder_transaction_log *log)
{
	struct binder_transaction_log_entry *e;
<<<<<<< HEAD

	e = &log->entry[log->next];
	memset(e, 0, sizeof(*e));
	log->next++;
#ifdef BINDER_MONITOR
	if (log->next == log->size) {
		log->next = 0;
		log->full = 1;
	}
#else
	if (log->next == ARRAY_SIZE(log->entry)) {
		log->next = 0;
		log->full = 1;
	}
#endif
	return e;
}

#ifdef BINDER_MONITOR
static struct binder_transaction_log_entry entry_failed[32];

/* log_disable bitmap
 * bit: 31...43210
 *       |   |||||_ 0: log enable / 1: log disable
 *       |   ||||__ 1: self resume
 *       |   |||____2: manually trigger kernel warning for buffer allocation
 *       |   ||____ 3: 1:rt_inherit log enable / 0: rt_inherit log disable
 *       |   |
 */
static int log_disable;
#define BINDER_LOG_RESUME	0x2
#define BINDER_BUF_WARN		0x4
#ifdef RT_PRIO_INHERIT
#define BINDER_RT_LOG_ENABLE	0x8
#endif
#ifdef CONFIG_MTK_EXTMEM
#include <linux/exm_driver.h>
#else
static struct binder_transaction_log_entry entry_t[MAX_ENG_TRANS_LOG_BUFF_LEN];
#endif
#endif
struct binder_work {
	struct list_head entry;
	enum {
		BINDER_WORK_TRANSACTION = 1,
		BINDER_WORK_TRANSACTION_COMPLETE,
=======
	unsigned int cur = atomic_inc_return(&log->cur);

	if (cur >= ARRAY_SIZE(log->entry))
		log->full = 1;
	e = &log->entry[cur % ARRAY_SIZE(log->entry)];
	WRITE_ONCE(e->debug_id_done, 0);
	/*
	 * write-barrier to synchronize access to e->debug_id_done.
	 * We make sure the initialized 0 value is seen before
	 * memset() other fields are zeroed by memset.
	 */
	smp_wmb();
	memset(e, 0, sizeof(*e));
	return e;
}

struct binder_context {
	struct binder_node *binder_context_mgr_node;
	struct mutex context_mgr_node_lock;

	kuid_t binder_context_mgr_uid;
	const char *name;
};

struct binder_device {
	struct hlist_node hlist;
	struct miscdevice miscdev;
	struct binder_context context;
};

/**
 * struct binder_work - work enqueued on a worklist
 * @entry:             node enqueued on list
 * @type:              type of work to be performed
 *
 * There are separate work lists for proc, thread, and node (async).
 */
struct binder_work {
	struct list_head entry;

	enum {
		BINDER_WORK_TRANSACTION = 1,
		BINDER_WORK_TRANSACTION_COMPLETE,
		BINDER_WORK_RETURN_ERROR,
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		BINDER_WORK_NODE,
		BINDER_WORK_DEAD_BINDER,
		BINDER_WORK_DEAD_BINDER_AND_CLEAR,
		BINDER_WORK_CLEAR_DEATH_NOTIFICATION,
	} type;
};

<<<<<<< HEAD
struct binder_node {
	int debug_id;
=======
struct binder_error {
	struct binder_work work;
	uint32_t cmd;
};

/**
 * struct binder_node - binder node bookkeeping
 * @debug_id:             unique ID for debugging
 *                        (invariant after initialized)
 * @lock:                 lock for node fields
 * @work:                 worklist element for node work
 *                        (protected by @proc->inner_lock)
 * @rb_node:              element for proc->nodes tree
 *                        (protected by @proc->inner_lock)
 * @dead_node:            element for binder_dead_nodes list
 *                        (protected by binder_dead_nodes_lock)
 * @proc:                 binder_proc that owns this node
 *                        (invariant after initialized)
 * @refs:                 list of references on this node
 *                        (protected by @lock)
 * @internal_strong_refs: used to take strong references when
 *                        initiating a transaction
 *                        (protected by @proc->inner_lock if @proc
 *                        and by @lock)
 * @local_weak_refs:      weak user refs from local process
 *                        (protected by @proc->inner_lock if @proc
 *                        and by @lock)
 * @local_strong_refs:    strong user refs from local process
 *                        (protected by @proc->inner_lock if @proc
 *                        and by @lock)
 * @tmp_refs:             temporary kernel refs
 *                        (protected by @proc->inner_lock while @proc
 *                        is valid, and by binder_dead_nodes_lock
 *                        if @proc is NULL. During inc/dec and node release
 *                        it is also protected by @lock to provide safety
 *                        as the node dies and @proc becomes NULL)
 * @ptr:                  userspace pointer for node
 *                        (invariant, no lock needed)
 * @cookie:               userspace cookie for node
 *                        (invariant, no lock needed)
 * @has_strong_ref:       userspace notified of strong ref
 *                        (protected by @proc->inner_lock if @proc
 *                        and by @lock)
 * @pending_strong_ref:   userspace has acked notification of strong ref
 *                        (protected by @proc->inner_lock if @proc
 *                        and by @lock)
 * @has_weak_ref:         userspace notified of weak ref
 *                        (protected by @proc->inner_lock if @proc
 *                        and by @lock)
 * @pending_weak_ref:     userspace has acked notification of weak ref
 *                        (protected by @proc->inner_lock if @proc
 *                        and by @lock)
 * @has_async_transaction: async transaction to node in progress
 *                        (protected by @lock)
 * @sched_policy:         minimum scheduling policy for node
 *                        (invariant after initialized)
 * @accept_fds:           file descriptor operations supported for node
 *                        (invariant after initialized)
 * @min_priority:         minimum scheduling priority
 *                        (invariant after initialized)
 * @inherit_rt:           inherit RT scheduling policy from caller
 * @txn_security_ctx:     require sender's security context
 *                        (invariant after initialized)
 * @async_todo:           list of async work items
 *                        (protected by @proc->inner_lock)
 *
 * Bookkeeping structure for binder nodes.
 */
struct binder_node {
	int debug_id;
	spinlock_t lock;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	struct binder_work work;
	union {
		struct rb_node rb_node;
		struct hlist_node dead_node;
	};
	struct binder_proc *proc;
	struct hlist_head refs;
	int internal_strong_refs;
	int local_weak_refs;
	int local_strong_refs;
<<<<<<< HEAD
	binder_uintptr_t ptr;
	binder_uintptr_t cookie;
	unsigned has_strong_ref:1;
	unsigned pending_strong_ref:1;
	unsigned has_weak_ref:1;
	unsigned pending_weak_ref:1;
	unsigned has_async_transaction:1;
	unsigned accept_fds:1;
	unsigned min_priority:8;
	struct list_head async_todo;
#ifdef BINDER_MONITOR
	char name[MAX_SERVICE_NAME_LEN];
#endif
#ifdef MTK_BINDER_DEBUG
	int async_pid;
#endif
};

struct binder_ref_death {
=======
	int tmp_refs;
	binder_uintptr_t ptr;
	binder_uintptr_t cookie;
	struct {
		/*
		 * bitfield elements protected by
		 * proc inner_lock
		 */
		u8 has_strong_ref:1;
		u8 pending_strong_ref:1;
		u8 has_weak_ref:1;
		u8 pending_weak_ref:1;
	};
	struct {
		/*
		 * invariant after initialization
		 */
		u8 sched_policy:2;
		u8 inherit_rt:1;
		u8 accept_fds:1;
		u8 txn_security_ctx:1;
		u8 min_priority;
	};
	bool has_async_transaction;
	struct list_head async_todo;
};

struct binder_ref_death {
	/**
	 * @work: worklist element for death notifications
	 *        (protected by inner_lock of the proc that
	 *        this ref belongs to)
	 */
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	struct binder_work work;
	binder_uintptr_t cookie;
};

<<<<<<< HEAD
=======
/**
 * struct binder_ref_data - binder_ref counts and id
 * @debug_id:        unique ID for the ref
 * @desc:            unique userspace handle for ref
 * @strong:          strong ref count (debugging only if not locked)
 * @weak:            weak ref count (debugging only if not locked)
 *
 * Structure to hold ref count and ref id information. Since
 * the actual ref can only be accessed with a lock, this structure
 * is used to return information about the ref to callers of
 * ref inc/dec functions.
 */
struct binder_ref_data {
	int debug_id;
	uint32_t desc;
	int strong;
	int weak;
};

/**
 * struct binder_ref - struct to track references on nodes
 * @data:        binder_ref_data containing id, handle, and current refcounts
 * @rb_node_desc: node for lookup by @data.desc in proc's rb_tree
 * @rb_node_node: node for lookup by @node in proc's rb_tree
 * @node_entry:  list entry for node->refs list in target node
 *               (protected by @node->lock)
 * @proc:        binder_proc containing ref
 * @node:        binder_node of target node. When cleaning up a
 *               ref for deletion in binder_cleanup_ref, a non-NULL
 *               @node indicates the node must be freed
 * @death:       pointer to death notification (ref_death) if requested
 *               (protected by @node->lock)
 *
 * Structure to track references from procA to target node (on procB). This
 * structure is unsafe to access without holding @proc->outer_lock.
 */
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
struct binder_ref {
	/* Lookups needed: */
	/*   node + proc => ref (transaction) */
	/*   desc + proc => ref (transaction, inc/dec ref) */
	/*   node => refs + procs (proc exit) */
<<<<<<< HEAD
	int debug_id;
=======
	struct binder_ref_data data;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	struct rb_node rb_node_desc;
	struct rb_node rb_node_node;
	struct hlist_node node_entry;
	struct binder_proc *proc;
	struct binder_node *node;
<<<<<<< HEAD
	uint32_t desc;
	int strong;
	int weak;
	struct binder_ref_death *death;
};

struct binder_buffer {
	struct list_head entry;	/* free and allocated entries by address */
	struct rb_node rb_node;	/* free entry by size or allocated entry */
							/* by address */
	unsigned free:1;
	unsigned allow_user_free:1;
	unsigned async_transaction:1;
	unsigned debug_id:29;

	struct binder_transaction *transaction;
#ifdef BINDER_MONITOR
	struct binder_transaction_log_entry *log_entry;
#endif
	struct binder_node *target_node;
	size_t data_size;
	size_t offsets_size;
	uint8_t data[0];
};

enum binder_deferred_state {
	BINDER_DEFERRED_PUT_FILES	= 0x01,
	BINDER_DEFERRED_FLUSH		= 0x02,
	BINDER_DEFERRED_RELEASE		= 0x04,
};

#ifdef BINDER_MONITOR
enum wait_on_reason {
	WAIT_ON_NONE = 0U,
	WAIT_ON_READ = 1U,
	WAIT_ON_EXEC = 2U,
	WAIT_ON_REPLY_READ = 3U
};
#endif

=======
	struct binder_ref_death *death;
};

enum binder_deferred_state {
	BINDER_DEFERRED_PUT_FILES    = 0x01,
	BINDER_DEFERRED_FLUSH        = 0x02,
	BINDER_DEFERRED_RELEASE      = 0x04,
};

/**
 * struct binder_priority - scheduler policy and priority
 * @sched_policy            scheduler policy
 * @prio                    [100..139] for SCHED_NORMAL, [0..99] for FIFO/RT
 *
 * The binder driver supports inheriting the following scheduler policies:
 * SCHED_NORMAL
 * SCHED_BATCH
 * SCHED_FIFO
 * SCHED_RR
 */
struct binder_priority {
	unsigned int sched_policy;
	int prio;
};

/**
 * struct binder_proc - binder process bookkeeping
 * @proc_node:            element for binder_procs list
 * @threads:              rbtree of binder_threads in this proc
 *                        (protected by @inner_lock)
 * @nodes:                rbtree of binder nodes associated with
 *                        this proc ordered by node->ptr
 *                        (protected by @inner_lock)
 * @refs_by_desc:         rbtree of refs ordered by ref->desc
 *                        (protected by @outer_lock)
 * @refs_by_node:         rbtree of refs ordered by ref->node
 *                        (protected by @outer_lock)
 * @waiting_threads:      threads currently waiting for proc work
 *                        (protected by @inner_lock)
 * @pid                   PID of group_leader of process
 *                        (invariant after initialized)
 * @tsk                   task_struct for group_leader of process
 *                        (invariant after initialized)
 * @files                 files_struct for process
 *                        (protected by @files_lock)
 * @files_lock            mutex to protect @files
 * @deferred_work_node:   element for binder_deferred_list
 *                        (protected by binder_deferred_lock)
 * @deferred_work:        bitmap of deferred work to perform
 *                        (protected by binder_deferred_lock)
 * @is_dead:              process is dead and awaiting free
 *                        when outstanding transactions are cleaned up
 *                        (protected by @inner_lock)
 * @todo:                 list of work for this process
 *                        (protected by @inner_lock)
 * @stats:                per-process binder statistics
 *                        (atomics, no lock needed)
 * @delivered_death:      list of delivered death notification
 *                        (protected by @inner_lock)
 * @max_threads:          cap on number of binder threads
 *                        (protected by @inner_lock)
 * @requested_threads:    number of binder threads requested but not
 *                        yet started. In current implementation, can
 *                        only be 0 or 1.
 *                        (protected by @inner_lock)
 * @requested_threads_started: number binder threads started
 *                        (protected by @inner_lock)
 * @tmp_ref:              temporary reference to indicate proc is in use
 *                        (protected by @inner_lock)
 * @default_priority:     default scheduler priority
 *                        (invariant after initialized)
 * @debugfs_entry:        debugfs node
 * @alloc:                binder allocator bookkeeping
 * @context:              binder_context for this proc
 *                        (invariant after initialized)
 * @inner_lock:           can nest under outer_lock and/or node lock
 * @outer_lock:           no nesting under innor or node lock
 *                        Lock order: 1) outer, 2) node, 3) inner
 *
 * Bookkeeping structure for binder processes
 */
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
struct binder_proc {
	struct hlist_node proc_node;
	struct rb_root threads;
	struct rb_root nodes;
	struct rb_root refs_by_desc;
	struct rb_root refs_by_node;
<<<<<<< HEAD
	int pid;
	struct vm_area_struct *vma;
	struct mm_struct *vma_vm_mm;
	struct task_struct *tsk;
	struct files_struct *files;
	struct hlist_node deferred_work_node;
	int deferred_work;
	void *buffer;
	ptrdiff_t user_buffer_offset;

	struct list_head buffers;
	struct rb_root free_buffers;
	struct rb_root allocated_buffers;
	size_t free_async_space;

	struct page **pages;
	size_t buffer_size;
	uint32_t buffer_free;
	struct list_head todo;
	wait_queue_head_t wait;
=======
	struct list_head waiting_threads;
	int pid;
	struct task_struct *tsk;
	struct files_struct *files;
	struct mutex files_lock;
	struct hlist_node deferred_work_node;
	int deferred_work;
	bool is_dead;

	struct list_head todo;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	struct binder_stats stats;
	struct list_head delivered_death;
	int max_threads;
	int requested_threads;
	int requested_threads_started;
<<<<<<< HEAD
	int ready_threads;
	long default_priority;
	struct dentry *debugfs_entry;
#ifdef RT_PRIO_INHERIT
	unsigned long default_rt_prio:16;
	unsigned long default_policy:16;
#endif
#ifdef BINDER_MONITOR
	struct binder_buffer *large_buffer;
#endif
#ifdef MTK_BINDER_PAGE_USED_RECORD
	unsigned int page_used;
	unsigned int page_used_peak;
#endif
};

enum {
	BINDER_LOOPER_STATE_REGISTERED	= 0x01,
	BINDER_LOOPER_STATE_ENTERED		= 0x02,
	BINDER_LOOPER_STATE_EXITED		= 0x04,
	BINDER_LOOPER_STATE_INVALID		= 0x08,
	BINDER_LOOPER_STATE_WAITING		= 0x10,
	BINDER_LOOPER_STATE_NEED_RETURN = 0x20
};

struct binder_thread {
	struct binder_proc *proc;
	struct rb_node rb_node;
	int pid;
	int looper;
	struct binder_transaction *transaction_stack;
	struct list_head todo;
	uint32_t return_error;	/* Write failed, return error code in read buf */
	uint32_t return_error2;	/* Write failed, return error code in read */
	/* buffer. Used when sending a reply to a dead process that */
	/* we are also waiting on */
	wait_queue_head_t wait;
	struct binder_stats stats;
=======
	int tmp_ref;
	struct binder_priority default_priority;
	struct dentry *debugfs_entry;
	struct binder_alloc alloc;
	struct binder_context *context;
	spinlock_t inner_lock;
	spinlock_t outer_lock;
};

enum {
	BINDER_LOOPER_STATE_REGISTERED  = 0x01,
	BINDER_LOOPER_STATE_ENTERED     = 0x02,
	BINDER_LOOPER_STATE_EXITED      = 0x04,
	BINDER_LOOPER_STATE_INVALID     = 0x08,
	BINDER_LOOPER_STATE_WAITING     = 0x10,
	BINDER_LOOPER_STATE_POLL        = 0x20,
};

/**
 * struct binder_thread - binder thread bookkeeping
 * @proc:                 binder process for this thread
 *                        (invariant after initialization)
 * @rb_node:              element for proc->threads rbtree
 *                        (protected by @proc->inner_lock)
 * @waiting_thread_node:  element for @proc->waiting_threads list
 *                        (protected by @proc->inner_lock)
 * @pid:                  PID for this thread
 *                        (invariant after initialization)
 * @looper:               bitmap of looping state
 *                        (only accessed by this thread)
 * @looper_needs_return:  looping thread needs to exit driver
 *                        (no lock needed)
 * @transaction_stack:    stack of in-progress transactions for this thread
 *                        (protected by @proc->inner_lock)
 * @todo:                 list of work to do for this thread
 *                        (protected by @proc->inner_lock)
 * @process_todo:         whether work in @todo should be processed
 *                        (protected by @proc->inner_lock)
 * @return_error:         transaction errors reported by this thread
 *                        (only accessed by this thread)
 * @reply_error:          transaction errors reported by target thread
 *                        (protected by @proc->inner_lock)
 * @wait:                 wait queue for thread work
 * @stats:                per-thread statistics
 *                        (atomics, no lock needed)
 * @tmp_ref:              temporary reference to indicate thread is in use
 *                        (atomic since @proc->inner_lock cannot
 *                        always be acquired)
 * @is_dead:              thread is dead and awaiting free
 *                        when outstanding transactions are cleaned up
 *                        (protected by @proc->inner_lock)
 * @task:                 struct task_struct for this thread
 *
 * Bookkeeping structure for binder threads.
 */
struct binder_thread {
	struct binder_proc *proc;
	struct rb_node rb_node;
	struct list_head waiting_thread_node;
	int pid;
	int looper;              /* only modified by this thread */
	bool looper_need_return; /* can be written by other thread */
	struct binder_transaction *transaction_stack;
	struct list_head todo;
	bool process_todo;
	struct binder_error return_error;
	struct binder_error reply_error;
	wait_queue_head_t wait;
	struct binder_stats stats;
	atomic_t tmp_ref;
	bool is_dead;
	struct task_struct *task;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
};

struct binder_transaction {
	int debug_id;
	struct binder_work work;
	struct binder_thread *from;
	struct binder_transaction *from_parent;
	struct binder_proc *to_proc;
	struct binder_thread *to_thread;
	struct binder_transaction *to_parent;
	unsigned need_reply:1;
<<<<<<< HEAD
	/* unsigned is_dead:1; *//* not used at the moment */

	struct binder_buffer *buffer;
	unsigned int code;
	unsigned int flags;
	long priority;
	long saved_priority;
	kuid_t sender_euid;
#ifdef RT_PRIO_INHERIT
	unsigned long rt_prio:16;
	unsigned long policy:16;
	unsigned long saved_rt_prio:16;
	unsigned long saved_policy:16;
#endif
#ifdef BINDER_MONITOR
	struct timespec timestamp;

	enum wait_on_reason wait_on;
	enum wait_on_reason bark_on;
	struct rb_node rb_node;	/* by bark_time */
	struct timespec bark_time;
	struct timespec exe_timestamp;
	struct timeval tv;
	char service[MAX_SERVICE_NAME_LEN];
	pid_t fproc;
	pid_t fthrd;
	pid_t tproc;
	pid_t tthrd;
	unsigned int log_idx;
#endif
};

static void
binder_defer_work(struct binder_proc *proc, enum binder_deferred_state defer);
static inline void binder_lock(const char *tag);
static inline void binder_unlock(const char *tag);

#ifdef BINDER_MONITOR
/* work should be done within how many secs */
#define WAIT_BUDGET_READ  2
#define WAIT_BUDGET_EXEC  4
#define WAIT_BUDGET_MIN   min(WAIT_BUDGET_READ, WAIT_BUDGET_EXEC)

static struct rb_root bwdog_transacts;

static const char *const binder_wait_on_str[] = {
	"none",
	"read",
	"exec",
	"rply"
};

struct binder_timeout_log_entry {
	enum wait_on_reason r;
	pid_t from_proc;
	pid_t from_thrd;
	pid_t to_proc;
	pid_t to_thrd;
	unsigned over_sec;
	struct timespec ts;
	struct timeval tv;
	unsigned int code;
	char service[MAX_SERVICE_NAME_LEN];
	int debug_id;
};

struct binder_timeout_log {
	int next;
	int full;
#ifdef BINDER_PERF_EVAL
	struct binder_timeout_log_entry entry[256];
#else
	struct binder_timeout_log_entry entry[64];
#endif
};

static struct binder_timeout_log binder_timeout_log_t;

/**
 * binder_timeout_log_add - Insert a timeout log
 */
static struct binder_timeout_log_entry *binder_timeout_log_add(void)
{
	struct binder_timeout_log *log = &binder_timeout_log_t;
	struct binder_timeout_log_entry *e;

	e = &log->entry[log->next];
	memset(e, 0, sizeof(*e));
	log->next++;
	if (log->next == ARRAY_SIZE(log->entry)) {
		log->next = 0;
		log->full = 1;
	}
	return e;
}

/**
 * binder_print_bwdog - Output info of a timeout transaction
 * @t:		pointer to the timeout transaction
 * @cur_in:	current timespec while going to print
 * @e:		timeout log entry to record
 * @r:		output reason, either while barking or after barked
 */
static void binder_print_bwdog(struct binder_transaction *t,
			       struct timespec *cur_in,
			       struct binder_timeout_log_entry *e, enum wait_on_reason r)
{
	struct rtc_time tm;
	struct timespec *startime;
	struct timespec cur, sub_t;

	if (cur_in && e) {
		memcpy(&cur, cur_in, sizeof(struct timespec));
	} else {
		do_posix_clock_monotonic_gettime(&cur);
		/*monotonic_to_bootbased(&cur); */
	}
	startime = (r == WAIT_ON_EXEC) ? &t->exe_timestamp : &t->timestamp;
	sub_t = timespec_sub(cur, *startime);

	rtc_time_to_tm(t->tv.tv_sec, &tm);
	pr_debug("%d %s %d:%d to %d:%d %s %u.%03ld sec (%s) dex_code %u",
		 t->debug_id, binder_wait_on_str[r],
		 t->fproc, t->fthrd, t->tproc, t->tthrd,
		 (cur_in && e) ? "over" : "total",
		 (unsigned)sub_t.tv_sec, (sub_t.tv_nsec / NSEC_PER_MSEC),
		 t->service, t->code);
	pr_debug(" start_at %lu.%03ld android %d-%02d-%02d %02d:%02d:%02d.%03lu\n",
		 (unsigned long)startime->tv_sec,
		 (startime->tv_nsec / NSEC_PER_MSEC),
		 (tm.tm_year + 1900), (tm.tm_mon + 1), tm.tm_mday,
		 tm.tm_hour, tm.tm_min, tm.tm_sec, (unsigned long)(t->tv.tv_usec / USEC_PER_MSEC));

	if (e) {
		e->over_sec = sub_t.tv_sec;
		memcpy(&e->ts, startime, sizeof(struct timespec));
	}
}

/**
 * binder_bwdog_safe - Check a transaction is monitor-free or not
 * @t:	pointer to the transaction to check
 *
 * Returns 1 means safe.
 */
static inline int binder_bwdog_safe(struct binder_transaction *t)
{
	return (t->wait_on == WAIT_ON_NONE) ? 1 : 0;
}

/**
 * binder_query_bwdog - Check a transaction is queued or not
 * @t:	pointer to the transaction to check
 *
 * Returns a pointer points to t, or NULL if it's not queued.
 */
static struct rb_node **binder_query_bwdog(struct binder_transaction *t)
{
	struct rb_node **p = &bwdog_transacts.rb_node;
	struct rb_node *parent = NULL;
	struct binder_transaction *transact = NULL;
	int comp;

	while (*p) {
		parent = *p;
		transact = rb_entry(parent, struct binder_transaction, rb_node);

		comp = timespec_compare(&t->bark_time, &transact->bark_time);
		if (comp < 0)
			p = &(*p)->rb_left;
		else if (comp > 0)
			p = &(*p)->rb_right;
		else
			break;
	}
	return p;
}

/**
 * binder_queue_bwdog - Queue a transaction to keep tracking
 * @t:		pointer to the transaction being tracked
 * @budget:	seconds, which this transaction can afford
 */
static void binder_queue_bwdog(struct binder_transaction *t, time_t budget)
{
	struct rb_node **p = &bwdog_transacts.rb_node;
	struct rb_node *parent = NULL;
	struct binder_transaction *transact = NULL;
	int ret;

	do_posix_clock_monotonic_gettime(&t->bark_time);
	/* monotonic_to_bootbased(&t->bark_time); */
	t->bark_time.tv_sec += budget;

	while (*p) {
		parent = *p;
		transact = rb_entry(parent, struct binder_transaction, rb_node);

		ret = timespec_compare(&t->bark_time, &transact->bark_time);
		if (ret < 0)
			p = &(*p)->rb_left;
		else if (ret > 0)
			p = &(*p)->rb_right;
		else {
			pr_debug("%d found same key\n", t->debug_id);
			t->bark_time.tv_nsec += 1;
			p = &(*p)->rb_right;
		}
	}
	rb_link_node(&t->rb_node, parent, p);
	rb_insert_color(&t->rb_node, &bwdog_transacts);
}

/**
 * binder_cancel_bwdog - Cancel a transaction from tracking list
 * @t:		pointer to the transaction being cancelled
 */
static void binder_cancel_bwdog(struct binder_transaction *t)
{
	struct rb_node **p = NULL;

	if (binder_bwdog_safe(t)) {
		if (t->bark_on) {
			binder_print_bwdog(t, NULL, NULL, t->bark_on);
			t->bark_on = WAIT_ON_NONE;
		}
		return;
	}

	p = binder_query_bwdog(t);
	if (*p == NULL) {
		pr_err("%d waits %s, but not queued...\n",
		       t->debug_id, binder_wait_on_str[t->wait_on]);
		return;
	}

	rb_erase(&t->rb_node, &bwdog_transacts);
	t->wait_on = WAIT_ON_NONE;
}

/**
 * binder_bwdog_bark -
 *     Barking function while timeout. Record target process or thread, which
 * cannot handle transaction in time, including todo list. Also add a log
 * entry for AMS reference.
 *
 * @t:		pointer to the transaction, which triggers watchdog
 * @cur:	current kernel timespec
 */
static void binder_bwdog_bark(struct binder_transaction *t, struct timespec *cur)
{
	struct binder_timeout_log_entry *e;

	if (binder_bwdog_safe(t)) {
		pr_debug("%d watched, but wait nothing\n", t->debug_id);
		return;
	}

	e = binder_timeout_log_add();
	binder_print_bwdog(t, cur, e, t->wait_on);

	e->r = t->wait_on;
	e->from_proc = t->fproc;
	e->from_thrd = t->fthrd;
	e->debug_id = t->debug_id;
	memcpy(&e->tv, &t->tv, sizeof(struct timeval));

	switch (t->wait_on) {
	case WAIT_ON_READ:{
			if (!t->to_proc) {
				pr_err("%d has NULL target\n", t->debug_id);
				return;
			}
			e->to_proc = t->tproc;
			e->to_thrd = t->tthrd;
			e->code = t->code;
			strcpy(e->service, t->service);
			break;
		}

	case WAIT_ON_EXEC:{
			if (!t->to_thread) {
				pr_err("%d has NULL target for " "execution\n", t->debug_id);
				return;
			}
			e->to_proc = t->tproc;
			e->to_thrd = t->tthrd;
			e->code = t->code;
			strcpy(e->service, t->service);
			goto dumpBackTrace;
		}

	case WAIT_ON_REPLY_READ:{
			if (!t->to_thread) {
				pr_err("%d has NULL target thread\n", t->debug_id);
				return;
			}
			e->to_proc = t->tproc;
			e->to_thrd = t->tthrd;
			strcpy(e->service, "");
			break;
		}

	default:{
			return;
		}
	}

dumpBackTrace:
	return;
}

/**
 * binder_bwdog_thread - Main thread to check timeout list periodically
 */
static int binder_bwdog_thread(void *__unused)
{
	unsigned long sleep_sec;
	struct rb_node *n = NULL;
	struct timespec cur_time;
	struct binder_transaction *t = NULL;

	for (;;) {
		binder_lock(__func__);
		do_posix_clock_monotonic_gettime(&cur_time);
		/* monotonic_to_bootbased(&cur_time); */

		for (n = rb_first(&bwdog_transacts); n != NULL; n = rb_next(n)) {
			t = rb_entry(n, struct binder_transaction, rb_node);
			if (timespec_compare(&cur_time, &t->bark_time) < 0)
				break;

			binder_bwdog_bark(t, &cur_time);
			rb_erase(&t->rb_node, &bwdog_transacts);
			t->bark_on = t->wait_on;
			t->wait_on = WAIT_ON_NONE;
		}

		if (!n)
			sleep_sec = WAIT_BUDGET_MIN;
		else
			sleep_sec = timespec_sub(t->bark_time, cur_time).tv_sec;
		binder_unlock(__func__);

		msleep(sleep_sec * MSEC_PER_SEC);
	}
	pr_debug("%s exit...\n", __func__);
	return 0;
}

/**
 * find_process_by_pid - convert pid to task_struct
 * @pid:	pid for convert task
 */
static inline struct task_struct *find_process_by_pid(pid_t pid)
{
	return pid ? find_task_by_vpid(pid) : NULL;
}

/**
 * binder_find_buffer_sender - find the sender task_struct of this buffer
 * @buf	binder buffer
 * @tsk	task_struct of buf sender
 */
static struct task_struct *binder_find_buffer_sender(struct binder_buffer *buf)
{
	struct binder_transaction *t;
	struct binder_transaction_log_entry *e;
	struct task_struct *tsk;

	t = buf->transaction;
	if (t && t->fproc)
		tsk = find_process_by_pid(t->fproc);

	else {
		e = buf->log_entry;
		if ((buf->debug_id == e->debug_id) && e->from_proc)
			tsk = find_process_by_pid(e->from_proc);
		else
			tsk = NULL;
	}
	return tsk;
}

/**
 * copy from /kernel/fs/proc/base.c and modified to get task full name
 */
static int binder_proc_pid_cmdline(struct task_struct *task, char *buf)
{
	int res = 0;
	unsigned int len;
	struct mm_struct *mm;
	/*============ add begin =============================*/
	char c = ' ';
	char *str;
	unsigned int size;
	char *buffer;

	if (NULL == task)
		goto out;
	/*============ add end  ===============================*/
	mm = get_task_mm(task);
	if (!mm)
		goto out;
	if (!mm->arg_end)
		goto out_mm;	/* Shh! No looking before we're done */
	/*============ add begin =============================*/
	buffer = kzalloc(PAGE_SIZE, GFP_KERNEL);
	if (NULL == buffer)
		goto out_mm;
	/*============ add end  ===============================*/

	len = mm->arg_end - mm->arg_start;

	if (len > PAGE_SIZE)
		len = PAGE_SIZE;

	res = access_process_vm(task, mm->arg_start, buffer, len, 0);

	/* If the nul at the end of args has been overwritten, then */
	/* assume application is using setproctitle(3). */
	if (res > 0 && buffer[res - 1] != '\0' && len < PAGE_SIZE) {
		len = strnlen(buffer, res);
		if (len < res) {
			res = len;
		} else {
			len = mm->env_end - mm->env_start;
			if (len > PAGE_SIZE - res)
				len = PAGE_SIZE - res;
			res += access_process_vm(task, mm->env_start, buffer + res, len, 0);
			res = strnlen(buffer, res);
		}
	}
	/*============ add begin =============================*/
	str = strchr(buffer, c);
	if (NULL != str)
		size = (unsigned int)(str - buffer);
	else
		size = res;
	if (size > 256)
		size = 256;
	snprintf(buf, size, buffer);
	kfree(buffer);
	/*============ add end  ===============================*/
out_mm:
	mmput(mm);
out:
	return res;
}

/**
 * binder_print_buf - Print buffer info
 * @t:		transaction
 * @buffer:	target buffer
 * @dest:	dest string pointer
 * @success:	does this buffer allocate success
 * @check:	check this log for owner finding
 */
static void binder_print_buf(struct binder_buffer *buffer, char *dest, int success, int check)
{
	struct rtc_time tm;
	struct binder_transaction *t = buffer->transaction;
	char str[TRANS_LOG_LEN];
	struct task_struct *sender_tsk;
	struct task_struct *rec_tsk;
	char sender_name[256], rec_name[256];
	int len_s, len_r;
	int ptr = 0;

	if (NULL == t) {
		struct binder_transaction_log_entry *log_entry = buffer->log_entry;
		if ((log_entry != NULL)
		    && (buffer->debug_id == log_entry->debug_id)) {
			rtc_time_to_tm(log_entry->tv.tv_sec, &tm);
			sender_tsk = find_process_by_pid(log_entry->from_proc);
			rec_tsk = find_process_by_pid(log_entry->to_proc);
			len_s = binder_proc_pid_cmdline(sender_tsk, sender_name);
			len_r = binder_proc_pid_cmdline(rec_tsk, rec_name);
			ptr += snprintf(str+ptr, sizeof(str)-ptr,
				"binder:check=%d,success=%d,id=%d,call=%s,type=%s,",
				check, success, buffer->debug_id,
				buffer->async_transaction ? "async" : "sync",
				(2 == log_entry->call_type) ? "reply" :
				((1 == log_entry->call_type) ? "async" : "call"));
			ptr += snprintf(str+ptr, sizeof(str)-ptr,
				"from=%d,tid=%d,name=%s,to=%d,name=%s,tid=%d,name=%s,",
				log_entry->from_proc, log_entry->from_thread,
				len_s ? sender_name : ((sender_tsk != NULL) ?
							sender_tsk->comm : ""),
				log_entry->to_proc,
				len_r ? rec_name : ((rec_tsk != NULL) ? rec_tsk->comm : ""),
				log_entry->to_thread, log_entry->service);
			ptr += snprintf(str+ptr, sizeof(str)-ptr,
				"size=%zd,node=%d,handle=%d,dex=%u,auf=%d,start=%lu.%03ld,",
				(buffer->data_size + buffer->offsets_size),
				log_entry->to_node, log_entry->target_handle,
				log_entry->code, buffer->allow_user_free,
				(unsigned long)log_entry->timestamp.tv_sec,
				(log_entry->timestamp.tv_nsec / NSEC_PER_MSEC));
			ptr += snprintf(str+ptr, sizeof(str)-ptr,
				"android=%d-%02d-%02d %02d:%02d:%02d.%03lu\n",
				(tm.tm_year + 1900), (tm.tm_mon + 1),
				tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
				(unsigned long)(log_entry->tv.tv_usec / USEC_PER_MSEC));
		} else {
			ptr += snprintf(str+ptr, sizeof(str)-ptr,
				"binder:check=%d,success=%d,id=%d,call=%s, ,",
				check, success, buffer->debug_id,
				buffer->async_transaction ? "async" : "sync");
			ptr += snprintf(str+ptr, sizeof(str)-ptr,
				",,,,,,,size=%zd,,,," "auf=%d,,\n",
				(buffer->data_size + buffer->offsets_size),
				buffer->allow_user_free);
		}
	} else {
		rtc_time_to_tm(t->tv.tv_sec, &tm);
		sender_tsk = find_process_by_pid(t->fproc);
		rec_tsk = find_process_by_pid(t->tproc);
		len_s = binder_proc_pid_cmdline(sender_tsk, sender_name);
		len_r = binder_proc_pid_cmdline(rec_tsk, rec_name);
		ptr += snprintf(str+ptr, sizeof(str)-ptr,
			"binder:check=%d,success=%d,id=%d,call=%s,type=%s,",
			check, success, t->debug_id,
			buffer->async_transaction ? "async" : "sync ",
			binder_wait_on_str[t->wait_on]);
		ptr += snprintf(str+ptr, sizeof(str)-ptr,
			"from=%d,tid=%d,name=%s,to=%d,name=%s,tid=%d,name=%s,",
			t->fproc, t->fthrd,
			len_s ? sender_name : ((sender_tsk != NULL) ?
						sender_tsk->comm : ""),
			t->tproc,
			len_r ? rec_name : ((rec_tsk != NULL) ? rec_tsk->comm : ""),
			t->tthrd, t->service);
		ptr += snprintf(str+ptr, sizeof(str)-ptr,
			"size=%zd,,,dex=%u,auf=%d,start=%lu.%03ld,android=",
			(buffer->data_size + buffer->offsets_size), t->code,
			buffer->allow_user_free, (unsigned long)t->timestamp.tv_sec,
			(t->timestamp.tv_nsec / NSEC_PER_MSEC));
		ptr += snprintf(str+ptr, sizeof(str)-ptr,
			"%d-%02d-%02d %02d:%02d:%02d.%03lu\n",
			(tm.tm_year + 1900),
			(tm.tm_mon + 1), tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
			(unsigned long)(t->tv.tv_usec / USEC_PER_MSEC));
	}
	pr_debug("%s", str);
	if (dest != NULL)
		strncat(dest, str, sizeof(str) - strlen(dest) - 1);
}

/**
 * binder_check_buf_checked -
 *     Consider buffer related issue usually makes a series of failure.
 * Only care about the first problem time to minimize debug overhead.
 */
static int binder_check_buf_checked(void)
{
	return (binder_check_buf_pid == -1);
}

static size_t binder_buffer_size(struct binder_proc *proc, struct binder_buffer *buffer);

/**
 * binder_check_buf - Dump necessary info for buffer usage analysis
 * @target_proc:	receiver
 * @size:		requested size
 * @is_async:		1 if an async call
 */
static void binder_check_buf(struct binder_proc *target_proc, size_t size, int is_async)
{
	struct rb_node *n;
	struct binder_buffer *buffer;
	int i;
	int large_buffer_count = 0;
	size_t tmp_size, threshold;
	struct task_struct *sender;
	struct task_struct *larger;
	char sender_name[256], rec_name[256];
	struct timespec exp_timestamp;
	struct timeval tv;
	struct rtc_time tm;
#if defined(CONFIG_MTK_AEE_FEATURE)
	int db_flag = DB_OPT_BINDER_INFO;
#endif
	int len_s, len_r;
	int ptr = 0;

	pr_debug("buffer allocation failed on %d:0 %s from %d:%d size %zd\n",
		 target_proc->pid,
		 is_async ? "async" : "call ", binder_check_buf_pid, binder_check_buf_tid, size);

	if (binder_check_buf_checked())
		return;
	/* check blocked service for async call */
	if (is_async) {
		pr_debug("buffer allocation failed on %d:0 (%s) async service blocked\n",
			 target_proc->pid, target_proc->tsk ? target_proc->tsk->comm : "");
	}

	pr_debug("%d:0 pending transactions:\n", target_proc->pid);
	threshold = target_proc->buffer_size / 16;
	for (n = rb_last(&target_proc->allocated_buffers), i = 0; n; n = rb_prev(n), i++) {
		buffer = rb_entry(n, struct binder_buffer, rb_node);
		tmp_size = binder_buffer_size(target_proc, buffer);
		BUG_ON(buffer->free);

		if (tmp_size > threshold) {
			if ((NULL == target_proc->large_buffer) ||
			    (target_proc->large_buffer &&
			     (tmp_size >
			      binder_buffer_size(target_proc, target_proc->large_buffer))))
				target_proc->large_buffer = buffer;
			large_buffer_count++;
			binder_print_buf(buffer, NULL, 1, 0);
		} else {
			if (i < 20)
				binder_print_buf(buffer, NULL, 1, 0);
		}
	}
	pr_debug("%d:0 total pending trans: %d(%d large isze)\n",
		 target_proc->pid, i, large_buffer_count);

	do_posix_clock_monotonic_gettime(&exp_timestamp);
	/* monotonic_to_bootbased(&exp_timestamp); */
	do_gettimeofday(&tv);
	/* consider time zone. translate to android time */
	tv.tv_sec -= (sys_tz.tz_minuteswest * 60);
	rtc_time_to_tm(tv.tv_sec, &tm);

	sender = find_process_by_pid(binder_check_buf_pid);
	len_s = binder_proc_pid_cmdline(sender, sender_name);
	len_r = binder_proc_pid_cmdline(target_proc->tsk, rec_name);
	if (size > threshold) {
		if (target_proc->large_buffer) {
			pr_debug("on %d:0 the largest pending trans is:\n", target_proc->pid);
			binder_print_buf(target_proc->large_buffer, large_msg, 1, 0);
		}
		snprintf(aee_word, sizeof(aee_word),
			 "check %s: large binder trans fail on %d:0 size %zd",
			 len_s ? sender_name : ((sender != NULL) ? sender->comm : ""),
			 target_proc->pid, size);
		ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
			"BINDER_BUF_DEBUG\n%s",
			large_msg);
		ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
			"binder:check=%d,success=%d,,call=%s,,from=%d,tid=%d,",
			1, 0, is_async ? "async" : "sync",
			binder_check_buf_pid, binder_check_buf_tid);
		ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
			"name=%s,to=%d,name=%s,,,size=%zd,,,," ",start=%lu.%03ld,android=",
			len_s ? sender_name : ((sender != NULL) ? sender->comm : ""),
			target_proc->pid,
			len_r ? rec_name : ((target_proc->tsk != NULL) ? target_proc->tsk->
						comm : ""), size, (unsigned long)exp_timestamp.tv_sec,
			(exp_timestamp.tv_nsec / NSEC_PER_MSEC));
		ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
			"%d-%02d-%02d %02d:%02d:%02d.%03lu\n",
			(tm.tm_year + 1900), (tm.tm_mon + 1), tm.tm_mday, tm.tm_hour,
			tm.tm_min, tm.tm_sec, (unsigned long)(tv.tv_usec / USEC_PER_MSEC));
		ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
			"large data size,check sender %d(%s)! check kernel log\n",
			binder_check_buf_pid, sender ? sender->comm : "");
	} else {
		if (target_proc->large_buffer) {
			pr_debug("on %d:0 the largest pending trans is:\n", target_proc->pid);
			binder_print_buf(target_proc->large_buffer, large_msg, 1, 1);
			larger = binder_find_buffer_sender(target_proc->large_buffer);
			snprintf(aee_word, sizeof(aee_word),
				 "check %s: large binder trans",
				 (larger != NULL) ? larger->comm : "");
			ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
				"BINDER_BUF_DEBUG:\n%s",
				large_msg);
			ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
				"binder:check=%d,success=%d,,call=%s,,from=%d,tid=%d,name=%s,",
				0, 0, is_async ? "async" : "sync",
				binder_check_buf_pid, binder_check_buf_tid,
				len_s ? sender_name : ((sender != NULL) ?
							sender->comm : ""));
			ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
				"to=%d,name=%s,,,size=%zd,,,,",
				target_proc->pid, len_r ? rec_name : ((target_proc->tsk != NULL)
				? target_proc->tsk->comm : ""), size);
			ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
				",start=%lu.%03ld,android=",
				(unsigned long)exp_timestamp.tv_sec,
				(exp_timestamp.tv_nsec / NSEC_PER_MSEC));
			ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
				"%d-%02d-%02d %02d:%02d:%02d.%03lu\n",
				(tm.tm_year + 1900), (tm.tm_mon + 1), tm.tm_mday,
				tm.tm_hour, tm.tm_min, tm.tm_sec,
				(unsigned long)(tv.tv_usec / USEC_PER_MSEC));
			ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
				"large data size,check sender %d(%s)! check kernel log\n",
				(larger != NULL) ? larger->pid : 0,
				(larger != NULL) ? larger->comm : "");
		} else {
			snprintf(aee_word, sizeof(aee_word),
				 "check %s: binder buffer exhaust ",
				 len_r ? rec_name : ((target_proc->tsk != NULL)
						     ? target_proc->tsk->comm : ""));
			ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
				"BINDER_BUF_DEBUG\n binder:check=%d,success=%d,",
				1, 0);
			ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
				"call=%s,from=%d,tid=%d,name=%s,to=%d,name=%s,,,size=%zd,,,,",
				is_async ? "async" : "sync",
				binder_check_buf_pid, binder_check_buf_tid,
				len_s ? sender_name : ((sender != NULL) ?
								sender->comm : ""),
				target_proc->pid, len_r ? rec_name : ((target_proc->tsk != NULL)
									? target_proc->
									tsk->comm : ""), size);
			ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
				",start=%lu.%03ld,android=%d-%02d-%02d %02d:%02d:%02d.%03lu\n",
				(unsigned long)exp_timestamp.tv_sec,
				(exp_timestamp.tv_nsec / NSEC_PER_MSEC), (tm.tm_year + 1900),
				(tm.tm_mon + 1), tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
				(unsigned long)(tv.tv_usec / USEC_PER_MSEC));
			ptr += snprintf(aee_msg+ptr, sizeof(aee_msg)-ptr,
				"%d small trans pending, check receiver %d(%s)! check kernel log\n",
				i, target_proc->pid,
				target_proc->tsk ? target_proc->tsk->comm : "");
		}

	}

	binder_check_buf_pid = -1;
	binder_check_buf_tid = -1;
#if defined(CONFIG_MTK_AEE_FEATURE)
	aee_kernel_warning_api(__FILE__, __LINE__, db_flag, &aee_word[0], &aee_msg[0]);
#endif
}
#endif
static int task_get_unused_fd_flags(struct binder_proc *proc, int flags)
{
	struct files_struct *files = proc->files;
	unsigned long rlim_cur;
	unsigned long irqs;

	if (files == NULL)
		return -ESRCH;

	if (!lock_task_sighand(proc->tsk, &irqs))
		return -EMFILE;

	rlim_cur = task_rlimit(proc->tsk, RLIMIT_NOFILE);
	unlock_task_sighand(proc->tsk, &irqs);

	return __alloc_fd(files, 0, rlim_cur, flags);
=======
	/* unsigned is_dead:1; */	/* not used at the moment */

	struct binder_buffer *buffer;
	unsigned int	code;
	unsigned int	flags;
	struct binder_priority	priority;
	struct binder_priority	saved_priority;
	bool    set_priority_called;
	kuid_t	sender_euid;
	binder_uintptr_t security_ctx;
	/**
	 * @lock:  protects @from, @to_proc, and @to_thread
	 *
	 * @from, @to_proc, and @to_thread can be set to NULL
	 * during thread teardown
	 */
	spinlock_t lock;
};

/**
 * binder_proc_lock() - Acquire outer lock for given binder_proc
 * @proc:         struct binder_proc to acquire
 *
 * Acquires proc->outer_lock. Used to protect binder_ref
 * structures associated with the given proc.
 */
#define binder_proc_lock(proc) _binder_proc_lock(proc, __LINE__)
static void
_binder_proc_lock(struct binder_proc *proc, int line)
{
	binder_debug(BINDER_DEBUG_SPINLOCKS,
		     "%s: line=%d\n", __func__, line);
	spin_lock(&proc->outer_lock);
}

/**
 * binder_proc_unlock() - Release spinlock for given binder_proc
 * @proc:         struct binder_proc to acquire
 *
 * Release lock acquired via binder_proc_lock()
 */
#define binder_proc_unlock(_proc) _binder_proc_unlock(_proc, __LINE__)
static void
_binder_proc_unlock(struct binder_proc *proc, int line)
{
	binder_debug(BINDER_DEBUG_SPINLOCKS,
		     "%s: line=%d\n", __func__, line);
	spin_unlock(&proc->outer_lock);
}

/**
 * binder_inner_proc_lock() - Acquire inner lock for given binder_proc
 * @proc:         struct binder_proc to acquire
 *
 * Acquires proc->inner_lock. Used to protect todo lists
 */
#define binder_inner_proc_lock(proc) _binder_inner_proc_lock(proc, __LINE__)
static void
_binder_inner_proc_lock(struct binder_proc *proc, int line)
{
	binder_debug(BINDER_DEBUG_SPINLOCKS,
		     "%s: line=%d\n", __func__, line);
	spin_lock(&proc->inner_lock);
}

/**
 * binder_inner_proc_unlock() - Release inner lock for given binder_proc
 * @proc:         struct binder_proc to acquire
 *
 * Release lock acquired via binder_inner_proc_lock()
 */
#define binder_inner_proc_unlock(proc) _binder_inner_proc_unlock(proc, __LINE__)
static void
_binder_inner_proc_unlock(struct binder_proc *proc, int line)
{
	binder_debug(BINDER_DEBUG_SPINLOCKS,
		     "%s: line=%d\n", __func__, line);
	spin_unlock(&proc->inner_lock);
}

/**
 * binder_node_lock() - Acquire spinlock for given binder_node
 * @node:         struct binder_node to acquire
 *
 * Acquires node->lock. Used to protect binder_node fields
 */
#define binder_node_lock(node) _binder_node_lock(node, __LINE__)
static void
_binder_node_lock(struct binder_node *node, int line)
{
	binder_debug(BINDER_DEBUG_SPINLOCKS,
		     "%s: line=%d\n", __func__, line);
	spin_lock(&node->lock);
}

/**
 * binder_node_unlock() - Release spinlock for given binder_proc
 * @node:         struct binder_node to acquire
 *
 * Release lock acquired via binder_node_lock()
 */
#define binder_node_unlock(node) _binder_node_unlock(node, __LINE__)
static void
_binder_node_unlock(struct binder_node *node, int line)
{
	binder_debug(BINDER_DEBUG_SPINLOCKS,
		     "%s: line=%d\n", __func__, line);
	spin_unlock(&node->lock);
}

/**
 * binder_node_inner_lock() - Acquire node and inner locks
 * @node:         struct binder_node to acquire
 *
 * Acquires node->lock. If node->proc also acquires
 * proc->inner_lock. Used to protect binder_node fields
 */
#define binder_node_inner_lock(node) _binder_node_inner_lock(node, __LINE__)
static void
_binder_node_inner_lock(struct binder_node *node, int line)
{
	binder_debug(BINDER_DEBUG_SPINLOCKS,
		     "%s: line=%d\n", __func__, line);
	spin_lock(&node->lock);
	if (node->proc)
		binder_inner_proc_lock(node->proc);
}

/**
 * binder_node_unlock() - Release node and inner locks
 * @node:         struct binder_node to acquire
 *
 * Release lock acquired via binder_node_lock()
 */
#define binder_node_inner_unlock(node) _binder_node_inner_unlock(node, __LINE__)
static void
_binder_node_inner_unlock(struct binder_node *node, int line)
{
	struct binder_proc *proc = node->proc;

	binder_debug(BINDER_DEBUG_SPINLOCKS,
		     "%s: line=%d\n", __func__, line);
	if (proc)
		binder_inner_proc_unlock(proc);
	spin_unlock(&node->lock);
}

static bool binder_worklist_empty_ilocked(struct list_head *list)
{
	return list_empty(list);
}

/**
 * binder_worklist_empty() - Check if no items on the work list
 * @proc:       binder_proc associated with list
 * @list:	list to check
 *
 * Return: true if there are no items on list, else false
 */
static bool binder_worklist_empty(struct binder_proc *proc,
				  struct list_head *list)
{
	bool ret;

	binder_inner_proc_lock(proc);
	ret = binder_worklist_empty_ilocked(list);
	binder_inner_proc_unlock(proc);
	return ret;
}

/**
 * binder_enqueue_work_ilocked() - Add an item to the work list
 * @work:         struct binder_work to add to list
 * @target_list:  list to add work to
 *
 * Adds the work to the specified list. Asserts that work
 * is not already on a list.
 *
 * Requires the proc->inner_lock to be held.
 */
static void
binder_enqueue_work_ilocked(struct binder_work *work,
			   struct list_head *target_list)
{
	BUG_ON(target_list == NULL);
	BUG_ON(work->entry.next && !list_empty(&work->entry));
	list_add_tail(&work->entry, target_list);
}

/**
 * binder_enqueue_deferred_thread_work_ilocked() - Add deferred thread work
 * @thread:       thread to queue work to
 * @work:         struct binder_work to add to list
 *
 * Adds the work to the todo list of the thread. Doesn't set the process_todo
 * flag, which means that (if it wasn't already set) the thread will go to
 * sleep without handling this work when it calls read.
 *
 * Requires the proc->inner_lock to be held.
 */
static void
binder_enqueue_deferred_thread_work_ilocked(struct binder_thread *thread,
					    struct binder_work *work)
{
	binder_enqueue_work_ilocked(work, &thread->todo);
}

/**
 * binder_enqueue_thread_work_ilocked() - Add an item to the thread work list
 * @thread:       thread to queue work to
 * @work:         struct binder_work to add to list
 *
 * Adds the work to the todo list of the thread, and enables processing
 * of the todo queue.
 *
 * Requires the proc->inner_lock to be held.
 */
static void
binder_enqueue_thread_work_ilocked(struct binder_thread *thread,
				   struct binder_work *work)
{
	binder_enqueue_work_ilocked(work, &thread->todo);
	thread->process_todo = true;
}

/**
 * binder_enqueue_thread_work() - Add an item to the thread work list
 * @thread:       thread to queue work to
 * @work:         struct binder_work to add to list
 *
 * Adds the work to the todo list of the thread, and enables processing
 * of the todo queue.
 */
static void
binder_enqueue_thread_work(struct binder_thread *thread,
			   struct binder_work *work)
{
	binder_inner_proc_lock(thread->proc);
	binder_enqueue_thread_work_ilocked(thread, work);
	binder_inner_proc_unlock(thread->proc);
}

static void
binder_dequeue_work_ilocked(struct binder_work *work)
{
	list_del_init(&work->entry);
}

/**
 * binder_dequeue_work() - Removes an item from the work list
 * @proc:         binder_proc associated with list
 * @work:         struct binder_work to remove from list
 *
 * Removes the specified work item from whatever list it is on.
 * Can safely be called if work is not on any list.
 */
static void
binder_dequeue_work(struct binder_proc *proc, struct binder_work *work)
{
	binder_inner_proc_lock(proc);
	binder_dequeue_work_ilocked(work);
	binder_inner_proc_unlock(proc);
}

static struct binder_work *binder_dequeue_work_head_ilocked(
					struct list_head *list)
{
	struct binder_work *w;

	w = list_first_entry_or_null(list, struct binder_work, entry);
	if (w)
		list_del_init(&w->entry);
	return w;
}

/**
 * binder_dequeue_work_head() - Dequeues the item at head of list
 * @proc:         binder_proc associated with list
 * @list:         list to dequeue head
 *
 * Removes the head of the list if there are items on the list
 *
 * Return: pointer dequeued binder_work, NULL if list was empty
 */
static struct binder_work *binder_dequeue_work_head(
					struct binder_proc *proc,
					struct list_head *list)
{
	struct binder_work *w;

	binder_inner_proc_lock(proc);
	w = binder_dequeue_work_head_ilocked(list);
	binder_inner_proc_unlock(proc);
	return w;
}

static void
binder_defer_work(struct binder_proc *proc, enum binder_deferred_state defer);
static void binder_free_thread(struct binder_thread *thread);
static void binder_free_proc(struct binder_proc *proc);
static void binder_inc_node_tmpref_ilocked(struct binder_node *node);

static int task_get_unused_fd_flags(struct binder_proc *proc, int flags)
{
	unsigned long rlim_cur;
	unsigned long irqs;
	int ret;

	mutex_lock(&proc->files_lock);
	if (proc->files == NULL) {
		ret = -ESRCH;
		goto err;
	}
	if (!lock_task_sighand(proc->tsk, &irqs)) {
		ret = -EMFILE;
		goto err;
	}
	rlim_cur = task_rlimit(proc->tsk, RLIMIT_NOFILE);
	unlock_task_sighand(proc->tsk, &irqs);

	ret = __alloc_fd(proc->files, 0, rlim_cur, flags);
err:
	mutex_unlock(&proc->files_lock);
	return ret;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
}

/*
 * copied from fd_install
 */
<<<<<<< HEAD
static void task_fd_install(struct binder_proc *proc, unsigned int fd, struct file *file)
{
	if (proc->files)
		__fd_install(proc->files, fd, file);
=======
static void task_fd_install(
	struct binder_proc *proc, unsigned int fd, struct file *file)
{
	mutex_lock(&proc->files_lock);
	if (proc->files)
		__fd_install(proc->files, fd, file);
	mutex_unlock(&proc->files_lock);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
}

/*
 * copied from sys_close
 */
static long task_close_fd(struct binder_proc *proc, unsigned int fd)
{
	int retval;

<<<<<<< HEAD
	if (proc->files == NULL)
		return -ESRCH;

=======
	mutex_lock(&proc->files_lock);
	if (proc->files == NULL) {
		retval = -ESRCH;
		goto err;
	}
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	retval = __close_fd(proc->files, fd);
	/* can't restart close syscall because file table entry was cleared */
	if (unlikely(retval == -ERESTARTSYS ||
		     retval == -ERESTARTNOINTR ||
<<<<<<< HEAD
		     retval == -ERESTARTNOHAND || retval == -ERESTART_RESTARTBLOCK))
		retval = -EINTR;

	return retval;
}

static inline void binder_lock(const char *tag)
{
	trace_binder_lock(tag);
	mutex_lock(&binder_main_lock);
	trace_binder_locked(tag);
}

static inline void binder_unlock(const char *tag)
{
	trace_binder_unlock(tag);
	mutex_unlock(&binder_main_lock);
}

static void binder_set_nice(long nice)
{
	long min_nice;

	if (can_nice(current, nice)) {
		set_user_nice(current, nice);
		return;
	}
	min_nice = rlimit_to_nice(current->signal->rlim[RLIMIT_NICE].rlim_cur);
	binder_debug(BINDER_DEBUG_PRIORITY_CAP,
		     "%d: nice value %ld not allowed use %ld instead\n",
		     current->pid, nice, min_nice);

	set_user_nice(current, min_nice);
	if (min_nice <= MAX_NICE)
		return;
	binder_user_error("%d RLIMIT_NICE not set\n", current->pid);
}

static size_t binder_buffer_size(struct binder_proc *proc, struct binder_buffer *buffer)
{
	if (list_is_last(&buffer->entry, &proc->buffers))
		return proc->buffer + proc->buffer_size - (void *)buffer->data;
	return (size_t) list_entry(buffer->entry.next,
				   struct binder_buffer, entry)-(size_t) buffer->data;
}

static void binder_insert_free_buffer(struct binder_proc *proc, struct binder_buffer *new_buffer)
{
	struct rb_node **p = &proc->free_buffers.rb_node;
	struct rb_node *parent = NULL;
	struct binder_buffer *buffer;
	size_t buffer_size;
	size_t new_buffer_size;

	BUG_ON(!new_buffer->free);

	new_buffer_size = binder_buffer_size(proc, new_buffer);

	binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
		     "%d: add free buffer, size %zd, at %pK\n",
		     proc->pid, new_buffer_size, new_buffer);

	while (*p) {
		parent = *p;
		buffer = rb_entry(parent, struct binder_buffer, rb_node);
		BUG_ON(!buffer->free);

		buffer_size = binder_buffer_size(proc, buffer);

		if (new_buffer_size < buffer_size)
			p = &parent->rb_left;
		else
			p = &parent->rb_right;
	}
	rb_link_node(&new_buffer->rb_node, parent, p);
	rb_insert_color(&new_buffer->rb_node, &proc->free_buffers);
}

static void binder_insert_allocated_buffer(struct binder_proc *proc,
					   struct binder_buffer *new_buffer)
{
	struct rb_node **p = &proc->allocated_buffers.rb_node;
	struct rb_node *parent = NULL;
	struct binder_buffer *buffer;

	BUG_ON(new_buffer->free);

	while (*p) {
		parent = *p;
		buffer = rb_entry(parent, struct binder_buffer, rb_node);
		BUG_ON(buffer->free);

		if (new_buffer < buffer)
			p = &parent->rb_left;
		else if (new_buffer > buffer)
			p = &parent->rb_right;
		else
			BUG();
	}
	rb_link_node(&new_buffer->rb_node, parent, p);
	rb_insert_color(&new_buffer->rb_node, &proc->allocated_buffers);
}

static struct binder_buffer *binder_buffer_lookup(struct binder_proc *proc, uintptr_t user_ptr)
{
	struct rb_node *n = proc->allocated_buffers.rb_node;
	struct binder_buffer *buffer;
	struct binder_buffer *kern_ptr;

	kern_ptr = (struct binder_buffer *)(user_ptr - proc->user_buffer_offset
					    - offsetof(struct binder_buffer, data));

	while (n) {
		buffer = rb_entry(n, struct binder_buffer, rb_node);
		BUG_ON(buffer->free);

		if (kern_ptr < buffer)
			n = n->rb_left;
		else if (kern_ptr > buffer)
			n = n->rb_right;
		else
			return buffer;
	}
	return NULL;
}

static int binder_update_page_range(struct binder_proc *proc, int allocate,
				    void *start, void *end, struct vm_area_struct *vma)
{
	void *page_addr;
	unsigned long user_page_addr;
	struct vm_struct tmp_area;
	struct page **page;
	struct mm_struct *mm;

	binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
		     "%d: %s pages %pK-%pK\n", proc->pid, allocate ? "allocate" : "free", start, end);

	if (end <= start)
		return 0;

	trace_binder_update_page_range(proc, allocate, start, end);

	if (vma)
		mm = NULL;
	else
		mm = get_task_mm(proc->tsk);

	if (mm) {
		down_write(&mm->mmap_sem);
		vma = proc->vma;
		if (vma && mm != proc->vma_vm_mm) {
			pr_err("%d: vma mm and task mm mismatch\n", proc->pid);
			vma = NULL;
		}
	}

	if (allocate == 0)
		goto free_range;

	if (vma == NULL) {
		pr_err
		    ("%d: binder_alloc_buf failed to map pages in userspace, no vma\n", proc->pid);
		goto err_no_vma;
	}

	for (page_addr = start; page_addr < end; page_addr += PAGE_SIZE) {
		int ret;

		page = &proc->pages[(page_addr - proc->buffer) / PAGE_SIZE];

		BUG_ON(*page);
		*page = alloc_page(GFP_KERNEL | __GFP_HIGHMEM | __GFP_ZERO);
		if (*page == NULL) {
			pr_err("%d: binder_alloc_buf failed for page at %pK\n",
			       proc->pid, page_addr);
			goto err_alloc_page_failed;
		}
#ifdef MTK_BINDER_PAGE_USED_RECORD
		binder_page_used++;
		proc->page_used++;
		if (binder_page_used > binder_page_used_peak)
			binder_page_used_peak = binder_page_used;
		if (proc->page_used > proc->page_used_peak)
			proc->page_used_peak = proc->page_used;
#endif
		tmp_area.addr = page_addr;
		tmp_area.size = PAGE_SIZE + PAGE_SIZE /* guard page? */;
		ret = map_vm_area(&tmp_area, PAGE_KERNEL, page);
		if (ret) {
			pr_err
			    ("%d: binder_alloc_buf failed to map page at %pK in kernel\n",
			     proc->pid, page_addr);
			goto err_map_kernel_failed;
		}
		user_page_addr = (uintptr_t) page_addr + proc->user_buffer_offset;
		ret = vm_insert_page(vma, user_page_addr, page[0]);
		if (ret) {
			pr_err
			    ("%d: binder_alloc_buf failed to map page at %lx in userspace\n",
			     proc->pid, user_page_addr);
			goto err_vm_insert_page_failed;
		}
		/* vm_insert_page does not seem to increment the refcount */
	}
	if (mm) {
		up_write(&mm->mmap_sem);
		mmput(mm);
	}
	return 0;

free_range:
	for (page_addr = end - PAGE_SIZE; page_addr >= start; page_addr -= PAGE_SIZE) {
		page = &proc->pages[(page_addr - proc->buffer) / PAGE_SIZE];
		if (vma)
			zap_page_range(vma, (uintptr_t) page_addr +
				       proc->user_buffer_offset, PAGE_SIZE, NULL);
err_vm_insert_page_failed:
		unmap_kernel_range((unsigned long)page_addr, PAGE_SIZE);
err_map_kernel_failed:
		__free_page(*page);
		*page = NULL;
#ifdef MTK_BINDER_PAGE_USED_RECORD
		if (binder_page_used > 0)
			binder_page_used--;
		if (proc->page_used > 0)
			proc->page_used--;
#endif
err_alloc_page_failed:
		;
	}
err_no_vma:
	if (mm) {
		up_write(&mm->mmap_sem);
		mmput(mm);
	}
	return -ENOMEM;
}

static struct binder_buffer *binder_alloc_buf(struct binder_proc *proc,
					      size_t data_size, size_t offsets_size, int is_async)
{
	struct rb_node *n = proc->free_buffers.rb_node;
	struct binder_buffer *buffer;
	size_t buffer_size;
	struct rb_node *best_fit = NULL;
	void *has_page_addr;
	void *end_page_addr;
	size_t size;
#ifdef MTK_BINDER_DEBUG
	size_t proc_max_size;
#endif
	if (proc->vma == NULL) {
		pr_err("%d: binder_alloc_buf, no vma\n", proc->pid);
		return NULL;
	}

	size = ALIGN(data_size, sizeof(void *)) + ALIGN(offsets_size, sizeof(void *));

	if (size < data_size || size < offsets_size) {
		binder_user_error
		    ("%d: got transaction with invalid size %zd-%zd\n",
		     proc->pid, data_size, offsets_size);
		return NULL;
	}
#ifdef MTK_BINDER_DEBUG
	proc_max_size = (is_async ? (proc->buffer_size / 2) : proc->buffer_size);

	if (proc_max_size < size + sizeof(struct binder_buffer)) {
		binder_user_error("%d: got transaction with too large size %s alloc size %zd-%zd allowed size %zd\n",
				  proc->pid, is_async ? "async" : "sync",
				  data_size, offsets_size,
				  (proc_max_size - sizeof(struct binder_buffer)));
		return NULL;
	}
#endif
	if (is_async && proc->free_async_space < size + sizeof(struct binder_buffer)) {
#ifdef MTK_BINDER_DEBUG
		pr_err("%d: binder_alloc_buf size %zd failed, no async space left (%zd)\n",
		       proc->pid, size, proc->free_async_space);
#else
		binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
			     "%d: binder_alloc_buf size %zd failed, no async space left\n",
			     proc->pid, size);
#endif
#ifdef BINDER_MONITOR
		binder_check_buf(proc, size, 1);
#endif
		return NULL;
	}

	while (n) {
		buffer = rb_entry(n, struct binder_buffer, rb_node);
		BUG_ON(!buffer->free);
		buffer_size = binder_buffer_size(proc, buffer);

		if (size < buffer_size) {
			best_fit = n;
			n = n->rb_left;
		} else if (size > buffer_size)
			n = n->rb_right;
		else {
			best_fit = n;
			break;
		}
	}
#ifdef BINDER_MONITOR
	if (log_disable & BINDER_BUF_WARN) {
		if (size > 64) {
			pr_err
			    ("%d: binder_alloc_buf size %zd failed, UT auto triggerd!\n",
			     proc->pid, size);
			binder_check_buf(proc, size, 0);
		}
	}
#endif
	if (best_fit == NULL) {
		pr_err("%d: binder_alloc_buf size %zd failed, no address space\n", proc->pid, size);
#ifdef BINDER_MONITOR
		binder_check_buf(proc, size, 0);
#endif
		return NULL;
	}
	if (n == NULL) {
		buffer = rb_entry(best_fit, struct binder_buffer, rb_node);
		buffer_size = binder_buffer_size(proc, buffer);
	}

	binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
		     "%d: binder_alloc_buf size %zd got buffer %pK size %zd\n",
		     proc->pid, size, buffer, buffer_size);

	has_page_addr = (void *)(((uintptr_t) buffer->data + buffer_size) & PAGE_MASK);
	if (n == NULL) {
		if (size + sizeof(struct binder_buffer) + 4 >= buffer_size)
			buffer_size = size;	/* no room for other buffers */
		else
			buffer_size = size + sizeof(struct binder_buffer);
	}
	end_page_addr = (void *)PAGE_ALIGN((uintptr_t) buffer->data + buffer_size);
	if (end_page_addr > has_page_addr)
		end_page_addr = has_page_addr;
	if (binder_update_page_range(proc, 1,
				     (void *)PAGE_ALIGN((uintptr_t) buffer->data), end_page_addr,
				     NULL))
		return NULL;

	rb_erase(best_fit, &proc->free_buffers);
	buffer->free = 0;
	binder_insert_allocated_buffer(proc, buffer);
	if (buffer_size != size) {
		struct binder_buffer *new_buffer = (void *)buffer->data + size;

		list_add(&new_buffer->entry, &buffer->entry);
		new_buffer->free = 1;
		binder_insert_free_buffer(proc, new_buffer);
	}
	binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
		     "%d: binder_alloc_buf size %zd got %pK\n", proc->pid, size, buffer);
	buffer->data_size = data_size;
	buffer->offsets_size = offsets_size;
	buffer->async_transaction = is_async;
	if (is_async) {
		proc->free_async_space -= size + sizeof(struct binder_buffer);
		binder_debug(BINDER_DEBUG_BUFFER_ALLOC_ASYNC,
			     "%d: binder_alloc_buf size %zd async free %zd\n",
			     proc->pid, size, proc->free_async_space);
	}

	return buffer;
}

static void *buffer_start_page(struct binder_buffer *buffer)
{
	return (void *)((uintptr_t) buffer & PAGE_MASK);
}

static void *buffer_end_page(struct binder_buffer *buffer)
{
	return (void *)(((uintptr_t) (buffer + 1) - 1) & PAGE_MASK);
}

static void binder_delete_free_buffer(struct binder_proc *proc, struct binder_buffer *buffer)
{
	struct binder_buffer *prev, *next = NULL;
	int free_page_end = 1;
	int free_page_start = 1;

	BUG_ON(proc->buffers.next == &buffer->entry);
	prev = list_entry(buffer->entry.prev, struct binder_buffer, entry);
	BUG_ON(!prev->free);
	if (buffer_end_page(prev) == buffer_start_page(buffer)) {
		free_page_start = 0;
		if (buffer_end_page(prev) == buffer_end_page(buffer))
			free_page_end = 0;
		binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
			     "%d: merge free, buffer %pK share page with %pK\n",
			     proc->pid, buffer, prev);
	}

	if (!list_is_last(&buffer->entry, &proc->buffers)) {
		next = list_entry(buffer->entry.next, struct binder_buffer, entry);
		if (buffer_start_page(next) == buffer_end_page(buffer)) {
			free_page_end = 0;
			if (buffer_start_page(next) == buffer_start_page(buffer))
				free_page_start = 0;
			binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
				     "%d: merge free, buffer %pK share page with %pK\n",
				     proc->pid, buffer, prev);
		}
	}
	list_del(&buffer->entry);
	if (free_page_start || free_page_end) {
		binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
			     "%d: merge free, buffer %pK do not share page%s%s with %pK or %pK\n",
			     proc->pid, buffer, free_page_start ? "" : " end",
			     free_page_end ? "" : " start", prev, next);
		binder_update_page_range(proc, 0, free_page_start ?
					 buffer_start_page(buffer) :
					 buffer_end_page(buffer),
					 (free_page_end ?
					  buffer_end_page(buffer) :
					  buffer_start_page(buffer)) + PAGE_SIZE, NULL);
	}
}

static void binder_free_buf(struct binder_proc *proc, struct binder_buffer *buffer)
{
	size_t size, buffer_size;

	buffer_size = binder_buffer_size(proc, buffer);

	size = ALIGN(buffer->data_size, sizeof(void *)) +
	    ALIGN(buffer->offsets_size, sizeof(void *));

	binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
		     "%d: binder_free_buf %pK size %zd buffer_size %zd\n",
		     proc->pid, buffer, size, buffer_size);

	BUG_ON(buffer->free);
	BUG_ON(size > buffer_size);
	BUG_ON(buffer->transaction != NULL);
	BUG_ON((void *)buffer < proc->buffer);
	BUG_ON((void *)buffer > proc->buffer + proc->buffer_size);
#ifdef BINDER_MONITOR
	buffer->log_entry = NULL;
#endif

	if (buffer->async_transaction) {
		proc->free_async_space += size + sizeof(struct binder_buffer);

		binder_debug(BINDER_DEBUG_BUFFER_ALLOC_ASYNC,
			     "%d: binder_free_buf size %zd async free %zd\n",
			     proc->pid, size, proc->free_async_space);
	}

	binder_update_page_range(proc, 0,
				 (void *)PAGE_ALIGN((uintptr_t) buffer->data),
				 (void
				  *)(((uintptr_t) buffer->data + buffer_size) & PAGE_MASK), NULL);
	rb_erase(&buffer->rb_node, &proc->allocated_buffers);
	buffer->free = 1;
	if (!list_is_last(&buffer->entry, &proc->buffers)) {
		struct binder_buffer *next = list_entry(buffer->entry.next,
							struct binder_buffer,
							entry);
		if (next->free) {
			rb_erase(&next->rb_node, &proc->free_buffers);
			binder_delete_free_buffer(proc, next);
		}
	}
	if (proc->buffers.next != &buffer->entry) {
		struct binder_buffer *prev = list_entry(buffer->entry.prev,
							struct binder_buffer,
							entry);
		if (prev->free) {
			binder_delete_free_buffer(proc, buffer);
			rb_erase(&prev->rb_node, &proc->free_buffers);
			buffer = prev;
		}
	}
	binder_insert_free_buffer(proc, buffer);
}

static struct binder_node *binder_get_node(struct binder_proc *proc, binder_uintptr_t ptr)
=======
		     retval == -ERESTARTNOHAND ||
		     retval == -ERESTART_RESTARTBLOCK))
		retval = -EINTR;
err:
	mutex_unlock(&proc->files_lock);
	return retval;
}

static bool binder_has_work_ilocked(struct binder_thread *thread,
				    bool do_proc_work)
{
	return thread->process_todo ||
		thread->looper_need_return ||
		(do_proc_work &&
		 !binder_worklist_empty_ilocked(&thread->proc->todo));
}

static bool binder_has_work(struct binder_thread *thread, bool do_proc_work)
{
	bool has_work;

	binder_inner_proc_lock(thread->proc);
	has_work = binder_has_work_ilocked(thread, do_proc_work);
	binder_inner_proc_unlock(thread->proc);

	return has_work;
}

static bool binder_available_for_proc_work_ilocked(struct binder_thread *thread)
{
	return !thread->transaction_stack &&
		binder_worklist_empty_ilocked(&thread->todo) &&
		(thread->looper & (BINDER_LOOPER_STATE_ENTERED |
				   BINDER_LOOPER_STATE_REGISTERED));
}

static void binder_wakeup_poll_threads_ilocked(struct binder_proc *proc,
					       bool sync)
{
	struct rb_node *n;
	struct binder_thread *thread;

	for (n = rb_first(&proc->threads); n != NULL; n = rb_next(n)) {
		thread = rb_entry(n, struct binder_thread, rb_node);
		if (thread->looper & BINDER_LOOPER_STATE_POLL &&
		    binder_available_for_proc_work_ilocked(thread)) {
			if (sync)
				wake_up_interruptible_sync(&thread->wait);
			else
				wake_up_interruptible(&thread->wait);
		}
	}
}

/**
 * binder_select_thread_ilocked() - selects a thread for doing proc work.
 * @proc:	process to select a thread from
 *
 * Note that calling this function moves the thread off the waiting_threads
 * list, so it can only be woken up by the caller of this function, or a
 * signal. Therefore, callers *should* always wake up the thread this function
 * returns.
 *
 * Return:	If there's a thread currently waiting for process work,
 *		returns that thread. Otherwise returns NULL.
 */
static struct binder_thread *
binder_select_thread_ilocked(struct binder_proc *proc)
{
	struct binder_thread *thread;

	assert_spin_locked(&proc->inner_lock);
	thread = list_first_entry_or_null(&proc->waiting_threads,
					  struct binder_thread,
					  waiting_thread_node);

	if (thread)
		list_del_init(&thread->waiting_thread_node);

	return thread;
}

/**
 * binder_wakeup_thread_ilocked() - wakes up a thread for doing proc work.
 * @proc:	process to wake up a thread in
 * @thread:	specific thread to wake-up (may be NULL)
 * @sync:	whether to do a synchronous wake-up
 *
 * This function wakes up a thread in the @proc process.
 * The caller may provide a specific thread to wake-up in
 * the @thread parameter. If @thread is NULL, this function
 * will wake up threads that have called poll().
 *
 * Note that for this function to work as expected, callers
 * should first call binder_select_thread() to find a thread
 * to handle the work (if they don't have a thread already),
 * and pass the result into the @thread parameter.
 */
static void binder_wakeup_thread_ilocked(struct binder_proc *proc,
					 struct binder_thread *thread,
					 bool sync)
{
	assert_spin_locked(&proc->inner_lock);

	if (thread) {
		if (sync)
			wake_up_interruptible_sync(&thread->wait);
		else
			wake_up_interruptible(&thread->wait);
		return;
	}

	/* Didn't find a thread waiting for proc work; this can happen
	 * in two scenarios:
	 * 1. All threads are busy handling transactions
	 *    In that case, one of those threads should call back into
	 *    the kernel driver soon and pick up this work.
	 * 2. Threads are using the (e)poll interface, in which case
	 *    they may be blocked on the waitqueue without having been
	 *    added to waiting_threads. For this case, we just iterate
	 *    over all threads not handling transaction work, and
	 *    wake them all up. We wake all because we don't know whether
	 *    a thread that called into (e)poll is handling non-binder
	 *    work currently.
	 */
	binder_wakeup_poll_threads_ilocked(proc, sync);
}

static void binder_wakeup_proc_ilocked(struct binder_proc *proc)
{
	struct binder_thread *thread = binder_select_thread_ilocked(proc);

	binder_wakeup_thread_ilocked(proc, thread, /* sync = */false);
}

static bool is_rt_policy(int policy)
{
	return policy == SCHED_FIFO || policy == SCHED_RR;
}

static bool is_fair_policy(int policy)
{
	return policy == SCHED_NORMAL || policy == SCHED_BATCH;
}

static bool binder_supported_policy(int policy)
{
	return is_fair_policy(policy) || is_rt_policy(policy);
}

static int to_userspace_prio(int policy, int kernel_priority)
{
	if (is_fair_policy(policy))
		return PRIO_TO_NICE(kernel_priority);
	else
		return MAX_USER_RT_PRIO - 1 - kernel_priority;
}

static int to_kernel_prio(int policy, int user_priority)
{
	if (is_fair_policy(policy))
		return NICE_TO_PRIO(user_priority);
	else
		return MAX_USER_RT_PRIO - 1 - user_priority;
}

static void binder_do_set_priority(struct task_struct *task,
				   struct binder_priority desired,
				   bool verify)
{
	int priority; /* user-space prio value */
	bool has_cap_nice;
	unsigned int policy = desired.sched_policy;

	if (task->policy == policy && task->normal_prio == desired.prio)
		return;

	has_cap_nice = has_capability_noaudit(task, CAP_SYS_NICE);

	priority = to_userspace_prio(policy, desired.prio);

	if (verify && is_rt_policy(policy) && !has_cap_nice) {
		long max_rtprio = task_rlimit(task, RLIMIT_RTPRIO);

		if (max_rtprio == 0) {
			policy = SCHED_NORMAL;
			priority = MIN_NICE;
		} else if (priority > max_rtprio) {
			priority = max_rtprio;
		}
	}

	if (verify && is_fair_policy(policy) && !has_cap_nice) {
		long min_nice = rlimit_to_nice(task_rlimit(task, RLIMIT_NICE));

		if (min_nice > MAX_NICE) {
			binder_user_error("%d RLIMIT_NICE not set\n",
					  task->pid);
			return;
		} else if (priority < min_nice) {
			priority = min_nice;
		}
	}

	if (policy != desired.sched_policy ||
	    to_kernel_prio(policy, priority) != desired.prio)
		binder_debug(BINDER_DEBUG_PRIORITY_CAP,
			     "%d: priority %d not allowed, using %d instead\n",
			      task->pid, desired.prio,
			      to_kernel_prio(policy, priority));

	trace_binder_set_priority(task->tgid, task->pid, task->normal_prio,
				  to_kernel_prio(policy, priority),
				  desired.prio);

	/* Set the actual priority */
	if (task->policy != policy || is_rt_policy(policy)) {
		struct sched_param params;

		params.sched_priority = is_rt_policy(policy) ? priority : 0;

		sched_setscheduler_nocheck(task,
					   policy | SCHED_RESET_ON_FORK,
					   &params);
	}
	if (is_fair_policy(policy))
		set_user_nice(task, priority);
}

static void binder_set_priority(struct task_struct *task,
				struct binder_priority desired)
{
	binder_do_set_priority(task, desired, /* verify = */ true);
}

static void binder_restore_priority(struct task_struct *task,
				    struct binder_priority desired)
{
	binder_do_set_priority(task, desired, /* verify = */ false);
}

static void binder_transaction_priority(struct task_struct *task,
					struct binder_transaction *t,
					struct binder_priority node_prio,
					bool inherit_rt)
{
	struct binder_priority desired_prio = t->priority;

	if (t->set_priority_called)
		return;

	t->set_priority_called = true;
	t->saved_priority.sched_policy = task->policy;
	t->saved_priority.prio = task->normal_prio;

	if (!inherit_rt && is_rt_policy(desired_prio.sched_policy)) {
		desired_prio.prio = NICE_TO_PRIO(0);
		desired_prio.sched_policy = SCHED_NORMAL;
	}

	if (node_prio.prio < t->priority.prio ||
	    (node_prio.prio == t->priority.prio &&
	     node_prio.sched_policy == SCHED_FIFO)) {
		/*
		 * In case the minimum priority on the node is
		 * higher (lower value), use that priority. If
		 * the priority is the same, but the node uses
		 * SCHED_FIFO, prefer SCHED_FIFO, since it can
		 * run unbounded, unlike SCHED_RR.
		 */
		desired_prio = node_prio;
	}

	binder_set_priority(task, desired_prio);
}

static struct binder_node *binder_get_node_ilocked(struct binder_proc *proc,
						   binder_uintptr_t ptr)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	struct rb_node *n = proc->nodes.rb_node;
	struct binder_node *node;

<<<<<<< HEAD
=======
	assert_spin_locked(&proc->inner_lock);

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	while (n) {
		node = rb_entry(n, struct binder_node, rb_node);

		if (ptr < node->ptr)
			n = n->rb_left;
		else if (ptr > node->ptr)
			n = n->rb_right;
<<<<<<< HEAD
		else
			return node;
=======
		else {
			/*
			 * take an implicit weak reference
			 * to ensure node stays alive until
			 * call to binder_put_node()
			 */
			binder_inc_node_tmpref_ilocked(node);
			return node;
		}
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	}
	return NULL;
}

<<<<<<< HEAD
static struct binder_node *binder_new_node(struct binder_proc *proc,
					   binder_uintptr_t ptr, binder_uintptr_t cookie)
=======
static struct binder_node *binder_get_node(struct binder_proc *proc,
					   binder_uintptr_t ptr)
{
	struct binder_node *node;

	binder_inner_proc_lock(proc);
	node = binder_get_node_ilocked(proc, ptr);
	binder_inner_proc_unlock(proc);
	return node;
}

static struct binder_node *binder_init_node_ilocked(
						struct binder_proc *proc,
						struct binder_node *new_node,
						struct flat_binder_object *fp)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	struct rb_node **p = &proc->nodes.rb_node;
	struct rb_node *parent = NULL;
	struct binder_node *node;
<<<<<<< HEAD

	while (*p) {
=======
	binder_uintptr_t ptr = fp ? fp->binder : 0;
	binder_uintptr_t cookie = fp ? fp->cookie : 0;
	__u32 flags = fp ? fp->flags : 0;
	s8 priority;

	assert_spin_locked(&proc->inner_lock);

	while (*p) {

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		parent = *p;
		node = rb_entry(parent, struct binder_node, rb_node);

		if (ptr < node->ptr)
			p = &(*p)->rb_left;
		else if (ptr > node->ptr)
			p = &(*p)->rb_right;
<<<<<<< HEAD
		else
			return NULL;
	}

	node = kzalloc(sizeof(*node), GFP_KERNEL);
	if (node == NULL)
		return NULL;
	binder_stats_created(BINDER_STAT_NODE);
	rb_link_node(&node->rb_node, parent, p);
	rb_insert_color(&node->rb_node, &proc->nodes);
	node->debug_id = ++binder_last_id;
=======
		else {
			/*
			 * A matching node is already in
			 * the rb tree. Abandon the init
			 * and return it.
			 */
			binder_inc_node_tmpref_ilocked(node);
			return node;
		}
	}
	node = new_node;
	binder_stats_created(BINDER_STAT_NODE);
	node->tmp_refs++;
	rb_link_node(&node->rb_node, parent, p);
	rb_insert_color(&node->rb_node, &proc->nodes);
	node->debug_id = atomic_inc_return(&binder_last_id);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	node->proc = proc;
	node->ptr = ptr;
	node->cookie = cookie;
	node->work.type = BINDER_WORK_NODE;
<<<<<<< HEAD
=======
	priority = flags & FLAT_BINDER_FLAG_PRIORITY_MASK;
	node->sched_policy = (flags & FLAT_BINDER_FLAG_SCHED_POLICY_MASK) >>
		FLAT_BINDER_FLAG_SCHED_POLICY_SHIFT;
	node->min_priority = to_kernel_prio(node->sched_policy, priority);
	node->accept_fds = !!(flags & FLAT_BINDER_FLAG_ACCEPTS_FDS);
	node->inherit_rt = !!(flags & FLAT_BINDER_FLAG_INHERIT_RT);
	node->txn_security_ctx = !!(flags & FLAT_BINDER_FLAG_TXN_SECURITY_CTX);
	spin_lock_init(&node->lock);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	INIT_LIST_HEAD(&node->work.entry);
	INIT_LIST_HEAD(&node->async_todo);
	binder_debug(BINDER_DEBUG_INTERNAL_REFS,
		     "%d:%d node %d u%016llx c%016llx created\n",
<<<<<<< HEAD
		     proc->pid, current->pid, node->debug_id, (u64) node->ptr, (u64) node->cookie);
	return node;
}

static int binder_inc_node(struct binder_node *node, int strong, int internal,
			   struct list_head *target_list)
{
=======
		     proc->pid, current->pid, node->debug_id,
		     (u64)node->ptr, (u64)node->cookie);

	return node;
}

static struct binder_node *binder_new_node(struct binder_proc *proc,
					   struct flat_binder_object *fp)
{
	struct binder_node *node;
	struct binder_node *new_node = kzalloc(sizeof(*node), GFP_KERNEL);

	if (!new_node)
		return NULL;
	binder_inner_proc_lock(proc);
	node = binder_init_node_ilocked(proc, new_node, fp);
	binder_inner_proc_unlock(proc);
	if (node != new_node)
		/*
		 * The node was already added by another thread
		 */
		kfree(new_node);

	return node;
}

static void binder_free_node(struct binder_node *node)
{
	kfree(node);
	binder_stats_deleted(BINDER_STAT_NODE);
}

static int binder_inc_node_nilocked(struct binder_node *node, int strong,
				    int internal,
				    struct list_head *target_list)
{
	struct binder_proc *proc = node->proc;

	assert_spin_locked(&node->lock);
	if (proc)
		assert_spin_locked(&proc->inner_lock);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	if (strong) {
		if (internal) {
			if (target_list == NULL &&
			    node->internal_strong_refs == 0 &&
<<<<<<< HEAD
			    !(node == binder_context_mgr_node && node->has_strong_ref)) {
				pr_err("invalid inc strong node for %d\n", node->debug_id);
=======
			    !(node->proc &&
			      node == node->proc->context->
				      binder_context_mgr_node &&
			      node->has_strong_ref)) {
				pr_err("invalid inc strong node for %d\n",
					node->debug_id);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
				return -EINVAL;
			}
			node->internal_strong_refs++;
		} else
			node->local_strong_refs++;
		if (!node->has_strong_ref && target_list) {
<<<<<<< HEAD
			list_del_init(&node->work.entry);
			list_add_tail(&node->work.entry, target_list);
=======
			binder_dequeue_work_ilocked(&node->work);
			/*
			 * Note: this function is the only place where we queue
			 * directly to a thread->todo without using the
			 * corresponding binder_enqueue_thread_work() helper
			 * functions; in this case it's ok to not set the
			 * process_todo flag, since we know this node work will
			 * always be followed by other work that starts queue
			 * processing: in case of synchronous transactions, a
			 * BR_REPLY or BR_ERROR; in case of oneway
			 * transactions, a BR_TRANSACTION_COMPLETE.
			 */
			binder_enqueue_work_ilocked(&node->work, target_list);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		}
	} else {
		if (!internal)
			node->local_weak_refs++;
		if (!node->has_weak_ref && list_empty(&node->work.entry)) {
			if (target_list == NULL) {
<<<<<<< HEAD
				pr_err("invalid inc weak node for %d\n", node->debug_id);
				return -EINVAL;
			}
			list_add_tail(&node->work.entry, target_list);
=======
				pr_err("invalid inc weak node for %d\n",
					node->debug_id);
				return -EINVAL;
			}
			/*
			 * See comment above
			 */
			binder_enqueue_work_ilocked(&node->work, target_list);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		}
	}
	return 0;
}

<<<<<<< HEAD
static int binder_dec_node(struct binder_node *node, int strong, int internal)
{
=======
static int binder_inc_node(struct binder_node *node, int strong, int internal,
			   struct list_head *target_list)
{
	int ret;

	binder_node_inner_lock(node);
	ret = binder_inc_node_nilocked(node, strong, internal, target_list);
	binder_node_inner_unlock(node);

	return ret;
}

static bool binder_dec_node_nilocked(struct binder_node *node,
				     int strong, int internal)
{
	struct binder_proc *proc = node->proc;

	assert_spin_locked(&node->lock);
	if (proc)
		assert_spin_locked(&proc->inner_lock);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	if (strong) {
		if (internal)
			node->internal_strong_refs--;
		else
			node->local_strong_refs--;
		if (node->local_strong_refs || node->internal_strong_refs)
<<<<<<< HEAD
			return 0;
	} else {
		if (!internal)
			node->local_weak_refs--;
		if (node->local_weak_refs || !hlist_empty(&node->refs))
			return 0;
	}
	if (node->proc && (node->has_strong_ref || node->has_weak_ref)) {
		if (list_empty(&node->work.entry)) {
			list_add_tail(&node->work.entry, &node->proc->todo);
			wake_up_interruptible(&node->proc->wait);
		}
	} else {
		if (hlist_empty(&node->refs) && !node->local_strong_refs && !node->local_weak_refs) {
			list_del_init(&node->work.entry);
			if (node->proc) {
				rb_erase(&node->rb_node, &node->proc->nodes);
				binder_debug(BINDER_DEBUG_INTERNAL_REFS,
					     "refless node %d deleted\n", node->debug_id);
			} else {
				hlist_del(&node->dead_node);
				binder_debug(BINDER_DEBUG_INTERNAL_REFS,
					     "dead node %d deleted\n", node->debug_id);
			}
			kfree(node);
			binder_stats_deleted(BINDER_STAT_NODE);
		}
	}

	return 0;
}

static struct binder_ref *binder_get_ref(struct binder_proc *proc,
					uint32_t desc, bool need_strong_ref)
{
	struct rb_node *n = proc->refs_by_desc.rb_node;
	struct binder_ref *ref;

	while (n) {
		ref = rb_entry(n, struct binder_ref, rb_node_desc);

		if (desc < ref->desc) {
			n = n->rb_left;
		} else if (desc > ref->desc) {
			n = n->rb_right;
		} else if (need_strong_ref && !ref->strong) {
			binder_user_error("tried to use weak ref as strong ref\n");
			return NULL;
		} else {
			return ref;
		}
	}
	return NULL;
}

static struct binder_ref *binder_get_ref_for_node(struct binder_proc *proc,
						  struct binder_node *node)
{
	struct rb_node *n;
	struct rb_node **p = &proc->refs_by_node.rb_node;
	struct rb_node *parent = NULL;
	struct binder_ref *ref, *new_ref;
=======
			return false;
	} else {
		if (!internal)
			node->local_weak_refs--;
		if (node->local_weak_refs || node->tmp_refs ||
				!hlist_empty(&node->refs))
			return false;
	}

	if (proc && (node->has_strong_ref || node->has_weak_ref)) {
		if (list_empty(&node->work.entry)) {
			binder_enqueue_work_ilocked(&node->work, &proc->todo);
			binder_wakeup_proc_ilocked(proc);
		}
	} else {
		if (hlist_empty(&node->refs) && !node->local_strong_refs &&
		    !node->local_weak_refs && !node->tmp_refs) {
			if (proc) {
				binder_dequeue_work_ilocked(&node->work);
				rb_erase(&node->rb_node, &proc->nodes);
				binder_debug(BINDER_DEBUG_INTERNAL_REFS,
					     "refless node %d deleted\n",
					     node->debug_id);
			} else {
				BUG_ON(!list_empty(&node->work.entry));
				spin_lock(&binder_dead_nodes_lock);
				/*
				 * tmp_refs could have changed so
				 * check it again
				 */
				if (node->tmp_refs) {
					spin_unlock(&binder_dead_nodes_lock);
					return false;
				}
				hlist_del(&node->dead_node);
				spin_unlock(&binder_dead_nodes_lock);
				binder_debug(BINDER_DEBUG_INTERNAL_REFS,
					     "dead node %d deleted\n",
					     node->debug_id);
			}
			return true;
		}
	}
	return false;
}

static void binder_dec_node(struct binder_node *node, int strong, int internal)
{
	bool free_node;

	binder_node_inner_lock(node);
	free_node = binder_dec_node_nilocked(node, strong, internal);
	binder_node_inner_unlock(node);
	if (free_node)
		binder_free_node(node);
}

static void binder_inc_node_tmpref_ilocked(struct binder_node *node)
{
	/*
	 * No call to binder_inc_node() is needed since we
	 * don't need to inform userspace of any changes to
	 * tmp_refs
	 */
	node->tmp_refs++;
}

/**
 * binder_inc_node_tmpref() - take a temporary reference on node
 * @node:	node to reference
 *
 * Take reference on node to prevent the node from being freed
 * while referenced only by a local variable. The inner lock is
 * needed to serialize with the node work on the queue (which
 * isn't needed after the node is dead). If the node is dead
 * (node->proc is NULL), use binder_dead_nodes_lock to protect
 * node->tmp_refs against dead-node-only cases where the node
 * lock cannot be acquired (eg traversing the dead node list to
 * print nodes)
 */
static void binder_inc_node_tmpref(struct binder_node *node)
{
	binder_node_lock(node);
	if (node->proc)
		binder_inner_proc_lock(node->proc);
	else
		spin_lock(&binder_dead_nodes_lock);
	binder_inc_node_tmpref_ilocked(node);
	if (node->proc)
		binder_inner_proc_unlock(node->proc);
	else
		spin_unlock(&binder_dead_nodes_lock);
	binder_node_unlock(node);
}

/**
 * binder_dec_node_tmpref() - remove a temporary reference on node
 * @node:	node to reference
 *
 * Release temporary reference on node taken via binder_inc_node_tmpref()
 */
static void binder_dec_node_tmpref(struct binder_node *node)
{
	bool free_node;

	binder_node_inner_lock(node);
	if (!node->proc)
		spin_lock(&binder_dead_nodes_lock);
	node->tmp_refs--;
	BUG_ON(node->tmp_refs < 0);
	if (!node->proc)
		spin_unlock(&binder_dead_nodes_lock);
	/*
	 * Call binder_dec_node() to check if all refcounts are 0
	 * and cleanup is needed. Calling with strong=0 and internal=1
	 * causes no actual reference to be released in binder_dec_node().
	 * If that changes, a change is needed here too.
	 */
	free_node = binder_dec_node_nilocked(node, 0, 1);
	binder_node_inner_unlock(node);
	if (free_node)
		binder_free_node(node);
}

static void binder_put_node(struct binder_node *node)
{
	binder_dec_node_tmpref(node);
}

static struct binder_ref *binder_get_ref_olocked(struct binder_proc *proc,
						 u32 desc, bool need_strong_ref)
{
	struct rb_node *n = proc->refs_by_desc.rb_node;
	struct binder_ref *ref;

	while (n) {
		ref = rb_entry(n, struct binder_ref, rb_node_desc);

		if (desc < ref->data.desc) {
			n = n->rb_left;
		} else if (desc > ref->data.desc) {
			n = n->rb_right;
		} else if (need_strong_ref && !ref->data.strong) {
			binder_user_error("tried to use weak ref as strong ref\n");
			return NULL;
		} else {
			return ref;
		}
	}
	return NULL;
}

/**
 * binder_get_ref_for_node_olocked() - get the ref associated with given node
 * @proc:	binder_proc that owns the ref
 * @node:	binder_node of target
 * @new_ref:	newly allocated binder_ref to be initialized or %NULL
 *
 * Look up the ref for the given node and return it if it exists
 *
 * If it doesn't exist and the caller provides a newly allocated
 * ref, initialize the fields of the newly allocated ref and insert
 * into the given proc rb_trees and node refs list.
 *
 * Return:	the ref for node. It is possible that another thread
 *		allocated/initialized the ref first in which case the
 *		returned ref would be different than the passed-in
 *		new_ref. new_ref must be kfree'd by the caller in
 *		this case.
 */
static struct binder_ref *binder_get_ref_for_node_olocked(
					struct binder_proc *proc,
					struct binder_node *node,
					struct binder_ref *new_ref)
{
	struct binder_context *context = proc->context;
	struct rb_node **p = &proc->refs_by_node.rb_node;
	struct rb_node *parent = NULL;
	struct binder_ref *ref;
	struct rb_node *n;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	while (*p) {
		parent = *p;
		ref = rb_entry(parent, struct binder_ref, rb_node_node);

		if (node < ref->node)
			p = &(*p)->rb_left;
		else if (node > ref->node)
			p = &(*p)->rb_right;
		else
			return ref;
	}
<<<<<<< HEAD
	new_ref = kzalloc(sizeof(*ref), GFP_KERNEL);
	if (new_ref == NULL)
		return NULL;
	binder_stats_created(BINDER_STAT_REF);
	new_ref->debug_id = ++binder_last_id;
=======
	if (!new_ref)
		return NULL;

	binder_stats_created(BINDER_STAT_REF);
	new_ref->data.debug_id = atomic_inc_return(&binder_last_id);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	new_ref->proc = proc;
	new_ref->node = node;
	rb_link_node(&new_ref->rb_node_node, parent, p);
	rb_insert_color(&new_ref->rb_node_node, &proc->refs_by_node);

<<<<<<< HEAD
	new_ref->desc = (node == binder_context_mgr_node) ? 0 : 1;
	for (n = rb_first(&proc->refs_by_desc); n != NULL; n = rb_next(n)) {
		ref = rb_entry(n, struct binder_ref, rb_node_desc);
		if (ref->desc > new_ref->desc)
			break;
		new_ref->desc = ref->desc + 1;
=======
	new_ref->data.desc = (node == context->binder_context_mgr_node) ? 0 : 1;
	for (n = rb_first(&proc->refs_by_desc); n != NULL; n = rb_next(n)) {
		ref = rb_entry(n, struct binder_ref, rb_node_desc);
		if (ref->data.desc > new_ref->data.desc)
			break;
		new_ref->data.desc = ref->data.desc + 1;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	}

	p = &proc->refs_by_desc.rb_node;
	while (*p) {
		parent = *p;
		ref = rb_entry(parent, struct binder_ref, rb_node_desc);

<<<<<<< HEAD
		if (new_ref->desc < ref->desc)
			p = &(*p)->rb_left;
		else if (new_ref->desc > ref->desc)
=======
		if (new_ref->data.desc < ref->data.desc)
			p = &(*p)->rb_left;
		else if (new_ref->data.desc > ref->data.desc)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			p = &(*p)->rb_right;
		else
			BUG();
	}
	rb_link_node(&new_ref->rb_node_desc, parent, p);
	rb_insert_color(&new_ref->rb_node_desc, &proc->refs_by_desc);
<<<<<<< HEAD
	if (node) {
		hlist_add_head(&new_ref->node_entry, &node->refs);

		binder_debug(BINDER_DEBUG_INTERNAL_REFS,
			     "%d new ref %d desc %d for node %d\n",
			     proc->pid, new_ref->debug_id, new_ref->desc, node->debug_id);
	} else {
		binder_debug(BINDER_DEBUG_INTERNAL_REFS,
			     "%d new ref %d desc %d for dead node\n",
			     proc->pid, new_ref->debug_id, new_ref->desc);
	}
	return new_ref;
}

static void binder_delete_ref(struct binder_ref *ref)
{
	binder_debug(BINDER_DEBUG_INTERNAL_REFS,
		     "%d delete ref %d desc %d for node %d\n",
		     ref->proc->pid, ref->debug_id, ref->desc, ref->node->debug_id);

	rb_erase(&ref->rb_node_desc, &ref->proc->refs_by_desc);
	rb_erase(&ref->rb_node_node, &ref->proc->refs_by_node);
	if (ref->strong)
		binder_dec_node(ref->node, 1, 1);
	hlist_del(&ref->node_entry);
	binder_dec_node(ref->node, 0, 1);
	if (ref->death) {
		binder_debug(BINDER_DEBUG_DEAD_BINDER,
			     "%d delete ref %d desc %d has death notification\n",
			     ref->proc->pid, ref->debug_id, ref->desc);
		list_del(&ref->death->work.entry);
		kfree(ref->death);
		binder_stats_deleted(BINDER_STAT_DEATH);
	}
	kfree(ref);
	binder_stats_deleted(BINDER_STAT_REF);
}

static int binder_inc_ref(struct binder_ref *ref, int strong, struct list_head *target_list)
=======

	binder_node_lock(node);
	hlist_add_head(&new_ref->node_entry, &node->refs);

	binder_debug(BINDER_DEBUG_INTERNAL_REFS,
		     "%d new ref %d desc %d for node %d\n",
		      proc->pid, new_ref->data.debug_id, new_ref->data.desc,
		      node->debug_id);
	binder_node_unlock(node);
	return new_ref;
}

static void binder_cleanup_ref_olocked(struct binder_ref *ref)
{
	bool delete_node = false;

	binder_debug(BINDER_DEBUG_INTERNAL_REFS,
		     "%d delete ref %d desc %d for node %d\n",
		      ref->proc->pid, ref->data.debug_id, ref->data.desc,
		      ref->node->debug_id);

	rb_erase(&ref->rb_node_desc, &ref->proc->refs_by_desc);
	rb_erase(&ref->rb_node_node, &ref->proc->refs_by_node);

	binder_node_inner_lock(ref->node);
	if (ref->data.strong)
		binder_dec_node_nilocked(ref->node, 1, 1);

	hlist_del(&ref->node_entry);
	delete_node = binder_dec_node_nilocked(ref->node, 0, 1);
	binder_node_inner_unlock(ref->node);
	/*
	 * Clear ref->node unless we want the caller to free the node
	 */
	if (!delete_node) {
		/*
		 * The caller uses ref->node to determine
		 * whether the node needs to be freed. Clear
		 * it since the node is still alive.
		 */
		ref->node = NULL;
	}

	if (ref->death) {
		binder_debug(BINDER_DEBUG_DEAD_BINDER,
			     "%d delete ref %d desc %d has death notification\n",
			      ref->proc->pid, ref->data.debug_id,
			      ref->data.desc);
		binder_dequeue_work(ref->proc, &ref->death->work);
		binder_stats_deleted(BINDER_STAT_DEATH);
	}
	binder_stats_deleted(BINDER_STAT_REF);
}

/**
 * binder_inc_ref_olocked() - increment the ref for given handle
 * @ref:         ref to be incremented
 * @strong:      if true, strong increment, else weak
 * @target_list: list to queue node work on
 *
 * Increment the ref. @ref->proc->outer_lock must be held on entry
 *
 * Return: 0, if successful, else errno
 */
static int binder_inc_ref_olocked(struct binder_ref *ref, int strong,
				  struct list_head *target_list)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	int ret;

	if (strong) {
<<<<<<< HEAD
		if (ref->strong == 0) {
=======
		if (ref->data.strong == 0) {
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			ret = binder_inc_node(ref->node, 1, 1, target_list);
			if (ret)
				return ret;
		}
<<<<<<< HEAD
		ref->strong++;
	} else {
		if (ref->weak == 0) {
=======
		ref->data.strong++;
	} else {
		if (ref->data.weak == 0) {
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			ret = binder_inc_node(ref->node, 0, 1, target_list);
			if (ret)
				return ret;
		}
<<<<<<< HEAD
		ref->weak++;
=======
		ref->data.weak++;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	}
	return 0;
}

<<<<<<< HEAD
static int binder_dec_ref(struct binder_ref *ref, int strong)
{
	if (strong) {
		if (ref->strong == 0) {
			binder_user_error
			    ("%d invalid dec strong, ref %d desc %d s %d w %d\n",
			     ref->proc->pid, ref->debug_id, ref->desc, ref->strong, ref->weak);
			return -EINVAL;
		}
		ref->strong--;
		if (ref->strong == 0) {
			int ret;

			ret = binder_dec_node(ref->node, strong, 1);
			if (ret)
				return ret;
		}
	} else {
		if (ref->weak == 0) {
			binder_user_error
			    ("%d invalid dec weak, ref %d desc %d s %d w %d\n",
			     ref->proc->pid, ref->debug_id, ref->desc, ref->strong, ref->weak);
			return -EINVAL;
		}
		ref->weak--;
	}
	if (ref->strong == 0 && ref->weak == 0)
		binder_delete_ref(ref);
	return 0;
}

static void binder_pop_transaction(struct binder_thread *target_thread,
				   struct binder_transaction *t)
{
	if (target_thread) {
		BUG_ON(target_thread->transaction_stack != t);
		BUG_ON(target_thread->transaction_stack->from != target_thread);
		target_thread->transaction_stack = target_thread->transaction_stack->from_parent;
		t->from = NULL;
	}
	t->need_reply = 0;
	if (t->buffer)
		t->buffer->transaction = NULL;
#ifdef BINDER_MONITOR
	binder_cancel_bwdog(t);
#endif
=======
/**
 * binder_dec_ref() - dec the ref for given handle
 * @ref:	ref to be decremented
 * @strong:	if true, strong decrement, else weak
 *
 * Decrement the ref.
 *
 * Return: true if ref is cleaned up and ready to be freed
 */
static bool binder_dec_ref_olocked(struct binder_ref *ref, int strong)
{
	if (strong) {
		if (ref->data.strong == 0) {
			binder_user_error("%d invalid dec strong, ref %d desc %d s %d w %d\n",
					  ref->proc->pid, ref->data.debug_id,
					  ref->data.desc, ref->data.strong,
					  ref->data.weak);
			return false;
		}
		ref->data.strong--;
		if (ref->data.strong == 0)
			binder_dec_node(ref->node, strong, 1);
	} else {
		if (ref->data.weak == 0) {
			binder_user_error("%d invalid dec weak, ref %d desc %d s %d w %d\n",
					  ref->proc->pid, ref->data.debug_id,
					  ref->data.desc, ref->data.strong,
					  ref->data.weak);
			return false;
		}
		ref->data.weak--;
	}
	if (ref->data.strong == 0 && ref->data.weak == 0) {
		binder_cleanup_ref_olocked(ref);
		return true;
	}
	return false;
}

/**
 * binder_get_node_from_ref() - get the node from the given proc/desc
 * @proc:	proc containing the ref
 * @desc:	the handle associated with the ref
 * @need_strong_ref: if true, only return node if ref is strong
 * @rdata:	the id/refcount data for the ref
 *
 * Given a proc and ref handle, return the associated binder_node
 *
 * Return: a binder_node or NULL if not found or not strong when strong required
 */
static struct binder_node *binder_get_node_from_ref(
		struct binder_proc *proc,
		u32 desc, bool need_strong_ref,
		struct binder_ref_data *rdata)
{
	struct binder_node *node;
	struct binder_ref *ref;

	binder_proc_lock(proc);
	ref = binder_get_ref_olocked(proc, desc, need_strong_ref);
	if (!ref)
		goto err_no_ref;
	node = ref->node;
	/*
	 * Take an implicit reference on the node to ensure
	 * it stays alive until the call to binder_put_node()
	 */
	binder_inc_node_tmpref(node);
	if (rdata)
		*rdata = ref->data;
	binder_proc_unlock(proc);

	return node;

err_no_ref:
	binder_proc_unlock(proc);
	return NULL;
}

/**
 * binder_free_ref() - free the binder_ref
 * @ref:	ref to free
 *
 * Free the binder_ref. Free the binder_node indicated by ref->node
 * (if non-NULL) and the binder_ref_death indicated by ref->death.
 */
static void binder_free_ref(struct binder_ref *ref)
{
	if (ref->node)
		binder_free_node(ref->node);
	kfree(ref->death);
	kfree(ref);
}

/**
 * binder_update_ref_for_handle() - inc/dec the ref for given handle
 * @proc:	proc containing the ref
 * @desc:	the handle associated with the ref
 * @increment:	true=inc reference, false=dec reference
 * @strong:	true=strong reference, false=weak reference
 * @rdata:	the id/refcount data for the ref
 *
 * Given a proc and ref handle, increment or decrement the ref
 * according to "increment" arg.
 *
 * Return: 0 if successful, else errno
 */
static int binder_update_ref_for_handle(struct binder_proc *proc,
		uint32_t desc, bool increment, bool strong,
		struct binder_ref_data *rdata)
{
	int ret = 0;
	struct binder_ref *ref;
	bool delete_ref = false;

	binder_proc_lock(proc);
	ref = binder_get_ref_olocked(proc, desc, strong);
	if (!ref) {
		ret = -EINVAL;
		goto err_no_ref;
	}
	if (increment)
		ret = binder_inc_ref_olocked(ref, strong, NULL);
	else
		delete_ref = binder_dec_ref_olocked(ref, strong);

	if (rdata)
		*rdata = ref->data;
	binder_proc_unlock(proc);

	if (delete_ref)
		binder_free_ref(ref);
	return ret;

err_no_ref:
	binder_proc_unlock(proc);
	return ret;
}

/**
 * binder_dec_ref_for_handle() - dec the ref for given handle
 * @proc:	proc containing the ref
 * @desc:	the handle associated with the ref
 * @strong:	true=strong reference, false=weak reference
 * @rdata:	the id/refcount data for the ref
 *
 * Just calls binder_update_ref_for_handle() to decrement the ref.
 *
 * Return: 0 if successful, else errno
 */
static int binder_dec_ref_for_handle(struct binder_proc *proc,
		uint32_t desc, bool strong, struct binder_ref_data *rdata)
{
	return binder_update_ref_for_handle(proc, desc, false, strong, rdata);
}


/**
 * binder_inc_ref_for_node() - increment the ref for given proc/node
 * @proc:	 proc containing the ref
 * @node:	 target node
 * @strong:	 true=strong reference, false=weak reference
 * @target_list: worklist to use if node is incremented
 * @rdata:	 the id/refcount data for the ref
 *
 * Given a proc and node, increment the ref. Create the ref if it
 * doesn't already exist
 *
 * Return: 0 if successful, else errno
 */
static int binder_inc_ref_for_node(struct binder_proc *proc,
			struct binder_node *node,
			bool strong,
			struct list_head *target_list,
			struct binder_ref_data *rdata)
{
	struct binder_ref *ref;
	struct binder_ref *new_ref = NULL;
	int ret = 0;

	binder_proc_lock(proc);
	ref = binder_get_ref_for_node_olocked(proc, node, NULL);
	if (!ref) {
		binder_proc_unlock(proc);
		new_ref = kzalloc(sizeof(*ref), GFP_KERNEL);
		if (!new_ref)
			return -ENOMEM;
		binder_proc_lock(proc);
		ref = binder_get_ref_for_node_olocked(proc, node, new_ref);
	}
	ret = binder_inc_ref_olocked(ref, strong, target_list);
	*rdata = ref->data;
	binder_proc_unlock(proc);
	if (new_ref && ref != new_ref)
		/*
		 * Another thread created the ref first so
		 * free the one we allocated
		 */
		kfree(new_ref);
	return ret;
}

static void binder_pop_transaction_ilocked(struct binder_thread *target_thread,
					   struct binder_transaction *t)
{
	BUG_ON(!target_thread);
	assert_spin_locked(&target_thread->proc->inner_lock);
	BUG_ON(target_thread->transaction_stack != t);
	BUG_ON(target_thread->transaction_stack->from != target_thread);
	target_thread->transaction_stack =
		target_thread->transaction_stack->from_parent;
	t->from = NULL;
}

/**
 * binder_thread_dec_tmpref() - decrement thread->tmp_ref
 * @thread:	thread to decrement
 *
 * A thread needs to be kept alive while being used to create or
 * handle a transaction. binder_get_txn_from() is used to safely
 * extract t->from from a binder_transaction and keep the thread
 * indicated by t->from from being freed. When done with that
 * binder_thread, this function is called to decrement the
 * tmp_ref and free if appropriate (thread has been released
 * and no transaction being processed by the driver)
 */
static void binder_thread_dec_tmpref(struct binder_thread *thread)
{
	/*
	 * atomic is used to protect the counter value while
	 * it cannot reach zero or thread->is_dead is false
	 */
	binder_inner_proc_lock(thread->proc);
	atomic_dec(&thread->tmp_ref);
	if (thread->is_dead && !atomic_read(&thread->tmp_ref)) {
		binder_inner_proc_unlock(thread->proc);
		binder_free_thread(thread);
		return;
	}
	binder_inner_proc_unlock(thread->proc);
}

/**
 * binder_proc_dec_tmpref() - decrement proc->tmp_ref
 * @proc:	proc to decrement
 *
 * A binder_proc needs to be kept alive while being used to create or
 * handle a transaction. proc->tmp_ref is incremented when
 * creating a new transaction or the binder_proc is currently in-use
 * by threads that are being released. When done with the binder_proc,
 * this function is called to decrement the counter and free the
 * proc if appropriate (proc has been released, all threads have
 * been released and not currenly in-use to process a transaction).
 */
static void binder_proc_dec_tmpref(struct binder_proc *proc)
{
	binder_inner_proc_lock(proc);
	proc->tmp_ref--;
	if (proc->is_dead && RB_EMPTY_ROOT(&proc->threads) &&
			!proc->tmp_ref) {
		binder_inner_proc_unlock(proc);
		binder_free_proc(proc);
		return;
	}
	binder_inner_proc_unlock(proc);
}

/**
 * binder_get_txn_from() - safely extract the "from" thread in transaction
 * @t:	binder transaction for t->from
 *
 * Atomically return the "from" thread and increment the tmp_ref
 * count for the thread to ensure it stays alive until
 * binder_thread_dec_tmpref() is called.
 *
 * Return: the value of t->from
 */
static struct binder_thread *binder_get_txn_from(
		struct binder_transaction *t)
{
	struct binder_thread *from;

	spin_lock(&t->lock);
	from = t->from;
	if (from)
		atomic_inc(&from->tmp_ref);
	spin_unlock(&t->lock);
	return from;
}

/**
 * binder_get_txn_from_and_acq_inner() - get t->from and acquire inner lock
 * @t:	binder transaction for t->from
 *
 * Same as binder_get_txn_from() except it also acquires the proc->inner_lock
 * to guarantee that the thread cannot be released while operating on it.
 * The caller must call binder_inner_proc_unlock() to release the inner lock
 * as well as call binder_dec_thread_txn() to release the reference.
 *
 * Return: the value of t->from
 */
static struct binder_thread *binder_get_txn_from_and_acq_inner(
		struct binder_transaction *t)
{
	struct binder_thread *from;

	from = binder_get_txn_from(t);
	if (!from)
		return NULL;
	binder_inner_proc_lock(from->proc);
	if (t->from) {
		BUG_ON(from != t->from);
		return from;
	}
	binder_inner_proc_unlock(from->proc);
	binder_thread_dec_tmpref(from);
	return NULL;
}

static void binder_free_transaction(struct binder_transaction *t)
{
	struct binder_proc *target_proc = t->to_proc;

	if (target_proc) {
		binder_inner_proc_lock(target_proc);
		if (t->buffer)
			t->buffer->transaction = NULL;
		binder_inner_proc_unlock(target_proc);
	}
	/*
	 * If the transaction has no target_proc, then
	 * t->buffer->transaction has already been cleared.
	 */
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	kfree(t);
	binder_stats_deleted(BINDER_STAT_TRANSACTION);
}

<<<<<<< HEAD
static void binder_send_failed_reply(struct binder_transaction *t, uint32_t error_code)
=======
static void binder_send_failed_reply(struct binder_transaction *t,
				     uint32_t error_code)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	struct binder_thread *target_thread;
	struct binder_transaction *next;

	BUG_ON(t->flags & TF_ONE_WAY);
	while (1) {
<<<<<<< HEAD
		target_thread = t->from;
		if (target_thread) {
			if (target_thread->return_error != BR_OK &&
			    target_thread->return_error2 == BR_OK) {
				target_thread->return_error2 = target_thread->return_error;
				target_thread->return_error = BR_OK;
			}
			if (target_thread->return_error == BR_OK) {
				binder_debug(BINDER_DEBUG_FAILED_TRANSACTION,
					     "send failed reply for transaction %d to %d:%d\n",
					     t->debug_id,
					     target_thread->proc->pid, target_thread->pid);

				binder_pop_transaction(target_thread, t);
				target_thread->return_error = error_code;
				wake_up_interruptible(&target_thread->wait);
			} else {
				pr_err
				    ("reply failed, target thread, %d:%d, has error code %d already\n",
				     target_thread->proc->pid,
				     target_thread->pid, target_thread->return_error);
			}
=======
		target_thread = binder_get_txn_from_and_acq_inner(t);
		if (target_thread) {
			binder_debug(BINDER_DEBUG_FAILED_TRANSACTION,
				     "send failed reply for transaction %d to %d:%d\n",
				      t->debug_id,
				      target_thread->proc->pid,
				      target_thread->pid);

			binder_pop_transaction_ilocked(target_thread, t);
			if (target_thread->reply_error.cmd == BR_OK) {
				target_thread->reply_error.cmd = error_code;
				binder_enqueue_thread_work_ilocked(
					target_thread,
					&target_thread->reply_error.work);
				wake_up_interruptible(&target_thread->wait);
			} else {
				WARN(1, "Unexpected reply error: %u\n",
						target_thread->reply_error.cmd);
			}
			binder_inner_proc_unlock(target_thread->proc);
			binder_thread_dec_tmpref(target_thread);
			binder_free_transaction(t);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			return;
		}
		next = t->from_parent;

		binder_debug(BINDER_DEBUG_FAILED_TRANSACTION,
<<<<<<< HEAD
			     "send failed reply for transaction %d, target dead\n", t->debug_id);

		binder_pop_transaction(target_thread, t);
=======
			     "send failed reply for transaction %d, target dead\n",
			     t->debug_id);

		binder_free_transaction(t);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		if (next == NULL) {
			binder_debug(BINDER_DEBUG_DEAD_BINDER,
				     "reply failed, no target thread at root\n");
			return;
		}
		t = next;
		binder_debug(BINDER_DEBUG_DEAD_BINDER,
<<<<<<< HEAD
			     "reply failed, no target thread -- retry %d\n", t->debug_id);
	}
}

=======
			     "reply failed, no target thread -- retry %d\n",
			      t->debug_id);
	}
}

/**
 * binder_cleanup_transaction() - cleans up undelivered transaction
 * @t:		transaction that needs to be cleaned up
 * @reason:	reason the transaction wasn't delivered
 * @error_code:	error to return to caller (if synchronous call)
 */
static void binder_cleanup_transaction(struct binder_transaction *t,
				       const char *reason,
				       uint32_t error_code)
{
	if (t->buffer->target_node && !(t->flags & TF_ONE_WAY)) {
		binder_send_failed_reply(t, error_code);
	} else {
		binder_debug(BINDER_DEBUG_DEAD_TRANSACTION,
			"undelivered transaction %d, %s\n",
			t->debug_id, reason);
		binder_free_transaction(t);
	}
}

/**
 * binder_validate_object() - checks for a valid metadata object in a buffer.
 * @buffer:	binder_buffer that we're parsing.
 * @offset:	offset in the buffer at which to validate an object.
 *
 * Return:	If there's a valid metadata object at @offset in @buffer, the
 *		size of that object. Otherwise, it returns zero.
 */
static size_t binder_validate_object(struct binder_buffer *buffer, u64 offset)
{
	/* Check if we can read a header first */
	struct binder_object_header *hdr;
	size_t object_size = 0;

	if (offset > buffer->data_size - sizeof(*hdr) ||
	    buffer->data_size < sizeof(*hdr) ||
	    !IS_ALIGNED(offset, sizeof(u32)))
		return 0;

	/* Ok, now see if we can read a complete object. */
	hdr = (struct binder_object_header *)(buffer->data + offset);
	switch (hdr->type) {
	case BINDER_TYPE_BINDER:
	case BINDER_TYPE_WEAK_BINDER:
	case BINDER_TYPE_HANDLE:
	case BINDER_TYPE_WEAK_HANDLE:
		object_size = sizeof(struct flat_binder_object);
		break;
	case BINDER_TYPE_FD:
		object_size = sizeof(struct binder_fd_object);
		break;
	case BINDER_TYPE_PTR:
		object_size = sizeof(struct binder_buffer_object);
		break;
	case BINDER_TYPE_FDA:
		object_size = sizeof(struct binder_fd_array_object);
		break;
	default:
		return 0;
	}
	if (offset <= buffer->data_size - object_size &&
	    buffer->data_size >= object_size)
		return object_size;
	else
		return 0;
}

/**
 * binder_validate_ptr() - validates binder_buffer_object in a binder_buffer.
 * @b:		binder_buffer containing the object
 * @index:	index in offset array at which the binder_buffer_object is
 *		located
 * @start:	points to the start of the offset array
 * @num_valid:	the number of valid offsets in the offset array
 *
 * Return:	If @index is within the valid range of the offset array
 *		described by @start and @num_valid, and if there's a valid
 *		binder_buffer_object at the offset found in index @index
 *		of the offset array, that object is returned. Otherwise,
 *		%NULL is returned.
 *		Note that the offset found in index @index itself is not
 *		verified; this function assumes that @num_valid elements
 *		from @start were previously verified to have valid offsets.
 */
static struct binder_buffer_object *binder_validate_ptr(struct binder_buffer *b,
							binder_size_t index,
							binder_size_t *start,
							binder_size_t num_valid)
{
	struct binder_buffer_object *buffer_obj;
	binder_size_t *offp;

	if (index >= num_valid)
		return NULL;

	offp = start + index;
	buffer_obj = (struct binder_buffer_object *)(b->data + *offp);
	if (buffer_obj->hdr.type != BINDER_TYPE_PTR)
		return NULL;

	return buffer_obj;
}

/**
 * binder_validate_fixup() - validates pointer/fd fixups happen in order.
 * @b:			transaction buffer
 * @objects_start	start of objects buffer
 * @buffer:		binder_buffer_object in which to fix up
 * @offset:		start offset in @buffer to fix up
 * @last_obj:		last binder_buffer_object that we fixed up in
 * @last_min_offset:	minimum fixup offset in @last_obj
 *
 * Return:		%true if a fixup in buffer @buffer at offset @offset is
 *			allowed.
 *
 * For safety reasons, we only allow fixups inside a buffer to happen
 * at increasing offsets; additionally, we only allow fixup on the last
 * buffer object that was verified, or one of its parents.
 *
 * Example of what is allowed:
 *
 * A
 *   B (parent = A, offset = 0)
 *   C (parent = A, offset = 16)
 *     D (parent = C, offset = 0)
 *   E (parent = A, offset = 32) // min_offset is 16 (C.parent_offset)
 *
 * Examples of what is not allowed:
 *
 * Decreasing offsets within the same parent:
 * A
 *   C (parent = A, offset = 16)
 *   B (parent = A, offset = 0) // decreasing offset within A
 *
 * Referring to a parent that wasn't the last object or any of its parents:
 * A
 *   B (parent = A, offset = 0)
 *   C (parent = A, offset = 0)
 *   C (parent = A, offset = 16)
 *     D (parent = B, offset = 0) // B is not A or any of A's parents
 */
static bool binder_validate_fixup(struct binder_buffer *b,
				  binder_size_t *objects_start,
				  struct binder_buffer_object *buffer,
				  binder_size_t fixup_offset,
				  struct binder_buffer_object *last_obj,
				  binder_size_t last_min_offset)
{
	if (!last_obj) {
		/* Nothing to fix up in */
		return false;
	}

	while (last_obj != buffer) {
		/*
		 * Safe to retrieve the parent of last_obj, since it
		 * was already previously verified by the driver.
		 */
		if ((last_obj->flags & BINDER_BUFFER_FLAG_HAS_PARENT) == 0)
			return false;
		last_min_offset = last_obj->parent_offset + sizeof(uintptr_t);
		last_obj = (struct binder_buffer_object *)
			(b->data + *(objects_start + last_obj->parent));
	}
	return (fixup_offset >= last_min_offset);
}

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
static void binder_transaction_buffer_release(struct binder_proc *proc,
					      struct binder_buffer *buffer,
					      binder_size_t *failed_at)
{
<<<<<<< HEAD
	binder_size_t *offp, *off_end;
	int debug_id = buffer->debug_id;

	binder_debug(BINDER_DEBUG_TRANSACTION,
		     "%d buffer release %d, size %zd-%zd, failed at %pK\n",
=======
	binder_size_t *offp, *off_start, *off_end;
	int debug_id = buffer->debug_id;

	binder_debug(BINDER_DEBUG_TRANSACTION,
		     "%d buffer release %d, size %zd-%zd, failed at %p\n",
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		     proc->pid, buffer->debug_id,
		     buffer->data_size, buffer->offsets_size, failed_at);

	if (buffer->target_node)
		binder_dec_node(buffer->target_node, 1, 0);

<<<<<<< HEAD
	offp = (binder_size_t *) (buffer->data + ALIGN(buffer->data_size, sizeof(void *)));
	if (failed_at)
		off_end = failed_at;
	else
		off_end = (void *)offp + buffer->offsets_size;
	for (; offp < off_end; offp++) {
		struct flat_binder_object *fp;

		if (*offp > buffer->data_size - sizeof(*fp) ||
		    buffer->data_size < sizeof(*fp) || !IS_ALIGNED(*offp, sizeof(u32))) {
			pr_err
			    ("transaction release %d bad offset %lld, size %zd\n",
			     debug_id, (u64) *offp, buffer->data_size);
			continue;
		}
		fp = (struct flat_binder_object *)(buffer->data + *offp);
		switch (fp->type) {
		case BINDER_TYPE_BINDER:
		case BINDER_TYPE_WEAK_BINDER:{
				struct binder_node *node = binder_get_node(proc, fp->binder);

				if (node == NULL) {
					pr_err
					    ("transaction release %d bad node %016llx\n",
					     debug_id, (u64) fp->binder);
					break;
				}
				binder_debug(BINDER_DEBUG_TRANSACTION,
					     "        node %d u%016llx\n",
					     node->debug_id, (u64) node->ptr);
				binder_dec_node(node, fp->type == BINDER_TYPE_BINDER, 0);
			}
			break;
		case BINDER_TYPE_HANDLE:
		case BINDER_TYPE_WEAK_HANDLE:{
				struct binder_ref *ref = binder_get_ref(proc, fp->handle,
							fp->type == BINDER_TYPE_HANDLE);

				if (ref == NULL) {
					pr_err
					    ("transaction release %d bad handle %d\n",
					     debug_id, fp->handle);
					break;
				}
				binder_debug(BINDER_DEBUG_TRANSACTION,
					     "        ref %d desc %d (node %d)\n",
					     ref->debug_id, ref->desc, ref->node->debug_id);
				binder_dec_ref(ref, fp->type == BINDER_TYPE_HANDLE);
			}
			break;

		case BINDER_TYPE_FD:
			binder_debug(BINDER_DEBUG_TRANSACTION, "        fd %d\n", fp->handle);
			if (failed_at)
				task_close_fd(proc, fp->handle);
			break;

		default:
			pr_err("transaction release %d bad object type %x\n", debug_id, fp->type);
=======
	off_start = (binder_size_t *)(buffer->data +
				      ALIGN(buffer->data_size, sizeof(void *)));
	if (failed_at)
		off_end = failed_at;
	else
		off_end = (void *)off_start + buffer->offsets_size;
	for (offp = off_start; offp < off_end; offp++) {
		struct binder_object_header *hdr;
		size_t object_size = binder_validate_object(buffer, *offp);

		if (object_size == 0) {
			pr_err("transaction release %d bad object at offset %lld, size %zd\n",
			       debug_id, (u64)*offp, buffer->data_size);
			continue;
		}
		hdr = (struct binder_object_header *)(buffer->data + *offp);
		switch (hdr->type) {
		case BINDER_TYPE_BINDER:
		case BINDER_TYPE_WEAK_BINDER: {
			struct flat_binder_object *fp;
			struct binder_node *node;

			fp = to_flat_binder_object(hdr);
			node = binder_get_node(proc, fp->binder);
			if (node == NULL) {
				pr_err("transaction release %d bad node %016llx\n",
				       debug_id, (u64)fp->binder);
				break;
			}
			binder_debug(BINDER_DEBUG_TRANSACTION,
				     "        node %d u%016llx\n",
				     node->debug_id, (u64)node->ptr);
			binder_dec_node(node, hdr->type == BINDER_TYPE_BINDER,
					0);
			binder_put_node(node);
		} break;
		case BINDER_TYPE_HANDLE:
		case BINDER_TYPE_WEAK_HANDLE: {
			struct flat_binder_object *fp;
			struct binder_ref_data rdata;
			int ret;

			fp = to_flat_binder_object(hdr);
			ret = binder_dec_ref_for_handle(proc, fp->handle,
				hdr->type == BINDER_TYPE_HANDLE, &rdata);

			if (ret) {
				pr_err("transaction release %d bad handle %d, ret = %d\n",
				 debug_id, fp->handle, ret);
				break;
			}
			binder_debug(BINDER_DEBUG_TRANSACTION,
				     "        ref %d desc %d\n",
				     rdata.debug_id, rdata.desc);
		} break;

		case BINDER_TYPE_FD: {
			struct binder_fd_object *fp = to_binder_fd_object(hdr);

			binder_debug(BINDER_DEBUG_TRANSACTION,
				     "        fd %d\n", fp->fd);
			if (failed_at)
				task_close_fd(proc, fp->fd);
		} break;
		case BINDER_TYPE_PTR:
			/*
			 * Nothing to do here, this will get cleaned up when the
			 * transaction buffer gets freed
			 */
			break;
		case BINDER_TYPE_FDA: {
			struct binder_fd_array_object *fda;
			struct binder_buffer_object *parent;
			uintptr_t parent_buffer;
			u32 *fd_array;
			size_t fd_index;
			binder_size_t fd_buf_size;

			fda = to_binder_fd_array_object(hdr);
			parent = binder_validate_ptr(buffer, fda->parent,
						     off_start,
						     offp - off_start);
			if (!parent) {
				pr_err("transaction release %d bad parent offset",
				       debug_id);
				continue;
			}
			/*
			 * Since the parent was already fixed up, convert it
			 * back to kernel address space to access it
			 */
			parent_buffer = parent->buffer -
				binder_alloc_get_user_buffer_offset(
						&proc->alloc);

			fd_buf_size = sizeof(u32) * fda->num_fds;
			if (fda->num_fds >= SIZE_MAX / sizeof(u32)) {
				pr_err("transaction release %d invalid number of fds (%lld)\n",
				       debug_id, (u64)fda->num_fds);
				continue;
			}
			if (fd_buf_size > parent->length ||
			    fda->parent_offset > parent->length - fd_buf_size) {
				/* No space for all file descriptors here. */
				pr_err("transaction release %d not enough space for %lld fds in buffer\n",
				       debug_id, (u64)fda->num_fds);
				continue;
			}
			fd_array = (u32 *)(parent_buffer + (uintptr_t)fda->parent_offset);
			for (fd_index = 0; fd_index < fda->num_fds; fd_index++)
				task_close_fd(proc, fd_array[fd_index]);
		} break;
		default:
			pr_err("transaction release %d bad object type %x\n",
				debug_id, hdr->type);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			break;
		}
	}
}

<<<<<<< HEAD
#ifdef RT_PRIO_INHERIT
static void mt_sched_setscheduler_nocheck(struct task_struct *p, int policy,
					  struct sched_param *param)
{
	int ret;

	ret = sched_setscheduler_nocheck(p, policy, param);
	if (ret)
		pr_err("set scheduler fail, error code: %d\n", ret);
}
#endif

#ifdef BINDER_MONITOR
/* binder_update_transaction_time - update read/exec done time for transaction
** step:
**			0: start // not used
**			1: read
**			2: reply
*/
static void binder_update_transaction_time(struct binder_transaction_log *t_log,
					   struct binder_transaction *bt, int step)
{
	if (step < 1 || step > 2) {
		pr_err("update trans time fail, wrong step value for id %d\n", bt->debug_id);
		return;
	}

	if ((NULL == bt) || (bt->log_idx == -1)
	    || (bt->log_idx > (t_log->size - 1)))
		return;
	if (t_log->entry[bt->log_idx].debug_id == bt->debug_id) {
		if (step == 1)
			do_posix_clock_monotonic_gettime(&t_log->entry[bt->log_idx].readstamp);
		else if (step == 2)
			do_posix_clock_monotonic_gettime(&t_log->entry[bt->log_idx].endstamp);
	}
}

/* binder_update_transaction_tid - update to thread pid transaction
*/
static void binder_update_transaction_ttid(struct binder_transaction_log *t_log,
					   struct binder_transaction *bt)
{
	if ((NULL == bt) || (NULL == t_log))
		return;
	if ((bt->log_idx == -1) || (bt->log_idx > (t_log->size - 1)))
		return;
	if (bt->tthrd < 0)
		return;
	if ((t_log->entry[bt->log_idx].debug_id == bt->debug_id) &&
	    (t_log->entry[bt->log_idx].to_thread == 0)) {
		t_log->entry[bt->log_idx].to_thread = bt->tthrd;
	}
}

/* this is an addService() transaction identified by:
* fp->type == BINDER_TYPE_BINDER && tr->target.handle == 0
 */
static void parse_service_name(struct binder_transaction_data *tr,
			struct binder_proc *proc, char *name)
{
	unsigned int i, len = 0;
	char *tmp;

	if (tr->target.handle == 0) {
		for (i = 0; (2 * i) < tr->data_size; i++) {
			/* hack into addService() payload:
			* service name string is located at MAGIC_SERVICE_NAME_OFFSET,
			* and interleaved with character '\0'.
			* for example, 'p', '\0', 'h', '\0', 'o', '\0', 'n', '\0', 'e'
			*/
			if ((2 * i) < MAGIC_SERVICE_NAME_OFFSET)
				continue;
			/* prevent array index overflow */
			if (len >= (MAX_SERVICE_NAME_LEN - 1))
				break;
			tmp = (char *)(uintptr_t)(tr->data.ptr.buffer + (2 * i));
			len += sprintf(name + len, "%c", *tmp);
		}
		name[len] = '\0';
	} else {
		name[0] = '\0';
	}
	/* via addService of activity service, identify
	* system_server's process id.
	*/
	if (!strcmp(name, "activity")) {
		system_server_pid = proc->pid;
		pr_debug("system_server %d\n", system_server_pid);
	}
}

#endif

static void binder_transaction(struct binder_proc *proc,
			       struct binder_thread *thread,
			       struct binder_transaction_data *tr, int reply)
{
	struct binder_transaction *t;
	struct binder_work *tcomplete;
	binder_size_t *offp, *off_end;
	binder_size_t off_min;
	struct binder_proc *target_proc;
	struct binder_thread *target_thread = NULL;
	struct binder_node *target_node = NULL;
	struct list_head *target_list;
	wait_queue_head_t *target_wait;
	struct binder_transaction *in_reply_to = NULL;
	struct binder_transaction_log_entry *e;
	uint32_t return_error;

#ifdef BINDER_MONITOR
	struct binder_transaction_log_entry log_entry;
	unsigned int log_idx = -1;

	if ((reply && (tr->data_size < (proc->buffer_size / 16)))
	    || log_disable)
		e = &log_entry;
	else {
		e = binder_transaction_log_add(&binder_transaction_log);
		if (binder_transaction_log.next)
			log_idx = binder_transaction_log.next - 1;
		else
			log_idx = binder_transaction_log.size - 1;
	}
#else
	e = binder_transaction_log_add(&binder_transaction_log);
#endif
=======
static int binder_translate_binder(struct flat_binder_object *fp,
				   struct binder_transaction *t,
				   struct binder_thread *thread)
{
	struct binder_node *node;
	struct binder_proc *proc = thread->proc;
	struct binder_proc *target_proc = t->to_proc;
	struct binder_ref_data rdata;
	int ret = 0;

	node = binder_get_node(proc, fp->binder);
	if (!node) {
		node = binder_new_node(proc, fp);
		if (!node)
			return -ENOMEM;
	}
	if (fp->cookie != node->cookie) {
		binder_user_error("%d:%d sending u%016llx node %d, cookie mismatch %016llx != %016llx\n",
				  proc->pid, thread->pid, (u64)fp->binder,
				  node->debug_id, (u64)fp->cookie,
				  (u64)node->cookie);
		ret = -EINVAL;
		goto done;
	}
	if (security_binder_transfer_binder(proc->tsk, target_proc->tsk)) {
		ret = -EPERM;
		goto done;
	}

	ret = binder_inc_ref_for_node(target_proc, node,
			fp->hdr.type == BINDER_TYPE_BINDER,
			&thread->todo, &rdata);
	if (ret)
		goto done;

	if (fp->hdr.type == BINDER_TYPE_BINDER)
		fp->hdr.type = BINDER_TYPE_HANDLE;
	else
		fp->hdr.type = BINDER_TYPE_WEAK_HANDLE;
	fp->binder = 0;
	fp->handle = rdata.desc;
	fp->cookie = 0;

	trace_binder_transaction_node_to_ref(t, node, &rdata);
	binder_debug(BINDER_DEBUG_TRANSACTION,
		     "        node %d u%016llx -> ref %d desc %d\n",
		     node->debug_id, (u64)node->ptr,
		     rdata.debug_id, rdata.desc);
done:
	binder_put_node(node);
	return ret;
}

static int binder_translate_handle(struct flat_binder_object *fp,
				   struct binder_transaction *t,
				   struct binder_thread *thread)
{
	struct binder_proc *proc = thread->proc;
	struct binder_proc *target_proc = t->to_proc;
	struct binder_node *node;
	struct binder_ref_data src_rdata;
	int ret = 0;

	node = binder_get_node_from_ref(proc, fp->handle,
			fp->hdr.type == BINDER_TYPE_HANDLE, &src_rdata);
	if (!node) {
		binder_user_error("%d:%d got transaction with invalid handle, %d\n",
				  proc->pid, thread->pid, fp->handle);
		return -EINVAL;
	}
	if (security_binder_transfer_binder(proc->tsk, target_proc->tsk)) {
		ret = -EPERM;
		goto done;
	}

	binder_node_lock(node);
	if (node->proc == target_proc) {
		if (fp->hdr.type == BINDER_TYPE_HANDLE)
			fp->hdr.type = BINDER_TYPE_BINDER;
		else
			fp->hdr.type = BINDER_TYPE_WEAK_BINDER;
		fp->binder = node->ptr;
		fp->cookie = node->cookie;
		if (node->proc)
			binder_inner_proc_lock(node->proc);
		binder_inc_node_nilocked(node,
					 fp->hdr.type == BINDER_TYPE_BINDER,
					 0, NULL);
		if (node->proc)
			binder_inner_proc_unlock(node->proc);
		trace_binder_transaction_ref_to_node(t, node, &src_rdata);
		binder_debug(BINDER_DEBUG_TRANSACTION,
			     "        ref %d desc %d -> node %d u%016llx\n",
			     src_rdata.debug_id, src_rdata.desc, node->debug_id,
			     (u64)node->ptr);
		binder_node_unlock(node);
	} else {
		struct binder_ref_data dest_rdata;

		binder_node_unlock(node);
		ret = binder_inc_ref_for_node(target_proc, node,
				fp->hdr.type == BINDER_TYPE_HANDLE,
				NULL, &dest_rdata);
		if (ret)
			goto done;

		fp->binder = 0;
		fp->handle = dest_rdata.desc;
		fp->cookie = 0;
		trace_binder_transaction_ref_to_ref(t, node, &src_rdata,
						    &dest_rdata);
		binder_debug(BINDER_DEBUG_TRANSACTION,
			     "        ref %d desc %d -> ref %d desc %d (node %d)\n",
			     src_rdata.debug_id, src_rdata.desc,
			     dest_rdata.debug_id, dest_rdata.desc,
			     node->debug_id);
	}
done:
	binder_put_node(node);
	return ret;
}

static int binder_translate_fd(int fd,
			       struct binder_transaction *t,
			       struct binder_thread *thread,
			       struct binder_transaction *in_reply_to)
{
	struct binder_proc *proc = thread->proc;
	struct binder_proc *target_proc = t->to_proc;
	int target_fd;
	struct file *file;
	int ret;
	bool target_allows_fd;

	if (in_reply_to)
		target_allows_fd = !!(in_reply_to->flags & TF_ACCEPT_FDS);
	else
		target_allows_fd = t->buffer->target_node->accept_fds;
	if (!target_allows_fd) {
		binder_user_error("%d:%d got %s with fd, %d, but target does not allow fds\n",
				  proc->pid, thread->pid,
				  in_reply_to ? "reply" : "transaction",
				  fd);
		ret = -EPERM;
		goto err_fd_not_accepted;
	}

	file = fget(fd);
	if (!file) {
		binder_user_error("%d:%d got transaction with invalid fd, %d\n",
				  proc->pid, thread->pid, fd);
		ret = -EBADF;
		goto err_fget;
	}
	ret = security_binder_transfer_file(proc->tsk, target_proc->tsk, file);
	if (ret < 0) {
		ret = -EPERM;
		goto err_security;
	}

	target_fd = task_get_unused_fd_flags(target_proc, O_CLOEXEC);
	if (target_fd < 0) {
		ret = -ENOMEM;
		goto err_get_unused_fd;
	}
	task_fd_install(target_proc, target_fd, file);
	trace_binder_transaction_fd(t, fd, target_fd);
	binder_debug(BINDER_DEBUG_TRANSACTION, "        fd %d -> %d\n",
		     fd, target_fd);

	return target_fd;

err_get_unused_fd:
err_security:
	fput(file);
err_fget:
err_fd_not_accepted:
	return ret;
}

static int binder_translate_fd_array(struct binder_fd_array_object *fda,
				     struct binder_buffer_object *parent,
				     struct binder_transaction *t,
				     struct binder_thread *thread,
				     struct binder_transaction *in_reply_to)
{
	binder_size_t fdi, fd_buf_size, num_installed_fds;
	int target_fd;
	uintptr_t parent_buffer;
	u32 *fd_array;
	struct binder_proc *proc = thread->proc;
	struct binder_proc *target_proc = t->to_proc;

	fd_buf_size = sizeof(u32) * fda->num_fds;
	if (fda->num_fds >= SIZE_MAX / sizeof(u32)) {
		binder_user_error("%d:%d got transaction with invalid number of fds (%lld)\n",
				  proc->pid, thread->pid, (u64)fda->num_fds);
		return -EINVAL;
	}
	if (fd_buf_size > parent->length ||
	    fda->parent_offset > parent->length - fd_buf_size) {
		/* No space for all file descriptors here. */
		binder_user_error("%d:%d not enough space to store %lld fds in buffer\n",
				  proc->pid, thread->pid, (u64)fda->num_fds);
		return -EINVAL;
	}
	/*
	 * Since the parent was already fixed up, convert it
	 * back to the kernel address space to access it
	 */
	parent_buffer = parent->buffer -
		binder_alloc_get_user_buffer_offset(&target_proc->alloc);
	fd_array = (u32 *)(parent_buffer + (uintptr_t)fda->parent_offset);
	if (!IS_ALIGNED((unsigned long)fd_array, sizeof(u32))) {
		binder_user_error("%d:%d parent offset not aligned correctly.\n",
				  proc->pid, thread->pid);
		return -EINVAL;
	}
	for (fdi = 0; fdi < fda->num_fds; fdi++) {
		target_fd = binder_translate_fd(fd_array[fdi], t, thread,
						in_reply_to);
		if (target_fd < 0)
			goto err_translate_fd_failed;
		fd_array[fdi] = target_fd;
	}
	return 0;

err_translate_fd_failed:
	/*
	 * Failed to allocate fd or security error, free fds
	 * installed so far.
	 */
	num_installed_fds = fdi;
	for (fdi = 0; fdi < num_installed_fds; fdi++)
		task_close_fd(target_proc, fd_array[fdi]);
	return target_fd;
}

static int binder_fixup_parent(struct binder_transaction *t,
			       struct binder_thread *thread,
			       struct binder_buffer_object *bp,
			       binder_size_t *off_start,
			       binder_size_t num_valid,
			       struct binder_buffer_object *last_fixup_obj,
			       binder_size_t last_fixup_min_off)
{
	struct binder_buffer_object *parent;
	u8 *parent_buffer;
	struct binder_buffer *b = t->buffer;
	struct binder_proc *proc = thread->proc;
	struct binder_proc *target_proc = t->to_proc;

	if (!(bp->flags & BINDER_BUFFER_FLAG_HAS_PARENT))
		return 0;

	parent = binder_validate_ptr(b, bp->parent, off_start, num_valid);
	if (!parent) {
		binder_user_error("%d:%d got transaction with invalid parent offset or type\n",
				  proc->pid, thread->pid);
		return -EINVAL;
	}

	if (!binder_validate_fixup(b, off_start,
				   parent, bp->parent_offset,
				   last_fixup_obj,
				   last_fixup_min_off)) {
		binder_user_error("%d:%d got transaction with out-of-order buffer fixup\n",
				  proc->pid, thread->pid);
		return -EINVAL;
	}

	if (parent->length < sizeof(binder_uintptr_t) ||
	    bp->parent_offset > parent->length - sizeof(binder_uintptr_t)) {
		/* No space for a pointer here! */
		binder_user_error("%d:%d got transaction with invalid parent offset\n",
				  proc->pid, thread->pid);
		return -EINVAL;
	}
	parent_buffer = (u8 *)((uintptr_t)parent->buffer -
			binder_alloc_get_user_buffer_offset(
				&target_proc->alloc));
	*(binder_uintptr_t *)(parent_buffer + bp->parent_offset) = bp->buffer;

	return 0;
}

/**
 * binder_proc_transaction() - sends a transaction to a process and wakes it up
 * @t:		transaction to send
 * @proc:	process to send the transaction to
 * @thread:	thread in @proc to send the transaction to (may be NULL)
 *
 * This function queues a transaction to the specified process. It will try
 * to find a thread in the target process to handle the transaction and
 * wake it up. If no thread is found, the work is queued to the proc
 * waitqueue.
 *
 * If the @thread parameter is not NULL, the transaction is always queued
 * to the waitlist of that specific thread.
 *
 * Return:	true if the transactions was successfully queued
 *		false if the target process or thread is dead
 */
static bool binder_proc_transaction(struct binder_transaction *t,
				    struct binder_proc *proc,
				    struct binder_thread *thread)
{
	struct binder_node *node = t->buffer->target_node;
	struct binder_priority node_prio;
	bool oneway = !!(t->flags & TF_ONE_WAY);
	bool pending_async = false;

	BUG_ON(!node);
	binder_node_lock(node);
	node_prio.prio = node->min_priority;
	node_prio.sched_policy = node->sched_policy;

	if (oneway) {
		BUG_ON(thread);
		if (node->has_async_transaction) {
			pending_async = true;
		} else {
			node->has_async_transaction = 1;
		}
	}

	binder_inner_proc_lock(proc);

	if (proc->is_dead || (thread && thread->is_dead)) {
		binder_inner_proc_unlock(proc);
		binder_node_unlock(node);
		return false;
	}

	if (!thread && !pending_async)
		thread = binder_select_thread_ilocked(proc);

	if (thread) {
		binder_transaction_priority(thread->task, t, node_prio,
					    node->inherit_rt);
		binder_enqueue_thread_work_ilocked(thread, &t->work);
	} else if (!pending_async) {
		binder_enqueue_work_ilocked(&t->work, &proc->todo);
	} else {
		binder_enqueue_work_ilocked(&t->work, &node->async_todo);
	}

	if (!pending_async)
		binder_wakeup_thread_ilocked(proc, thread, !oneway /* sync */);

	binder_inner_proc_unlock(proc);
	binder_node_unlock(node);

	return true;
}

/**
 * binder_get_node_refs_for_txn() - Get required refs on node for txn
 * @node:         struct binder_node for which to get refs
 * @proc:         returns @node->proc if valid
 * @error:        if no @proc then returns BR_DEAD_REPLY
 *
 * User-space normally keeps the node alive when creating a transaction
 * since it has a reference to the target. The local strong ref keeps it
 * alive if the sending process dies before the target process processes
 * the transaction. If the source process is malicious or has a reference
 * counting bug, relying on the local strong ref can fail.
 *
 * Since user-space can cause the local strong ref to go away, we also take
 * a tmpref on the node to ensure it survives while we are constructing
 * the transaction. We also need a tmpref on the proc while we are
 * constructing the transaction, so we take that here as well.
 *
 * Return: The target_node with refs taken or NULL if no @node->proc is NULL.
 * Also sets @proc if valid. If the @node->proc is NULL indicating that the
 * target proc has died, @error is set to BR_DEAD_REPLY
 */
static struct binder_node *binder_get_node_refs_for_txn(
		struct binder_node *node,
		struct binder_proc **procp,
		uint32_t *error)
{
	struct binder_node *target_node = NULL;

	binder_node_inner_lock(node);
	if (node->proc) {
		target_node = node;
		binder_inc_node_nilocked(node, 1, 0, NULL);
		binder_inc_node_tmpref_ilocked(node);
		node->proc->tmp_ref++;
		*procp = node->proc;
	} else
		*error = BR_DEAD_REPLY;
	binder_node_inner_unlock(node);

	return target_node;
}

static void binder_transaction(struct binder_proc *proc,
			       struct binder_thread *thread,
			       struct binder_transaction_data *tr, int reply,
			       binder_size_t extra_buffers_size)
{
	int ret;
	struct binder_transaction *t;
	struct binder_work *tcomplete;
	binder_size_t *offp, *off_end, *off_start;
	binder_size_t off_min;
	u8 *sg_bufp, *sg_buf_end;
	struct binder_proc *target_proc = NULL;
	struct binder_thread *target_thread = NULL;
	struct binder_node *target_node = NULL;
	struct binder_transaction *in_reply_to = NULL;
	struct binder_transaction_log_entry *e;
	uint32_t return_error = 0;
	uint32_t return_error_param = 0;
	uint32_t return_error_line = 0;
	struct binder_buffer_object *last_fixup_obj = NULL;
	binder_size_t last_fixup_min_off = 0;
	struct binder_context *context = proc->context;
	int t_debug_id = atomic_inc_return(&binder_last_id);
	char *secctx = NULL;
	u32 secctx_sz = 0;

	e = binder_transaction_log_add(&binder_transaction_log);
	e->debug_id = t_debug_id;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	e->call_type = reply ? 2 : !!(tr->flags & TF_ONE_WAY);
	e->from_proc = proc->pid;
	e->from_thread = thread->pid;
	e->target_handle = tr->target.handle;
	e->data_size = tr->data_size;
	e->offsets_size = tr->offsets_size;
<<<<<<< HEAD
#ifdef BINDER_MONITOR
	e->code = tr->code;
	/* fd 0 is also valid... set initial value to -1 */
	e->fd = -1;
	do_posix_clock_monotonic_gettime(&e->timestamp);
	/* monotonic_to_bootbased(&e->timestamp); */

	do_gettimeofday(&e->tv);
	/* consider time zone. translate to android time */
	e->tv.tv_sec -= (sys_tz.tz_minuteswest * 60);
#endif

	if (reply) {
		in_reply_to = thread->transaction_stack;
		if (in_reply_to == NULL) {
			binder_user_error
			    ("%d:%d got reply transaction with no transaction stack\n",
			     proc->pid, thread->pid);
			return_error = BR_FAILED_REPLY;
			goto err_empty_call_stack;
		}
#ifdef BINDER_MONITOR
		binder_cancel_bwdog(in_reply_to);
#endif
		binder_set_nice(in_reply_to->saved_priority);
#ifdef RT_PRIO_INHERIT
		if (rt_task(current)
		    && (MAX_RT_PRIO != in_reply_to->saved_rt_prio)
		    && !(thread->looper & (BINDER_LOOPER_STATE_REGISTERED |
					   BINDER_LOOPER_STATE_ENTERED))) {
			struct sched_param param = {
				.sched_priority = in_reply_to->saved_rt_prio,
			};
			mt_sched_setscheduler_nocheck(current, in_reply_to->saved_policy, &param);
#ifdef BINDER_MONITOR
			if (log_disable & BINDER_RT_LOG_ENABLE) {
				pr_debug
				    ("reply reset %d sched_policy from %d to %d rt_prio from %d to %d\n",
				     proc->pid, in_reply_to->policy,
				     in_reply_to->saved_policy,
				     in_reply_to->rt_prio, in_reply_to->saved_rt_prio);
			}
#endif
		}
#endif
		if (in_reply_to->to_thread != thread) {
			binder_user_error("%d:%d got reply transaction with bad transaction stack, transaction %d has target %d:%d\n",
			     proc->pid, thread->pid, in_reply_to->debug_id,
			     in_reply_to->to_proc ? in_reply_to->to_proc->pid : 0,
			     in_reply_to->to_thread ?
				 in_reply_to->to_thread->pid : 0);
			return_error = BR_FAILED_REPLY;
=======
	e->context_name = proc->context->name;

	if (reply) {
		binder_inner_proc_lock(proc);
		in_reply_to = thread->transaction_stack;
		if (in_reply_to == NULL) {
			binder_inner_proc_unlock(proc);
			binder_user_error("%d:%d got reply transaction with no transaction stack\n",
					  proc->pid, thread->pid);
			return_error = BR_FAILED_REPLY;
			return_error_param = -EPROTO;
			return_error_line = __LINE__;
			goto err_empty_call_stack;
		}
		if (in_reply_to->to_thread != thread) {
			spin_lock(&in_reply_to->lock);
			binder_user_error("%d:%d got reply transaction with bad transaction stack, transaction %d has target %d:%d\n",
				proc->pid, thread->pid, in_reply_to->debug_id,
				in_reply_to->to_proc ?
				in_reply_to->to_proc->pid : 0,
				in_reply_to->to_thread ?
				in_reply_to->to_thread->pid : 0);
			spin_unlock(&in_reply_to->lock);
			binder_inner_proc_unlock(proc);
			return_error = BR_FAILED_REPLY;
			return_error_param = -EPROTO;
			return_error_line = __LINE__;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			in_reply_to = NULL;
			goto err_bad_call_stack;
		}
		thread->transaction_stack = in_reply_to->to_parent;
<<<<<<< HEAD
		target_thread = in_reply_to->from;
		if (target_thread == NULL) {
#ifdef MTK_BINDER_DEBUG
			binder_user_error("%d:%d got reply transaction with bad transaction reply_from, ",
					  proc->pid, thread->pid);
			binder_user_error("transaction %d has target %d:%d\n",
					  in_reply_to->debug_id,
					  in_reply_to->to_proc ? in_reply_to->to_proc->pid : 0,
					  in_reply_to->to_thread ? in_reply_to->to_thread->pid : 0);
#endif
			return_error = BR_DEAD_REPLY;
			goto err_dead_binder;
		}
		if (target_thread->transaction_stack != in_reply_to) {
			binder_user_error
			    ("%d:%d got reply transaction with bad target transaction stack %d, expected %d\n",
			     proc->pid, thread->pid,
			     target_thread->transaction_stack ? target_thread->transaction_stack->
			     debug_id : 0, in_reply_to->debug_id);
			return_error = BR_FAILED_REPLY;
=======
		binder_inner_proc_unlock(proc);
		target_thread = binder_get_txn_from_and_acq_inner(in_reply_to);
		if (target_thread == NULL) {
			return_error = BR_DEAD_REPLY;
			return_error_line = __LINE__;
			goto err_dead_binder;
		}
		if (target_thread->transaction_stack != in_reply_to) {
			binder_user_error("%d:%d got reply transaction with bad target transaction stack %d, expected %d\n",
				proc->pid, thread->pid,
				target_thread->transaction_stack ?
				target_thread->transaction_stack->debug_id : 0,
				in_reply_to->debug_id);
			binder_inner_proc_unlock(target_thread->proc);
			return_error = BR_FAILED_REPLY;
			return_error_param = -EPROTO;
			return_error_line = __LINE__;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			in_reply_to = NULL;
			target_thread = NULL;
			goto err_dead_binder;
		}
		target_proc = target_thread->proc;
<<<<<<< HEAD
#ifdef BINDER_MONITOR
		e->service[0] = '\0';
#endif
=======
		target_proc->tmp_ref++;
		binder_inner_proc_unlock(target_thread->proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	} else {
		if (tr->target.handle) {
			struct binder_ref *ref;

<<<<<<< HEAD
			ref = binder_get_ref(proc, tr->target.handle, true);
			if (ref == NULL) {
				binder_user_error
				    ("%d:%d got transaction to invalid handle\n",
				     proc->pid, thread->pid);
				return_error = BR_FAILED_REPLY;
				goto err_invalid_target_handle;
			}
			target_node = ref->node;
		} else {
			target_node = binder_context_mgr_node;
			if (target_node == NULL) {
#ifdef MTK_BINDER_DEBUG
				binder_user_error("%d:%d binder_context_mgr_node is NULL\n",
						  proc->pid, thread->pid);
#endif
				return_error = BR_DEAD_REPLY;
				goto err_no_context_mgr_node;
			}
		}
		e->to_node = target_node->debug_id;
#ifdef BINDER_MONITOR
		strcpy(e->service, target_node->name);
#endif
		target_proc = target_node->proc;
		if (target_proc == NULL) {
#ifdef MTK_BINDER_DEBUG
			binder_user_error("%d:%d target_proc is NULL\n", proc->pid, thread->pid);
#endif
			return_error = BR_DEAD_REPLY;
			goto err_dead_binder;
		}
		if (security_binder_transaction(proc->tsk, target_proc->tsk) < 0) {
			return_error = BR_FAILED_REPLY;
			goto err_invalid_target_handle;
		}
=======
			/*
			 * There must already be a strong ref
			 * on this node. If so, do a strong
			 * increment on the node to ensure it
			 * stays alive until the transaction is
			 * done.
			 */
			binder_proc_lock(proc);
			ref = binder_get_ref_olocked(proc, tr->target.handle,
						     true);
			if (ref) {
				target_node = binder_get_node_refs_for_txn(
						ref->node, &target_proc,
						&return_error);
			} else {
				binder_user_error("%d:%d got transaction to invalid handle\n",
						  proc->pid, thread->pid);
				return_error = BR_FAILED_REPLY;
			}
			binder_proc_unlock(proc);
		} else {
			mutex_lock(&context->context_mgr_node_lock);
			target_node = context->binder_context_mgr_node;
			if (target_node)
				target_node = binder_get_node_refs_for_txn(
						target_node, &target_proc,
						&return_error);
			else
				return_error = BR_DEAD_REPLY;
			mutex_unlock(&context->context_mgr_node_lock);
			if (target_node && target_proc == proc) {
				binder_user_error("%d:%d got transaction to context manager from process owning it\n",
						  proc->pid, thread->pid);
				return_error = BR_FAILED_REPLY;
				return_error_param = -EINVAL;
				return_error_line = __LINE__;
				goto err_invalid_target_handle;
			}
		}
		if (!target_node) {
			/*
			 * return_error is set above
			 */
			return_error_param = -EINVAL;
			return_error_line = __LINE__;
			goto err_dead_binder;
		}
		e->to_node = target_node->debug_id;
		if (security_binder_transaction(proc->tsk,
						target_proc->tsk) < 0) {
			return_error = BR_FAILED_REPLY;
			return_error_param = -EPERM;
			return_error_line = __LINE__;
			goto err_invalid_target_handle;
		}
		binder_inner_proc_lock(proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		if (!(tr->flags & TF_ONE_WAY) && thread->transaction_stack) {
			struct binder_transaction *tmp;

			tmp = thread->transaction_stack;
			if (tmp->to_thread != thread) {
<<<<<<< HEAD
				binder_user_error("%d:%d got new transaction with bad transaction stack, transaction %d has target %d:%d\n",
				     proc->pid, thread->pid, tmp->debug_id,
				     tmp->to_proc ? tmp->to_proc->pid : 0,
				     tmp->to_thread ?
					 tmp->to_thread->pid : 0);
				return_error = BR_FAILED_REPLY;
				goto err_bad_call_stack;
			}
			while (tmp) {
				if (tmp->from && tmp->from->proc == target_proc)
					target_thread = tmp->from;
				tmp = tmp->from_parent;
			}
		}
	}
	if (target_thread) {
		e->to_thread = target_thread->pid;
		target_list = &target_thread->todo;
		target_wait = &target_thread->wait;
	} else {
		target_list = &target_proc->todo;
		target_wait = &target_proc->wait;
	}
=======
				spin_lock(&tmp->lock);
				binder_user_error("%d:%d got new transaction with bad transaction stack, transaction %d has target %d:%d\n",
					proc->pid, thread->pid, tmp->debug_id,
					tmp->to_proc ? tmp->to_proc->pid : 0,
					tmp->to_thread ?
					tmp->to_thread->pid : 0);
				spin_unlock(&tmp->lock);
				binder_inner_proc_unlock(proc);
				return_error = BR_FAILED_REPLY;
				return_error_param = -EPROTO;
				return_error_line = __LINE__;
				goto err_bad_call_stack;
			}
			while (tmp) {
				struct binder_thread *from;

				spin_lock(&tmp->lock);
				from = tmp->from;
				if (from && from->proc == target_proc) {
					atomic_inc(&from->tmp_ref);
					target_thread = from;
					spin_unlock(&tmp->lock);
					break;
				}
				spin_unlock(&tmp->lock);
				tmp = tmp->from_parent;
			}
		}
		binder_inner_proc_unlock(proc);
	}
	if (target_thread)
		e->to_thread = target_thread->pid;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	e->to_proc = target_proc->pid;

	/* TODO: reuse incoming transaction for reply */
	t = kzalloc(sizeof(*t), GFP_KERNEL);
	if (t == NULL) {
<<<<<<< HEAD
#ifdef MTK_BINDER_DEBUG
		binder_user_error("%d:%d transaction allocation failed\n", proc->pid, thread->pid);
#endif
		return_error = BR_FAILED_REPLY;
		goto err_alloc_t_failed;
	}
#ifdef BINDER_MONITOR
	memcpy(&t->timestamp, &e->timestamp, sizeof(struct timespec));
	/* do_gettimeofday(&t->tv); */
	/* consider time zone. translate to android time */
	/* t->tv.tv_sec -= (sys_tz.tz_minuteswest * 60); */
	memcpy(&t->tv, &e->tv, sizeof(struct timeval));
	if (!reply)
		strcpy(t->service, target_node->name);
#endif
	binder_stats_created(BINDER_STAT_TRANSACTION);

	tcomplete = kzalloc(sizeof(*tcomplete), GFP_KERNEL);
	if (tcomplete == NULL) {
#ifdef MTK_BINDER_DEBUG
		binder_user_error("%d:%d tcomplete allocation failed\n", proc->pid, thread->pid);
#endif
		return_error = BR_FAILED_REPLY;
=======
		return_error = BR_FAILED_REPLY;
		return_error_param = -ENOMEM;
		return_error_line = __LINE__;
		goto err_alloc_t_failed;
	}
	binder_stats_created(BINDER_STAT_TRANSACTION);
	spin_lock_init(&t->lock);

	tcomplete = kzalloc(sizeof(*tcomplete), GFP_KERNEL);
	if (tcomplete == NULL) {
		return_error = BR_FAILED_REPLY;
		return_error_param = -ENOMEM;
		return_error_line = __LINE__;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		goto err_alloc_tcomplete_failed;
	}
	binder_stats_created(BINDER_STAT_TRANSACTION_COMPLETE);

<<<<<<< HEAD
	t->debug_id = ++binder_last_id;
	e->debug_id = t->debug_id;

	if (reply)
		binder_debug(BINDER_DEBUG_TRANSACTION,
			     "%d:%d BC_REPLY %d -> %d:%d, data %016llx-%016llx size %lld-%lld\n",
			     proc->pid, thread->pid, t->debug_id,
			     target_proc->pid, target_thread->pid,
			     (u64) tr->data.ptr.buffer,
			     (u64) tr->data.ptr.offsets,
			     (u64) tr->data_size, (u64) tr->offsets_size);
	else
		binder_debug(BINDER_DEBUG_TRANSACTION,
			     "%d:%d BC_TRANSACTION %d -> %d - node %d, data %016llx-%016llx size %lld-%lld\n",
			     proc->pid, thread->pid, t->debug_id,
			     target_proc->pid, target_node->debug_id,
			     (u64) tr->data.ptr.buffer,
			     (u64) tr->data.ptr.offsets,
			     (u64) tr->data_size, (u64) tr->offsets_size);

#ifdef BINDER_MONITOR
	t->fproc = proc->pid;
	t->fthrd = thread->pid;
	t->tproc = target_proc->pid;
	t->tthrd = target_thread ? target_thread->pid : 0;
	t->log_idx = log_idx;

	if (!binder_check_buf_checked()) {
		binder_check_buf_pid = proc->pid;
		binder_check_buf_tid = thread->pid;
	}
#endif
=======
	t->debug_id = t_debug_id;

	if (reply)
		binder_debug(BINDER_DEBUG_TRANSACTION,
			     "%d:%d BC_REPLY %d -> %d:%d, data %016llx-%016llx size %lld-%lld-%lld\n",
			     proc->pid, thread->pid, t->debug_id,
			     target_proc->pid, target_thread->pid,
			     (u64)tr->data.ptr.buffer,
			     (u64)tr->data.ptr.offsets,
			     (u64)tr->data_size, (u64)tr->offsets_size,
			     (u64)extra_buffers_size);
	else
		binder_debug(BINDER_DEBUG_TRANSACTION,
			     "%d:%d BC_TRANSACTION %d -> %d - node %d, data %016llx-%016llx size %lld-%lld-%lld\n",
			     proc->pid, thread->pid, t->debug_id,
			     target_proc->pid, target_node->debug_id,
			     (u64)tr->data.ptr.buffer,
			     (u64)tr->data.ptr.offsets,
			     (u64)tr->data_size, (u64)tr->offsets_size,
			     (u64)extra_buffers_size);

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	if (!reply && !(tr->flags & TF_ONE_WAY))
		t->from = thread;
	else
		t->from = NULL;
	t->sender_euid = task_euid(proc->tsk);
	t->to_proc = target_proc;
	t->to_thread = target_thread;
	t->code = tr->code;
	t->flags = tr->flags;
<<<<<<< HEAD
	t->priority = task_nice(current);
#ifdef RT_PRIO_INHERIT
	t->rt_prio = current->rt_priority;
	t->policy = current->policy;
	t->saved_rt_prio = MAX_RT_PRIO;
#endif

	trace_binder_transaction(reply, t, target_node);

	t->buffer = binder_alloc_buf(target_proc, tr->data_size,
				     tr->offsets_size, !reply && (t->flags & TF_ONE_WAY));
	if (t->buffer == NULL) {
#ifdef MTK_BINDER_DEBUG
		binder_user_error("%d:%d buffer allocation failed on %d:0\n", proc->pid, thread->pid, target_proc->pid);
#endif
		return_error = BR_FAILED_REPLY;
		goto err_binder_alloc_buf_failed;
	}
	t->buffer->allow_user_free = 0;
	t->buffer->debug_id = t->debug_id;
	t->buffer->transaction = t;
#ifdef BINDER_MONITOR
	t->buffer->log_entry = e;
#endif
	t->buffer->target_node = target_node;
	trace_binder_transaction_alloc_buf(t->buffer);
	if (target_node)
		binder_inc_node(target_node, 1, 0, NULL);

	offp = (binder_size_t *) (t->buffer->data + ALIGN(tr->data_size, sizeof(void *)));

	if (copy_from_user(t->buffer->data, (const void __user *)(uintptr_t)
			   tr->data.ptr.buffer, tr->data_size)) {
		binder_user_error
		    ("%d:%d got transaction with invalid data ptr\n", proc->pid, thread->pid);
		return_error = BR_FAILED_REPLY;
=======
	if (!(t->flags & TF_ONE_WAY) &&
	    binder_supported_policy(current->policy)) {
		/* Inherit supported policies for synchronous transactions */
		t->priority.sched_policy = current->policy;
		t->priority.prio = current->normal_prio;
	} else {
		/* Otherwise, fall back to the default priority */
		t->priority = target_proc->default_priority;
	}

	if (target_node && target_node->txn_security_ctx) {
		u32 secid;
		size_t added_size;

		security_task_getsecid(proc->tsk, &secid);
		ret = security_secid_to_secctx(secid, &secctx, &secctx_sz);
		if (ret) {
			return_error = BR_FAILED_REPLY;
			return_error_param = ret;
			return_error_line = __LINE__;
			goto err_get_secctx_failed;
		}
		added_size = ALIGN(secctx_sz, sizeof(u64));
		extra_buffers_size += added_size;
		if (extra_buffers_size < added_size) {
			/* integer overflow of extra_buffers_size */
			return_error = BR_FAILED_REPLY;
			return_error_param = EINVAL;
			return_error_line = __LINE__;
			goto err_bad_extra_size;
		}
	}

	trace_binder_transaction(reply, t, target_node);

	t->buffer = binder_alloc_new_buf(&target_proc->alloc, tr->data_size,
		tr->offsets_size, extra_buffers_size,
		!reply && (t->flags & TF_ONE_WAY));
	if (IS_ERR(t->buffer)) {
		/*
		 * -ESRCH indicates VMA cleared. The target is dying.
		 */
		return_error_param = PTR_ERR(t->buffer);
		return_error = return_error_param == -ESRCH ?
			BR_DEAD_REPLY : BR_FAILED_REPLY;
		return_error_line = __LINE__;
		t->buffer = NULL;
		goto err_binder_alloc_buf_failed;
	}
	if (secctx) {
		size_t buf_offset = ALIGN(tr->data_size, sizeof(void *)) +
				    ALIGN(tr->offsets_size, sizeof(void *)) +
				    ALIGN(extra_buffers_size, sizeof(void *)) -
				    ALIGN(secctx_sz, sizeof(u64));
		char *kptr = t->buffer->data + buf_offset;

		t->security_ctx = (uintptr_t)kptr +
		    binder_alloc_get_user_buffer_offset(&target_proc->alloc);
		memcpy(kptr, secctx, secctx_sz);
		security_release_secctx(secctx, secctx_sz);
		secctx = NULL;
	}
	t->buffer->debug_id = t->debug_id;
	t->buffer->transaction = t;
	t->buffer->target_node = target_node;
	trace_binder_transaction_alloc_buf(t->buffer);
	off_start = (binder_size_t *)(t->buffer->data +
				      ALIGN(tr->data_size, sizeof(void *)));
	offp = off_start;

	if (copy_from_user(t->buffer->data, (const void __user *)(uintptr_t)
			   tr->data.ptr.buffer, tr->data_size)) {
		binder_user_error("%d:%d got transaction with invalid data ptr\n",
				proc->pid, thread->pid);
		return_error = BR_FAILED_REPLY;
		return_error_param = -EFAULT;
		return_error_line = __LINE__;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		goto err_copy_data_failed;
	}
	if (copy_from_user(offp, (const void __user *)(uintptr_t)
			   tr->data.ptr.offsets, tr->offsets_size)) {
<<<<<<< HEAD
		binder_user_error
		    ("%d:%d got transaction with invalid offsets ptr\n", proc->pid, thread->pid);
		return_error = BR_FAILED_REPLY;
		goto err_copy_data_failed;
	}
	if (!IS_ALIGNED(tr->offsets_size, sizeof(binder_size_t))) {
		binder_user_error
		    ("%d:%d got transaction with invalid offsets size, %lld\n",
		     proc->pid, thread->pid, (u64) tr->offsets_size);
		return_error = BR_FAILED_REPLY;
		goto err_bad_offset;
	}
	off_end = (void *)offp + tr->offsets_size;
	off_min = 0;
	for (; offp < off_end; offp++) {
		struct flat_binder_object *fp;

		if (*offp > t->buffer->data_size - sizeof(*fp) ||
		    *offp < off_min ||
		    t->buffer->data_size < sizeof(*fp) || !IS_ALIGNED(*offp, sizeof(u32))) {
			binder_user_error
			    ("%d:%d got transaction with invalid offset, %lld (min %lld, max %lld)\n",
			     proc->pid, thread->pid, (u64) *offp,
			     (u64) off_min, (u64) (t->buffer->data_size - sizeof(*fp)));
			return_error = BR_FAILED_REPLY;
			goto err_bad_offset;
		}
		fp = (struct flat_binder_object *)(t->buffer->data + *offp);
		off_min = *offp + sizeof(struct flat_binder_object);
		switch (fp->type) {
		case BINDER_TYPE_BINDER:
		case BINDER_TYPE_WEAK_BINDER:{
				struct binder_ref *ref;
				struct binder_node *node = binder_get_node(proc, fp->binder);

				if (node == NULL) {
					node = binder_new_node(proc, fp->binder, fp->cookie);
					if (node == NULL) {
#ifdef MTK_BINDER_DEBUG
						binder_user_error
						    ("%d:%d create new node failed\n",
						     proc->pid, thread->pid);
#endif
						return_error = BR_FAILED_REPLY;
						goto err_binder_new_node_failed;
					}
					node->min_priority =
					    fp->flags & FLAT_BINDER_FLAG_PRIORITY_MASK;
					node->accept_fds =
					    !!(fp->flags & FLAT_BINDER_FLAG_ACCEPTS_FDS);
#ifdef BINDER_MONITOR
					parse_service_name(tr, proc, node->name);
#endif
				}
				if (fp->cookie != node->cookie) {
					binder_user_error
					    ("%d:%d sending u%016llx node %d, cookie mismatch %016llx != %016llx\n",
					     proc->pid, thread->pid,
					     (u64) fp->binder, node->debug_id,
					     (u64) fp->cookie, (u64) node->cookie);
					return_error = BR_FAILED_REPLY;
					goto err_binder_get_ref_for_node_failed;
				}
				if (security_binder_transfer_binder(proc->tsk, target_proc->tsk)) {
					return_error = BR_FAILED_REPLY;
					goto err_binder_get_ref_for_node_failed;
				}
				ref = binder_get_ref_for_node(target_proc, node);
				if (ref == NULL) {
#ifdef MTK_BINDER_DEBUG
					binder_user_error
					    ("%d:%d get binder ref failed\n",
					     proc->pid, thread->pid);
#endif
					return_error = BR_FAILED_REPLY;
					goto err_binder_get_ref_for_node_failed;
				}
				if (fp->type == BINDER_TYPE_BINDER)
					fp->type = BINDER_TYPE_HANDLE;
				else
					fp->type = BINDER_TYPE_WEAK_HANDLE;
				fp->binder = 0;
				fp->handle = ref->desc;
				fp->cookie = 0;
				binder_inc_ref(ref, fp->type == BINDER_TYPE_HANDLE, &thread->todo);

				trace_binder_transaction_node_to_ref(t, node, ref);
				binder_debug(BINDER_DEBUG_TRANSACTION,
					     "        node %d u%016llx -> ref %d desc %d\n",
					     node->debug_id, (u64) node->ptr,
					     ref->debug_id, ref->desc);
			}
			break;
		case BINDER_TYPE_HANDLE:
		case BINDER_TYPE_WEAK_HANDLE:{
				struct binder_ref *ref = binder_get_ref(proc, fp->handle,
							fp->type == BINDER_TYPE_HANDLE);

				if (ref == NULL) {
					binder_user_error
					    ("%d:%d got transaction with invalid handle, %d\n",
					     proc->pid, thread->pid, fp->handle);
					return_error = BR_FAILED_REPLY;
					goto err_binder_get_ref_failed;
				}
				if (security_binder_transfer_binder(proc->tsk, target_proc->tsk)) {
					return_error = BR_FAILED_REPLY;
					goto err_binder_get_ref_failed;
				}
				if (ref->node->proc == target_proc) {
					if (fp->type == BINDER_TYPE_HANDLE)
						fp->type = BINDER_TYPE_BINDER;
					else
						fp->type = BINDER_TYPE_WEAK_BINDER;
					fp->binder = ref->node->ptr;
					fp->cookie = ref->node->cookie;
					binder_inc_node(ref->node,
							fp->type == BINDER_TYPE_BINDER, 0, NULL);
					trace_binder_transaction_ref_to_node(t, ref);
					binder_debug(BINDER_DEBUG_TRANSACTION,
						     "        ref %d desc %d -> node %d u%016llx\n",
						     ref->debug_id, ref->desc,
						     ref->node->debug_id, (u64) ref->node->ptr);
				} else {
					struct binder_ref *new_ref;

					new_ref = binder_get_ref_for_node(target_proc, ref->node);
					if (new_ref == NULL) {
#ifdef MTK_BINDER_DEBUG
						binder_user_error
						    ("%d:%d get new binder ref failed\n",
						     proc->pid, thread->pid);
#endif
						return_error = BR_FAILED_REPLY;
						goto err_binder_get_ref_for_node_failed;
					}
					fp->binder = 0;
					fp->handle = new_ref->desc;
					fp->cookie = 0;
					binder_inc_ref(new_ref,
						       fp->type == BINDER_TYPE_HANDLE, NULL);
					trace_binder_transaction_ref_to_ref(t, ref, new_ref);
					binder_debug(BINDER_DEBUG_TRANSACTION,
						     "        ref %d desc %d -> ref %d desc %d (node %d)\n",
						     ref->debug_id, ref->desc,
						     new_ref->debug_id,
						     new_ref->desc, ref->node->debug_id);
				}
			}
			break;

		case BINDER_TYPE_FD:{
				int target_fd;
				struct file *file;

				if (reply) {
					if (!(in_reply_to->flags & TF_ACCEPT_FDS)) {
						binder_user_error
						    ("%d:%d got reply with fd, %d, but target does not allow fds\n",
						     proc->pid, thread->pid, fp->handle);
						return_error = BR_FAILED_REPLY;
						goto err_fd_not_allowed;
					}
				} else if (!target_node->accept_fds) {
					binder_user_error
					    ("%d:%d got transaction with fd, %d, but target does not allow fds\n",
					     proc->pid, thread->pid, fp->handle);
					return_error = BR_FAILED_REPLY;
					goto err_fd_not_allowed;
				}

				file = fget(fp->handle);
				if (file == NULL) {
					binder_user_error
					    ("%d:%d got transaction with invalid fd, %d\n",
					     proc->pid, thread->pid, fp->handle);
					return_error = BR_FAILED_REPLY;
					goto err_fget_failed;
				}
				if (security_binder_transfer_file
				    (proc->tsk, target_proc->tsk, file) < 0) {
					fput(file);
					return_error = BR_FAILED_REPLY;
					goto err_get_unused_fd_failed;
				}
				target_fd = task_get_unused_fd_flags(target_proc, O_CLOEXEC);
				if (target_fd < 0) {
					fput(file);
#ifdef MTK_BINDER_DEBUG
					binder_user_error
					    ("%d:%d to %d failed, %d no unused fd available(%d:%s fd leak?), %d\n",
					     proc->pid, thread->pid,
					     target_proc->pid, target_proc->pid,
					     target_proc->pid,
					     target_proc->tsk ? target_proc->tsk->comm : "",
					     target_fd);
#endif
					return_error = BR_FAILED_REPLY;
					goto err_get_unused_fd_failed;
				}
				task_fd_install(target_proc, target_fd, file);
				trace_binder_transaction_fd(t, fp->handle, target_fd);
				binder_debug(BINDER_DEBUG_TRANSACTION,
					     "        fd %d -> %d\n", fp->handle, target_fd);
				/* TODO: fput? */
				fp->binder = 0;
				fp->handle = target_fd;
#ifdef BINDER_MONITOR
				e->fd = target_fd;
#endif
			}
			break;

		default:
			binder_user_error
			    ("%d:%d got transaction with invalid object type, %x\n",
			     proc->pid, thread->pid, fp->type);
			return_error = BR_FAILED_REPLY;
			goto err_bad_object_type;
		}
	}
	if (reply) {
		BUG_ON(t->buffer->async_transaction != 0);
#ifdef BINDER_MONITOR
		binder_update_transaction_time(&binder_transaction_log, in_reply_to, 2);
#endif
		binder_pop_transaction(target_thread, in_reply_to);
	} else if (!(t->flags & TF_ONE_WAY)) {
		BUG_ON(t->buffer->async_transaction != 0);
		t->need_reply = 1;
		t->from_parent = thread->transaction_stack;
		thread->transaction_stack = t;
	} else {
		BUG_ON(target_node == NULL);
		BUG_ON(t->buffer->async_transaction != 1);
		if (target_node->has_async_transaction) {
			target_list = &target_node->async_todo;
			target_wait = NULL;
		} else
			target_node->has_async_transaction = 1;
	}
	t->work.type = BINDER_WORK_TRANSACTION;
	list_add_tail(&t->work.entry, target_list);
	tcomplete->type = BINDER_WORK_TRANSACTION_COMPLETE;
	list_add_tail(&tcomplete->entry, &thread->todo);
#ifdef RT_PRIO_INHERIT
	if (target_wait) {
		unsigned long flag;
		wait_queue_t *curr, *next;
		bool is_lock = false;

		spin_lock_irqsave(&target_wait->lock, flag);
		is_lock = true;
		list_for_each_entry_safe(curr, next, &target_wait->task_list, task_list) {
			unsigned flags = curr->flags;
			struct task_struct *tsk = curr->private;

			if (tsk == NULL) {
				spin_unlock_irqrestore(&target_wait->lock, flag);
				is_lock = false;
				wake_up_interruptible(target_wait);
				break;
			}
#ifdef MTK_BINDER_DEBUG
			if (tsk->state == TASK_UNINTERRUPTIBLE) {
				pr_err("from %d:%d to %d:%d target thread state: %ld\n",
				       proc->pid, thread->pid, tsk->tgid, tsk->pid, tsk->state);
				show_stack(tsk, NULL);
			}
#endif
			if (!reply && (t->policy == SCHED_RR || t->policy == SCHED_FIFO)
			    && t->rt_prio > tsk->rt_priority && !(t->flags & TF_ONE_WAY)) {
				struct sched_param param = {
					.sched_priority = t->rt_prio,
				};

				t->saved_rt_prio = tsk->rt_priority;
				t->saved_policy = tsk->policy;
				mt_sched_setscheduler_nocheck(tsk, t->policy, &param);
#ifdef BINDER_MONITOR
				if (log_disable & BINDER_RT_LOG_ENABLE) {
					pr_debug
					    ("write set %d sched_policy from %d to %d rt_prio from %d to %d\n",
					     tsk->pid, t->saved_policy,
					     t->policy, t->saved_rt_prio, t->rt_prio);
				}
#endif
			}
			if (curr->func(curr, TASK_INTERRUPTIBLE, 0, NULL) &&
			    (flags & WQ_FLAG_EXCLUSIVE))
				break;
		}
		if (is_lock)
			spin_unlock_irqrestore(&target_wait->lock, flag);
	}
#else
	if (target_wait)
		wake_up_interruptible(target_wait);
#endif

#ifdef BINDER_MONITOR
	t->wait_on = reply ? WAIT_ON_REPLY_READ : WAIT_ON_READ;
	binder_queue_bwdog(t, (time_t) WAIT_BUDGET_READ);
#endif
	return;

err_get_unused_fd_failed:
err_fget_failed:
err_fd_not_allowed:
err_binder_get_ref_for_node_failed:
err_binder_get_ref_failed:
err_binder_new_node_failed:
err_bad_object_type:
err_bad_offset:
err_copy_data_failed:
	trace_binder_transaction_failed_buffer_release(t->buffer);
	binder_transaction_buffer_release(target_proc, t->buffer, offp);
	t->buffer->transaction = NULL;
	binder_free_buf(target_proc, t->buffer);
err_binder_alloc_buf_failed:
	kfree(tcomplete);
	binder_stats_deleted(BINDER_STAT_TRANSACTION_COMPLETE);
err_alloc_tcomplete_failed:
#ifdef BINDER_MONITOR
	binder_cancel_bwdog(t);
#endif
=======
		binder_user_error("%d:%d got transaction with invalid offsets ptr\n",
				proc->pid, thread->pid);
		return_error = BR_FAILED_REPLY;
		return_error_param = -EFAULT;
		return_error_line = __LINE__;
		goto err_copy_data_failed;
	}
	if (!IS_ALIGNED(tr->offsets_size, sizeof(binder_size_t))) {
		binder_user_error("%d:%d got transaction with invalid offsets size, %lld\n",
				proc->pid, thread->pid, (u64)tr->offsets_size);
		return_error = BR_FAILED_REPLY;
		return_error_param = -EINVAL;
		return_error_line = __LINE__;
		goto err_bad_offset;
	}
	if (!IS_ALIGNED(extra_buffers_size, sizeof(u64))) {
		binder_user_error("%d:%d got transaction with unaligned buffers size, %lld\n",
				  proc->pid, thread->pid,
				  (u64)extra_buffers_size);
		return_error = BR_FAILED_REPLY;
		return_error_param = -EINVAL;
		return_error_line = __LINE__;
		goto err_bad_offset;
	}
	off_end = (void *)off_start + tr->offsets_size;
	sg_bufp = (u8 *)(PTR_ALIGN(off_end, sizeof(void *)));
	sg_buf_end = sg_bufp + extra_buffers_size -
		ALIGN(secctx_sz, sizeof(u64));
	off_min = 0;
	for (; offp < off_end; offp++) {
		struct binder_object_header *hdr;
		size_t object_size = binder_validate_object(t->buffer, *offp);

		if (object_size == 0 || *offp < off_min) {
			binder_user_error("%d:%d got transaction with invalid offset (%lld, min %lld max %lld) or object.\n",
					  proc->pid, thread->pid, (u64)*offp,
					  (u64)off_min,
					  (u64)t->buffer->data_size);
			return_error = BR_FAILED_REPLY;
			return_error_param = -EINVAL;
			return_error_line = __LINE__;
			goto err_bad_offset;
		}

		hdr = (struct binder_object_header *)(t->buffer->data + *offp);
		off_min = *offp + object_size;
		switch (hdr->type) {
		case BINDER_TYPE_BINDER:
		case BINDER_TYPE_WEAK_BINDER: {
			struct flat_binder_object *fp;

			fp = to_flat_binder_object(hdr);
			ret = binder_translate_binder(fp, t, thread);
			if (ret < 0) {
				return_error = BR_FAILED_REPLY;
				return_error_param = ret;
				return_error_line = __LINE__;
				goto err_translate_failed;
			}
		} break;
		case BINDER_TYPE_HANDLE:
		case BINDER_TYPE_WEAK_HANDLE: {
			struct flat_binder_object *fp;

			fp = to_flat_binder_object(hdr);
			ret = binder_translate_handle(fp, t, thread);
			if (ret < 0) {
				return_error = BR_FAILED_REPLY;
				return_error_param = ret;
				return_error_line = __LINE__;
				goto err_translate_failed;
			}
		} break;

		case BINDER_TYPE_FD: {
			struct binder_fd_object *fp = to_binder_fd_object(hdr);
			int target_fd = binder_translate_fd(fp->fd, t, thread,
							    in_reply_to);

			if (target_fd < 0) {
				return_error = BR_FAILED_REPLY;
				return_error_param = target_fd;
				return_error_line = __LINE__;
				goto err_translate_failed;
			}
			fp->pad_binder = 0;
			fp->fd = target_fd;
		} break;
		case BINDER_TYPE_FDA: {
			struct binder_fd_array_object *fda =
				to_binder_fd_array_object(hdr);
			struct binder_buffer_object *parent =
				binder_validate_ptr(t->buffer, fda->parent,
						    off_start,
						    offp - off_start);
			if (!parent) {
				binder_user_error("%d:%d got transaction with invalid parent offset or type\n",
						  proc->pid, thread->pid);
				return_error = BR_FAILED_REPLY;
				return_error_param = -EINVAL;
				return_error_line = __LINE__;
				goto err_bad_parent;
			}
			if (!binder_validate_fixup(t->buffer, off_start,
						   parent, fda->parent_offset,
						   last_fixup_obj,
						   last_fixup_min_off)) {
				binder_user_error("%d:%d got transaction with out-of-order buffer fixup\n",
						  proc->pid, thread->pid);
				return_error = BR_FAILED_REPLY;
				return_error_param = -EINVAL;
				return_error_line = __LINE__;
				goto err_bad_parent;
			}
			ret = binder_translate_fd_array(fda, parent, t, thread,
							in_reply_to);
			if (ret < 0) {
				return_error = BR_FAILED_REPLY;
				return_error_param = ret;
				return_error_line = __LINE__;
				goto err_translate_failed;
			}
			last_fixup_obj = parent;
			last_fixup_min_off =
				fda->parent_offset + sizeof(u32) * fda->num_fds;
		} break;
		case BINDER_TYPE_PTR: {
			struct binder_buffer_object *bp =
				to_binder_buffer_object(hdr);
			size_t buf_left = sg_buf_end - sg_bufp;

			if (bp->length > buf_left) {
				binder_user_error("%d:%d got transaction with too large buffer\n",
						  proc->pid, thread->pid);
				return_error = BR_FAILED_REPLY;
				return_error_param = -EINVAL;
				return_error_line = __LINE__;
				goto err_bad_offset;
			}
			if (copy_from_user(sg_bufp,
					   (const void __user *)(uintptr_t)
					   bp->buffer, bp->length)) {
				binder_user_error("%d:%d got transaction with invalid offsets ptr\n",
						  proc->pid, thread->pid);
				return_error_param = -EFAULT;
				return_error = BR_FAILED_REPLY;
				return_error_line = __LINE__;
				goto err_copy_data_failed;
			}
			/* Fixup buffer pointer to target proc address space */
			bp->buffer = (uintptr_t)sg_bufp +
				binder_alloc_get_user_buffer_offset(
						&target_proc->alloc);
			sg_bufp += ALIGN(bp->length, sizeof(u64));

			ret = binder_fixup_parent(t, thread, bp, off_start,
						  offp - off_start,
						  last_fixup_obj,
						  last_fixup_min_off);
			if (ret < 0) {
				return_error = BR_FAILED_REPLY;
				return_error_param = ret;
				return_error_line = __LINE__;
				goto err_translate_failed;
			}
			last_fixup_obj = bp;
			last_fixup_min_off = 0;
		} break;
		default:
			binder_user_error("%d:%d got transaction with invalid object type, %x\n",
				proc->pid, thread->pid, hdr->type);
			return_error = BR_FAILED_REPLY;
			return_error_param = -EINVAL;
			return_error_line = __LINE__;
			goto err_bad_object_type;
		}
	}
	tcomplete->type = BINDER_WORK_TRANSACTION_COMPLETE;
	t->work.type = BINDER_WORK_TRANSACTION;

	if (reply) {
		binder_enqueue_thread_work(thread, tcomplete);
		binder_inner_proc_lock(target_proc);
		if (target_thread->is_dead) {
			binder_inner_proc_unlock(target_proc);
			goto err_dead_proc_or_thread;
		}
		BUG_ON(t->buffer->async_transaction != 0);
		binder_pop_transaction_ilocked(target_thread, in_reply_to);
		binder_enqueue_thread_work_ilocked(target_thread, &t->work);
		binder_inner_proc_unlock(target_proc);
		wake_up_interruptible_sync(&target_thread->wait);
		binder_restore_priority(current, in_reply_to->saved_priority);
		binder_free_transaction(in_reply_to);
	} else if (!(t->flags & TF_ONE_WAY)) {
		BUG_ON(t->buffer->async_transaction != 0);
		binder_inner_proc_lock(proc);
		/*
		 * Defer the TRANSACTION_COMPLETE, so we don't return to
		 * userspace immediately; this allows the target process to
		 * immediately start processing this transaction, reducing
		 * latency. We will then return the TRANSACTION_COMPLETE when
		 * the target replies (or there is an error).
		 */
		binder_enqueue_deferred_thread_work_ilocked(thread, tcomplete);
		t->need_reply = 1;
		t->from_parent = thread->transaction_stack;
		thread->transaction_stack = t;
		binder_inner_proc_unlock(proc);
		if (!binder_proc_transaction(t, target_proc, target_thread)) {
			binder_inner_proc_lock(proc);
			binder_pop_transaction_ilocked(thread, t);
			binder_inner_proc_unlock(proc);
			goto err_dead_proc_or_thread;
		}
	} else {
		BUG_ON(target_node == NULL);
		BUG_ON(t->buffer->async_transaction != 1);
		binder_enqueue_thread_work(thread, tcomplete);
		if (!binder_proc_transaction(t, target_proc, NULL))
			goto err_dead_proc_or_thread;
	}
	if (target_thread)
		binder_thread_dec_tmpref(target_thread);
	binder_proc_dec_tmpref(target_proc);
	if (target_node)
		binder_dec_node_tmpref(target_node);
	/*
	 * write barrier to synchronize with initialization
	 * of log entry
	 */
	smp_wmb();
	WRITE_ONCE(e->debug_id_done, t_debug_id);
	return;

err_dead_proc_or_thread:
	return_error = BR_DEAD_REPLY;
	return_error_line = __LINE__;
	binder_dequeue_work(proc, tcomplete);
err_translate_failed:
err_bad_object_type:
err_bad_offset:
err_bad_parent:
err_copy_data_failed:
	trace_binder_transaction_failed_buffer_release(t->buffer);
	binder_transaction_buffer_release(target_proc, t->buffer, offp);
	if (target_node)
		binder_dec_node_tmpref(target_node);
	target_node = NULL;
	t->buffer->transaction = NULL;
	binder_alloc_free_buf(&target_proc->alloc, t->buffer);
err_binder_alloc_buf_failed:
err_bad_extra_size:
	if (secctx)
		security_release_secctx(secctx, secctx_sz);
err_get_secctx_failed:
	kfree(tcomplete);
	binder_stats_deleted(BINDER_STAT_TRANSACTION_COMPLETE);
err_alloc_tcomplete_failed:
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	kfree(t);
	binder_stats_deleted(BINDER_STAT_TRANSACTION);
err_alloc_t_failed:
err_bad_call_stack:
err_empty_call_stack:
err_dead_binder:
err_invalid_target_handle:
<<<<<<< HEAD
err_no_context_mgr_node:
	binder_debug(BINDER_DEBUG_FAILED_TRANSACTION,
		     "%d:%d transaction failed %d, size %lld-%lld\n",
		     proc->pid, thread->pid, return_error,
		     (u64) tr->data_size, (u64) tr->offsets_size);
=======
	if (target_thread)
		binder_thread_dec_tmpref(target_thread);
	if (target_proc)
		binder_proc_dec_tmpref(target_proc);
	if (target_node) {
		binder_dec_node(target_node, 1, 0);
		binder_dec_node_tmpref(target_node);
	}

	binder_debug(BINDER_DEBUG_FAILED_TRANSACTION,
		     "%d:%d transaction failed %d/%d, size %lld-%lld line %d\n",
		     proc->pid, thread->pid, return_error, return_error_param,
		     (u64)tr->data_size, (u64)tr->offsets_size,
		     return_error_line);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	{
		struct binder_transaction_log_entry *fe;

<<<<<<< HEAD
		fe = binder_transaction_log_add(&binder_transaction_log_failed);
		*fe = *e;
	}

	BUG_ON(thread->return_error != BR_OK);
	if (in_reply_to) {
		thread->return_error = BR_TRANSACTION_COMPLETE;
		binder_send_failed_reply(in_reply_to, return_error);
	} else
		thread->return_error = return_error;
=======
		e->return_error = return_error;
		e->return_error_param = return_error_param;
		e->return_error_line = return_error_line;
		fe = binder_transaction_log_add(&binder_transaction_log_failed);
		*fe = *e;
		/*
		 * write barrier to synchronize with initialization
		 * of log entry
		 */
		smp_wmb();
		WRITE_ONCE(e->debug_id_done, t_debug_id);
		WRITE_ONCE(fe->debug_id_done, t_debug_id);
	}

	BUG_ON(thread->return_error.cmd != BR_OK);
	if (in_reply_to) {
		binder_restore_priority(current, in_reply_to->saved_priority);
		thread->return_error.cmd = BR_TRANSACTION_COMPLETE;
		binder_enqueue_thread_work(thread, &thread->return_error.work);
		binder_send_failed_reply(in_reply_to, return_error);
	} else {
		thread->return_error.cmd = return_error;
		binder_enqueue_thread_work(thread, &thread->return_error.work);
	}
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
}

static int binder_thread_write(struct binder_proc *proc,
			struct binder_thread *thread,
			binder_uintptr_t binder_buffer, size_t size,
			binder_size_t *consumed)
{
	uint32_t cmd;
<<<<<<< HEAD
=======
	struct binder_context *context = proc->context;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	void __user *buffer = (void __user *)(uintptr_t)binder_buffer;
	void __user *ptr = buffer + *consumed;
	void __user *end = buffer + size;

<<<<<<< HEAD
	while (ptr < end && thread->return_error == BR_OK) {
=======
	while (ptr < end && thread->return_error.cmd == BR_OK) {
		int ret;

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		if (get_user(cmd, (uint32_t __user *)ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
		trace_binder_command(cmd);
		if (_IOC_NR(cmd) < ARRAY_SIZE(binder_stats.bc)) {
<<<<<<< HEAD
			binder_stats.bc[_IOC_NR(cmd)]++;
			proc->stats.bc[_IOC_NR(cmd)]++;
			thread->stats.bc[_IOC_NR(cmd)]++;
=======
			atomic_inc(&binder_stats.bc[_IOC_NR(cmd)]);
			atomic_inc(&proc->stats.bc[_IOC_NR(cmd)]);
			atomic_inc(&thread->stats.bc[_IOC_NR(cmd)]);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		}
		switch (cmd) {
		case BC_INCREFS:
		case BC_ACQUIRE:
		case BC_RELEASE:
		case BC_DECREFS: {
			uint32_t target;
<<<<<<< HEAD
			struct binder_ref *ref;
			const char *debug_string;

			if (get_user(target, (uint32_t __user *) ptr))
				return -EFAULT;
			ptr += sizeof(uint32_t);
			if (target == 0 && binder_context_mgr_node &&
				(cmd == BC_INCREFS || cmd == BC_ACQUIRE)) {
				ref = binder_get_ref_for_node(proc,
								binder_context_mgr_node);
				if (ref->desc != target) {
					binder_user_error("%d:%d tried to acquire reference to desc 0, got %d instead\n",
					proc->pid, thread->pid,
					ref->desc);
				}
			} else
				ref = binder_get_ref(proc, target,
						cmd == BC_ACQUIRE ||
						cmd == BC_RELEASE);
			if (ref == NULL) {
				binder_user_error("%d:%d refcount change on invalid ref %d\n",
					proc->pid, thread->pid, target);
				break;
=======
			const char *debug_string;
			bool strong = cmd == BC_ACQUIRE || cmd == BC_RELEASE;
			bool increment = cmd == BC_INCREFS || cmd == BC_ACQUIRE;
			struct binder_ref_data rdata;

			if (get_user(target, (uint32_t __user *)ptr))
				return -EFAULT;

			ptr += sizeof(uint32_t);
			ret = -1;
			if (increment && !target) {
				struct binder_node *ctx_mgr_node;
				mutex_lock(&context->context_mgr_node_lock);
				ctx_mgr_node = context->binder_context_mgr_node;
				if (ctx_mgr_node)
					ret = binder_inc_ref_for_node(
							proc, ctx_mgr_node,
							strong, NULL, &rdata);
				mutex_unlock(&context->context_mgr_node_lock);
			}
			if (ret)
				ret = binder_update_ref_for_handle(
						proc, target, increment, strong,
						&rdata);
			if (!ret && rdata.desc != target) {
				binder_user_error("%d:%d tried to acquire reference to desc %d, got %d instead\n",
					proc->pid, thread->pid,
					target, rdata.desc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			}
			switch (cmd) {
			case BC_INCREFS:
				debug_string = "IncRefs";
<<<<<<< HEAD
				binder_inc_ref(ref, 0, NULL);
				break;
			case BC_ACQUIRE:
				debug_string = "Acquire";
				binder_inc_ref(ref, 1, NULL);
				break;
			case BC_RELEASE:
				debug_string = "Release";
				binder_dec_ref(ref, 1);
=======
				break;
			case BC_ACQUIRE:
				debug_string = "Acquire";
				break;
			case BC_RELEASE:
				debug_string = "Release";
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
				break;
			case BC_DECREFS:
			default:
				debug_string = "DecRefs";
<<<<<<< HEAD
				binder_dec_ref(ref, 0);
				break;
			}
			binder_debug(BINDER_DEBUG_USER_REFS,
						"%d:%d %s ref %d desc %d s %d w %d for node %d\n",
					     proc->pid, thread->pid, debug_string, ref->debug_id,
					     ref->desc, ref->strong, ref->weak, ref->node->debug_id);
			break;
		}
		case BC_INCREFS_DONE:
		case BC_ACQUIRE_DONE:{
			binder_uintptr_t node_ptr;
			binder_uintptr_t cookie;
			struct binder_node *node;

			if (get_user(node_ptr, (binder_uintptr_t __user *) ptr))
				return -EFAULT;
			ptr += sizeof(binder_uintptr_t);
			if (get_user(cookie, (binder_uintptr_t __user *) ptr))
=======
				break;
			}
			if (ret) {
				binder_user_error("%d:%d %s %d refcount change on invalid ref %d ret %d\n",
					proc->pid, thread->pid, debug_string,
					strong, target, ret);
				break;
			}
			binder_debug(BINDER_DEBUG_USER_REFS,
				     "%d:%d %s ref %d desc %d s %d w %d\n",
				     proc->pid, thread->pid, debug_string,
				     rdata.debug_id, rdata.desc, rdata.strong,
				     rdata.weak);
			break;
		}
		case BC_INCREFS_DONE:
		case BC_ACQUIRE_DONE: {
			binder_uintptr_t node_ptr;
			binder_uintptr_t cookie;
			struct binder_node *node;
			bool free_node;

			if (get_user(node_ptr, (binder_uintptr_t __user *)ptr))
				return -EFAULT;
			ptr += sizeof(binder_uintptr_t);
			if (get_user(cookie, (binder_uintptr_t __user *)ptr))
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
				return -EFAULT;
			ptr += sizeof(binder_uintptr_t);
			node = binder_get_node(proc, node_ptr);
			if (node == NULL) {
				binder_user_error("%d:%d %s u%016llx no match\n",
					proc->pid, thread->pid,
					cmd == BC_INCREFS_DONE ?
					"BC_INCREFS_DONE" :
					"BC_ACQUIRE_DONE",
<<<<<<< HEAD
					(u64) node_ptr);
=======
					(u64)node_ptr);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
				break;
			}
			if (cookie != node->cookie) {
				binder_user_error("%d:%d %s u%016llx node %d cookie mismatch %016llx != %016llx\n",
					proc->pid, thread->pid,
					cmd == BC_INCREFS_DONE ?
					"BC_INCREFS_DONE" : "BC_ACQUIRE_DONE",
<<<<<<< HEAD
					(u64) node_ptr, node->debug_id,
					(u64) cookie, (u64) node->cookie);
				break;
			}
=======
					(u64)node_ptr, node->debug_id,
					(u64)cookie, (u64)node->cookie);
				binder_put_node(node);
				break;
			}
			binder_node_inner_lock(node);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			if (cmd == BC_ACQUIRE_DONE) {
				if (node->pending_strong_ref == 0) {
					binder_user_error("%d:%d BC_ACQUIRE_DONE node %d has no pending acquire request\n",
						proc->pid, thread->pid,
						node->debug_id);
<<<<<<< HEAD
=======
					binder_node_inner_unlock(node);
					binder_put_node(node);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
					break;
				}
				node->pending_strong_ref = 0;
			} else {
				if (node->pending_weak_ref == 0) {
					binder_user_error("%d:%d BC_INCREFS_DONE node %d has no pending increfs request\n",
						proc->pid, thread->pid,
						node->debug_id);
<<<<<<< HEAD
=======
					binder_node_inner_unlock(node);
					binder_put_node(node);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
					break;
				}
				node->pending_weak_ref = 0;
			}
<<<<<<< HEAD
			binder_dec_node(node, cmd == BC_ACQUIRE_DONE, 0);
			binder_debug(BINDER_DEBUG_USER_REFS,
				     "%d:%d %s node %d ls %d lw %d\n",
				     proc->pid, thread->pid,
				     cmd == BC_INCREFS_DONE ? "BC_INCREFS_DONE"	: "BC_ACQUIRE_DONE",
				     node->debug_id, node->local_strong_refs, node->local_weak_refs);
=======
			free_node = binder_dec_node_nilocked(node,
					cmd == BC_ACQUIRE_DONE, 0);
			WARN_ON(free_node);
			binder_debug(BINDER_DEBUG_USER_REFS,
				     "%d:%d %s node %d ls %d lw %d tr %d\n",
				     proc->pid, thread->pid,
				     cmd == BC_INCREFS_DONE ? "BC_INCREFS_DONE" : "BC_ACQUIRE_DONE",
				     node->debug_id, node->local_strong_refs,
				     node->local_weak_refs, node->tmp_refs);
			binder_node_inner_unlock(node);
			binder_put_node(node);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			break;
		}
		case BC_ATTEMPT_ACQUIRE:
			pr_err("BC_ATTEMPT_ACQUIRE not supported\n");
			return -EINVAL;
		case BC_ACQUIRE_RESULT:
			pr_err("BC_ACQUIRE_RESULT not supported\n");
			return -EINVAL;

		case BC_FREE_BUFFER: {
			binder_uintptr_t data_ptr;
			struct binder_buffer *buffer;

<<<<<<< HEAD
			if (get_user(data_ptr, (binder_uintptr_t __user *) ptr))
				return -EFAULT;
			ptr += sizeof(binder_uintptr_t);

			buffer = binder_buffer_lookup(proc, data_ptr);
			if (buffer == NULL) {
				binder_user_error("%d:%d BC_FREE_BUFFER u%016llx no match\n",
					proc->pid, thread->pid, (u64)data_ptr);
				break;
			}
			if (!buffer->allow_user_free) {
				binder_user_error("%d:%d BC_FREE_BUFFER u%016llx matched unreturned buffer\n",
					proc->pid, thread->pid, (u64) data_ptr);
=======
			if (get_user(data_ptr, (binder_uintptr_t __user *)ptr))
				return -EFAULT;
			ptr += sizeof(binder_uintptr_t);

			buffer = binder_alloc_prepare_to_free(&proc->alloc,
							      data_ptr);
			if (IS_ERR_OR_NULL(buffer)) {
				if (PTR_ERR(buffer) == -EPERM) {
					binder_user_error(
						"%d:%d BC_FREE_BUFFER u%016llx matched unreturned or currently freeing buffer\n",
						proc->pid, thread->pid,
						(u64)data_ptr);
				} else {
					binder_user_error(
						"%d:%d BC_FREE_BUFFER u%016llx no match\n",
						proc->pid, thread->pid,
						(u64)data_ptr);
				}
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
				break;
			}
			binder_debug(BINDER_DEBUG_FREE_BUFFER,
				     "%d:%d BC_FREE_BUFFER u%016llx found buffer %d for %s transaction\n",
<<<<<<< HEAD
				     proc->pid, thread->pid,
				     (u64) data_ptr, buffer->debug_id,
				     buffer->transaction ? "active" : "finished");

=======
				     proc->pid, thread->pid, (u64)data_ptr,
				     buffer->debug_id,
				     buffer->transaction ? "active" : "finished");

			binder_inner_proc_lock(proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			if (buffer->transaction) {
				buffer->transaction->buffer = NULL;
				buffer->transaction = NULL;
			}
<<<<<<< HEAD
			if (buffer->async_transaction && buffer->target_node) {
				BUG_ON(!buffer->target_node->has_async_transaction);
#ifdef MTK_BINDER_DEBUG
				if (list_empty(&buffer->target_node->async_todo)) {
					buffer->target_node->has_async_transaction = 0;
					buffer->target_node->async_pid = 0;
				} else {
					list_move_tail(buffer->target_node->async_todo.next, &thread->todo);
					buffer->target_node->async_pid = thread->pid;
				}
#else
				if (list_empty(&buffer->target_node->async_todo))
					buffer->target_node->has_async_transaction = 0;
				else
					list_move_tail(buffer->target_node->async_todo.next, &thread->todo);
#endif
			}
			trace_binder_transaction_buffer_release(buffer);
			binder_transaction_buffer_release(proc, buffer, NULL);
			binder_free_buf(proc, buffer);
			break;
		}

=======
			binder_inner_proc_unlock(proc);
			if (buffer->async_transaction && buffer->target_node) {
				struct binder_node *buf_node;
				struct binder_work *w;

				buf_node = buffer->target_node;
				binder_node_inner_lock(buf_node);
				BUG_ON(!buf_node->has_async_transaction);
				BUG_ON(buf_node->proc != proc);
				w = binder_dequeue_work_head_ilocked(
						&buf_node->async_todo);
				if (!w) {
					buf_node->has_async_transaction = 0;
				} else {
					binder_enqueue_work_ilocked(
							w, &proc->todo);
					binder_wakeup_proc_ilocked(proc);
				}
				binder_node_inner_unlock(buf_node);
			}
			trace_binder_transaction_buffer_release(buffer);
			binder_transaction_buffer_release(proc, buffer, NULL);
			binder_alloc_free_buf(&proc->alloc, buffer);
			break;
		}

		case BC_TRANSACTION_SG:
		case BC_REPLY_SG: {
			struct binder_transaction_data_sg tr;

			if (copy_from_user(&tr, ptr, sizeof(tr)))
				return -EFAULT;
			ptr += sizeof(tr);
			binder_transaction(proc, thread, &tr.transaction_data,
					   cmd == BC_REPLY_SG, tr.buffers_size);
			break;
		}
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		case BC_TRANSACTION:
		case BC_REPLY: {
			struct binder_transaction_data tr;

			if (copy_from_user(&tr, ptr, sizeof(tr)))
				return -EFAULT;
			ptr += sizeof(tr);
<<<<<<< HEAD
			binder_transaction(proc, thread, &tr, cmd == BC_REPLY);
=======
			binder_transaction(proc, thread, &tr,
					   cmd == BC_REPLY, 0);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			break;
		}

		case BC_REGISTER_LOOPER:
			binder_debug(BINDER_DEBUG_THREADS,
<<<<<<< HEAD
			     "%d:%d BC_REGISTER_LOOPER\n", proc->pid, thread->pid);
=======
				     "%d:%d BC_REGISTER_LOOPER\n",
				     proc->pid, thread->pid);
			binder_inner_proc_lock(proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			if (thread->looper & BINDER_LOOPER_STATE_ENTERED) {
				thread->looper |= BINDER_LOOPER_STATE_INVALID;
				binder_user_error("%d:%d ERROR: BC_REGISTER_LOOPER called after BC_ENTER_LOOPER\n",
					proc->pid, thread->pid);
			} else if (proc->requested_threads == 0) {
				thread->looper |= BINDER_LOOPER_STATE_INVALID;
				binder_user_error("%d:%d ERROR: BC_REGISTER_LOOPER called without request\n",
					proc->pid, thread->pid);
			} else {
				proc->requested_threads--;
				proc->requested_threads_started++;
			}
			thread->looper |= BINDER_LOOPER_STATE_REGISTERED;
<<<<<<< HEAD
=======
			binder_inner_proc_unlock(proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			break;
		case BC_ENTER_LOOPER:
			binder_debug(BINDER_DEBUG_THREADS,
				     "%d:%d BC_ENTER_LOOPER\n",
<<<<<<< HEAD
					 proc->pid, thread->pid);
=======
				     proc->pid, thread->pid);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			if (thread->looper & BINDER_LOOPER_STATE_REGISTERED) {
				thread->looper |= BINDER_LOOPER_STATE_INVALID;
				binder_user_error("%d:%d ERROR: BC_ENTER_LOOPER called after BC_REGISTER_LOOPER\n",
					proc->pid, thread->pid);
			}
			thread->looper |= BINDER_LOOPER_STATE_ENTERED;
			break;
		case BC_EXIT_LOOPER:
			binder_debug(BINDER_DEBUG_THREADS,
				     "%d:%d BC_EXIT_LOOPER\n",
<<<<<<< HEAD
					 proc->pid, thread->pid);
=======
				     proc->pid, thread->pid);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			thread->looper |= BINDER_LOOPER_STATE_EXITED;
			break;

		case BC_REQUEST_DEATH_NOTIFICATION:
<<<<<<< HEAD
		case BC_CLEAR_DEATH_NOTIFICATION:{
			uint32_t target;
			binder_uintptr_t cookie;
			struct binder_ref *ref;
			struct binder_ref_death *death;

			if (get_user(target, (uint32_t __user *) ptr))
				return -EFAULT;
			ptr += sizeof(uint32_t);
			if (get_user(cookie, (binder_uintptr_t __user *) ptr))
				return -EFAULT;
			ptr += sizeof(binder_uintptr_t);
			ref = binder_get_ref(proc, target, false);
=======
		case BC_CLEAR_DEATH_NOTIFICATION: {
			uint32_t target;
			binder_uintptr_t cookie;
			struct binder_ref *ref;
			struct binder_ref_death *death = NULL;

			if (get_user(target, (uint32_t __user *)ptr))
				return -EFAULT;
			ptr += sizeof(uint32_t);
			if (get_user(cookie, (binder_uintptr_t __user *)ptr))
				return -EFAULT;
			ptr += sizeof(binder_uintptr_t);
			if (cmd == BC_REQUEST_DEATH_NOTIFICATION) {
				/*
				 * Allocate memory for death notification
				 * before taking lock
				 */
				death = kzalloc(sizeof(*death), GFP_KERNEL);
				if (death == NULL) {
					WARN_ON(thread->return_error.cmd !=
						BR_OK);
					thread->return_error.cmd = BR_ERROR;
					binder_enqueue_thread_work(
						thread,
						&thread->return_error.work);
					binder_debug(
						BINDER_DEBUG_FAILED_TRANSACTION,
						"%d:%d BC_REQUEST_DEATH_NOTIFICATION failed\n",
						proc->pid, thread->pid);
					break;
				}
			}
			binder_proc_lock(proc);
			ref = binder_get_ref_olocked(proc, target, false);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			if (ref == NULL) {
				binder_user_error("%d:%d %s invalid ref %d\n",
					proc->pid, thread->pid,
					cmd == BC_REQUEST_DEATH_NOTIFICATION ?
					"BC_REQUEST_DEATH_NOTIFICATION" :
<<<<<<< HEAD
					"BC_CLEAR_DEATH_NOTIFICATION", target);
				break;
			}
#ifdef MTK_DEATH_NOTIFY_MONITOR
			binder_debug(BINDER_DEBUG_DEATH_NOTIFICATION,
				"[DN #%s]binder: %d:%d %s %d(%s) cookie 0x%016llx\n",
				cmd == BC_REQUEST_DEATH_NOTIFICATION ? "1" :
				"2", proc->pid, thread->pid,
				cmd == BC_REQUEST_DEATH_NOTIFICATION ?
				"BC_REQUEST_DEATH_NOTIFICATION" :
				"BC_CLEAR_DEATH_NOTIFICATION",
				ref->node->proc ? ref->node->proc->pid : 0,
#ifdef BINDER_MONITOR
				ref->node ? ref->node->name : "",
#else
				"",
#endif
				(u64) cookie);
#else
=======
					"BC_CLEAR_DEATH_NOTIFICATION",
					target);
				binder_proc_unlock(proc);
				kfree(death);
				break;
			}

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			binder_debug(BINDER_DEBUG_DEATH_NOTIFICATION,
				     "%d:%d %s %016llx ref %d desc %d s %d w %d for node %d\n",
				     proc->pid, thread->pid,
				     cmd == BC_REQUEST_DEATH_NOTIFICATION ?
				     "BC_REQUEST_DEATH_NOTIFICATION" :
				     "BC_CLEAR_DEATH_NOTIFICATION",
<<<<<<< HEAD
				     (u64) cookie, ref->debug_id,
				     ref->desc, ref->strong, ref->weak,
				     ref->node->debug_id);
#endif

=======
				     (u64)cookie, ref->data.debug_id,
				     ref->data.desc, ref->data.strong,
				     ref->data.weak, ref->node->debug_id);

			binder_node_lock(ref->node);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			if (cmd == BC_REQUEST_DEATH_NOTIFICATION) {
				if (ref->death) {
					binder_user_error("%d:%d BC_REQUEST_DEATH_NOTIFICATION death notification already set\n",
						proc->pid, thread->pid);
<<<<<<< HEAD
					break;
				}
				death = kzalloc(sizeof(*death), GFP_KERNEL);
				if (death == NULL) {
					thread->return_error = BR_ERROR;
					binder_debug(BINDER_DEBUG_FAILED_TRANSACTION,
						     "%d:%d BC_REQUEST_DEATH_NOTIFICATION failed\n",
						     proc->pid, thread->pid);
=======
					binder_node_unlock(ref->node);
					binder_proc_unlock(proc);
					kfree(death);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
					break;
				}
				binder_stats_created(BINDER_STAT_DEATH);
				INIT_LIST_HEAD(&death->work.entry);
				death->cookie = cookie;
				ref->death = death;
				if (ref->node->proc == NULL) {
					ref->death->work.type = BINDER_WORK_DEAD_BINDER;
<<<<<<< HEAD
					if (thread->looper & (BINDER_LOOPER_STATE_REGISTERED | BINDER_LOOPER_STATE_ENTERED)) {
						list_add_tail(&ref->death->work.entry, &thread->todo);
					} else {
						list_add_tail(&ref->death->work.entry, &proc->todo);
						wake_up_interruptible(&proc->wait);
					}
=======

					binder_inner_proc_lock(proc);
					binder_enqueue_work_ilocked(
						&ref->death->work, &proc->todo);
					binder_wakeup_proc_ilocked(proc);
					binder_inner_proc_unlock(proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
				}
			} else {
				if (ref->death == NULL) {
					binder_user_error("%d:%d BC_CLEAR_DEATH_NOTIFICATION death notification not active\n",
						proc->pid, thread->pid);
<<<<<<< HEAD
=======
					binder_node_unlock(ref->node);
					binder_proc_unlock(proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
					break;
				}
				death = ref->death;
				if (death->cookie != cookie) {
					binder_user_error("%d:%d BC_CLEAR_DEATH_NOTIFICATION death notification cookie mismatch %016llx != %016llx\n",
						proc->pid, thread->pid,
<<<<<<< HEAD
						(u64) death->cookie, (u64) cookie);
					break;
				}
				ref->death = NULL;
				if (list_empty(&death->work.entry)) {
					death->work.type = BINDER_WORK_CLEAR_DEATH_NOTIFICATION;
					if (thread->looper & (BINDER_LOOPER_STATE_REGISTERED | BINDER_LOOPER_STATE_ENTERED)) {
						list_add_tail(&death->work.entry, &thread->todo);
					} else {
						list_add_tail(&death->work.entry, &proc->todo);
						wake_up_interruptible(&proc->wait);
=======
						(u64)death->cookie,
						(u64)cookie);
					binder_node_unlock(ref->node);
					binder_proc_unlock(proc);
					break;
				}
				ref->death = NULL;
				binder_inner_proc_lock(proc);
				if (list_empty(&death->work.entry)) {
					death->work.type = BINDER_WORK_CLEAR_DEATH_NOTIFICATION;
					if (thread->looper &
					    (BINDER_LOOPER_STATE_REGISTERED |
					     BINDER_LOOPER_STATE_ENTERED))
						binder_enqueue_thread_work_ilocked(
								thread,
								&death->work);
					else {
						binder_enqueue_work_ilocked(
								&death->work,
								&proc->todo);
						binder_wakeup_proc_ilocked(
								proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
					}
				} else {
					BUG_ON(death->work.type != BINDER_WORK_DEAD_BINDER);
					death->work.type = BINDER_WORK_DEAD_BINDER_AND_CLEAR;
				}
<<<<<<< HEAD
			}
		}
		break;
=======
				binder_inner_proc_unlock(proc);
			}
			binder_node_unlock(ref->node);
			binder_proc_unlock(proc);
		} break;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		case BC_DEAD_BINDER_DONE: {
			struct binder_work *w;
			binder_uintptr_t cookie;
			struct binder_ref_death *death = NULL;

<<<<<<< HEAD
			if (get_user(cookie, (binder_uintptr_t __user *) ptr))
				return -EFAULT;

#ifdef MTK_DEATH_NOTIFY_MONITOR
			binder_debug(BINDER_DEBUG_DEATH_NOTIFICATION,
				     "[DN #6]binder: %d:%d cookie 0x%016llx\n",
				     proc->pid, thread->pid, (u64) cookie);
#endif

			ptr += sizeof(void *);
			list_for_each_entry(w, &proc->delivered_death, entry) {
				struct binder_ref_death *tmp_death = container_of(w, struct binder_ref_death, work);
=======
			if (get_user(cookie, (binder_uintptr_t __user *)ptr))
				return -EFAULT;

			ptr += sizeof(cookie);
			binder_inner_proc_lock(proc);
			list_for_each_entry(w, &proc->delivered_death,
					    entry) {
				struct binder_ref_death *tmp_death =
					container_of(w,
						     struct binder_ref_death,
						     work);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

				if (tmp_death->cookie == cookie) {
					death = tmp_death;
					break;
				}
			}
			binder_debug(BINDER_DEBUG_DEAD_BINDER,
<<<<<<< HEAD
				     "%d:%d BC_DEAD_BINDER_DONE %016llx found %pK\n",
				     proc->pid, thread->pid, (u64) cookie,
					 death);
			if (death == NULL) {
				binder_user_error("%d:%d BC_DEAD_BINDER_DONE %016llx not found\n",
				     proc->pid, thread->pid, (u64) cookie);
				break;
			}

			list_del_init(&death->work.entry);
			if (death->work.type == BINDER_WORK_DEAD_BINDER_AND_CLEAR) {
				death->work.type = BINDER_WORK_CLEAR_DEATH_NOTIFICATION;
				if (thread->looper & (BINDER_LOOPER_STATE_REGISTERED | BINDER_LOOPER_STATE_ENTERED)) {
					list_add_tail(&death->work.entry, &thread->todo);
				} else {
					list_add_tail(&death->work.entry, &proc->todo);
					wake_up_interruptible(&proc->wait);
				}
			}
		}
		break;

		default:
			pr_err("%d:%d unknown command %d\n",
				proc->pid, thread->pid, cmd);
=======
				     "%d:%d BC_DEAD_BINDER_DONE %016llx found %p\n",
				     proc->pid, thread->pid, (u64)cookie,
				     death);
			if (death == NULL) {
				binder_user_error("%d:%d BC_DEAD_BINDER_DONE %016llx not found\n",
					proc->pid, thread->pid, (u64)cookie);
				binder_inner_proc_unlock(proc);
				break;
			}
			binder_dequeue_work_ilocked(&death->work);
			if (death->work.type == BINDER_WORK_DEAD_BINDER_AND_CLEAR) {
				death->work.type = BINDER_WORK_CLEAR_DEATH_NOTIFICATION;
				if (thread->looper &
					(BINDER_LOOPER_STATE_REGISTERED |
					 BINDER_LOOPER_STATE_ENTERED))
					binder_enqueue_thread_work_ilocked(
						thread, &death->work);
				else {
					binder_enqueue_work_ilocked(
							&death->work,
							&proc->todo);
					binder_wakeup_proc_ilocked(proc);
				}
			}
			binder_inner_proc_unlock(proc);
		} break;

		default:
			pr_err("%d:%d unknown command %d\n",
			       proc->pid, thread->pid, cmd);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			return -EINVAL;
		}
		*consumed = ptr - buffer;
	}
	return 0;
}

static void binder_stat_br(struct binder_proc *proc,
<<<<<<< HEAD
			 struct binder_thread *thread, uint32_t cmd)
{
	trace_binder_return(cmd);
	if (_IOC_NR(cmd) < ARRAY_SIZE(binder_stats.br)) {
		binder_stats.br[_IOC_NR(cmd)]++;
		proc->stats.br[_IOC_NR(cmd)]++;
		thread->stats.br[_IOC_NR(cmd)]++;
	}
}

static int binder_has_proc_work(struct binder_proc *proc,
				 struct binder_thread *thread)
{
	return !list_empty(&proc->todo) ||
		(thread->looper & BINDER_LOOPER_STATE_NEED_RETURN);
}

static int binder_has_thread_work(struct binder_thread *thread)
{
	return !list_empty(&thread->todo) || thread->return_error != BR_OK ||
		(thread->looper & BINDER_LOOPER_STATE_NEED_RETURN);
=======
			   struct binder_thread *thread, uint32_t cmd)
{
	trace_binder_return(cmd);
	if (_IOC_NR(cmd) < ARRAY_SIZE(binder_stats.br)) {
		atomic_inc(&binder_stats.br[_IOC_NR(cmd)]);
		atomic_inc(&proc->stats.br[_IOC_NR(cmd)]);
		atomic_inc(&thread->stats.br[_IOC_NR(cmd)]);
	}
}

static int binder_put_node_cmd(struct binder_proc *proc,
			       struct binder_thread *thread,
			       void __user **ptrp,
			       binder_uintptr_t node_ptr,
			       binder_uintptr_t node_cookie,
			       int node_debug_id,
			       uint32_t cmd, const char *cmd_name)
{
	void __user *ptr = *ptrp;

	if (put_user(cmd, (uint32_t __user *)ptr))
		return -EFAULT;
	ptr += sizeof(uint32_t);

	if (put_user(node_ptr, (binder_uintptr_t __user *)ptr))
		return -EFAULT;
	ptr += sizeof(binder_uintptr_t);

	if (put_user(node_cookie, (binder_uintptr_t __user *)ptr))
		return -EFAULT;
	ptr += sizeof(binder_uintptr_t);

	binder_stat_br(proc, thread, cmd);
	binder_debug(BINDER_DEBUG_USER_REFS, "%d:%d %s %d u%016llx c%016llx\n",
		     proc->pid, thread->pid, cmd_name, node_debug_id,
		     (u64)node_ptr, (u64)node_cookie);

	*ptrp = ptr;
	return 0;
}

static int binder_wait_for_work(struct binder_thread *thread,
				bool do_proc_work)
{
	DEFINE_WAIT(wait);
	struct binder_proc *proc = thread->proc;
	int ret = 0;

	freezer_do_not_count();
	binder_inner_proc_lock(proc);
	for (;;) {
		prepare_to_wait(&thread->wait, &wait, TASK_INTERRUPTIBLE);
		if (binder_has_work_ilocked(thread, do_proc_work))
			break;
		if (do_proc_work)
			list_add(&thread->waiting_thread_node,
				 &proc->waiting_threads);
		binder_inner_proc_unlock(proc);
		schedule();
		binder_inner_proc_lock(proc);
		list_del_init(&thread->waiting_thread_node);
		if (signal_pending(current)) {
			ret = -ERESTARTSYS;
			break;
		}
	}
	finish_wait(&thread->wait, &wait);
	binder_inner_proc_unlock(proc);
	freezer_count();

	return ret;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
}

static int binder_thread_read(struct binder_proc *proc,
			      struct binder_thread *thread,
			      binder_uintptr_t binder_buffer, size_t size,
			      binder_size_t *consumed, int non_block)
{
<<<<<<< HEAD
	void __user *buffer = (void __user *)(uintptr_t) binder_buffer;
=======
	void __user *buffer = (void __user *)(uintptr_t)binder_buffer;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	void __user *ptr = buffer + *consumed;
	void __user *end = buffer + size;

	int ret = 0;
	int wait_for_proc_work;

	if (*consumed == 0) {
		if (put_user(BR_NOOP, (uint32_t __user *)ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
	}

retry:
<<<<<<< HEAD
	wait_for_proc_work = thread->transaction_stack == NULL &&
				list_empty(&thread->todo);

	if (thread->return_error != BR_OK && ptr < end) {
		if (thread->return_error2 != BR_OK) {
			if (put_user(thread->return_error2, (uint32_t __user *) ptr))
				return -EFAULT;
			ptr += sizeof(uint32_t);
			pr_err
			    ("read put err2 %u to user %p, thread error %u:%u\n",
			     thread->return_error2, ptr, thread->return_error,
			     thread->return_error2);
			binder_stat_br(proc, thread, thread->return_error2);
			if (ptr == end)
				goto done;
			thread->return_error2 = BR_OK;
		}
		if (put_user(thread->return_error, (uint32_t __user *) ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
		pr_err("read put err %u to user %p, thread error %u:%u\n",
		       thread->return_error, ptr, thread->return_error, thread->return_error2);
		binder_stat_br(proc, thread, thread->return_error);
		thread->return_error = BR_OK;
		goto done;
	}


	thread->looper |= BINDER_LOOPER_STATE_WAITING;
	if (wait_for_proc_work)
		proc->ready_threads++;

	binder_unlock(__func__);

	trace_binder_wait_for_work(wait_for_proc_work,
				   !!thread->transaction_stack, !list_empty(&thread->todo));
=======
	binder_inner_proc_lock(proc);
	wait_for_proc_work = binder_available_for_proc_work_ilocked(thread);
	binder_inner_proc_unlock(proc);

	thread->looper |= BINDER_LOOPER_STATE_WAITING;

	trace_binder_wait_for_work(wait_for_proc_work,
				   !!thread->transaction_stack,
				   !binder_worklist_empty(proc, &thread->todo));
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	if (wait_for_proc_work) {
		if (!(thread->looper & (BINDER_LOOPER_STATE_REGISTERED |
					BINDER_LOOPER_STATE_ENTERED))) {
			binder_user_error("%d:%d ERROR: Thread waiting for process work before calling BC_REGISTER_LOOPER or BC_ENTER_LOOPER (state %x)\n",
<<<<<<< HEAD
			     proc->pid, thread->pid, thread->looper);
			wait_event_interruptible(binder_user_error_wait,
						 binder_stop_on_user_error < 2);
		}
#ifdef RT_PRIO_INHERIT
		/* disable preemption to prevent from schedule-out immediately */
		preempt_disable();
#endif
		binder_set_nice(proc->default_priority);
#ifdef RT_PRIO_INHERIT
		if (rt_task(current) && !binder_has_proc_work(proc, thread)) {
			/* make sure binder has no work before setting priority back */
			struct sched_param param = {
				.sched_priority = proc->default_rt_prio,
			};
#ifdef BINDER_MONITOR
			if (log_disable & BINDER_RT_LOG_ENABLE) {
				pr_debug
				    ("enter threadpool reset %d sched_policy from %u to %d rt_prio from %u to %d\n",
				     current->pid, current->policy,
				     proc->default_policy, current->rt_priority,
				     proc->default_rt_prio);
			}
#endif
			mt_sched_setscheduler_nocheck(current, proc->default_policy, &param);
		}
		preempt_enable_no_resched();
#endif
		if (non_block) {
			if (!binder_has_proc_work(proc, thread))
				ret = -EAGAIN;
		} else
			ret = wait_event_freezable_exclusive(proc->wait, binder_has_proc_work(proc, thread));
	} else {
		if (non_block) {
			if (!binder_has_thread_work(thread))
				ret = -EAGAIN;
		} else
			ret = wait_event_freezable(thread->wait, binder_has_thread_work(thread));
	}

	binder_lock(__func__);

	if (wait_for_proc_work)
		proc->ready_threads--;
=======
				proc->pid, thread->pid, thread->looper);
			wait_event_interruptible(binder_user_error_wait,
						 binder_stop_on_user_error < 2);
		}
		binder_restore_priority(current, proc->default_priority);
	}

	if (non_block) {
		if (!binder_has_work(thread, wait_for_proc_work))
			ret = -EAGAIN;
	} else {
		ret = binder_wait_for_work(thread, wait_for_proc_work);
	}

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	thread->looper &= ~BINDER_LOOPER_STATE_WAITING;

	if (ret)
		return ret;

	while (1) {
		uint32_t cmd;
<<<<<<< HEAD
		struct binder_transaction_data tr;
		struct binder_work *w;
		struct binder_transaction *t = NULL;

		if (!list_empty(&thread->todo)) {
			w = list_first_entry(&thread->todo, struct binder_work, entry);
		} else if (!list_empty(&proc->todo) && wait_for_proc_work) {
			w = list_first_entry(&proc->todo, struct binder_work, entry);
		} else {
			/* no data added */
			if (ptr - buffer == 4 &&
				!(thread->looper & BINDER_LOOPER_STATE_NEED_RETURN))
=======
		struct binder_transaction_data_secctx tr;
		struct binder_transaction_data *trd = &tr.transaction_data;
		struct binder_work *w = NULL;
		struct list_head *list = NULL;
		struct binder_transaction *t = NULL;
		struct binder_thread *t_from;
		size_t trsize = sizeof(*trd);

		binder_inner_proc_lock(proc);
		if (!binder_worklist_empty_ilocked(&thread->todo))
			list = &thread->todo;
		else if (!binder_worklist_empty_ilocked(&proc->todo) &&
			   wait_for_proc_work)
			list = &proc->todo;
		else {
			binder_inner_proc_unlock(proc);

			/* no data added */
			if (ptr - buffer == 4 && !thread->looper_need_return)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
				goto retry;
			break;
		}

<<<<<<< HEAD
		if (end - ptr < sizeof(tr) + 4)
			break;

		switch (w->type) {
		case BINDER_WORK_TRANSACTION:{
			t = container_of(w, struct binder_transaction, work);
#ifdef BINDER_MONITOR
			binder_cancel_bwdog(t);
#endif
		} break;
		case BINDER_WORK_TRANSACTION_COMPLETE:{
				cmd = BR_TRANSACTION_COMPLETE;
				if (put_user(cmd, (uint32_t __user *) ptr))
					return -EFAULT;
				ptr += sizeof(uint32_t);

				binder_stat_br(proc, thread, cmd);
				binder_debug(BINDER_DEBUG_TRANSACTION_COMPLETE,
					     "%d:%d BR_TRANSACTION_COMPLETE\n",
					     proc->pid, thread->pid);

				list_del(&w->entry);
				kfree(w);
				binder_stats_deleted(BINDER_STAT_TRANSACTION_COMPLETE);
			}
			break;
		case BINDER_WORK_NODE:{
				struct binder_node *node =
				    container_of(w, struct binder_node, work);
				uint32_t cmd = BR_NOOP;
				const char *cmd_name;
				int strong = node->internal_strong_refs || node->local_strong_refs;
				int weak = !hlist_empty(&node->refs)
				    || node->local_weak_refs || strong;
				if (weak && !node->has_weak_ref) {
					cmd = BR_INCREFS;
					cmd_name = "BR_INCREFS";
					node->has_weak_ref = 1;
					node->pending_weak_ref = 1;
					node->local_weak_refs++;
				} else if (strong && !node->has_strong_ref) {
					cmd = BR_ACQUIRE;
					cmd_name = "BR_ACQUIRE";
					node->has_strong_ref = 1;
					node->pending_strong_ref = 1;
					node->local_strong_refs++;
				} else if (!strong && node->has_strong_ref) {
					cmd = BR_RELEASE;
					cmd_name = "BR_RELEASE";
					node->has_strong_ref = 0;
				} else if (!weak && node->has_weak_ref) {
					cmd = BR_DECREFS;
					cmd_name = "BR_DECREFS";
					node->has_weak_ref = 0;
				}
				if (cmd != BR_NOOP) {
					if (put_user(cmd, (uint32_t __user *) ptr))
						return -EFAULT;
					ptr += sizeof(uint32_t);
					if (put_user(node->ptr, (binder_uintptr_t __user *)
						     ptr))
						return -EFAULT;
					ptr += sizeof(binder_uintptr_t);
					if (put_user(node->cookie, (binder_uintptr_t __user *)
						     ptr))
						return -EFAULT;
					ptr += sizeof(binder_uintptr_t);

					binder_stat_br(proc, thread, cmd);
					binder_debug(BINDER_DEBUG_USER_REFS,
						     "%d:%d %s %d u%016llx c%016llx\n",
						     proc->pid, thread->pid,
						     cmd_name, node->debug_id,
						     (u64) node->ptr, (u64) node->cookie);
				} else {
					list_del_init(&w->entry);
					if (!weak && !strong) {
						binder_debug
						    (BINDER_DEBUG_INTERNAL_REFS,
						     "%d:%d node %d u%016llx c%016llx deleted\n",
						     proc->pid, thread->pid,
						     node->debug_id,
						     (u64) node->ptr, (u64) node->cookie);
						rb_erase(&node->rb_node, &proc->nodes);
						kfree(node);
						binder_stats_deleted(BINDER_STAT_NODE);
					} else {
						binder_debug
						    (BINDER_DEBUG_INTERNAL_REFS,
						     "%d:%d node %d u%016llx c%016llx state unchanged\n",
						     proc->pid, thread->pid,
						     node->debug_id,
						     (u64) node->ptr, (u64) node->cookie);
					}
				}
			}
			break;
		case BINDER_WORK_DEAD_BINDER:
		case BINDER_WORK_DEAD_BINDER_AND_CLEAR:
		case BINDER_WORK_CLEAR_DEATH_NOTIFICATION:{
				struct binder_ref_death *death;
				uint32_t cmd;

				death = container_of(w, struct binder_ref_death, work);

#ifdef MTK_DEATH_NOTIFY_MONITOR
				binder_debug
					(BINDER_DEBUG_DEATH_NOTIFICATION,
					"[DN #4]binder: %d:%d ",
						 proc->pid, thread->pid);
				switch (w->type) {
				case BINDER_WORK_DEAD_BINDER:
					binder_debug
					    (BINDER_DEBUG_DEATH_NOTIFICATION,
					     "BINDER_WORK_DEAD_BINDER cookie 0x%016llx\n",
					     (u64) death->cookie);
					break;
				case BINDER_WORK_DEAD_BINDER_AND_CLEAR:
					binder_debug
					    (BINDER_DEBUG_DEATH_NOTIFICATION,
					     "BINDER_WORK_DEAD_BINDER_AND_CLEAR cookie 0x%016llx\n",
					     (u64) death->cookie);
					break;
				case BINDER_WORK_CLEAR_DEATH_NOTIFICATION:
					binder_debug
					    (BINDER_DEBUG_DEATH_NOTIFICATION,
					     "BINDER_WORK_CLEAR_DEATH_NOTIFICATION cookie 0x%016llx\n",
					     (u64) death->cookie);
					break;
				default:
					binder_debug
					    (BINDER_DEBUG_DEATH_NOTIFICATION,
					     "UNKNOWN-%d cookie 0x%016llx\n",
					     w->type, (u64) death->cookie);
					break;
				}
#endif

				if (w->type == BINDER_WORK_CLEAR_DEATH_NOTIFICATION)
					cmd = BR_CLEAR_DEATH_NOTIFICATION_DONE;
				else
					cmd = BR_DEAD_BINDER;
				if (put_user(cmd, (uint32_t __user *) ptr))
					return -EFAULT;
				ptr += sizeof(uint32_t);
				if (put_user(death->cookie, (binder_uintptr_t __user *) ptr))
					return -EFAULT;
				ptr += sizeof(binder_uintptr_t);
				binder_stat_br(proc, thread, cmd);
				binder_debug(BINDER_DEBUG_DEATH_NOTIFICATION,
					     "%d:%d %s %016llx\n",
					     proc->pid, thread->pid,
					     cmd == BR_DEAD_BINDER ?
					     "BR_DEAD_BINDER" :
					     "BR_CLEAR_DEATH_NOTIFICATION_DONE",
					     (u64) death->cookie);

				if (w->type == BINDER_WORK_CLEAR_DEATH_NOTIFICATION) {
					list_del(&w->entry);
					kfree(death);
					binder_stats_deleted(BINDER_STAT_DEATH);
				} else
					list_move(&w->entry, &proc->delivered_death);
				if (cmd == BR_DEAD_BINDER)
					goto done;	/* DEAD_BINDER notifications can cause transactions */
			}
			break;
=======
		if (end - ptr < sizeof(tr) + 4) {
			binder_inner_proc_unlock(proc);
			break;
		}
		w = binder_dequeue_work_head_ilocked(list);
		if (binder_worklist_empty_ilocked(&thread->todo))
			thread->process_todo = false;

		switch (w->type) {
		case BINDER_WORK_TRANSACTION: {
			binder_inner_proc_unlock(proc);
			t = container_of(w, struct binder_transaction, work);
		} break;
		case BINDER_WORK_RETURN_ERROR: {
			struct binder_error *e = container_of(
					w, struct binder_error, work);

			WARN_ON(e->cmd == BR_OK);
			binder_inner_proc_unlock(proc);
			if (put_user(e->cmd, (uint32_t __user *)ptr))
				return -EFAULT;
			e->cmd = BR_OK;
			ptr += sizeof(uint32_t);

			binder_stat_br(proc, thread, e->cmd);
		} break;
		case BINDER_WORK_TRANSACTION_COMPLETE: {
			binder_inner_proc_unlock(proc);
			cmd = BR_TRANSACTION_COMPLETE;
			if (put_user(cmd, (uint32_t __user *)ptr))
				return -EFAULT;
			ptr += sizeof(uint32_t);

			binder_stat_br(proc, thread, cmd);
			binder_debug(BINDER_DEBUG_TRANSACTION_COMPLETE,
				     "%d:%d BR_TRANSACTION_COMPLETE\n",
				     proc->pid, thread->pid);
			kfree(w);
			binder_stats_deleted(BINDER_STAT_TRANSACTION_COMPLETE);
		} break;
		case BINDER_WORK_NODE: {
			struct binder_node *node = container_of(w, struct binder_node, work);
			int strong, weak;
			binder_uintptr_t node_ptr = node->ptr;
			binder_uintptr_t node_cookie = node->cookie;
			int node_debug_id = node->debug_id;
			int has_weak_ref;
			int has_strong_ref;
			void __user *orig_ptr = ptr;

			BUG_ON(proc != node->proc);
			strong = node->internal_strong_refs ||
					node->local_strong_refs;
			weak = !hlist_empty(&node->refs) ||
					node->local_weak_refs ||
					node->tmp_refs || strong;
			has_strong_ref = node->has_strong_ref;
			has_weak_ref = node->has_weak_ref;

			if (weak && !has_weak_ref) {
				node->has_weak_ref = 1;
				node->pending_weak_ref = 1;
				node->local_weak_refs++;
			}
			if (strong && !has_strong_ref) {
				node->has_strong_ref = 1;
				node->pending_strong_ref = 1;
				node->local_strong_refs++;
			}
			if (!strong && has_strong_ref)
				node->has_strong_ref = 0;
			if (!weak && has_weak_ref)
				node->has_weak_ref = 0;
			if (!weak && !strong) {
				binder_debug(BINDER_DEBUG_INTERNAL_REFS,
					     "%d:%d node %d u%016llx c%016llx deleted\n",
					     proc->pid, thread->pid,
					     node_debug_id,
					     (u64)node_ptr,
					     (u64)node_cookie);
				rb_erase(&node->rb_node, &proc->nodes);
				binder_inner_proc_unlock(proc);
				binder_node_lock(node);
				/*
				 * Acquire the node lock before freeing the
				 * node to serialize with other threads that
				 * may have been holding the node lock while
				 * decrementing this node (avoids race where
				 * this thread frees while the other thread
				 * is unlocking the node after the final
				 * decrement)
				 */
				binder_node_unlock(node);
				binder_free_node(node);
			} else
				binder_inner_proc_unlock(proc);

			if (weak && !has_weak_ref)
				ret = binder_put_node_cmd(
						proc, thread, &ptr, node_ptr,
						node_cookie, node_debug_id,
						BR_INCREFS, "BR_INCREFS");
			if (!ret && strong && !has_strong_ref)
				ret = binder_put_node_cmd(
						proc, thread, &ptr, node_ptr,
						node_cookie, node_debug_id,
						BR_ACQUIRE, "BR_ACQUIRE");
			if (!ret && !strong && has_strong_ref)
				ret = binder_put_node_cmd(
						proc, thread, &ptr, node_ptr,
						node_cookie, node_debug_id,
						BR_RELEASE, "BR_RELEASE");
			if (!ret && !weak && has_weak_ref)
				ret = binder_put_node_cmd(
						proc, thread, &ptr, node_ptr,
						node_cookie, node_debug_id,
						BR_DECREFS, "BR_DECREFS");
			if (orig_ptr == ptr)
				binder_debug(BINDER_DEBUG_INTERNAL_REFS,
					     "%d:%d node %d u%016llx c%016llx state unchanged\n",
					     proc->pid, thread->pid,
					     node_debug_id,
					     (u64)node_ptr,
					     (u64)node_cookie);
			if (ret)
				return ret;
		} break;
		case BINDER_WORK_DEAD_BINDER:
		case BINDER_WORK_DEAD_BINDER_AND_CLEAR:
		case BINDER_WORK_CLEAR_DEATH_NOTIFICATION: {
			struct binder_ref_death *death;
			uint32_t cmd;
			binder_uintptr_t cookie;

			death = container_of(w, struct binder_ref_death, work);
			if (w->type == BINDER_WORK_CLEAR_DEATH_NOTIFICATION)
				cmd = BR_CLEAR_DEATH_NOTIFICATION_DONE;
			else
				cmd = BR_DEAD_BINDER;
			cookie = death->cookie;

			binder_debug(BINDER_DEBUG_DEATH_NOTIFICATION,
				     "%d:%d %s %016llx\n",
				      proc->pid, thread->pid,
				      cmd == BR_DEAD_BINDER ?
				      "BR_DEAD_BINDER" :
				      "BR_CLEAR_DEATH_NOTIFICATION_DONE",
				      (u64)cookie);
			if (w->type == BINDER_WORK_CLEAR_DEATH_NOTIFICATION) {
				binder_inner_proc_unlock(proc);
				kfree(death);
				binder_stats_deleted(BINDER_STAT_DEATH);
			} else {
				binder_enqueue_work_ilocked(
						w, &proc->delivered_death);
				binder_inner_proc_unlock(proc);
			}
			if (put_user(cmd, (uint32_t __user *)ptr))
				return -EFAULT;
			ptr += sizeof(uint32_t);
			if (put_user(cookie,
				     (binder_uintptr_t __user *)ptr))
				return -EFAULT;
			ptr += sizeof(binder_uintptr_t);
			binder_stat_br(proc, thread, cmd);
			if (cmd == BR_DEAD_BINDER)
				goto done; /* DEAD_BINDER notifications can cause transactions */
		} break;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		}

		if (!t)
			continue;

		BUG_ON(t->buffer == NULL);
		if (t->buffer->target_node) {
			struct binder_node *target_node = t->buffer->target_node;
<<<<<<< HEAD

			tr.target.ptr = target_node->ptr;
			tr.cookie = target_node->cookie;
			t->saved_priority = task_nice(current);
#ifdef RT_PRIO_INHERIT
			/* since we may fail the rt inherit due to target
			 * wait queue task_list is empty, check again here.
			 */
			if ((SCHED_RR == t->policy || SCHED_FIFO == t->policy)
			    && t->rt_prio > current->rt_priority && !(t->flags & TF_ONE_WAY)) {
				struct sched_param param = {
					.sched_priority = t->rt_prio,
				};

				t->saved_rt_prio = current->rt_priority;
				t->saved_policy = current->policy;
				mt_sched_setscheduler_nocheck(current, t->policy, &param);
#ifdef BINDER_MONITOR
				if (log_disable & BINDER_RT_LOG_ENABLE) {
					pr_debug
					    ("read set %d sched_policy from %d to %d rt_prio from %d to %d\n",
					     proc->pid, t->saved_policy,
					     t->policy, t->saved_rt_prio, t->rt_prio);
				}
#endif
			}
#endif
			if (t->priority < target_node->min_priority && !(t->flags & TF_ONE_WAY))
				binder_set_nice(t->priority);
			else if (!(t->flags & TF_ONE_WAY) ||
				 t->saved_priority > target_node->min_priority)
				binder_set_nice(target_node->min_priority);
			cmd = BR_TRANSACTION;
		} else {
			tr.target.ptr = 0;
			tr.cookie = 0;
			cmd = BR_REPLY;
		}
		tr.code = t->code;
		tr.flags = t->flags;
		tr.sender_euid = from_kuid(current_user_ns(), t->sender_euid);

		if (t->from) {
			struct task_struct *sender = t->from->proc->tsk;

			tr.sender_pid = task_tgid_nr_ns(sender, task_active_pid_ns(current));
		} else {
			tr.sender_pid = 0;
		}

		tr.data_size = t->buffer->data_size;
		tr.offsets_size = t->buffer->offsets_size;
		tr.data.ptr.buffer = (binder_uintptr_t) ((uintptr_t) t->buffer->data +
							 proc->user_buffer_offset);
		tr.data.ptr.offsets =
		    tr.data.ptr.buffer + ALIGN(t->buffer->data_size, sizeof(void *));

		if (put_user(cmd, (uint32_t __user *) ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
		if (copy_to_user(ptr, &tr, sizeof(tr)))
			return -EFAULT;
		ptr += sizeof(tr);
=======
			struct binder_priority node_prio;

			trd->target.ptr = target_node->ptr;
			trd->cookie =  target_node->cookie;
			node_prio.sched_policy = target_node->sched_policy;
			node_prio.prio = target_node->min_priority;
			binder_transaction_priority(current, t, node_prio,
						    target_node->inherit_rt);
			cmd = BR_TRANSACTION;
		} else {
			trd->target.ptr = 0;
			trd->cookie = 0;
			cmd = BR_REPLY;
		}
		trd->code = t->code;
		trd->flags = t->flags;
		trd->sender_euid = from_kuid(current_user_ns(), t->sender_euid);

		t_from = binder_get_txn_from(t);
		if (t_from) {
			struct task_struct *sender = t_from->proc->tsk;

			trd->sender_pid =
				task_tgid_nr_ns(sender,
						task_active_pid_ns(current));
		} else {
			trd->sender_pid = 0;
		}

		trd->data_size = t->buffer->data_size;
		trd->offsets_size = t->buffer->offsets_size;
		trd->data.ptr.buffer = (binder_uintptr_t)
			((uintptr_t)t->buffer->data +
			binder_alloc_get_user_buffer_offset(&proc->alloc));
		trd->data.ptr.offsets = trd->data.ptr.buffer +
					ALIGN(t->buffer->data_size,
					    sizeof(void *));

		tr.secctx = t->security_ctx;
		if (t->security_ctx) {
			cmd = BR_TRANSACTION_SEC_CTX;
			trsize = sizeof(tr);
		}
		if (put_user(cmd, (uint32_t __user *)ptr)) {
			if (t_from)
				binder_thread_dec_tmpref(t_from);

			binder_cleanup_transaction(t, "put_user failed",
						   BR_FAILED_REPLY);

			return -EFAULT;
		}
		ptr += sizeof(uint32_t);
		if (copy_to_user(ptr, &tr, trsize)) {
			if (t_from)
				binder_thread_dec_tmpref(t_from);

			binder_cleanup_transaction(t, "copy_to_user failed",
						   BR_FAILED_REPLY);

			return -EFAULT;
		}
		ptr += trsize;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

		trace_binder_transaction_received(t);
		binder_stat_br(proc, thread, cmd);
		binder_debug(BINDER_DEBUG_TRANSACTION,
			     "%d:%d %s %d %d:%d, cmd %d size %zd-%zd ptr %016llx-%016llx\n",
			     proc->pid, thread->pid,
			     (cmd == BR_TRANSACTION) ? "BR_TRANSACTION" :
<<<<<<< HEAD
			     "BR_REPLY",
			     t->debug_id, t->from ? t->from->proc->pid : 0,
			     t->from ? t->from->pid : 0, cmd,
			     t->buffer->data_size, t->buffer->offsets_size,
			     (u64) tr.data.ptr.buffer, (u64) tr.data.ptr.offsets);

		list_del(&t->work.entry);
		t->buffer->allow_user_free = 1;
		if (cmd == BR_TRANSACTION && !(t->flags & TF_ONE_WAY)) {
			t->to_parent = thread->transaction_stack;
			t->to_thread = thread;
			thread->transaction_stack = t;
#ifdef BINDER_MONITOR
			do_posix_clock_monotonic_gettime(&t->exe_timestamp);
			/* monotonic_to_bootbased(&t->exe_timestamp); */
			do_gettimeofday(&t->tv);
			/* consider time zone. translate to android time */
			t->tv.tv_sec -= (sys_tz.tz_minuteswest * 60);
			t->wait_on = WAIT_ON_EXEC;
			t->tthrd = thread->pid;
			binder_queue_bwdog(t, (time_t) WAIT_BUDGET_EXEC);
			binder_update_transaction_time(&binder_transaction_log, t, 1);
			binder_update_transaction_ttid(&binder_transaction_log, t);
#endif
		} else {
			t->buffer->transaction = NULL;
#ifdef BINDER_MONITOR
			binder_cancel_bwdog(t);
			if (cmd == BR_TRANSACTION && (t->flags & TF_ONE_WAY)) {
				binder_update_transaction_time(&binder_transaction_log, t, 1);
				t->tthrd = thread->pid;
				binder_update_transaction_ttid(&binder_transaction_log, t);
			}
#endif
			kfree(t);
			binder_stats_deleted(BINDER_STAT_TRANSACTION);
=======
				(cmd == BR_TRANSACTION_SEC_CTX) ?
				     "BR_TRANSACTION_SEC_CTX" : "BR_REPLY",
			     t->debug_id, t_from ? t_from->proc->pid : 0,
			     t_from ? t_from->pid : 0, cmd,
			     t->buffer->data_size, t->buffer->offsets_size,
			     (u64)trd->data.ptr.buffer,
			     (u64)trd->data.ptr.offsets);

		if (t_from)
			binder_thread_dec_tmpref(t_from);
		t->buffer->allow_user_free = 1;
		if (cmd != BR_REPLY && !(t->flags & TF_ONE_WAY)) {
			binder_inner_proc_lock(thread->proc);
			t->to_parent = thread->transaction_stack;
			t->to_thread = thread;
			thread->transaction_stack = t;
			binder_inner_proc_unlock(thread->proc);
		} else {
			binder_free_transaction(t);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		}
		break;
	}

done:

	*consumed = ptr - buffer;
<<<<<<< HEAD
	if (proc->requested_threads + proc->ready_threads == 0 &&
	    proc->requested_threads_started < proc->max_threads &&
	    (thread->looper & (BINDER_LOOPER_STATE_REGISTERED | BINDER_LOOPER_STATE_ENTERED))
	    /* the user-space code fails to */
	    /*spawn a new thread if we leave this out */
	    ) {
		proc->requested_threads++;
		binder_debug(BINDER_DEBUG_THREADS,
			     "%d:%d BR_SPAWN_LOOPER\n", proc->pid, thread->pid);
		if (put_user(BR_SPAWN_LOOPER, (uint32_t __user *) buffer))
			return -EFAULT;
		binder_stat_br(proc, thread, BR_SPAWN_LOOPER);
	}
	return 0;
}

static void binder_release_work(struct list_head *list)
{
	struct binder_work *w;

	while (!list_empty(list)) {
		w = list_first_entry(list, struct binder_work, entry);
		list_del_init(&w->entry);
		switch (w->type) {
		case BINDER_WORK_TRANSACTION:{
				struct binder_transaction *t;

				t = container_of(w, struct binder_transaction, work);
				if (t->buffer->target_node && !(t->flags & TF_ONE_WAY)) {
					binder_send_failed_reply(t, BR_DEAD_REPLY);
				} else {
					binder_debug
					    (BINDER_DEBUG_DEAD_TRANSACTION,
					     "undelivered transaction %d\n", t->debug_id);
					t->buffer->transaction = NULL;
#ifdef BINDER_MONITOR
					binder_cancel_bwdog(t);
#endif
					kfree(t);
					binder_stats_deleted(BINDER_STAT_TRANSACTION);
				}
			}
			break;
		case BINDER_WORK_TRANSACTION_COMPLETE:{
				binder_debug(BINDER_DEBUG_DEAD_TRANSACTION,
					     "undelivered TRANSACTION_COMPLETE\n");
				kfree(w);
				binder_stats_deleted(BINDER_STAT_TRANSACTION_COMPLETE);
			}
			break;
		case BINDER_WORK_DEAD_BINDER_AND_CLEAR:
		case BINDER_WORK_CLEAR_DEATH_NOTIFICATION:{
				struct binder_ref_death *death;

				death = container_of(w, struct binder_ref_death, work);
				binder_debug(BINDER_DEBUG_DEAD_TRANSACTION,
					     "undelivered death notification, %016llx\n",
					     (u64) death->cookie);
				kfree(death);
				binder_stats_deleted(BINDER_STAT_DEATH);
			} break;
		default:
			pr_err("unexpected work type, %d, not freed\n", w->type);
=======
	binder_inner_proc_lock(proc);
	if (proc->requested_threads == 0 &&
	    list_empty(&thread->proc->waiting_threads) &&
	    proc->requested_threads_started < proc->max_threads &&
	    (thread->looper & (BINDER_LOOPER_STATE_REGISTERED |
	     BINDER_LOOPER_STATE_ENTERED)) /* the user-space code fails to */
	     /*spawn a new thread if we leave this out */) {
		proc->requested_threads++;
		binder_inner_proc_unlock(proc);
		binder_debug(BINDER_DEBUG_THREADS,
			     "%d:%d BR_SPAWN_LOOPER\n",
			     proc->pid, thread->pid);
		if (put_user(BR_SPAWN_LOOPER, (uint32_t __user *)buffer))
			return -EFAULT;
		binder_stat_br(proc, thread, BR_SPAWN_LOOPER);
	} else
		binder_inner_proc_unlock(proc);
	return 0;
}

static void binder_release_work(struct binder_proc *proc,
				struct list_head *list)
{
	struct binder_work *w;

	while (1) {
		w = binder_dequeue_work_head(proc, list);
		if (!w)
			return;

		switch (w->type) {
		case BINDER_WORK_TRANSACTION: {
			struct binder_transaction *t;

			t = container_of(w, struct binder_transaction, work);

			binder_cleanup_transaction(t, "process died.",
						   BR_DEAD_REPLY);
		} break;
		case BINDER_WORK_RETURN_ERROR: {
			struct binder_error *e = container_of(
					w, struct binder_error, work);

			binder_debug(BINDER_DEBUG_DEAD_TRANSACTION,
				"undelivered TRANSACTION_ERROR: %u\n",
				e->cmd);
		} break;
		case BINDER_WORK_TRANSACTION_COMPLETE: {
			binder_debug(BINDER_DEBUG_DEAD_TRANSACTION,
				"undelivered TRANSACTION_COMPLETE\n");
			kfree(w);
			binder_stats_deleted(BINDER_STAT_TRANSACTION_COMPLETE);
		} break;
		case BINDER_WORK_DEAD_BINDER_AND_CLEAR:
		case BINDER_WORK_CLEAR_DEATH_NOTIFICATION: {
			struct binder_ref_death *death;

			death = container_of(w, struct binder_ref_death, work);
			binder_debug(BINDER_DEBUG_DEAD_TRANSACTION,
				"undelivered death notification, %016llx\n",
				(u64)death->cookie);
			kfree(death);
			binder_stats_deleted(BINDER_STAT_DEATH);
		} break;
		default:
			pr_err("unexpected work type, %d, not freed\n",
			       w->type);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			break;
		}
	}

}

<<<<<<< HEAD
static struct binder_thread *binder_get_thread(struct binder_proc *proc)
=======
static struct binder_thread *binder_get_thread_ilocked(
		struct binder_proc *proc, struct binder_thread *new_thread)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	struct binder_thread *thread = NULL;
	struct rb_node *parent = NULL;
	struct rb_node **p = &proc->threads.rb_node;

	while (*p) {
		parent = *p;
		thread = rb_entry(parent, struct binder_thread, rb_node);

		if (current->pid < thread->pid)
			p = &(*p)->rb_left;
		else if (current->pid > thread->pid)
			p = &(*p)->rb_right;
		else
<<<<<<< HEAD
			break;
	}
	if (*p == NULL) {
		thread = kzalloc(sizeof(*thread), GFP_KERNEL);
		if (thread == NULL)
			return NULL;
		binder_stats_created(BINDER_STAT_THREAD);
		thread->proc = proc;
		thread->pid = current->pid;
		init_waitqueue_head(&thread->wait);
		INIT_LIST_HEAD(&thread->todo);
		rb_link_node(&thread->rb_node, parent, p);
		rb_insert_color(&thread->rb_node, &proc->threads);
		thread->looper |= BINDER_LOOPER_STATE_NEED_RETURN;
		thread->return_error = BR_OK;
		thread->return_error2 = BR_OK;
=======
			return thread;
	}
	if (!new_thread)
		return NULL;
	thread = new_thread;
	binder_stats_created(BINDER_STAT_THREAD);
	thread->proc = proc;
	thread->pid = current->pid;
	get_task_struct(current);
	thread->task = current;
	atomic_set(&thread->tmp_ref, 0);
	init_waitqueue_head(&thread->wait);
	INIT_LIST_HEAD(&thread->todo);
	rb_link_node(&thread->rb_node, parent, p);
	rb_insert_color(&thread->rb_node, &proc->threads);
	thread->looper_need_return = true;
	thread->return_error.work.type = BINDER_WORK_RETURN_ERROR;
	thread->return_error.cmd = BR_OK;
	thread->reply_error.work.type = BINDER_WORK_RETURN_ERROR;
	thread->reply_error.cmd = BR_OK;
	INIT_LIST_HEAD(&new_thread->waiting_thread_node);
	return thread;
}

static struct binder_thread *binder_get_thread(struct binder_proc *proc)
{
	struct binder_thread *thread;
	struct binder_thread *new_thread;

	binder_inner_proc_lock(proc);
	thread = binder_get_thread_ilocked(proc, NULL);
	binder_inner_proc_unlock(proc);
	if (!thread) {
		new_thread = kzalloc(sizeof(*thread), GFP_KERNEL);
		if (new_thread == NULL)
			return NULL;
		binder_inner_proc_lock(proc);
		thread = binder_get_thread_ilocked(proc, new_thread);
		binder_inner_proc_unlock(proc);
		if (thread != new_thread)
			kfree(new_thread);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	}
	return thread;
}

<<<<<<< HEAD
static int binder_free_thread(struct binder_proc *proc, struct binder_thread *thread)
=======
static void binder_free_proc(struct binder_proc *proc)
{
	BUG_ON(!list_empty(&proc->todo));
	BUG_ON(!list_empty(&proc->delivered_death));
	binder_alloc_deferred_release(&proc->alloc);
	put_task_struct(proc->tsk);
	binder_stats_deleted(BINDER_STAT_PROC);
	kfree(proc);
}

static void binder_free_thread(struct binder_thread *thread)
{
	BUG_ON(!list_empty(&thread->todo));
	binder_stats_deleted(BINDER_STAT_THREAD);
	binder_proc_dec_tmpref(thread->proc);
	put_task_struct(thread->task);
	kfree(thread);
}

static int binder_thread_release(struct binder_proc *proc,
				 struct binder_thread *thread)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	struct binder_transaction *t;
	struct binder_transaction *send_reply = NULL;
	int active_transactions = 0;
<<<<<<< HEAD

	rb_erase(&thread->rb_node, &proc->threads);
	t = thread->transaction_stack;
	if (t && t->to_thread == thread)
		send_reply = t;
	while (t) {
		active_transactions++;
		binder_debug(BINDER_DEBUG_DEAD_TRANSACTION,
			     "release %d:%d transaction %d %s, still active\n",
			     proc->pid, thread->pid,
			     t->debug_id, (t->to_thread == thread) ? "in" : "out");

#ifdef MTK_BINDER_DEBUG
		pr_err("%d: %p from %d:%d to %d:%d code %x flags %x " "pri %ld r%d "
#ifdef BINDER_MONITOR
		       "start %lu.%06lu"
#endif
		       ,
		       t->debug_id, t,
		       t->from ? t->from->proc->pid : 0,
		       t->from ? t->from->pid : 0,
		       t->to_proc ? t->to_proc->pid : 0,
		       t->to_thread ? t->to_thread->pid : 0,
		       t->code, t->flags, t->priority, t->need_reply
#ifdef BINDER_MONITOR
		       , (unsigned long)t->timestamp.tv_sec, (t->timestamp.tv_nsec / NSEC_PER_USEC)
#endif
		    );
#endif
=======
	struct binder_transaction *last_t = NULL;

	binder_inner_proc_lock(thread->proc);
	/*
	 * take a ref on the proc so it survives
	 * after we remove this thread from proc->threads.
	 * The corresponding dec is when we actually
	 * free the thread in binder_free_thread()
	 */
	proc->tmp_ref++;
	/*
	 * take a ref on this thread to ensure it
	 * survives while we are releasing it
	 */
	atomic_inc(&thread->tmp_ref);
	rb_erase(&thread->rb_node, &proc->threads);
	t = thread->transaction_stack;
	if (t) {
		spin_lock(&t->lock);
		if (t->to_thread == thread)
			send_reply = t;
	}
	thread->is_dead = true;

	while (t) {
		last_t = t;
		active_transactions++;
		binder_debug(BINDER_DEBUG_DEAD_TRANSACTION,
			     "release %d:%d transaction %d %s, still active\n",
			      proc->pid, thread->pid,
			     t->debug_id,
			     (t->to_thread == thread) ? "in" : "out");

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		if (t->to_thread == thread) {
			t->to_proc = NULL;
			t->to_thread = NULL;
			if (t->buffer) {
				t->buffer->transaction = NULL;
				t->buffer = NULL;
			}
			t = t->to_parent;
		} else if (t->from == thread) {
			t->from = NULL;
			t = t->from_parent;
		} else
			BUG();
<<<<<<< HEAD
	}
	if (send_reply)
		binder_send_failed_reply(send_reply, BR_DEAD_REPLY);
	binder_release_work(&thread->todo);
	kfree(thread);
	binder_stats_deleted(BINDER_STAT_THREAD);
	return active_transactions;
}

static unsigned int binder_poll(struct file *filp, struct poll_table_struct *wait)
{
	struct binder_proc *proc = filp->private_data;
	struct binder_thread *thread = NULL;
	int wait_for_proc_work;

	binder_lock(__func__);

	thread = binder_get_thread(proc);

	wait_for_proc_work = thread->transaction_stack == NULL &&
	    list_empty(&thread->todo) && thread->return_error == BR_OK;

	binder_unlock(__func__);

	if (wait_for_proc_work) {
		if (binder_has_proc_work(proc, thread))
			return POLLIN;
		poll_wait(filp, &proc->wait, wait);
		if (binder_has_proc_work(proc, thread))
			return POLLIN;
	} else {
		if (binder_has_thread_work(thread))
			return POLLIN;
		poll_wait(filp, &thread->wait, wait);
		if (binder_has_thread_work(thread))
			return POLLIN;
	}
=======
		spin_unlock(&last_t->lock);
		if (t)
			spin_lock(&t->lock);
	}

	/*
	 * If this thread used poll, make sure we remove the waitqueue
	 * from any epoll data structures holding it with POLLFREE.
	 * waitqueue_active() is safe to use here because we're holding
	 * the inner lock.
	 */
	if ((thread->looper & BINDER_LOOPER_STATE_POLL) &&
	    waitqueue_active(&thread->wait)) {
		wake_up_poll(&thread->wait, POLLHUP | POLLFREE);
	}

	binder_inner_proc_unlock(thread->proc);

	/*
	 * This is needed to avoid races between wake_up_poll() above and
	 * and ep_remove_waitqueue() called for other reasons (eg the epoll file
	 * descriptor being closed); ep_remove_waitqueue() holds an RCU read
	 * lock, so we can be sure it's done after calling synchronize_rcu().
	 */
	if (thread->looper & BINDER_LOOPER_STATE_POLL)
		synchronize_rcu();

	if (send_reply)
		binder_send_failed_reply(send_reply, BR_DEAD_REPLY);
	binder_release_work(proc, &thread->todo);
	binder_thread_dec_tmpref(thread);
	return active_transactions;
}

static unsigned int binder_poll(struct file *filp,
				struct poll_table_struct *wait)
{
	struct binder_proc *proc = filp->private_data;
	struct binder_thread *thread = NULL;
	bool wait_for_proc_work;

	thread = binder_get_thread(proc);

	binder_inner_proc_lock(thread->proc);
	thread->looper |= BINDER_LOOPER_STATE_POLL;
	wait_for_proc_work = binder_available_for_proc_work_ilocked(thread);

	binder_inner_proc_unlock(thread->proc);

	poll_wait(filp, &thread->wait, wait);

	if (binder_has_work(thread, wait_for_proc_work))
		return POLLIN;

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	return 0;
}

static int binder_ioctl_write_read(struct file *filp,
<<<<<<< HEAD
				   unsigned int cmd, unsigned long arg,
				   struct binder_thread *thread)
=======
				unsigned int cmd, unsigned long arg,
				struct binder_thread *thread)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	int ret = 0;
	struct binder_proc *proc = filp->private_data;
	unsigned int size = _IOC_SIZE(cmd);
	void __user *ubuf = (void __user *)arg;
	struct binder_write_read bwr;

	if (size != sizeof(struct binder_write_read)) {
		ret = -EINVAL;
		goto out;
	}
	if (copy_from_user(&bwr, ubuf, sizeof(bwr))) {
		ret = -EFAULT;
		goto out;
	}
	binder_debug(BINDER_DEBUG_READ_WRITE,
		     "%d:%d write %lld at %016llx, read %lld at %016llx\n",
		     proc->pid, thread->pid,
<<<<<<< HEAD
		     (u64) bwr.write_size, (u64) bwr.write_buffer,
		     (u64) bwr.read_size, (u64) bwr.read_buffer);
	if (bwr.write_size > 0) {
		ret = binder_thread_write(proc, thread,
					  bwr.write_buffer, bwr.write_size, &bwr.write_consumed);
=======
		     (u64)bwr.write_size, (u64)bwr.write_buffer,
		     (u64)bwr.read_size, (u64)bwr.read_buffer);

	if (bwr.write_size > 0) {
		ret = binder_thread_write(proc, thread,
					  bwr.write_buffer,
					  bwr.write_size,
					  &bwr.write_consumed);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		trace_binder_write_done(ret);
		if (ret < 0) {
			bwr.read_consumed = 0;
			if (copy_to_user(ubuf, &bwr, sizeof(bwr)))
				ret = -EFAULT;
			goto out;
		}
	}
	if (bwr.read_size > 0) {
		ret = binder_thread_read(proc, thread, bwr.read_buffer,
					 bwr.read_size,
<<<<<<< HEAD
					 &bwr.read_consumed, filp->f_flags & O_NONBLOCK);
		trace_binder_read_done(ret);
		if (!list_empty(&proc->todo)) {
			if (thread->proc != proc) {
				int i;
				unsigned int *p;

				pr_debug("binder: " "thread->proc != proc\n");
				pr_debug("binder: thread %p\n", thread);
				p = (unsigned int *)thread - 32;
				for (i = -4; i <= 3; i++, p += 8) {
					pr_debug("%p %08x %08x %08x %08x  %08x %08x %08x %08x\n",
						 p, *(p), *(p + 1), *(p + 2),
						 *(p + 3), *(p + 4), *(p + 5), *(p + 6), *(p + 7));
				}
				pr_debug("binder: thread->proc " "%p\n", thread->proc);
				p = (unsigned int *)thread->proc - 32;
				for (i = -4; i <= 5; i++, p += 8) {
					pr_debug("%p %08x %08x %08x %08x  %08x %08x %08x %08x\n",
						 p, *(p), *(p + 1), *(p + 2),
						 *(p + 3), *(p + 4), *(p + 5), *(p + 6), *(p + 7));
				}
				pr_debug("binder: proc %p\n", proc);
				p = (unsigned int *)proc - 32;
				for (i = -4; i <= 5; i++, p += 8) {
					pr_debug("%p %08x %08x %08x %08x  %08x %08x %08x %08x\n",
						 p, *(p), *(p + 1), *(p + 2),
						 *(p + 3), *(p + 4), *(p + 5), *(p + 6), *(p + 7));
				}
				BUG();
			}
			wake_up_interruptible(&proc->wait);
		}
=======
					 &bwr.read_consumed,
					 filp->f_flags & O_NONBLOCK);
		trace_binder_read_done(ret);
		binder_inner_proc_lock(proc);
		if (!binder_worklist_empty_ilocked(&proc->todo))
			binder_wakeup_proc_ilocked(proc);
		binder_inner_proc_unlock(proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		if (ret < 0) {
			if (copy_to_user(ubuf, &bwr, sizeof(bwr)))
				ret = -EFAULT;
			goto out;
		}
	}
	binder_debug(BINDER_DEBUG_READ_WRITE,
		     "%d:%d wrote %lld of %lld, read return %lld of %lld\n",
		     proc->pid, thread->pid,
<<<<<<< HEAD
		     (u64) bwr.write_consumed, (u64) bwr.write_size,
		     (u64) bwr.read_consumed, (u64) bwr.read_size);
=======
		     (u64)bwr.write_consumed, (u64)bwr.write_size,
		     (u64)bwr.read_consumed, (u64)bwr.read_size);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	if (copy_to_user(ubuf, &bwr, sizeof(bwr))) {
		ret = -EFAULT;
		goto out;
	}
out:
	return ret;
}

<<<<<<< HEAD
static int binder_ioctl_set_ctx_mgr(struct file *filp, struct binder_thread
				    *thread)
{
	int ret = 0;
	struct binder_proc *proc = filp->private_data;
	kuid_t curr_euid = current_euid();

	if (binder_context_mgr_node != NULL) {
=======
static int binder_ioctl_set_ctx_mgr(struct file *filp,
				    struct flat_binder_object *fbo)
{
	int ret = 0;
	struct binder_proc *proc = filp->private_data;
	struct binder_context *context = proc->context;
	struct binder_node *new_node;
	kuid_t curr_euid = current_euid();

	mutex_lock(&context->context_mgr_node_lock);
	if (context->binder_context_mgr_node) {
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		pr_err("BINDER_SET_CONTEXT_MGR already set\n");
		ret = -EBUSY;
		goto out;
	}
<<<<<<< HEAD

	ret = security_binder_set_context_mgr(proc->tsk);
	if (ret < 0)
		goto out;
	if (uid_valid(binder_context_mgr_uid)) {
		if (!uid_eq(binder_context_mgr_uid, curr_euid)) {
			pr_err("BINDER_SET_CONTEXT_MGR bad uid %d != %d\n",
			       from_kuid(&init_user_ns, curr_euid),
			       from_kuid(&init_user_ns, binder_context_mgr_uid));
=======
	ret = security_binder_set_context_mgr(proc->tsk);
	if (ret < 0)
		goto out;
	if (uid_valid(context->binder_context_mgr_uid)) {
		if (!uid_eq(context->binder_context_mgr_uid, curr_euid)) {
			pr_err("BINDER_SET_CONTEXT_MGR bad uid %d != %d\n",
			       from_kuid(&init_user_ns, curr_euid),
			       from_kuid(&init_user_ns,
					 context->binder_context_mgr_uid));
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			ret = -EPERM;
			goto out;
		}
	} else {
<<<<<<< HEAD
		binder_context_mgr_uid = curr_euid;
	}
	binder_context_mgr_node = binder_new_node(proc, 0, 0);
	if (binder_context_mgr_node == NULL) {
		ret = -ENOMEM;
		goto out;
	}
#ifdef BINDER_MONITOR
	strcpy(binder_context_mgr_node->name, "servicemanager");
	pr_debug("%d:%d set as servicemanager uid %d\n",
		 proc->pid, thread->pid, __kuid_val(binder_context_mgr_uid));
#endif
	binder_context_mgr_node->local_weak_refs++;
	binder_context_mgr_node->local_strong_refs++;
	binder_context_mgr_node->has_strong_ref = 1;
	binder_context_mgr_node->has_weak_ref = 1;
out:
	return ret;
}

=======
		context->binder_context_mgr_uid = curr_euid;
	}
	new_node = binder_new_node(proc, fbo);
	if (!new_node) {
		ret = -ENOMEM;
		goto out;
	}
	binder_node_lock(new_node);
	new_node->local_weak_refs++;
	new_node->local_strong_refs++;
	new_node->has_strong_ref = 1;
	new_node->has_weak_ref = 1;
	context->binder_context_mgr_node = new_node;
	binder_node_unlock(new_node);
	binder_put_node(new_node);
out:
	mutex_unlock(&context->context_mgr_node_lock);
	return ret;
}

static int binder_ioctl_get_node_info_for_ref(struct binder_proc *proc,
		struct binder_node_info_for_ref *info)
{
	struct binder_node *node;
	struct binder_context *context = proc->context;
	__u32 handle = info->handle;

	if (info->strong_count || info->weak_count || info->reserved1 ||
	    info->reserved2 || info->reserved3) {
		binder_user_error("%d BINDER_GET_NODE_INFO_FOR_REF: only handle may be non-zero.",
				  proc->pid);
		return -EINVAL;
	}

	/* This ioctl may only be used by the context manager */
	mutex_lock(&context->context_mgr_node_lock);
	if (!context->binder_context_mgr_node ||
		context->binder_context_mgr_node->proc != proc) {
		mutex_unlock(&context->context_mgr_node_lock);
		return -EPERM;
	}
	mutex_unlock(&context->context_mgr_node_lock);

	node = binder_get_node_from_ref(proc, handle, true, NULL);
	if (!node)
		return -EINVAL;

	info->strong_count = node->local_strong_refs +
		node->internal_strong_refs;
	info->weak_count = node->local_weak_refs;

	binder_put_node(node);

	return 0;
}

static int binder_ioctl_get_node_debug_info(struct binder_proc *proc,
				struct binder_node_debug_info *info) {
	struct rb_node *n;
	binder_uintptr_t ptr = info->ptr;

	memset(info, 0, sizeof(*info));

	binder_inner_proc_lock(proc);
	for (n = rb_first(&proc->nodes); n != NULL; n = rb_next(n)) {
		struct binder_node *node = rb_entry(n, struct binder_node,
						    rb_node);
		if (node->ptr > ptr) {
			info->ptr = node->ptr;
			info->cookie = node->cookie;
			info->has_strong_ref = node->has_strong_ref;
			info->has_weak_ref = node->has_weak_ref;
			break;
		}
	}
	binder_inner_proc_unlock(proc);

	return 0;
}

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
static long binder_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret;
	struct binder_proc *proc = filp->private_data;
	struct binder_thread *thread;
	unsigned int size = _IOC_SIZE(cmd);
	void __user *ubuf = (void __user *)arg;

<<<<<<< HEAD
	/*pr_info("binder_ioctl: %d:%d %x %lx\n", proc->pid, current->pid, cmd, arg); */
=======
	/*pr_info("binder_ioctl: %d:%d %x %lx\n",
			proc->pid, current->pid, cmd, arg);*/

	binder_selftest_alloc(&proc->alloc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	trace_binder_ioctl(cmd, arg);

	ret = wait_event_interruptible(binder_user_error_wait, binder_stop_on_user_error < 2);
	if (ret)
		goto err_unlocked;

<<<<<<< HEAD
	binder_lock(__func__);
=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	thread = binder_get_thread(proc);
	if (thread == NULL) {
		ret = -ENOMEM;
		goto err;
	}

	switch (cmd) {
	case BINDER_WRITE_READ:
		ret = binder_ioctl_write_read(filp, cmd, arg, thread);
		if (ret)
			goto err;
		break;
<<<<<<< HEAD
	case BINDER_SET_MAX_THREADS:
		if (copy_from_user(&proc->max_threads, ubuf, sizeof(proc->max_threads))) {
			ret = -EINVAL;
			goto err;
		}
		break;
	case BINDER_SET_CONTEXT_MGR:
		ret = binder_ioctl_set_ctx_mgr(filp, thread);
		if (ret)
			goto err;
		break;
	case BINDER_THREAD_EXIT:
		binder_debug(BINDER_DEBUG_THREADS, "%d:%d exit\n", proc->pid, thread->pid);
		binder_free_thread(proc, thread);
		thread = NULL;
		break;
	case BINDER_VERSION:{
			struct binder_version __user *ver = ubuf;

			if (size != sizeof(struct binder_version)) {
				ret = -EINVAL;
				goto err;
			}
			if (put_user(BINDER_CURRENT_PROTOCOL_VERSION, &ver->protocol_version)) {
				ret = -EINVAL;
				goto err;
			}
			break;
		}
=======
	case BINDER_SET_MAX_THREADS: {
		int max_threads;

		if (copy_from_user(&max_threads, ubuf,
				   sizeof(max_threads))) {
			ret = -EINVAL;
			goto err;
		}
		binder_inner_proc_lock(proc);
		proc->max_threads = max_threads;
		binder_inner_proc_unlock(proc);
		break;
	}
	case BINDER_SET_CONTEXT_MGR_EXT: {
		struct flat_binder_object fbo;

		if (copy_from_user(&fbo, ubuf, sizeof(fbo))) {
			ret = -EINVAL;
			goto err;
		}
		ret = binder_ioctl_set_ctx_mgr(filp, &fbo);
		if (ret)
			goto err;
		break;
	}
	case BINDER_SET_CONTEXT_MGR:
		ret = binder_ioctl_set_ctx_mgr(filp, NULL);
		if (ret)
			goto err;
		break;
	case BINDER_THREAD_EXIT:
		binder_debug(BINDER_DEBUG_THREADS, "%d:%d exit\n",
			     proc->pid, thread->pid);
		binder_thread_release(proc, thread);
		thread = NULL;
		break;
	case BINDER_VERSION: {
		struct binder_version __user *ver = ubuf;

		if (size != sizeof(struct binder_version)) {
			ret = -EINVAL;
			goto err;
		}
		if (put_user(BINDER_CURRENT_PROTOCOL_VERSION,
			     &ver->protocol_version)) {
			ret = -EINVAL;
			goto err;
		}
		break;
	}
	case BINDER_GET_NODE_INFO_FOR_REF: {
		struct binder_node_info_for_ref info;

		if (copy_from_user(&info, ubuf, sizeof(info))) {
			ret = -EFAULT;
			goto err;
		}

		ret = binder_ioctl_get_node_info_for_ref(proc, &info);
		if (ret < 0)
			goto err;

		if (copy_to_user(ubuf, &info, sizeof(info))) {
			ret = -EFAULT;
			goto err;
		}

		break;
	}
	case BINDER_GET_NODE_DEBUG_INFO: {
		struct binder_node_debug_info info;

		if (copy_from_user(&info, ubuf, sizeof(info))) {
			ret = -EFAULT;
			goto err;
		}

		ret = binder_ioctl_get_node_debug_info(proc, &info);
		if (ret < 0)
			goto err;

		if (copy_to_user(ubuf, &info, sizeof(info))) {
			ret = -EFAULT;
			goto err;
		}
		break;
	}
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	default:
		ret = -EINVAL;
		goto err;
	}
	ret = 0;
err:
	if (thread)
<<<<<<< HEAD
		thread->looper &= ~BINDER_LOOPER_STATE_NEED_RETURN;
	binder_unlock(__func__);
=======
		thread->looper_need_return = false;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	wait_event_interruptible(binder_user_error_wait, binder_stop_on_user_error < 2);
	if (ret && ret != -ERESTARTSYS)
		pr_info("%d:%d ioctl %x %lx returned %d\n", proc->pid, current->pid, cmd, arg, ret);
err_unlocked:
	trace_binder_ioctl_done(ret);
	return ret;
}

static void binder_vma_open(struct vm_area_struct *vma)
{
	struct binder_proc *proc = vma->vm_private_data;

	binder_debug(BINDER_DEBUG_OPEN_CLOSE,
		     "%d open vm area %lx-%lx (%ld K) vma %lx pagep %lx\n",
		     proc->pid, vma->vm_start, vma->vm_end,
		     (vma->vm_end - vma->vm_start) / SZ_1K, vma->vm_flags,
		     (unsigned long)pgprot_val(vma->vm_page_prot));
}

static void binder_vma_close(struct vm_area_struct *vma)
{
	struct binder_proc *proc = vma->vm_private_data;

	binder_debug(BINDER_DEBUG_OPEN_CLOSE,
		     "%d close vm area %lx-%lx (%ld K) vma %lx pagep %lx\n",
		     proc->pid, vma->vm_start, vma->vm_end,
		     (vma->vm_end - vma->vm_start) / SZ_1K, vma->vm_flags,
		     (unsigned long)pgprot_val(vma->vm_page_prot));
<<<<<<< HEAD
	proc->vma = NULL;
	proc->vma_vm_mm = NULL;
=======
	binder_alloc_vma_close(&proc->alloc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	binder_defer_work(proc, BINDER_DEFERRED_PUT_FILES);
}

static int binder_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	return VM_FAULT_SIGBUS;
}

static struct vm_operations_struct binder_vm_ops = {
	.open = binder_vma_open,
	.close = binder_vma_close,
	.fault = binder_vm_fault,
};

static int binder_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret;
<<<<<<< HEAD
	struct vm_struct *area;
	struct binder_proc *proc = filp->private_data;
	const char *failure_string;
	struct binder_buffer *buffer;

	if (proc->tsk != current)
=======
	struct binder_proc *proc = filp->private_data;
	const char *failure_string;

	if (proc->tsk != current->group_leader)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		return -EINVAL;

	if ((vma->vm_end - vma->vm_start) > SZ_4M)
		vma->vm_end = vma->vm_start + SZ_4M;

	binder_debug(BINDER_DEBUG_OPEN_CLOSE,
<<<<<<< HEAD
		     "binder_mmap: %d %lx-%lx (%ld K) vma %lx pagep %lx\n",
		     proc->pid, vma->vm_start, vma->vm_end,
=======
		     "%s: %d %lx-%lx (%ld K) vma %lx pagep %lx\n",
		     __func__, proc->pid, vma->vm_start, vma->vm_end,
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		     (vma->vm_end - vma->vm_start) / SZ_1K, vma->vm_flags,
		     (unsigned long)pgprot_val(vma->vm_page_prot));

	if (vma->vm_flags & FORBIDDEN_MMAP_FLAGS) {
		ret = -EPERM;
		failure_string = "bad vm_flags";
		goto err_bad_arg;
	}
	vma->vm_flags = (vma->vm_flags | VM_DONTCOPY) & ~VM_MAYWRITE;
<<<<<<< HEAD

	mutex_lock(&binder_mmap_lock);
	if (proc->buffer) {
		ret = -EBUSY;
		failure_string = "already mapped";
		goto err_already_mapped;
	}

	area = get_vm_area(vma->vm_end - vma->vm_start, VM_IOREMAP);
	if (area == NULL) {
		ret = -ENOMEM;
		failure_string = "get_vm_area";
		goto err_get_vm_area_failed;
	}
	proc->buffer = area->addr;
	proc->user_buffer_offset = vma->vm_start - (uintptr_t) proc->buffer;
	mutex_unlock(&binder_mmap_lock);

#ifdef CONFIG_CPU_CACHE_VIPT
	if (cache_is_vipt_aliasing()) {
		while (CACHE_COLOUR((vma->vm_start ^ (uint32_t) proc->buffer))) {
			pr_info
			    ("binder_mmap: %d %lx-%lx maps %pK bad alignment\n",
			     proc->pid, vma->vm_start, vma->vm_end, proc->buffer);
			vma->vm_start += PAGE_SIZE;
		}
	}
#endif
	proc->pages =
	    kzalloc(sizeof(proc->pages[0]) *
		    ((vma->vm_end - vma->vm_start) / PAGE_SIZE), GFP_KERNEL);
	if (proc->pages == NULL) {
		ret = -ENOMEM;
		failure_string = "alloc page array";
		goto err_alloc_pages_failed;
	}
	proc->buffer_size = vma->vm_end - vma->vm_start;

	vma->vm_ops = &binder_vm_ops;
	vma->vm_private_data = proc;

	if (binder_update_page_range(proc, 1, proc->buffer, proc->buffer + PAGE_SIZE, vma)) {
		ret = -ENOMEM;
		failure_string = "alloc small buf";
		goto err_alloc_small_buf_failed;
	}
	buffer = proc->buffer;
	INIT_LIST_HEAD(&proc->buffers);
	list_add(&buffer->entry, &proc->buffers);
	buffer->free = 1;
	binder_insert_free_buffer(proc, buffer);
	proc->free_async_space = proc->buffer_size / 2;
	barrier();
	proc->files = get_files_struct(current);
	proc->vma = vma;
	proc->vma_vm_mm = vma->vm_mm;

	/*pr_info("binder_mmap: %d %lx-%lx maps %pK\n",
	   proc->pid, vma->vm_start, vma->vm_end, proc->buffer); */
	return 0;

err_alloc_small_buf_failed:
	kfree(proc->pages);
	proc->pages = NULL;
err_alloc_pages_failed:
	mutex_lock(&binder_mmap_lock);
	vfree(proc->buffer);
	proc->buffer = NULL;
err_get_vm_area_failed:
err_already_mapped:
	mutex_unlock(&binder_mmap_lock);
=======
	vma->vm_ops = &binder_vm_ops;
	vma->vm_private_data = proc;

	ret = binder_alloc_mmap_handler(&proc->alloc, vma);
	if (ret)
		return ret;
	mutex_lock(&proc->files_lock);
	proc->files = get_files_struct(current);
	mutex_unlock(&proc->files_lock);
	return 0;

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
err_bad_arg:
	pr_err("binder_mmap: %d %lx-%lx %s failed %d\n",
	       proc->pid, vma->vm_start, vma->vm_end, failure_string, ret);
	return ret;
}

static int binder_open(struct inode *nodp, struct file *filp)
{
	struct binder_proc *proc;
<<<<<<< HEAD
=======
	struct binder_device *binder_dev;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	binder_debug(BINDER_DEBUG_OPEN_CLOSE, "binder_open: %d:%d\n",
		     current->group_leader->pid, current->pid);

	proc = kzalloc(sizeof(*proc), GFP_KERNEL);
	if (proc == NULL)
		return -ENOMEM;
<<<<<<< HEAD
	get_task_struct(current);
	proc->tsk = current;
	INIT_LIST_HEAD(&proc->todo);
	init_waitqueue_head(&proc->wait);
	proc->default_priority = task_nice(current);
#ifdef RT_PRIO_INHERIT
	proc->default_rt_prio = current->rt_priority;
	proc->default_policy = current->policy;
#endif

	binder_lock(__func__);

	binder_stats_created(BINDER_STAT_PROC);
	hlist_add_head(&proc->proc_node, &binder_procs);
	proc->pid = current->group_leader->pid;
	INIT_LIST_HEAD(&proc->delivered_death);
	filp->private_data = proc;

	binder_unlock(__func__);
=======
	spin_lock_init(&proc->inner_lock);
	spin_lock_init(&proc->outer_lock);
	get_task_struct(current->group_leader);
	proc->tsk = current->group_leader;
	mutex_init(&proc->files_lock);
	INIT_LIST_HEAD(&proc->todo);
	if (binder_supported_policy(current->policy)) {
		proc->default_priority.sched_policy = current->policy;
		proc->default_priority.prio = current->normal_prio;
	} else {
		proc->default_priority.sched_policy = SCHED_NORMAL;
		proc->default_priority.prio = NICE_TO_PRIO(0);
	}

	binder_dev = container_of(filp->private_data, struct binder_device,
				  miscdev);
	proc->context = &binder_dev->context;
	binder_alloc_init(&proc->alloc);

	binder_stats_created(BINDER_STAT_PROC);
	proc->pid = current->group_leader->pid;
	INIT_LIST_HEAD(&proc->delivered_death);
	INIT_LIST_HEAD(&proc->waiting_threads);
	filp->private_data = proc;

	mutex_lock(&binder_procs_lock);
	hlist_add_head(&proc->proc_node, &binder_procs);
	mutex_unlock(&binder_procs_lock);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	if (binder_debugfs_dir_entry_proc) {
		char strbuf[11];

		snprintf(strbuf, sizeof(strbuf), "%u", proc->pid);
<<<<<<< HEAD
		proc->debugfs_entry = debugfs_create_file(strbuf, S_IRUGO,
							  binder_debugfs_dir_entry_proc,
							  proc, &binder_proc_fops);
=======
		/*
		 * proc debug entries are shared between contexts, so
		 * this will fail if the process tries to open the driver
		 * again with a different context. The priting code will
		 * anyway print all contexts that a given PID has, so this
		 * is not a problem.
		 */
		proc->debugfs_entry = debugfs_create_file(strbuf, S_IRUGO,
			binder_debugfs_dir_entry_proc,
			(void *)(unsigned long)proc->pid,
			&binder_proc_fops);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	}

	return 0;
}

static int binder_flush(struct file *filp, fl_owner_t id)
{
	struct binder_proc *proc = filp->private_data;

	binder_defer_work(proc, BINDER_DEFERRED_FLUSH);

	return 0;
}

static void binder_deferred_flush(struct binder_proc *proc)
{
	struct rb_node *n;
	int wake_count = 0;

<<<<<<< HEAD
	for (n = rb_first(&proc->threads); n != NULL; n = rb_next(n)) {
		struct binder_thread *thread = rb_entry(n, struct binder_thread, rb_node);

		thread->looper |= BINDER_LOOPER_STATE_NEED_RETURN;
=======
	binder_inner_proc_lock(proc);
	for (n = rb_first(&proc->threads); n != NULL; n = rb_next(n)) {
		struct binder_thread *thread = rb_entry(n, struct binder_thread, rb_node);

		thread->looper_need_return = true;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		if (thread->looper & BINDER_LOOPER_STATE_WAITING) {
			wake_up_interruptible(&thread->wait);
			wake_count++;
		}
	}
<<<<<<< HEAD
	wake_up_interruptible_all(&proc->wait);

#ifdef MTK_BINDER_DEBUG
	if (wake_count)
		pr_debug("binder_flush: %d woke %d threads\n", proc->pid, wake_count);
#else
	binder_debug(BINDER_DEBUG_OPEN_CLOSE,
		     "binder_flush: %d woke %d threads\n", proc->pid, wake_count);
#endif
=======
	binder_inner_proc_unlock(proc);

	binder_debug(BINDER_DEBUG_OPEN_CLOSE,
		     "binder_flush: %d woke %d threads\n", proc->pid,
		     wake_count);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
}

static int binder_release(struct inode *nodp, struct file *filp)
{
	struct binder_proc *proc = filp->private_data;

	debugfs_remove(proc->debugfs_entry);
	binder_defer_work(proc, BINDER_DEFERRED_RELEASE);

	return 0;
}

static int binder_node_release(struct binder_node *node, int refs)
{
	struct binder_ref *ref;
	int death = 0;
<<<<<<< HEAD
#ifdef BINDER_MONITOR
	int sys_reg = 0;
#endif
#if defined(MTK_DEATH_NOTIFY_MONITOR) || defined(MTK_BINDER_DEBUG)
	int dead_pid = node->proc ? node->proc->pid : 0;
	char dead_pname[TASK_COMM_LEN] = "";

	if (node->proc && node->proc->tsk)
		strcpy(dead_pname, node->proc->tsk->comm);
#endif

	list_del_init(&node->work.entry);
	binder_release_work(&node->async_todo);

	if (hlist_empty(&node->refs)) {
		kfree(node);
		binder_stats_deleted(BINDER_STAT_NODE);
=======
	struct binder_proc *proc = node->proc;

	binder_release_work(proc, &node->async_todo);

	binder_node_lock(node);
	binder_inner_proc_lock(proc);
	binder_dequeue_work_ilocked(&node->work);
	/*
	 * The caller must have taken a temporary ref on the node,
	 */
	BUG_ON(!node->tmp_refs);
	if (hlist_empty(&node->refs) && node->tmp_refs == 1) {
		binder_inner_proc_unlock(proc);
		binder_node_unlock(node);
		binder_free_node(node);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

		return refs;
	}

	node->proc = NULL;
	node->local_strong_refs = 0;
	node->local_weak_refs = 0;
<<<<<<< HEAD
	hlist_add_head(&node->dead_node, &binder_dead_nodes);

	hlist_for_each_entry(ref, &node->refs, node_entry) {
		refs++;

		if (!ref->death)
			continue;

#ifdef MTK_DEATH_NOTIFY_MONITOR
		binder_debug(BINDER_DEBUG_DEATH_NOTIFICATION,
			     "[DN #3]binder: %d:(%s) cookie 0x%016llx\n", dead_pid,
#ifdef BINDER_MONITOR
			     node->name,
#else
			     dead_pname,
#endif
			     (u64) ref->death->cookie);
#endif
#ifdef BINDER_MONITOR
		if (!sys_reg && ref->proc->pid == system_server_pid)
			sys_reg = 1;
#endif
		death++;

		if (list_empty(&ref->death->work.entry)) {
			ref->death->work.type = BINDER_WORK_DEAD_BINDER;
			list_add_tail(&ref->death->work.entry, &ref->proc->todo);
			wake_up_interruptible(&ref->proc->wait);
		} else
			BUG();
	}

#if defined(BINDER_MONITOR) && defined(MTK_BINDER_DEBUG)
	if (sys_reg)
		pr_debug
		    ("%d:%s node %d:%s exits with %d:system_server DeathNotify\n",
		     dead_pid, dead_pname, node->debug_id, node->name, system_server_pid);
#endif

	binder_debug(BINDER_DEBUG_DEAD_BINDER,
		     "node %d now dead, refs %d, death %d\n", node->debug_id, refs, death);
=======
	binder_inner_proc_unlock(proc);

	spin_lock(&binder_dead_nodes_lock);
	hlist_add_head(&node->dead_node, &binder_dead_nodes);
	spin_unlock(&binder_dead_nodes_lock);

	hlist_for_each_entry(ref, &node->refs, node_entry) {
		refs++;
		/*
		 * Need the node lock to synchronize
		 * with new notification requests and the
		 * inner lock to synchronize with queued
		 * death notifications.
		 */
		binder_inner_proc_lock(ref->proc);
		if (!ref->death) {
			binder_inner_proc_unlock(ref->proc);
			continue;
		}

		death++;

		BUG_ON(!list_empty(&ref->death->work.entry));
		ref->death->work.type = BINDER_WORK_DEAD_BINDER;
		binder_enqueue_work_ilocked(&ref->death->work,
					    &ref->proc->todo);
		binder_wakeup_proc_ilocked(ref->proc);
		binder_inner_proc_unlock(ref->proc);
	}

	binder_debug(BINDER_DEBUG_DEAD_BINDER,
		     "node %d now dead, refs %d, death %d\n",
		     node->debug_id, refs, death);
	binder_node_unlock(node);
	binder_put_node(node);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	return refs;
}

static void binder_deferred_release(struct binder_proc *proc)
{
<<<<<<< HEAD
	struct binder_transaction *t;
	struct rb_node *n;
	int threads, nodes, incoming_refs, outgoing_refs, buffers, active_transactions, page_count;

	BUG_ON(proc->vma);
	BUG_ON(proc->files);

	hlist_del(&proc->proc_node);

	if (binder_context_mgr_node && binder_context_mgr_node->proc == proc) {
		binder_debug(BINDER_DEBUG_DEAD_BINDER,
			     "%s: %d context_mgr_node gone\n", __func__, proc->pid);
		binder_context_mgr_node = NULL;
	}

=======
	struct binder_context *context = proc->context;
	struct rb_node *n;
	int threads, nodes, incoming_refs, outgoing_refs, active_transactions;

	BUG_ON(proc->files);

	mutex_lock(&binder_procs_lock);
	hlist_del(&proc->proc_node);
	mutex_unlock(&binder_procs_lock);

	mutex_lock(&context->context_mgr_node_lock);
	if (context->binder_context_mgr_node &&
	    context->binder_context_mgr_node->proc == proc) {
		binder_debug(BINDER_DEBUG_DEAD_BINDER,
			     "%s: %d context_mgr_node gone\n",
			     __func__, proc->pid);
		context->binder_context_mgr_node = NULL;
	}
	mutex_unlock(&context->context_mgr_node_lock);
	binder_inner_proc_lock(proc);
	/*
	 * Make sure proc stays alive after we
	 * remove all the threads
	 */
	proc->tmp_ref++;

	proc->is_dead = true;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	threads = 0;
	active_transactions = 0;
	while ((n = rb_first(&proc->threads))) {
		struct binder_thread *thread;

		thread = rb_entry(n, struct binder_thread, rb_node);
<<<<<<< HEAD
		threads++;
		active_transactions += binder_free_thread(proc, thread);
=======
		binder_inner_proc_unlock(proc);
		threads++;
		active_transactions += binder_thread_release(proc, thread);
		binder_inner_proc_lock(proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	}

	nodes = 0;
	incoming_refs = 0;
	while ((n = rb_first(&proc->nodes))) {
		struct binder_node *node;

		node = rb_entry(n, struct binder_node, rb_node);
		nodes++;
<<<<<<< HEAD
		rb_erase(&node->rb_node, &proc->nodes);
		incoming_refs = binder_node_release(node, incoming_refs);
	}

	outgoing_refs = 0;
=======
		/*
		 * take a temporary ref on the node before
		 * calling binder_node_release() which will either
		 * kfree() the node or call binder_put_node()
		 */
		binder_inc_node_tmpref_ilocked(node);
		rb_erase(&node->rb_node, &proc->nodes);
		binder_inner_proc_unlock(proc);
		incoming_refs = binder_node_release(node, incoming_refs);
		binder_inner_proc_lock(proc);
	}
	binder_inner_proc_unlock(proc);

	outgoing_refs = 0;
	binder_proc_lock(proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	while ((n = rb_first(&proc->refs_by_desc))) {
		struct binder_ref *ref;

		ref = rb_entry(n, struct binder_ref, rb_node_desc);
		outgoing_refs++;
<<<<<<< HEAD
		binder_delete_ref(ref);
	}

	binder_release_work(&proc->todo);
	binder_release_work(&proc->delivered_death);

	buffers = 0;
	while ((n = rb_first(&proc->allocated_buffers))) {
		struct binder_buffer *buffer;

		buffer = rb_entry(n, struct binder_buffer, rb_node);

		t = buffer->transaction;
		if (t) {
			t->buffer = NULL;
			buffer->transaction = NULL;
			pr_err("release proc %d, transaction %d, not freed\n",
			       proc->pid, t->debug_id);
			/*BUG(); */
#ifdef MTK_BINDER_DEBUG
			pr_err("%d: %p from %d:%d to %d:%d code %x flags %x " "pri %ld r%d "
#ifdef BINDER_MONITOR
			       "start %lu.%06lu"
#endif
			       ,
			       t->debug_id, t,
			       t->from ? t->from->proc->pid : 0,
			       t->from ? t->from->pid : 0,
			       t->to_proc ? t->to_proc->pid : 0,
			       t->to_thread ? t->to_thread->pid : 0,
			       t->code, t->flags, t->priority, t->need_reply
#ifdef BINDER_MONITOR
			       , (unsigned long)t->timestamp.tv_sec,
			       (t->timestamp.tv_nsec / NSEC_PER_USEC)
#endif
			    );
#endif
		}

		binder_free_buf(proc, buffer);
		buffers++;
	}

	binder_stats_deleted(BINDER_STAT_PROC);

	page_count = 0;
	if (proc->pages) {
		int i;

		for (i = 0; i < proc->buffer_size / PAGE_SIZE; i++) {
			void *page_addr;

			if (!proc->pages[i])
				continue;

			page_addr = proc->buffer + i * PAGE_SIZE;
			binder_debug(BINDER_DEBUG_BUFFER_ALLOC,
				     "%s: %d: page %d at %pK not freed\n",
				     __func__, proc->pid, i, page_addr);
			unmap_kernel_range((unsigned long)page_addr, PAGE_SIZE);
			__free_page(proc->pages[i]);
			page_count++;
#ifdef MTK_BINDER_PAGE_USED_RECORD
			if (binder_page_used > 0)
				binder_page_used--;
			if (proc->page_used > 0)
				proc->page_used--;
#endif
		}
		kfree(proc->pages);
		vfree(proc->buffer);
	}

	put_task_struct(proc->tsk);

	binder_debug(BINDER_DEBUG_OPEN_CLOSE,
		     "%s: %d threads %d, nodes %d (ref %d), refs %d, active transactions %d, buffers %d, pages %d\n",
		     __func__, proc->pid, threads, nodes, incoming_refs,
		     outgoing_refs, active_transactions, buffers, page_count);

	kfree(proc);
=======
		binder_cleanup_ref_olocked(ref);
		binder_proc_unlock(proc);
		binder_free_ref(ref);
		binder_proc_lock(proc);
	}
	binder_proc_unlock(proc);

	binder_release_work(proc, &proc->todo);
	binder_release_work(proc, &proc->delivered_death);

	binder_debug(BINDER_DEBUG_OPEN_CLOSE,
		     "%s: %d threads %d, nodes %d (ref %d), refs %d, active transactions %d\n",
		     __func__, proc->pid, threads, nodes, incoming_refs,
		     outgoing_refs, active_transactions);

	binder_proc_dec_tmpref(proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
}

static void binder_deferred_func(struct work_struct *work)
{
	struct binder_proc *proc;
	struct files_struct *files;

	int defer;

	do {
<<<<<<< HEAD
		binder_lock(__func__);
		mutex_lock(&binder_deferred_lock);
		if (!hlist_empty(&binder_deferred_list)) {
			proc = hlist_entry(binder_deferred_list.first,
					   struct binder_proc, deferred_work_node);
=======
		mutex_lock(&binder_deferred_lock);
		if (!hlist_empty(&binder_deferred_list)) {
			proc = hlist_entry(binder_deferred_list.first,
					struct binder_proc, deferred_work_node);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			hlist_del_init(&proc->deferred_work_node);
			defer = proc->deferred_work;
			proc->deferred_work = 0;
		} else {
			proc = NULL;
			defer = 0;
		}
		mutex_unlock(&binder_deferred_lock);

		files = NULL;
		if (defer & BINDER_DEFERRED_PUT_FILES) {
<<<<<<< HEAD
			files = proc->files;
			if (files)
				proc->files = NULL;
=======
			mutex_lock(&proc->files_lock);
			files = proc->files;
			if (files)
				proc->files = NULL;
			mutex_unlock(&proc->files_lock);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		}

		if (defer & BINDER_DEFERRED_FLUSH)
			binder_deferred_flush(proc);

		if (defer & BINDER_DEFERRED_RELEASE)
<<<<<<< HEAD
			binder_deferred_release(proc);	/* frees proc */

		binder_unlock(__func__);
=======
			binder_deferred_release(proc); /* frees proc */

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		if (files)
			put_files_struct(files);
	} while (proc);
}
<<<<<<< HEAD

static DECLARE_WORK(binder_deferred_work, binder_deferred_func);

static void binder_defer_work(struct binder_proc *proc, enum binder_deferred_state defer)
=======
static DECLARE_WORK(binder_deferred_work, binder_deferred_func);

static void
binder_defer_work(struct binder_proc *proc, enum binder_deferred_state defer)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	mutex_lock(&binder_deferred_lock);
	proc->deferred_work |= defer;
	if (hlist_unhashed(&proc->deferred_work_node)) {
<<<<<<< HEAD
		hlist_add_head(&proc->deferred_work_node, &binder_deferred_list);
=======
		hlist_add_head(&proc->deferred_work_node,
				&binder_deferred_list);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		queue_work(binder_deferred_workqueue, &binder_deferred_work);
	}
	mutex_unlock(&binder_deferred_lock);
}

<<<<<<< HEAD
static void print_binder_transaction(struct seq_file *m, const char *prefix,
				     struct binder_transaction *t)
{
#ifdef BINDER_MONITOR
	struct rtc_time tm;

	rtc_time_to_tm(t->tv.tv_sec, &tm);
#endif
	seq_printf(m,
		   "%s %d: %pK from %d:%d to %d:%d code %x flags %x pri %ld r%d",
		   prefix, t->debug_id, t,
		   t->from ? t->from->proc->pid : 0,
		   t->from ? t->from->pid : 0,
		   t->to_proc ? t->to_proc->pid : 0,
		   t->to_thread ? t->to_thread->pid : 0,
		   t->code, t->flags, t->priority, t->need_reply);
	if (t->buffer == NULL) {
#ifdef BINDER_MONITOR
		seq_printf(m,
			   " start %lu.%06lu android %d-%02d-%02d %02d:%02d:%02d.%03lu",
			   (unsigned long)t->timestamp.tv_sec,
			   (t->timestamp.tv_nsec / NSEC_PER_USEC),
			   (tm.tm_year + 1900), (tm.tm_mon + 1), tm.tm_mday,
			   tm.tm_hour, tm.tm_min, tm.tm_sec,
			   (unsigned long)(t->tv.tv_usec / USEC_PER_MSEC));
#endif
		seq_puts(m, " buffer free\n");
		return;
	}
	if (t->buffer->target_node)
		seq_printf(m, " node %d", t->buffer->target_node->debug_id);
#ifdef BINDER_MONITOR
	seq_printf(m, " size %zd:%zd data %p auf %d start %lu.%06lu",
		   t->buffer->data_size, t->buffer->offsets_size,
		   t->buffer->data, t->buffer->allow_user_free,
		   (unsigned long)t->timestamp.tv_sec,
		   (t->timestamp.tv_nsec / NSEC_PER_USEC));
	seq_printf(m, " android %d-%02d-%02d %02d:%02d:%02d.%03lu\n",
		   (tm.tm_year + 1900), (tm.tm_mon + 1), tm.tm_mday,
		   tm.tm_hour, tm.tm_min, tm.tm_sec,
		   (unsigned long)(t->tv.tv_usec / USEC_PER_MSEC));
#else
	seq_printf(m, " size %zd:%zd data %pK\n",
		   t->buffer->data_size, t->buffer->offsets_size, t->buffer->data);
#endif
}

static void print_binder_buffer(struct seq_file *m, const char *prefix,
				struct binder_buffer *buffer)
{
	seq_printf(m, "%s %d: %pK size %zd:%zd %s\n",
		   prefix, buffer->debug_id, buffer->data,
		   buffer->data_size, buffer->offsets_size,
		   buffer->transaction ? "active" : "delivered");
}

static void print_binder_work(struct seq_file *m, const char *prefix,
			      const char *transaction_prefix, struct binder_work *w)
=======
static void print_binder_transaction_ilocked(struct seq_file *m,
					     struct binder_proc *proc,
					     const char *prefix,
					     struct binder_transaction *t)
{
	struct binder_proc *to_proc;
	struct binder_buffer *buffer = t->buffer;

	spin_lock(&t->lock);
	to_proc = t->to_proc;
	seq_printf(m,
		   "%s %d: %p from %d:%d to %d:%d code %x flags %x pri %d:%d r%d",
		   prefix, t->debug_id, t,
		   t->from ? t->from->proc->pid : 0,
		   t->from ? t->from->pid : 0,
		   to_proc ? to_proc->pid : 0,
		   t->to_thread ? t->to_thread->pid : 0,
		   t->code, t->flags, t->priority.sched_policy,
		   t->priority.prio, t->need_reply);
	spin_unlock(&t->lock);

	if (proc != to_proc) {
		/*
		 * Can only safely deref buffer if we are holding the
		 * correct proc inner lock for this node
		 */
		seq_puts(m, "\n");
		return;
	}

	if (buffer == NULL) {
		seq_puts(m, " buffer free\n");
		return;
	}
	if (buffer->target_node)
		seq_printf(m, " node %d", buffer->target_node->debug_id);
	seq_printf(m, " size %zd:%zd data %p\n",
		   buffer->data_size, buffer->offsets_size,
		   buffer->data);
}

static void print_binder_work_ilocked(struct seq_file *m,
				     struct binder_proc *proc,
				     const char *prefix,
				     const char *transaction_prefix,
				     struct binder_work *w)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	struct binder_node *node;
	struct binder_transaction *t;

	switch (w->type) {
	case BINDER_WORK_TRANSACTION:
		t = container_of(w, struct binder_transaction, work);
<<<<<<< HEAD
		print_binder_transaction(m, transaction_prefix, t);
		break;
=======
		print_binder_transaction_ilocked(
				m, proc, transaction_prefix, t);
		break;
	case BINDER_WORK_RETURN_ERROR: {
		struct binder_error *e = container_of(
				w, struct binder_error, work);

		seq_printf(m, "%stransaction error: %u\n",
			   prefix, e->cmd);
	} break;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	case BINDER_WORK_TRANSACTION_COMPLETE:
		seq_printf(m, "%stransaction complete\n", prefix);
		break;
	case BINDER_WORK_NODE:
		node = container_of(w, struct binder_node, work);
		seq_printf(m, "%snode work %d: u%016llx c%016llx\n",
<<<<<<< HEAD
			   prefix, node->debug_id, (u64) node->ptr, (u64) node->cookie);
=======
			   prefix, node->debug_id,
			   (u64)node->ptr, (u64)node->cookie);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		break;
	case BINDER_WORK_DEAD_BINDER:
		seq_printf(m, "%shas dead binder\n", prefix);
		break;
	case BINDER_WORK_DEAD_BINDER_AND_CLEAR:
		seq_printf(m, "%shas cleared dead binder\n", prefix);
		break;
	case BINDER_WORK_CLEAR_DEATH_NOTIFICATION:
		seq_printf(m, "%shas cleared death notification\n", prefix);
		break;
	default:
		seq_printf(m, "%sunknown work: type %d\n", prefix, w->type);
		break;
	}
}

<<<<<<< HEAD
static void print_binder_thread(struct seq_file *m, struct binder_thread *thread, int print_always)
=======
static void print_binder_thread_ilocked(struct seq_file *m,
					struct binder_thread *thread,
					int print_always)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	struct binder_transaction *t;
	struct binder_work *w;
	size_t start_pos = m->count;
	size_t header_pos;

<<<<<<< HEAD
	seq_printf(m, "  thread %d: l %02x\n", thread->pid, thread->looper);
=======
	seq_printf(m, "  thread %d: l %02x need_return %d tr %d\n",
			thread->pid, thread->looper,
			thread->looper_need_return,
			atomic_read(&thread->tmp_ref));
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	header_pos = m->count;
	t = thread->transaction_stack;
	while (t) {
		if (t->from == thread) {
<<<<<<< HEAD
			print_binder_transaction(m, "    outgoing transaction", t);
			t = t->from_parent;
		} else if (t->to_thread == thread) {
			print_binder_transaction(m, "    incoming transaction", t);
			t = t->to_parent;
		} else {
			print_binder_transaction(m, "    bad transaction", t);
=======
			print_binder_transaction_ilocked(m, thread->proc,
					"    outgoing transaction", t);
			t = t->from_parent;
		} else if (t->to_thread == thread) {
			print_binder_transaction_ilocked(m, thread->proc,
						 "    incoming transaction", t);
			t = t->to_parent;
		} else {
			print_binder_transaction_ilocked(m, thread->proc,
					"    bad transaction", t);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			t = NULL;
		}
	}
	list_for_each_entry(w, &thread->todo, entry) {
<<<<<<< HEAD
		print_binder_work(m, "    ", "    pending transaction", w);
=======
		print_binder_work_ilocked(m, thread->proc, "    ",
					  "    pending transaction", w);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	}
	if (!print_always && m->count == header_pos)
		m->count = start_pos;
}

<<<<<<< HEAD
static void print_binder_node(struct seq_file *m, struct binder_node *node)
=======
static void print_binder_node_nilocked(struct seq_file *m,
				       struct binder_node *node)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	struct binder_ref *ref;
	struct binder_work *w;
	int count;

	count = 0;
	hlist_for_each_entry(ref, &node->refs, node_entry)
		count++;

<<<<<<< HEAD
#ifdef BINDER_MONITOR
	seq_printf(m,
		   "  node %d (%s): u%016llx c%016llx hs %d hw %d ls %d lw %d is %d iw %d",
		   node->debug_id, node->name, (u64) node->ptr,
		   (u64) node->cookie, node->has_strong_ref, node->has_weak_ref,
		   node->local_strong_refs, node->local_weak_refs,
		   node->internal_strong_refs, count);
#else
	seq_printf(m,
		   "  node %d: u%016llx c%016llx hs %d hw %d ls %d lw %d is %d iw %d",
		   node->debug_id, (u64) node->ptr, (u64) node->cookie,
		   node->has_strong_ref, node->has_weak_ref,
		   node->local_strong_refs, node->local_weak_refs,
		   node->internal_strong_refs, count);
#endif
=======
	seq_printf(m, "  node %d: u%016llx c%016llx pri %d:%d hs %d hw %d ls %d lw %d is %d iw %d tr %d",
		   node->debug_id, (u64)node->ptr, (u64)node->cookie,
		   node->sched_policy, node->min_priority,
		   node->has_strong_ref, node->has_weak_ref,
		   node->local_strong_refs, node->local_weak_refs,
		   node->internal_strong_refs, count, node->tmp_refs);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	if (count) {
		seq_puts(m, " proc");
		hlist_for_each_entry(ref, &node->refs, node_entry)
			seq_printf(m, " %d", ref->proc->pid);
	}
	seq_puts(m, "\n");
<<<<<<< HEAD
#ifdef MTK_BINDER_DEBUG
	if (node->async_pid)
		seq_printf(m, "    pending async transaction on %d:\n", node->async_pid);
#endif
	list_for_each_entry(w, &node->async_todo, entry)
		print_binder_work(m, "    ", "    pending async transaction", w);
}

static void print_binder_ref(struct seq_file *m, struct binder_ref *ref)
{
	seq_printf(m, "  ref %d: desc %d %snode %d s %d w %d d %pK\n",
		   ref->debug_id, ref->desc, ref->node->proc ? "" : "dead ",
		   ref->node->debug_id, ref->strong, ref->weak, ref->death);
}

static void print_binder_proc(struct seq_file *m, struct binder_proc *proc, int print_all)
=======
	if (node->proc) {
		list_for_each_entry(w, &node->async_todo, entry)
			print_binder_work_ilocked(m, node->proc, "    ",
					  "    pending async transaction", w);
	}
}

static void print_binder_ref_olocked(struct seq_file *m,
				     struct binder_ref *ref)
{
	binder_node_lock(ref->node);
	seq_printf(m, "  ref %d: desc %d %snode %d s %d w %d d %pK\n",
		   ref->data.debug_id, ref->data.desc,
		   ref->node->proc ? "" : "dead ",
		   ref->node->debug_id, ref->data.strong,
		   ref->data.weak, ref->death);
	binder_node_unlock(ref->node);
}

static void print_binder_proc(struct seq_file *m,
			      struct binder_proc *proc, int print_all)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	struct binder_work *w;
	struct rb_node *n;
	size_t start_pos = m->count;
	size_t header_pos;
<<<<<<< HEAD

	seq_printf(m, "proc %d\n", proc->pid);
	header_pos = m->count;

	for (n = rb_first(&proc->threads); n != NULL; n = rb_next(n))
		print_binder_thread(m, rb_entry(n, struct binder_thread, rb_node), print_all);
	for (n = rb_first(&proc->nodes); n != NULL; n = rb_next(n)) {
		struct binder_node *node = rb_entry(n, struct binder_node,
						    rb_node);
		if (print_all || node->has_async_transaction)
			print_binder_node(m, node);
	}
	if (print_all) {
		for (n = rb_first(&proc->refs_by_desc); n != NULL; n = rb_next(n))
			print_binder_ref(m, rb_entry(n, struct binder_ref, rb_node_desc));
	}
	for (n = rb_first(&proc->allocated_buffers); n != NULL; n = rb_next(n))
		print_binder_buffer(m, "  buffer", rb_entry(n, struct binder_buffer, rb_node));
	list_for_each_entry(w, &proc->todo, entry)
		print_binder_work(m, "  ", "  pending transaction", w);
=======
	struct binder_node *last_node = NULL;

	seq_printf(m, "proc %d\n", proc->pid);
	seq_printf(m, "context %s\n", proc->context->name);
	header_pos = m->count;

	binder_inner_proc_lock(proc);
	for (n = rb_first(&proc->threads); n != NULL; n = rb_next(n))
		print_binder_thread_ilocked(m, rb_entry(n, struct binder_thread,
						rb_node), print_all);

	for (n = rb_first(&proc->nodes); n != NULL; n = rb_next(n)) {
		struct binder_node *node = rb_entry(n, struct binder_node,
						    rb_node);
		/*
		 * take a temporary reference on the node so it
		 * survives and isn't removed from the tree
		 * while we print it.
		 */
		binder_inc_node_tmpref_ilocked(node);
		/* Need to drop inner lock to take node lock */
		binder_inner_proc_unlock(proc);
		if (last_node)
			binder_put_node(last_node);
		binder_node_inner_lock(node);
		print_binder_node_nilocked(m, node);
		binder_node_inner_unlock(node);
		last_node = node;
		binder_inner_proc_lock(proc);
	}
	binder_inner_proc_unlock(proc);
	if (last_node)
		binder_put_node(last_node);

	if (print_all) {
		binder_proc_lock(proc);
		for (n = rb_first(&proc->refs_by_desc);
		     n != NULL;
		     n = rb_next(n))
			print_binder_ref_olocked(m, rb_entry(n,
							    struct binder_ref,
							    rb_node_desc));
		binder_proc_unlock(proc);
	}
	binder_alloc_print_allocated(m, &proc->alloc);
	binder_inner_proc_lock(proc);
	list_for_each_entry(w, &proc->todo, entry)
		print_binder_work_ilocked(m, proc, "  ",
					  "  pending transaction", w);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	list_for_each_entry(w, &proc->delivered_death, entry) {
		seq_puts(m, "  has delivered dead binder\n");
		break;
	}
<<<<<<< HEAD
=======
	binder_inner_proc_unlock(proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	if (!print_all && m->count == header_pos)
		m->count = start_pos;
}

<<<<<<< HEAD
static const char *const binder_return_strings[] = {
=======
static const char * const binder_return_strings[] = {
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	"BR_ERROR",
	"BR_OK",
	"BR_TRANSACTION",
	"BR_REPLY",
	"BR_ACQUIRE_RESULT",
	"BR_DEAD_REPLY",
	"BR_TRANSACTION_COMPLETE",
	"BR_INCREFS",
	"BR_ACQUIRE",
	"BR_RELEASE",
	"BR_DECREFS",
	"BR_ATTEMPT_ACQUIRE",
	"BR_NOOP",
	"BR_SPAWN_LOOPER",
	"BR_FINISHED",
	"BR_DEAD_BINDER",
	"BR_CLEAR_DEATH_NOTIFICATION_DONE",
	"BR_FAILED_REPLY"
};

<<<<<<< HEAD
static const char *const binder_command_strings[] = {
=======
static const char * const binder_command_strings[] = {
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	"BC_TRANSACTION",
	"BC_REPLY",
	"BC_ACQUIRE_RESULT",
	"BC_FREE_BUFFER",
	"BC_INCREFS",
	"BC_ACQUIRE",
	"BC_RELEASE",
	"BC_DECREFS",
	"BC_INCREFS_DONE",
	"BC_ACQUIRE_DONE",
	"BC_ATTEMPT_ACQUIRE",
	"BC_REGISTER_LOOPER",
	"BC_ENTER_LOOPER",
	"BC_EXIT_LOOPER",
	"BC_REQUEST_DEATH_NOTIFICATION",
	"BC_CLEAR_DEATH_NOTIFICATION",
<<<<<<< HEAD
	"BC_DEAD_BINDER_DONE"
};

static const char *const binder_objstat_strings[] = {
=======
	"BC_DEAD_BINDER_DONE",
	"BC_TRANSACTION_SG",
	"BC_REPLY_SG",
};

static const char * const binder_objstat_strings[] = {
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	"proc",
	"thread",
	"node",
	"ref",
	"death",
	"transaction",
	"transaction_complete"
};

<<<<<<< HEAD
static void print_binder_stats(struct seq_file *m, const char *prefix, struct binder_stats *stats)
{
	int i;

	BUILD_BUG_ON(ARRAY_SIZE(stats->bc) != ARRAY_SIZE(binder_command_strings));
	for (i = 0; i < ARRAY_SIZE(stats->bc); i++) {
		if (stats->bc[i])
			seq_printf(m, "%s%s: %d\n", prefix,
				   binder_command_strings[i], stats->bc[i]);
	}

	BUILD_BUG_ON(ARRAY_SIZE(stats->br) != ARRAY_SIZE(binder_return_strings));
	for (i = 0; i < ARRAY_SIZE(stats->br); i++) {
		if (stats->br[i])
			seq_printf(m, "%s%s: %d\n", prefix, binder_return_strings[i], stats->br[i]);
	}

	BUILD_BUG_ON(ARRAY_SIZE(stats->obj_created) != ARRAY_SIZE(binder_objstat_strings));
	BUILD_BUG_ON(ARRAY_SIZE(stats->obj_created) != ARRAY_SIZE(stats->obj_deleted));
	for (i = 0; i < ARRAY_SIZE(stats->obj_created); i++) {
		if (stats->obj_created[i] || stats->obj_deleted[i])
			seq_printf(m, "%s%s: active %d total %d\n", prefix,
				   binder_objstat_strings[i],
				   stats->obj_created[i] -
				   stats->obj_deleted[i], stats->obj_created[i]);
	}
}

static void print_binder_proc_stats(struct seq_file *m, struct binder_proc *proc)
{
	struct binder_work *w;
	struct rb_node *n;
	int count, strong, weak;

	seq_printf(m, "proc %d\n", proc->pid);
	count = 0;
	for (n = rb_first(&proc->threads); n != NULL; n = rb_next(n))
		count++;
	seq_printf(m, "  threads: %d\n", count);
	seq_printf(m, "  requested threads: %d+%d/%d\n"
		   "  ready threads %d\n"
		   "  free async space %zd\n", proc->requested_threads,
		   proc->requested_threads_started, proc->max_threads,
		   proc->ready_threads, proc->free_async_space);
	count = 0;
	for (n = rb_first(&proc->nodes); n != NULL; n = rb_next(n))
		count++;
=======
static void print_binder_stats(struct seq_file *m, const char *prefix,
			       struct binder_stats *stats)
{
	int i;

	BUILD_BUG_ON(ARRAY_SIZE(stats->bc) !=
		     ARRAY_SIZE(binder_command_strings));
	for (i = 0; i < ARRAY_SIZE(stats->bc); i++) {
		int temp = atomic_read(&stats->bc[i]);

		if (temp)
			seq_printf(m, "%s%s: %d\n", prefix,
				   binder_command_strings[i], temp);
	}

	BUILD_BUG_ON(ARRAY_SIZE(stats->br) !=
		     ARRAY_SIZE(binder_return_strings));
	for (i = 0; i < ARRAY_SIZE(stats->br); i++) {
		int temp = atomic_read(&stats->br[i]);

		if (temp)
			seq_printf(m, "%s%s: %d\n", prefix,
				   binder_return_strings[i], temp);
	}

	BUILD_BUG_ON(ARRAY_SIZE(stats->obj_created) !=
		     ARRAY_SIZE(binder_objstat_strings));
	BUILD_BUG_ON(ARRAY_SIZE(stats->obj_created) !=
		     ARRAY_SIZE(stats->obj_deleted));
	for (i = 0; i < ARRAY_SIZE(stats->obj_created); i++) {
		int created = atomic_read(&stats->obj_created[i]);
		int deleted = atomic_read(&stats->obj_deleted[i]);

		if (created || deleted)
			seq_printf(m, "%s%s: active %d total %d\n",
				prefix,
				binder_objstat_strings[i],
				created - deleted,
				created);
	}
}

static void print_binder_proc_stats(struct seq_file *m,
				    struct binder_proc *proc)
{
	struct binder_work *w;
	struct binder_thread *thread;
	struct rb_node *n;
	int count, strong, weak, ready_threads;
	size_t free_async_space =
		binder_alloc_get_free_async_space(&proc->alloc);

	seq_printf(m, "proc %d\n", proc->pid);
	seq_printf(m, "context %s\n", proc->context->name);
	count = 0;
	ready_threads = 0;
	binder_inner_proc_lock(proc);
	for (n = rb_first(&proc->threads); n != NULL; n = rb_next(n))
		count++;

	list_for_each_entry(thread, &proc->waiting_threads, waiting_thread_node)
		ready_threads++;

	seq_printf(m, "  threads: %d\n", count);
	seq_printf(m, "  requested threads: %d+%d/%d\n"
			"  ready threads %d\n"
			"  free async space %zd\n", proc->requested_threads,
			proc->requested_threads_started, proc->max_threads,
			ready_threads,
			free_async_space);
	count = 0;
	for (n = rb_first(&proc->nodes); n != NULL; n = rb_next(n))
		count++;
	binder_inner_proc_unlock(proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	seq_printf(m, "  nodes: %d\n", count);
	count = 0;
	strong = 0;
	weak = 0;
<<<<<<< HEAD
=======
	binder_proc_lock(proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	for (n = rb_first(&proc->refs_by_desc); n != NULL; n = rb_next(n)) {
		struct binder_ref *ref = rb_entry(n, struct binder_ref,
						  rb_node_desc);
		count++;
<<<<<<< HEAD
		strong += ref->strong;
		weak += ref->weak;
	}
	seq_printf(m, "  refs: %d s %d w %d\n", count, strong, weak);

	count = 0;
	for (n = rb_first(&proc->allocated_buffers); n != NULL; n = rb_next(n))
		count++;
	seq_printf(m, "  buffers: %d\n", count);

	count = 0;
	list_for_each_entry(w, &proc->todo, entry) {
		switch (w->type) {
		case BINDER_WORK_TRANSACTION:
			count++;
			break;
		default:
			break;
		}
	}
=======
		strong += ref->data.strong;
		weak += ref->data.weak;
	}
	binder_proc_unlock(proc);
	seq_printf(m, "  refs: %d s %d w %d\n", count, strong, weak);

	count = binder_alloc_get_allocated_count(&proc->alloc);
	seq_printf(m, "  buffers: %d\n", count);

	binder_alloc_print_pages(m, &proc->alloc);

	count = 0;
	binder_inner_proc_lock(proc);
	list_for_each_entry(w, &proc->todo, entry) {
		if (w->type == BINDER_WORK_TRANSACTION)
			count++;
	}
	binder_inner_proc_unlock(proc);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	seq_printf(m, "  pending transactions: %d\n", count);

	print_binder_stats(m, "  ", &proc->stats);
}

<<<<<<< HEAD
=======

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
static int binder_state_show(struct seq_file *m, void *unused)
{
	struct binder_proc *proc;
	struct binder_node *node;
<<<<<<< HEAD
	int do_lock = !binder_debug_no_lock;

	if (do_lock)
		binder_lock(__func__);

	seq_puts(m, "binder state:\n");

	if (!hlist_empty(&binder_dead_nodes))
		seq_puts(m, "dead nodes:\n");
	hlist_for_each_entry(node, &binder_dead_nodes, dead_node)
		print_binder_node(m, node);

	hlist_for_each_entry(proc, &binder_procs, proc_node)
		print_binder_proc(m, proc, 1);
	if (do_lock)
		binder_unlock(__func__);
=======
	struct binder_node *last_node = NULL;

	seq_puts(m, "binder state:\n");

	spin_lock(&binder_dead_nodes_lock);
	if (!hlist_empty(&binder_dead_nodes))
		seq_puts(m, "dead nodes:\n");
	hlist_for_each_entry(node, &binder_dead_nodes, dead_node) {
		/*
		 * take a temporary reference on the node so it
		 * survives and isn't removed from the list
		 * while we print it.
		 */
		node->tmp_refs++;
		spin_unlock(&binder_dead_nodes_lock);
		if (last_node)
			binder_put_node(last_node);
		binder_node_lock(node);
		print_binder_node_nilocked(m, node);
		binder_node_unlock(node);
		last_node = node;
		spin_lock(&binder_dead_nodes_lock);
	}
	spin_unlock(&binder_dead_nodes_lock);
	if (last_node)
		binder_put_node(last_node);

	mutex_lock(&binder_procs_lock);
	hlist_for_each_entry(proc, &binder_procs, proc_node)
		print_binder_proc(m, proc, 1);
	mutex_unlock(&binder_procs_lock);

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	return 0;
}

static int binder_stats_show(struct seq_file *m, void *unused)
{
	struct binder_proc *proc;
<<<<<<< HEAD
	int do_lock = !binder_debug_no_lock;

	if (do_lock)
		binder_lock(__func__);
=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	seq_puts(m, "binder stats:\n");

	print_binder_stats(m, "", &binder_stats);

<<<<<<< HEAD
	hlist_for_each_entry(proc, &binder_procs, proc_node)
		print_binder_proc_stats(m, proc);
	if (do_lock)
		binder_unlock(__func__);
=======
	mutex_lock(&binder_procs_lock);
	hlist_for_each_entry(proc, &binder_procs, proc_node)
		print_binder_proc_stats(m, proc);
	mutex_unlock(&binder_procs_lock);

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	return 0;
}

static int binder_transactions_show(struct seq_file *m, void *unused)
{
	struct binder_proc *proc;
<<<<<<< HEAD
	int do_lock = !binder_debug_no_lock;

	if (do_lock)
		binder_lock(__func__);

	seq_puts(m, "binder transactions:\n");
	hlist_for_each_entry(proc, &binder_procs, proc_node)
		print_binder_proc(m, proc, 0);
	if (do_lock)
		binder_unlock(__func__);
=======

	seq_puts(m, "binder transactions:\n");
	mutex_lock(&binder_procs_lock);
	hlist_for_each_entry(proc, &binder_procs, proc_node)
		print_binder_proc(m, proc, 0);
	mutex_unlock(&binder_procs_lock);

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	return 0;
}

static int binder_proc_show(struct seq_file *m, void *unused)
{
	struct binder_proc *itr;
<<<<<<< HEAD
	struct binder_proc *proc = m->private;
	int do_lock = !binder_debug_no_lock;
	bool valid_proc = false;

	if (do_lock)
		binder_lock(__func__);

	hlist_for_each_entry(itr, &binder_procs, proc_node) {
		if (itr == proc) {
			valid_proc = true;
			break;
		}
	}
	if (valid_proc) {
		seq_puts(m, "binder proc state:\n");
		print_binder_proc(m, proc, 1);
	}
#ifdef MTK_BINDER_DEBUG
	else
		pr_debug("show proc addr 0x%p exit\n", proc);
#endif
	if (do_lock)
		binder_unlock(__func__);
	return 0;
}

static void print_binder_transaction_log_entry(struct seq_file *m, struct
					       binder_transaction_log_entry * e)
{
#ifdef BINDER_MONITOR
	char tmp[30];
	struct rtc_time tm;
	struct timespec sub_read_t, sub_total_t;
	unsigned long read_ms = 0;
	unsigned long total_ms = 0;

	memset(&sub_read_t, 0, sizeof(sub_read_t));
	memset(&sub_total_t, 0, sizeof(sub_total_t));

	if (e->fd != -1)
		sprintf(tmp, " (fd %d)", e->fd);
	else
		tmp[0] = '\0';

	if ((e->call_type == 0) && timespec_valid_strict(&e->endstamp) &&
	    (timespec_compare(&e->endstamp, &e->timestamp) > 0)) {
		sub_total_t = timespec_sub(e->endstamp, e->timestamp);
		total_ms = ((unsigned long)sub_total_t.tv_sec) * MSEC_PER_SEC +
		    sub_total_t.tv_nsec / NSEC_PER_MSEC;
	}
	if ((e->call_type == 1) && timespec_valid_strict(&e->readstamp) &&
	    (timespec_compare(&e->readstamp, &e->timestamp) > 0)) {
		sub_read_t = timespec_sub(e->readstamp, e->timestamp);
		read_ms = ((unsigned long)sub_read_t.tv_sec) * MSEC_PER_SEC +
		    sub_read_t.tv_nsec / NSEC_PER_MSEC;
	}

	rtc_time_to_tm(e->tv.tv_sec, &tm);
	seq_printf(m,
		   "%d: %s from %d:%d to %d:%d node %d handle %d (%s) size %d:%d%s dex %u",
		   e->debug_id, (e->call_type == 2) ? "reply" :
		   ((e->call_type == 1) ? "async" : "call "),
		   e->from_proc, e->from_thread, e->to_proc, e->to_thread,
		   e->to_node, e->target_handle, e->service,
		   e->data_size, e->offsets_size, tmp, e->code);
	seq_printf(m,
			" start %lu.%06lu android %d-%02d-%02d %02d:%02d:%02d.%03lu read %lu.%06lu %s %lu.%06lu total %lu.%06lums\n",
		   (unsigned long)e->timestamp.tv_sec,
		   (e->timestamp.tv_nsec / NSEC_PER_USEC),
		   (tm.tm_year + 1900), (tm.tm_mon + 1), tm.tm_mday,
		   tm.tm_hour, tm.tm_min, tm.tm_sec,
		   (unsigned long)(e->tv.tv_usec / USEC_PER_MSEC),
		   (unsigned long)e->readstamp.tv_sec,
		   (e->readstamp.tv_nsec / NSEC_PER_USEC),
		   (e->call_type == 0) ? "end" : "",
		   (e->call_type ==
		    0) ? ((unsigned long)e->endstamp.tv_sec) : 0,
		   (e->call_type ==
		    0) ? (e->endstamp.tv_nsec / NSEC_PER_USEC) : 0,
		   (e->call_type == 0) ? total_ms : read_ms,
		   (e->call_type ==
		    0) ? (sub_total_t.tv_nsec %
			  NSEC_PER_MSEC) : (sub_read_t.tv_nsec % NSEC_PER_MSEC));
#else
	seq_printf(m,
		   "%d: %s from %d:%d to %d:%d node %d handle %d size %d:%d\n",
		   e->debug_id, (e->call_type == 2) ? "reply" :
		   ((e->call_type == 1) ? "async" : "call "), e->from_proc,
		   e->from_thread, e->to_proc, e->to_thread, e->to_node,
		   e->target_handle, e->data_size, e->offsets_size);
#endif
}

#ifdef BINDER_MONITOR
static void log_resume_func(struct work_struct *w)
{
	pr_debug("transaction log is self resumed\n");
	log_disable = 0;
}

static DECLARE_DELAYED_WORK(log_resume_work, log_resume_func);

static int binder_transaction_log_show(struct seq_file *m, void *unused)
{
	struct binder_transaction_log *log = m->private;
	int i;

	if (!log->entry)
		return 0;

	if (log->full) {
		for (i = log->next; i < log->size; i++)
			print_binder_transaction_log_entry(m, &log->entry[i]);
	}
	for (i = 0; i < log->next; i++)
		print_binder_transaction_log_entry(m, &log->entry[i]);

	if (log_disable & BINDER_LOG_RESUME) {
		pr_debug("%d (%s) read transaction log and resume\n", task_pid_nr(current), current->comm);
		cancel_delayed_work(&log_resume_work);
		log_disable = 0;
	}
	return 0;
}
#else

static int binder_transaction_log_show(struct seq_file *m, void *unused)
{
	struct binder_transaction_log *log = m->private;
	int i;

	if (log->full) {
		for (i = log->next; i < ARRAY_SIZE(log->entry); i++)
			print_binder_transaction_log_entry(m, &log->entry[i]);
	}
	for (i = 0; i < log->next; i++)
		print_binder_transaction_log_entry(m, &log->entry[i]);
	return 0;
}
#endif
=======
	int pid = (unsigned long)m->private;

	mutex_lock(&binder_procs_lock);
	hlist_for_each_entry(itr, &binder_procs, proc_node) {
		if (itr->pid == pid) {
			seq_puts(m, "binder proc state:\n");
			print_binder_proc(m, itr, 1);
		}
	}
	mutex_unlock(&binder_procs_lock);

	return 0;
}

static void print_binder_transaction_log_entry(struct seq_file *m,
					struct binder_transaction_log_entry *e)
{
	int debug_id = READ_ONCE(e->debug_id_done);
	/*
	 * read barrier to guarantee debug_id_done read before
	 * we print the log values
	 */
	smp_rmb();
	seq_printf(m,
		   "%d: %s from %d:%d to %d:%d context %s node %d handle %d size %d:%d ret %d/%d l=%d",
		   e->debug_id, (e->call_type == 2) ? "reply" :
		   ((e->call_type == 1) ? "async" : "call "), e->from_proc,
		   e->from_thread, e->to_proc, e->to_thread, e->context_name,
		   e->to_node, e->target_handle, e->data_size, e->offsets_size,
		   e->return_error, e->return_error_param,
		   e->return_error_line);
	/*
	 * read-barrier to guarantee read of debug_id_done after
	 * done printing the fields of the entry
	 */
	smp_rmb();
	seq_printf(m, debug_id && debug_id == READ_ONCE(e->debug_id_done) ?
			"\n" : " (incomplete)\n");
}

static int binder_transaction_log_show(struct seq_file *m, void *unused)
{
	struct binder_transaction_log *log = m->private;
	unsigned int log_cur = atomic_read(&log->cur);
	unsigned int count;
	unsigned int cur;
	int i;

	count = log_cur + 1;
	cur = count < ARRAY_SIZE(log->entry) && !log->full ?
		0 : count % ARRAY_SIZE(log->entry);
	if (count > ARRAY_SIZE(log->entry) || log->full)
		count = ARRAY_SIZE(log->entry);
	for (i = 0; i < count; i++) {
		unsigned int index = cur++ % ARRAY_SIZE(log->entry);

		print_binder_transaction_log_entry(m, &log->entry[index]);
	}
	return 0;
}
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

static const struct file_operations binder_fops = {
	.owner = THIS_MODULE,
	.poll = binder_poll,
	.unlocked_ioctl = binder_ioctl,
	.compat_ioctl = binder_ioctl,
	.mmap = binder_mmap,
	.open = binder_open,
	.flush = binder_flush,
	.release = binder_release,
};

<<<<<<< HEAD
static struct miscdevice binder_miscdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "binder",
	.fops = &binder_fops
};

#ifdef BINDER_MONITOR
static int binder_log_level_show(struct seq_file *m, void *unused)
{
	seq_printf(m, " Current log level: %lu\n", binder_log_level);
	return 0;
}

static ssize_t binder_log_level_write(struct file *filp, const char *ubuf,
				      size_t cnt, loff_t *data)
{
	char buf[32];
	size_t copy_size = cnt;
	unsigned long val;
	int ret;

	if (cnt >= sizeof(buf))
		copy_size = 32 - 1;
	buf[copy_size] = '\0';

	if (copy_from_user(&buf, ubuf, copy_size))
		return -EFAULT;

	pr_debug("[Binder] Set binder log level:%lu -> ", binder_log_level);
	ret = kstrtoul(buf, 10, &val);
	if (ret < 0) {
		pr_debug("Null\ninvalid string, need number foramt, err:%d\n", ret);
		pr_debug("Log Level:   0  ---- 4\n");
		pr_debug("           Less ---- More\n");
		return cnt;	/* string to unsined long fail */
	}
	pr_debug("%lu\n", val);
	if (val == 0) {
		binder_debug_mask =
		    BINDER_DEBUG_USER_ERROR | BINDER_DEBUG_FAILED_TRANSACTION |
		    BINDER_DEBUG_DEAD_TRANSACTION;
		binder_log_level = val;
	} else if (val == 1) {
		binder_debug_mask =
		    BINDER_DEBUG_USER_ERROR | BINDER_DEBUG_FAILED_TRANSACTION |
		    BINDER_DEBUG_DEAD_TRANSACTION | BINDER_DEBUG_DEAD_BINDER |
		    BINDER_DEBUG_DEATH_NOTIFICATION;
		binder_log_level = val;
	} else if (val == 2) {
		binder_debug_mask =
		    BINDER_DEBUG_USER_ERROR | BINDER_DEBUG_FAILED_TRANSACTION |
		    BINDER_DEBUG_DEAD_TRANSACTION | BINDER_DEBUG_DEAD_BINDER |
		    BINDER_DEBUG_DEATH_NOTIFICATION | BINDER_DEBUG_THREADS |
		    BINDER_DEBUG_TRANSACTION | BINDER_DEBUG_TRANSACTION_COMPLETE;
		binder_log_level = val;
	} else if (val == 3) {
		binder_debug_mask =
		    BINDER_DEBUG_USER_ERROR | BINDER_DEBUG_FAILED_TRANSACTION |
		    BINDER_DEBUG_DEAD_TRANSACTION | BINDER_DEBUG_DEAD_BINDER |
		    BINDER_DEBUG_DEATH_NOTIFICATION | BINDER_DEBUG_THREADS |
		    BINDER_DEBUG_TRANSACTION | BINDER_DEBUG_TRANSACTION_COMPLETE
		    | BINDER_DEBUG_OPEN_CLOSE | BINDER_DEBUG_READ_WRITE;
		binder_log_level = val;
	} else if (val == 4) {
		binder_debug_mask =
		    BINDER_DEBUG_USER_ERROR | BINDER_DEBUG_FAILED_TRANSACTION |
		    BINDER_DEBUG_DEAD_TRANSACTION | BINDER_DEBUG_DEAD_BINDER |
		    BINDER_DEBUG_DEATH_NOTIFICATION | BINDER_DEBUG_THREADS |
		    BINDER_DEBUG_OPEN_CLOSE | BINDER_DEBUG_READ_WRITE |
		    BINDER_DEBUG_TRANSACTION | BINDER_DEBUG_TRANSACTION_COMPLETE
		    | BINDER_DEBUG_USER_REFS | BINDER_DEBUG_INTERNAL_REFS |
		    BINDER_DEBUG_PRIORITY_CAP | BINDER_DEBUG_FREE_BUFFER |
		    BINDER_DEBUG_BUFFER_ALLOC;
		binder_log_level = val;
	} else {
		pr_debug("invalid value:%lu, should be 0 ~ 4\n", val);
	}
	return cnt;
}

static void print_binder_timeout_log_entry(struct seq_file *m, struct binder_timeout_log_entry *e)
{
	struct rtc_time tm;

	rtc_time_to_tm(e->tv.tv_sec, &tm);
	seq_printf(m, "%d:%s %d:%d to %d:%d spends %u000 ms (%s) dex_code %u ",
		   e->debug_id, binder_wait_on_str[e->r],
		   e->from_proc, e->from_thrd, e->to_proc, e->to_thrd,
		   e->over_sec, e->service, e->code);
	seq_printf(m, "start_at %lu.%03ld android %d-%02d-%02d %02d:%02d:%02d.%03lu\n",
		   (unsigned long)e->ts.tv_sec,
		   (e->ts.tv_nsec / NSEC_PER_MSEC),
		   (tm.tm_year + 1900), (tm.tm_mon + 1), tm.tm_mday,
		   tm.tm_hour, tm.tm_min, tm.tm_sec,
		   (unsigned long)(e->tv.tv_usec / USEC_PER_MSEC));
}

static int binder_timeout_log_show(struct seq_file *m, void *unused)
{
	struct binder_timeout_log *log = m->private;
	int i, latest;
	int end_idx = ARRAY_SIZE(log->entry) - 1;

	binder_lock(__func__);

	latest = log->next ? (log->next - 1) : end_idx;
	if (log->next == 0 && !log->full)
		goto timeout_log_show_unlock;

	if (latest >= ARRAY_SIZE(log->entry) || latest < 0) {
		int j;

		pr_alert("timeout log index error, log %p latest %d next %d end_idx %d\n",
			log, latest, log->next, end_idx);
		for (j = -4; j <= 3; j++) {
			unsigned int *tmp = (unsigned int *)log + (j * 8);

			pr_alert("0x%p %08x %08x %08x %08x %08x %08x %08x %08x\n",
				 tmp,
				 *tmp, *(tmp + 1), *(tmp + 2), *(tmp + 3),
				 *(tmp + 4), *(tmp + 5), *(tmp + 6), *(tmp + 7));
		}
#if defined(CONFIG_MTK_AEE_FEATURE)
		aee_kernel_warning_api(__FILE__, __LINE__,
				       DB_OPT_SWT_JBT_TRACES |
				       DB_OPT_BINDER_INFO,
				       "binder: timeout log index error",
				       "detect for memory corruption\n\n"
				       "check kernel log for more details\n");
#endif
		goto timeout_log_show_unlock;
	}

	for (i = latest; i >= 0; i--)
		print_binder_timeout_log_entry(m, &log->entry[i]);
	if (log->full) {
		for (i = end_idx; i > latest; i--)
			print_binder_timeout_log_entry(m, &log->entry[i]);
	}

timeout_log_show_unlock:
	binder_unlock(__func__);
	return 0;
}

BINDER_DEBUG_SETTING_ENTRY(log_level);
BINDER_DEBUG_ENTRY(timeout_log);

static int binder_transaction_log_enable_show(struct seq_file *m, void *unused)
{
#ifdef BINDER_MONITOR
	seq_printf(m, " Current transaciton log is %s %s %s"
#ifdef RT_PRIO_INHERIT
		   " %s"
#endif
		   "\n",
		   (log_disable & 0x1) ? "disabled" : "enabled",
		   (log_disable & BINDER_LOG_RESUME) ? "(self resume)" : "",
		   (log_disable & BINDER_BUF_WARN) ? "(buf warning enabled)" : ""
#ifdef RT_PRIO_INHERIT
		   , (log_disable & BINDER_RT_LOG_ENABLE) ? "(rt inherit log enabled)" : ""
#endif
	    );
#else
	seq_printf(m, " Current transaciton log is %s %s\n",
		   log_disable ? "disabled" : "enabled",
		   (log_disable & BINDER_LOG_RESUME) ? "(self resume)" : "");
#endif
	return 0;
}

static ssize_t binder_transaction_log_enable_write(struct file *filp,
						   const char *ubuf, size_t cnt, loff_t *data)
{
	char buf[32];
	size_t copy_size = cnt;
	unsigned long val;
	int ret;

	if (cnt >= sizeof(buf))
		copy_size = 32 - 1;

	buf[copy_size] = '\0';

	if (copy_from_user(&buf, ubuf, copy_size))
		return -EFAULT;

	ret = kstrtoul(buf, 10, &val);
	if (ret < 0) {
		pr_debug("failed to switch logging, " "need number format\n");
		return cnt;
	}

	log_disable = !(val & 0x1);
	if (log_disable && (val & BINDER_LOG_RESUME)) {
		log_disable |= BINDER_LOG_RESUME;
		queue_delayed_work(binder_deferred_workqueue, &log_resume_work, (120 * HZ));
	}
#ifdef BINDER_MONITOR
	if (val & BINDER_BUF_WARN)
		log_disable |= BINDER_BUF_WARN;

#ifdef RT_PRIO_INHERIT
	if (val & BINDER_RT_LOG_ENABLE)
		log_disable |= BINDER_RT_LOG_ENABLE;

#endif
	pr_debug("%d (%s) set transaction log %s %s %s"
#ifdef RT_PRIO_INHERIT
		 " %s"
#endif
		 "\n",
		 task_pid_nr(current), current->comm,
		 (log_disable & 0x1) ? "disabled" : "enabled",
		 (log_disable & BINDER_LOG_RESUME) ?
		 "(self resume)" : "", (log_disable & BINDER_BUF_WARN) ? "(buf warning)" : ""
#ifdef RT_PRIO_INHERIT
		 , (log_disable & BINDER_RT_LOG_ENABLE) ? "(rt inherit log enabled)" : ""
#endif
	    );
#else
	pr_debug("%d (%s) set transaction log %s %s\n",
		 task_pid_nr(current), current->comm,
		 log_disable ? "disabled" : "enabled",
		 (log_disable & BINDER_LOG_RESUME) ? "(self resume)" : "");
#endif
	return cnt;
}

BINDER_DEBUG_SETTING_ENTRY(transaction_log_enable);
#endif

#ifdef MTK_BINDER_PAGE_USED_RECORD
static int binder_page_used_show(struct seq_file *s, void *p)
{
	struct binder_proc *proc;
	int do_lock = !binder_debug_no_lock;

	seq_printf(s, "page_used:%d[%dMB]\npage_used_peak:%d[%dMB]\n",
		   binder_page_used, binder_page_used >> 8,
		   binder_page_used_peak, binder_page_used_peak >> 8);

	if (do_lock)
		binder_lock(__func__);
	seq_puts(s, "binder page stats by binder_proc:\n");
	hlist_for_each_entry(proc, &binder_procs, proc_node) {
		seq_printf(s,
			   "    proc %d(%s):page_used:%d[%dMB] page_used_peak:%d[%dMB]\n",
			   proc->pid, proc->tsk ? proc->tsk->comm : " ",
			   proc->page_used, proc->page_used >> 8,
			   proc->page_used_peak, proc->page_used_peak >> 8);
	}
	if (do_lock)
		binder_unlock(__func__);

	return 0;
}

BINDER_DEBUG_ENTRY(page_used);
#endif

BINDER_DEBUG_ENTRY(state);
BINDER_DEBUG_ENTRY(stats);
BINDER_DEBUG_ENTRY(transactions);
BINDER_DEBUG_ENTRY(transaction_log);

static int __init binder_init(void)
{
	int ret;
#ifdef BINDER_MONITOR
	struct task_struct *th;

	th = kthread_create(binder_bwdog_thread, NULL, "binder_watchdog");
	if (IS_ERR(th))
		pr_err("fail to create watchdog thread " "(err:%li)\n", PTR_ERR(th));
	else
		wake_up_process(th);

	binder_transaction_log_failed.entry = &entry_failed[0];
	binder_transaction_log_failed.size = ARRAY_SIZE(entry_failed);

#ifdef CONFIG_MTK_EXTMEM
	binder_transaction_log.entry =
	    extmem_malloc_page_align(sizeof(struct binder_transaction_log_entry)
				     * MAX_ENG_TRANS_LOG_BUFF_LEN);
	binder_transaction_log.size = MAX_ENG_TRANS_LOG_BUFF_LEN;

	if (binder_transaction_log.entry == NULL) {
		pr_err("%s[%s] ext emory alloc failed!!!\n", __FILE__, __func__);
		binder_transaction_log.entry =
		    vmalloc(sizeof(struct binder_transaction_log_entry) *
			    MAX_ENG_TRANS_LOG_BUFF_LEN);
	}
#else
	binder_transaction_log.entry = &entry_t[0];
	binder_transaction_log.size = ARRAY_SIZE(entry_t);
#endif
#endif

=======
BINDER_DEBUG_ENTRY(state);
BINDER_DEBUG_ENTRY(stats);
BINDER_DEBUG_ENTRY(transactions);
BINDER_DEBUG_ENTRY(transaction_log);

static int __init init_binder_device(const char *name)
{
	int ret;
	struct binder_device *binder_device;

	binder_device = kzalloc(sizeof(*binder_device), GFP_KERNEL);
	if (!binder_device)
		return -ENOMEM;

	binder_device->miscdev.fops = &binder_fops;
	binder_device->miscdev.minor = MISC_DYNAMIC_MINOR;
	binder_device->miscdev.name = name;

	binder_device->context.binder_context_mgr_uid = INVALID_UID;
	binder_device->context.name = name;
	mutex_init(&binder_device->context.context_mgr_node_lock);

	ret = misc_register(&binder_device->miscdev);
	if (ret < 0) {
		kfree(binder_device);
		return ret;
	}

	hlist_add_head(&binder_device->hlist, &binder_devices);

	return ret;
}

static int __init binder_init(void)
{
	int ret;
	char *device_name, *device_names;
	struct binder_device *device;
	struct hlist_node *tmp;

	binder_alloc_shrinker_init();

	atomic_set(&binder_transaction_log.cur, ~0U);
	atomic_set(&binder_transaction_log_failed.cur, ~0U);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	binder_deferred_workqueue = create_singlethread_workqueue("binder");
	if (!binder_deferred_workqueue)
		return -ENOMEM;

	binder_debugfs_dir_entry_root = debugfs_create_dir("binder", NULL);
	if (binder_debugfs_dir_entry_root)
		binder_debugfs_dir_entry_proc = debugfs_create_dir("proc",
<<<<<<< HEAD
								   binder_debugfs_dir_entry_root);
	ret = misc_register(&binder_miscdev);
	if (binder_debugfs_dir_entry_root) {
		debugfs_create_file("state",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root, NULL, &binder_state_fops);
		debugfs_create_file("stats",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root, NULL, &binder_stats_fops);
		debugfs_create_file("transactions",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root, NULL, &binder_transactions_fops);
		debugfs_create_file("transaction_log",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root,
				    &binder_transaction_log, &binder_transaction_log_fops);
		debugfs_create_file("failed_transaction_log",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root,
				    &binder_transaction_log_failed, &binder_transaction_log_fops);
#ifdef BINDER_MONITOR
		/* system_server is the main writer, remember to
		 * change group as "system" for write permission
		 * via related init.rc */
		debugfs_create_file("transaction_log_enable",
				    (S_IRUGO | S_IWUSR | S_IWGRP),
				    binder_debugfs_dir_entry_root,
				    NULL, &binder_transaction_log_enable_fops);
		debugfs_create_file("log_level",
				    (S_IRUGO | S_IWUSR | S_IWGRP),
				    binder_debugfs_dir_entry_root, NULL, &binder_log_level_fops);
		debugfs_create_file("timeout_log",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root,
				    &binder_timeout_log_t, &binder_timeout_log_fops);
#endif
#ifdef MTK_BINDER_PAGE_USED_RECORD
		debugfs_create_file("page_used",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root, NULL, &binder_page_used_fops);
#endif
	}
=======
						 binder_debugfs_dir_entry_root);

	if (binder_debugfs_dir_entry_root) {
		debugfs_create_file("state",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root,
				    NULL,
				    &binder_state_fops);
		debugfs_create_file("stats",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root,
				    NULL,
				    &binder_stats_fops);
		debugfs_create_file("transactions",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root,
				    NULL,
				    &binder_transactions_fops);
		debugfs_create_file("transaction_log",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root,
				    &binder_transaction_log,
				    &binder_transaction_log_fops);
		debugfs_create_file("failed_transaction_log",
				    S_IRUGO,
				    binder_debugfs_dir_entry_root,
				    &binder_transaction_log_failed,
				    &binder_transaction_log_fops);
	}

	/*
	 * Copy the module_parameter string, because we don't want to
	 * tokenize it in-place.
	 */
	device_names = kzalloc(strlen(binder_devices_param) + 1, GFP_KERNEL);
	if (!device_names) {
		ret = -ENOMEM;
		goto err_alloc_device_names_failed;
	}
	strcpy(device_names, binder_devices_param);

	while ((device_name = strsep(&device_names, ","))) {
		ret = init_binder_device(device_name);
		if (ret)
			goto err_init_binder_device_failed;
	}

	return ret;

err_init_binder_device_failed:
	hlist_for_each_entry_safe(device, tmp, &binder_devices, hlist) {
		misc_deregister(&device->miscdev);
		hlist_del(&device->hlist);
		kfree(device);
	}
err_alloc_device_names_failed:
	debugfs_remove_recursive(binder_debugfs_dir_entry_root);

	destroy_workqueue(binder_deferred_workqueue);

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	return ret;
}

device_initcall(binder_init);

#define CREATE_TRACE_POINTS
#include "binder_trace.h"

MODULE_LICENSE("GPL v2");
