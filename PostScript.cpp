#include ".\postscript.h"

ColorType::ColorType(){
	sk[0]=sk[1]=sk[2]=sk[3]=0;
	SysColor=-1;
}

void ColorType::SetCMYKColor(UINT C,UINT M,UINT Y,UINT B){
	sk[0]=C;
	sk[1]=M;
	sk[2]=Y;
	sk[3]=B;
	SysColor=COLOR_CMYK;
}

void ColorType::SetGrayColor(UINT G){
	sk[0]=G;
	sk[1]=sk[2]=sk[3]=0;
	SysColor=COLOR_GRAY;
}

void ColorType::SetHSBColor(UINT H,UINT S,UINT B){
	sk[0]=H;
	sk[1]=S;
	sk[2]=B;
	sk[3]=0;
	SysColor=COLOR_HSB;
}

void ColorType::SetRGBColor(UINT R,UINT G,UINT B){
	sk[0]=R;
	sk[1]=G;
	sk[2]=B;
	sk[3]=0;
	SysColor=COLOR_RGB;
}

int ColorType::GetSysColor(){
	return SysColor;
}

void ColorType::GetSk(UINT nrOfSkIndex,UINT *c){
	if(nrOfSkIndex<4)
		*c=sk[nrOfSkIndex];
}

float PostScript::GetFontSize(){
	return this->FontSize;
}

void PostScript::SetRGBColor(UINT R,UINT G,UINT B){
	if(R>255)
		R=255;
	if(G>255)
		G=255;
	if(B>255)
		B=255;
	Color.SetRGBColor(R,G,B);
	*plik<<((double)R)/255<<" "<<double(G)/255<<" "<<double(B)/255<<" setrgbcolor"<<std::endl;
}

void PostScript::SetRGBColor(COLORREF color){
	SetRGBColor(GetRValue(color),GetGValue(color),GetBValue(color));
}
//odcieñ (barwa), nasycenie, jasnoœæ
void PostScript::SetHSBColor(UINT hue,UINT saturation,UINT brightness){
	if(hue>255)
		hue=255;
	if(saturation>255)
		saturation=255;
	if(brightness>255)
		brightness=255;
	Color.SetHSBColor(hue,saturation,brightness);
	*plik<<double(hue)/255<<" "<<double(saturation)/255<<" "<<double(brightness)/255<<" sethsbcolor"<<std::endl;
}

void PostScript::SetCMYKColor(UINT cyan,UINT magenta,UINT yellow,UINT black){
	if(cyan>100)
		cyan=100;
	if(magenta>100)
		magenta=100;
	if(yellow>100)
		yellow=100;
	if(black>100)
		black=100;
	Color.SetCMYKColor(cyan,magenta,yellow,black);
	*plik<<double(cyan)/100<<" "<<double(magenta)/100<<" "<<double(yellow)/100<<" "<<double(black)/100<<" setcmykcolor"<<std::endl;
}

void PostScript::SetGray(UINT gray){
	if(gray>255)
		gray=255;
	Color.SetGrayColor(gray);
	*plik<<double(gray)/255<<" setgray"<<std::endl;
}

void PostScript::SetFont(LPSTR FontName,float FontSize){
	FontSize=abs(FontSize);
	if(FontSize>1000)
		FontSize=1000;
	*plik<<FontSize<<" "<<FontName<<" font"<<std::endl;
	this->FontSize=FontSize;
	if(this->FontName)
		delete[] this->FontName;
	this->FontName=new char[strlen(FontName)+1];
	strcpy(this->FontName,FontName);
}

void PostScript::SetLineWidth(UINT lWidth){
	if(!lWidth)
		lWidth=1;
	if(lWidth!=this->lWidth)
		*plik<<lWidth<<" setlinewidth"<<std::endl;
	this->lWidth=lWidth;
}

void PostScript::SetDash(){
	*plik<<"[] 0 setdash"<<std::endl;
}

void PostScript::SetDash(UINT array,int offset){
	*plik<<"["<<array<<"] "<<offset<<" setdash"<<std::endl;
}

void PostScript::SetDash(UINT array1,UINT array2,int offset){
	*plik<<"["<<array1<<" "<<array2<<"] "<<offset<<" setdash"<<std::endl;
}

