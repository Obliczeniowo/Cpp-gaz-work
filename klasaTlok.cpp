#include <windows.h>
#include <math.h>
#include <stdlib.h>

#include "ObsRysowania.h"
extern ObsRys *RYS;
#include "klasaTlok.h"

void TlokSpr::SetCisnienieOtoczenia(double CsOtoczenia){
	csOtoczenia=CsOtoczenia;
}

void TlokSpr::Set_uElapse(UINT _uElapse){
	if(_uElapse<100)
		uElapse=100;
	else if(_uElapse>1000)
		uElapse=1000;
	else
		uElapse=_uElapse;

}

UINT TlokSpr::Get_uElapse(){
	return uElapse;
}

void TlokSpr::SetCzas(void){
	GetLocalTime(&czas);
}

int TlokSpr::GetCzasMS(void){
	return czas.wMilliseconds+czas.wSecond*1000;
}

void TlokSpr::SetCisnienieW_Tloku(double CsW_Tloku){
	csW_Tloku=CsW_Tloku;
}

double TlokSpr::SilaGazu(void){
	double silaOtoczeniaNaTlok=csOtoczenia*(2*asin(1.)*pow((double)Ray/100,2.0));
	double silaGazuNaTlok=csW_Tloku*(2*asin(1.)*pow((double)Ray/100,2.0));
	if(GetVtloka()!=0)
		return silaGazuNaTlok-silaOtoczeniaNaTlok;
	else
		return 0;
}

bool TlokSpr::GetTimerOnOff(void){
	return timerOnOff;
}

bool TlokSpr::StartTimer(HWND hWnd){
	SetTimer(hWnd,idEvent,uElapse,TimerProc);
	GetLocalTime(&czas);
	timerOnOff=true;
	return timerOnOff;
}

bool TlokSpr::StopTimer(HWND hWnd){
	KillTimer(hWnd,idEvent);
	timerOnOff=false;
	return timerOnOff;
}

void TlokSpr::SetOdsTloka(int OdsTloka){
	if(OdsTloka<-1)
		OdsTloka=0;
	if(OdsTloka>odsMaxTloka-odsMinTloka)
		odsTloka=odsMaxTloka-odsMinTloka;
	else
		odsTloka=OdsTloka;
	UINT k=(UINT)(155./(odsMaxTloka-odsMinTloka)*odsTloka);
	kGazu=RGB((UINT)(205-(int)k),0,0);
}

RECT TlokSpr::GetObszTloka(void){
	return GetRect(xPos()+odsTloka+odsMinTloka+20,yPos()-(int)Ray/6,dlTloka,Ray/3);
}

void TlokSpr::lButtonDown(POINT MousePos){
	lastMousePos=MousePos;
}

void TlokSpr::MouseMove(POINT MousePos,UINT MK,HWND hWnd){
	if(PtInRect(&GetObszTloka(),MousePos) && MK==MK_LBUTTON){	
		SetOdsTloka(odsTloka+MousePos.x-lastMousePos.x);
		lastMousePos=MousePos;
		InvalidateRect(hWnd,NULL,false);
	}
}

double TlokSpr::GetVtloka(void){
	return Vmax*(double)odsTloka/odsMaxTloka+Vmin;
}

void TlokSpr::SetVtloka(double Vtloka){
	SetOdsTloka((int)(odsMaxTloka*Vtloka/Vmax-odsMinTloka));
	if(odsTloka<0)
		SetOdsTloka(odsTloka);
	if(odsTloka>odsMaxTloka-odsMinTloka)
		SetOdsTloka(odsMaxTloka-odsMinTloka);
}

void TlokSpr::SetDlTloka(UINT _dlTloka){
	if(_dlTloka<200)
		dlTloka=200;
	else
		dlTloka=_dlTloka;
	odsMaxTloka=dlTloka-20;
	odsMinTloka=(int)(odsMaxTloka*Vmin/Vmax);
}

void TlokSpr::SetV(double vMin,double vMax){
	vMin=abs(vMin);
	vMax=abs(vMax);
	if(vMin>vMax){
		Vmin=vMax;
		Vmax=vMin;
	}else{
		Vmin=vMin;
		Vmax=vMax;
	}
	if(Vmin<0)
		Vmin=0;
	odsMinTloka=(int)(odsMaxTloka*Vmin/Vmax);
}

