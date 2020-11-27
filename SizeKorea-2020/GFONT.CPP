#include "pch.h"
#include "GFont.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//
//GFont::GFont()
//{
//	font = NULL;
//	hdc = NULL;
//	height = 0;
//}
//GFont::GFont(char *font_type, int height, int weight, DWORD italic)
//{
//	hdc = wglGetCurrentDC();
//	FontCreate( hdc, font_type, height, weight, italic );
//}
//GFont::~GFont()
//{
//	FontDestroy();
//}
//
//
//void GFont::FontCreate( HDC hdc, const char *typeface, int height, int weight, DWORD italic )
//{
//	
//	HFONT fontid;
//	int charset;
//
//	//if ( ( font = (GFont *) calloc( 1, sizeof( GFont ) ) ) == ( GFont * ) 0 )
//	
//	font = new ( GFont );
//	if ( ( base = glGenLists( 256 ) ) == 0 )
//	{
//		delete font;
//	}
//
//	if ( _stricmp( typeface, "symbol" ) == 0 )
//		charset = SYMBOL_CHARSET;
//	else
//		charset = ANSI_CHARSET;
//
//	fontid = CreateFont( height, 0, 0, 0, weight, italic, FALSE, FALSE, charset, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, (CString)typeface );
//	SelectObject( hdc, fontid );
//
//	wglUseFontBitmaps( hdc, 0, 256, base );
//	GetCharWidth( hdc, 0, 255, widths );
//	height = height;
//}
//
//void GFont::FontDestroy( )
//{
//	if ( font == ( GFont * ) 0 )
//		return;
//	glDeleteLists( base, 256 );
//	delete font;
//}
//
//void GFont::FontPuts( int x, int y, const char *s )
//{
//	if ( font == ( GFont *) 0 || s == ( char * ) 0 )
//		return;
//
//	glPushAttrib( GL_LIST_BIT );
//
//	int index = 0;
//	if ( glIsEnabled( GL_TEXTURE_1D ) )
//		index = 1;
//	else if ( glIsEnabled( GL_TEXTURE_2D ) )
//		index = 2;
//	switch( index )
//	{
//	case 0:
//		break;
//	case 1:
//		glDisable( GL_TEXTURE_1D );
//		break;
//	case 2:
//		glDisable( GL_TEXTURE_2D );
//		break;
//	}
//	glPushMatrix(); // current modelview matrix is pushed.
//		glLoadIdentity(); // identity matrix is loaded in modelview matrix.
//		glMatrixMode(GL_PROJECTION); // current matrix is changed to projection matrix.
//		glPushMatrix(); // current projection matrix is pushed.
//			glLoadIdentity(); // identity matrix is loaded in projection matrix.
//			glOrtho(0.0, 100.0, 0.0, 100.0, -100.0, 100.0);
//			glColor3f( 0.0f, 1.0f, 0.0f );
//			glRasterPos2i( x, y );
//			glListBase( base );
//			glCallLists( strlen( s ), GL_UNSIGNED_BYTE, s );
//		glPopMatrix(); // projection matrix is popped
//		glMatrixMode( GL_MODELVIEW ); // current matrix is changed to modelview matrix.
//	glPopMatrix(); // modelview matrix is popped.
//
//	switch( index )
//	{
//	case 0:
//		break;
//	case 1:
//		glEnable( GL_TEXTURE_1D );
//		break;
//	case 2:
//		glEnable( GL_TEXTURE_2D );
//		break;
//	}
//	
//	glPopAttrib();
//}
//
//void GFont::FontPrintf( int x, int y, int align, const char *format, ...)
//{
//	va_list ap;
//	unsigned char s[1024], *ptr;
//	int width;
//	if ( font == (GFont *) 0 || format == (char *) 0 )
//		return;
//	va_start( ap, format );
//	vsprintf( (char *)s, format, ap );
//	va_end( ap );
//
//	for (ptr = s, width = 0; *ptr; ptr ++)
//        width += widths[*ptr];
//
//    /* Adjust the bitmap position as needed to justify the text */
//    if (align < 0)
//        glBitmap(0, 0, 0, 0, -width, 0, NULL);
//    else if (align == 0)
//        glBitmap(0, 0, 0, 0, -width / 2, 0, NULL);
//	FontPuts( x, y, (const char *)s );
//}
//
//void GFont::drawTitle()
//{
//		FontPrintf( 45, 6, 1, "Made by shyun. Copyright(c) 2000."); 
//		FontPrintf( 45, 2, 1, "All right reserved." );
//}
