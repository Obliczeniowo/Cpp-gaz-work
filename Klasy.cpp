#include "Klasy.h"
extern ObsRys *RYS;

RECT wykres::GetAreaCsOtoczenia(void){
	int y=(int)(AreaW.bottom+Przes.y-csOtoczenia*(AreaW.bottom-AreaW.top-20)/yMax);
	return GetRect(AreaW.left-10,y-5,10,10);
}

void wykres::Timer(HWND hWnd){
	if(tlok->GetTimerOnOff()){
		SYSTEMTIME Czas;
		GetLocalTime(&Czas);
		int dt=Czas.wMilliseconds+Czas.wSecond*1000-tlok->GetCzasMS();
		if(dt<0)
			tlok->SetCzas();
		if(Size()>0 && dt>=(int)tlok->Get_uElapse()){
			tlok->SetCzas();
			n=(n+1)%Size();
			InvalidateRect(hWnd,NULL,false);
		}
		tlok->SetVtloka(GetPoint(n).x);
		tlok->SetCisnienieW_Tloku(GetPoint(n).y*100000);
	}
}

UINT wykres::Size(void){
	return ilP;
}

dPoint wykres::GetPoint(UINT nrP){
	dPoint p;
	p.x=p.y=0;
	if(ilP<=nrP)
		return p;
	return punkty[nrP];
}

double wykres::tempI(UINT NrPunktu){
	if(NrPunktu<=ilP){
		double temp=punkty[NrPunktu-1].x*punkty[NrPunktu-1].y/St;
		return temp;
	}else{
		return 0;
	}
}

bool wykres::SetSkokOnOff(void){
	if(!wykresOnOff)
		return wykresOnOff;
	skokOnOff=!skokOnOff;
	return skokOnOff;
}

bool wykres::SetPozListyLWab(int dp){
	if(wysRozwLegWab<10)
		return false;
	if(pozPoczRozwWab+dp+10>ilP){
		pozPoczRozwWab=ilP-10;
		return true;
	}
	if((int)(pozPoczRozwWab)+dp<1){
		pozPoczRozwWab=1;
		return true;
	}
	pozPoczRozwWab=pozPoczRozwWab+dp;
	return true;
}

bool wykres::SetPozListyLWt(int dp){
	if(wysRozwLegWt<10)
		return false;
	if(pozPoczRozwWt+dp+10>ilP){
		pozPoczRozwWt=ilP-10;
		return true;
	}else if((int)(pozPoczRozwWt)+dp<1){
		pozPoczRozwWt=1;
		return true;
	}
	pozPoczRozwWt=pozPoczRozwWt+dp;
	return true;
}

void wykres::PasekPrzew(HDC &hdc,int x,int y){
	RYS->frameRectangle(hdc,x,y,(UINT)11,(UINT)11,RGB(255,255,255),PS_SOLID,1);
	POINT p[]={x+5,y+2,x+2,y+8,x+8,y+8};
	RYS->polygon(hdc,p,3);
	y+=13;
	RYS->frameRectangle(hdc,x,y,(UINT)11,(UINT)11);
	p[0].x=x+2;p[0].y=y+2;p[1].x=x+8;p[1].y=y+2;p[2].x=x+5;p[2].y=y+11-3;
	RYS->polygon(hdc,p,3);
}

void wykres::SetUstLeg(void){
	if(ilP>2){
		if(ilP<11){
			wysRozwLegWab=wysRozwLegWt=ilP-1;
		}else{
			wysRozwLegWab=wysRozwLegWt=10;
		}
		pozPoczRozwWab=pozPoczRozwWt=1;
	}
}

//FUNKCJA ZAPISU DANYCH DO PLIKU
void wykres::SaveToFile(OPENFILENAME* ofn){
	ofn->Flags=OFN_OVERWRITEPROMPT;
	if(GetSaveFileName(ofn)){
		std::fstream plik(ofn->lpstrFile,std::ios::out);
		plik<<ilP<<"\r\n";
		for(UINT i=0;i<ilP;i++){
			plik<<punkty[i].x<<"\r\n";
			plik<<punkty[i].y<<"\r\n";
		}
		plik.close();
	}
}
//FUNKCJA ODCZYTU DANYCH Z PLIKU
bool wykres::OpenFile(OPENFILENAME *ofn){
	ofn->Flags=OFN_HIDEREADONLY|OFN_CREATEPROMPT;
	if(!GetOpenFileName(ofn))
		return false;
	std::fstream plik(ofn->lpstrFile,std::ios::in);
	char linia[101];
	plik.getline(linia,100);
	if(strlen(linia))
		if(atoi(linia)>0){
			ilP=(UINT)atoi(linia);
			if(ilP<100)
				tlok->Set_uElapse((UINT)(500.-(double)ilP*400/99));
			else
				tlok->Set_uElapse(100);
		}
		else
			return false;
	else
		return false;
	double _xMax=0,_yMax=0;
	if(punkty) delete[] punkty;
	punkty=new dPoint[ilP];
	for(UINT i=0;i<ilP;i++){
		plik.getline(linia,100);
		if(strlen(linia)){
			punkty[i].x=abs(atof(linia));
			if(punkty[i].x>_xMax)
				_xMax=punkty[i].x;
		}else
			punkty[i].x=0;
		plik.getline(linia,100);
		if(strlen(linia)){
			punkty[i].y=abs(atof(linia));
			if(punkty[i].y>_yMax)
				_yMax=punkty[i].y;
		}else
			punkty[i].y=0;		
	}
	if(_xMax>xMax)
		while(xMax<_xMax)
			xMax+=(double)(xG-1)/2;
	else
		while(xMax>_xMax+(double)(xG-1)/2)
			xMax-=(double)(xG-1)/2;
	if(_yMax>yMax)
		while(yMax<_yMax)
			yMax+=(double)(yG-1)/2;
	else
		while(yMax>_yMax+(double)(yG-1)/2)
			yMax-=(double)(yG-1)/2;
	plik.close();
	SetUstLeg();
	return true;
}

//FUNKCJE POBIERAJ¥CE
bool wykres::GetOznOnOff(void){
	return oznOnOff;//Oznaczenia wykresu
}

bool wykres::GetProwOnOff(void){
	return prowOnOff;//Prowadnice
}

bool wykres::GetSiatkaOnOff(void){
	return siatkaOnOff;
}

double wykres::GetXmax(void){
	return xMax;
}

double wykres::GetYmax(void){
	return yMax;
}

RECT wykres::GetRectLegendy(UINT nrFu,bool WabTrueWtFalse){
	RECT rect;
	if(nrFu>ilP-1)
		nrFu=ilP-1;
	if(WabTrueWtFalse){
		if(!wabRozw && (nrFu>=pozPoczRozwWab && nrFu<pozPoczRozwWab+wysRozwLegWab)){
			SetRect(&rect,PozLegendy.x+20,PozLegendy.y+((int)nrFu-(int)pozPoczRozwWab+1)*20,PozLegendy.x+220,PozLegendy.y+(nrFu+2-pozPoczRozwWab)*20);
			OffsetRect(&rect,Przes.x,Przes.y);
			return rect;
		}else{
			SetRect(&rect,PozLegendy.x+20,PozLegendy.y,PozLegendy.x+220,PozLegendy.y+20);
			OffsetRect(&rect,Przes.x,Przes.y);
			return rect;
		}
	}else{
		int dy;
		if(!wabRozw)
			dy=PozLegendy.y+(wysRozwLegWab+1)*20;
		else
			dy=PozLegendy.y+20;
		if(nrFu>=pozPoczRozwWt && nrFu<pozPoczRozwWt+wysRozwLegWt && !wtRozw)
			dy+=((int)nrFu-(int)pozPoczRozwWt+1)*20;

		SetRect(&rect,PozLegendy.x+20,dy,PozLegendy.x+220,dy+20);
		OffsetRect(&rect,Przes.x,Przes.y);
		return rect;
	}
}
//Pobie¿ prostok¹t pasków przewijania legendy
RECT wykres::GetRectPPL(bool WabTrueWtFalse,bool UpTrueDownFalse){
	RECT rect=GetRectWl(WabTrueWtFalse);
	if(UpTrueDownFalse){
		OffsetRect(&rect,-13,13);
		return rect;
	}
	OffsetRect(&rect,-13,0);
	return rect;
}