TlokSpr::TlokSpr(int xZ,int yZ,int xO,int yO,double vMax,double vMin,UINT ray,UINT DlTloka,TIMERPROC timerProc,UINT IDevent,double CsW_Tloku,double CsOtoczenia){
	pZacz.x=xZ;pZacz.y=yZ;
	pOds.x=xO;pOds.y=yO;
	Ray=ray;
	kTloka=RGB(255,255,255);
	kCylindra=RGB(200,200,200);
	kOdsMin=RGB(200,100,0);
	kOdsMax=RGB(0,100,200);
	SetDlTloka(DlTloka);
	SetV(vMin,vMax);
	SetOdsTloka(0);
	TimerProc=timerProc;
	idEvent=IDevent;
	uElapse=100;
	timerOnOff=false;
	csW_Tloku=CsW_Tloku;
	csOtoczenia=CsOtoczenia;
}

int TlokSpr::xPos(void){
	return pZacz.x+pOds.x;
}

int TlokSpr::yPos(void){
	return pZacz.y+pOds.y;
}

void TlokSpr::RysujTlok(HDC &hdc){
	int x=xPos(),y=yPos();
	SetBkMode(hdc,TRANSPARENT);
	int rop=SetROP2(hdc,R2_XORPEN);
	RYS->rectangle(hdc,x+2,y-(int)Ray+2,odsTloka+odsMinTloka,(UINT)(2*Ray-4),kGazu,PS_SOLID,1,kGazu);
	SetROP2(hdc,rop);
	RYS->line(hdc,x+dlTloka,y-(int)Ray,x,y-(int)Ray,kCylindra,PS_SOLID,2);
	RYS->line(hdc,x,y-(int)Ray,x,y+(int)Ray);
	RYS->line(hdc,x+dlTloka,y+(int)Ray,x,y+(int)Ray);
	RYS->line(hdc,x+odsMinTloka,y-(int)Ray,x+odsMinTloka,y+(int)Ray,kOdsMin,PS_DOT,1);
	RYS->line(hdc,x+odsMaxTloka,y-(int)Ray,x+odsMaxTloka,y+(int)Ray,kOdsMax,PS_DOT,1);
	RYS->fillRectangle(hdc,x+odsTloka+odsMinTloka,y+2-(int)Ray,20,(UINT)(2*Ray-4),kTloka);
	RYS->fillRectangle(hdc,GetObszTloka(),kCylindra);
	RYS->line(hdc,x+dlTloka+2,y-(int)Ray/6-2,x+dlTloka+20,y-(int)Ray/6-2,kTloka,PS_SOLID,2);
	RYS->line(hdc,x+dlTloka+2,y+(int)Ray/6+2,x+dlTloka+20,y+(int)Ray/6+2);
	double fG=SilaGazu();
	if(GetVtloka()!=0){
		if(fG>0)
			RYS->Wektor(hdc,x+odsTloka+odsMinTloka,y,x+odsTloka+odsMinTloka-50,y,false,RGB(0,255,0),PS_SOLID,1,RGB(0,255,0));
		else if(fG<0)
			RYS->Wektor(hdc,x+odsTloka+odsMinTloka+20,y,x+odsTloka+odsMinTloka+70,y,false,RGB(0,255,0),PS_SOLID,1,RGB(0,255,0));
	}
	char liczba[20];
	if(GetVtloka()>0)
		_gcvt(SilaGazu(),10,liczba);
	else
		_gcvt(0.,10,liczba);
	if(liczba[strlen(liczba)-1]=='.')
		liczba[strlen(liczba)-1]=NULL;
	char F[]="F=";
	char jF[]=" [N]";
    char *tekst=new char[strlen(liczba)+1+strlen(F)+strlen(jF)];
	wsprintf(tekst,"%s%s%s",F,liczba,jF);
	RYS->textOut(hdc,x,y+Ray+10,tekst,-1,RGB(0,255,0),TRANSPARENT);
	delete[] tekst;
	char Pg[]="Pg=";
	char jPg[]=" [Pa]";
	_gcvt(TlokSpr::csW_Tloku,10,liczba);
	if(liczba[strlen(liczba)-1]=='.')
		liczba[strlen(liczba)-1]=NULL;
	tekst=new char[strlen(liczba)+1+strlen(Pg)+strlen(jPg)];
	wsprintf(tekst,"%s%s%s",Pg,liczba,jPg);
	RYS->textOut(hdc,x+150,y+Ray+10,tekst,-1,RGB(255,0,0),TRANSPARENT);
	delete[] tekst;
	char Pot[]="Pot=";
	char jPot[]=" [Pa]";
	_gcvt(csOtoczenia,10,liczba);
	if(liczba[strlen(liczba)-1]=='.')
		liczba[strlen(liczba)-1]=NULL;
	tekst=new char[strlen(liczba)+1+strlen(Pot)+strlen(jPot)];
	wsprintf(tekst,"%s%s%s",Pot,liczba,jPot);
	RYS->textOut(hdc,x+300,y+Ray+10,tekst,-1,RGB(255,255,0),TRANSPARENT);
	delete[] tekst;
}

