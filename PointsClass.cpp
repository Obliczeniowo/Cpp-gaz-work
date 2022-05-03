#include ".\pointsclass.h"
//Klasa Point
Point::Point(){
	x=y=0;
}

Point::Point(POINT p){
	x=p.x;
	y=p.y;
}

Point::Point(int x,int y){
	this->x=x;
	this->y=y;
}

void Point::SetPoint(int x,int y){
	this->x=x;
	this->y=y;
}

void Point::SetPoint(POINT p){
	x=p.x;
	y=p.y;
}

void Point::SetPoint(Point p){
	x=p.x;
	y=p.y;
}

void Point::SetOffsetPoint(Point dp){
	x+=dp.x;
	y+=dp.y;
}

void Point::SetOffsetPoint(int dx,int dy){
	x+=dx;
	y+=dy;
}

void Point::SetOffsetPoint(POINT dp){
	x+=dp.x;
	y+=dp.y;
}

void Point::RotatePoint(int xObrotu,int yObrotu,double ArrayInRadians){
	SetPoint(myMath::RotatePoint(x,y,xObrotu,yObrotu,ArrayInRadians));
}

void Point::RotatePoint(Point pObr,double ArrayInRadians){
	RotatePoint(pObr.x,pObr.y,ArrayInRadians);
}

void Point::RotatePoint(POINT pObr,double ArrayInRadians){
	RotatePoint(pObr.x,pObr.y,ArrayInRadians);
}

double Point::GetKtInRadians(int x,int y,bool fromTo){
	if(fromTo)
		return myMath::GetAngle(x,y,this->x,this->y);
	return myMath::GetAngle(this->x,this->y,x,y);
}

double Point::GetKtInRadians(POINT MyPoint,bool fromTo){
	return GetKtInRadians(MyPoint.x,MyPoint.y,fromTo);
}

double Point::GetKtInRadians(Point MyPoint,bool fromTo){
	return GetKtInRadians(MyPoint.x,MyPoint.y,fromTo);
}

double Point::GetOdl(POINT MyPoint){
	return GetOdl(MyPoint.x,MyPoint.y);
}

double Point::GetOdl(Point MyPoint){
	return GetOdl(MyPoint.x,MyPoint.y);
}

double Point::GetOdl(int x,int y){
	return myMath::Dist(this->x,this->y,x,y);
}

POINT Point::GetPoint(void){
	POINT p;
	p.x=x;
	p.y=y;
	return p;
}
//Koniec klasy Point
//Pocz¹tek klasy dPoint
dPoint::dPoint(){
	x=y=0;
}

dPoint::dPoint(POINT p){
	x=p.x;
	y=p.y;
}

dPoint::dPoint(double x,double y){
	this->x=x;
	this->y=y;
}

void dPoint::SetPoint(double x,double y){
	this->x=x;
	this->y=y;
}

void dPoint::SetPoint(POINT p){
	x=p.x;
	y=p.y;
}

void dPoint::SetPoint(dPoint p){
	x=p.x;
	y=p.y;
}

void dPoint::SetOffsetPoint(dPoint dp){
	x+=dp.x;
	y+=dp.y;
}

void dPoint::SetOffsetPoint(double dx,double dy){
	x+=dx;
	y+=dy;
}

void dPoint::SetOffsetPoint(POINT dp){
	x+=dp.x;
	y+=dp.y;
}

void dPoint::RotatePoint(double xObrotu,double yObrotu,double ArrayInRadians){
	double odl = myMath::GetAngle(x, y, xObrotu, yObrotu);
	double ktPocz = myMath::GetAngle(xObrotu, yObrotu, x, y);
	x = odl * cos(ArrayInRadians + ktPocz) + xObrotu;
	y = odl * sin(ArrayInRadians + ktPocz) + yObrotu;
}

void dPoint::RotatePoint(dPoint pObr,double ArrayInRadians){
	RotatePoint(pObr.x,pObr.y,ArrayInRadians);
}

void dPoint::RotatePoint(POINT pObr,double ArrayInRadians){
	RotatePoint(pObr.x,pObr.y,ArrayInRadians);
}

