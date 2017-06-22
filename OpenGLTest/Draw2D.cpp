#include <math.h>
#include <stdlib.h>
#include <string>
#include "Draw2D.h"

using namespace std;

namespace PSEngine{

	const float C_PI = 3.1415926;

	void PSAPI SetPixel(int x, int y)
	{
		glVertex2i(x, y);
	}

	void PSAPI SetPixel(float x, float y)
	{
		glVertex2f(x, y);
	}

	void PSAPI SetCirclePixel(int x, int y, Vector2Di& pt)
	{
		SetPixel(x + pt.x, y + pt.y);
		SetPixel(x + pt.x, -y + pt.y);
		SetPixel(-x + pt.x, y + pt.y);
		SetPixel(-x + pt.x, -y + pt.y);
		SetPixel(y + pt.x, x + pt.y);
		SetPixel(-y + pt.x, x + pt.y);
		SetPixel(y + pt.x, -x + pt.y);
		SetPixel(-y + pt.x, -x + pt.y);
	}

	void PSAPI SetEllipsePixel(int x, int y, Vector2Di& pt)
	{
		SetPixel(x + pt.x, y + pt.y);
		SetPixel(-x + pt.x, y + pt.y);
		SetPixel(x + pt.x, -y + pt.y);
		SetPixel(-x + pt.x, -y + pt.y);
	}

	void PSAPI DrawLine(float xb, float yb, float xe, float ye)
	{
		DrawLineBres(xb, yb, xe, ye);
	}

	__inline float DDARound(float x)
	{
		return float(x + 0.5);
	}

	void PSAPI DrawLineDDA(float xb, float yb, float xe, float ye)
	{
		float dx = xe - xb;
		float dy = ye - yb;
		float steps;
		float xInc;
		float yInc;

		if (fabs(dx) > fabs(dy))
			steps = fabs(dx);
		else
			steps = fabs(dy);

		xInc = dx / steps;
		yInc = dy / steps;

		SetPixel(DDARound(xb), DDARound(yb));
		for (int i = 0; i != steps; ++i)
		{
			xb += xInc;
			yb += yInc;
						
			SetPixel(DDARound(xb), DDARound(yb));
		}
	}

	void PSAPI DrawLineBres(int xb, int yb, int xe, int ye)
	{
		//Bresenham draw-line algorithm.
		//P(k+1) = 
		//1. P(k) + 2dy; (P(k)<0)
		//2. P(k) + 2dy - 2dx; (P(k)>0)
		int dx = xe - xb;
		int dy = ye - yb;
		int i = 1, j = 1;

		if (dx < 0)
		{
			dx = abs(dx);
			i = -1;
		}
		if (dy < 0)
		{
			dy = abs(dy);
			j = -1;
		}

		//If is nessesary,four conditions.
		if (!dx)
		{
			while (yb != ye)
			{
				SetPixel(xb, yb);
				yb += j;
			}
		}
		else if (!dy)
		{
			while (xb != xe)
			{
				SetPixel(xb, yb);
				xb += i;
			}
		}
		else if (dx == dy)
		{
			while (xb != xe)
			{
				SetPixel(xb, yb);
				xb += i;
				yb += j;
			}
		}
		else
		{
			if (dx > dy)
			{
				int p = (dy << 1) - dx;
				int d0 = (dy << 1);
				int d1 = (dy - dx) << 1;

				SetPixel(xb, yb);
				for (; xb != xe; xb += i)
				{
					if (p < 0)
					{
						p += d0;
					}
					else
					{
						yb += j;
						p += d1;
					}
					SetPixel(xb, yb);
				}
			}
			else
			{
				int p = (dx << 1) - dy;
				int d0 = (dx << 1);
				int d1 = (dx - dy) << 1;

				SetPixel(xb, yb);
				for (; yb != ye; yb += j)
				{
					if (p < 0)
					{
						p += d0;
					}
					else
					{
						xb += i;
						p += d1;
					}
					SetPixel(xb, yb);
				}
			}
		}
	}

	void PSAPI DrawRectangle(float xb, float yb, float xe, float ye)
	{
		DrawLine(xb, yb, xe, yb);
		DrawLine(xb, yb, xb, ye);
		DrawLine(xe, yb, xe, ye);
		DrawLine(xb, ye, xe, ye);
	}

	void PSAPI DrawCircle(float x, float y, float radius)
	{
		DrawCircleMidPoint(x, y, radius);
	}

	void PSAPI DrawCircleMidPoint(int x, int y, int radius)
	{
		int p = 1 - radius; // 5/4 - r ~ 1 - r;
		int xb = 0;
		int yb = radius;
		Vector2Di pt;
		pt.x = x;
		pt.y = y;

		do
		{
			SetCirclePixel(xb, yb, pt);
			++xb;
			if (p < 0)
			{
				p += (xb << 1) + 1;
			}
			else
			{
				--yb;
				p += ((xb - yb) << 1) + 1;
			}
		} while (xb < yb);
	}

	void PSAPI DrawEllipse(float x, float y, float rx, float ry)
	{
		DrawEllipseMidPoint(x, y, rx, ry);
	}

	void PSAPI DrawEllipseMidPoint(int x, int y, int rx, int ry)
	{
	}
};