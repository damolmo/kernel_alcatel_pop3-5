/*
 * L2TPv3 ethernet pseudowire driver
 *
 * Copyright (c) 2008,2009,2010 Katalix Systems Ltd
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/socket.h>
#include <linux/hash.h>
#include <linux/l2tp.h>
#include <linux/in.h>
#include <linux/etherdevice.h>
#include <linux/spinlock.h>
#include <net/sock.h>
#include <net/ip.h>
#include <net/icmp.h>
#include <net/udp.h>
#include <net/inet_common.h>
#include <net/inet_hashtables.h>
#include <net/tcp_states.h>
#include <net/protocol.h>
#include <net/xfrm.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>
<<<<<<< HEAD
=======
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/udp.h>
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

#include "l2tp_core.h"

/* Default device name. May be overridden by name specified by user */
#define L2TP_ETH_DEV_NAME	"l2tpeth%d"

/* via netdev_priv() */
struct l2tp_eth {
	struct net_device	*dev;
	struct sock		*tunnel_sock;
	struct l2tp_session	*session;
<<<<<<< HEAD
	struct list_head	list;
=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	atomic_long_t		tx_bytes;
	atomic_long_t		tx_packets;
	atomic_long_t		tx_dropped;
	atomic_long_t		rx_bytes;
	atomic_long_t		rx_packets;
	atomic_long_t		rx_errors;
};

/* via l2tp_session_priv() */
struct l2tp_eth_sess {
<<<<<<< HEAD
	struct net_device	*dev;
};

/* per-net private data for this module */
static unsigned int l2tp_eth_net_id;
struct l2tp_eth_net {
	struct list_head l2tp_eth_dev_list;
	spinlock_t l2tp_eth_lock;
};

static inline struct l2tp_eth_net *l2tp_eth_pernet(struct net *net)
{
	return net_generic(net, l2tp_eth_net_id);
}
=======
	struct net_device __rcu *dev;
};

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

static struct lock_class_key l2tp_eth_tx_busylock;
static int l2tp_eth_dev_init(struct net_device *dev)
{
	struct l2tp_eth *priv = netdev_priv(dev);

	priv->dev = dev;
	eth_hw_addr_random(dev);
	memset(&dev->broadcast[0], 0xff, 6);
	dev->qdisc_tx_busylock = &l2tp_eth_tx_busylock;
	return 0;
}

static void l2tp_eth_dev_uninit(struct net_device *dev)
{
	struct l2tp_eth *priv = netdev_priv(dev);
<<<<<<< HEAD
	struct l2tp_eth_net *pn = l2tp_eth_pernet(dev_net(dev));

	spin_lock(&pn->l2tp_eth_lock);
	list_del_init(&priv->list);
	spin_unlock(&pn->l2tp_eth_lock);
	dev_put(dev);
=======
	struct l2tp_eth_sess *spriv;

	spriv = l2tp_session_priv(priv->session);
	RCU_INIT_POINTER(spriv->dev, NULL);
	/* No need for synchronize_net() here. We're called by
	 * unregister_netdev*(), which does the synchronisation for us.
	 */
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
}

static int l2tp_eth_dev_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct l2tp_eth *priv = netdev_priv(dev);
	struct l2tp_session *session = priv->session;
	unsigned int len = skb->len;
	int ret = l2tp_xmit_skb(session, skb, session->hdr_len);

	if (likely(ret == NET_XMIT_SUCCESS)) {
		atomic_long_add(len, &priv->tx_bytes);
		atomic_long_inc(&priv->tx_packets);
	} else {
		atomic_long_inc(&priv->tx_dropped);
	}
	return NETDEV_TX_OK;
}

static struct rtnl_link_stats64 *l2tp_eth_get_stats64(struct net_device *dev,
						      struct rtnl_link_stats64 *stats)
{
	struct l2tp_eth *priv = netdev_priv(dev);

	stats->tx_bytes   = atomic_long_read(&priv->tx_bytes);
	stats->tx_packets = atomic_long_read(&priv->tx_packets);
	stats->tx_dropped = atomic_long_read(&priv->tx_dropped);
	stats->rx_bytes   = atomic_long_read(&priv->rx_bytes);
	stats->rx_packets = atomic_long_read(&priv->rx_packets);
	stats->rx_errors  = atomic_long_read(&priv->rx_errors);
	return stats;
}


