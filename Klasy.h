#include <windows.h>
#include <math.h>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "ObsRysowania.h"
#include "klasaTlok.h"
#include "FunkcjeLiczace.h"
#include "PostScript.h"

#ifndef Klasy_h
#define Klasy_h

#include "PostScript.h"
#include "PointsClass.h"

struct warLEGENDY{int nrFu;bool WabTrueWtFalse;};

class wykres{
	//Opcje typu W��CZ/WY��CZ
	bool rysOnOff;		//W��cz/Wy��cz rysowanie wykresu
	bool wykresOnOff;	//W��cz/Wy��cz wy�wietlanie wykresu
	bool siatkaOnOff;	//W��cz/Wy��cz siatk�
	bool prowOnOff;		//W��cz/Wy��cz prowafnice
	bool oznOnOff;		//W��cz/Wy��cz oznaczenia
	bool wtRozw;		//Rozwini�cie lub zwini�cie listy prac technicznych
	bool wabRozw;		//Rozwini�cie lub zwini�cie listy prac absolutnych
	bool skokOnOff;		//W��cz/Wy��cz skok
	bool csOtControl;	//Klikni�cie mych� kontrolki ci�nienia otoczenia
	//Ustawienia dotycz�ce kolor�w wy�wietlania
	CHOOSECOLOR cc;			//Struktura s�u��ca do ustawiania kolor�w wykresu za pomoc� funkcji ChooseColor
	COLORREF CustColors[16];//Tablica 16-nastu wybranych kolor�w przeznaczona dla struktury cc (CHOOSECOLOR)
	COLORREF kOsiX;			//Kolor osi x wykresu
	COLORREF kOsiY;			//Kolor osi y wykresu
	COLORREF kSiatki;		//Kolor siatki wykresu
	COLORREF kProw;			//Kolor prowadnicy
	COLORREF kWykr;			//Kolor wykresu
	COLORREF kWykr2;		//Kolor rysowanej linii wykresu
	COLORREF kWab;			//Kolor graficznej interpretacji pracy absolutnej
	COLORREF kWt;			//Kolor graficznej interpretacji pracy technicznej
	COLORREF kgLegendy;		//Kolor g��wny legendy
	COLORREF krLegendy;		//Kolor rozwini�cia legendy
	COLORREF kCsOtoczenia;	//Kolor lini ci�nienia otoczenia
	//identyfikatory kursor�w myszy
	int idCrossCursor;		//Krzy�yk
	int idEmptyCursor;		//Pusta ikona kursora
	int idSelectedCursor;	//Identyfikator aktualnie zaznaczonego kursora myszy
	//
	UINT ilP;				//ilo�� utwo�onych punkt�w
	UINT wysRozwLegWab;		//Wysoko�� rozwini�cia legendy pracy absolutnej
	UINT pozPoczRozwWab;	//Pozycja pocz�tku rozwini�cia legendy pracy absolutnej
	UINT wysRozwLegWt;		//Wysoko�� rozwini�cia legendy pracy technicznej
	UINT pozPoczRozwWt;		//Pozycja pocz�tku rozwini�cia legendy pracy technicznej
	POINT PozLegendy;		//Pozycja legendy
	warLEGENDY zaznPoz;		//Zaznaczenie poprzez najechanie kursorem myszy na dan� pozycj� legendy
	warLEGENDY ZaznPoz;		//Zaznaczenie poprzez klikni�cie danej pozycji legendy
	POINT Przes;			//Przesuni�cie wykresu
	UINT selectP;			//Zaznacz punkt
	dPoint *punkty;			//Wska�nik do tablicy punk�w rysowanego wykresu
	RECT AreaW;				//Obszar wykresu
	LPSTR JednX;			//Jednostka na x-sie
	LPSTR JednY;			//Jednostka na y-ku
	LPSTR Title;			//Tytu� wykresu
	double xMax;			//Maksymalna warto�� na x-sie
	double yMax;			//Maksymalna warto�� na y-ku
	double xMin;			//Minimalna warto�� na x-sie
	double yMin;			//Minimalna warto�� na y-ku
	double csOtoczenia;		//Ci�nienie otoczenia
	UINT xG;				//G�sto�� skali (siatki) na x-sie
	UINT yG;				//G�sto�� skali (siatki) na y-ku
	double St;				//Temperatura pocz�tkowa gazu
	int n;					//Nr. punktu po�o�enia t�oka
	//FUNKCJE
	int wykrToScreen(UINT nr,bool XtrueYfalse);
	bool SetPozPunktu(POINT MousePos,UINT MK);
	RECT GetRectWl(bool WabTrueWtFalse);
	RECT GetRectLegendy(UINT nrFu,bool WabTrueWtFalse);
	bool SetZaznPozLeg(POINT MousePos);
	void SetUstLeg(void);
	RECT GetRectPPL(bool WabTrueWtFalse,bool UpTrueDownFalse);	//Pask�w Przewijania Legendy
	//Obliczenia
	double Wab(UINT nrFu);			//Oblicza i zwraca warto�� pracy absolutnej dla podanego numeru linii wykresu
	double Wab(void);				//Oblicza i zwraca warto�� pracy absolutnej dla ca�ego wykresu
	double Wt(UINT nrFu);			//Oblicza i zwraca warto�� pracy technicznej dla podanego numeru linii wykresu
	double Wt(void);				//Oblicza i zwraca warto�� pracy technicznej dla ca�ego wykresu
	double tempI(UINT NrPunktu);	//Obicza temperatur� gazu dla podanego numeru punktu wykresu
	//Funkcje obs�ugi rysowania
	int RysujXSkala(HDC &hdc);						//Rysowanie osi X i jej skali
	int RysujXSkala(PostScript *p);
	int RysujYSkala(HDC &hdc);						//Rysowanie osi Y i jej skali
	int RysujYSkala(PostScript *p);
	bool RysujSiatke(HDC &hdc,int dx,int dy);		//Rysowanie siatki
	bool RysujSiatke(PostScript *p,int dx,int dy);
	bool RysujProw(HDC &hdc,POINT MousePos);		//Rysowanie prowadnic
	void RysujPozMychy(HDC &hdc,POINT MousePos);	//Rysowanie wsp��dnych myszy na wykresie
	bool RysujW(HDC &hdc,POINT MousePos);			//Rysowanie wykresu (linie)
	bool RysujW(PostScript *p);
	bool RysujOzn(HDC &hdc);						//Rysowanie oznacze� wykresu (1,2,3....,ilP)
	bool RysujWab(HDC &hdc,UINT nrFu);				//Rysowanie graficznej interpretacji pracy absolutnej dla danego numeru linii wykresu
	bool RysujWab(HDC &hdc);						//Rysowanie graficznej interpretacji pracy absolutnej dla ca�ego wykresu
	bool RysujWabC(HDC &hdc);						//Rysowanie graficznej interpretacji pracy absolutnej dla ca�ego wykresu
	bool RysujWt(HDC &hdc,UINT nrFu);				//Rysowanie graficznej interpretacji pracy technicznej dla danego numeru linii wykresu
	bool RysujWt(HDC &hdc);							//Rysowanie graficznej interpretacji pracy technicznej dla ca�ego wykresu
	bool RysujWtC(HDC &hdc);						//Rysowanie graficznej interpretacji pracy technicznej dla ca�ego wykresu
	bool RysujLegend(HDC &hdc);						//Rysowanie legendy wykresu
	bool RysujCsOtoczenia(HDC &hdc,POINT MousePos);	//Rysowanie ci�nienia otoczenia
	bool RysujCsOtoczenia(PostScript *p);
	void Arrow(HDC &hdc,int x,int y,bool PionPoziom,COLORREF kWyp,int Styl,UINT gr,COLORREF kObr);
	void Arrow(PostScript *p,int x,int y,bool PionPoziom,COLORREF kWyp,int Styl,UINT gr,COLORREF kObr);
	void PasekPrzew(HDC &hdc,int x,int y);
	void PlusMinus(HDC &hdc,int x,int y,int szerIwys,int Styl,int Gr,COLORREF Kolor,bool PtrueMFalse);
	//Funkcje obs�ugi wska�nika *punkty
	void AddPoint(POINT MousePos);	//Dodaj punkt do tablicy
	bool DeletePoint(UINT nr);		//Usu� punkt z tablicy
	RECT GetAreaPrzes(void);
	RECT GetAreaCsOtoczenia(void);		//Prostok�t uchwytu do przesuwania linii ci�nienia otoczenia
	bool SetPozListyLWab(int dp);
	bool SetPozListyLWt(int dp);
public:
	TlokSpr *tlok;
	wykres(int x,int y,UINT szer,UINT wys,LPSTR jednX,LPSTR jednY,LPSTR title,double _xMax,double _yMax,double _xMin,double _yMin,UINT _xG,UINT _yG,TIMERPROC timerProc,UINT idEvent,int IdKrosCursor,int IdEmptyCursor);
	wykres(int x,int y,UINT szer,UINT wys,LPSTR jednX,LPSTR jednY,LPSTR title,double _xMax,double _yMax,double _xMin,double _yMin,UINT _xG,UINT _yG,COLORREF _kOsiX,COLORREF _kOsiY,COLORREF _kSiatki,COLORREF _kProw,COLORREF _kWykr,COLORREF _kWykr2,TIMERPROC timerProc,UINT idEvent,int IdKrosCursor,int IdEmptyCursor);
	void Timer(HWND hWnd);
	void AddPoint(double x,double y);
	//FUNKCJE USTAWIAJ�CE SET
	void SetJedn(LPSTR jednX,LPSTR jednY);			//Funkcja ustawiaj�ca jednostki wykresu
	void SetZakrX(double _xMin,double _xMax);		//Funkcja ustawiaj�ca warto�ci min i max na osi x
	void SetZakrY(double _yMin,double _yMax);		//Funkcja ustawiaj�ca warto�ci min i max na osi y
	void SetGx(UINT _xG);							//Funkcja ustawiaj�ca g�sto�� siatki no x-sie
	void SetGy(UINT _yG);							//Funkcja ustawiaj�ca g�sto�� siatki na y-ku
	bool SetRysOnOff(void);							//Funkcja w��cz/wy��cz rysowanie wykresu
	void SetTitle(LPSTR title);						//Funkcja ustawiaj�ca Tytu� wykresu
	void SetAreaW(int x,int y,UINT szer,UINT wys);	//Funkcja ustawiaj�ca obszar wykresu
	void SetkWykresu(void);							//Ustawienie koloru linii wykresu
	void SetkWykresu2(void);						//Ustawienie koloru rysowanej linii wykresu
	void SetkOsiX(void);							//Ustawienie koloru osi X
	void SetkOsiY(void);							//Ustawienie koloru osi Y
	void SetkSiatki(void);							//Ustawienie koloru siatki
	void SetkWab(void);								//Ustawienie koloru graficznej interpretacji pracy absolutnej
	void SetkWt(void);								//Ustawienie koloru graficznej interpretacji pracy technicznej
	void SetkgLegendy(void);						//Ustawienie koloru g��wnego legendy
	void SetkrLegendy(void);						//Ustawienie koloru legendy rozwijanej
	void SetkCsOtoczenia(void);						//Ustawienie koloru linii ci�nienia otoczenia
	void SetCsOtoczenia(double CsOtoczenia);		//Ustawienie Ci�nienia otoczenia
	//Funkcje ustawie� opcji typu W��CZ/WY��CZ
	bool SetProwOnOff(void);	//prowOnOff=!prowOnOff
	bool SetSkokOnOff(void);	//skokOnOff=!skokOnOff
	bool SetSiatkaOnOff(void);	//siatkaOnOff!=siatkaOnOff
	bool SetOznOnOff(void);		//oznOnOff=!oznOnOff
	bool SaveToPostScript(OPENFILENAME *opf);
	//FUNKCJE ZWRACAJ�CE GET
	bool GetProwOnOff(void);	//Pobieranie informacji o ustawieniu wy�wietlania prowadnic wykresu
	bool GetSiatkaOnOff(void);	//Pobranie informacji o ustawieniu wy�wietlania siatki wykresu
	bool GetOznOnOff(void);		//Pobieranie informacji o ustawieniu wy�wietlania oznacze� wykresu (oznOnOff/RysujOzn)
	double GetXmax(void);		//Pobieranie warto�ci maksymalnej dla osi X
	double GetYmax(void);		//Pobierania warto�ci maksymalnej dla osi Y
	dPoint GetPoint(UINT nrP);
	UINT Size(void);
	//Funkcje obs�ugi zda�e� i rysowania wykresu
	bool Rysuj(HDC &hdc,POINT MousePos,UINT MK);	//Rysowanie wykresu
	bool lButtonDown(POINT MousePos);		//Obs�uga komunikatu: lewy przycisk myszy wci�ni�ty
	bool lButtonUp(void);					//Obs�uga komunikatu: lewy przycisk myszy wy��czony
	bool MouseMove(HWND hWnd,HINSTANCE hInst,POINT MousePos,UINT MK);	//Obs�uga komunikatu: ruch myszki
	void vScroll(int dy);					//Przesuni�cie wykresu
	bool Delete(void);						//Usuni�cie zaznaczonego punktu wykresu
	bool ClearW(void);						//Wyczyszczenie wykresu
	//FUNKCJA ZAPISU USTAWIE� DO PLIKU
	void WriteToFile(char *LokalizacjaINazwa);
	//FUNKCJA ODCZYTU USTAWIE� Z PLIKU
	void ReadFromFile(char *LokalizacjaINazwa);
	//FUNKCJA ZAPISU DANYCH DO PLIKU
	void SaveToFile(OPENFILENAME* ofn);
	//FUNKCJA ODCZYTU DANYCH Z PLIKU
	bool OpenFile(OPENFILENAME* ofn);
};
#endif