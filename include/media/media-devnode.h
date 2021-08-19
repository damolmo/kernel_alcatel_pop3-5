/*
 * Media device node
 *
 * Copyright (C) 2010 Nokia Corporation
 *
 * Contacts: Laurent Pinchart <laurent.pinchart@ideasonboard.com>
 *	     Sakari Ailus <sakari.ailus@iki.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * --
 *
 * Common functions for media-related drivers to register and unregister media
 * device nodes.
 */

#ifndef _MEDIA_DEVNODE_H
#define _MEDIA_DEVNODE_H

#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

<<<<<<< HEAD
=======
struct media_device;

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
/*
 * Flag to mark the media_devnode struct as registered. Drivers must not touch
 * this flag directly, it will be set and cleared by media_devnode_register and
 * media_devnode_unregister.
 */
#define MEDIA_FLAG_REGISTERED	0

struct media_file_operations {
	struct module *owner;
	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	unsigned int (*poll) (struct file *, struct poll_table_struct *);
	long (*ioctl) (struct file *, unsigned int, unsigned long);
	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
	int (*open) (struct file *);
	int (*release) (struct file *);
};

/**
 * struct media_devnode - Media device node
 * @parent:	parent device
 * @minor:	device node minor number
 * @flags:	flags, combination of the MEDIA_FLAG_* constants
 *
 * This structure represents a media-related device node.
 *
 * The @parent is a physical device. It must be set by core or device drivers
 * before registering the node.
 */
struct media_devnode {
<<<<<<< HEAD
=======
	struct media_device *media_dev;

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	/* device ops */
	const struct media_file_operations *fops;

	/* sysfs */
	struct device dev;		/* media device */
	struct cdev cdev;		/* character device */
	struct device *parent;		/* device parent */

	/* device info */
	int minor;
	unsigned long flags;		/* Use bitops to access flags */

	/* callbacks */
<<<<<<< HEAD
	void (*release)(struct media_devnode *mdev);
=======
	void (*release)(struct media_devnode *devnode);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
};

/* dev to media_devnode */
#define to_media_devnode(cd) container_of(cd, struct media_devnode, dev)

<<<<<<< HEAD
int __must_check media_devnode_register(struct media_devnode *mdev,
					struct module *owner);
void media_devnode_unregister(struct media_devnode *mdev);
=======
int __must_check media_devnode_register(struct media_device *mdev,
					struct media_devnode *devnode,
					struct module *owner);

/**
 * media_devnode_unregister_prepare - clear the media device node register bit
 * @devnode: the device node to prepare for unregister
 *
 * This clears the passed device register bit. Future open calls will be met
 * with errors. Should be called before media_devnode_unregister() to avoid
 * races with unregister and device file open calls.
 *
 * This function can safely be called if the device node has never been
 * registered or has already been unregistered.
 */
void media_devnode_unregister_prepare(struct media_devnode *devnode);

void media_devnode_unregister(struct media_devnode *devnode);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

static inline struct media_devnode *media_devnode_data(struct file *filp)
{
	return filp->private_data;
}

<<<<<<< HEAD
static inline int media_devnode_is_registered(struct media_devnode *mdev)
{
	return test_bit(MEDIA_FLAG_REGISTERED, &mdev->flags);
=======
static inline int media_devnode_is_registered(struct media_devnode *devnode)
{
	if (!devnode)
		return false;

	return test_bit(MEDIA_FLAG_REGISTERED, &devnode->flags);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
}

#endif /* _MEDIA_DEVNODE_H */
