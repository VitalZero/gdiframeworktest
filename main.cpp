#include <windows.h>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "globals.h"
#include "graphics.h"
#include <chrono>

unsigned int* pBitmap = nullptr; // changed from void*
int bmpMemSize = 0;
int bmpWidth = 0;
int bmpHeight = 0;
HDC hdc = nullptr;
BITMAPINFO bi = {0};

static const char* CLASS_NAME = "gditest";
static const char* WND_NAME = "GDI test";
static PAINTSTRUCT ps;

void PrintFPS(HWND hwnd)
{
	static auto oldTime = std::chrono::steady_clock::now();
	static int fps; fps++;
	
	if ((std::chrono::steady_clock::now() - oldTime) >= std::chrono::seconds{ 1 }) 
	{
		oldTime = std::chrono::steady_clock::now();
		char buffer[20] = {0};
		sprintf(buffer, "%s - %d", WND_NAME, fps);
		SetWindowText(hwnd, buffer);
		fps = 0;
	}
}

void Go()
{
	StartFrame();
	Update();
	Render();
	EndFrame();
}

void Register();
HWND Create(int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
	HINSTANCE,
	HINSTANCE,
	LPSTR,
	int)
{
	Register();
	HWND hwnd = Create(640, 480);
	assert(hwnd != nullptr);
	
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);
	
	MSG msg = {0};
	BOOL bRet = TRUE;
	clock_t start;
	clock_t end;
	float fps;
		
	while(bRet)
	{
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		if(msg.message == WM_QUIT)
		{
			bRet = FALSE;
			break;
		}
		
		Go();
		PrintFPS(hwnd);		
	}
	
	return msg.wParam;
}

void Register()
{
	WNDCLASSEX wcx = {0};
	wcx.lpfnWndProc = WndProc;
	wcx.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcx.hIconSm = wcx.hIcon;
	wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcx.lpszClassName = CLASS_NAME;
	wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcx.cbSize = sizeof(WNDCLASSEX);
	
	assert(RegisterClassEx(&wcx) != 0);
}

HWND Create(int width, int height)
{
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = width;
	rc.bottom = height;
	
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW &~WS_SIZEBOX &~ WS_MAXIMIZEBOX, FALSE);

	return CreateWindowEx(
		WS_EX_CLIENTEDGE,
		CLASS_NAME,
		WND_NAME,
		WS_OVERLAPPEDWINDOW &~WS_SIZEBOX &~ WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
		case WM_CREATE:
		{
			hdc = GetDC(hwnd);
			RECT rc;
			GetClientRect(hwnd, &rc);
			bmpWidth = rc.right - rc.left;
			bmpHeight = rc.bottom - rc.top;
			int bpp = 4;
						
			bi.bmiHeader.biSize = sizeof(BITMAPINFO);
			bi.bmiHeader.biWidth = bmpWidth;
			bi.bmiHeader.biHeight = -bmpHeight;
			bi.bmiHeader.biPlanes = 1;
			bi.bmiHeader.biBitCount = 32;
			bi.bmiHeader.biCompression = BI_RGB;
			
			bmpMemSize = bmpWidth * bmpHeight * bpp;
			
			pBitmap = (unsigned int*)VirtualAlloc(0, bmpMemSize, MEM_COMMIT, PAGE_READWRITE);
		}
			return 0;
			
		case WM_DESTROY:
			VirtualFree((void*)pBitmap, bmpMemSize, MEM_DECOMMIT);
			ReleaseDC(hwnd, hdc);
			PostQuitMessage(0);
			return 0;
	}
	
	return DefWindowProc(hwnd, msg, wparam, lparam);
}