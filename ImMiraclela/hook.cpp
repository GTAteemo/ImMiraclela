#include "hook.h"
#include <wingdi.h>

GdiSelectBrush_t GdiSelectBrush = NULL;
PatBlt_t NtGdiPatBlt = NULL;
NtUserGetDC_t NtUserGetDC = NULL;
NtGdiCreateSolidBrush_t NtGdiCreateSolidBrush = NULL;
ReleaseDC_t NtUserReleaseDC = NULL;
DeleteObjectApp_t NtGdiDeleteObjectApp = NULL;
NtGdiExtTextOutW_t NtGdiExtTextOutW = NULL;
NtGdiHfontCreate_t NtGdiHfontCreate = NULL;
NtGdiSelectFont_t NtGdiSelectFont = NULL;


bool miraclehook::call_kernel_function(void* kernel_function_address)
{
	if (!kernel_function_address)
		return false;
	//C:\\Windows\\System32\\drivers\\dxgkrnl.sys
	//\\SystemRoot\\System32\\drivers\\dxgkrnl.sys
	void** function = reinterpret_cast<void**>(get_system_module_export("\\SystemRoot\\System32\\drivers\\dxgkrnl.sys",
		"NtDxgkGetTrackedWorkloadStatistics"));

	if (!function)
		return false;

	unsigned char orig[] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

	unsigned char shell_code[] = { 0x48,0xB8 }; //mov rax, xxx
	unsigned char shell_code_end[] = { 0xFF,0xE0 }; //jmp rax

	//pop rax and push rax not dectec
	//BYTE shell_code[] = { 0x48,0xB8 }; //mov rax, xxx
	//BYTE shell_code_end[] = { 0xFF,0xE0 }; //jmp rax

	RtlSecureZeroMemory(&orig, sizeof(orig));
	memcpy((void*)((ULONG_PTR)orig), &shell_code, sizeof(shell_code));
	uintptr_t hook_address = reinterpret_cast<uintptr_t>(kernel_function_address);
	memcpy((void*)((ULONG_PTR)orig + sizeof(shell_code)), &hook_address, sizeof(void*));
	memcpy((void*)((ULONG_PTR)orig + sizeof(shell_code) + sizeof(void*)), &shell_code_end, sizeof(shell_code_end));

	write_to_read_only_memory(function, &orig, sizeof(orig));

	GdiSelectBrush = (GdiSelectBrush_t)get_system_module_export(L"win32kfull.sys", "NtGdiSelectBrush");
	NtGdiCreateSolidBrush = (NtGdiCreateSolidBrush_t)get_system_module_export(L"win32kfull.sys", "NtGdiCreateSolidBrush");
	NtGdiPatBlt = (PatBlt_t)get_system_module_export(L"win32kfull.sys", "NtGdiPatBlt");
	NtUserGetDC = (NtUserGetDC_t)get_system_module_export(L"win32kbase.sys", "NtUserGetDC");
	NtUserReleaseDC = (ReleaseDC_t)get_system_module_export(L"win32kbase.sys", "NtUserReleaseDC");
	NtGdiDeleteObjectApp = (DeleteObjectApp_t)get_system_module_export(L"win32kbase.sys", "NtGdiDeleteObjectApp");

	//FONT SYScall
	NtGdiExtTextOutW = (NtGdiExtTextOutW_t)get_system_module_export(L"win32kfull.sys", "NtGdiExtTextOutW");
	NtGdiHfontCreate = (NtGdiHfontCreate_t)get_system_module_export(L"win32kfull.sys", "hfontCreate");
	NtGdiSelectFont = (NtGdiSelectFont_t)get_system_module_export(L"win32kfull.sys", "NtGdiSelectFont");

	NtUserGetDC = (NtUserGetDC_t)get_system_module_export(L"win32kbase.sys", "NtUserGetDC");
	NtUserReleaseDC = (ReleaseDC_t)get_system_module_export(L"win32kbase.sys", "NtUserReleaseDC");
	NtGdiDeleteObjectApp = (DeleteObjectApp_t)get_system_module_export(L"win32kbase.sys", "NtGdiDeleteObjectApp");

	return true;
}

