<<<<<<< HEAD
=======
#include <linux/kernel.h>
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#include <linux/skbuff.h>
#include <linux/export.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/if_vlan.h>
#include <net/ip.h>
#include <net/ipv6.h>
#include <linux/igmp.h>
#include <linux/icmp.h>
#include <linux/sctp.h>
#include <linux/dccp.h>
#include <linux/if_tunnel.h>
#include <linux/if_pppox.h>
#include <linux/ppp_defs.h>
<<<<<<< HEAD
#include <net/flow_keys.h>
#include <scsi/fc/fc_fcoe.h>

/* copy saddr & daddr, possibly using 64bit load/store
 * Equivalent to :	flow->src = iph->saddr;
 *			flow->dst = iph->daddr;
 */
static void iph_to_flow_copy_addrs(struct flow_keys *flow, const struct iphdr *iph)
{
	BUILD_BUG_ON(offsetof(typeof(*flow), dst) !=
		     offsetof(typeof(*flow), src) + sizeof(flow->src));
	memcpy(&flow->src, &iph->saddr, sizeof(flow->src) + sizeof(flow->dst));
}
=======
#include <linux/stddef.h>
#include <net/flow_dissector.h>
#include <scsi/fc/fc_fcoe.h>

static bool dissector_uses_key(const struct flow_dissector *flow_dissector,
			       enum flow_dissector_key_id key_id)
{
	return flow_dissector->used_keys & (1 << key_id);
}

static void dissector_set_key(struct flow_dissector *flow_dissector,
			      enum flow_dissector_key_id key_id)
{
	flow_dissector->used_keys |= (1 << key_id);
}

static void *skb_flow_dissector_target(struct flow_dissector *flow_dissector,
				       enum flow_dissector_key_id key_id,
				       void *target_container)
{
	return ((char *) target_container) + flow_dissector->offset[key_id];
}

void skb_flow_dissector_init(struct flow_dissector *flow_dissector,
			     const struct flow_dissector_key *key,
			     unsigned int key_count)
{
	unsigned int i;

	memset(flow_dissector, 0, sizeof(*flow_dissector));

	for (i = 0; i < key_count; i++, key++) {
		/* User should make sure that every key target offset is withing
		 * boundaries of unsigned short.
		 */
		BUG_ON(key->offset > USHRT_MAX);
		BUG_ON(dissector_uses_key(flow_dissector,
					  key->key_id));

		dissector_set_key(flow_dissector, key->key_id);
		flow_dissector->offset[key->key_id] = key->offset;
	}

	/* Ensure that the dissector always includes control and basic key.
	 * That way we are able to avoid handling lack of these in fast path.
	 */
	BUG_ON(!dissector_uses_key(flow_dissector,
				   FLOW_DISSECTOR_KEY_CONTROL));
	BUG_ON(!dissector_uses_key(flow_dissector,
				   FLOW_DISSECTOR_KEY_BASIC));
}
EXPORT_SYMBOL(skb_flow_dissector_init);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

/**
 * __skb_flow_get_ports - extract the upper layer ports and return them
 * @skb: sk_buff to extract the ports from
 * @thoff: transport header offset
 * @ip_proto: protocol for which to get port offset
 * @data: raw buffer pointer to the packet, if NULL use skb->data
 * @hlen: packet header length, if @data is NULL use skb_headlen(skb)
 *
 * The function will try to retrieve the ports at offset thoff + poff where poff
 * is the protocol port offset returned from proto_ports_offset
 */
__be32 __skb_flow_get_ports(const struct sk_buff *skb, int thoff, u8 ip_proto,
			    void *data, int hlen)
{
	int poff = proto_ports_offset(ip_proto);

	if (!data) {
		data = skb->data;
		hlen = skb_headlen(skb);
	}

	if (poff >= 0) {
		__be32 *ports, _ports;

		ports = __skb_header_pointer(skb, thoff + poff,
					     sizeof(_ports), data, hlen, &_ports);
		if (ports)
			return *ports;
	}

	return 0;
}
EXPORT_SYMBOL(__skb_flow_get_ports);

