#include "hook.h"

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING reg_path)
{

	UNREFERENCED_PARAMETER(reg_path);

	UNREFERENCED_PARAMETER(driver_object);

	miraclehook::call_kernel_function(&miraclehook::hook_handler);

	return STATUS_SUCCESS;
}
