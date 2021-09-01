#include "definitions.h"

PVOID get_system_module_base(const char* module_name);
PVOID get_system_module_export(const char* module_name, LPCSTR routine_name);
bool write_memory(void* address, void* buffer, size_t size);
bool write_to_read_only_memory(void* address, void* buffer, size_t size);
ULONG64 get_module_base_x64(PEPROCESS proc, UNICODE_STRING module_name);
bool read_kernel_memory(HANDLE pid, uintptr_t address, void* buffer, SIZE_T size);
bool write_kernel_memory(HANDLE pid, uintptr_t address, void* buffer, SIZE_T size);
PVOID get_system_routine_address(PCWSTR routine_name);
PVOID get_system_module_export(LPCWSTR module_name, LPCSTR routine_name);

VOID FreeVirtualMemory(PVOID VirtualAddress, SIZE_T Size);

PVOID AllocateVirtualMemory(SIZE_T Size);

typedef struct _INFO_STRUCT
{
	void* buffer_address;
	UINT_PTR address;
	ULONGLONG size;
	ULONG pid;
	BOOLEAN write;
	BOOLEAN read;
	BOOLEAN req_base;
	BOOLEAN draw_box;
	int		r, g, b, x, y, w, h, t;
	void* output;
	const char* module_name;
	ULONG64 base_address;

	BOOLEAN draw_text;
	const char* text;

}INFO_STRUCT;
