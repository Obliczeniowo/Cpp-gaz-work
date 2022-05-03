#include <stdlib.h>

#ifndef klasaTlok_h
#define klasaTlok_h

#include "PostScript.h"

class TlokSpr{
	bool timerOnOff;
	POINT pZacz;
	POINT pOds;
	POINT lastMousePos;
	double Vmax;
	double Vmin;
	UINT Ray;
	UINT dlTloka;
	int odsMinTloka;
	int odsMaxTloka;
	int odsTloka;
	COLORREF kCylindra;
	COLORREF kTloka;
	COLORREF kOdsMin;
	COLORREF kOdsMax;
	COLORREF kGazu;
	RECT GetObszTloka(void);//Funkcja zwracaj¹ca prostok¹t objêtoœci gazu w cylindrze
	TIMERPROC TimerProc;	//Procedura obs³ugi zegara
	UINT idEvent;			//identyfikator zegara
	UINT uElapse;			//T zegara
	double csOtoczenia;		//ciœnienie otoczenia
	double csW_Tloku;		//ciœnienie w t³oku	
	SYSTEMTIME czas;		//Czas systemowy
	void SetOdsTloka(int OdsTloka);
public:
	int xPos(void);
	int yPos(void);
	TlokSpr(int xZ,int yZ,int xO,int yO,double vMax,double vMin,UINT ray,UINT DlTloka,TIMERPROC timerProc,UINT IDevent,double CsW_Tloku,double CsOtoczenia);
	void RysujTlok(HDC &hdc);
	void RysujTlok(PostScript *p);
	void SetV(double vMin,double vMax);
	void SetDlTloka(UINT _dlTloka);
	void SetVtloka(double Vtloka);
	void SetCisnienieW_Tloku(double CsW_Tloku);
	void SetCisnienieOtoczenia(double CsOtoczenia);
	double GetVtloka(void);
	int GetCzasMS(void);
	UINT Get_uElapse(void);
	void Set_uElapse(UINT _uElapse);
	void SetCzas(void);
	void MouseMove(POINT MousePos,UINT MK,HWND hWnd);
	void lButtonDown(POINT MousePos);
	bool StartTimer(HWND hWnd);
	bool StopTimer(HWND hWnd);
	bool GetTimerOnOff(void);
	double SilaGazu(void);
};


#endif