#include "stdafx.h"

void *__fastcall MemoryManager_Alloc(void *Thisptr, void *_EDX, uint32_t Size, uint32_t Alignment, bool Aligned)
{
	if (Size <= 0)
		Size = 1;

	//if (Aligned)
	//	return _aligned_malloc(Size, Alignment);

	return malloc(Size);
}

void __fastcall MemoryManager_Free(void *Thisptr, void *_EDX, void *Ptr, bool Aligned)
{
	if (!Ptr)
		return;

	free(Ptr);
}

void *ScrapHeap_Alloc(uint32_t Size)
{
	if (Size <= 0)
		Size = 1;

	return malloc(Size);
}

void ScrapHeap_Free(void *Ptr)
{
	if (Ptr)
		free(Ptr);
}

void PatchMemory(ULONG_PTR Address, PBYTE Data, SIZE_T Size)
{
	DWORD d = 0;
	VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &d);

	for (SIZE_T i = Address; i < (Address + Size); i++)
		*(volatile BYTE *)i = *Data++;

	VirtualProtect((LPVOID)Address, Size, d, &d);
	FlushInstructionCache(GetCurrentProcess(), (LPVOID)Address, Size);
}

void PatchJump(uintptr_t Address, uintptr_t Target)
{
	BYTE data[5];

	data[0] = 0xE9;
	*(int32_t *)&data[1] = (int32_t)(Target - Address) - 5;

	PatchMemory(Address, data, sizeof(data));
}