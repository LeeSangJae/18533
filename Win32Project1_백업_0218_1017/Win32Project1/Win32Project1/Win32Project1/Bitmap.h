#pragma once
#include "header.h"

VOID Draw_Image(HDC hdc, char* Image_name, int xpos, int ypos)
{
	wchar_t buf[100] = { 0 };
	int lens = 0;
	lens = swprintf(buf, 100, L"%hs", Image_name); // %hs «ÿ¡‡æﬂ«‘

	Image image(buf);
	Graphics graphics(hdc);
	graphics.DrawImage(&image, xpos, ypos);
}

