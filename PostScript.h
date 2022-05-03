#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <fstream>

#include "myMath.h"

#define A5_WIDTH_DPI	421
#define A5_HEIGHT_DPI	594
#define A4_WIDTH_DPI	594
#define A4_HEIGHT_DPI	842
#define A3_WIDTH_DPI	842
#define A3_HEIGHT_DPI	1189
#define A2_WIDTH_DPI	1189
#define A2_HEIGHT_DPI	1684
#define A1_WIDTH_DPI	1684
#define A1_HEIGHT_DPI	2378
#define A0_WIDTH_DPI	2378
#define A0_HEIGHT_DPI	3368

#define A5_WIDTH_MM		148
#define A5_HEIGHT_MM	210
#define A4_WIDTH_MM		210
#define A4_HEIGHT_MM	297
#define A3_WIDTH_MM		297
#define A3_HEIGHT_MM	420
#define A2_WIDTH_MM		420
#define A2_HEIGHT_MM	594
#define A1_WIDTH_MM		594
#define A1_HEIGHT_MM	840
#define A0_WIDTH_MM		840
#define A0_HEIGHT_MM	1189

#define NOEFFECT	-1
#define STROKE		0
#define FILL		1
#define EOFILL		2

#define AvantGardeBook				"/AvantGarde-Book"
#define AvantGardebookOblique		"/AvantGarde-BookOblique"
#define AvantGardeDemi				"/AvantGarde-Demi"
#define AvantGardeDemiOblique		"/AvantGarde-DemiOblique"
#define BookmanDemi					"/Bookman-Demi"
#define BookmanDemiItalic			"/Bookman-DemiItalic"
#define BookmanLight				"/Bookman-Light"
#define BookmanLightItalic			"/Bookman-LightItalic"
#define Courier						"/Courier"
#define CourierBold					"/Courier-Bold"
#define CourierBoldOblique			"/Courier-BoldOblique"
#define CourierOblique				"/Courier-Oblique"
#define Helvetica					"/Helvetica"
#define HelveticaBold				"/Helvetica-Bold"
#define HelveticaBoldOblique		"/Helvetica-BoldOblique"
#define HelveticaNarrow				"/Helvetica-Narrow"
#define HelveticaNarrowBold			"/Helvetica-Narrow-Bold"
#define HelveticaNarrowBoldOblique	"/Helvetica-Narrow-BoldOblique"
#define HelveticaNarrowOblique		"/Helvetica-Narrow-Oblique"
#define HelveticaOblique			"/Helvetica-Oblique"
#define NewCenturySchlbkBold		"/NewCenturySchlbk-Bold"
#define NewCenturySchlbkBoldItalic	"/NewCenturySchlbk-BoldItalic"
#define NewCenturySchlbkItalic		"/NewCenturySchlbk-Italic"
#define NewCenturySchlbkRoman		"/NewCenturySchlbk-Roman"
#define PalatinoBold				"/Palatino-Bold"
#define PalatinoBoldItalic			"/Palatino-BoldItalic"
#define PalatinoItalic				"/Palatino-Italic"
#define PalatinoRoman				"/Palatino-Roman"
#define Symbol						"/Symbol"
#define TimesBold					"/Times-Bold"
#define TimesItalic					"/Times-Italic"
#define TimesRoman					"/Times-Roman"
#define Chancery					"/ZapfChancery-MediumItalic"
#define Dingbats					"/ZapfDingbats"
#define TimesNewRoman				"/Times-New-Roman"
//Nie standardowe czcionki
#define URWChanceryLMed				"/URWChanceryLMed"
#define URWGothicsL					"/URWGothicsL"
#define URWGothicsLDem				"/URWGothicsLDem"
//Opcje dla SetLineCap
#define BUTTON_CAP	0
#define ROUND_CAP	1
#define SQUARE_CAP	2

#define COLOR_RGB	0
#define COLOR_HSB	1
#define COLOR_GRAY	2
#define COLOR_CMYK	3