RECT wykres::GetRectWl(bool WabTrueWtFalse){
	RECT rect;
	if(WabTrueWtFalse){
		SetRect(&rect,PozLegendy.x+Przes.x+3,PozLegendy.y+Przes.y+3,PozLegendy.x+Przes.x+14,PozLegendy.y+Przes.y+14);
		return rect;
	}
	if(wabRozw){
		SetRect(&rect,PozLegendy.x+Przes.x+3,PozLegendy.y+Przes.y+23,PozLegendy.x+Przes.x+14,PozLegendy.y+Przes.y+34);
		return rect;
	}
	SetRect(&rect,PozLegendy.x+Przes.x+3,PozLegendy.y+Przes.y+(wysRozwLegWab+1)*20,PozLegendy.x+Przes.x+14,PozLegendy.y+Przes.y+14+(wysRozwLegWab+1)*20);
	return rect;
}
//FUNKCJA ZAPISU USTAWIEÑ G£ÓWNYCH WYKRESU DO PLIKU ini.ini
void wykres::WriteToFile(char *LokalizacjaINazwa){
	std::fstream plik(LokalizacjaINazwa,std::ios::out);
	plik<<(int)kOsiX<<"\r\n";
	plik<<(int)kOsiY<<"\r\n";
	plik<<(int)kSiatki<<"\r\n";
	plik<<(int)kProw<<"\r\n";
	plik<<(int)kWykr<<"\r\n";
	plik<<(int)kWykr2<<"\r\n";
	plik<<(int)kWab<<"\r\n";
	plik<<(int)kWt<<"\r\n";
	plik<<(int)kgLegendy<<"\r\n";
	plik<<(int)krLegendy<<"\r\n";
	plik<<(int)kCsOtoczenia<<"\r\n";
	plik<<csOtoczenia<<"\r\n";
	plik<<(int)siatkaOnOff<<"\r\n";
	plik<<(int)prowOnOff<<"\r\n";
	plik<<(int)oznOnOff<<"\r\n";
	plik<<(int)rysOnOff<<"\r\n";
	plik<<(int)skokOnOff<<"\r\n";
	plik.close();
}
//FUNKCJA ODCZYTU USTAWIEÑ G£ÓWNYCH WYKRESU Z PLIKU ini.ini
void wykres::ReadFromFile(char *LokalizacjaINazwa){
	std::fstream plik(LokalizacjaINazwa,std::ios::in);
	char linia[101];
	plik.getline(linia,100);
	if(strlen(linia))
		kOsiX=(COLORREF)atol(linia);
	plik.getline(linia,100);
	if(strlen(linia))
		kOsiY=(COLORREF)atol(linia);
	plik.getline(linia,100);
	if(strlen(linia))
		kSiatki=(COLORREF)atol(linia);
	plik.getline(linia,100);
	if(strlen(linia))
		kProw=(COLORREF)atol(linia);
	plik.getline(linia,100);
	if(strlen(linia))
		kWykr=(COLORREF)atol(linia);
	plik.getline(linia,100);
	if(strlen(linia))
		kWykr2=(COLORREF)atol(linia);
	plik.getline(linia,100);
	if(strlen(linia))
		kWab=(COLORREF)atol(linia);
	plik.getline(linia,100);
	if(strlen(linia))
		kWt=(COLORREF)atol(linia);
	plik.getline(linia,100);
	if(strlen(linia))
		kgLegendy=(COLORREF)atol(linia);
	plik.getline(linia,100);
	if(strlen(linia))
		krLegendy=(COLORREF)atol(linia);
	plik.getline(linia,100);
	if(strlen(linia))
		kCsOtoczenia=(COLORREF)atol(linia);
	plik.getline(linia,100);
	if(strlen(linia))
		wykres::SetCsOtoczenia(atof(linia));
	plik.getline(linia,100);
	if(strlen(linia))
		siatkaOnOff=atoi(linia)?true:false;
	plik.getline(linia,100);
	if(strlen(linia))
		prowOnOff=atoi(linia)?true:false;
	plik.getline(linia,100);
	if(strlen(linia))
		oznOnOff=atoi(linia)?true:false;
	plik.getline(linia,100);
	if(strlen(linia))
		rysOnOff=atoi(linia)?true:false;
	plik.getline(linia,100);
	if(strlen(linia))
		skokOnOff=atoi(linia)?true:false;
	plik.close();
}

bool wykres::ClearW(void){//Usuwa wszystkie punkty wykresu
	if(!wykresOnOff || !ilP)
		return wykresOnOff;
	delete[] punkty;
	punkty=NULL;
	ilP=0;
	return wykresOnOff;
}

//FUNKCJE USTWAWIAJ¥CE KOLORY
void wykres::SetkgLegendy(void){
	cc.rgbResult=kgLegendy;
	if(ChooseColor(&cc))
		kgLegendy=cc.rgbResult;	
}

void wykres::SetkrLegendy(void){
	cc.rgbResult=krLegendy;
	if(ChooseColor(&cc))
		krLegendy=cc.rgbResult;	
}

void wykres::SetkCsOtoczenia(void){
	cc.rgbResult=kCsOtoczenia;
	if(ChooseColor(&cc))
		kCsOtoczenia=cc.rgbResult;
}

void wykres::SetkWt(void){
	cc.rgbResult=kWt;
	if(ChooseColor(&cc))
		kWt=cc.rgbResult;
}

void wykres::SetkWab(void){
	cc.rgbResult=kWab;
	if(ChooseColor(&cc))
		kWab=cc.rgbResult;
}

void wykres::SetkOsiX(void){
	cc.rgbResult=kOsiX;
	if(ChooseColor(&cc))
		kOsiX=cc.rgbResult;
}
//Ustaw kolor osi X
void wykres::SetkOsiY(void){
	cc.rgbResult=kOsiY;
	if(ChooseColor(&cc))
		kOsiY=cc.rgbResult;
}
//Ustaw kolor siatki
void wykres::SetkSiatki(void){
	cc.rgbResult=kSiatki;
	if(ChooseColor(&cc))
		kSiatki=cc.rgbResult;
}
//Ustaw kolor wykresu
void wykres::SetkWykresu(void){
	cc.rgbResult=kWykr;
	if(ChooseColor(&cc))
		kWykr=cc.rgbResult;
}
//Ustaw kolor wykresu 2
void wykres::SetkWykresu2(void){
	cc.rgbResult=kWykr2;
	if(ChooseColor(&cc))
		kWykr2=cc.rgbResult;
}
//Funkcje ustawiaj¹ce opcje typu W£¥CZ/WY£¥CZ
//W£¥CZ/WY£¥CZ oznaczenia wykresu
bool wykres::SetOznOnOff(void){
	oznOnOff=!oznOnOff;
	return oznOnOff;
}
//W£¥CZ/WY£¥CZ siatkê
bool wykres::SetSiatkaOnOff(void){
	siatkaOnOff=!siatkaOnOff;
	return siatkaOnOff;
}
//W£¥CZ/WY£¥CZ prowadnice
bool wykres::SetProwOnOff(void){
	prowOnOff=!prowOnOff;
	return prowOnOff;
}

bool wykres::SetZaznPozLeg(POINT MousePos){
	if(!wabRozw){
		if(PtInRect(&GetRectLegendy(0,true),MousePos)){
				zaznPoz.nrFu=0;
				zaznPoz.WabTrueWtFalse=true;
				return true;
		}
		for(UINT i=pozPoczRozwWab;i<pozPoczRozwWab+wysRozwLegWab;i++)
			if(PtInRect(&GetRectLegendy(i,true),MousePos)){
				zaznPoz.nrFu=i;
				zaznPoz.WabTrueWtFalse=true;
				return true;
			}
	}
	if(!wtRozw){
		if(PtInRect(&GetRectLegendy(0,false),MousePos)){
				zaznPoz.nrFu=0;
				zaznPoz.WabTrueWtFalse=false;
				return true;
		}
		for(UINT i=pozPoczRozwWt;i<pozPoczRozwWt+wysRozwLegWt;i++)
			if(PtInRect(&GetRectLegendy(i,false),MousePos)){
				zaznPoz.nrFu=i;
				zaznPoz.WabTrueWtFalse=false;
				return true;
			}
	}
	if(PtInRect(&GetRectLegendy(0,true),MousePos)){
		zaznPoz.nrFu=0;
		zaznPoz.WabTrueWtFalse=true;
		return true;
	}
	if(PtInRect(&GetRectLegendy(0,false),MousePos)){
		zaznPoz.nrFu=0;
		zaznPoz.WabTrueWtFalse=false;
		return true;
	}
	zaznPoz.nrFu=-1;
	zaznPoz.WabTrueWtFalse=ZaznPoz.WabTrueWtFalse;
	return false;
}

