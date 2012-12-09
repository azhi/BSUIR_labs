#include "Creator.h"

Creator::Creator() {}

void Creator::BeginCreatingShape(int type, int x, int y)
{
	currentCreatingShapeType = type;
  points.clear();
	POINT startPosition = {x, y};
  points.push_back(startPosition);
}

void Creator::AddPoint(int x, int y)
{
  POINT add = {x, y};
  points.push_back(add);
}

void Creator::DrawShape(HDC hdc, int x, int y, LPCSTR text, DWORD textLength)
{
	//XFORM* PrevConvertion = new XFORM;
	//GetWorldTransform(hdc, PrevConvertion);
	XFORM* Convertion = new XFORM;
	Convertion->eDx = 0;
	Convertion->eDy = 0;
	Convertion->eM11 = 1;
	Convertion->eM22 = 1;
	Convertion->eM12 = 0;
	Convertion->eM21 = 0;
	SetWorldTransform(hdc, Convertion);
	switch (currentCreatingShapeType)
	{
	case LINE:
		MoveToEx(hdc, points[0].x, points[0].y, NULL);
		LineTo(hdc, x, y);
		break;
  case CLIP:
	case RECTANGLE:
		Rectangle(hdc, points[0].x, points[0].y, x, y);
		break;
	case ELLIPSE:
		Ellipse(hdc, points[0].x, points[0].y, x, y);
		break;
  case MTEXT:
    TextOut(hdc, points[0].x, points[0].y, text, textLength);
    break;
  case PEN:
  case POLYLINE:
    Polyline(hdc, &points[0], points.size());
    break;
  case POLYGON:
    Polygon(hdc, &points[0], points.size());
    break;
	}
}

Figure Creator::CreateShape()
{
	Figure CreatedFigure(currentCreatingShapeType, points);
	currentCreatingShapeType = 0;
	return CreatedFigure;
}

Figure Creator::CreateShape(LPCSTR text)
{
	Figure CreatedFigure(currentCreatingShapeType, points[0], text);
	currentCreatingShapeType = 0;
	return CreatedFigure;
}

Creator::~Creator(void)
{
}
