#ifndef zegar_h
#define zegar_h
class Zegar{
	SYSTEMTIME czas;
	HBITMAP hCyfry[11];
	HBITMAP hMask;
	TIMERPROC timerProc;
	UINT idEvent;
	HWND hWnd;
	COLORREF kOn;
	COLORREF kOff;
	POINT pZacz;
	POINT pPrzes;
	void SetCyfry(void);
public:
	void Timer();
	Zegar(int xZ,int yZ,int xP,int yP,HWND hwnd,TIMERPROC TimerProc,UINT _idEvent,COLORREF _kOn,COLORREF _kOff);
	~Zegar(void);
	void Rysuj(HDC &hdc);
	int xPos(void);
	int yPos(void);
};


#endif