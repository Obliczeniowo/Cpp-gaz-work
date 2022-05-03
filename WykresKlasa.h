#include <vector>
#include "SiatkaKlasa.h"

struct dPoint{double x,y;};

class Wykres{
	//Dane prywatne
	HWND hwnd;
	bool OnOffPoint;		//W³¹cz/Wy³¹cz rysowanie punktów wykresu
	bool OnOffProwadnice;	//W³¹cz/Wy³¹cz prowadnice
	int SelPoint;
	RECT pZazn;
	COLORREF kProstZazn;	//Kolor prostok¹ta zaznaczenia
	COLORREF kLiniWykresu;	//Kolor linii wykresu
	COLORREF kProwadnicy;	//Kolor prowadnicy
	CHOOSECOLOR cc;
	COLORREF ccKolory[16];
	std::vector <dPoint> Punkty;
	//Funkcje prywatne
	dPoint PozNaWykr(POINT MousePos);
	POINT PozNaEkranie(dPoint Punkt);
	RECT ObszWykresu(void);
	void RysujProwadnice(HDC &hdc,POINT MousePos);
public:
	Siatka *siatka;
	POINT pZacz;
	POINT pOds;
    HBITMAP hSiatka;
	Wykres(HWND hWnd,int xZ,int yZ,int xO,int yO,int OsX_Dl,int OsY_Dl);
	~Wykres();
	void Rysuj(HDC &hdc,POINT MousePos);
	int xPos(void);
	int yPos(void);
	void SetX_Dl(UINT dlugosc);
	void SetY_Dl(UINT dlugosc);
	void SetX_Y_Dl(UINT xDlugosc,UINT yDlugosc);
	void SetX_MinMax(double Min,double Max);
	void SetY_MinMax(double Min,double Max);
	void SetX_Label(LPSTR xLabel);
	void SetY_Label(LPSTR yLabel);
	void SetKLiniWykresu(COLORREF _kLiniWykresu);
	void SetKLiniWykresu(void);
	void SetKOsiX(void);
	void SetKOsiY(void);
	void SetKSiatki(void);
	void SetKolory(COLORREF kSiatki,COLORREF kOsiX,COLORREF kOsiY,COLORREF kLiniWykresu);
	void SetOnOffPoint(void);
	void SetOnOffPoint(bool onOffPoint);
	void SetPoint(UINT nrPunktu,double x,double y);
	void MouseMove(POINT MousePos);
	void lMouseButtonDown(POINT MousePos);
	void DeleteButtonDown(POINT MousePos);
	void AddPoint(double x,double y);
	void Przerysuj(bool Resize);
	void WyczyscWykres(void);
	void WczytajPunkty(dPoint* punkty,int iP);
};

void Wykres::AddPoint(double x,double y){
	dPoint p;
	p.x=x;p.y=y;
	Punkty.push_back(p);
	if(x>siatka->osX->GetMax())
		SetX_MinMax(siatka->osX->GetMin(),x);
	if(y>siatka->osY->GetMax())
		SetY_MinMax(siatka->osY->GetMin(),y);
}

void Wykres::SetPoint(UINT nrPunktu,double x,double y){
	UINT ilP=(UINT)Punkty.size();
	if(nrPunktu<=ilP && nrPunktu>0){
		Punkty.at(nrPunktu-1).x=x;
		Punkty.at(nrPunktu-1).y=y;
		if(x>siatka->osX->GetMax())
			SetX_MinMax(siatka->osX->GetMin(),x);
		if(y>siatka->osY->GetMax())
			SetY_MinMax(siatka->osY->GetMin(),y);
	}
}

void Wykres::SetOnOffPoint(void){
	OnOffPoint=!OnOffPoint;
	Przerysuj(false);
}

void Wykres::SetOnOffPoint(bool onOffPoint){
	OnOffPoint=onOffPoint;
	Przerysuj(false);
}

void Wykres::SetKolory(COLORREF kSiatki,COLORREF kOsiX,COLORREF kOsiY,COLORREF kLiniWykresu){
	siatka->SetKSiatki(kSiatki);
	siatka->SetKOsi(kOsiX,kOsiY);
	SetKLiniWykresu(kLiniWykresu);
	Przerysuj(false);
}

void Wykres::MouseMove(POINT MousePos){
	POINT p;
	RECT rect=pZazn;
	for(UINT i=0;i<Punkty.size();i++){
		p=PozNaEkranie(Punkty.at(i));
		OffsetRect(&rect,p.x,p.y);
		if(PtInRect(&rect,MousePos)){
			SelPoint=i;
			InvalidateRect(hwnd,NULL,false);
			break;
		}else{
			rect=pZazn;
			SelPoint=-1;
		}
	}
}

void Wykres::SetKSiatki(void){
	cc.rgbResult=siatka->GetKSiatki();
	if(ChooseColor(&cc))
		siatka->SetKSiatki(cc.rgbResult);
}