void TlokSpr::RysujTlok(PostScript *p){
	int x=xPos(),y=yPos();
	p->SetRGBColor(kGazu);
	p->rectangle(x+2,y-(int)Ray+2,(UINT)(odsTloka+odsMinTloka),(UINT)(2*Ray-4),FILL);
	p->SetRGBColor(kCylindra);
	p->SetLineWidth(2);
	p->lineA(x+dlTloka,y-(int)Ray,x,y-(int)Ray);
	p->lineA(x,y-(int)Ray,x,y+(int)Ray);
	p->lineA(x+dlTloka,y+(int)Ray,x,y+(int)Ray);
	p->SetRGBColor(kOdsMin);
	p->SetDash(2,2,0);
	p->SetLineWidth(1);
	p->lineA(x+odsMinTloka,y-(int)Ray,x+odsMinTloka,y+(int)Ray);
	p->lineA(x+odsMaxTloka,y-(int)Ray,x+odsMaxTloka,y+(int)Ray);
	p->SetRGBColor(0,0,0);
	p->rectangle(x+odsTloka+odsMinTloka,y+2-(int)Ray,(UINT)20,(UINT)(2*Ray-4),FILL);
	p->SetRGBColor(kCylindra);
	p->rectangle(GetObszTloka(),FILL);
	p->SetRGBColor(kTloka);
	p->SetLineWidth(2);
	p->lineA(x+dlTloka+2,y-(int)Ray/6-2,x+dlTloka+20,y-(int)Ray/6-2);
	p->lineA(x+dlTloka+2,y+(int)Ray/6+2,x+dlTloka+20,y+(int)Ray/6+2);
	double fG=SilaGazu();
	/*if(GetVtloka()!=0){
		if(fG>0)
			RYS->Wektor(hdc,x+odsTloka+odsMinTloka,y,x+odsTloka+odsMinTloka-50,y,false,RGB(0,255,0),PS_SOLID,1,RGB(0,255,0));
		else if(fG<0)
			RYS->Wektor(hdc,x+odsTloka+odsMinTloka+20,y,x+odsTloka+odsMinTloka+70,y,false,RGB(0,255,0),PS_SOLID,1,RGB(0,255,0));
	}*/
	char liczba[20];
	if(GetVtloka()>0)
		_gcvt(SilaGazu(),10,liczba);
	else
		_gcvt(0.,10,liczba);
	if(liczba[strlen(liczba)-1]=='.')
		liczba[strlen(liczba)-1]=NULL;
	char F[]="F=";
	char jF[]=" [N]";
    char *tekst=new char[strlen(liczba)+1+strlen(F)+strlen(jF)];
	wsprintf(tekst,"%s%s%s",F,liczba,jF);
	p->SetRGBColor(0,0,0);
	p->translate(0,y+Ray+20);
	p->scale(1,-1);
	p->showTextOut(tekst,x,0);
	delete[] tekst;
	char Pg[]="Pg=";
	char jPg[]=" [Pa]";
	_gcvt(TlokSpr::csW_Tloku,10,liczba);
	if(liczba[strlen(liczba)-1]=='.')
		liczba[strlen(liczba)-1]=NULL;
	tekst=new char[strlen(liczba)+1+strlen(Pg)+strlen(jPg)];
	wsprintf(tekst,"%s%s%s",Pg,liczba,jPg);
	p->showTextOut(tekst,x+150,0);
	delete[] tekst;
	char Pot[]="Pot=";
	char jPot[]=" [Pa]";
	_gcvt(csOtoczenia,10,liczba);
	if(liczba[strlen(liczba)-1]=='.')
		liczba[strlen(liczba)-1]=NULL;
	tekst=new char[strlen(liczba)+1+strlen(Pot)+strlen(jPot)];
	wsprintf(tekst,"%s%s%s",Pot,liczba,jPot);
	p->showTextOut(tekst,x+300,0);
	p->scale(1,-1);
	p->translate(0,-int(y+Ray+20));
	delete[] tekst;
}