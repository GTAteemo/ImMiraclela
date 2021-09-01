#include "memory.h"
#include <wingdi.h>

typedef HBRUSH(*GdiSelectBrush_t)(
	_In_ HDC 	hdc,
	_In_ HBRUSH 	hbr
	);

typedef BOOL(*PatBlt_t)(
	_In_ 	HDC,
	_In_ int x,
	_In_ int y,
	_In_ int w,
	_In_ int h,
	_In_ 	DWORD
	);

typedef HDC(*NtUserGetDC_t)(HWND hWnd);

typedef HBRUSH(*NtGdiCreateSolidBrush_t)(
	_In_ COLORREF 	crColor,
	_In_opt_ HBRUSH 	hbr
	);

typedef int (*ReleaseDC_t)(HDC hdc);
typedef BOOL(*DeleteObjectApp_t)(HANDLE hobj);

typedef BOOL(*NtGdiExtTextOutW_t)
(
	_In_ HDC 	hdc,
	_In_ INT 	x,
	_In_ INT 	y,
	_In_ UINT 	flOpts,
	_In_opt_ LPRECT 	prcl,
	_In_reads_opt_(cwc) LPWSTR 	pwsz,
	_In_range_(0, 0xffff) INT 	cwc,
	_In_reads_opt_(_Inexpressible_(cwc)) LPINT 	pdx,
	_In_ DWORD 	dwCodePage
	);

typedef DWORD LFTYPE;
typedef HFONT(*NtGdiHfontCreate_t)
(
	IN PENUMLOGFONTEXDVW pelfw,
	IN ULONG cjElfw,
	IN LFTYPE lft,
	IN FLONG fl,
	IN PVOID pvCliData
	); //verified

typedef HFONT(*NtGdiSelectFont_t)(
	_In_ HDC 	hdc, //verified
	_In_ HFONT 	hfont
	);

namespace miraclehook
{
	bool call_kernel_function(void* kernel_function_adress);
	NTSTATUS hook_handler(void* called_param);
	INT FrameRect(HDC hDC, const RECT* lprc, HBRUSH hbr, int thickness);


	HFONT CreateFontW(int nHeight,
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
		LPCWSTR lpszFace);

	HFONT CreateFontIndirectW(CONST LOGFONTW* lplf);
	HFONT CreateFontIndirectExW(CONST ENUMLOGFONTEXDVW* elfexd);
	BOOL extTextOutW(HDC hdc, INT x, INT y, UINT fuOptions, RECT* lprc, LPWSTR lpString, UINT cwc, INT* lpDx);
}