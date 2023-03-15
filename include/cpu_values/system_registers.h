#ifndef _CPU_SYSTEM_REGISTERS_H
#define _CPU_SYSTEM_REGISTERS_H

// System Control Register Values
#define SCTLR_RESERVED                  (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
#define SCTLR_EE_LITTLE_ENDIAN          (0 << 25)
#define SCTLR_I_CACHE_DISABLED          (0 << 12)
#define SCTLR_C_CACHE_DISABLED          (0 << 2)
#define SCTLR_MMU_DISABLED              (0 << 0)
#define SCTLR_MMU_ENABLED               (1 << 0)

#define SCTLR_VALUE_MMU_DISABLED	(SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED | SCTLR_C_CACHE_DISABLED | SCTLR_MMU_DISABLED)

// Architectural Feature Access Control Register VALUES
#define CPACR_FPEN_SIMD_ENABLED         (1 << 21) | (1 << 20) // enable SVE, floating-point and SIMD at EL1 - used by gcc
#define CPACR_SVE_ENABLED               (1 << 17) | (1 << 16)

#define CPACR_VALUE (CPACR_FPEN_SIMD_ENABLED | CPACR_SVE_ENABLED)

// Hypervisor Configuraion Register Values
#define HCR_RW_AARCH64	    (1 << 31)

#define HCR_VALUE			HCR_RW_AARCH64

// Saved Program Status Register Values
#define SPSR_MASK_ALL 		(7 << 6)

#define SPSR_M_EL0t         (0 << 0)
#define SPSR_M_EL1t         (4 << 0)
#define SPSR_M_EL1h			(5 << 0)
#define SPSR_M_EL2h			(9 << 0)

#define SPSR_VALUE_NO_EL    (SPSR_MASK_ALL)
#define SPSR_VALUE			(SPSR_MASK_ALL | SPSR_M_EL1t)

#endif // _CPU_SYSTEM_REGISTERS_H