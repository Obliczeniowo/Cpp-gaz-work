#include <windows.h>
#include "myMath.h"
#pragma once
#ifndef POINTSCLASS_H
#define POINTSCLASS_H
class Point{
public:
	int x;
	int y;
	Point();
	Point(int x,int y);
	Point(POINT p);
	void SetPoint(POINT p);
	void SetPoint(Point p);
	void SetPoint(int x,int y);
	void SetOffsetPoint(int dp);
	void SetOffsetPoint(int dx,int dy);
	void SetOffsetPoint(POINT dp);
	void SetOffsetPoint(Point dp);
	void RotatePoint(int xObrotu,int yObrotu,double ArrayInRadians);
	void RotatePoint(Point pObr,double ArrayInRadians);
	void RotatePoint(POINT pObr,double ArrayInRadians);
	double GetOdl(POINT MyPoint);
	double GetOdl(Point MyPoint);
	double GetOdl(int x,int y);
	double GetKtInRadians(POINT MyPoint,bool fromTo);
	double GetKtInRadians(Point MyPoint,bool fromTo);
	double GetKtInRadians(int x,int y,bool fromTo);
	POINT GetPoint(void);
};

class dPoint{
public:
	double x;
	double y;
	dPoint();
	dPoint(double x,double y);
	dPoint(POINT p);
	void SetPoint(POINT p);
	void SetPoint(dPoint p);
	void SetPoint(double x,double y);
	void SetOffsetPoint(double dp);
	void SetOffsetPoint(double dx,double dy);
	void SetOffsetPoint(POINT dp);
	void SetOffsetPoint(dPoint dp);
	void RotatePoint(double xObrotu,double yObrotu,double ArrayInRadians);
	void RotatePoint(dPoint pObr,double ArrayInRadians);
	void RotatePoint(POINT pObr,double ArrayInRadians);
	double GetOdl(POINT MyPoint);
	double GetOdl(dPoint MyPoint);
	double GetOdl(double x,double y);
	double GetKtInRadians(POINT MyPoint,bool fromTo);
	double GetKtInRadians(dPoint MyPoint,bool fromTo);
	double GetKtInRadians(double x,double y,bool fromTo);
	POINT GetPoint(void);
};

class dPoint3D{
public:
	double x;
	double y;
	double z;
	dPoint3D(void);
	dPoint3D(double x,double y,double z);
	dPoint3D(dPoint p,double z);
	dPoint3D(dPoint p);
	dPoint3D(POINT p,double z);
	dPoint3D(POINT p);
	void SetPoint(dPoint3D p);
	void SetPoint(double x,double y,double z);
	void SetPoint(dPoint p,double z);
	void SetPoint(dPoint p);
	void SetPoint(POINT p,double z);
	void SetPoint(POINT p);
	void SetOffset(double dx,double dy,double dz);
	void SetOffset(dPoint3D dp);
	double GetOdl3D(double x,double y,double z);
	double GetOdl3D(dPoint3D p);
	double GetOdlXY(double x,double y);
	double GetOdlYZ(double y,double z);
	double GetOdlXZ(double x,double z);
	POINT convertToPoint(int dx,int dy,double ktOsiX,double ktOsiY,double ktOsiZ,double xSkala,double zSkala,double ySkala);
};

class PointsClass
{
public:
	PointsClass(void);
	~PointsClass(void);
};
#endif