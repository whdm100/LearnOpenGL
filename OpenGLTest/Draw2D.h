#ifndef _DRAW_METHOD_H
#define _DRAW_METHOD_H

#include "GLImp.h"

#ifndef PSAPI
#define PSAPI _cdecl
#endif // PSAPI

namespace PSEngine{

	struct Vector2Df{ float x; float y; };
	struct Vector2Di{ int x; int y; };

	struct Vector3Df{ float x; float y; float z; };
	struct Vector3Di{ int x; int y; int z; };

	__inline void PSAPI SetPixel(int x, int y);
	__inline void PSAPI SetPixel(float x, float y);
	__inline void PSAPI SetCirclePixel(int x, int y, Vector2Di& pt);
	__inline void PSAPI SetEllipsePixel(int x, int y, Vector2Di& pt);

	__inline void PSAPI DrawLine(float xbegin, float ybegin, float xend, float yend);

	void PSAPI DrawLineDDA(float xbegin, float ybegin, float xend, float yend);
	void PSAPI DrawLineBres(int xbegin, int ybegin, int xend, int yend);

	__inline void PSAPI DrawRectangle(float xbegin, float ybegin, float xend, float yend);

	__inline void PSAPI DrawCircle(float x, float y, float radius);

	void PSAPI DrawCircleMidPoint(int x, int y, int radius);

	__inline void PSAPI DrawEllipse(float x, float y, float dh, float dv);

	void PSAPI DrawEllipseMidPoint(int x, int y, int rx, int ry);
};

#endif //_DRAW_METHOD_H