bool wykres::RysujLegend(HDC &hdc){
	if(ilP<2)
		return false;
	POINT przes=PozLegendy;
	przes.x+=Przes.x;
	przes.y+=Przes.y;
	char W[20];
	char _W[200];
	_gcvt(Wab()*100000,10,W);
	if(W[strlen(W)-1]=='.')
		W[strlen(W)-1]=NULL;
	wsprintf(_W,"Wab%i-%i =%s[J]",1,ilP,W);
	if((!zaznPoz.nrFu && zaznPoz.WabTrueWtFalse) || (!ZaznPoz.nrFu && ZaznPoz.WabTrueWtFalse))
		RYS->textOut(hdc,przes.x+20,przes.y,_W,-1,kWab,TRANSPARENT);
	else
		RYS->textOut(hdc,przes.x+20,przes.y,_W,-1,kgLegendy,TRANSPARENT);
	PlusMinus(hdc,przes.x+3,przes.y+3,11,PS_SOLID,1,kgLegendy,wabRozw);
	int dy=20;
	if(!wabRozw){
		if(ilP>wysRozwLegWab+1)
			PasekPrzew(hdc,przes.x-11,przes.y+3);
		for(UINT i=pozPoczRozwWab;i<=wysRozwLegWab+pozPoczRozwWab-1;i++){
			_gcvt(Wab(i)*100000,10,W);
			if(W[strlen(W)-1]=='.')
				W[strlen(W)-1]=NULL;
			wsprintf(_W,"Wab%i-%i =%s[J]",i,i+1,W);
			if(zaznPoz.nrFu!=i && ZaznPoz.nrFu!=i || !zaznPoz.WabTrueWtFalse)
				RYS->textOut(hdc,przes.x+20,przes.y+dy,_W,-1,krLegendy,TRANSPARENT);
			else if(zaznPoz.WabTrueWtFalse || ZaznPoz.WabTrueWtFalse)
				RYS->textOut(hdc,przes.x+20,przes.y+dy,_W,-1,kWab,TRANSPARENT);
			RYS->line(hdc,przes.x+7,przes.y+dy+8,przes.x+13,przes.y+dy+8,krLegendy,PS_SOLID,1);
			dy+=20;
		}
		RYS->line(hdc,przes.x+7,przes.y+20,przes.x+7,przes.y+dy-11,krLegendy,PS_SOLID,1);
	}
	_gcvt(Wt()*100000,10,W);
	if(W[strlen(W)-1]=='.')
		W[strlen(W)-1]=NULL;
	wsprintf(_W,"Wt%i-%i =%s[J]",1,ilP,W);
	if((!zaznPoz.nrFu && !zaznPoz.WabTrueWtFalse) || (!ZaznPoz.nrFu && !ZaznPoz.WabTrueWtFalse))
		RYS->textOut(hdc,przes.x+20,przes.y+dy,_W,-1,kWt,TRANSPARENT);
	else
		RYS->textOut(hdc,przes.x+20,przes.y+dy,_W,-1,kgLegendy,TRANSPARENT);
	PlusMinus(hdc,przes.x+3,przes.y+3+dy,11,PS_SOLID,1,kgLegendy,wtRozw);
	if(!wtRozw){
		if(ilP>wysRozwLegWt+1)
			PasekPrzew(hdc,przes.x-11,przes.y+3+dy);
		dy+=20;
		for(UINT i=pozPoczRozwWt;i<=wysRozwLegWt+pozPoczRozwWt-1;i++){
			_gcvt(Wt(i)*100000,10,W);
			if(W[strlen(W)-1]=='.')
				W[strlen(W)-1]=NULL;
			wsprintf(_W,"Wt%i-%i =%s[J]",i,i+1,W);
			if(zaznPoz.nrFu!=i && ZaznPoz.nrFu!=i || zaznPoz.WabTrueWtFalse)
				RYS->textOut(hdc,przes.x+20,przes.y+dy,_W,-1,krLegendy,TRANSPARENT);
			else if(!zaznPoz.WabTrueWtFalse || !ZaznPoz.WabTrueWtFalse)
				RYS->textOut(hdc,przes.x+20,przes.y+dy,_W,-1,kWt,TRANSPARENT);
			RYS->line(hdc,przes.x+7,przes.y+dy+8,przes.x+13,przes.y+dy+8,krLegendy,PS_SOLID,1);
			dy+=20;
		}
		RYS->line(hdc,przes.x+7,przes.y+dy-(wysRozwLegWt)*20,przes.x+7,przes.y+dy-11,krLegendy,PS_SOLID,1);
	}
	return true;
}

bool wykres::RysujCsOtoczenia(HDC &hdc,POINT MousePos){
	int y=(int)(AreaW.bottom+Przes.y-csOtoczenia*(AreaW.bottom-AreaW.top-20)/yMax);
	RYS->line(hdc,AreaW.left,y,AreaW.right,y,kCsOtoczenia,PS_DOT,1);
	RYS->wielForemny(hdc,AreaW.left-5,y,3,5,0,kCsOtoczenia,PS_SOLID,1,kCsOtoczenia);
	return true;
}

bool wykres::RysujCsOtoczenia(PostScript *p){
	int y=(int)(AreaW.bottom+Przes.y-csOtoczenia*(AreaW.bottom-AreaW.top-20)/yMax);
	p->SetLineWidth(1);
	p->SetRGBColor(255,155,0);
	p->SetDash(2,2,0);
	p->lineA(AreaW.left,y,AreaW.right,y);
	p->wielForemny(AreaW.left-5,y,3,5,0,FILL);
	p->SetDash();
	return true;
}

bool wykres::RysujWabC(HDC &hdc){
	if(zaznPoz.nrFu!=0 || !zaznPoz.WabTrueWtFalse || ilP<2){
		if(ZaznPoz.nrFu!=0 || !ZaznPoz.WabTrueWtFalse || ilP<2 || zaznPoz.nrFu>0)		
			return false;
	}
	UINT ilPn=0;
	POINT *p;
	if(punkty[0].x==punkty[ilP-1].x){
		ilPn=ilP;
	}else{
		ilPn=ilP+2;
	}
	p=new POINT[ilPn];
	if(punkty[0].x!=punkty[ilP-1].x){
		p[ilPn-2].x=wykrToScreen(ilP,true);
		p[ilPn-2].y=GetAreaPrzes().bottom;
		p[ilPn-1].x=wykrToScreen(1,true);
		p[ilPn-1].y=GetAreaPrzes().bottom;
	}
	for(UINT i=0;i<ilP;i++){
		p[i].x=wykrToScreen(i+1,true);
		p[i].y=wykrToScreen(i+1,false);
	}
	HBRUSH hBrush=CreateSolidBrush(kWab);
	SelectObject(hdc,hBrush);
	SetROP2(hdc,R2_XORPEN);
	RYS->polygon(hdc,p,ilPn);
	SetROP2(hdc,R2_COPYPEN);
	DeleteObject(hBrush);
	delete[] p;
	return true;
}

bool wykres::RysujWtC(HDC &hdc){
	if(zaznPoz.nrFu!=0 || zaznPoz.WabTrueWtFalse || ilP<2){
		if(ZaznPoz.nrFu!=0 || ZaznPoz.WabTrueWtFalse ||	ilP<2 || zaznPoz.nrFu>0)
			return false;
	}
	UINT ilPn=0;
	POINT *p;
	if(punkty[0].y==punkty[ilP-1].y){
		ilPn=ilP;
	}else{
		ilPn=ilP+2;
	}
	p=new POINT[ilPn];
	if(punkty[0].y!=punkty[ilP-1].y){
		p[ilPn-2].y=wykrToScreen(ilP,false);
		p[ilPn-2].x=GetAreaPrzes().left;
		p[ilPn-1].y=wykrToScreen(1,false);
		p[ilPn-1].x=GetAreaPrzes().left;
	}
	for(UINT i=0;i<ilP;i++){
		p[i].x=wykrToScreen(i+1,true);
		p[i].y=wykrToScreen(i+1,false);
	}
	HBRUSH hBrush=CreateSolidBrush(kWt);
	SelectObject(hdc,hBrush);
	SetROP2(hdc,R2_XORPEN);
	RYS->polygon(hdc,p,ilPn);
	SetROP2(hdc,R2_COPYPEN);
	DeleteObject(hBrush);
	delete[] p;
	return true;
}

