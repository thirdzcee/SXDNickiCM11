/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/export.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <asm/user_accessible_timer.h>
#include <asm/traps.h>

#define USER_ACCESS_TIMER_OFFSET	0xf30
#define USER_ACCESS_FEATURE_OFFSET	0xf34
#define USER_ACCESS_FEATURE_FLAG	0xffff0f20

static struct vm_area_struct user_timers_vma;
static int __init user_timers_vma_init(void)
{
	user_timers_vma.vm_start        = CONFIG_ARM_USER_ACCESSIBLE_TIMER_BASE;
	user_timers_vma.vm_end          = CONFIG_ARM_USER_ACCESSIBLE_TIMER_BASE
						+ PAGE_SIZE;
	user_timers_vma.vm_page_prot    = PAGE_READONLY;
	user_timers_vma.vm_flags        = VM_READ | VM_MAYREAD;
	return 0;
}
arch_initcall(user_timers_vma_init);

void setup_user_timer_offset(unsigned long addr)
{
#if defined(CONFIG_CPU_USE_DOMAINS)
	unsigned long vectors = CONFIG_VECTORS_BASE;
#else
	unsigned long vectors = (unsigned long)vectors_page;
#endif
	unsigned long *timer_offset = (unsigned long *)(vectors +
		USER_ACCESS_TIMER_OFFSET);
	*timer_offset = addr;
}
EXPORT_SYMBOL(setup_user_timer_offset);

void set_user_accessible_timer_flag(bool flag)
{
#if defined(CONFIG_CPU_USE_DOMAINS)
	unsigned long vectors = CONFIG_VECTORS_BASE;
#else
	unsigned long vectors = (unsigned long)vectors_page;
#endif
	unsigned long *timer_offset = (unsigned long *)(vectors +
		USER_ACCESS_FEATURE_OFFSET);
	*timer_offset = (flag ? USER_ACCESS_FEATURE_FLAG : 0);
}
EXPORT_SYMBOL(set_user_accessible_timer_flag);
