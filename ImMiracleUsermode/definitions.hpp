#include <Windows.h>
#include <iostream>
#include <vector>

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