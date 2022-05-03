#include "FunkcjeLiczace.h"

double Obliczenia::GetPI(void){
	return acos(0.)*2;
}

double Obliczenia::RadiansToDegrees(double ktInRadians){
	return ktInRadians*90/acos(0.);
}

double Obliczenia::DegreesToRadians(double ktInDegrees){
	return ktInDegrees*acos(0.)/90;
}

double Obliczenia::Odl3D(double x1,double y1,double z1,double x2,double y2,double z2){
	return pow(pow(x1-x2,2.)+pow(y1-y2,2.)+pow(z1-z2,2.),.5);
}

int Obliczenia::Zaokr(double w){
	double w2=w-(int)w;
	int a=(int)w;
	if(abs(w2)>.5)
		if(w2>0)
			return (int)w+1;
		else
			return (int)w-1;
	else
		return (int)w;
}

double Obliczenia::Zaokr(double w,UINT nrPoPrzecinku){
	w=Zaokr(w*pow(10.,(double)nrPoPrzecinku));
	w=w/pow(10.,(double)nrPoPrzecinku);
	return w;
}

POINT Obliczenia::RotatePoint(int xObracany,int yObracany,int xObrotu,int yObrotu,double Angle){
	double R=Odl(xObracany,yObracany,xObrotu,yObrotu);
	double dx=xObracany-xObrotu;
	double dy=yObracany-yObrotu;
	POINT RotatePt;
	RotatePt.x=int(xObrotu+dx*cos(Angle)-dy*sin(Angle));
	RotatePt.y=int(yObrotu+dx*sin(Angle)+dy*cos(Angle));
	return RotatePt;
}

double Obliczenia::RayOfArc(int x1,int y1,int x2,int y2,int x3,int y3){
	double l1=Odl(x1,y1,x2,y2),l2=Odl(x1,y1,x3,y3),l3=Odl(x2,y2,x3,y3);
	double alfa,beta;
	alfa= acos((pow(l1,2.) + pow(l2,2.) - pow(l3,2.)) / (2 * l1 * l2));
	beta=4*asin(1.)-2*alfa;
	return pow(pow(l3,2.) / (2 - 2 * cos(beta)),0.5);
}

double Obliczenia::RayOfArc(POINT p1,POINT p2,POINT p3){
	double l1=Odl(p1,p2),l2=Odl(p1,p3),l3=Odl(p2,p3);
	double alfa,beta;
	alfa= acos((pow(l1,2.) + pow(l2,2.) - pow(l3,2.)) / (2 * l1 * l2));
	beta=4*asin(1.)-2*alfa;
	return pow(pow(l3,2.) / (2 - 2 * cos(beta)),0.5);
}

Obliczenia::Obliczenia(void){
}
//Funkcja licz¹ca odleg³oœæ miêdzy dwiema wspó³¿êdnymi
double Obliczenia::Odl(double x1,double y1,double x2,double y2){
	return pow(pow((double)(x1-x2),2.0)+pow((double)(y1-y2),2.0),.5);
}

double Obliczenia::Odl(POINT p1,POINT p2){
	return Odl(p1.x,p1.y,p2.x,p2.y);
}
//Funkcje licz¹ca k¹t zawarty miêdzy dwoma punktami
double Obliczenia::KatPt(int x1,int y1,int x2,int y2){
	if(x1==x2 && y1==y2)
		return 0;
	double L=pow(pow((double)(x1-x2),2.)+pow((double)(y1-y2),2.),0.5);
	double PI=asin(1.0)*2;
	if(y2<y1)
		return acos((x1-x2)/L);
	else
		return acos((x1-x2)/-L)+PI;
}

double Obliczenia::KatPt(double x1,double y1,double x2,double y2){
	if(x1==x2 && y1==y2)
		return 0;
	double L=pow(pow(x1-x2,2.)+pow(y1-y2,2.),0.5);
	double PI=asin(1.0)*2;
	if(y2<y1)
		return acos((x1-x2)/L);
	else
		return acos((x1-x2)/-L)+PI;
}