void PostScript::SetLineCap(UINT LineCapStyle){
	if(LineCapStyle<=SQUARE_CAP){
		if(this->LineCapStyle!=LineCapStyle)
			*plik<<LineCapStyle<<" setlinecap"<<std::endl;
		this->LineCapStyle=LineCapStyle;
	}
}

void PostScript::SetFlat(UINT nrOfFlats){
	if(!nrOfFlats)
		nrOfFlats=1;
	*plik<<nrOfFlats<<" setflat"<<std::endl;
}

void PostScript::SetLineRaster(int ods,double ktInDegrees){
	gsave();
	*plik<<ods<<" "<<ktInDegrees<<" {pop} setscreen"<<std::endl;
	SetGray(200);
	fill();
	grestore();
	stroke();
}

void PostScript::polyPolygon(CONST POINT *points,UINT nrOfPoints,int styl){
	if(nrOfPoints){
		*plik<<points[0].x<<" "<<points[0].y<<" moveto"<<std::endl;
		for(UINT i=1;i<nrOfPoints;i++){
			*plik<<points[i].x<<" "<<points[i].y<<" lineto"<<std::endl;
		}
		closepath();
		if(styl==STROKE)
			stroke();
		else if(styl==FILL)
			fill();
	}
}

void PostScript::rlineto(int dx,int dy){
	*plik<<dx<<" "<<dy<<" rlineto"<<std::endl;
}

void PostScript::lineto(int x,int y){
	*plik<<x<<" "<<y<<" lineto"<<std::endl;
}

void PostScript::rlineto(int l,double ktInDegrees){
	ktInDegrees=ktInDegrees*acos(0.)/90;
	int x=int(l*cos(ktInDegrees));
	int y=int(l*sin(ktInDegrees));
	*plik<<x<<" "<<y<<" rlineto"<<std::endl;
	stroke();
}

void PostScript::line(int x,int y,int l,double ktInDegrees){
	moveto(x,y);
	rlineto(l,ktInDegrees);
}

void PostScript::line(int x,int y,int dx,int dy){
	*plik<<x<<" "<<y<<" moveto "<<dx<<" "<<dy<<" rlineto"<<std::endl;
	stroke();
}
void PostScript::line(POINT pP,POINT pK){
	line(pP.x,pP.y,pP.x-pK.x,pP.y-pK.y);
}
void PostScript::lineA(int x1,int y1,int x2,int y2){
	line(x1,y1,x2-x1,y2-y1);
}
void PostScript::rectangle(int x,int y,UINT width,UINT height,int styl){
	*plik<<x<<" "<<y<<" "<<width<<" "<<height<<" rectangle"<<std::endl;
	if(styl==STROKE)
		stroke();
	else if(styl==FILL)
		fill();
}
void PostScript::rectangle(int xP,int yP,int xK,int yK,int styl){
	rectangle(xP,yP,(UINT)abs(xP-xK),(UINT)abs(yP-yK),styl);
	if(styl==STROKE)
		stroke();
	else if(styl==FILL)
		fill();
}
void PostScript::rectangle(POINT pP,POINT pK,int styl){
	rectangle(pP.x,pP.y,(UINT)abs(pP.x-pK.x),(UINT)abs(pP.y-pK.y),styl);
	if(styl==STROKE)
		stroke();
	else if(styl==FILL)
		fill();
}
void PostScript::rectangle(RECT rect,int styl){
	rectangle(rect.left,rect.top,rect.right,rect.bottom,styl);
}

void PostScript::roundRect(int x,int y,UINT width,UINT height,UINT xRay,UINT yRay,int styl){
	if(styl==FILL){
		ellipse(x+xRay,y+yRay,xRay,yRay,0,styl);
		ellipse(x+width-xRay,y+yRay,xRay,yRay,0,styl);
		ellipse(x+width-xRay,y+height-yRay,xRay,yRay,0,styl);
		ellipse(x+xRay,y+height-yRay,xRay,yRay,0,styl);
		rectangle(x,y+yRay,width,UINT(height-2*yRay),styl);
		rectangle(x+xRay,y,UINT(width-2*xRay),height,styl);
	}else if(styl==STROKE){
		ellipseArc(x+xRay,y+yRay,xRay,yRay,180,270,0);
		line(x+xRay,y,(int)(width-2*xRay),0);
		ellipseArc(x+width-xRay,y+yRay,xRay,yRay,270,360,0);
		line(x+width,y+yRay,int(0),(int)(height-2*yRay));
		ellipseArc(x+width-xRay,y+height-yRay,xRay,yRay,0,90,0);
		line(x+xRay,(int)(y+height),(int)(width-2*xRay),0);
		line(x,y+yRay,0,int(height-2*yRay));
		ellipseArc(x+xRay,y+height-yRay,xRay,yRay,90,180,0);
	}
}