NTSTATUS miraclehook::hook_handler(void* called_param)
{
	INFO_STRUCT* instructions = (INFO_STRUCT*)called_param;

	if (instructions->req_base == TRUE)
	{
		ANSI_STRING AS;
		UNICODE_STRING ModuleName;

		RtlInitAnsiString(&AS, instructions->module_name);
		RtlAnsiStringToUnicodeString(&ModuleName, &AS, TRUE);

		PEPROCESS process;
		PsLookupProcessByProcessId((HANDLE)instructions->pid, &process);

		KAPC_STATE apc;
		ULONG64 base_address64 = NULL;
		base_address64 = get_module_base_x64(process, ModuleName);
		instructions->base_address = base_address64;
		RtlFreeUnicodeString(&ModuleName);
	}

	else if (instructions->write == TRUE)
	{
		if (instructions->address < 0x7FFFFFFFFFFF && instructions->address > 0)
		{
			PVOID kernelBuff = ExAllocatePool(NonPagedPool, instructions->size);

			if (!kernelBuff)
			{
				return STATUS_UNSUCCESSFUL;
			}

			if (!memcpy(kernelBuff, instructions->buffer_address, instructions->size))
			{
				return STATUS_UNSUCCESSFUL;
			}

			PEPROCESS process;
			PsLookupProcessByProcessId((HANDLE)instructions->pid, &process);
			write_kernel_memory((HANDLE)instructions->pid, instructions->address, kernelBuff, instructions->size);
			ExFreePool(kernelBuff);
		}
	}

	else if (instructions->read == TRUE)
	{
		if (instructions->address < 0x7FFFFFFFFFFF && instructions->address>0)
		{
			read_kernel_memory((HANDLE)instructions->pid, instructions->address, instructions->output, instructions->size);
		}
	}

	else if (instructions->draw_box == TRUE)
	{
		HDC hdc = NtUserGetDC(NULL);
		if (!hdc)
			return STATUS_UNSUCCESSFUL;

		HBRUSH brush = NtGdiCreateSolidBrush(RGB(instructions->r, instructions->g, instructions->b), NULL);
		if (!brush)
			return STATUS_UNSUCCESSFUL;

		RECT rect = { instructions->x, instructions->y, instructions->x + instructions->w, instructions->y + instructions->h };
		FrameRect(hdc, &rect, brush, instructions->t);
		NtUserReleaseDC(hdc);
		NtGdiDeleteObjectApp(brush);
	}

	else if (instructions->draw_text == TRUE)
	{
		HDC hdc = NtUserGetDC(NULL);
		if (!hdc)
			return STATUS_UNSUCCESSFUL;

		const char* text = instructions->text;

		ANSI_STRING StringA;
		UNICODE_STRING StringU;
		BOOL ret;

		RtlInitAnsiString(&StringA, (LPSTR)text);
		RtlAnsiStringToUnicodeString(&StringU, &StringA, TRUE);

		LPWSTR aStr = StringU.Buffer;


		UINT len = wcslen(aStr);

		//font test
		HFONT font = CreateFontW(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, CHINESEBIG5_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, L"Arial");//L"Arial"
		HFONT oldfont = NtGdiSelectFont(hdc, font);
		bool textSucess = extTextOutW(hdc, instructions->x, instructions->y, 0, NULL, aStr, len, 0);
		NtGdiSelectFont(hdc, oldfont);
		bool deleted2 = NtGdiDeleteObjectApp(font);
		NtUserReleaseDC(hdc);
	}

	return STATUS_SUCCESS;
}