double dPoint::GetKtInRadians(double x,double y,bool fromTo){
	if(fromTo)
		return myMath::GetAngle(x,y,this->x,this->y);
	return myMath::GetAngle(this->x,this->y,x,y);
}

double dPoint::GetKtInRadians(POINT MyPoint,bool fromTo){
	return GetKtInRadians(MyPoint.x,MyPoint.y,fromTo);
}

double dPoint::GetKtInRadians(dPoint MyPoint,bool fromTo){
	return GetKtInRadians(MyPoint.x,MyPoint.y,fromTo);
}

double dPoint::GetOdl(POINT MyPoint){
	return myMath::Dist(MyPoint.x,MyPoint.y,0,0);
}

double dPoint::GetOdl(dPoint MyPoint){
	return myMath::Dist(MyPoint.x,MyPoint.y,0,0);
}

double dPoint::GetOdl(double x,double y){
	return myMath::Dist(this->x,this->y,x,y);
}

POINT dPoint::GetPoint(void){
	POINT p;
	p.x=(int)x;
	p.y=(int)y;
	return p;
}
//Koniec klasy dPoint
//Pocz¹tek klasy dPoint3D
dPoint3D::dPoint3D(void){
	x=y=z=0;
}

dPoint3D::dPoint3D(POINT p){
	x=p.x;
	y=p.y;
	z=0;
}

dPoint3D::dPoint3D(double x,double y,double z){
	this->x=x;
	this->y=y;
	this->z=z;
}

dPoint3D::dPoint3D(dPoint p,double z){
	x=p.x;
	y=p.y;
	this->z=z;
}

dPoint3D::dPoint3D(dPoint p){
	x=p.x;
	y=p.y;
	z=0;
}

dPoint3D::dPoint3D(POINT p,double z){
	x=p.x;
	y=p.y;
	this->z=z;
}

void dPoint3D::SetPoint(POINT p){
	x=p.x;
	y=p.y;
}

void dPoint3D::SetPoint(dPoint3D p){
	x=p.x;
	y=p.y;
	y=p.y;
}

void dPoint3D::SetPoint(double x,double y,double z){
	this->x=x;
	this->y=y;
	this->z=z;
}

void dPoint3D::SetPoint(dPoint p,double z){
	x=p.x;
	y=p.y;
	this->z=z;
}

void dPoint3D::SetPoint(dPoint p){
	x=p.x;
	y=p.y;
}

void dPoint3D::SetPoint(POINT p,double z){
	x=p.x;
	y=p.y;
	this->z=z;
}
void dPoint3D::SetOffset(dPoint3D dp){
	x+=dp.x;
	y+=dp.y;
	z+=dp.z;
}
void dPoint3D::SetOffset(double dx,double dy,double dz){
	x+=dx;
	y+=dy;
	z+=dz;
}
double dPoint3D::GetOdl3D(dPoint3D p){
	return myMath::Dist3D(p.x,p.y,p.z,x,y,z);
}
double dPoint3D::GetOdl3D(double x,double y,double z){
	return myMath::Dist3D(x,y,z,this->x,this->y,this->z);
}
double dPoint3D::GetOdlXY(double x,double y){
	return myMath::Dist(x,y,this->x,this->y);
}
double dPoint3D::GetOdlXZ(double x,double z){
	return myMath::Dist(x,z,this->x,this->z);
}
double dPoint3D::GetOdlYZ(double y,double z){
	return myMath::Dist(y,z,this->y,this->z);
}
POINT dPoint3D::convertToPoint(int dx,int dy,double ktOsiX,double ktOsiY,double ktOsiZ,double xSkala,double zSkala,double ySkala){
	POINT p;
	p.x=dx+(int)(x*xSkala*cos(ktOsiX)+y*ySkala*cos(ktOsiY)+z*zSkala*cos(ktOsiZ));
	p.y=dy+(int)(x*xSkala*sin(ktOsiX)+y*ySkala*sin(ktOsiY)+z*zSkala*sin(ktOsiZ));
	return p;
}
//Koniec klasy dPoint3D
PointsClass::PointsClass(void)
{
}

PointsClass::~PointsClass(void)
{
}