bool wykres::RysujWab(HDC &hdc){
	if(rysOnOff || 2>ilP || !zaznPoz.WabTrueWtFalse || zaznPoz.nrFu==-1){
		if(rysOnOff || 2>ilP || !ZaznPoz.WabTrueWtFalse || ZaznPoz.nrFu==-1)
            return false;
		HBRUSH hBrush=CreateSolidBrush(kWab);
		HPEN hPen=(HPEN)GetStockObject(NULL_PEN);
		SelectObject(hdc,hPen);
		SelectObject(hdc,hBrush);
		SetROP2(hdc,R2_XORPEN);
		if(ZaznPoz.nrFu>0)
			RysujWab(hdc,ZaznPoz.nrFu);
		else
			RysujWabC(hdc);
		DeleteObject(hBrush);
		DeleteObject(hPen);
		SetROP2(hdc,R2_COPYPEN);
		return true;
	}
	HBRUSH hBrush=CreateSolidBrush(kWab);
	HPEN hPen=(HPEN)GetStockObject(NULL_PEN);
	SelectObject(hdc,hPen);
	SelectObject(hdc,hBrush);
	SetROP2(hdc,R2_XORPEN);
	if(zaznPoz.nrFu>0)
		RysujWab(hdc,zaznPoz.nrFu);
	else
		RysujWabC(hdc);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	SetROP2(hdc,R2_COPYPEN);
	return true;
}

bool wykres::RysujWab(HDC &hdc,UINT nrFu){
	if(nrFu>ilP-1 || ilP<2 || !nrFu)
		return false;
	POINT Punkty[4]=	{wykrToScreen(nrFu,true),wykrToScreen(nrFu,false),
						wykrToScreen(nrFu+1,true),wykrToScreen(nrFu+1,false),
						wykrToScreen(nrFu+1,true),GetAreaPrzes().bottom,
						wykrToScreen(nrFu,true),GetAreaPrzes().bottom};
	RYS->polygon(hdc,Punkty,4);
	return true;
}

bool wykres::RysujWt(HDC &hdc,UINT nrFu){
	if(nrFu>ilP-1 || ilP<2 || !nrFu)
		return false;
	POINT Punkty[4]=	{wykrToScreen(nrFu,true),wykrToScreen(nrFu,false),
						wykrToScreen(nrFu+1,true),wykrToScreen(nrFu+1,false),
						GetAreaPrzes().left,wykrToScreen(nrFu+1,false),
						GetAreaPrzes().left,wykrToScreen(nrFu,false)};
	RYS->polygon(hdc,Punkty,4);
	return true;
}

bool wykres::RysujWt(HDC &hdc){
	if(rysOnOff || 2>ilP || zaznPoz.WabTrueWtFalse || zaznPoz.nrFu==-1){
		if(rysOnOff || 2>ilP ||ZaznPoz.WabTrueWtFalse || ZaznPoz.nrFu==-1)	
			return false;
		HBRUSH hBrush=CreateSolidBrush(kWt);
		HPEN hPen=(HPEN)GetStockObject(NULL_PEN);
		SelectObject(hdc,hPen);
		SelectObject(hdc,hBrush);
		SetROP2(hdc,R2_XORPEN);
		if(ZaznPoz.nrFu>0)
			RysujWt(hdc,ZaznPoz.nrFu);
		else
			RysujWtC(hdc);
		DeleteObject(hBrush);
		DeleteObject(hPen);
		SetROP2(hdc,R2_COPYPEN);
		return true;
	}
	HBRUSH hBrush=CreateSolidBrush(kWt);
	HPEN hPen=(HPEN)GetStockObject(NULL_PEN);
	SelectObject(hdc,hPen);
	SelectObject(hdc,hBrush);
	SetROP2(hdc,R2_XORPEN);
	if(zaznPoz.nrFu>0)
		RysujWt(hdc,zaznPoz.nrFu);
	else
		RysujWtC(hdc);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	SetROP2(hdc,R2_COPYPEN);
	return true;
}
bool wykres::RysujOzn(HDC &hdc){
	if(!oznOnOff)
		return oznOnOff;
	SetTextColor(hdc,RGB(255,255,255));
	for(UINT i=0;i<ilP;i++){
		RYS->textOut(hdc,wykrToScreen(i+1,true),wykrToScreen(i+1,false),(int)(i+1),10);
	}
	return oznOnOff;
}

bool wykres::Delete(void){
	if(!selectP || selectP>ilP || !wykresOnOff)
		return false;
	DeletePoint(selectP);
	selectP=0;
	SetUstLeg();
	return wykresOnOff;
}

RECT wykres::GetAreaPrzes(void){
	RECT rect;
	SetRect(&rect,AreaW.left+Przes.x,AreaW.top+Przes.y,AreaW.right+Przes.x,AreaW.bottom+Przes.y);
	return rect;
}

void wykres::vScroll(int dy){
	Przes.y=-dy*10;
}

double wykres::Wab(UINT nrFu){
	double a=0,b=0;
	if(nrFu>ilP-1 || nrFu==0)
		return 0;
	if(punkty[nrFu-1].x==punkty[nrFu].x)
		return 0;
    a=(punkty[nrFu-1].y-punkty[nrFu].y)/(punkty[nrFu-1].x-punkty[nrFu].x);
	b=punkty[nrFu].y-punkty[nrFu].x*a;
	return .5*a*pow(punkty[nrFu].x,2.0)+b*punkty[nrFu].x-.5*a*pow(punkty[nrFu-1].x,2.)-b*punkty[nrFu-1].x;
}

double wykres::Wab(void){
	double wab=0;
	if(ilP<2)
		return wab;
	for(UINT i=1;i<ilP;i++)
		wab+=Wab(i);
	return wab;
}

double wykres::Wt(UINT nrFu){
	double a=0,b=0;
	if(nrFu>ilP-1 || nrFu==0)
		return 0;
	if(punkty[nrFu-1].y==punkty[nrFu].y)
		return 0;
	a=(punkty[nrFu-1].x-punkty[nrFu].x)/(punkty[nrFu-1].y-punkty[nrFu].y);
	b=punkty[nrFu].x-a*punkty[nrFu].y;
	return .5*a*pow(punkty[nrFu].y,2.0)+b*punkty[nrFu].y-.5*a*pow(punkty[nrFu-1].y,2.)-b*punkty[nrFu-1].y;
}

double wykres::Wt(void){
	double wt=0;
	if(ilP<2)
		return wt;
	for(UINT i=1;i<ilP;i++){
		wt+=Wt(i);
	}
	return wt;
}

bool wykres::SetPozPunktu(POINT MousePos,UINT MK){
	MousePos.x-=Przes.x;
	MousePos.y-=Przes.y;
	if(!selectP || selectP>ilP || rysOnOff || MK!=MK_LBUTTON || !PtInRect(&AreaW,MousePos))
		return false;
	double dx=(double)(MousePos.x-AreaW.left)*xMax/(double)(AreaW.right-AreaW.left-20);
	double dy=(double)(AreaW.bottom-MousePos.y)*yMax/(double)(AreaW.bottom-AreaW.top-20);
	punkty[selectP-1].x=dx;
	punkty[selectP-1].y=dy;
	return true;
}

