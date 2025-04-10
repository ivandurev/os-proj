#ifndef _MMU_H
#define _MMU_H

// Values for MMU related registers

// first 4 bits of attribute
#define MAIR_DEVICE_MEM 		0 // device memory
#define MAIR_NORMAL_MEM_ONC 	4 // outer non-cacheable

// attributes in MAIR - type of memory
#define MAIR_ATTR_0_DEVICE_NGNRNE	((MAIR_DEVICE_MEM << 4) | 0)	 // non-gathering, non-reordering, non-early-write-acknowledging
#define MAIR_ATTR_1_NORMAL_ONC_INC  ((MAIR_NORMAL_MEM_ONC << 4) | 4) // outer + inner non-cacheable normal memory

// final value
#define MAIR_EL1_VALUE		(MAIR_ATTR_1_NORMAL_ONC_INC << 8 | MAIR_ATTR_0_DEVICE_NGNRNE << 0)

// translation control register
#define TCR_T0SZ			((64 - 48) << 0) // size offset for user
#define TCR_T1SZ			((64 - 48) << 16) // size offset for kernel
#define TCR_TG0_4KB			(0 << 14) // 4KB page size mode for user
#define TCR_TG1_4KB			(2 << 30) // 4KB page size mode for kernel
#define TCR_EL1_VALUE		(TCR_T0SZ | TCR_T1SZ | TCR_TG0_4KB | TCR_TG1_4KB)

#endif // _MMU_H
