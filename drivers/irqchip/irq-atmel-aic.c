/*
 * Atmel AT91 AIC (Advanced Interrupt Controller) driver
 *
 *  Copyright (C) 2004 SAN People
 *  Copyright (C) 2004 ATMEL
 *  Copyright (C) Rick Bronson
 *  Copyright (C) 2014 Free Electrons
 *
 *  Author: Boris BREZILLON <boris.brezillon@free-electrons.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/bitmap.h>
#include <linux/types.h>
#include <linux/irq.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/irqdomain.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/io.h>

#include <asm/exception.h>
#include <asm/mach/irq.h>

#include "irq-atmel-aic-common.h"
#include "irqchip.h"

/* Number of irq lines managed by AIC */
#define NR_AIC_IRQS	32

#define AT91_AIC_SMR(n)			((n) * 4)

#define AT91_AIC_SVR(n)			(0x80 + ((n) * 4))
#define AT91_AIC_IVR			0x100
#define AT91_AIC_FVR			0x104
#define AT91_AIC_ISR			0x108

#define AT91_AIC_IPR			0x10c
#define AT91_AIC_IMR			0x110
#define AT91_AIC_CISR			0x114

#define AT91_AIC_IECR			0x120
#define AT91_AIC_IDCR			0x124
#define AT91_AIC_ICCR			0x128
#define AT91_AIC_ISCR			0x12c
#define AT91_AIC_EOICR			0x130
#define AT91_AIC_SPU			0x134
#define AT91_AIC_DCR			0x138

static struct irq_domain *aic_domain;

static asmlinkage void __exception_irq_entry
aic_handle(struct pt_regs *regs)
{
	struct irq_domain_chip_generic *dgc = aic_domain->gc;
	struct irq_chip_generic *gc = dgc->gc[0];
	u32 irqnr;
	u32 irqstat;

<<<<<<< HEAD
	irqnr = irq_reg_readl(gc->reg_base + AT91_AIC_IVR);
	irqstat = irq_reg_readl(gc->reg_base + AT91_AIC_ISR);

	if (!irqstat)
		irq_reg_writel(0, gc->reg_base + AT91_AIC_EOICR);
=======
	irqnr = irq_reg_readl(gc, AT91_AIC_IVR);
	irqstat = irq_reg_readl(gc, AT91_AIC_ISR);

	if (!irqstat)
		irq_reg_writel(gc, 0, AT91_AIC_EOICR);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	else
		handle_domain_irq(aic_domain, irqnr, regs);
}

static int aic_retrigger(struct irq_data *d)
{
	struct irq_chip_generic *gc = irq_data_get_irq_chip_data(d);

	/* Enable interrupt on AIC5 */
	irq_gc_lock(gc);
<<<<<<< HEAD
	irq_reg_writel(d->mask, gc->reg_base + AT91_AIC_ISCR);
=======
	irq_reg_writel(gc, d->mask, AT91_AIC_ISCR);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	irq_gc_unlock(gc);

	return 0;
}

static int aic_set_type(struct irq_data *d, unsigned type)
{
	struct irq_chip_generic *gc = irq_data_get_irq_chip_data(d);
	unsigned int smr;
	int ret;

<<<<<<< HEAD
	smr = irq_reg_readl(gc->reg_base + AT91_AIC_SMR(d->hwirq));
=======
	smr = irq_reg_readl(gc, AT91_AIC_SMR(d->hwirq));
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	ret = aic_common_set_type(d, type, &smr);
	if (ret)
		return ret;

<<<<<<< HEAD
	irq_reg_writel(smr, gc->reg_base + AT91_AIC_SMR(d->hwirq));
=======
	irq_reg_writel(gc, smr, AT91_AIC_SMR(d->hwirq));
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	return 0;
}

