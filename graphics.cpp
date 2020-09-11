#include <windows.h>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "graphics.h"
#include "globals.h"

static int counter = 0;
static int x = 0;
static int y = 0;
static int dirx = 1;
static int diry = 1;

void swap(int& a, int& b)
{
	int tmp = a;
	a = b;
	b = tmp;
}

void PutPixel(int x, int y, unsigned int color)
{
	assert(x >= 0);
	assert(x < bmpWidth);
	assert(y >= 0);
	assert(y < bmpHeight);
	
	unsigned int* pixel = (unsigned int*)pBitmap;
	
  *(pixel + (y * bmpWidth + x)) =  color;
}

void StartFrame()
{
	memset(pBitmap, 0, bmpMemSize);
}

void EndFrame()
{	
	// copy to front buffer
/* 	StretchDIBits(
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
		SRCCOPY); */
	SetDIBitsToDevice(
		hdc,
		0,
		0,
		bmpWidth,
		bmpHeight,
		0,
		0,
		0,
		bmpHeight,
		pBitmap,
		&bi,
		DIB_RGB_COLORS);
}

void Update()
{	
	x += dirx;
	y += diry;
	
	if(x < 0)
	{
		x = 0;
		dirx = 1;
	}
	else if(x >= (bmpWidth - 200))
	{
		x = bmpWidth - 200;
		dirx = -1;
	}
	
	if(y < 0)
	{
		y = 0;
		diry = 1;
	}
	else if(y >= (bmpHeight - 100))
	{
		y = bmpHeight - 100;
		diry = -1;
	}
}

void Render()
{	
/* 	for(int y = 0; y < bmpHeight; ++y)
	{
		for(int x = 0; x < bmpWidth; ++x)
		{
			PutPixel(x, y, RGB(rand() % 256, rand() % 256, rand() % 256));
		}
	} */
	unsigned int color = rand() % 0xFFFFFF;
	DrawRectDim(x, y, 200, 100, color);
}

void DrawRect( int x0,int y0,int x1,int y1, unsigned int c )
{
	if( x0 > x1 )
	{
		swap( x0,x1 );
	}
	if( y0 > y1 )
	{
		swap( y0,y1 );
	}

	for( int y = y0; y < y1; ++y )
	{
		for( int x = x0; x < x1; ++x )
		{
			PutPixel( x,y,c );
		}
	}
}
void DrawRectDim( int x0,int y0,int width,int height, unsigned int c )
{
	DrawRect( x0,y0,x0 + width,y0 + height,c );
}