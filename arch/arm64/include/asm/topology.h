#ifndef __ASM_TOPOLOGY_H
#define __ASM_TOPOLOGY_H

<<<<<<< HEAD
#ifdef CONFIG_SMP

=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#include <linux/cpumask.h>

struct cpu_topology {
	int thread_id;
	int core_id;
	int cluster_id;
<<<<<<< HEAD
	unsigned int partno;
=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	cpumask_t thread_sibling;
	cpumask_t core_sibling;
};

extern struct cpu_topology cpu_topology[NR_CPUS];
<<<<<<< HEAD
extern unsigned long arch_get_max_cpu_capacity(int);
=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

#define topology_physical_package_id(cpu)	(cpu_topology[cpu].cluster_id)
#define topology_core_id(cpu)		(cpu_topology[cpu].core_id)
#define topology_core_cpumask(cpu)	(&cpu_topology[cpu].core_sibling)
#define topology_thread_cpumask(cpu)	(&cpu_topology[cpu].thread_sibling)
<<<<<<< HEAD
#define topology_max_cpu_capacity(cpu)	(arch_get_max_cpu_capacity(cpu))
=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

void init_cpu_topology(void);
void store_cpu_topology(unsigned int cpuid);
const struct cpumask *cpu_coregroup_mask(int cpu);

<<<<<<< HEAD
/* Extras of CPU & Cluster functions */
extern int arch_cpu_is_big(unsigned int cpu);
extern int arch_cpu_is_little(unsigned int cpu);
extern int arch_is_multi_cluster(void);
extern int arch_is_smp(void);
extern int arch_get_nr_clusters(void);
extern int arch_get_cluster_id(unsigned int cpu);
extern void arch_get_cluster_cpus(struct cpumask *cpus, int cluster_id);
extern int arch_better_capacity(unsigned int cpu);

#else /* !CONFIG_ARM_CPU_TOPOLOGY */

static inline void init_cpu_topology(void) { }
static inline void store_cpu_topology(unsigned int cpuid) { }

static inline int arch_cpu_is_big(unsigned int cpu) { return 0; }
static inline int arch_cpu_is_little(unsigned int cpu) { return 1; }
static inline int arch_is_multi_cluster(void) { return 0; }
static inline int arch_is_smp(void) { return 1; }
static inline int arch_get_nr_clusters(void) { return 1; }
static inline int arch_get_cluster_id(unsigned int cpu) { return 0; }
static inline void arch_get_cluster_cpus(struct cpumask *cpus, int cluster_id)
{
	cpumask_clear(cpus);
	if (0 == cluster_id) {
		unsigned int cpu;

		for_each_possible_cpu(cpu)
			cpumask_set_cpu(cpu, cpus);
	}
}
static inline int arch_better_capacity(unsigned int cpu) { return 0; }

#endif /* CONFIG_ARM_CPU_TOPOLOGY */

#ifdef CONFIG_MTK_CPU_TOPOLOGY
void arch_build_cpu_topology_domain(void);
#endif
=======
struct sched_domain;
#ifdef CONFIG_CPU_FREQ
#define arch_scale_freq_capacity cpufreq_scale_freq_capacity
extern unsigned long cpufreq_scale_freq_capacity(struct sched_domain *sd, int cpu);
extern unsigned long cpufreq_scale_max_freq_capacity(int cpu);
#endif
#define arch_scale_cpu_capacity scale_cpu_capacity
extern unsigned long scale_cpu_capacity(struct sched_domain *sd, int cpu);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

#include <asm-generic/topology.h>

#endif /* _ASM_ARM_TOPOLOGY_H */