INT miraclehook::FrameRect(HDC hDC, CONST RECT* lprc, HBRUSH hbr, int thickness)
{
	HBRUSH oldbrush;
	RECT r = *lprc;

	if (!(oldbrush = GdiSelectBrush(hDC, hbr))) return 0;

	NtGdiPatBlt(hDC, r.left, r.top, thickness, r.bottom - r.top, PATCOPY);
	NtGdiPatBlt(hDC, r.right - thickness, r.top, thickness, r.bottom - r.top, PATCOPY);
	NtGdiPatBlt(hDC, r.left, r.top, r.right - r.left, thickness, PATCOPY);
	NtGdiPatBlt(hDC, r.left, r.bottom - thickness, r.right - r.left, thickness, PATCOPY);

	GdiSelectBrush(hDC, oldbrush);
	return TRUE;

}

//font²Ù×÷
HFONT miraclehook::CreateFontW
(
	int nHeight,
	int nWidth,
	int nEscapement,
	int nOrientation,
	int nWeight,
	DWORD   fnItalic,
	DWORD   fdwUnderline,
	DWORD   fdwStrikeOut,
	DWORD   fdwCharSet,
	DWORD   fdwOutputPrecision,
	DWORD   fdwClipPrecision,
	DWORD   fdwQuality,
	DWORD   fdwPitchAndFamily,
	LPCWSTR lpszFace) {
	LOGFONTW logfont;
	logfont.lfHeight = nHeight;
	logfont.lfWidth = nWidth;
	logfont.lfEscapement = nEscapement;
	logfont.lfOrientation = nOrientation;
	logfont.lfWeight = nWeight;
	logfont.lfItalic = (BYTE)fnItalic;
	logfont.lfUnderline = (BYTE)fdwUnderline;
	logfont.lfStrikeOut = (BYTE)fdwStrikeOut;
	logfont.lfCharSet = (BYTE)fdwCharSet;
	logfont.lfOutPrecision = (BYTE)fdwOutputPrecision;
	logfont.lfClipPrecision = (BYTE)fdwClipPrecision;
	logfont.lfQuality = (BYTE)fdwQuality;
	logfont.lfPitchAndFamily = (BYTE)fdwPitchAndFamily;
	if (NULL != lpszFace)
	{
		int Size = sizeof(logfont.lfFaceName) / sizeof(WCHAR);
		wcsncpy((wchar_t*)logfont.lfFaceName, lpszFace, Size - 1);
		/* Be 101% sure to have '\0' at end of string */
		logfont.lfFaceName[Size - 1] = '\0';
	}
	else
	{
		logfont.lfFaceName[0] = L'\0';
	}
	return miraclehook::CreateFontIndirectW(&logfont);
}

HFONT miraclehook::CreateFontIndirectW(CONST LOGFONTW* lplf)
{
	if (lplf)
	{
		ENUMLOGFONTEXDVW Logfont;

		RtlCopyMemory(&Logfont.elfEnumLogfontEx.elfLogFont, lplf, sizeof(LOGFONTW));
		// Need something other than just cleaning memory here.
		// Guess? Use caller data to determine the rest.
		RtlZeroMemory(&Logfont.elfEnumLogfontEx.elfFullName,
			sizeof(Logfont.elfEnumLogfontEx.elfFullName));
		RtlZeroMemory(&Logfont.elfEnumLogfontEx.elfStyle,
			sizeof(Logfont.elfEnumLogfontEx.elfStyle));
		RtlZeroMemory(&Logfont.elfEnumLogfontEx.elfScript,
			sizeof(Logfont.elfEnumLogfontEx.elfScript));

		Logfont.elfDesignVector.dvNumAxes = 0; // No more than MM_MAX_NUMAXES

		RtlZeroMemory(&Logfont.elfDesignVector, sizeof(DESIGNVECTOR));

		return miraclehook::CreateFontIndirectExW(&Logfont);
	}
	else {
		DbgPrintEx(0, 0, "[-] CreateFontIndirectW lplf is missing");
		return NULL;
	}
}

