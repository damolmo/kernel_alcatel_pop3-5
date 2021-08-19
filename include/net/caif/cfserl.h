/*
 * Copyright (C) ST-Ericsson AB 2010
 * Author:	Sjur Brendeland
 * License terms: GNU General Public License (GPL) version 2
 */

#ifndef CFSERL_H_
#define CFSERL_H_
#include <net/caif/caif_layer.h>

struct cflayer *cfserl_create(int instance, bool use_stx);
<<<<<<< HEAD
=======
void cfserl_release(struct cflayer *layer);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#endif
