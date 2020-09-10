#pragma once
// graphics
#include "globals.h"

void Update();
void Render();
void StartFrame();
void EndFrame();

// drawing routines
void PutPixel(int x, int y, unsigned int color);