bool wykres::MouseMove(HWND hWnd,HINSTANCE hInst,POINT MousePos,UINT MK){
	if(csOtControl){
		SetCsOtoczenia((double)(AreaW.bottom-MousePos.y)*yMax/(double)(AreaW.bottom-AreaW.top-20));
	}
	if(PtInRect(&AreaW,MousePos)){
		if(!prowOnOff && idSelectedCursor!=idCrossCursor){
			SetClassLong(hWnd,GCL_HCURSOR,MAKELONG(LoadCursor(hInst,MAKEINTRESOURCE(idCrossCursor)),0));
			idSelectedCursor=idCrossCursor;
		}else if(prowOnOff && idSelectedCursor!=idEmptyCursor){
			SetClassLong(hWnd,GCL_HCURSOR,MAKELONG(LoadCursor(hInst,MAKEINTRESOURCE(idEmptyCursor)),0));
			idSelectedCursor=idEmptyCursor;
		}
	}else if(idSelectedCursor!=0){
		SetClassLong(hWnd,GCL_HCURSOR,MAKELONG(LoadCursor(NULL,IDC_ARROW),0));
		idSelectedCursor=0;
	}
	SetPozPunktu(MousePos,MK);
	SetZaznPozLeg(MousePos);
	if(!wykresOnOff || !PtInRect(&GetAreaPrzes(),MousePos) || MK==MK_LBUTTON)
		return wykresOnOff;
	for(UINT i=0;i<ilP;i++)
		if(myMath::Dist(MousePos.x,MousePos.y,wykrToScreen(i+1,true),wykrToScreen(i+1,false))<=10){
			selectP=i+1;
			return wykresOnOff;
		}
	selectP=0;
	return wykresOnOff;
}

void wykres::SetTitle(LPSTR title){
	if(Title)
		delete[] Title;
	Title=new char[strlen(title)+1];
	strcpy(Title,title);
}

bool wykres::SetRysOnOff(void){
	if(!wykresOnOff)
		return wykresOnOff;
	rysOnOff=!rysOnOff;
	return wykresOnOff;
}

int wykres::wykrToScreen(UINT nr,bool XtrueYfalse){
	if(nr>ilP){
		return 0;
	}
	//int x=(int)(punkty[nr-1].x*(AreaW.right-AreaW.left-20)/xMax+AreaW.left);
	//int y=(int)(AreaW.bottom+Przes.y-punkty[nr-1].y*(AreaW.bottom-AreaW.top-20)/yMax);
	if(XtrueYfalse)
		return (int)(punkty[nr-1].x*(AreaW.right-AreaW.left-20)/xMax+AreaW.left+Przes.x);
	return (int)(AreaW.bottom+Przes.y-punkty[nr-1].y*(AreaW.bottom-AreaW.top-20)/yMax);
}

bool wykres::lButtonDown(POINT MousePos){
	if(!wykresOnOff)
		return wykresOnOff;
	bool war=false;
	if(zaznPoz.nrFu>-1){
		war=true;
		if(ZaznPoz.nrFu!=zaznPoz.nrFu || ZaznPoz.WabTrueWtFalse!=zaznPoz.WabTrueWtFalse)
			ZaznPoz=zaznPoz;
		else
			ZaznPoz.nrFu=-1;
	}
	if(PtInRect(&GetRectPPL(true,true),MousePos)){
		wykres::SetPozListyLWab(3);
		return true;
	}else if(PtInRect(&GetRectPPL(true,false),MousePos)){
		wykres::SetPozListyLWab(-3);
		return true;
	}else if(PtInRect(&GetRectPPL(false,true),MousePos)){
		wykres::SetPozListyLWt(3);
		return true;
	}else if(PtInRect(&GetRectPPL(false,false),MousePos)){
		wykres::SetPozListyLWt(-3);
		return true;
	}else if(PtInRect(&GetAreaCsOtoczenia(),MousePos)){
		csOtControl=true;
		return true;
	}
	if(PtInRect(&GetRectWl(true),MousePos)){
		wabRozw=!wabRozw;
		war=true;
	}
	if(PtInRect(&GetRectWl(false),MousePos)){
		wtRozw=!wtRozw;
		war=true;
	}
	if(!PtInRect(&GetAreaPrzes(),MousePos) || !rysOnOff)
		return war;
	AddPoint(MousePos);
	return wykresOnOff;
}

bool wykres::lButtonUp(void){
	csOtControl=false;
	return csOtControl;
}

bool wykres::DeletePoint(UINT nr){
	if(nr>ilP || !ilP)
		return false;
	if(nr<ilP)
		for(UINT i=nr-1;i<ilP;i++)
			punkty[i]=punkty[i+1];
	if(ilP==1){
		delete[] punkty;
		punkty=NULL;
		ilP=0;
	}else{
		dPoint *Punkty=new dPoint[ilP-1];
		for(dPoint *p=Punkty;p<Punkty+ilP-1;p++)
			*p=*(punkty+(p-Punkty));
        delete[] punkty;
		ilP--;
		punkty=new dPoint[ilP];
		for(dPoint *p=punkty;p<punkty+ilP;p++)
			*p=*(Punkty+(p-punkty));
		delete[] Punkty;
	}
	SetUstLeg();
	if(ilP<100)
		tlok->Set_uElapse((UINT)(500.-(double)ilP*400/99));
	else
		tlok->Set_uElapse(100);
	return true;
}

void wykres::AddPoint(POINT MousePos){
	MousePos.x-=Przes.x;
	MousePos.y-=Przes.y;
	double dx=(double)(MousePos.x-AreaW.left)*xMax/(double)(AreaW.right-AreaW.left-20);
	double dy=(double)(AreaW.bottom-MousePos.y)*yMax/(double)(AreaW.bottom-AreaW.top-20);
	if(skokOnOff){
		for(UINT i=0;i<xG-1;i++){
			if((double)i*(xMax-xMin)/((double)xG-1)+xMin>dx){
				if(dx-((double)(i-1)*(xMax-xMin)/((double)xG-1)+xMin)<-dx+((double)i*(xMax-xMin)/((double)xG-1)+xMin)){
					dx=(double)(i-1)*(xMax-xMin)/((double)xG-1)+xMin;
				}else{
					dx=(double)i*(xMax-xMin)/((double)xG-1)+xMin;
				}
				break;
			}
		}
		for(UINT i=0;i<yG-1;i++){
			if((double)i*(yMax-yMin)/((double)yG-1)+yMin>dy){
				if(dy-((double)(i-1)*(yMax-yMin)/((double)yG-1)+yMin)<-dy+((double)i*(yMax-yMin)/((double)yG-1)+yMin)){
					dy=(double)(i-1)*(yMax-yMin)/((double)yG-1)+yMin;
				}else{
					dy=(double)i*(yMax-yMin)/((double)yG-1)+yMin;
				}
				break;
			}
		}
	}
	if(!ilP){
		ilP++;
		punkty=new dPoint;
		punkty->x=dx;
		punkty->y=dy;
	}else{
		dPoint *Punkty=new dPoint[ilP];
		for(dPoint *p=punkty;p<punkty+ilP;p++)
			*(Punkty+(p-punkty))=*p;
		delete[] punkty;
		punkty=new dPoint[ilP+1];
		for(dPoint *p=Punkty;p<Punkty+ilP;p++)
			*(punkty+(p-Punkty))=*p;
		delete[] Punkty;
		punkty[ilP].x=dx;
		punkty[ilP].y=dy;
		ilP++;
	}
	SetUstLeg();
	if(ilP<100)
		tlok->Set_uElapse((UINT)(500.-(double)ilP*400/99));
	else
		tlok->Set_uElapse(100);
}

void wykres::AddPoint(double x, double y){
	if(!ilP){
		ilP++;
		punkty = new dPoint;
		punkty->x = x;
		punkty->y = y;
	}else{
		dPoint *Punkty = new dPoint[ilP];
		for(dPoint *p = punkty; p < punkty + ilP; p++)
			*(Punkty + (p - punkty)) = *p;
		delete[] punkty;
		punkty=new dPoint[ilP + 1];
		for(dPoint *p = Punkty;p < Punkty + ilP; p++)
			*(punkty + (p - Punkty)) = *p;
		delete[] Punkty;
		punkty[ilP].x = x;
		punkty[ilP].y = y;
		ilP++;
	}
	SetUstLeg();
	if(ilP < 100)
		tlok->Set_uElapse((UINT)(500.-(double)ilP*400/99));
	else
		tlok->Set_uElapse(100);
}

bool wykres::RysujW(HDC &hdc,POINT MousePos){
	if(!ilP)
		return false;
	for(UINT i=1;i<ilP;i++){
		RYS->line(hdc,wykrToScreen(i,true),wykrToScreen(i,false),wykrToScreen(i+1,true),wykrToScreen(i+1,false),kWykr,PS_SOLID,2);
	}
	if(!PtInRect(&GetAreaPrzes(),MousePos) || !rysOnOff)
		return true;
	RYS->line(hdc,wykrToScreen(ilP,true),wykrToScreen(ilP,false),MousePos.x,MousePos.y,kWykr2,PS_SOLID,2);
	return true;
}
bool wykres::RysujW(PostScript *p){
	if(!ilP)
		return false;
	p->SetLineWidth(2);
	p->SetRGBColor(kWykr);
	for(UINT i=1;i<ilP;i++){
		p->lineA(wykrToScreen(i,true),wykrToScreen(i,false),wykrToScreen(i+1,true),wykrToScreen(i+1,false));
	}
	return true;
}

