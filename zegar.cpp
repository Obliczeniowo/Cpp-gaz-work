#include <windows.h>

#include <math.h>

#include "ObsRysowania.h"
extern ObsRys *RYS;
#include "zegar.h"

int Zegar::xPos(void){
	return pZacz.x + pPrzes.x;
}

int Zegar::yPos(void){
	return pZacz.y + pPrzes.y;
}

void Zegar::Rysuj(HDC &hdc){
	int h=czas.wHour;
	int m=czas.wMinute;
	int s=czas.wSecond;
	BITMAP bmp;
	GetObject(hCyfry[0],sizeof(bmp),&bmp);
	RYS->MaskBmp(hdc,xPos(),yPos(),bmp.bmWidth,bmp.bmHeight,hCyfry[(h-(h%10))/10],hMask);
	RYS->MaskBmp(hdc,xPos()+bmp.bmWidth,yPos(),bmp.bmWidth,bmp.bmHeight,hCyfry[h%10],hMask);
	RYS->MaskBmp(hdc,xPos()+bmp.bmWidth*2,yPos(),bmp.bmWidth,bmp.bmHeight,hCyfry[10],hMask);
	RYS->MaskBmp(hdc,xPos()+bmp.bmWidth*3,yPos(),bmp.bmWidth,bmp.bmHeight,hCyfry[(m-(m%10))/10],hMask);
	RYS->MaskBmp(hdc,xPos()+bmp.bmWidth*4,yPos(),bmp.bmWidth,bmp.bmHeight,hCyfry[m%10],hMask);
	RYS->MaskBmp(hdc,xPos()+bmp.bmWidth*5,yPos(),bmp.bmWidth,bmp.bmHeight,hCyfry[10],hMask);
	RYS->MaskBmp(hdc,xPos()+bmp.bmWidth*6,yPos(),bmp.bmWidth,bmp.bmHeight,hCyfry[(s-(s%10))/10],hMask);
	RYS->MaskBmp(hdc,xPos()+bmp.bmWidth*7,yPos(),bmp.bmWidth,bmp.bmHeight,hCyfry[s%10],hMask);
}

void Zegar::Timer(void){
	GetLocalTime(&czas);
	InvalidateRect(hWnd,NULL,false);
}

Zegar::Zegar(int xZ,int yZ,int xP,int yP,HWND hwnd,TIMERPROC TimerProc,UINT _idEvent,COLORREF _kOn,COLORREF _kOff){
	pZacz.x		= xZ;
	pZacz.y		= yZ;
	pPrzes.x	= xP;
	pPrzes.y	= yP;
	hWnd		= hwnd;
	timerProc	= TimerProc;
	kOn			= _kOn;
	kOff		= _kOff;
	idEvent		= _idEvent;

	SetCyfry();
	SetTimer(hWnd,idEvent,1000,timerProc);
}

Zegar::~Zegar(void){
	KillTimer(hWnd,idEvent);
}

void Zegar::SetCyfry(void){
	for(int i=0;i<11;i++)
		RYS->CyfryWysEl(hCyfry[i],i,RGB(0,0,0),kOn,kOff,1.);
	BITMAP bmp;
	GetObject(hCyfry[0],sizeof(bmp),&bmp);
	DeleteObject(hMask);
	hMask	= CreateBitmap(bmp.bmWidth,bmp.bmHeight,1,1,NULL);
	HDC hdc	= GetDC(NULL);
	HDC mem	= CreateCompatibleDC(hdc);
	SelectObject(mem,hMask);
	HBITMAP hBmp;
	RYS->CyfryWysEl(hBmp,8,RGB(0,0,0),RGB(255,255,255),RGB(255,255,255),1.);
	RYS->bitBltBmpOnHdc(mem,hBmp,0,0);
	DeleteDC(hdc);
	DeleteDC(mem);
}