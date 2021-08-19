/*
 * Copyright (C) 2014 Sergey Senozhatsky.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _ZCOMP_H_
#define _ZCOMP_H_

#include <linux/mutex.h>

struct zcomp_strm {
	/* compression/decompression buffer */
	void *buffer;
	/*
	 * The private data of the compression stream, only compression
	 * stream backend can touch this (e.g. compression algorithm
	 * working memory)
	 */
	void *private;
	/* used in multi stream backend, protected by backend strm_lock */
	struct list_head list;
};

/* static compression backend */
<<<<<<< HEAD
#ifdef CONFIG_ZSM
struct zcomp_backend {

	int (*compress)(const unsigned char *src, unsigned char *dst,
			size_t *dst_len, void *private, int *checksum);
	int (*decompress)(const unsigned char *src, size_t src_len,
			unsigned char *dst);

	void *(*create)(void);
	void (*destroy)(void *private);

	const char *name;
};
#else
struct zcomp_backend {

	int (*compress)(const unsigned char *src, unsigned char *dst,
			size_t *dst_len, void *private);
=======
struct zcomp_backend {
	int (*compress)(const unsigned char *src, unsigned char *dst,
			size_t *dst_len, void *private);

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	int (*decompress)(const unsigned char *src, size_t src_len,
			unsigned char *dst);

	void *(*create)(void);
	void (*destroy)(void *private);

	const char *name;
};
<<<<<<< HEAD
#endif
=======

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
/* dynamic per-device compression frontend */
struct zcomp {
	void *stream;
	struct zcomp_backend *backend;

	struct zcomp_strm *(*strm_find)(struct zcomp *comp);
	void (*strm_release)(struct zcomp *comp, struct zcomp_strm *zstrm);
	bool (*set_max_streams)(struct zcomp *comp, int num_strm);
	void (*destroy)(struct zcomp *comp);
};

ssize_t zcomp_available_show(const char *comp, char *buf);

struct zcomp *zcomp_create(const char *comp, int max_strm);
void zcomp_destroy(struct zcomp *comp);

struct zcomp_strm *zcomp_strm_find(struct zcomp *comp);
void zcomp_strm_release(struct zcomp *comp, struct zcomp_strm *zstrm);
<<<<<<< HEAD
#ifdef CONFIG_ZSM
int zcomp_compress_zram(struct zcomp *comp, struct zcomp_strm *zstrm,
		const unsigned char *src, size_t *dst_len, int *checksum);
#else
int zcomp_compress(struct zcomp *comp, struct zcomp_strm *zstrm,
		const unsigned char *src, size_t *dst_len);
#endif
=======

int zcomp_compress(struct zcomp *comp, struct zcomp_strm *zstrm,
		const unsigned char *src, size_t *dst_len);

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
int zcomp_decompress(struct zcomp *comp, const unsigned char *src,
		size_t src_len, unsigned char *dst);

bool zcomp_set_max_streams(struct zcomp *comp, int num_strm);
#endif /* _ZCOMP_H_ */