/**
 * __skb_flow_dissect - extract the flow_keys struct and return it
 * @skb: sk_buff to extract the flow from, can be NULL if the rest are specified
<<<<<<< HEAD
=======
 * @flow_dissector: list of keys to dissect
 * @target_container: target structure to put dissected values into
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
 * @data: raw buffer pointer to the packet, if NULL use skb->data
 * @proto: protocol for which to get the flow, if @data is NULL use skb->protocol
 * @nhoff: network header offset, if @data is NULL use skb_network_offset(skb)
 * @hlen: packet header length, if @data is NULL use skb_headlen(skb)
 *
<<<<<<< HEAD
 * The function will try to retrieve the struct flow_keys from either the skbuff
 * or a raw buffer specified by the rest parameters
 */
bool __skb_flow_dissect(const struct sk_buff *skb, struct flow_keys *flow,
			void *data, __be16 proto, int nhoff, int hlen)
{
=======
 * The function will try to retrieve individual keys into target specified
 * by flow_dissector from either the skbuff or a raw buffer specified by the
 * rest parameters.
 *
 * Caller must take care of zeroing target container memory.
 */
bool __skb_flow_dissect(const struct sk_buff *skb,
			struct flow_dissector *flow_dissector,
			void *target_container,
			void *data, __be16 proto, int nhoff, int hlen)
{
	struct flow_dissector_key_control *key_control;
	struct flow_dissector_key_basic *key_basic;
	struct flow_dissector_key_addrs *key_addrs;
	struct flow_dissector_key_ports *key_ports;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	u8 ip_proto;

	if (!data) {
		data = skb->data;
		proto = skb->protocol;
		nhoff = skb_network_offset(skb);
		hlen = skb_headlen(skb);
	}

<<<<<<< HEAD
	memset(flow, 0, sizeof(*flow));
=======
	/* It is ensured by skb_flow_dissector_init() that control key will
	 * be always present.
	 */
	key_control = skb_flow_dissector_target(flow_dissector,
						FLOW_DISSECTOR_KEY_CONTROL,
						target_container);

	/* It is ensured by skb_flow_dissector_init() that basic key will
	 * be always present.
	 */
	key_basic = skb_flow_dissector_target(flow_dissector,
					      FLOW_DISSECTOR_KEY_BASIC,
					      target_container);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

again:
	switch (proto) {
	case htons(ETH_P_IP): {
		const struct iphdr *iph;
		struct iphdr _iph;
ip:
		iph = __skb_header_pointer(skb, nhoff, sizeof(_iph), data, hlen, &_iph);
		if (!iph || iph->ihl < 5)
			return false;
		nhoff += iph->ihl * 4;

		ip_proto = iph->protocol;
		if (ip_is_fragment(iph))
			ip_proto = 0;

<<<<<<< HEAD
		/* skip the address processing if skb is NULL.  The assumption
		 * here is that if there is no skb we are not looking for flow
		 * info but lengths and protocols.
		 */
		if (!skb)
			break;

		iph_to_flow_copy_addrs(flow, iph);
=======
		if (!dissector_uses_key(flow_dissector,
					FLOW_DISSECTOR_KEY_IPV4_ADDRS))
			break;
		key_addrs = skb_flow_dissector_target(flow_dissector,
						      FLOW_DISSECTOR_KEY_IPV4_ADDRS,
						      target_container);
		memcpy(key_addrs, &iph->saddr, sizeof(*key_addrs));
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		break;
	}
	case htons(ETH_P_IPV6): {
		const struct ipv6hdr *iph;
		struct ipv6hdr _iph;
		__be32 flow_label;

ipv6:
		iph = __skb_header_pointer(skb, nhoff, sizeof(_iph), data, hlen, &_iph);
		if (!iph)
			return false;

		ip_proto = iph->nexthdr;
		nhoff += sizeof(struct ipv6hdr);

<<<<<<< HEAD
		/* see comment above in IPv4 section */
		if (!skb)
			break;

		flow->src = (__force __be32)ipv6_addr_hash(&iph->saddr);
		flow->dst = (__force __be32)ipv6_addr_hash(&iph->daddr);
=======
		if (!dissector_uses_key(flow_dissector,
					FLOW_DISSECTOR_KEY_IPV6_HASH_ADDRS))
			break;
		key_addrs = skb_flow_dissector_target(flow_dissector,
						      FLOW_DISSECTOR_KEY_IPV6_HASH_ADDRS,
						      target_container);

		key_addrs->src = (__force __be32)ipv6_addr_hash(&iph->saddr);
		key_addrs->dst = (__force __be32)ipv6_addr_hash(&iph->daddr);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

		flow_label = ip6_flowlabel(iph);
		if (flow_label) {
			/* Awesome, IPv6 packet has a flow label so we can
			 * use that to represent the ports without any
			 * further dissection.
			 */
<<<<<<< HEAD
			flow->n_proto = proto;
			flow->ip_proto = ip_proto;
			flow->ports = flow_label;
			flow->thoff = (u16)nhoff;
=======

			key_basic->n_proto = proto;
			key_basic->ip_proto = ip_proto;
			key_control->thoff = (u16)nhoff;

			if (!dissector_uses_key(flow_dissector,
						FLOW_DISSECTOR_KEY_PORTS))
				break;
			key_ports = skb_flow_dissector_target(flow_dissector,
							      FLOW_DISSECTOR_KEY_PORTS,
							      target_container);
			key_ports->ports = flow_label;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

			return true;
		}

		break;
	}
	case htons(ETH_P_8021AD):
	case htons(ETH_P_8021Q): {
		const struct vlan_hdr *vlan;
		struct vlan_hdr _vlan;

		vlan = __skb_header_pointer(skb, nhoff, sizeof(_vlan), data, hlen, &_vlan);
		if (!vlan)
			return false;

		proto = vlan->h_vlan_encapsulated_proto;
		nhoff += sizeof(*vlan);
		goto again;
	}
	case htons(ETH_P_PPP_SES): {
		struct {
			struct pppoe_hdr hdr;
			__be16 proto;
		} *hdr, _hdr;
		hdr = __skb_header_pointer(skb, nhoff, sizeof(_hdr), data, hlen, &_hdr);
		if (!hdr)
			return false;
		proto = hdr->proto;
		nhoff += PPPOE_SES_HLEN;
		switch (proto) {
		case htons(PPP_IP):
			goto ip;
		case htons(PPP_IPV6):
			goto ipv6;
		default:
			return false;
		}
	}
	case htons(ETH_P_FCOE):
<<<<<<< HEAD
		flow->thoff = (u16)(nhoff + FCOE_HEADER_LEN);
=======
		key_control->thoff = (u16)(nhoff + FCOE_HEADER_LEN);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		/* fall through */
	default:
		return false;
	}

	switch (ip_proto) {
	case IPPROTO_GRE: {
		struct gre_hdr {
			__be16 flags;
			__be16 proto;
		} *hdr, _hdr;

		hdr = __skb_header_pointer(skb, nhoff, sizeof(_hdr), data, hlen, &_hdr);
		if (!hdr)
			return false;
		/*
		 * Only look inside GRE if version zero and no
		 * routing
		 */
		if (!(hdr->flags & (GRE_VERSION|GRE_ROUTING))) {
			proto = hdr->proto;
			nhoff += 4;
			if (hdr->flags & GRE_CSUM)
				nhoff += 4;
			if (hdr->flags & GRE_KEY)
				nhoff += 4;
			if (hdr->flags & GRE_SEQ)
				nhoff += 4;
			if (proto == htons(ETH_P_TEB)) {
				const struct ethhdr *eth;
				struct ethhdr _eth;

				eth = __skb_header_pointer(skb, nhoff,
							   sizeof(_eth),
							   data, hlen, &_eth);
				if (!eth)
					return false;
				proto = eth->h_proto;
				nhoff += sizeof(*eth);
			}
			goto again;
		}
		break;
	}
	case IPPROTO_IPIP:
		proto = htons(ETH_P_IP);
		goto ip;
	case IPPROTO_IPV6:
		proto = htons(ETH_P_IPV6);
		goto ipv6;
	default:
		break;
	}

<<<<<<< HEAD
	flow->n_proto = proto;
	flow->ip_proto = ip_proto;
	flow->thoff = (u16) nhoff;

	/* unless skb is set we don't need to record port info */
	if (skb)
		flow->ports = __skb_flow_get_ports(skb, nhoff, ip_proto,
						   data, hlen);
=======
	/* It is ensured by skb_flow_dissector_init() that basic key will
	 * be always present.
	 */
	key_basic = skb_flow_dissector_target(flow_dissector,
					      FLOW_DISSECTOR_KEY_BASIC,
					      target_container);
	key_basic->n_proto = proto;
	key_basic->ip_proto = ip_proto;
	key_control->thoff = (u16)nhoff;

	if (dissector_uses_key(flow_dissector,
			       FLOW_DISSECTOR_KEY_PORTS)) {
		key_ports = skb_flow_dissector_target(flow_dissector,
						      FLOW_DISSECTOR_KEY_PORTS,
						      target_container);
		key_ports->ports = __skb_flow_get_ports(skb, nhoff, ip_proto,
							data, hlen);
	}
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	return true;
}
EXPORT_SYMBOL(__skb_flow_dissect);

<<<<<<< HEAD
static u32 hashrnd __read_mostly;
=======
static siphash_key_t hashrnd __read_mostly;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
static __always_inline void __flow_hash_secret_init(void)
{
	net_get_random_once(&hashrnd, sizeof(hashrnd));
}

<<<<<<< HEAD
static __always_inline u32 __flow_hash_3words(u32 a, u32 b, u32 c)
{
	__flow_hash_secret_init();
	return jhash_3words(a, b, c, hashrnd);
}

static inline u32 __flow_hash_from_keys(struct flow_keys *keys)
=======
static const void *flow_keys_hash_start(const struct flow_keys *flow)
{
	BUILD_BUG_ON(FLOW_KEYS_HASH_OFFSET % SIPHASH_ALIGNMENT);
	return &flow->FLOW_KEYS_HASH_START_FIELD;
}

static inline size_t flow_keys_hash_length(const struct flow_keys *flow)
{
	BUILD_BUG_ON((sizeof(*flow) - FLOW_KEYS_HASH_OFFSET) % sizeof(u32));
	return (sizeof(*flow) - FLOW_KEYS_HASH_OFFSET) / sizeof(u32);
}

static inline u32 __flow_hash_from_keys(struct flow_keys *keys,
					const siphash_key_t *keyval)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	u32 hash;

	/* get a consistent hash (same value on both flow directions) */
<<<<<<< HEAD
	if (((__force u32)keys->dst < (__force u32)keys->src) ||
	    (((__force u32)keys->dst == (__force u32)keys->src) &&
	     ((__force u16)keys->port16[1] < (__force u16)keys->port16[0]))) {
		swap(keys->dst, keys->src);
		swap(keys->port16[0], keys->port16[1]);
	}

	hash = __flow_hash_3words((__force u32)keys->dst,
				  (__force u32)keys->src,
				  (__force u32)keys->ports);
=======
	if (((__force u32)keys->addrs.dst < (__force u32)keys->addrs.src) ||
	    (((__force u32)keys->addrs.dst == (__force u32)keys->addrs.src) &&
	     ((__force u16)keys->ports.port16[1] < (__force u16)keys->ports.port16[0]))) {
		swap(keys->addrs.dst, keys->addrs.src);
		swap(keys->ports.port16[0], keys->ports.port16[1]);
	}

	hash = siphash(flow_keys_hash_start(keys),
		       flow_keys_hash_length(keys), keyval);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	if (!hash)
		hash = 1;

	return hash;
}