void PostScript::circle(int x,int y,int ray,int styl){
	*plik<<x<<" "<<y<<" "<<ray<<" 0 360 arc"<<std::endl;
	if(styl==STROKE)
		stroke();
	else if(styl==FILL)
		fill();
}

void PostScript::arc(int x,int y,int ray,int kPInDegrees,int kKInDegrees){
	*plik<<x<<" "<<y<<" "<<ray<<" "<<kPInDegrees<<" "<<kKInDegrees<<" arc"<<std::endl;
	stroke();
}

void PostScript::arc(int x,int y,int ray,int kPInDegrees,int kKInDegrees,int styl){
	*plik<<x<<" "<<y<<" "<<ray<<" "<<kPInDegrees<<" "<<kKInDegrees<<" arc"<<std::endl;
	if(styl==STROKE)
		stroke();
}

void PostScript::arcPie(int x,int y,int ray,int kPInDegrees,int kKInDegrees,int styl){
	*plik<<"newpath"<<std::endl;
	*plik<<x<<" "<<y<<" moveto "<<x<<" "<<y<<" "<<ray<<" "<<kPInDegrees<<" "<<kKInDegrees<<" arc"<<std::endl;
	*plik<<x<<" "<<y<<" lineto"<<std::endl;
	closepath();
	if(styl==STROKE)
		stroke();
	else if(styl==FILL)
		fill();
}

void PostScript::ellipse(int x,int y,int rayX,int rayY,int kOsiElipsy,int styl){
	translate(x,y);
	rotate(kOsiElipsy);
	scale(1,double(rayY)/rayX);
	circle(0,0,rayX,styl);
	scale(1,double(rayX)/rayY);
	rotate(-kOsiElipsy);
	translate(-x,-y);
}

void PostScript::ellipseArc(int x,int y,int rayX,int rayY,int ktPocz,int ktKon,int kOsiElipsy){
	translate(x,y);
	rotate(kOsiElipsy);
	scale(1,double(rayY)/rayX);
	arc(0,0,rayX,ktPocz,ktKon);
	scale(1,double(rayX)/rayY);
	rotate(-kOsiElipsy);
	translate(-x,-y);
}

void PostScript::ellipsePie(int x,int y,int rayX,int rayY,int ktPocz,int ktKon,int kOsiElipsy,int styl){
	translate(x,y);
	rotate(kOsiElipsy);
	scale(1,double(rayY)/rayX);
	arcPie(0,0,rayX,ktPocz,ktKon,styl);
	scale(1,double(rayX)/rayY);
	rotate(-kOsiElipsy);
	translate(-x,-y);
}

void PostScript::wielForemny(int xS,int yS,UINT ilBokow,UINT R,double ktPrzes,int styl){
	POINT *p;
	p=new POINT[ilBokow];
	double dk=asin(1.)*4/(double)ilBokow;
	for(POINT* pn=p;pn<p+ilBokow;pn++){
		pn->x=xS+(int)((double)R*cos(dk*(pn-p)+ktPrzes));
		pn->y=yS+(int)((double)R*sin(dk*(pn-p)+ktPrzes));
	}
	this->polyPolygon(p,ilBokow,styl);
	delete[] p;
}

void PostScript::curve(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,int styl){
	moveto(x1,y1);
	*plik<<x2<<" "<<y2<<" "<<x3<<" "<<y3<<" "<<x4<<" "<<y4<<" curveto"<<std::endl;
	if(styl==STROKE)
		stroke();
}
void PostScript::curve(POINT p1,POINT p2,POINT p3,POINT p4,int styl){
	curve(p1.x,p1.y,p2.x,p2.y,p3.x,p3.y,p4.x,p4.y,NOEFFECT);
	if(styl==STROKE)
		stroke();
}
void PostScript::curveto(int dx1,int dy1,int dx2,int dy2,int dx3,int dy3,int styl){
	*plik<<dx1<<" "<<dy1<<" "<<dx2<<" "<<dy2<<" "<<dx3<<" "<<dy3<<" curveto"<<std::endl;
	if(styl==STROKE)
		stroke();
}

