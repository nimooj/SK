#ifndef	_TYPEDEF_H
#define _TYPEDEF_H


#define ROUND(x) (int)((x)+0.5)

#define	LightOff()	GLboolean m_bLight = glIsEnabled( GL_LIGHTING ); \
					if ( m_bLight )	glDisable( GL_LIGHTING )

#define	LightOn()	if ( m_bLight ) glEnable( GL_LIGHTING )	

#endif //_TYPEDEF_H
