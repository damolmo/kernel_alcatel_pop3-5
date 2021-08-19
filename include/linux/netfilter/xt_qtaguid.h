#ifndef _XT_QTAGUID_MATCH_H
#define _XT_QTAGUID_MATCH_H

/* For now we just replace the xt_owner.
 * FIXME: make iptables aware of qtaguid. */
#include <linux/netfilter/xt_owner.h>

#define XT_QTAGUID_UID    XT_OWNER_UID
#define XT_QTAGUID_GID    XT_OWNER_GID
#define XT_QTAGUID_SOCKET XT_OWNER_SOCKET
#define xt_qtaguid_match_info xt_owner_match_info

<<<<<<< HEAD
=======
int qtaguid_untag(struct socket *sock, bool kernel);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#endif /* _XT_QTAGUID_MATCH_H */
