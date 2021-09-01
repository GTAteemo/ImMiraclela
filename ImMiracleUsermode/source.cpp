
#include <Windows.h>
#include <iostream>
#include "memory.hpp"
#include "cheat.hpp"
#include "overlay.h"

#include "visuals.hpp"

using namespace std;

auto pRender = PRENDER::Instance();
auto gameData = EFTData::Instance();


char* lootAllChar = "关闭";
char* lootAll2Char = "高级";
char* playerALlChar = "开启";
char* playerRectChar = "关闭";
char* aimCurseChar = "关闭";

char* toogle[2] = { "开启","关闭" };
char* lootToogle[7] = { "全部","高级","任务","装备","食物","医疗","配件"};

boolean insertToogle = true;
// render function
void render_scene()
{
	// clear the window alpha
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();    // begins the 3D scene

	//=============================================menu=================================================
	static char cTitle[256];
	sprintf_s(cTitle, "ImMiracleLaa V3.0 \n\n\
		外部帧率: %i\n\n\
		玩家数量: %i\n\n\
		F1 物资: %s\n\n\
		F2 物资模式: %s\n\n\
		F3 方框: %s\n\n\
		F4 骨骼: %s\n\n\
		F5 圈圈: %s\n\n", FrameRate(), gameData->playercount, lootAllChar, lootAll2Char, playerRectChar, playerALlChar, aimCurseChar);



	if (insertToogle)
	{
		
		GradientBoxOutlined(40, 70, StringWidth(pFont, cTitle) + 4, 190, 1, D3DCOLOR_RGBA(60, 169, 129, 255), D3DCOLOR_RGBA(33, 133, 199, 255), D3DCOLOR_RGBA(100, 0, 0, 0), true);
		String(42, 72, D3DCOLOR_RGBA(255, 255, 255, 255), pFont, true, cTitle);
	}

	if (GetAsyncKeyState(VK_INSERT))
	{
		insertToogle = !insertToogle; Sleep(200);
	}

	if (GetAsyncKeyState(VK_F1))
	{
		PRENDER::Instance()->lootAll = !PRENDER::Instance()->lootAll; Sleep(200);
		if (lootAllChar == toogle[0])
		{
			lootAllChar = toogle[1];
		}
		else
		{
			lootAllChar = toogle[0];
		}

	}
	if (GetAsyncKeyState(VK_F2))
	{
		
		if (lootAll2Char == lootToogle[0])
		{
			lootAll2Char = lootToogle[1];
			PRENDER::Instance()->lootAll2 = true; 
			Sleep(200);
		}
		else if (lootAll2Char == lootToogle[1])
		{
			lootAll2Char = lootToogle[2];
			PRENDER::Instance()->taskLoot = true;
			PRENDER::Instance()->lootAll2 = false;
			Sleep(200);
		}
		else if (lootAll2Char == lootToogle[2])
		{
			lootAll2Char = lootToogle[3];
			PRENDER::Instance()->equipAndPlayer = true;
			PRENDER::Instance()->taskLoot = false;
			Sleep(200);
		}
		else if (lootAll2Char == lootToogle[3])
		{
			lootAll2Char = lootToogle[4];
			PRENDER::Instance()->foods = true;
			PRENDER::Instance()->equipAndPlayer = false;
			Sleep(200);
		}
		else if (lootAll2Char == lootToogle[4])
		{
			lootAll2Char = lootToogle[5];
			PRENDER::Instance()->meds = true;
			PRENDER::Instance()->foods = false;
			Sleep(200);
		}
		else if (lootAll2Char == lootToogle[5])
		{
			lootAll2Char = lootToogle[6];
			PRENDER::Instance()->mods = true;
			PRENDER::Instance()->meds = false;
			Sleep(200);
		}
		else if (lootAll2Char == lootToogle[6])
		{
			lootAll2Char = lootToogle[0];
			PRENDER::Instance()->mods = false;
			Sleep(200);
		}
	}
	if (GetAsyncKeyState(VK_F3))
	{
		PRENDER::Instance()->playerRect = !PRENDER::Instance()->playerRect; Sleep(200);
		if (playerRectChar == toogle[0])
		{
			playerRectChar = toogle[1];
		}
		else
		{
			playerRectChar = toogle[0];
		}

		Sleep(200);

	}
	if (GetAsyncKeyState(VK_F4))
	{
		PRENDER::Instance()->playerALL = !PRENDER::Instance()->playerALL; Sleep(200);
		if (playerALlChar == toogle[0])
		{
			playerALlChar = toogle[1];
		}
		else
		{
			playerALlChar = toogle[0];
		}
	}
	if (GetAsyncKeyState(VK_F5))
	{
		PRENDER::Instance()->aimCurse = !PRENDER::Instance()->aimCurse;

		if (aimCurseChar == toogle[0])
		{
			aimCurseChar = toogle[1];
		}
		else
		{
			aimCurseChar = toogle[0];
		}

	}
	//=============================================menu===================================================
	//calculate and and draw esp stuff
	pRender->Render();

	//draw_string(10, 10, color, pFont, "output");

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
}
//set up overlay window
void start_window()
{
	RECT rc;

	twnd = NULL;
	while (!twnd)
		twnd = FindWindow("UnityWndClass", 0);
	if (twnd != NULL)
	{
		GetWindowRect(twnd, &rc);
		s_width = rc.right - rc.left;
		s_height = rc.bottom - rc.top;
	}

	WNDCLASSEX wndclass;
	ZeroMemory(&wndclass, sizeof(WNDCLASSEX)); // Initialises, sets all bits to 0 to remove garbage data
	wndclass.cbClsExtra = NULL;
	wndclass.cbWndExtra = NULL;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WindowProc; // Function that will be executed when the window receives a "message" (input). Required! (crashes if set to NULL)
	wndclass.hInstance = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(0, IDI_APPLICATION);

	wndclass.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	wndclass.lpszClassName = "Class_RiotWnd";
	RegisterClassEx(&wndclass);

	hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, wndclass.lpszClassName, "", WS_POPUP, rc.left, rc.top, s_width, s_height, NULL, NULL, wndclass.hInstance, NULL);

	// Activate transparency on color black.
	/*SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, ULW_COLORKEY);
	SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);*/
	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, 0);
	SetLayeredWindowAttributes(hWnd, 0, 255, 0);
	//	SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);

	ShowWindow(hWnd, SW_SHOW);
	initD3D(hWnd);

	
	

}

uint32_t EntryMode()
{
	MSG msg;
	//RECT rc;

	while (true)
	{
		if (gameData->load())
		{
			break;
		}
		Sleep(1000);
	}

	
	
	
	
	
	

	while (TRUE)
	{

		ZeroMemory(&msg, sizeof(MSG));
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			exit(0);

		SetWindowPos(twnd, hWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		UpdateWindow(hWnd);
		

		if (!gameData->read())
		{
			gameData->init();
			continue;
		}

		//render your esp
		render_scene();

		Sleep(2);
	}
	return 0;
}

int main()
{
	printf("Loading necessary modules...\n");
	LoadLibraryA("user32.dll");
	LoadLibraryA("win32u.dll");
	if (GetModuleHandleA("win32u.dll"))
	{
		printf("[TARKOV]: Modules loaded\n");
		
		start_window();

		

		uint32_t entry = (uint32_t)EntryMode();
		//getchar();
		return entry;
	}
	else
	{
		printf("[TARKOV]: Modules failed to load, quitting\n");
	}

	getchar();
	return 0;
}