BOOL PostScript::spline(CONST POINT *p,UINT nrOfPoints){
	if((nrOfPoints % 2 ) && nrOfPoints<4)
		return FALSE;
	curve(p[0],p[1],p[2],p[3],NOEFFECT);
	for(int i=4;i<(int)nrOfPoints;i+=2){
		curve(p[i-1].x,p[i-1].y,p[i-1].x+(p[i-1].x-p[i-2].x),p[i-1].y+(p[i-1].y-p[i-2].y),p[i].x,p[i].y,p[i+1].x,p[i+1].y,NOEFFECT);
	}
	stroke();
	return TRUE;
}

BOOL PostScript::splineSinusoide(int x,int y,int WidthInPeriod,int Amplitude,UINT Period){
	if(!WidthInPeriod)
		return FALSE;
	POINT *pt=new POINT[WidthInPeriod*4];
	double PI=acos(0.)*2;
	pt[0].x=x;
	pt[0].y=y;
	for(int i=1;i<WidthInPeriod*4;i++){
		pt[i].x=x+i*Period/4;
		double kt=.5*PI/(double(Period)/4.)*double(i*Period)/4.;
		double dy=(double)Amplitude*sin(kt);
		pt[i].y=int(y+dy);
	}
	spline(pt,WidthInPeriod*4);
	delete[] pt;
	return TRUE;
}

void PostScript::DrawBitmap(int x,int y,HBITMAP hBmp){
	BITMAP bmp;
	GetObject(hBmp,sizeof(bmp),&bmp);
	HDC hdc=GetDC(NULL);
	HDC mem=CreateCompatibleDC(hdc);
	SelectObject(mem,hBmp);
	if(bmp.bmWidth>100)
		bmp.bmWidth=100;
	if(bmp.bmHeight>100)
		bmp.bmHeight=100;
	for(int i=0;i<bmp.bmWidth;i++){
		for(int j=0;j<bmp.bmHeight;j++){
			SetRGBColor(GetPixel(mem,i,j));
			rectangle(x+i,y+j,(UINT)1,(UINT)1,FILL);
		}
	}
	DeleteDC(mem);
	ReleaseDC(NULL,hdc);
}

RECT PostScript::frameRect(int x,int y,UINT width,UINT height,CONST int *gr,CONST int *ods,int nrOfFrames,int nrOfFrameToFill,COLORREF color){
	if(nrOfFrameToFill>0 && nrOfFrameToFill<=nrOfFrames){
		gsave();
		SetRGBColor(color);
		rectangle(x+ods[nrOfFrameToFill-1],y+ods[nrOfFrameToFill-1],UINT(int(width-ods[nrOfFrameToFill-1]*2)),UINT(int(height-ods[nrOfFrameToFill-1]*2)),FILL);
		grestore();
	}
	gsave();
	for(int i=0;i<nrOfFrames;i++){
		SetLineWidth(abs(gr[i]));
		rectangle(x+ods[i],y+ods[i],width-2*ods[i],height-2*ods[i],STROKE);
	}
	grestore();
	RECT r;
	SetRect(&r,x+ods[nrOfFrames-1]+gr[nrOfFrames-1]-1,y+ods[nrOfFrames-1]+gr[nrOfFrames-1]-1,x+width-ods[nrOfFrames-1]-gr[nrOfFrames-1]+1,y+height-ods[nrOfFrames-1]-gr[nrOfFrames-1]+1);
	return r;
}

void PostScript::frameCircle(int x,int y,int ray,CONST int *gr,CONST int *ods,int nrOfFrames,int nrOfFrameToFill,COLORREF color){
	if(nrOfFrameToFill>0 && nrOfFrameToFill<=nrOfFrames){
		gsave();
		SetRGBColor(color);
		circle(x,y,UINT(int(ray)-ods[nrOfFrameToFill-1]),FILL);
		grestore();
	}
	for(int i=0;i<nrOfFrames;i++){
		SetLineWidth(abs(gr[i]));
		circle(x,y,ray-ods[i],STROKE);
	}
}

