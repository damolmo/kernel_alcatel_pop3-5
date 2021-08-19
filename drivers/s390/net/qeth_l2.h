/*
 *    Copyright IBM Corp. 2013
 *    Author(s): Eugene Crosser <eugene.crosser@ru.ibm.com>
 */

#ifndef __QETH_L2_H__
#define __QETH_L2_H__

#include "qeth_core.h"

<<<<<<< HEAD
=======
extern const struct attribute_group *qeth_l2_attr_groups[];

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
int qeth_l2_create_device_attributes(struct device *);
void qeth_l2_remove_device_attributes(struct device *);
void qeth_l2_setup_bridgeport_attrs(struct qeth_card *card);

#endif /* __QETH_L2_H__ */