double Obliczenia::KatPt(POINT p1,POINT p2){
	return KatPt(p1.x,p1.y,p2.x,p2.y);
}

void  Obliczenia::SetKat3D(double katXY,double katXwyp,double &ktOsiX,double &ktOsiY,double &ktOsiZ,double &xSkala,double &ySkala,double &zSkala){
	double l=100;
	double przek=l*pow(2.0,.5);
	POINT pX,pY,pZ;
	double PI=GetPI();
	pY.x=(int)(l*cos(katXwyp)*(cos(katXY)*cos(katXY))+l*sin(katXwyp)*cos(katXY+PI/2));
	pY.y=(int)(l*cos(katXwyp)*(sin(katXY)*cos(katXY))+l*sin(katXwyp)*sin(katXY+PI/2));
	pX.x=(int)(l*cos(katXwyp+PI/2)*(cos(katXY)*cos(katXY))+l*sin(katXwyp+PI/2)*cos(katXY+PI/2));
	pX.y=(int)(l*cos(katXwyp+PI/2)*(sin(katXY)*cos(katXY))+l*sin(katXwyp+PI/2)*sin(katXY+PI/2));
	int dx=(int)(l*cos(PI/2-katXY)*cos(katXY)),dy=(int)(l*cos(PI/2-katXY)*sin(katXY));
	pZ.x=-dx;
	pZ.y=-dy;
	ktOsiX=KatPt(pX.x,pX.y,0,0);
	ktOsiY=KatPt(pY.x,pY.y,0,0);
	ktOsiZ=KatPt(pZ.x,pZ.y,0,0);
	xSkala=Odl(pX.x,pX.y,0,0)/l;
	ySkala=Odl(pY.x,pY.y,0,0)/l;
	zSkala=Odl(pZ.x,pZ.y,0,0)/l;
	/*if(abs(ktGl2.GetAngleInRadians())>=PI)
		ktKor=PI*0.5-ktOsiZ;
	else
		ktKor=PI*1.5-ktOsiZ;*/
}

void StartupFile(LPSTR &startupFile,LPSTR startupFileName){
	char *path=GetCommandLine();
	char *p=strrchr(path, '\\');
	*p=NULL;
	if(path[0]==34)
		path++;
	strcat(path,startupFileName);
	startupFile=new char[strlen(path)+1];
	strcpy(startupFile,path);
}

void StartupPath(LPSTR &startupPath){
	char *path=GetCommandLine();
	char *p=strrchr(path,'\\');
	*p=NULL;
	if(path[0]==34)
		path++;
	startupPath=new char[strlen(path)+1];
	strcpy(startupPath,path);
}

LPSTR Right(LPSTR text,UINT nr){
	char *txt;
	UINT size=(UINT)strlen(text);
	if(!size)
		return NULL;
	txt=new char[strlen(text)+1];
	strcpy(txt,text);
	if((int)size-(int)nr<=0)
		return txt;
	return txt+(size-nr);
}

LPSTR Left(LPSTR text,UINT nr){
	char *txt;
	UINT size=(UINT)strlen(text);
	if(!size)
		return NULL;
	txt=new char[strlen(text)+1];
	strcpy(txt,text);
	if(nr>=size)
		return txt;
	char *p=text;
	p+=nr;
	*(txt+(p-text))=NULL;
	return txt;
}

LPSTR WytnijTekst(LPSTR text,UINT nrPocz,UINT szer){
	return Right(Left(text,nrPocz+szer),szer);
}

LPSTR ZastTekst(LPSTR text,LPSTR textZ,UINT nrPocz,UINT szer){
	char *txt;
	UINT tSize=(UINT)strlen(text);
	UINT tzSize=(UINT)strlen(textZ);
	if(!tSize || !tzSize || nrPocz+szer>=tSize)
		return text;
	txt=Left(text,nrPocz);
    strcat(txt,textZ);
	strcat(txt,Right(text,tSize-nrPocz-szer));
	return txt;
}