void PostScript::frameWiel(int xS,int yS,UINT ilBokow,UINT R,double ktPrzes,CONST int *gr,CONST int *ods,int nrOfFrames,int nrOfFrameToFill,COLORREF color){
	if(nrOfFrameToFill>0 && nrOfFrameToFill<=nrOfFrames){
		gsave();
		SetRGBColor(color);
		this->wielForemny(xS,yS,ilBokow,UINT(int(R)-ods[nrOfFrameToFill-1]),ktPrzes,FILL);
		grestore();
	}
	for(int i=0;i<nrOfFrames;i++){
		this->SetLineWidth(abs(gr[i]));
		wielForemny(xS,yS,ilBokow,(UINT)((int)R-ods[i]),ktPrzes,STROKE);
	}
}

void PostScript::frameEllipse(int x,int y,int rayX,int rayY,int kOsiElipsy,CONST int *gr,CONST int *ods,int nrOfFrames,int nrOfFrameToFill,COLORREF color){
	if(nrOfFrameToFill>0 && nrOfFrameToFill<=nrOfFrames){
		gsave();
		SetRGBColor(color);
		ellipse(x,y,UINT(int(rayX)-ods[nrOfFrameToFill-1]),UINT(int(rayY)-ods[nrOfFrameToFill-1]),kOsiElipsy,FILL);
		grestore();
	}
	for(int i=0;i<nrOfFrames;i++){
		SetLineWidth(abs(gr[i]));
		ellipse(x,y,UINT(int(rayX)-ods[i]),UINT(int(rayY)-ods[i]),kOsiElipsy,STROKE);
	}
}

void PostScript::MandalaArguellesa(int x,int y,int RayMax,UINT nrOfMandalaCircles,int styl){
	if(nrOfMandalaCircles<5)
		nrOfMandalaCircles=5;
	else if(nrOfMandalaCircles>20)
		nrOfMandalaCircles=20;
	gsave();
	translate(x,y);
	double dr=RayMax/nrOfMandalaCircles;
	for(int j=0;j<4;j++){
		translate(-RayMax,0);
		moveto(0,0);
		for(int i=0;i<(int)nrOfMandalaCircles;i++){
			arc(int((i+1)*dr),0,(int)((i+1)*dr),-180,180,-1);
		}
		translate(RayMax,0);
		rotate(360/4);
	}
	rotate(360/4);
	translate(-x,-y);
	if(styl==STROKE)
		stroke();
	if(styl==FILL)
		fill();
	if(styl==EOFILL)
		eofill();
	grestore();
}

void PostScript::showTextOut(LPSTR string,int x,int y){
	*plik<<x<<" "<<y<<" moveto ("<<string<<") show"<<std::endl;
}

void PostScript::showTextOut(LPSTR string,int x,int y,LPSTR FontName,float FontSize){
	LPSTR fontName;
	float fontSize=this->FontSize;
	fontName=new char[strlen(FontName)+1];
	strcpy(fontName,this->FontName);
	SetFont(FontName,FontSize);
	moveto(x,y);
	*plik<<"("<<string<<") show"<<std::endl;
	SetFont(fontName,fontSize);
	delete[] fontName;
}

void PostScript::showDrawText(LPSTR string,int x,int y,int dtstyl){
	moveto(x,y);
	switch(dtstyl){
		case DT_LEFT:
			*plik<<"("<<string<<") show"<<std::endl;
			break;
		case DT_RIGHT:
			*plik<<"("<<string<<") right show"<<std::endl;
			break;
		case DT_CENTER:
			*plik<<"("<<string<<") center show"<<std::endl;
			break;
	}
}

void PostScript::showDrawText(LPSTR string,RECT rect,COLORREF kTla,COLORREF kText,int dtstyl){
	gsave();
	SetRGBColor(kTla);
	rectangle(rect,FILL);
	SetRGBColor(kText);
	int y=rect.bottom;
	int x=rect.left;
	int styl=DT_LEFT;
	if((dtstyl & DT_CENTER)==DT_CENTER){
		x+=(rect.right-rect.left)/2;
		styl=DT_CENTER;
	}else if((dtstyl & DT_RIGHT)==DT_RIGHT){
		x+=rect.right-rect.left;
		styl=DT_RIGHT;
	}
	if((dtstyl & (DT_VCENTER | DT_SINGLELINE))==(DT_VCENTER | DT_SINGLELINE))
		y-=(rect.bottom-rect.top)/2+int(FontSize/4);
	else
		y-=int(FontSize*.75);
	showDrawText(string,x,y,styl);
	grestore();
}