static struct net_device_ops l2tp_eth_netdev_ops = {
	.ndo_init		= l2tp_eth_dev_init,
	.ndo_uninit		= l2tp_eth_dev_uninit,
	.ndo_start_xmit		= l2tp_eth_dev_xmit,
	.ndo_get_stats64	= l2tp_eth_get_stats64,
};

static void l2tp_eth_dev_setup(struct net_device *dev)
{
	ether_setup(dev);
	dev->priv_flags		&= ~IFF_TX_SKB_SHARING;
	dev->features		|= NETIF_F_LLTX;
	dev->netdev_ops		= &l2tp_eth_netdev_ops;
	dev->destructor		= free_netdev;
}

static void l2tp_eth_dev_recv(struct l2tp_session *session, struct sk_buff *skb, int data_len)
{
	struct l2tp_eth_sess *spriv = l2tp_session_priv(session);
<<<<<<< HEAD
	struct net_device *dev = spriv->dev;
	struct l2tp_eth *priv = netdev_priv(dev);
=======
	struct net_device *dev;
	struct l2tp_eth *priv;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	if (session->debug & L2TP_MSG_DATA) {
		unsigned int length;

		length = min(32u, skb->len);
		if (!pskb_may_pull(skb, length))
			goto error;

		pr_debug("%s: eth recv\n", session->name);
		print_hex_dump_bytes("", DUMP_PREFIX_OFFSET, skb->data, length);
	}

	if (!pskb_may_pull(skb, ETH_HLEN))
		goto error;

	secpath_reset(skb);

	/* checksums verified by L2TP */
	skb->ip_summed = CHECKSUM_NONE;

	skb_dst_drop(skb);
	nf_reset(skb);

<<<<<<< HEAD
=======
	rcu_read_lock();
	dev = rcu_dereference(spriv->dev);
	if (!dev)
		goto error_rcu;

	priv = netdev_priv(dev);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	if (dev_forward_skb(dev, skb) == NET_RX_SUCCESS) {
		atomic_long_inc(&priv->rx_packets);
		atomic_long_add(data_len, &priv->rx_bytes);
	} else {
		atomic_long_inc(&priv->rx_errors);
	}
<<<<<<< HEAD
	return;

error:
	atomic_long_inc(&priv->rx_errors);
=======
	rcu_read_unlock();

	return;

error_rcu:
	rcu_read_unlock();
error:
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	kfree_skb(skb);
}

