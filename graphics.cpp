#include <windows.h>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "graphics.h"
#include "globals.h"

static int counter = 0;

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