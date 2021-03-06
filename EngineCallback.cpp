/*
enginecallback.cpp - actual engine callbacks
Copyright (C) 2017 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "extdll_menu.h"
#include "BaseMenu.h"
#include "Utils.h"

void EngFuncs::PIC_Set(HIMAGE hPic, int r, int g, int b, int a)
{
	if( uiStatic.enableAlphaFactor )
		a *= uiStatic.alphaFactor;

	engfuncs.pfnPIC_Set( hPic, r, g, b, a );
}

void EngFuncs::FillRGBA(int x, int y, int width, int height, int r, int g, int b, int a)
{
	if( uiStatic.enableAlphaFactor )
		a *= uiStatic.alphaFactor;

	engfuncs.pfnFillRGBA( x, y, width, height, r, g, b, a );
}

void EngFuncs::DrawCharacter(int x, int y, int width, int height, int ch, int ulRGBA, HIMAGE hFont)
{
	if( uiStatic.enableAlphaFactor )
	{
		return;

		/*int copy = ulRGBA;
		int a = UnpackAlpha( copy );
		a *= uiStatic.alphaFactor;

		ulRGBA = PackAlpha( copy, a );*/
	}

	engfuncs.pfnDrawCharacter( x, y, width, height, ch, ulRGBA, hFont );
}

#ifndef MAINUI_USE_CUSTOM_FONT_RENDER
void EngFuncs::DrawSetTextColor(int r, int g, int b, int alpha)
{
	if( uiStatic.enableAlphaFactor )
		alpha *= uiStatic.alphaFactor;

	engfuncs.pfnDrawSetTextColor( r, g, b, alpha );
}
#endif

#ifdef MAINUI_USE_CUSTOM_FONT_RENDER

unsigned int color;

void EngFuncs::DrawSetTextColor(int r, int g, int b, int alpha)
{
	color = PackRGBA( r, g, b, alpha );
}

int EngFuncs::DrawConsoleString(int x, int y, const char *string)
{
	const char *ptext = string;

	Point pt(x, y);

	for( ; *ptext; ptext++ )
	{
		if( *ptext == '\n' )
		{
			pt.x = x;
			pt.y += g_FontMgr.GetFontTall( uiStatic.hConsoleFont );
			continue;
		}

		pt.x += g_FontMgr.DrawCharacter( uiStatic.hConsoleFont, *ptext, pt, Size(), color );
	}

	return pt.x;
}

void EngFuncs::ConsoleStringLen(const char *string, int *length, int *height)
{
	g_FontMgr.GetTextSize( uiStatic.hConsoleFont, string, length, height );
}

int EngFuncs::ConsoleCharacterHeight()
{
	return g_FontMgr.GetFontTall( uiStatic.hConsoleFont );
}

#endif

int EngFuncs::UtfProcessChar(int ch)
{
#ifdef MAINUI_USE_CUSTOM_FONT_RENDER
	return Con_UtfProcessChar( ch );
#else
	if( textfuncs.pfnUtfProcessChar )
		return textfuncs.pfnUtfProcessChar( ch );
	else return ch;
#endif
}

int EngFuncs::UtfMoveLeft(char *str, int pos)
{
#ifndef MAINUI_USE_CUSTOM_FONT_RENDER
	if( textfuncs.pfnUtfMoveLeft )
		return textfuncs.pfnUtfMoveLeft( str, pos );
	else return pos - 1;
#else
	return Con_UtfMoveLeft( str, pos );
#endif
}

int EngFuncs::UtfMoveRight(char *str, int pos, int length)
{
#ifndef MAINUI_USE_CUSTOM_FONT_RENDER
	if( textfuncs.pfnUtfMoveLeft )
		return textfuncs.pfnUtfMoveRight( str, pos, length );
	else return pos + 1;
#else
	return Con_UtfMoveRight( str, pos, length );
#endif
}