LPSTR FindX(LPSTR fu,double x){
	char xStr[20];
	_gcvt(x,10,xStr);
	UINT size=(UINT)strlen(fu);
	if(!size)
		return fu;
	for(UINT i=0;i<size-1;i++){
		if(strcmp(WytnijTekst(fu,i,1),"x")==0){
			fu=ZastTekst(fu,xStr,i,1);
			size=(UINT)strlen(fu);
		}
	}
	return fu;
}

int SprNaw(LPSTR fu){
	int ilN=0;
	UINT size=(UINT)strlen(fu);
	if(!size) return 0;
	for(char* p=fu;p<fu+size;p++){
		if(*p=='(')
			ilN++;
		else if(*p==')'){
			ilN--;
			if(ilN<0)
				return ilN;
		}
	}
	return ilN;
}

void FindNaw(LPSTR fu,UINT &pocz,UINT &kon){
	pocz=kon=0;
	int ilN=0;
	for(char *p=fu;p<fu+strlen(fu);p++){
		if(*p=='('){
			ilN++;
			if(ilN==1)
				pocz=(UINT)(p-fu);
		}else if(*p==')'){
			ilN--;
			if(ilN==0){
				kon=(UINT)(p-fu);			
				break;
			}
		}
	}
}

/*double FindFu(LPSTR fu){
	for(char *p=fu+1;p<strlen(fu);p++){
		if(*p=='('){
		}
	}
}*/

bool IsNumeric(LPSTR liczba){
	double d=atof(liczba);
	if(!d && strcmp(WytnijTekst(liczba,0,1),"0")){
		return false;
	}else
		return true;
}

double Liczba(LPSTR fu,UINT p){
	UINT size=(UINT)strlen(fu);
	if(!fu) return 0;
	UINT i=1;
	for(i=1;i<size-p;i++){
		if(!IsNumeric(WytnijTekst(fu,p,i))){
			if(strcmp(WytnijTekst(fu,p+i-1,1),"e"))
				break;
		}
	}
    return atof(WytnijTekst(fu,p+i-1,1));
}

double LiczFu(LPSTR fu,double x){
	fu=FindX(fu,x);
	if(!SprNaw(fu)) return 0;
	UINT poczN,konN;
    FindNaw(fu,poczN,konN);
	char liczba[20];
	while(!poczN && !konN){
		if((int)poczN-3>=0){
			if(strcmp(WytnijTekst(fu,poczN-3,3),"sin")==0){
				_gcvt(sin(LiczFu(WytnijTekst(fu,poczN+1,konN-poczN-1),x)),10,liczba);
				ZastTekst(fu,liczba,poczN,konN-poczN+1);
			}else if(strcmp(WytnijTekst(fu,poczN-3,3),"cos")==0){
				_gcvt(cos(LiczFu(WytnijTekst(fu,poczN+1,konN-poczN-1),x)),10,liczba);
				ZastTekst(fu,liczba,poczN,konN-poczN+1);
			}else if(strcmp(WytnijTekst(fu,poczN-3,3),"tan")==0){
				_gcvt(tan(LiczFu(WytnijTekst(fu,poczN+1,konN-poczN-1),x)),10,liczba);
				ZastTekst(fu,liczba,poczN,konN-poczN+1);
			}else if(!strcmp(WytnijTekst(fu,poczN-1,1),"*") || !strcmp(WytnijTekst(fu,poczN-1,1),"/") || !strcmp(WytnijTekst(fu,poczN-1,1),"^") || !strcmp(WytnijTekst(fu,poczN-1,1),"+") || !strcmp(WytnijTekst(fu,poczN-1,1),"-")){
				_gcvt(LiczFu(WytnijTekst(fu,poczN+1,konN-poczN-1),x),10,liczba);
				ZastTekst(fu,liczba,poczN,konN-poczN+1);
			}
		}
	}	
	return 0;
}