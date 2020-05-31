/** @file
*
*  Copyright (c) 2011, ARM Limited. All rights reserved.
*  Copyright (c) 2020, Socionext Inc. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>

// The total number of descriptors, including the final "end-of-table" descriptor.
#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS 12

// DDR attributes
#define DDR_ATTRIBUTES_CACHED           ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
#define DDR_ATTRIBUTES_UNCACHED         ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED

/**
  Return the Virtual Memory Map of your platform

  This Virtual Memory Map is used by MemoryInitPei Module to initialize
   the MMU on your platform.

  @param[out]   VirtualMemoryMap    Array of ARM_MEMORY_REGION_DESCRIPTOR
                                    describing a Physical-to-Virtual Memory
                                    mapping. This array must be ended by a
                                    zero-filled entry.
**/
VOID
ArmPlatformGetVirtualMemoryMap (
  IN ARM_MEMORY_REGION_DESCRIPTOR** VirtualMemoryMap
  )
{
  ARM_MEMORY_REGION_ATTRIBUTES  CacheAttributes;
  ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable;
  UINTN Index = 0;

  ASSERT(VirtualMemoryMap != NULL);

  VirtualMemoryTable = (ARM_MEMORY_REGION_DESCRIPTOR*)
    AllocatePages (EFI_SIZE_TO_PAGES (sizeof(ARM_MEMORY_REGION_DESCRIPTOR)
                                      * MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS));
  if (VirtualMemoryTable == NULL) {
    return;
  }

  CacheAttributes = DDR_ATTRIBUTES_CACHED;

  // 0x2000_0000 - 0x2FFF_FFFF: PCI config space
  VirtualMemoryTable[Index].PhysicalBase   = 0x20000000UL;
  VirtualMemoryTable[Index].VirtualBase    = 0x20000000UL;
  VirtualMemoryTable[Index].Length         = 0x10000000UL;
  VirtualMemoryTable[Index].Attributes     = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  Index++;

  // 0x4000_0000 - 0x6FFF_FFFF: Mapped I/O space
  VirtualMemoryTable[Index].PhysicalBase   = 0x40000000UL;
  VirtualMemoryTable[Index].VirtualBase    = 0x40000000UL;
  VirtualMemoryTable[Index].Length         = 0x30000000UL;
  VirtualMemoryTable[Index].Attributes     = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  Index++;

  // 0x8000_0000 - 0x1_3FFF_FFFF: DDR
  VirtualMemoryTable[Index].PhysicalBase   = 0x80000000UL;
  VirtualMemoryTable[Index].VirtualBase    = 0x80000000UL;
  VirtualMemoryTable[Index].Length         = 0xC0000000UL;
  VirtualMemoryTable[Index].Attributes     = CacheAttributes;
  Index++;

  // End of Table
  VirtualMemoryTable[Index].PhysicalBase   = 0;
  VirtualMemoryTable[Index].VirtualBase    = 0;
  VirtualMemoryTable[Index].Length         = 0;
  VirtualMemoryTable[Index].Attributes     = (ARM_MEMORY_REGION_ATTRIBUTES)0;

  ASSERT((Index + 1) <= MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);

  *VirtualMemoryMap = VirtualMemoryTable;

}