bool wykres::RysujProw(HDC &hdc,POINT MousePos){
	if(!prowOnOff || !PtInRect(&GetAreaPrzes(),MousePos))
		return prowOnOff;
	RYS->line(hdc,MousePos.x,AreaW.bottom+Przes.y,MousePos.x,AreaW.top+Przes.y,kProw,PS_DOT,1);
	RYS->line(hdc,AreaW.left,MousePos.y,AreaW.right+Przes.x,MousePos.y,kProw,PS_DOT,1);
	return prowOnOff;
}

void wykres::RysujPozMychy(HDC &hdc,POINT MousePos){
	if(PtInRect(&GetAreaPrzes(),MousePos)){
		double dx=(double)(MousePos.x-AreaW.left)*xMax/(double)(AreaW.right-AreaW.left-20);
		double dy=(double)(AreaW.bottom-MousePos.y)*yMax/(double)(AreaW.bottom-AreaW.top-20);
		RYS->textOut(hdc,MousePos.x+10,MousePos.y+10,dx,10,RGB(255,255,255),TRANSPARENT);
		RYS->textOut(hdc,MousePos.x+10,MousePos.y+30,dy,10);
	}
}

bool wykres::RysujSiatke(HDC &hdc,int dx,int dy){
	if(!siatkaOnOff)
		return siatkaOnOff;
	RYS->SetPen(hdc,kSiatki,PS_DOT,1);
	for(UINT i=1;i<xG;i++)
		RYS->line(hdc,AreaW.left+Przes.x+i*dx,AreaW.bottom+Przes.y,AreaW.left+Przes.x+i*dx,AreaW.top+Przes.y);
	for(UINT i=1;i<yG;i++)
		RYS->line(hdc,AreaW.left+Przes.x,AreaW.bottom+Przes.y-(int)i*dy,AreaW.right+Przes.x,AreaW.bottom+Przes.y-(int)i*dy);
	return siatkaOnOff;
}

bool wykres::RysujSiatke(PostScript *p,int dx,int dy){
	if(!siatkaOnOff)
		return siatkaOnOff;
	p->SetDash(2,2,0);
	p->SetRGBColor(kSiatki);
	for(UINT i=1;i<xG;i++)
		p->lineA(AreaW.left+Przes.x+i*dx,AreaW.bottom+Przes.y,AreaW.left+Przes.x+i*dx,AreaW.top+Przes.y);
	for(UINT i=1;i<yG;i++)
		p->lineA(AreaW.left+Przes.x,AreaW.bottom+Przes.y-(int)i*dy,AreaW.right+Przes.x,AreaW.bottom+Przes.y-(int)i*dy);
	p->SetDash();
	return siatkaOnOff;
}

