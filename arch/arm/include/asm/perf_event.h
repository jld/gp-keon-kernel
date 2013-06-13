/*
 *  linux/arch/arm/include/asm/perf_event.h
 *
 *  Copyright (C) 2009 picoChip Designs Ltd, Jamie Iles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __ARM_PERF_EVENT_H__
#define __ARM_PERF_EVENT_H__

#include <asm/ptrace.h>

/* ARM performance counters start from 1 (in the cp15 accesses) so use the
 * same indexes here for consistency. */
#define PERF_EVENT_INDEX_OFFSET 1

/* ARM perf PMU IDs for use by internal perf clients. */
enum arm_perf_pmu_ids {
	ARM_PERF_PMU_ID_XSCALE1	= 0,
	ARM_PERF_PMU_ID_XSCALE2,
	ARM_PERF_PMU_ID_V6,
	ARM_PERF_PMU_ID_V6MP,
	ARM_PERF_PMU_ID_CA8,
	ARM_PERF_PMU_ID_CA9,
	ARM_PERF_PMU_ID_CA5,
	ARM_PERF_PMU_ID_CA15,
	ARM_PERF_PMU_ID_SCORPION,
	ARM_PERF_PMU_ID_SCORPIONMP,
	ARM_PERF_PMU_ID_KRAIT,
	ARM_NUM_PMU_IDS,
};

extern enum arm_perf_pmu_ids
armpmu_get_pmu_id(void);

extern int
armpmu_get_max_events(void);

/*
 * We shouldn't need fp unless CONFIG_FRAME_POINTER is set, but
 * conditionalizing this is probably more effort than it's worth.
 *
 * We can't actually get the caller's registers here; the saved PC and
 * SP values have to be consistent or else EHABI unwinding won't work,
 * and the only way to find the matching SP for the return address is
 * to unwind the current function.  So we save the current state
 * instead.
 *
 * Note that Thumb1 can't store high registers directly.
 */
#define perf_arch_fetch_caller_regs(regs, ip)			\
do {								\
	__u32 _cpsr, _sp, _fp, _lr, _pc;			\
	__asm__ __volatile__("mrs %[_cpsr], cpsr\n\t"		\
			     "mov %[_pc], pc\n\t"		\
			     "mov %[_lr], lr\n\t"		\
			     "mov %[_sp], sp\n\t"		\
			     "mov %[_fp], fp\n\t"		\
			     : [_cpsr] "=r" (_cpsr),		\
			       [_pc] "=r" (_pc),		\
			       [_lr] "=r" (_lr),		\
			       [_sp] "=r" (_sp),		\
			       [_fp] "=r" (_fp));		\
	(regs)->ARM_cpsr = _cpsr;				\
	(regs)->ARM_pc = _pc;					\
	(regs)->ARM_lr = _lr;					\
	(regs)->ARM_sp = _sp;					\
	(regs)->ARM_fp = _fp;					\
} while (0)

#endif /* __ARM_PERF_EVENT_H__ */
