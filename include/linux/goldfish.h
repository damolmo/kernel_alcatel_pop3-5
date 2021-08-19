#ifndef __LINUX_GOLDFISH_H
#define __LINUX_GOLDFISH_H

/* Helpers for Goldfish virtual platform */

<<<<<<< HEAD
static inline void gf_write64(unsigned long data,
		void __iomem *portl, void __iomem *porth)
{
	writel((u32)data, portl);
#ifdef CONFIG_64BIT
	writel(data>>32, porth);
#endif
}

=======
static inline void gf_write_ptr(const void *ptr, void __iomem *portl,
				void __iomem *porth)
{
	writel((u32)(unsigned long)ptr, portl);
#ifdef CONFIG_64BIT
	writel((unsigned long)ptr >> 32, porth);
#endif
}

static inline void gf_write_dma_addr(const dma_addr_t addr,
				     void __iomem *portl,
				     void __iomem *porth)
{
	writel((u32)addr, portl);
#ifdef CONFIG_ARCH_DMA_ADDR_T_64BIT
	writel(addr >> 32, porth);
#endif
}


>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#endif /* __LINUX_GOLDFISH_H */