void Wykres::WczytajPunkty(dPoint* punkty,int iP){
	WyczyscWykres();
	double yMax,xMax;
	if(iP>0){
		yMax=punkty[0].y;
		xMax=punkty[0].x;
	}
    for(dPoint* i=punkty;i<punkty+iP;i++){
		Punkty.push_back(*i);
		if(yMax<i->y)
			yMax=i->y;
		if(xMax<i->x)
			xMax=i->x;
	}
	SetY_MinMax(siatka->osY->GetMin(),yMax);
	SetX_MinMax(siatka->osX->GetMin(),xMax);
}

void Wykres::SetKOsiY(void){
	cc.rgbResult=siatka->osY->GetKOsiY();
	if(ChooseColor(&cc))
		siatka->osY->SetKOsiY(cc.rgbResult);
}

void Wykres::SetKOsiX(void){
	cc.rgbResult=siatka->osX->GetKOsiX();
	if(ChooseColor(&cc))
		siatka->osX->SetKOsiX(cc.rgbResult);
	Przerysuj(false);
}

void Wykres::SetKLiniWykresu(void){
	cc.rgbResult=kLiniWykresu;
	if(ChooseColor(&cc))
		kLiniWykresu=cc.rgbResult;
	Przerysuj(false);
}

void Wykres::WyczyscWykres(void){
	Punkty.clear();
	Przerysuj(false);
}

void Wykres::SetX_Label(LPSTR xLabel){
	siatka->osX->SetTitle(xLabel);
	Przerysuj(false);
}

void Wykres::SetY_Label(LPSTR yLabel){
	siatka->osY->SetTitle(yLabel);
	Przerysuj(false);
}

void Wykres::DeleteButtonDown(POINT MousePos){
	if(SelPoint>-1 && SelPoint<(int)Punkty.size()){
		Punkty.erase(Punkty.begin()+SelPoint);
		Przerysuj(false);
		InvalidateRect(hwnd,NULL,false);
		SelPoint=-1;
	}
}

void Wykres::SetKLiniWykresu(COLORREF _kLiniWykresu){
	kLiniWykresu=_kLiniWykresu;
}

POINT Wykres::PozNaEkranie(dPoint Punkt){
	POINT punkt;
	Punkt.x-=siatka->osX->GetMin();
	Punkt.y-=siatka->osY->GetMin();
	Punkt.x=Punkt.x/siatka->xSkala();
	Punkt.y=Punkt.y/siatka->ySkala();
	punkt.x=xPos()+(int)Punkt.x;
	punkt.y=yPos()-(int)Punkt.y;
	return punkt;
}

void Wykres::SetX_MinMax(double Min,double Max){
	siatka->osX->SetMinMax(Min,Max);
	Przerysuj(false);
}

void Wykres::SetY_MinMax(double Min,double Max){
	siatka->osY->SetMinMax(Min,Max);
	Przerysuj(false);
}

dPoint Wykres::PozNaWykr(POINT MousePos){
	dPoint pozNaWykr;
	pozNaWykr.x=(double)(MousePos.x-xPos())*siatka->xSkala()+siatka->osX->GetMin();
	pozNaWykr.y=(double)(yPos()-MousePos.y)*siatka->ySkala()+siatka->osY->GetMin();
	return pozNaWykr;
}

RECT Wykres::ObszWykresu(void){
	RECT obsz;
	SetRect(&obsz,xPos(),yPos()-siatka->osY->GetDlugosc(),xPos()+siatka->osX->GetDlugosc(),yPos());
	return obsz;
}

void Wykres::lMouseButtonDown(POINT MousePos){
	if(PtInRect(&ObszWykresu(),MousePos)){
		Punkty.push_back(PozNaWykr(MousePos));
	}
}

void Wykres::SetX_Y_Dl(UINT xDlugosc,UINT yDlugosc){
	pZacz.y=pZacz.y-(int)siatka->osY->GetDlugosc()+yDlugosc;
	siatka->SetPZacz(siatka->osX->GetZX(),siatka->osY->GetZY()-siatka->osY->GetDlugosc()+yDlugosc);
	siatka->osY->SetDlugosc(yDlugosc);
	siatka->osX->SetDlugosc(xDlugosc);
	Przerysuj(true);
}

void Wykres::SetY_Dl(UINT dlugosc){
	pZacz.y=pZacz.y-(int)siatka->osY->GetDlugosc()+dlugosc;
	siatka->SetPZacz(siatka->osX->GetZX(),siatka->osY->GetZY()-siatka->osY->GetDlugosc()+dlugosc);
	siatka->osY->SetDlugosc(dlugosc);
	Przerysuj(true);
}

void Wykres::SetX_Dl(UINT dlugosc){
	siatka->osX->SetDlugosc(dlugosc);
	Przerysuj(true);
}

void Wykres::Przerysuj(bool Resize){
	HDC hdc=CreateCompatibleDC(GetDC(hwnd));
	RECT size=siatka->Size();
	if(Resize){
		DeleteObject(hSiatka);
		hSiatka=CreateCompatibleBitmap(GetDC(hwnd),size.right,size.bottom);
	}
	SelectObject(hdc,hSiatka);
	Rectangle(hdc,0,0,size.right,size.bottom,RGB(0,0,0),RGB(0,0,0),PS_SOLID,1);
	siatka->Rysuj(hdc);
	DeleteDC(hdc);
}

