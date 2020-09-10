#include <windows.h>
#include <cassert>
#include <cstdlib>
#include <ctime>

static const char* CLASS_NAME = "gditest";
static const char* WND_NAME = "GDI test";
static void* pBitmap = nullptr;
static int bmpMemSize = 0;
static int bmpWidth;
static int bmpHeight;
static BITMAPINFO bi = {0};
static PAINTSTRUCT ps;
static int counter = 0;
static HDC hdc;

void OnPaint(HWND hwnd);
void PutPixel(int x, int y, unsigned int color);
void Update();
void Render();
void StartFrame();
void EndFrame();
void Go()
{
	StartFrame();
	Update();
	Render();
	EndFrame();
}

void Register();
HWND Create();

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
	HINSTANCE,
	HINSTANCE,
	LPSTR,
	int)
{
	Register();
	HWND hwnd = Create();
	assert(hwnd != nullptr);
	
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);
	
	MSG msg = {0};
	BOOL bRet = TRUE;
	
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

HWND Create()
{
	return CreateWindowEx(
		WS_EX_CLIENTEDGE,
		CLASS_NAME,
		WND_NAME,
		WS_OVERLAPPEDWINDOW &~WS_SIZEBOX &~ WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
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
			
			pBitmap = VirtualAlloc(0, bmpMemSize, MEM_COMMIT, PAGE_READWRITE);
		}
			return 0;
			
		case WM_DESTROY:
			VirtualFree(pBitmap, bmpMemSize, MEM_DECOMMIT);
			ReleaseDC(hwnd, hdc);
			PostQuitMessage(0);
			return 0;
	}
	
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void PutPixel(int x, int y, unsigned int color)
{
	unsigned long* pixel = (unsigned long*)pBitmap;
	
  *(pixel + (y * bmpWidth + x)) =  color;
}

void StartFrame()
{
	memset(pBitmap, 0, bmpMemSize);
}

void EndFrame()
{	
	// copy to front buffer
	StretchDIBits(
		hdc,
		0,
		0,
		bmpWidth,
		bmpHeight,
		0,
		0,
		bmpWidth,
		bmpHeight,
		pBitmap,
		&bi,
		DIB_RGB_COLORS,
		SRCCOPY);
}

void Update()
{
	counter++;
}

void Render()
{	
	for(int y = 0; y < bmpHeight; ++y)
	{
		for(int x = 0; x < bmpWidth; ++x)
		{
			PutPixel(x, y, RGB(rand() % 256, rand() % 256, rand() % 256));
		}
	}
}