u32 flow_hash_from_keys(struct flow_keys *keys)
{
<<<<<<< HEAD
	return __flow_hash_from_keys(keys);
}
EXPORT_SYMBOL(flow_hash_from_keys);

=======
	__flow_hash_secret_init();
	return __flow_hash_from_keys(keys, &hashrnd);
}
EXPORT_SYMBOL(flow_hash_from_keys);

static inline u32 ___skb_get_hash(const struct sk_buff *skb,
				  struct flow_keys *keys,
				  const siphash_key_t *keyval)
{
	if (!skb_flow_dissect_flow_keys(skb, keys))
		return 0;

	return __flow_hash_from_keys(keys, keyval);
}

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
/*
 * __skb_get_hash: calculate a flow hash based on src/dst addresses
 * and src/dst port numbers.  Sets hash in skb to non-zero hash value
 * on success, zero indicates no valid hash.  Also, sets l4_hash in skb
 * if hash is a canonical 4-tuple hash over transport ports.
 */
void __skb_get_hash(struct sk_buff *skb)
{
	struct flow_keys keys;
<<<<<<< HEAD

	if (!skb_flow_dissect(skb, &keys))
		return;

	if (keys.ports)
=======
	u32 hash;

	__flow_hash_secret_init();

	hash = ___skb_get_hash(skb, &keys, &hashrnd);
	if (!hash)
		return;

	if (keys.ports.ports)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		skb->l4_hash = 1;

	skb->sw_hash = 1;

<<<<<<< HEAD
	skb->hash = __flow_hash_from_keys(&keys);
}
EXPORT_SYMBOL(__skb_get_hash);