static void l2tp_eth_delete(struct l2tp_session *session)
{
	struct l2tp_eth_sess *spriv;
	struct net_device *dev;

	if (session) {
		spriv = l2tp_session_priv(session);
<<<<<<< HEAD
		dev = spriv->dev;
		if (dev) {
			unregister_netdev(dev);
			spriv->dev = NULL;
			module_put(THIS_MODULE);
=======

		rtnl_lock();
		dev = rtnl_dereference(spriv->dev);
		if (dev) {
			unregister_netdevice(dev);
			rtnl_unlock();
			module_put(THIS_MODULE);
		} else {
			rtnl_unlock();
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		}
	}
}

#if defined(CONFIG_L2TP_DEBUGFS) || defined(CONFIG_L2TP_DEBUGFS_MODULE)
static void l2tp_eth_show(struct seq_file *m, void *arg)
{
	struct l2tp_session *session = arg;
	struct l2tp_eth_sess *spriv = l2tp_session_priv(session);
<<<<<<< HEAD
	struct net_device *dev = spriv->dev;

	seq_printf(m, "   interface %s\n", dev->name);
}
#endif

static int l2tp_eth_create(struct net *net, u32 tunnel_id, u32 session_id, u32 peer_session_id, struct l2tp_session_cfg *cfg)
{
	struct net_device *dev;
	char name[IFNAMSIZ];
	struct l2tp_tunnel *tunnel;
=======
	struct net_device *dev;

	rcu_read_lock();
	dev = rcu_dereference(spriv->dev);
	if (!dev) {
		rcu_read_unlock();
		return;
	}
	dev_hold(dev);
	rcu_read_unlock();

	seq_printf(m, "   interface %s\n", dev->name);

	dev_put(dev);
}
#endif

static void l2tp_eth_adjust_mtu(struct l2tp_tunnel *tunnel,
				struct l2tp_session *session,
				struct net_device *dev)
{
	unsigned int overhead = 0;
	struct dst_entry *dst;
	u32 l3_overhead = 0;

	/* if the encap is UDP, account for UDP header size */
	if (tunnel->encap == L2TP_ENCAPTYPE_UDP) {
		overhead += sizeof(struct udphdr);
		dev->needed_headroom += sizeof(struct udphdr);
	}
	if (session->mtu != 0) {
		dev->mtu = session->mtu;
		dev->needed_headroom += session->hdr_len;
		return;
	}
	lock_sock(tunnel->sock);
	l3_overhead = kernel_sock_ip_overhead(tunnel->sock);
	release_sock(tunnel->sock);
	if (l3_overhead == 0) {
		/* L3 Overhead couldn't be identified, this could be
		 * because tunnel->sock was NULL or the socket's
		 * address family was not IPv4 or IPv6,
		 * dev mtu stays at 1500.
		 */
		return;
	}
	/* Adjust MTU, factor overhead - underlay L3, overlay L2 hdr
	 * UDP overhead, if any, was already factored in above.
	 */
	overhead += session->hdr_len + ETH_HLEN + l3_overhead;

	/* If PMTU discovery was enabled, use discovered MTU on L2TP device */
	dst = sk_dst_get(tunnel->sock);
	if (dst) {
		/* dst_mtu will use PMTU if found, else fallback to intf MTU */
		u32 pmtu = dst_mtu(dst);

		if (pmtu != 0)
			dev->mtu = pmtu;
		dst_release(dst);
	}
	session->mtu = dev->mtu - overhead;
	dev->mtu = session->mtu;
	dev->needed_headroom += session->hdr_len;
}

static int l2tp_eth_create(struct net *net, struct l2tp_tunnel *tunnel,
			   u32 session_id, u32 peer_session_id,
			   struct l2tp_session_cfg *cfg)
{
	struct net_device *dev;
	char name[IFNAMSIZ];
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	struct l2tp_session *session;
	struct l2tp_eth *priv;
	struct l2tp_eth_sess *spriv;
	int rc;
<<<<<<< HEAD
	struct l2tp_eth_net *pn;

	tunnel = l2tp_tunnel_find(net, tunnel_id);
	if (!tunnel) {
		rc = -ENODEV;
		goto out;
	}

	session = l2tp_session_find(net, tunnel, session_id);
	if (session) {
		rc = -EEXIST;
		goto out;
	}
=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	if (cfg->ifname) {
		dev = dev_get_by_name(net, cfg->ifname);
		if (dev) {
			dev_put(dev);
			rc = -EEXIST;
<<<<<<< HEAD
			goto out;
=======
			goto err;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		}
		strlcpy(name, cfg->ifname, IFNAMSIZ);
	} else
		strcpy(name, L2TP_ETH_DEV_NAME);

	session = l2tp_session_create(sizeof(*spriv), tunnel, session_id,
				      peer_session_id, cfg);
<<<<<<< HEAD
	if (!session) {
		rc = -ENOMEM;
		goto out;
=======
	if (IS_ERR(session)) {
		rc = PTR_ERR(session);
		goto err;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	}

	dev = alloc_netdev(sizeof(*priv), name, NET_NAME_UNKNOWN,
			   l2tp_eth_dev_setup);
	if (!dev) {
		rc = -ENOMEM;
<<<<<<< HEAD
		goto out_del_session;
	}

	dev_net_set(dev, net);
	if (session->mtu == 0)
		session->mtu = dev->mtu - session->hdr_len;
	dev->mtu = session->mtu;
	dev->needed_headroom += session->hdr_len;
=======
		goto err_sess;
	}

	dev_net_set(dev, net);
	l2tp_eth_adjust_mtu(tunnel, session, dev);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	priv = netdev_priv(dev);
	priv->dev = dev;
	priv->session = session;
<<<<<<< HEAD
	INIT_LIST_HEAD(&priv->list);
=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	priv->tunnel_sock = tunnel->sock;
	session->recv_skb = l2tp_eth_dev_recv;
	session->session_close = l2tp_eth_delete;
#if defined(CONFIG_L2TP_DEBUGFS) || defined(CONFIG_L2TP_DEBUGFS_MODULE)
	session->show = l2tp_eth_show;
#endif

	spriv = l2tp_session_priv(session);
<<<<<<< HEAD
	spriv->dev = dev;

	rc = register_netdev(dev);
	if (rc < 0)
		goto out_del_dev;

	__module_get(THIS_MODULE);
	/* Must be done after register_netdev() */
	strlcpy(session->ifname, dev->name, IFNAMSIZ);

	dev_hold(dev);
	pn = l2tp_eth_pernet(dev_net(dev));
	spin_lock(&pn->l2tp_eth_lock);
	list_add(&priv->list, &pn->l2tp_eth_dev_list);
	spin_unlock(&pn->l2tp_eth_lock);

	return 0;

out_del_dev:
	free_netdev(dev);
	spriv->dev = NULL;
out_del_session:
	l2tp_session_delete(session);
out:
	return rc;
}

static __net_init int l2tp_eth_init_net(struct net *net)
{
	struct l2tp_eth_net *pn = net_generic(net, l2tp_eth_net_id);

	INIT_LIST_HEAD(&pn->l2tp_eth_dev_list);
	spin_lock_init(&pn->l2tp_eth_lock);

	return 0;
}

static struct pernet_operations l2tp_eth_net_ops = {
	.init = l2tp_eth_init_net,
	.id   = &l2tp_eth_net_id,
	.size = sizeof(struct l2tp_eth_net),
};
=======

	l2tp_session_inc_refcount(session);

	rtnl_lock();

	/* Register both device and session while holding the rtnl lock. This
	 * ensures that l2tp_eth_delete() will see that there's a device to
	 * unregister, even if it happened to run before we assign spriv->dev.
	 */
	rc = l2tp_session_register(session, tunnel);
	if (rc < 0) {
		rtnl_unlock();
		goto err_sess_dev;
	}

	rc = register_netdevice(dev);
	if (rc < 0) {
		rtnl_unlock();
		l2tp_session_delete(session);
		l2tp_session_dec_refcount(session);
		free_netdev(dev);

		return rc;
	}

	strlcpy(session->ifname, dev->name, IFNAMSIZ);
	rcu_assign_pointer(spriv->dev, dev);

	rtnl_unlock();

	l2tp_session_dec_refcount(session);

	__module_get(THIS_MODULE);

	return 0;

err_sess_dev:
	l2tp_session_dec_refcount(session);
	free_netdev(dev);
err_sess:
	kfree(session);
err:
	return rc;
}
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916