int Wykres::yPos(void){
	return pZacz.y+pOds.y;
}

int Wykres::xPos(void){
	return pZacz.x+pOds.x;
}

void Wykres::RysujProwadnice(HDC &hdc,POINT MousePos){
	RECT rect=ObszWykresu();
	if(PtInRect(&rect,MousePos)){
		SetBkMode(hdc,TRANSPARENT);
		Linia(&hdc,rect.left,MousePos.y,rect.right,MousePos.y,kProwadnicy,PS_DOT,1);
		Linia(&hdc,MousePos.x,rect.top,MousePos.x,rect.bottom,kProwadnicy,PS_DOT,1);
	}
}

void Wykres::Rysuj(HDC &hdc,POINT MousePos){
	HDC hDC=CreateCompatibleDC(hdc);
	SelectObject(hDC,hSiatka);
	RECT size=siatka->Size();
	BitBlt(hdc,xPos()-200,yPos()-siatka->osY->GetDlugosc(),size.right,size.bottom,hDC,0,0,SRCCOPY);
	DeleteDC(hDC);
	int s=(int)Punkty.size();
	for(int i=0;i<s-1;i++){
		POINT wsp=PozNaEkranie(Punkty.at(i));
		POINT wsp2=PozNaEkranie(Punkty.at(i+1));
		Linia(&hdc,wsp.x,wsp.y,wsp2.x,wsp2.y,kLiniWykresu,PS_SOLID,2);
	}
	for(int i=0;i<s;i++){
		POINT wsp=PozNaEkranie(Punkty.at(i));
		TextOut(&hdc,wsp.x,wsp.y,i+1);
	}
	RysujProwadnice(hdc,MousePos);
	if(OnOffPoint)
        for(int i=0;i<s;i++){
			POINT wsp=PozNaEkranie(Punkty.at(i));
			Linia(&hdc,wsp.x,wsp.y-5,wsp.x,wsp.y+5,RGB(255,0,0),PS_SOLID,1);
			Linia(&hdc,wsp.x-5,wsp.y,wsp.x+5,wsp.y,RGB(0,255,0),PS_SOLID,1);
		}
		if(PtInRect(&ObszWykresu(),MousePos)){
			dPoint p=PozNaWykr(MousePos);
			char bufor[20];
			SetBkMode(hdc,TRANSPARENT);
			SetTextColor(hdc,RGB(255,255,255));
			_gcvt(p.x,10,bufor);
			TextOut(hdc,MousePos.x+20,MousePos.y+20,bufor,(int)strlen(bufor));
			_gcvt(p.y,10,bufor);
			TextOut(hdc,MousePos.x+20,MousePos.y+40,bufor,(int)strlen(bufor));
		}
		if(SelPoint>-1 && SelPoint<(int)Punkty.size()){
			RECT rect=pZazn;
			POINT p=PozNaEkranie(Punkty.at(SelPoint));
			OffsetRect(&rect,p.x,p.y);
			Rectangle(hdc,rect,kProstZazn,PS_SOLID,2);
		}
}

Wykres::Wykres(HWND hWnd,int xZ,int yZ,int xO,int yO,int OsX_Dl,int OsY_Dl){
	siatka=new Siatka(RGB(100,100,100),PS_DOT,1,200,OsY_Dl,0,0,OsX_Dl,OsY_Dl,RGB(255,0,0),RGB(0,255,0),0,10,0,10,true);
	SelPoint=-1;
	pZacz.x=xZ;
	pZacz.y=yZ;
	pOds.x=xO;
	pOds.y=yO;
	hwnd=hWnd;
	RECT size=siatka->Size();
	HDC hdc=CreateCompatibleDC(GetDC(hwnd));
	hSiatka=CreateCompatibleBitmap(GetDC(hwnd),size.right,size.bottom);
	SelectObject(hdc,hSiatka);
	siatka->Rysuj(hdc);
	DeleteDC(hdc);
	OnOffPoint=true;
	kProstZazn=RGB(255,255,0);
	kProwadnicy=RGB(0,200,255);
	OnOffProwadnice=true;
	SetRect(&pZazn,-5,-5,5,5);
	cc.lStructSize=sizeof(CHOOSECOLOR);
	cc.hwndOwner=NULL;
	cc.hInstance=NULL;
	cc.rgbResult=RGB(255,255,255);
	cc.lpCustColors=ccKolory;
	cc.Flags=CC_RGBINIT|CC_FULLOPEN;
	cc.lCustData=0;
	cc.lpfnHook=NULL;
	cc.lpTemplateName=NULL;
	kLiniWykresu=RGB(200,200,200);
	for(int i=0;i<16;i++)
		ccKolory[i]=RGB(i*15,i*15,i*15);
}

Wykres::~Wykres(){
}