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
	//Opcje typu W£¥CZ/WY£¥CZ
	bool rysOnOff;		//W³¹cz/Wy³¹cz rysowanie wykresu
	bool wykresOnOff;	//W³¹cz/Wy³¹cz wyœwietlanie wykresu
	bool siatkaOnOff;	//W³¹cz/Wy³¹cz siatkê
	bool prowOnOff;		//W³¹cz/Wy³¹cz prowafnice
	bool oznOnOff;		//W³¹cz/Wy³¹cz oznaczenia
	bool wtRozw;		//Rozwiniêcie lub zwiniêcie listy prac technicznych
	bool wabRozw;		//Rozwiniêcie lub zwiniêcie listy prac absolutnych
	bool skokOnOff;		//W³¹cz/Wy³¹cz skok
	bool csOtControl;	//Klikniêcie mych¹ kontrolki ciœnienia otoczenia
	//Ustawienia dotycz¹ce kolorów wyœwietlania
	CHOOSECOLOR cc;			//Struktura s³u¿¹ca do ustawiania kolorów wykresu za pomoc¹ funkcji ChooseColor
	COLORREF CustColors[16];//Tablica 16-nastu wybranych kolorów przeznaczona dla struktury cc (CHOOSECOLOR)
	COLORREF kOsiX;			//Kolor osi x wykresu
	COLORREF kOsiY;			//Kolor osi y wykresu
	COLORREF kSiatki;		//Kolor siatki wykresu
	COLORREF kProw;			//Kolor prowadnicy
	COLORREF kWykr;			//Kolor wykresu
	COLORREF kWykr2;		//Kolor rysowanej linii wykresu
	COLORREF kWab;			//Kolor graficznej interpretacji pracy absolutnej
	COLORREF kWt;			//Kolor graficznej interpretacji pracy technicznej
	COLORREF kgLegendy;		//Kolor g³ówny legendy
	COLORREF krLegendy;		//Kolor rozwiniêcia legendy
	COLORREF kCsOtoczenia;	//Kolor lini ciœnienia otoczenia
	//identyfikatory kursorów myszy
	int idCrossCursor;		//Krzy¿yk
	int idEmptyCursor;		//Pusta ikona kursora
	int idSelectedCursor;	//Identyfikator aktualnie zaznaczonego kursora myszy
	//
	UINT ilP;				//iloœæ utwo¿onych punktów
	UINT wysRozwLegWab;		//Wysokoœæ rozwiniêcia legendy pracy absolutnej
	UINT pozPoczRozwWab;	//Pozycja pocz¹tku rozwiniêcia legendy pracy absolutnej
	UINT wysRozwLegWt;		//Wysokoœæ rozwiniêcia legendy pracy technicznej
	UINT pozPoczRozwWt;		//Pozycja pocz¹tku rozwiniêcia legendy pracy technicznej
	POINT PozLegendy;		//Pozycja legendy
	warLEGENDY zaznPoz;		//Zaznaczenie poprzez najechanie kursorem myszy na dan¹ pozycjê legendy
	warLEGENDY ZaznPoz;		//Zaznaczenie poprzez klikniêcie danej pozycji legendy
	POINT Przes;			//Przesuniêcie wykresu
	UINT selectP;			//Zaznacz punkt
	dPoint *punkty;			//Wska¿nik do tablicy punków rysowanego wykresu
	RECT AreaW;				//Obszar wykresu
	LPSTR JednX;			//Jednostka na x-sie
	LPSTR JednY;			//Jednostka na y-ku
	LPSTR Title;			//Tytu³ wykresu
	double xMax;			//Maksymalna wartoœæ na x-sie
	double yMax;			//Maksymalna wartoœæ na y-ku
	double xMin;			//Minimalna wartoœæ na x-sie
	double yMin;			//Minimalna wartoœæ na y-ku
	double csOtoczenia;		//Ciœnienie otoczenia
	UINT xG;				//Gêstoœæ skali (siatki) na x-sie
	UINT yG;				//Gêstoœæ skali (siatki) na y-ku
	double St;				//Temperatura pocz¹tkowa gazu
	int n;					//Nr. punktu po³o¿enia t³oka
	//FUNKCJE
	int wykrToScreen(UINT nr,bool XtrueYfalse);
	bool SetPozPunktu(POINT MousePos,UINT MK);
	RECT GetRectWl(bool WabTrueWtFalse);
	RECT GetRectLegendy(UINT nrFu,bool WabTrueWtFalse);
	bool SetZaznPozLeg(POINT MousePos);
	void SetUstLeg(void);
	RECT GetRectPPL(bool WabTrueWtFalse,bool UpTrueDownFalse);	//Pasków Przewijania Legendy
	//Obliczenia
	double Wab(UINT nrFu);			//Oblicza i zwraca wartoœæ pracy absolutnej dla podanego numeru linii wykresu
	double Wab(void);				//Oblicza i zwraca wartoœæ pracy absolutnej dla ca³ego wykresu
	double Wt(UINT nrFu);			//Oblicza i zwraca wartoœæ pracy technicznej dla podanego numeru linii wykresu
	double Wt(void);				//Oblicza i zwraca wartoœæ pracy technicznej dla ca³ego wykresu
	double tempI(UINT NrPunktu);	//Obicza temperaturê gazu dla podanego numeru punktu wykresu
	//Funkcje obs³ugi rysowania
	int RysujXSkala(HDC &hdc);						//Rysowanie osi X i jej skali
	int RysujXSkala(PostScript *p);
	int RysujYSkala(HDC &hdc);						//Rysowanie osi Y i jej skali
	int RysujYSkala(PostScript *p);
	bool RysujSiatke(HDC &hdc,int dx,int dy);		//Rysowanie siatki
	bool RysujSiatke(PostScript *p,int dx,int dy);
	bool RysujProw(HDC &hdc,POINT MousePos);		//Rysowanie prowadnic
	void RysujPozMychy(HDC &hdc,POINT MousePos);	//Rysowanie wspó³¿êdnych myszy na wykresie
	bool RysujW(HDC &hdc,POINT MousePos);			//Rysowanie wykresu (linie)
	bool RysujW(PostScript *p);
	bool RysujOzn(HDC &hdc);						//Rysowanie oznaczeñ wykresu (1,2,3....,ilP)
	bool RysujWab(HDC &hdc,UINT nrFu);				//Rysowanie graficznej interpretacji pracy absolutnej dla danego numeru linii wykresu
	bool RysujWab(HDC &hdc);						//Rysowanie graficznej interpretacji pracy absolutnej dla ca³ego wykresu
	bool RysujWabC(HDC &hdc);						//Rysowanie graficznej interpretacji pracy absolutnej dla ca³ego wykresu
	bool RysujWt(HDC &hdc,UINT nrFu);				//Rysowanie graficznej interpretacji pracy technicznej dla danego numeru linii wykresu
	bool RysujWt(HDC &hdc);							//Rysowanie graficznej interpretacji pracy technicznej dla ca³ego wykresu
	bool RysujWtC(HDC &hdc);						//Rysowanie graficznej interpretacji pracy technicznej dla ca³ego wykresu
	bool RysujLegend(HDC &hdc);						//Rysowanie legendy wykresu
	bool RysujCsOtoczenia(HDC &hdc,POINT MousePos);	//Rysowanie ciœnienia otoczenia
	bool RysujCsOtoczenia(PostScript *p);
	void Arrow(HDC &hdc,int x,int y,bool PionPoziom,COLORREF kWyp,int Styl,UINT gr,COLORREF kObr);
	void Arrow(PostScript *p,int x,int y,bool PionPoziom,COLORREF kWyp,int Styl,UINT gr,COLORREF kObr);
	void PasekPrzew(HDC &hdc,int x,int y);
	void PlusMinus(HDC &hdc,int x,int y,int szerIwys,int Styl,int Gr,COLORREF Kolor,bool PtrueMFalse);
	//Funkcje obs³ugi wskaŸnika *punkty
	void AddPoint(POINT MousePos);	//Dodaj punkt do tablicy
	bool DeletePoint(UINT nr);		//Usuñ punkt z tablicy
	RECT GetAreaPrzes(void);
	RECT GetAreaCsOtoczenia(void);		//Prostok¹t uchwytu do przesuwania linii ciœnienia otoczenia
	bool SetPozListyLWab(int dp);
	bool SetPozListyLWt(int dp);