int wykres::RysujXSkala(HDC &hdc){
	int dx=(AreaW.right-AreaW.left-20)/((int)xG-1);
	double _dx=(xMax-xMin)/((double)xG-1);
	RYS->SetPen(hdc,kOsiX,PS_SOLID,1);
	SetTextColor(hdc,kOsiX);
	for(UINT i=0;i<xG;i++){
		RYS->line(hdc,AreaW.left+Przes.x+i*dx,AreaW.bottom+Przes.y,AreaW.left+Przes.x+i*dx,AreaW.bottom+Przes.y+5);
		RYS->drawText(hdc,_dx*(double)i,10,AreaW.left+Przes.x-100+i*dx,AreaW.bottom+Przes.y+5,200,20,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}
	return dx;
}
int wykres::RysujXSkala(PostScript *p){
	int dx=(AreaW.right-AreaW.left-20)/((int)xG-1);
	double _dx=(xMax-xMin)/((double)xG-1);
	p->SetDash();
	p->SetRGBColor(kOsiX);
	for(UINT i=0;i<xG;i++){
		p->lineA(AreaW.left+Przes.x+i*dx,AreaW.bottom+Przes.y,AreaW.left+Przes.x+i*dx,AreaW.bottom+Przes.y+5);
	}
	p->translate(0,AreaW.bottom+Przes.y+(int)p->GetFontSize()+5);
	p->scale(1,-1);
	for(UINT i=0;i<xG;i++){
		p->showDrawText(_dx*(double)i,AreaW.left+Przes.x+i*dx,0,DT_CENTER);
	}
	p->scale(1,-1);
	p->translate(0,-(AreaW.bottom+Przes.y+(int)p->GetFontSize()+5));
	return dx;
}

int wykres::RysujYSkala(HDC &hdc){
	int dy=(AreaW.bottom-AreaW.top-20)/((int)yG-1);
	double _dy=(yMax-yMin)/((double)yG-1);
	RYS->SetPen(hdc,kOsiY,PS_SOLID,1);
	SetTextColor(hdc,kOsiY);
	for(UINT i=0;i<yG;i++){
		RYS->line(hdc,AreaW.left+Przes.x,AreaW.bottom+Przes.y-(int)i*dy,AreaW.left+Przes.x-5,AreaW.bottom+Przes.y-(int)i*dy);
		char bufor[MAX_PATH];
		sprintf_s(bufor,MAX_PATH,"%.3f",_dy*(double)i);
		RYS->drawText(hdc,bufor,strlen(bufor),AreaW.left+Przes.x-200,AreaW.bottom+Przes.y-(int)i*dy-10,190,20,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
	}	
	return dy;
}

int wykres::RysujYSkala(PostScript *p){
	int dy=(AreaW.bottom-AreaW.top-20)/((int)yG-1);
	double _dy=(yMax-yMin)/((double)yG-1);
	p->SetDash();
	p->SetRGBColor(kOsiY);
	for(UINT i=0;i<yG;i++){
		p->lineA(AreaW.left+Przes.x,AreaW.bottom+Przes.y-(int)i*dy,AreaW.left+Przes.x-5,AreaW.bottom+Przes.y-(int)i*dy);
		p->translate(0,AreaW.bottom+Przes.y-(int)i*dy+int(p->GetFontSize()/2));
		p->scale(1,-1);
		//char bufor[MAX_PATH];
		//sprintf_s(bufor,MAX_PATH,"%.3f",_dy*(double)i);
		p->showDrawText(_dy*(double)i,AreaW.left+Przes.x-10,0,DT_RIGHT);
		p->scale(1,-1);
		p->translate(0,-(AreaW.bottom+Przes.y-(int)i*dy+int(p->GetFontSize()/2)));
	}	
	return dy;
}

void wykres::Arrow(HDC &hdc,int x,int y,bool PionPoziom,COLORREF kWyp,int Styl,UINT gr,COLORREF kObr){
	RYS->SetPen(hdc,kObr,Styl,gr);
	RYS->SetBrush(hdc,kWyp);
	if(!PionPoziom){
		POINT p[]={x,y,x-10,y+4,x-10,y-4};
		RYS->polygon(hdc,p,3);
	}else{
		POINT p[]={x,y,x+4,y+10,x-4,y+10};
		RYS->polygon(hdc,p,3);
	}
}

void wykres::Arrow(PostScript *p,int x,int y,bool PionPoziom,COLORREF kWyp,int Styl,UINT gr,COLORREF kObr){
	p->SetRGBColor(kWyp);
	if(!PionPoziom){
		POINT pn[]={x,y,x-10,y+4,x-10,y-4};
		p->polyPolygon(pn,3,FILL);
	}else{
		POINT pn[]={x,y,x+4,y+10,x-4,y+10};
		p->polyPolygon(pn,3,FILL);
	}
}

void wykres::PlusMinus(HDC &hdc,int x,int y,int szerIwys,int Styl,int Gr,COLORREF Kolor,bool PtrueMfalse){
	RYS->frameRectangle(hdc,x,y,(UINT)szerIwys,(UINT)szerIwys,Kolor,Styl,Gr);
	RYS->line(hdc,x+2,y+szerIwys/2,x+szerIwys-2,y+szerIwys/2,Kolor,PS_SOLID,Gr);
	if(PtrueMfalse)
		RYS->line(hdc,x+szerIwys/2,y+2,x+szerIwys/2,y+szerIwys-2);
}

bool wykres::Rysuj(HDC &hdc,POINT MousePos,UINT MK){
	if(!wykresOnOff)
		return wykresOnOff;
	RysujSiatke(hdc,RysujXSkala(hdc),RysujYSkala(hdc));
	RYS->line(hdc,AreaW.left+Przes.x,AreaW.bottom+Przes.y,AreaW.right+Przes.x,AreaW.bottom+Przes.y,kOsiX,PS_SOLID,2);
	RYS->line(hdc,AreaW.left+Przes.x,AreaW.bottom+Przes.y,AreaW.left+Przes.x,AreaW.top+Przes.y,kOsiY,PS_SOLID,2);
	Arrow(hdc,AreaW.right+Przes.x,AreaW.bottom+Przes.y,false,kOsiX,PS_SOLID,1,kOsiX);
	Arrow(hdc,AreaW.left+Przes.x,AreaW.top+Przes.y,true,kOsiY,PS_SOLID,1,kOsiY);
	RysujCsOtoczenia(hdc,MousePos);
	SetTextColor(hdc,RGB(255,255,255));
	RYS->drawText(hdc,JednX,-1,AreaW.right+Przes.x,AreaW.bottom+Przes.y,(UINT)(strlen(JednX)*10),20,DT_SINGLELINE|DT_VCENTER);
	RYS->drawText(hdc,JednY,-1,AreaW.left+Przes.x-(int)strlen(JednY)*10-10,AreaW.top+Przes.y-10,(UINT)(strlen(JednY)*10),20,DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
	RYS->drawText(hdc,Title,-1,AreaW.left,AreaW.top+Przes.y-40,(UINT)(AreaW.right+Przes.x-AreaW.left),40,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	//MaskBmp(*hdc,PozLegendy.x-70,PozLegendy.y,140,141,hLogo,hLogoMask);
	RysujLegend(hdc);
	RysujWab(hdc);
	RysujWt(hdc);
	if(selectP>0 && ilP>=selectP)
		RYS->frameRectangle(hdc,wykrToScreen(selectP,true)-5,wykrToScreen(selectP,false)-5,10,10,RGB(255,255,0),PS_SOLID,2);
	RysujW(hdc,MousePos);
	RysujOzn(hdc);
	RysujProw(hdc,MousePos);
	RysujPozMychy(hdc,MousePos);
	if(rysOnOff){
		char tekst[]="Tryb rysowania (wciœnij ENTER aby uruchomiæ tryb graficznej modyfikacji)";
		RYS->textOut(hdc,AreaW.left+Przes.x,AreaW.bottom+50+Przes.y,tekst,-1,RGB(255,255,0),TRANSPARENT);
	}else{
		char tekst[]="Tryb graficznej modyfikacji (wciœnij ENTER aby uruchomiæ tryb rysowania)";
		RYS->textOut(hdc,AreaW.left+Przes.x,AreaW.bottom+50+Przes.y,tekst,-1,RGB(255,255,255),TRANSPARENT);
	}
	tlok->RysujTlok(hdc);
	if(ilP>0 && n<(int)ilP){
		POINT p;
		p.x=wykrToScreen(n+1,true);p.y=wykrToScreen(n+1,false);
		RYS->line(hdc,AreaW.left+Przes.x,p.y,p.x,p.y,kOsiY,PS_DOT,1);
		RYS->line(hdc,p.x,p.y,p.x,AreaW.bottom+Przes.x,kOsiX,PS_DOT,1);
		RYS->textOut(hdc,p.x,p.y,n+1,10,RGB(255,0,0),TRANSPARENT);
		RYS->textOut(hdc,AreaW.left+Przes.x,p.y,punkty[n].y,10,kOsiY,TRANSPARENT);
		RYS->textOut(hdc,p.x,AreaW.bottom-20,punkty[n].x,10,kOsiX,TRANSPARENT);
	}
	return wykresOnOff;
}

void wykres::SetCsOtoczenia(double CsOtoczenia){
	if(CsOtoczenia<0)
		CsOtoczenia=0;
	else if(CsOtoczenia>yMax)
		CsOtoczenia=yMax;
	csOtoczenia=CsOtoczenia;
	tlok->SetCisnienieOtoczenia(CsOtoczenia*100000);
}

void wykres::SetAreaW(int x,int y,UINT szer,UINT wys){
	SetRect(&AreaW,x,y,x+szer,y+wys);
}

void wykres::SetGx(UINT _xG){
	if(_xG>0)
		xG=_xG;
	else
		xG=11;
}

void wykres::SetGy(UINT _yG){
	if(_yG>0)
		yG=_yG;
	else
		yG=11;
}

void wykres::SetJedn(LPSTR jednX,LPSTR jednY){
	if(JednX)
		delete[] JednX;
	JednX=new char[strlen(jednX)+1];
	strcpy(JednX,jednX);
	if(JednY)
		delete[] JednY;
	JednY=new char[strlen(jednY)+1];
	strcpy(JednY,jednY);
}

void wykres::SetZakrY(double _yMin,double _yMax){
	if(_yMin>_yMax){
		yMin=_yMax;
		yMax=_yMin;
	}else{
		yMin=_yMin;
		yMax=_yMax;
	}
	for(UINT i=0;i<ilP;i++){
		if(punkty[i].y>yMax)
			yMax=punkty[i].y;
	}
	if(yMax<csOtoczenia){
		MessageBox(NULL,"Poniewa¿ wartoœæ ciœnienia otoczenia wykracza poza zakres wykresu zosta³o ono zmodyfikowane do najwiêkszej mo¿liwej wartoœci","Ostrze¿enie",MB_OK);
		csOtoczenia=yMax;
	}
}

void wykres::SetZakrX(double _xMin,double _xMax){
	if(_xMin>_xMax){
		xMin=_xMin;
		xMax=_xMax;
	}else{
		xMin=_xMin;
		xMax=_xMax;
	}
	for(UINT i=0;i<ilP;i++){
		if(punkty[i].x>xMax)
			xMax=punkty[i].x;
	}
	tlok->SetV(0,xMax);
}

wykres::wykres(int x,int y,UINT szer,UINT wys,LPSTR jednX,LPSTR jednY,LPSTR title,double _xMax,double _yMax,double _xMin,double _yMin,UINT _xG,UINT _yG,TIMERPROC timerProc,UINT idEvent,int IdCrossCursor,int IdEmptyCursor){
	ilP=0;
	St=.2;
	n=0;
	punkty=NULL;
	wykresOnOff=siatkaOnOff=prowOnOff=rysOnOff=oznOnOff=wtRozw=wabRozw=true;
	skokOnOff=false;
	JednX=JednY=Title=NULL;
	SetJedn(jednX,jednY);
	SetTitle(title);
	SetGx(_xG);
	SetGy(_yG);
	SetAreaW(x,y,szer,wys);	//Ustaw obszar wykresu
	tlok=new TlokSpr(AreaW.left,AreaW.bottom+130,0,0,GetXmax(),0.,50,AreaW.right-AreaW.left,timerProc,idEvent,ilP>0?punkty[n].y:0,0);
	SetZakrX(_xMin,_xMax);
	SetZakrY(_yMin,_yMax);
	kOsiX=RGB(255,255,255);
	kOsiY=RGB(255,255,255);
	kSiatki=RGB(150,150,150);
	kProw=RGB(0,150,200);
	kWykr=RGB(255,255,0);
	kWykr2=RGB(150,150,0);
	kWab=RGB(50,150,0);
	kWt=RGB(0,100,200);
	kgLegendy=RGB(255,255,255);
	krLegendy=RGB(150,150,150);
	selectP=0;
	Przes.x=Przes.y=0;
	PozLegendy.x=AreaW.right+40;
	PozLegendy.y=AreaW.top;
	ZaznPoz.nrFu=-1;
	cc.lStructSize=sizeof(CHOOSECOLOR);
	cc.hwndOwner=NULL;
	cc.hInstance=NULL;
	cc.rgbResult=RGB(80,80,80);
	cc.lpCustColors=CustColors;
	cc.lCustData=0;
	cc.lpfnHook=NULL;
	cc.lpTemplateName=NULL;
	cc.Flags=CC_RGBINIT|CC_FULLOPEN;
	wysRozwLegWab=wysRozwLegWt=10;
	pozPoczRozwWab=pozPoczRozwWt=1;
	idCrossCursor=IdCrossCursor;
	idEmptyCursor=IdEmptyCursor;
	idSelectedCursor=0;
}

bool wykres::SaveToPostScript(OPENFILENAME *opf){
	opf->Flags=OFN_OVERWRITEPROMPT;
	if(GetSaveFileName(opf)){
		PostScript *p=new PostScript(opf->lpstrFile);
		if(!wykresOnOff)
			return wykresOnOff;
		p->scale(1,-1);
		p->translate(0,-1100);
		RysujSiatke(p,RysujXSkala(p),RysujYSkala(p));
		p->SetRGBColor(kOsiX);
		p->SetLineWidth(2);
		p->lineA(AreaW.left+Przes.x,AreaW.bottom+Przes.y,AreaW.right+Przes.x-3,AreaW.bottom+Przes.y);
		p->SetRGBColor(kOsiY);
		p->SetLineWidth(2);
		p->lineA(AreaW.left+Przes.x,AreaW.bottom+Przes.y,AreaW.left+Przes.x,AreaW.top+Przes.y+3);
		p->translate(0,AreaW.bottom+Przes.y+30);
		p->scale(1,-1);
		p->SetRGBColor(0,0,0);
		p->showDateTimeNow(AreaW.left+Przes.x,0);
		char bufor[MAX_PATH];
		sprintf(bufor,"Wab= %f [J] Wt= %f [J]",Wab()*1e5,Wt()*1e5);
		p->showTextOut(bufor,AreaW.left+Przes.x+150,0);
		p->scale(1,-1);
		p->translate(0,-AreaW.bottom+Przes.y-30);
		Arrow(p,AreaW.right+Przes.x,AreaW.bottom+Przes.y,false,kOsiX,PS_SOLID,1,kOsiX);
		Arrow(p,AreaW.left+Przes.x,AreaW.top+Przes.y,true,kOsiY,PS_SOLID,1,kOsiY);
		RysujCsOtoczenia(p);
		p->SetRGBColor(0,0,0);
		p->translate(AreaW.right+Przes.x,AreaW.bottom+Przes.y);
		p->scale(1,-1);
		p->showDrawText(JednX,0,0,DT_LEFT);
		p->scale(1,-1);
		p->translate(-AreaW.right-Przes.x,-AreaW.bottom-Przes.y);
		p->translate(AreaW.left+Przes.x-10,AreaW.top+Przes.y-10);
		p->scale(1,-1);
		p->showDrawText(JednY,0,0,DT_RIGHT);
		p->scale(1,-1);
		p->translate(-AreaW.left-Przes.x+10,-AreaW.top-Przes.y+10);
		p->translate((AreaW.left+AreaW.right)/2,AreaW.top+Przes.y-40);
		p->scale(1,-1);
		p->showDrawText(Title,0,0,DT_CENTER);
		p->scale(1,-1);
		p->translate(-((AreaW.left+AreaW.right)/2),-(AreaW.top+Przes.y-40));
		//MaskBmp(*hdc,PozLegendy.x-70,PozLegendy.y,140,141,hLogo,hLogoMask);
		/*RysujLegend(hdc);
		RysujWab(hdc);
		RysujWt(hdc);
		if(selectP>0 && ilP>=selectP)
			RYS->frameRectangle(hdc,wykrToScreen(selectP,true)-5,wykrToScreen(selectP,false)-5,10,10,RGB(255,255,0),PS_SOLID,2);*/
		RysujW(p);
		/*RysujOzn(hdc);
		RysujProw(hdc,MousePos);
		RysujPozMychy(hdc,MousePos);
		if(rysOnOff){
			char tekst[]="Tryb rysowania (wciœnij ENTER aby uruchomiæ tryb graficznej modyfikacji)";
			RYS->textOut(hdc,AreaW.left+Przes.x,AreaW.bottom+50+Przes.y,tekst,-1,RGB(255,255,0),TRANSPARENT);
		}else{
			char tekst[]="Tryb graficznej modyfikacji (wciœnij ENTER aby uruchomiæ tryb rysowania)";
			RYS->textOut(hdc,AreaW.left+Przes.x,AreaW.bottom+50+Przes.y,tekst,-1,RGB(255,255,255),TRANSPARENT);
		}*/
		tlok->RysujTlok(p);
		if(ilP>0 && n<(int)ilP){
			POINT pk;
			pk.x=wykrToScreen(n+1,true);pk.y=wykrToScreen(n+1,false);
			p->SetLineWidth(1);
			p->SetRGBColor(kOsiY);
			p->translate(0,pk.y+10);
			p->scale(1,-1);
			p->showDrawText(punkty[n].y,AreaW.left+Przes.x+5,0,DT_LEFT);
			p->scale(1,-1);
			p->translate(0,-pk.y-10);
			p->SetDash(2,2,0);
			p->lineA(AreaW.left+Przes.x,pk.y,pk.x,pk.y);
			p->SetRGBColor(kOsiX);
			p->translate(0,AreaW.bottom-10);
			p->scale(1,-1);
			p->showDrawText(punkty[n].x,pk.x+5,0,DT_LEFT);
			p->scale(1,-1);
			p->translate(0,-(AreaW.bottom-10));
			p->lineA(pk.x,pk.y,pk.x,AreaW.bottom+Przes.x);
			p->SetRGBColor(255,0,0);
			p->translate(0,pk.y+10);
			p->scale(1,-1);
			p->showDrawText(n+1,pk.x+5,0,DT_LEFT);
			p->scale(1,-1);
			p->translate(0,-pk.y-10);
		}
		return wykresOnOff;
		delete p;
	}else
		return false;
}

wykres::wykres(int x,int y,UINT szer,UINT wys,LPSTR jednX,LPSTR jednY,LPSTR title,double _xMax,double _yMax,double _xMin,double _yMin,UINT _xG,UINT _yG,COLORREF _kOsiX,COLORREF _kOsiY,COLORREF _kSiatki,COLORREF _kProw,COLORREF _kWykr,COLORREF _kWykr2,TIMERPROC timerProc,UINT idEvent,int IdCrossCursor,int IdEmptyCursor){
	ilP=0;
	St=.2;
	n=0;
	punkty=NULL;
	wykresOnOff=siatkaOnOff=prowOnOff=rysOnOff=oznOnOff=wtRozw=wabRozw=true;
	skokOnOff=true;
	JednX=JednY=Title=NULL;
	SetJedn(jednX,jednY);
	SetTitle(title);
	SetGx(_xG);
	SetGy(_yG);
	SetAreaW(x,y,szer,wys);
	tlok=new TlokSpr(AreaW.left,AreaW.bottom+130,0,0,GetXmax(),0.,50,AreaW.right-AreaW.left,timerProc,idEvent,ilP>0?punkty[n].y:0,0);
	SetZakrX(_xMin,_xMax);
	SetZakrY(_yMin,_yMax);
	kOsiX=_kOsiX;
	kOsiY=_kOsiY;
	kSiatki=_kSiatki;
	kProw=_kProw;
	kWykr=_kWykr;
	kWykr2=_kWykr2;
	kWab=RGB(50,150,0);
	kWt=RGB(0,100,200);
	kgLegendy=RGB(255,255,255);
	krLegendy=RGB(150,150,150);
	kCsOtoczenia=RGB(255,255,0);
	selectP=0;
	Przes.x=Przes.y=0;
	PozLegendy.x=AreaW.right+40;
	PozLegendy.y=AreaW.top;
	ZaznPoz.nrFu=-1;
	cc.lStructSize=sizeof(CHOOSECOLOR);
	cc.hwndOwner=NULL;
	cc.hInstance=NULL;
	cc.rgbResult=RGB(80,80,80);
	cc.lpCustColors=CustColors;
	cc.lCustData=0;
	cc.lpfnHook=NULL;
	cc.lpTemplateName=NULL;
	cc.Flags=CC_RGBINIT|CC_FULLOPEN;
	wysRozwLegWab=wysRozwLegWt=10;
	pozPoczRozwWab=pozPoczRozwWt=1;
	idCrossCursor=IdCrossCursor;
	idEmptyCursor=IdEmptyCursor;
	idSelectedCursor=0;
}