static const struct l2tp_nl_cmd_ops l2tp_eth_nl_cmd_ops = {
	.session_create	= l2tp_eth_create,
	.session_delete	= l2tp_session_delete,
};


static int __init l2tp_eth_init(void)
{
	int err = 0;

	err = l2tp_nl_register_ops(L2TP_PWTYPE_ETH, &l2tp_eth_nl_cmd_ops);
	if (err)
<<<<<<< HEAD
		goto out;

	err = register_pernet_device(&l2tp_eth_net_ops);
	if (err)
		goto out_unreg;
=======
		goto err;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	pr_info("L2TP ethernet pseudowire support (L2TPv3)\n");

	return 0;

<<<<<<< HEAD
out_unreg:
	l2tp_nl_unregister_ops(L2TP_PWTYPE_ETH);
out:
=======
err:
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	return err;
}

static void __exit l2tp_eth_exit(void)
{
<<<<<<< HEAD
	unregister_pernet_device(&l2tp_eth_net_ops);
=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	l2tp_nl_unregister_ops(L2TP_PWTYPE_ETH);
}

module_init(l2tp_eth_init);
module_exit(l2tp_eth_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("James Chapman <jchapman@katalix.com>");
MODULE_DESCRIPTION("L2TP ethernet pseudowire driver");
MODULE_VERSION("1.0");