#ifndef PostScript_h
#define PostScript_h
	class ColorType
	{
		UINT sk[4];
		int SysColor;
	public:
		ColorType();
		void SetRGBColor(UINT R,UINT G,UINT B);
		void SetHSBColor(UINT H,UINT S,UINT B);
		void SetCMYKColor(UINT C,UINT M,UINT Y,UINT B);
		void SetGrayColor(UINT G);
		int GetSysColor(void);
		void GetSk(UINT nrOfSkIndex,UINT *c);
	};
	class PostScript//:public ColorType
	{
		std::fstream *plik;
		float FontSize;
		LPSTR FontName;
		UINT lWidth;
		UINT LineCapStyle;
		ColorType Color;
		float gsFontSize;
		LPSTR gsFontName;
		UINT gslWidth;
		ColorType gsColor;
		UINT gsLineCapStyle;
		//Zmienne rysowania 3D
		double ktOsiX;
		double ktOsiY;
		double ktOsiZ;
		double xSkala;
		double ySkala;
		double zSkala;
	public:
		PostScript(LPSTR FileAndPathName);
		~PostScript(void);
		//Funkcje rysuj¹ce
		void rlineto(int dx,int dy);
		void rlineto(int l,double ktInDegrees);
		void lineto(int x,int y);
		void line(int x,int y,int l,double ktInDegrees);
		void line(int x,int y,int dx,int dy);
		void line(POINT pP,POINT pK);
		void lineA(int x1,int y1,int x2,int y2);
		void rectangle(int x,int y,UINT width,UINT height,int styl);
		void rectangle(int xP,int yP,int xK,int yK,int styl);
		void rectangle(POINT pP,POINT pK,int styl);
		void rectangle(RECT rect,int styl);
		void roundRect(int x,int y,UINT width,UINT height,UINT xRay,UINT yRay,int styl);
		void polyPolygon(CONST POINT *points,UINT nrOfPoints,int styl);
		void circle(int x,int y,int ray,int styl);
		void arc(int x,int y,int ray,int kPInDegrees,int kKInDegrees);
		void arc(int x,int y,int ray,int kPInDegrees,int kKInDegrees,int styl);
		void arcPie(int x,int y,int ray,int kPInDegrees,int kKInDegrees,int styl);
		void ellipse(int x,int y,int rayX,int rayY,int kOsiElipsy,int styl);
		void ellipseArc(int x,int y,int rayX,int rayY,int ktPocz,int ktKon,int kOsiElipsy);
		void ellipsePie(int x,int y,int rayX,int rayY,int ktPocz,int ktKon,int kOsiElipsy,int styl);
		void wielForemny(int xS,int yS,UINT ilBokow,UINT R,double ktPrzes,int styl);
		void curve(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,int styl);
		void curve(POINT p1,POINT p2,POINT p3,POINT p4,int styl);
		void curveto(int dx1,int dy1,int dx2,int dy2,int dx3,int dy3,int styl);
		BOOL spline(CONST POINT *p,UINT nrOfPoints);
		BOOL splineSinusoide(int x,int y,int WidthInPeriod,int Amplitude,UINT Period);
		void MandalaArguellesa(int x,int y,int RayMax,UINT nrOfMandalaCircles,int styl);
		RECT frameRect(int x,int y,UINT width,UINT height,CONST int *gr,CONST int *ods,int nrOfFrames,int nrOfFrameToFill,COLORREF color);
		void frameCircle(int x,int y,int ray,CONST int *gr,CONST int *ods,int nrOfFrames,int nrOfFrameToFill,COLORREF color);
		void frameWiel(int xS,int yS,UINT ilBokow,UINT R,double ktPrzes,CONST int *gr,CONST int *ods,int nrOfFrames,int nrOfFrameToFill,COLORREF color);
		void frameEllipse(int x,int y,int rayX,int rayY,int kOsiElipsy,CONST int *gr,CONST int *ods,int nrOfFrames,int nrOfFrameToFill,COLORREF color);
		//Funkcje obs³ugi tekstu
		void showTextOut(LPSTR string,int x,int y);
		void showTextOut(LPSTR string,int x,int y,LPSTR FontName,float FontSize);
		void showDrawText(LPSTR string,int x,int y,int dtstyl);
		void showDrawText(double Number,int x,int y,int dtstyl);
		void showDrawText(LPSTR string,RECT rect,COLORREF kTla,COLORREF kText,int dtStyl);
		void showDrawText(LPSTR string,RECT rect,COLORREF kText,int dtStyl);
		void showDateTimeNow(int x,int y);
		void DrawBitmap(int x,int y,HBITMAP hBmp);
		//Funkcje zmiany stanu grafiki
		void translate(int x,int y);
		void rotate(int ktInDegrees);
		void scale(double xScale,double yScale);
		void moveto(int x,int y);
		//
		void fill(void);
		void eofill(void);
		void stroke(void);
		void closepath(void);
		void clip(void);//to musi byæ pomiêdzy gsave() a grestore() wraz z instrukcj¹ rysowania obiektów obcinanych
		void gsave(void);
		void grestore(void);

		void SetRGBColor(UINT R,UINT G,UINT B);
		void SetRGBColor(COLORREF color);
		void SetHSBColor(UINT hue,UINT saturation,UINT brightness);//odcieñ (barwa), nasycenie, jasnoœæ
		void SetCMYKColor(UINT cyan,UINT magenta,UINT yellow,UINT black);
		void SetGray(UINT gray);
		void SetFont(LPSTR FontName,float FontSize);
		void SetLineWidth(UINT lWidth);
		void SetDash();
		void SetDash(UINT array,int offset);
		void SetDash(UINT array1,UINT array2,int offset);
		void SetLineCap(UINT LineCapStyle);
		void SetFlat(UINT nrOfFlats);//to musi byæ pomiêdzy gsave() a grestore()
		void SetLineRaster(int ods,double ktInDegrees);

		float GetFontSize(void);
	};
#endif