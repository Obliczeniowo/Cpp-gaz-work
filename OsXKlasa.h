//Klasa osi X
class OsX{
	UINT gSkali;
	double min;
	double max;
	char *title;
	COLORREF kOsiX;//kolor osi
public:
	POINT pOds;//Odsuniêcie osi
	POINT pZacz;//Punt zaczepienia osi
	UINT Dlugosc;//D³ugoœæ osi
	double Krok();
	void SetMinMax(double Min,double Max);
	void SetG_Skali(UINT _gSkali);
	int xPos(void);	//pozycja osi na X-sie
	int yPos(void);	//pozycja osi na Y-ku
	OsX(int zX,int zY,int oX,int oY,UINT dlugosc,COLORREF KOsiX,double Min,double Max);
	OsX();
	~OsX();
	void RysujOsX(HDC &hdc);
	void RysujSiatke(HDC &hdc,COLORREF kSiatki,int _dSiatki,int Styl,int Gr);
	void SetDlugosc(UINT dlugosc);
	void SetPOds(int oX,int oY);
	void SetTitle(char *Title);
	void SetPZacz(int zX,int zY);
	void SetKOsiX(COLORREF _kOsiX);
	UINT GetG_Skali(void);
	double GetMin(void);
	double GetMax(void);
	UINT GetDlugosc(void);
	int GetZX(void);
	int GetZY(void);
	COLORREF GetKOsiX(void);
};

void OsX::SetKOsiX(COLORREF _kOsiX){
	kOsiX=_kOsiX;
}

COLORREF OsX::GetKOsiX(void){
	return kOsiX;
}

int OsX::GetZX(void){
	return pZacz.x;
}

int OsX::GetZY(void){
	return pZacz.y;
}

void OsX::SetPZacz(int zX,int zY){
	pZacz.x=zX;
	pZacz.y=zY;
}

void OsX::SetDlugosc(UINT dlugosc){
	if(dlugosc<100)
		Dlugosc=100;
	else
		Dlugosc=dlugosc;
}

void OsX::SetTitle(char *Title){
	if(title) delete[] title;
	title=new char[strlen(Title)+1];
	strcpy(title,Title);
}

OsX::OsX(){
	title=NULL;
}

UINT OsX::GetDlugosc(void){
	return Dlugosc;
}

void OsX::RysujSiatke(HDC &hdc,COLORREF kSiatki,int _dSiatki,int Styl,int Gr){
	double Skok=(double)(Dlugosc-15)/(double)(gSkali-1);
	for(UINT i=0;i<gSkali;i++){
		Linia(&hdc,xPos()+(int)(i*Skok),yPos(),xPos()+(int)(i*Skok),yPos()-_dSiatki,kSiatki,Styl,Gr);
	}
}

UINT OsX::GetG_Skali(void){
	return gSkali;
}

double OsX::GetMax(void){
	return max;
}

double OsX::GetMin(void){
	return min;
}

void OsX::SetG_Skali(UINT _gSkali){
	if(_gSkali<2)
		gSkali=2;
	else
		gSkali=_gSkali;
}

void OsX::SetMinMax(double Min,double Max){
	if(Min>Max){
		min=Max;
		max=Min;
	}else if(Min<Max){
		min=Min;
		max=Max;
	}
}

double OsX::Krok(){
	return (max-min)/(double)(gSkali-1);
}

int OsX::xPos(void){
	return pZacz.x+pOds.x;
}

int OsX::yPos(void){
	return pZacz.y+pOds.y;
}

void OsX::RysujOsX(HDC &hdc){
	Linia(&hdc,xPos(),yPos(),xPos()+Dlugosc,yPos(),kOsiX,PS_SOLID,1);
	POINT pArrow[]={xPos()+Dlugosc,yPos(),xPos()+Dlugosc-10,yPos()-5,xPos()+Dlugosc-10,yPos()+5};
	Polygon(hdc,pArrow,3);
	double Skok=(double)(Dlugosc-15)/(double)(gSkali-1);
	double SkokSkali=Krok();
	SetTextColor(hdc,kOsiX);
	SetBkMode(hdc,TRANSPARENT);
	RECT rect;
	char bufor[20];
	for(UINT i=0;i<gSkali;i++){
		Linia(&hdc,xPos()+(int)(i*Skok),yPos(),xPos()+(int)(i*Skok),yPos()+5,kOsiX,PS_SOLID,1);
		SetRect(&rect,(int)(i*Skok)+xPos()-100,yPos()+5,(int)(i*Skok)+xPos()+100,yPos()+25);
		_gcvt(min+i*SkokSkali,10,bufor);
		if(bufor[strlen(bufor)-1]=='.')
			bufor[strlen(bufor)-1]=NULL;
        DrawText(hdc,bufor,-1,&rect,DT_CENTER|DT_SINGLELINE);
	}
	SetRect(&rect,xPos(),yPos()+30,xPos()+Dlugosc,yPos()+50);
	DrawText(hdc,title,-1,&rect,DT_SINGLELINE|DT_CENTER);
}

OsX::OsX(int zX,int zY,int oX,int oY,UINT dlugosc,COLORREF KOsiX,double Min,double Max){
	pOds.x=oX;
	pOds.y=oY;
	pZacz.x=zX;
	pZacz.y=zY;
	Dlugosc=dlugosc;
	kOsiX=KOsiX;
	SetMinMax(Min,Max);
	gSkali=11;
	title=NULL;
	SetTitle("X");
}

OsX::~OsX(){
}