=======
	skb->hash = hash;
}
EXPORT_SYMBOL(__skb_get_hash);

__u32 skb_get_hash_perturb(const struct sk_buff *skb,
			   const siphash_key_t *perturb)
{
	struct flow_keys keys;

	return ___skb_get_hash(skb, &keys, perturb);
}
EXPORT_SYMBOL(skb_get_hash_perturb);

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
/*
 * Returns a Tx hash based on the given packet descriptor a Tx queues' number
 * to be used as a distribution range.
 */
u16 __skb_tx_hash(const struct net_device *dev, struct sk_buff *skb,
		  unsigned int num_tx_queues)
{
	u32 hash;
	u16 qoffset = 0;
	u16 qcount = num_tx_queues;

	if (skb_rx_queue_recorded(skb)) {
		hash = skb_get_rx_queue(skb);
		while (unlikely(hash >= num_tx_queues))
			hash -= num_tx_queues;
		return hash;
	}

	if (dev->num_tc) {
		u8 tc = netdev_get_prio_tc_map(dev, skb->priority);
		qoffset = dev->tc_to_txq[tc].offset;
		qcount = dev->tc_to_txq[tc].count;
	}

	return (u16) reciprocal_scale(skb_get_hash(skb), qcount) + qoffset;
}
EXPORT_SYMBOL(__skb_tx_hash);

