#include "OsXKlasa.h"
#include "OsYKlasa.h"

//Klasa Siatki
class Siatka{
	COLORREF kSiatki;
	int styl;
	int gr;
	bool OnOffSiatka;
public:
	OsY *osY;
	OsX *osX;
	Siatka(OsX *_osX,OsY *_osY,COLORREF _kSiatki,int Styl,int Gr);
	Siatka(COLORREF _kSiatki,int Styl,int Gr,int xZ,int yZ,int xO,int yO,UINT OsX_Dl,UINT OsY_Dl,COLORREF kOsiX,COLORREF kOsiY,double xMin,double xMax,double yMin,double yMax,bool onOffSiatka);
	void Rysuj(HDC &hdc);
	void SetPZacz(int zX,int zY);
	RECT Size(void);
	double xSkala(void);
	double ySkala(void);
	COLORREF GetKSiatki(void);
	void SetKSiatki(COLORREF _kSiatki);
	void SetKOsi(COLORREF kOsiX,COLORREF kOsiY);
};

void Siatka::SetKOsi(COLORREF kOsiX,COLORREF kOsiY){
	osX->SetKOsiX(kOsiX);
	osY->SetKOsiY(kOsiY);
}

COLORREF Siatka::GetKSiatki(void){
	return kSiatki;
}

void Siatka::SetKSiatki(COLORREF _kSiatki){
	kSiatki=_kSiatki;
}

double Siatka::ySkala(void){
	return (osY->GetMax()-osY->GetMin())/((double)osY->GetDlugosc()-15);
}

double Siatka::xSkala(void){
	return (osX->GetMax()-osX->GetMin())/((double)osX->GetDlugosc()-15);
}

void Siatka::SetPZacz(int zX,int zY){
	osX->SetPZacz(zX,zY);
	osY->SetPZacz(zX,zY);
}

RECT Siatka::Size(void){
	RECT size;
	SetRect(&size,0,0,200+osX->GetDlugosc(),50+osY->GetDlugosc());
	return size;
}

void Siatka::Rysuj(HDC &hdc){
	if(OnOffSiatka){
		SetBkMode(hdc,TRANSPARENT);
		osX->RysujSiatke(hdc,kSiatki,osY->GetDlugosc(),styl,gr);
		osY->RysujSiatke(hdc,kSiatki,osX->GetDlugosc(),styl,gr);
	}
	osX->RysujOsX(hdc);
	osY->RysujOsY(hdc);
}

Siatka::Siatka(OsX *_osX,OsY *_osY,COLORREF _kSiatki,int Styl,int Gr){
	osX=_osX;
	osY=_osY;
	kSiatki=_kSiatki;
	styl=Styl;
	gr=Gr;
	OnOffSiatka=true;
}

Siatka::Siatka(COLORREF _kSiatki,int Styl,int Gr,int xZ,int yZ,int xO,int yO,UINT OsX_Dl,UINT OsY_Dl,COLORREF kOsiX,COLORREF kOsiY,double xMin,double xMax,double yMin,double yMax,bool onOffSiatka){
	osX=new OsX(xZ,yZ,xO,yO,OsX_Dl,kOsiX,xMin,xMax);
	osY=new OsY(xZ,yZ,xO,yO,OsY_Dl,kOsiY,yMin,yMax);
	OnOffSiatka=onOffSiatka;
	styl=Styl;
	gr=Gr;
    kSiatki=_kSiatki;    
}