#ifndef ObsRysowania_h
#define ObsRysowania_h

#include <windows.h>
#include <string>

RECT GetRect(int x,int y,UINT szer,UINT wys);

class ObsRys{
	HPEN hPen;
	HBRUSH hBrush;
	void Pion(HDC &hdc,int dx,int dy,COLORREF kolor,double skala);
	void Poziom(HDC &hdc,int dx,int dy,COLORREF kolor,double skala);
public:
	ObsRys(void);
	void rectangle(HDC &hdc,RECT rect);
	void rectangle(HDC &hdc,int x,int y,UINT szer,UINT wys);
	void rectangle(HDC &hdc,RECT rect,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void rectangle(HDC &hdc,int x,int y,UINT szer,UINT wys,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void fillRectangle(HDC &hdc,int x,int y,UINT szer,UINT wys);
	void fillRectangle(HDC &hdc,RECT rect);
	void fillRectangle(HDC &hdc,int x,int y,UINT szer,UINT wys,COLORREF kBrush);
	void fillRectangle(HDC &hdc,RECT rect,COLORREF kBrush);
	void frameRectangle(HDC &hdc,int x,int y,UINT szer,UINT wys);
	void frameRectangle(HDC &hdc,RECT rect);
	void frameRectangle(HDC &hdc,int x,int y,UINT szer,UINT wys,COLORREF kPen,int Styl,int Gr);
	void frameRectangle(HDC &hdc,RECT rect,COLORREF kPen,int Styl,int Gr);
	void circle(HDC &hdc,int xS,int yS,UINT Ray);
	void circle(HDC &hdc,int xS,int yS,UINT Ray,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void fillCircle(HDC &hdc,int xS,int yS,UINT Ray);
	void fillCircle(HDC &hdc,int xS,int yS,UINT Ray,COLORREF kBrush);
	void frameCircle(HDC &hdc,int xS,int yS,UINT Ray);
	void frameCircle(HDC &hdc,int xS,int yS,UINT Ray,COLORREF kPen,int Styl,int Gr);
	void ellipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay);
	void ellipse(HDC &hdc,RECT rect);
	void ellipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void ellipse(HDC &hdc,RECT rect,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void fillEllipse(HDC &hdc,RECT rect);
	void fillEllipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay);
	void fillEllipse(HDC &hdc,RECT rect,COLORREF kBrush);
	void fillEllipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,COLORREF kBrush);
	void frameEllipse(HDC &hdc,RECT rect);
	void frameEllipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay);
	void frameEllipse(HDC &hdc,RECT rect,COLORREF kPen,int Styl,int Gr);
	void frameEllipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr);
	void roundRect(HDC &hdc,RECT rect,UINT Ray);
	void roundRect(HDC &hdc,RECT rect,UINT xRay,UINT yRay);
	void roundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT Ray);
	void roundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT xRay,UINT yRay);
	void roundRect(HDC &hdc,RECT rect,UINT Ray,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void roundRect(HDC &hdc,RECT rect,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void roundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT Ray,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void roundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void fillRoundRect(HDC &hdc,RECT rect,UINT Ray,COLORREF kBrush);
	void fillRoundRect(HDC &hdc,RECT rect,UINT xRay,UINT yRay,COLORREF kBrush);
	void fillRoundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT Ray,COLORREF kBrush);
	void fillRoundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT xRay,UINT yRay,COLORREF kBrush);
	void frameRoundRect(HDC &hdc,RECT rect,UINT Ray,COLORREF kPen,int Styl,int Gr);
	void frameRoundRect(HDC &hdc,RECT rect,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr);
	void frameRoundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT Ray,COLORREF kPen,int Styl,int Gr);
	void frameRoundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr);
	void line(HDC &hdc,POINT p1,POINT p2);
	void line(HDC &hdc,int x1,int y1,int x2,int y2);
	void line(HDC &hdc,int X,int Y,int kt,UINT R);
	void line(HDC &hdc,int X,int Y,double kt,UINT R);
	void line(HDC &hdc,POINT p1,POINT p2,COLORREF kPen,int Styl,int Gr);
	void line(HDC &hdc,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void line(HDC &hdc,int X,int Y,int kt,UINT R,COLORREF kPen,int Styl,int Gr);
	void line(HDC &hdc,int X,int Y,double kt,UINT R,COLORREF kPen,int Styl,int Gr);
	void circleArc(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2);
	void circleArc(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2);
	void circleArc(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2);
	void circleArc(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void circleArc(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void circleArc(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	void ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2);
	void ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2);
	void ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2);
	void ellipseArc(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2);
	void ellipseArc(HDC &hdc,RECT rect,int kt1,int kt2);
	void ellipseArc(HDC &hdc,RECT rect,double kt1,double kt2);
	void ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	void ellipseArc(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void ellipseArc(HDC &hdc,RECT rect,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void ellipseArc(HDC &hdc,RECT rect,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	void circleChord(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2);
	void circleChord(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2);
	void circleChord(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2);
	void circleChord(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void circleChord(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void circleChord(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	void ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2);
	void ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2);
	void ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2);
	void ellipseChord(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2);
	void ellipseChord(HDC &hdc,RECT rect,int kt1,int kt2);
	void ellipseChord(HDC &hdc,RECT rect,double kt1,double kt2);
	void ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	void ellipseChord(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void ellipseChord(HDC &hdc,RECT rect,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void ellipseChord(HDC &hdc,RECT rect,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	void circlePie(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2);
	void circlePie(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2);
	void circlePie(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2);
	void circlePie(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void circlePie(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void circlePie(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	void ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2);
	void ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2);
	void ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2);
	void ellipsePie(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2);
	void ellipsePie(HDC &hdc,RECT rect,int kt1,int kt2);
	void ellipsePie(HDC &hdc,RECT rect,double kt1,double kt2);
	void ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	void ellipsePie(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void ellipsePie(HDC &hdc,RECT rect,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void ellipsePie(HDC &hdc,RECT rect,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	void polyline(HDC &hdc,CONST POINT* Punkty,int ilP);
	void polyline(HDC &hdc,CONST POINT* Punkty,int ilP,COLORREF kPen,int Styl,int Gr);
	void polygon(HDC &hdc,CONST POINT* Punkty,int ilP);
	void polygon(HDC &hdc,CONST POINT* Punkty,int ilP,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void framePolygon(HDC &hdc,CONST POINT* Punkty,int ilP);
	void framePolygon(HDC &hdc,CONST POINT* Punkty,int ilP,COLORREF kPen,int Styl,int Gr);
	void fillPolygon(HDC &hdc,CONST POINT* Punkty,int ilP,COLORREF kBrush);
	void fillPolygon(HDC &hdc,CONST POINT* Punkty,int ilP);
	void polyPolygon(HDC &hdc,CONST POINT* Punkty,CONST INT *ilP,int ilW);
	void polyPolygon(HDC &hdc,CONST POINT* Punkty,CONST INT *ilP,int ilW,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void wielForemny(HDC &hdc,int xS,int yS,UINT ilBokow,UINT R,int ktPrzes);
	void wielForemny(HDC &hdc,int xS,int yS,UINT ilBokow,UINT R,double ktPrzes);
	void wielForemny(HDC &hdc,int xS,int yS,UINT ilBokow,UINT R,int ktPrzes,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void wielForemny(HDC &hdc,int xS,int yS,UINT ilBokow,UINT R,double ktPrzes,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void Wektor(HDC &hdc,int X,int Y,double Kat,int dlWektora,bool Arrow);
	void Wektor(HDC &hdc,int x1,int y1,int x2,int y2,bool Arrow);
	void Wektor(HDC &hdc,int X,int Y,double Kat,int dlWektora,bool Arrow,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void Wektor(HDC &hdc,int x1,int y1,int x2,int y2,bool Arrow,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void frameWektor(HDC &hdc,int x1,int y1,int x2,int y2,bool Arrow);
	void frameWektor(HDC &hdc,int x1,int y1,int x2,int y2,bool Arrow,COLORREF kPen,int Styl,int Gr);
	void textOut(HDC &hdc,int x,int y,LPSTR tekst,int dlTekstu);
	void textOut(HDC &hdc,int x,int y,int liczba,int ilCyfr);
	void textOut(HDC &hdc,int x,int y,double liczba,int ilCyfr);
	void textOut(HDC &hdc,int x,int y,LPSTR tekst,int dlTekstu,COLORREF kTekstu,int bkMode);
	void textOut(HDC &hdc,int x,int y,int liczba,int ilCyfr,COLORREF kTekstu,int bkMode);
	void textOut(HDC &hdc,int x,int y,double liczba,int ilCyfr,COLORREF kTekstu,int bkMode);
	void drawText(HDC &hdc,LPSTR tekst,int dlTekstu,RECT *rect,UINT uFormat);
	void drawText(HDC &hdc,LPSTR tekst,int dlTekstu,int x,int y,UINT szer,UINT wys,UINT uFormat);
	void drawText(HDC &hdc,int liczba,int ilCyfr,int x,int y,UINT szer,UINT wys,UINT uFormat);
	void drawText(HDC &hdc,double liczba,int ilCyfr,int x,int y,UINT szer,UINT wys,UINT uFormat);
	void CyfryWysEl(HBITMAP &hBitmap,UINT nrLiczby/*0-11 (0 - zero, 1 - jeden .... 10 - -*/,COLORREF kTla,COLORREF On,COLORREF Off,double skala);
	void bitBltBmpOnHdc(HDC &hdc,HBITMAP hBmp,int x,int y,UINT szer,UINT wys);
	void bitBltBmpOnHdc(HDC &hdc,HBITMAP hBmp,int x,int y);
	void stretchBltBmpOnHdc(HDC &hdc,HBITMAP hBmp,int x,int y,UINT szer,UINT wys);
	void MaskBmp(HDC hdc,int x,int y,int szer,int wys,HBITMAP Image,HBITMAP Mask);
	void StretchMaskBmp(HDC hdc,int x,int y,int szer,int wys,HBITMAP Image,HBITMAP Mask);
	HBITMAP CreateMask(HBITMAP hBmp,COLORREF kMaski);
	void SetBrush(HDC &hdc,COLORREF kBrush);
	void SetBrush(HDC &hdc,HBRUSH _hBrush);
	void SetPen(HDC &hdc,COLORREF kPen,int Styl,int Gr);
	void SetPen(HDC &hdc,HPEN _hPen);
	PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd,HBITMAP hBmp);
	void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,HBITMAP hBMP, HDC hDC);
	HBITMAP GetBitmapFile(std::string szFileName);
};


#endif