#ifdef CONFIG_PM
static void aic_suspend(struct irq_data *d)
{
	struct irq_chip_generic *gc = irq_data_get_irq_chip_data(d);

	irq_gc_lock(gc);
<<<<<<< HEAD
	irq_reg_writel(gc->mask_cache, gc->reg_base + AT91_AIC_IDCR);
	irq_reg_writel(gc->wake_active, gc->reg_base + AT91_AIC_IECR);
=======
	irq_reg_writel(gc, gc->mask_cache, AT91_AIC_IDCR);
	irq_reg_writel(gc, gc->wake_active, AT91_AIC_IECR);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	irq_gc_unlock(gc);
}

static void aic_resume(struct irq_data *d)
{
	struct irq_chip_generic *gc = irq_data_get_irq_chip_data(d);

	irq_gc_lock(gc);
<<<<<<< HEAD
	irq_reg_writel(gc->wake_active, gc->reg_base + AT91_AIC_IDCR);
	irq_reg_writel(gc->mask_cache, gc->reg_base + AT91_AIC_IECR);
=======
	irq_reg_writel(gc, gc->wake_active, AT91_AIC_IDCR);
	irq_reg_writel(gc, gc->mask_cache, AT91_AIC_IECR);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	irq_gc_unlock(gc);
}

static void aic_pm_shutdown(struct irq_data *d)
{
	struct irq_chip_generic *gc = irq_data_get_irq_chip_data(d);

	irq_gc_lock(gc);
<<<<<<< HEAD
	irq_reg_writel(0xffffffff, gc->reg_base + AT91_AIC_IDCR);
	irq_reg_writel(0xffffffff, gc->reg_base + AT91_AIC_ICCR);
=======
	irq_reg_writel(gc, 0xffffffff, AT91_AIC_IDCR);
	irq_reg_writel(gc, 0xffffffff, AT91_AIC_ICCR);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	irq_gc_unlock(gc);
}
#else
#define aic_suspend		NULL
#define aic_resume		NULL
#define aic_pm_shutdown		NULL
#endif /* CONFIG_PM */

static void __init aic_hw_init(struct irq_domain *domain)
{
	struct irq_chip_generic *gc = irq_get_domain_generic_chip(domain, 0);
	int i;

	/*
	 * Perform 8 End Of Interrupt Command to make sure AIC
	 * will not Lock out nIRQ
	 */
	for (i = 0; i < 8; i++)
<<<<<<< HEAD
		irq_reg_writel(0, gc->reg_base + AT91_AIC_EOICR);
=======
		irq_reg_writel(gc, 0, AT91_AIC_EOICR);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	/*
	 * Spurious Interrupt ID in Spurious Vector Register.
	 * When there is no current interrupt, the IRQ Vector Register
	 * reads the value stored in AIC_SPU
	 */
<<<<<<< HEAD
	irq_reg_writel(0xffffffff, gc->reg_base + AT91_AIC_SPU);

	/* No debugging in AIC: Debug (Protect) Control Register */
	irq_reg_writel(0, gc->reg_base + AT91_AIC_DCR);

	/* Disable and clear all interrupts initially */
	irq_reg_writel(0xffffffff, gc->reg_base + AT91_AIC_IDCR);
	irq_reg_writel(0xffffffff, gc->reg_base + AT91_AIC_ICCR);

	for (i = 0; i < 32; i++)
		irq_reg_writel(i, gc->reg_base + AT91_AIC_SVR(i));
=======
	irq_reg_writel(gc, 0xffffffff, AT91_AIC_SPU);

	/* No debugging in AIC: Debug (Protect) Control Register */
	irq_reg_writel(gc, 0, AT91_AIC_DCR);

	/* Disable and clear all interrupts initially */
	irq_reg_writel(gc, 0xffffffff, AT91_AIC_IDCR);
	irq_reg_writel(gc, 0xffffffff, AT91_AIC_ICCR);

	for (i = 0; i < 32; i++)
		irq_reg_writel(gc, i, AT91_AIC_SVR(i));
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
}

