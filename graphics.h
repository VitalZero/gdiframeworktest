#pragma once
// graphics

void Update();
void Render();
void StartFrame();
void EndFrame();

// drawing routines
void PutPixel(int x, int y, unsigned int color);
void DrawRect( int x0,int y0,int x1,int y1,unsigned int c );
void DrawRectDim( int x0,int y0,int width,int height,unsigned int c );