HFONT miraclehook::CreateFontIndirectExW(const ENUMLOGFONTEXDVW* elfexd)
{
	/* Msdn: Note, this function ignores the elfDesignVector member in
			 ENUMLOGFONTEXDV.
	 */
	if (elfexd)
	{
		return NtGdiHfontCreate((PENUMLOGFONTEXDVW)elfexd, 0, 0, 0, NULL);
	}
	else {
		DbgPrintEx(0, 0, "[-] CreateFontIndirectExW elfexd is missing");
		return NULL;
	}
}

BOOL miraclehook::extTextOutW(HDC hdc, INT x, INT y, UINT fuOptions, RECT* lprc, LPWSTR lpString, UINT cwc, INT* lpDx)
{
	BOOL		nRet = FALSE;
	PVOID       local_lpString = NULL;
	RECT* local_lprc = NULL;
	INT* local_lpDx = NULL;

	if (lprc != NULL)
	{
		SIZE_T Len = sizeof(RECT);
		local_lprc = (RECT*)AllocateVirtualMemory(Len);
		if (local_lprc != NULL)
		{
			__try
			{
				RtlZeroMemory(local_lprc, Len);
				RtlCopyMemory(local_lprc, lprc, Len);
			}
			__except (1)
			{
				DbgPrintEx(0, 0, "GDI.cpp Line RtlCopyMemory  Triggers An Error.ExtTextOutW Internal Function\r\n");
				goto $EXIT;
			}
		}
		else
		{
			DbgPrintEx(0, 0, "GDI.cpp Line local_lprc = null  Triggers An Error.ExtTextOutW Internal Function\r\n");
			goto $EXIT;
		}
	}

	if (cwc != 0)
	{
		SIZE_T     AllocSize = sizeof(WCHAR) * cwc + 1;
		local_lpString = AllocateVirtualMemory(AllocSize);

		if (local_lpString != NULL)
		{
			__try
			{
				RtlZeroMemory(local_lpString, AllocSize);
				RtlCopyMemory(local_lpString, lpString, AllocSize);
			}
			__except (1)
			{
				DbgPrintEx(0, 0, "[-] GDI.cpp Line RtlCopyMemory  Triggers An Error.ExtTextOutW Internal Function\r\n");
				goto $EXIT;
			}
		}
		else
		{
			DbgPrintEx(0, 0, "[-] GDI.cpp Line local_lpString = null  Triggers An Error.ExtTextOutW Internal Function\r\n");
			goto $EXIT;
		}
	}

	if (local_lpDx != NULL)
	{
		SIZE_T     AllocSize = sizeof(INT);
		local_lpDx = (INT*)AllocateVirtualMemory(AllocSize);
		if (local_lpDx != NULL)
		{
			__try
			{
				RtlZeroMemory(local_lpString, AllocSize);
				*local_lpDx = *lpDx;
			}
			__except (1)
			{
				DbgPrintEx(0, 0, "[-] GDI.cpp Line RtlCopyMemory  Triggers An Error.ExtTextOutW Internal Function\r\n");
				goto $EXIT;
			}
		}
		else
		{
			DbgPrintEx(0, 0, "[-] GDI.cpp Line local_lpDx = null  Triggers An Error.ExtTextOutW Internal Function\r\n");
		}
	}

	if (NtGdiExtTextOutW != NULL) {
		nRet = NtGdiExtTextOutW(hdc, x, y, fuOptions, local_lprc, (LPWSTR)local_lpString, cwc, local_lpDx, 0);
	}
	else {
		DbgPrintEx(0, 0, "[-] GDI.cpp Line NtGdiExtTextOutW = NULL Triggers An Error.TextOutW Internal Function\r\n");
	}
$EXIT:
	if (lprc != NULL)
	{
		FreeVirtualMemory(lprc, sizeof(RECT));
		lprc = NULL;
	}

	if (local_lpDx != NULL)
	{
		FreeVirtualMemory(local_lpDx, sizeof(INT));
		local_lpDx = NULL;
	}

	if (local_lpString != NULL)
	{
		FreeVirtualMemory(local_lpString, cwc);
		local_lpString = NULL;
	}

	return nRet;
}