static int aic_irq_domain_xlate(struct irq_domain *d,
				struct device_node *ctrlr,
				const u32 *intspec, unsigned int intsize,
				irq_hw_number_t *out_hwirq,
				unsigned int *out_type)
{
	struct irq_domain_chip_generic *dgc = d->gc;
	struct irq_chip_generic *gc;
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	unsigned smr;
	int idx;
	int ret;

	if (!dgc)
		return -EINVAL;

	ret = aic_common_irq_domain_xlate(d, ctrlr, intspec, intsize,
					  out_hwirq, out_type);
	if (ret)
		return ret;

	idx = intspec[0] / dgc->irqs_per_chip;
	if (idx >= dgc->num_chips)
		return -EINVAL;

	gc = dgc->gc[idx];

<<<<<<< HEAD
	irq_gc_lock(gc);
	smr = irq_reg_readl(gc->reg_base + AT91_AIC_SMR(*out_hwirq));
	ret = aic_common_set_priority(intspec[2], &smr);
	if (!ret)
		irq_reg_writel(smr, gc->reg_base + AT91_AIC_SMR(*out_hwirq));
	irq_gc_unlock(gc);
=======
	irq_gc_lock_irqsave(gc, flags);
	smr = irq_reg_readl(gc, AT91_AIC_SMR(*out_hwirq));
	ret = aic_common_set_priority(intspec[2], &smr);
	if (!ret)
		irq_reg_writel(gc, smr, AT91_AIC_SMR(*out_hwirq));
	irq_gc_unlock_irqrestore(gc, flags);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	return ret;
}

static const struct irq_domain_ops aic_irq_ops = {
	.map	= irq_map_generic_chip,
	.xlate	= aic_irq_domain_xlate,
};

static void __init at91sam9_aic_irq_fixup(struct device_node *root)
{
	aic_common_rtc_irq_fixup(root);
}

static const struct of_device_id __initdata aic_irq_fixups[] = {
	{ .compatible = "atmel,at91sam9g45", .data = at91sam9_aic_irq_fixup },
	{ .compatible = "atmel,at91sam9n12", .data = at91sam9_aic_irq_fixup },
	{ .compatible = "atmel,at91sam9rl", .data = at91sam9_aic_irq_fixup },
	{ .compatible = "atmel,at91sam9x5", .data = at91sam9_aic_irq_fixup },
	{ /* sentinel */ },
};

static int __init aic_of_init(struct device_node *node,
			      struct device_node *parent)
{
	struct irq_chip_generic *gc;
	struct irq_domain *domain;

	if (aic_domain)
		return -EEXIST;

	domain = aic_common_of_init(node, &aic_irq_ops, "atmel-aic",
				    NR_AIC_IRQS);
	if (IS_ERR(domain))
		return PTR_ERR(domain);

	aic_common_irq_fixup(aic_irq_fixups);

	aic_domain = domain;
	gc = irq_get_domain_generic_chip(domain, 0);

	gc->chip_types[0].regs.eoi = AT91_AIC_EOICR;
	gc->chip_types[0].regs.enable = AT91_AIC_IECR;
	gc->chip_types[0].regs.disable = AT91_AIC_IDCR;
	gc->chip_types[0].chip.irq_mask = irq_gc_mask_disable_reg;
	gc->chip_types[0].chip.irq_unmask = irq_gc_unmask_enable_reg;
	gc->chip_types[0].chip.irq_retrigger = aic_retrigger;
	gc->chip_types[0].chip.irq_set_type = aic_set_type;
	gc->chip_types[0].chip.irq_suspend = aic_suspend;
	gc->chip_types[0].chip.irq_resume = aic_resume;
	gc->chip_types[0].chip.irq_pm_shutdown = aic_pm_shutdown;

	aic_hw_init(domain);
	set_handle_irq(aic_handle);

	return 0;
}
IRQCHIP_DECLARE(at91rm9200_aic, "atmel,at91rm9200-aic", aic_of_init);