void PostScript::showDrawText(LPSTR string,RECT rect,COLORREF kText,int dtstyl){
	gsave();
	SetRGBColor(kText);
	int y=rect.bottom;
	int x=rect.left;
	int styl=DT_LEFT;
	if((dtstyl & DT_CENTER)==DT_CENTER){
		x+=(rect.right-rect.left)/2;
		styl=DT_CENTER;
	}else if((dtstyl & DT_RIGHT)==DT_RIGHT){
		x+=rect.right-rect.left;
		styl=DT_RIGHT;
	}
	if((dtstyl & (DT_VCENTER | DT_SINGLELINE))==(DT_VCENTER | DT_SINGLELINE))
		y-=(rect.bottom-rect.top)/2+int(FontSize/4);
	else
		y-=int(FontSize*.75);
	showDrawText(string,x,y,styl);
	grestore();
}

void PostScript::showDateTimeNow(int x,int y){
	SYSTEMTIME st;
	GetSystemTime(&st);
	moveto(x,y);
	*plik<<"("<<st.wYear<<(st.wMonth>9?"-":"-0")<<st.wMonth<<(st.wDay>9?"-":"-0")<<st.wDay<<(st.wHour>9?" ":" 0")
		<<st.wHour<<(st.wMinute>9?":":":0")<<st.wMinute<<(st.wSecond>9?":":":0")<<st.wSecond<<(st.wMilliseconds>99?".":(st.wMilliseconds>9?".0":".00"))<<st.wMilliseconds<<") show"<<std::endl;
}

void PostScript::showDrawText(double Number,int x,int y,int dtstyl){
	moveto(x,y);
	switch(dtstyl){
		case DT_LEFT:
			*plik<<"("<<Number<<") show"<<std::endl;
			break;
		case DT_RIGHT:
			*plik<<"("<<Number<<") right show"<<std::endl;
			break;
		case DT_CENTER:
			*plik<<"("<<Number<<") center show"<<std::endl;
			break;
	}
}

void PostScript::fill(){
	*plik<<"fill"<<std::endl;
}
void PostScript::eofill(){
	*plik<<"eofill"<<std::endl;
}
void PostScript::stroke(){
	*plik<<"stroke"<<std::endl;
}

void PostScript::closepath(){
	*plik<<"closepath"<<std::endl;
}

void PostScript::gsave(){
	gsColor=Color;
	gsFontSize=FontSize;
	gsFontName=FontName;
	gslWidth=lWidth;
	gsLineCapStyle=LineCapStyle;
	*plik<<"gsave"<<std::endl;
}

void PostScript::grestore(){
	Color=gsColor;
	FontSize=gsFontSize;
	FontName=gsFontName;
	lWidth=gslWidth;
	LineCapStyle=gsLineCapStyle;
	*plik<<"grestore"<<std::endl;
}

void PostScript::translate(int x,int y){
	*plik<<x<<" "<<y<<" translate"<<std::endl;
}

void PostScript::rotate(int ktInDegrees){
	*plik<<ktInDegrees<<" rotate"<<std::endl;
}

void PostScript::scale(double xScale,double yScale){
	*plik<<xScale<<" "<<yScale<<" scale"<<std::endl;
}

void PostScript::moveto(int x,int y){
	*plik<<x<<" "<<y<<" moveto"<<std::endl;
}

PostScript::PostScript(LPSTR FileAndPathName)
{
	myMath::SetAngle3D(60,45,ktOsiX,ktOsiY,ktOsiZ,xSkala,ySkala,zSkala);
	plik=new std::fstream(FileAndPathName,std::ios::out);
	*plik<<"/font{findfont exch scalefont setfont}bind def"<<std::endl;
	*plik<<"/rectangle {/height exch def /width exch def /y exch def /x exch def x y moveto width 0 rlineto 0 height rlineto width neg 0 rlineto 0 height neg rlineto}bind def"<<std::endl;
	*plik<<"/center {dup stringwidth pop 2 div neg 0 rmoveto}bind def"<<std::endl;
	*plik<<"/right {dup stringwidth pop neg 0 rmoveto}bind def"<<std::endl;
	FontName=NULL;
	SetLineWidth(1);
	SetFont(Helvetica,10);
	SetLineCap(ROUND_CAP);
	Color.SetRGBColor(0,0,0);
}

PostScript::~PostScript(void)
{
	plik->close();
	delete plik;
}
