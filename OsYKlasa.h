//Klasa osi Y
class OsY{
	UINT gSkali;
	double min;
	double max;
	char* title;
public:
	POINT pOds;//Odsuniêcie osi
	POINT pZacz;//Punt zaczepienia osi
	UINT Dlugosc;//D³ugoœæ osi
	COLORREF kOsiY;//kolor osi
	double Krok();
	void SetMinMax(double Min,double Max);
	void SetG_Skali(UINT _gSkali);
	int xPos(void);	//pozycja osi na X-sie
	int yPos(void);	//pozycja osi na Y-ku
	OsY(int zX,int zY,int oX,int oY,UINT dlugosc,COLORREF KOsiY,double Min,double Max);
	OsY();
	~OsY();
	void RysujOsY(HDC &hdc);
	void RysujSiatke(HDC &hdc,COLORREF kSiatki,int _dSiatki,int Styl,int Gr);
	void SetDlugosc(UINT dlugosc);
	void SetPOds(int oX,int oY);
	void SetPZacz(int zX,int zY);
	void SetTitle(char *Title);
	void SetKOsiY(COLORREF _kOsiY);
	UINT GetG_Skali(void);
	double GetMin(void);
	double GetMax(void);
	UINT GetDlugosc(void);
	int GetZX(void);
	int GetZY(void);
	COLORREF GetKOsiY(void);
};

void OsY::SetKOsiY(COLORREF _kOsiY){
	kOsiY=_kOsiY;
}

COLORREF OsY::GetKOsiY(void){
	return kOsiY;
}

int OsY::GetZX(void){
	return pZacz.x;
}

int OsY::GetZY(void){
	return pZacz.y;
}

void OsY::SetPZacz(int zX,int zY){
	pZacz.x=zX;
	pZacz.y=zY;
}

void OsY::SetDlugosc(UINT dlugosc){
	if(dlugosc<100)
		Dlugosc=100;
	else
		Dlugosc=dlugosc;
}

void OsY::SetTitle(char *Title){
	if(title) delete[] title;
	title=new char[strlen(Title)+1];
	strcpy(title,Title);
}

OsY::OsY(){
	title=NULL;
}

UINT OsY::GetDlugosc(void){
	return Dlugosc;
}

UINT OsY::GetG_Skali(void){
	return gSkali;
}

double OsY::GetMax(void){
	return max;
}

double OsY::GetMin(void){
	return min;
}

void OsY::SetG_Skali(UINT _gSkali){
	if(_gSkali<2)
		gSkali=2;
	else
		gSkali=_gSkali;
}

void OsY::SetMinMax(double Min,double Max){
	if(Min>Max){
		min=Max;
		max=Min;
	}else if(Min<Max){
		min=Min;
		max=Max;
	}
}

double OsY::Krok(){
	return (max-min)/(double)(gSkali-1);
}

int OsY::xPos(void){
	return pZacz.x+pOds.x;
}

int OsY::yPos(void){
	return pZacz.y+pOds.y;
}

OsY::OsY(int zX,int zY,int oX,int oY,UINT dlugosc,COLORREF KOsiY,double Min,double Max){
	pOds.x=oX;
	pOds.y=oY;
	pZacz.x=zX;
	pZacz.y=zY;
	Dlugosc=dlugosc;
	kOsiY=KOsiY;
	SetMinMax(Min,Max);
	gSkali=11;
	title=NULL;
	SetTitle("Y");
}

OsY::~OsY(){
}
void OsY::RysujSiatke(HDC &hdc,COLORREF kSiatki,int _dSiatki,int Styl,int Gr){
	double Skok=(double)(Dlugosc-15)/(double)(gSkali-1);
	for(UINT i=0;i<gSkali;i++){
		Linia(&hdc,xPos(),yPos()-(int)(i*Skok),xPos()+_dSiatki,yPos()-(int)(i*Skok),kSiatki,Styl,Gr);
	}
};

void OsY::RysujOsY(HDC &hdc){
	Linia(&hdc,xPos(),yPos(),xPos(),yPos()-(int)Dlugosc,kOsiY,PS_SOLID,1);
	POINT pArrow[]={xPos(),yPos()-(int)Dlugosc,xPos()-5,yPos()-(int)Dlugosc+10,xPos()+5,yPos()-(int)Dlugosc+10};
	Polygon(hdc,pArrow,3);
	double Skok=(double)(Dlugosc-15)/(double)(gSkali-1);
	double SkokSkali=(max-min)/(gSkali-1);
	char bufor[20];
	RECT rect;
	SetTextColor(hdc,kOsiY);
	SetBkMode(hdc,TRANSPARENT);
	for(UINT i=0;i<gSkali;i++){
		Linia(&hdc,xPos()-5,yPos() -(int)(i*Skok),xPos(),yPos()-(int)(i*Skok),kOsiY,PS_SOLID,1);
		SetRect(&rect,xPos()-200,yPos()-10-(int)(Skok*i),xPos()-10,yPos()+10-(int)(Skok*i));
		_gcvt(SkokSkali*i+min,10,bufor);
		if(bufor[strlen(bufor)-1]=='.')
			bufor[strlen(bufor)-1]=NULL;
		DrawText(hdc,bufor,-1,&rect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
	}
	SetRect(&rect,xPos()+10,yPos()-Dlugosc,xPos()+10+(int)strlen(title)*10,yPos()+20-Dlugosc);
	DrawText(hdc,title,-1,&rect,DT_LEFT);
};