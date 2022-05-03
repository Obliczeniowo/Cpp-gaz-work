#include <windows.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#ifndef FunkcjeLiczace_h
#define FunkcjeLiczace_h
class Obliczenia{
public:
	Obliczenia(void);
	double Odl(double x1,double y1,double x2,double y2);
	double Odl(POINT p1,POINT p2);
	double Odl3D(double x1,double y1,double z1,double x2,double y2,double z2);
	double KatPt(int x1,int y1,int x2,int y2);
	double KatPt(double x1,double y1,double x2,double y2);
	double KatPt(POINT p1,POINT p2);
	void  SetKat3D(double katXY,double katXwyp,double &ktOsiX,double &ktOsiY,double &ktOsiZ,double &xSkala,double &ySkala,double &zSkala);
	double RayOfArc(POINT p1,POINT p2,POINT p3);
	double RayOfArc(int x1,int y1,int x2,int y2,int x3,int y3);
	double DegreesToRadians(double ktInDegrees);
	double RadiansToDegrees(double ktInRadians);
	double GetPI(void);
	int Zaokr(double w);
	double Zaokr(double w,UINT nrPoPrzecinku);
	POINT RotatePoint(int xObracany,int yObracany,int xObrotu,int yObrotu,double Angle);
};

void StartupFile(LPSTR &startupFile,LPSTR startupFileName);

void StartupPath(LPSTR &startupPath);

LPSTR Right(LPSTR text,UINT nr);

LPSTR Left(LPSTR text,UINT nr);

LPSTR WytnijTekst(LPSTR text,UINT nrPocz,UINT szer);

LPSTR ZastTekst(LPSTR text,LPSTR textZ,UINT nrPocz,UINT szer);

LPSTR FindX(LPSTR fu,double x);

int SprNaw(LPSTR fu);

void FindNaw(LPSTR fu,UINT &pocz,UINT &kon);

bool IsNumeric(LPSTR liczba);

double Liczba(LPSTR fu,UINT p);

double LiczFu(LPSTR fu,double x);
#endif