#ifndef myMath_h
#define myMath_h

#include <windows.h>

#define _USE_MATH_DEFINES

#include <math.h>

class myMath{
public:
	static double	Dist(double x1,double y1,double x2,double y2);
	static double	Dist(POINT p1,POINT p2);
	static double	Dist3D(double x1,double y1,double z1,double x2,double y2,double z2);
	static double	GetAngle(double x1,double y1,double x2,double y2);
	static double	GetAngle(POINT p1,POINT p2);
	static double	RayOfArc(POINT p1,POINT p2,POINT p3);
	static double	RayOfArc(int x1,int y1,int x2,int y2,int x3,int y3);
	static double	DegreesToRadians(double AngleInDegrees);
	static double	RadiansToDegrees(double AngleInRadians);
	static int		Zaokr(double w);
	static double	Zaokr(double w,UINT nrPoPrzecinku);
	static POINT	RotatePoint(int xObracany,int yObracany,int xObrotu,int yObrotu,double ktObrotu);
	static POINT	Bezier(double t, POINT firstPoint,POINT lastPoint,double firstAngle,double lastAngle, double firstRay, double lastRay);
	static POINT	Bezier(double t, POINT firstPoint,POINT lastPoint,POINT first,POINT last);
	static double	Silnia(int i);
	static int		Rand(int Min, int Max);
	static int		Rand(int Max);
	static double	RandD();
	static double	RandD(double Max,double Min);
	static double	log_a_b(double a,double b);
	static int		IntDivision(int dividend, int divisor);
	static float	IntDivision(float dividend, float divisor);
	static double	IntDivision(double dividend,double divisor);
	static void		SetAngle3D(double katXY,double katXwyp,double &ktOsiX,double &ktOsiY,double &ktOsiZ,double &xSkala,double &ySkala,double &zSkala);
};

#endif