public:
	TlokSpr *tlok;
	wykres(int x,int y,UINT szer,UINT wys,LPSTR jednX,LPSTR jednY,LPSTR title,double _xMax,double _yMax,double _xMin,double _yMin,UINT _xG,UINT _yG,TIMERPROC timerProc,UINT idEvent,int IdKrosCursor,int IdEmptyCursor);
	wykres(int x,int y,UINT szer,UINT wys,LPSTR jednX,LPSTR jednY,LPSTR title,double _xMax,double _yMax,double _xMin,double _yMin,UINT _xG,UINT _yG,COLORREF _kOsiX,COLORREF _kOsiY,COLORREF _kSiatki,COLORREF _kProw,COLORREF _kWykr,COLORREF _kWykr2,TIMERPROC timerProc,UINT idEvent,int IdKrosCursor,int IdEmptyCursor);
	void Timer(HWND hWnd);
	void AddPoint(double x,double y);
	//FUNKCJE USTAWIAJ¥CE SET
	void SetJedn(LPSTR jednX,LPSTR jednY);			//Funkcja ustawiaj¹ca jednostki wykresu
	void SetZakrX(double _xMin,double _xMax);		//Funkcja ustawiaj¹ca wartoœci min i max na osi x
	void SetZakrY(double _yMin,double _yMax);		//Funkcja ustawiaj¹ca wartoœci min i max na osi y
	void SetGx(UINT _xG);							//Funkcja ustawiaj¹ca gêstoœæ siatki no x-sie
	void SetGy(UINT _yG);							//Funkcja ustawiaj¹ca gêstoœæ siatki na y-ku
	bool SetRysOnOff(void);							//Funkcja w³¹cz/wy³¹cz rysowanie wykresu
	void SetTitle(LPSTR title);						//Funkcja ustawiaj¹ca Tytu³ wykresu
	void SetAreaW(int x,int y,UINT szer,UINT wys);	//Funkcja ustawiaj¹ca obszar wykresu
	void SetkWykresu(void);							//Ustawienie koloru linii wykresu
	void SetkWykresu2(void);						//Ustawienie koloru rysowanej linii wykresu
	void SetkOsiX(void);							//Ustawienie koloru osi X
	void SetkOsiY(void);							//Ustawienie koloru osi Y
	void SetkSiatki(void);							//Ustawienie koloru siatki
	void SetkWab(void);								//Ustawienie koloru graficznej interpretacji pracy absolutnej
	void SetkWt(void);								//Ustawienie koloru graficznej interpretacji pracy technicznej
	void SetkgLegendy(void);						//Ustawienie koloru g³ównego legendy
	void SetkrLegendy(void);						//Ustawienie koloru legendy rozwijanej
	void SetkCsOtoczenia(void);						//Ustawienie koloru linii ciœnienia otoczenia
	void SetCsOtoczenia(double CsOtoczenia);		//Ustawienie Ciœnienia otoczenia
	//Funkcje ustawieñ opcji typu W£¥CZ/WY£¥CZ
	bool SetProwOnOff(void);	//prowOnOff=!prowOnOff
	bool SetSkokOnOff(void);	//skokOnOff=!skokOnOff
	bool SetSiatkaOnOff(void);	//siatkaOnOff!=siatkaOnOff
	bool SetOznOnOff(void);		//oznOnOff=!oznOnOff
	bool SaveToPostScript(OPENFILENAME *opf);
	//FUNKCJE ZWRACAJ¥CE GET
	bool GetProwOnOff(void);	//Pobieranie informacji o ustawieniu wyœwietlania prowadnic wykresu
	bool GetSiatkaOnOff(void);	//Pobranie informacji o ustawieniu wyœwietlania siatki wykresu
	bool GetOznOnOff(void);		//Pobieranie informacji o ustawieniu wyœwietlania oznaczeñ wykresu (oznOnOff/RysujOzn)
	double GetXmax(void);		//Pobieranie wartoœci maksymalnej dla osi X
	double GetYmax(void);		//Pobierania wartoœci maksymalnej dla osi Y
	dPoint GetPoint(UINT nrP);
	UINT Size(void);
	//Funkcje obs³ugi zda¿eñ i rysowania wykresu
	bool Rysuj(HDC &hdc,POINT MousePos,UINT MK);	//Rysowanie wykresu
	bool lButtonDown(POINT MousePos);		//Obs³uga komunikatu: lewy przycisk myszy wciœniêty
	bool lButtonUp(void);					//Obs³uga komunikatu: lewy przycisk myszy wy³¹czony
	bool MouseMove(HWND hWnd,HINSTANCE hInst,POINT MousePos,UINT MK);	//Obs³uga komunikatu: ruch myszki
	void vScroll(int dy);					//Przesuniêcie wykresu
	bool Delete(void);						//Usuniêcie zaznaczonego punktu wykresu
	bool ClearW(void);						//Wyczyszczenie wykresu
	//FUNKCJA ZAPISU USTAWIEÑ DO PLIKU
	void WriteToFile(char *LokalizacjaINazwa);
	//FUNKCJA ODCZYTU USTAWIEÑ Z PLIKU
	void ReadFromFile(char *LokalizacjaINazwa);
	//FUNKCJA ZAPISU DANYCH DO PLIKU
	void SaveToFile(OPENFILENAME* ofn);
	//FUNKCJA ODCZYTU DANYCH Z PLIKU
	bool OpenFile(OPENFILENAME* ofn);
};
#endif