u32 __skb_get_poff(const struct sk_buff *skb, void *data,
		   const struct flow_keys *keys, int hlen)
{
<<<<<<< HEAD
	u32 poff = keys->thoff;

	switch (keys->ip_proto) {
=======
	u32 poff = keys->control.thoff;

	switch (keys->basic.ip_proto) {
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	case IPPROTO_TCP: {
		/* access doff as u8 to avoid unaligned access */
		const u8 *doff;
		u8 _doff;

		doff = __skb_header_pointer(skb, poff + 12, sizeof(_doff),
					    data, hlen, &_doff);
		if (!doff)
			return poff;

		poff += max_t(u32, sizeof(struct tcphdr), (*doff & 0xF0) >> 2);
		break;
	}
	case IPPROTO_UDP:
	case IPPROTO_UDPLITE:
		poff += sizeof(struct udphdr);
		break;
	/* For the rest, we do not really care about header
	 * extensions at this point for now.
	 */
	case IPPROTO_ICMP:
		poff += sizeof(struct icmphdr);
		break;
	case IPPROTO_ICMPV6:
		poff += sizeof(struct icmp6hdr);
		break;
	case IPPROTO_IGMP:
		poff += sizeof(struct igmphdr);
		break;
	case IPPROTO_DCCP:
		poff += sizeof(struct dccp_hdr);
		break;
	case IPPROTO_SCTP:
		poff += sizeof(struct sctphdr);
		break;
	}

	return poff;
}

/* skb_get_poff() returns the offset to the payload as far as it could
 * be dissected. The main user is currently BPF, so that we can dynamically
 * truncate packets without needing to push actual payload to the user
 * space and can analyze headers only, instead.
 */
u32 skb_get_poff(const struct sk_buff *skb)
{
	struct flow_keys keys;

<<<<<<< HEAD
	if (!skb_flow_dissect(skb, &keys))
=======
	if (!skb_flow_dissect_flow_keys(skb, &keys))
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		return 0;

	return __skb_get_poff(skb, skb->data, &keys, skb_headlen(skb));
}

static inline int get_xps_queue(struct net_device *dev, struct sk_buff *skb)
{
#ifdef CONFIG_XPS
	struct xps_dev_maps *dev_maps;
	struct xps_map *map;
	int queue_index = -1;

	rcu_read_lock();
	dev_maps = rcu_dereference(dev->xps_maps);
	if (dev_maps) {
		map = rcu_dereference(
		    dev_maps->cpu_map[raw_smp_processor_id()]);
		if (map) {
			if (map->len == 1)
				queue_index = map->queues[0];
			else
				queue_index = map->queues[reciprocal_scale(skb_get_hash(skb),
									   map->len)];
			if (unlikely(queue_index >= dev->real_num_tx_queues))
				queue_index = -1;
		}
	}
	rcu_read_unlock();

	return queue_index;
#else
	return -1;
#endif
}

static u16 __netdev_pick_tx(struct net_device *dev, struct sk_buff *skb)
{
	struct sock *sk = skb->sk;
	int queue_index = sk_tx_queue_get(sk);

	if (queue_index < 0 || skb->ooo_okay ||
	    queue_index >= dev->real_num_tx_queues) {
		int new_index = get_xps_queue(dev, skb);
		if (new_index < 0)
			new_index = skb_tx_hash(dev, skb);

		if (queue_index != new_index && sk &&
		    rcu_access_pointer(sk->sk_dst_cache))
			sk_tx_queue_set(sk, new_index);

		queue_index = new_index;
	}

	return queue_index;
}

struct netdev_queue *netdev_pick_tx(struct net_device *dev,
				    struct sk_buff *skb,
				    void *accel_priv)
{
	int queue_index = 0;

	if (dev->real_num_tx_queues != 1) {
		const struct net_device_ops *ops = dev->netdev_ops;
		if (ops->ndo_select_queue)
			queue_index = ops->ndo_select_queue(dev, skb, accel_priv,
							    __netdev_pick_tx);
		else
			queue_index = __netdev_pick_tx(dev, skb);

		if (!accel_priv)
			queue_index = netdev_cap_txqueue(dev, queue_index);
	}

	skb_set_queue_mapping(skb, queue_index);
	return netdev_get_tx_queue(dev, queue_index);
}
<<<<<<< HEAD
=======

static const struct flow_dissector_key flow_keys_dissector_keys[] = {
	{
		.key_id = FLOW_DISSECTOR_KEY_CONTROL,
		.offset = offsetof(struct flow_keys, control),
	},
	{
		.key_id = FLOW_DISSECTOR_KEY_BASIC,
		.offset = offsetof(struct flow_keys, basic),
	},
	{
		.key_id = FLOW_DISSECTOR_KEY_IPV4_ADDRS,
		.offset = offsetof(struct flow_keys, addrs),
	},
	{
		.key_id = FLOW_DISSECTOR_KEY_IPV6_HASH_ADDRS,
		.offset = offsetof(struct flow_keys, addrs),
	},
	{
		.key_id = FLOW_DISSECTOR_KEY_PORTS,
		.offset = offsetof(struct flow_keys, ports),
	},
};

static const struct flow_dissector_key flow_keys_buf_dissector_keys[] = {
	{
		.key_id = FLOW_DISSECTOR_KEY_CONTROL,
		.offset = offsetof(struct flow_keys, control),
	},
	{
		.key_id = FLOW_DISSECTOR_KEY_BASIC,
		.offset = offsetof(struct flow_keys, basic),
	},
};

struct flow_dissector flow_keys_dissector __read_mostly;
EXPORT_SYMBOL(flow_keys_dissector);

struct flow_dissector flow_keys_buf_dissector __read_mostly;

static int __init init_default_flow_dissectors(void)
{
	skb_flow_dissector_init(&flow_keys_dissector,
				flow_keys_dissector_keys,
				ARRAY_SIZE(flow_keys_dissector_keys));
	skb_flow_dissector_init(&flow_keys_buf_dissector,
				flow_keys_buf_dissector_keys,
				ARRAY_SIZE(flow_keys_buf_dissector_keys));
	return 0;
}

core_initcall(init_default_flow_dissectors);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
