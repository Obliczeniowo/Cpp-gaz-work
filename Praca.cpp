#include <windows.h>
#include <math.h>
#include <fstream>
#include <commdlg.h>
#include <stdlib.h>

#include "resource.h"

#include "ObsRysowania.h"
ObsRys *RYS = new ObsRys;
#include "klasaTlok.h"
#include "myMath.h"
#include "Klasy.h"
#include "Zegar.h"
#include "PostScript.h"
#include "PointsClass.h"

#include "Application.h"
#include "kompilator.h"

HWND okno;
HINSTANCE hInst;
HBITMAP tlo;
char* Path;

HWND hOProgramie;

HBITMAP hLogo;
HBITMAP hLogoMask;

VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
VOID CALLBACK TimerProcZeg(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);

wykres W(50,40,700,500,"V [m3]","P [Ba]","Wykres funkcji P(V)",10,5,0,0,11,11,RGB(64,156,172),RGB(88,154,29),RGB(61,112,114),RGB(0,150,200),RGB(113,125,73),RGB(0,255,155),TimerProc,1,IDC_CURSOR1,IDC_CURSOR2);

VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime){
	W.Timer(hwnd);
}

Zegar *zeg=NULL;

VOID CALLBACK TimerProcZeg(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime){
	zeg->Timer();
}

LRESULT CALLBACK DlgFuncProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam){
	static HWND hMin, hMax, hFu;
	switch(msg){
		case WM_INITDIALOG:
			{
				char title[50]="Zmiana skali";
				SetWindowText(hDlg,title);
				hMin	= GetDlgItem(hDlg,ID_MIN);
				hMax	= GetDlgItem(hDlg,ID_MAX);
				hFu		= GetDlgItem(hDlg,ID_FUNCTION);
				_gcvt(W.GetXmax(),10,title);
				SetWindowText(hMax,title);
				SetWindowText(hMin,"0.1");
			}
			return true;
		case WM_COMMAND:
			if(LOWORD(wParam)==IDOK){
				char title[1000]="";
				GetWindowText(hMax,title,1000);
				double _max = atof(title);
				GetWindowText(hMin,title,1000);
				double _min = atof(title);
				GetWindowText(hFu,title,1000);
				Kompilator k("");
				k.AddVarible("x",_min);
				k.Str(title);
				W.ClearW();
				double dx = (_max  - _min) / 100.0;
				double *x = k.VaribleAddress("v");
				double m = 0;
				while(*x < _max){
					double y = abs(k.Calculate());
					*x += dx;
					W.AddPoint(*x,y);
					m = max(y,m);
				}
				W.SetZakrY(0,m);
				W.SetZakrX(0,_max);
				EndDialog(hDlg,LOWORD(wParam));
			}else if(LOWORD(wParam) == IDCALCULATE){
				char title[1000]="";
				GetWindowText(hMax,title,1000);
				double _max = atof(title);
				GetWindowText(hMin,title,1000);
				double _min = atof(title);
				GetWindowText(hFu,title,1000);
				Kompilator k("");
				k.AddVarible("v",_min);
				k.Str(title);
				W.ClearW();
				double dx = (_max  - _min) / 100.0;
				double *x = k.VaribleAddress("v");
				double m = 0;
				while(*x < _max){
					double y = abs(k.Calculate());
					*x += dx;
					W.AddPoint(*x,y);
					m = max(y,m);
				}
				W.SetZakrY(0,m);
				W.SetZakrX(0,_max);
				InvalidateRect(okno,NULL,false);
			}else if(LOWORD(wParam)==IDCANCEL){
				EndDialog(hDlg,LOWORD(wParam));
			}
			return true;
		default:
			return false;
	}
}
//Okno dialogowe
LRESULT CALLBACK DlgProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam){
	static HWND hXmax, hYmax;
	switch(msg){
		case WM_INITDIALOG:
			{
				char title[50]="Zmiana skali";
				SetWindowText(hDlg,title);
				hXmax=GetDlgItem(hDlg,ID_XMAX);
				hYmax=GetDlgItem(hDlg,ID_YMAX);
				_gcvt(W.GetXmax(),10,title);
				SetWindowText(hXmax,title);
				_gcvt(W.GetYmax(),10,title);
				SetWindowText(hYmax,title);
			}
			return true;
		case WM_COMMAND:
			if(LOWORD(wParam)==IDOK){
				char title[20]="";
				GetWindowText(hXmax,title,19);
				W.SetZakrX(0,atof(title));
				GetWindowText(hYmax,title,19);
				W.SetZakrY(0,atof(title));
				EndDialog(hDlg,LOWORD(wParam));
			}else if(LOWORD(wParam)==IDCANCEL){
				EndDialog(hDlg,LOWORD(wParam));
			}
			return true;
		default:
			return false;
	}
}
//Okno dialogowe O programie
LRESULT CALLBACK oProgramie(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hTloP;
	static RECT WndSize;
	static RECT Close;
	static RECT Move;
	static POINT MousePos;
	static HBITMAP hClose,hCloseM;
	static COLORREF kTla=RGB(0,0,0);
	switch (message)
	{
	case WM_CREATE:
		SetWindowText(hDlg,"Pasowanie - ustawienie");
		GetClientRect(hDlg,&WndSize);
		SetRect(&Close,WndSize.right-21,0,WndSize.right,21);
		GetCursorPos(&MousePos);
		hClose=LoadBitmap(hInst,(LPSTR)CLOSE);
		hCloseM=LoadBitmap(hInst,(LPSTR)CLOSEM);
		break;
	case WM_SIZE:
		GetClientRect(hDlg,&WndSize);
		DeleteObject(hTloP);
		hTloP=CreateCompatibleBitmap(GetDC(NULL),WndSize.right,WndSize.bottom);
		break;
	case WM_LBUTTONDOWN:
		if(PtInRect(&Close,MousePos))
			DestroyWindow(hDlg);
		break;
	case WM_MOUSEMOVE:
		MousePos.x=LOWORD(lParam);
		MousePos.y=HIWORD(lParam);
		//InvalidateRect(hDlg,NULL,false);
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc=BeginPaint(hDlg,&ps);
			HDC mem=CreateCompatibleDC(hdc);
			SelectObject(mem,hTloP);
			SetBkMode(mem,TRANSPARENT);
			SetTextColor(mem,RGB(0,240,255));
			RYS->rectangle(mem,WndSize,RGB(255,255,255),PS_SOLID,1,kTla);
			RYS->MaskBmp(mem,0,0,160,160,hLogo,hLogoMask);
			RYS->drawText(mem,"Autor programu:",-1,150,50,500,20,DT_SINGLELINE|DT_VCENTER);
			RYS->drawText(mem,"Krzysztof Zaj¹czkowski ProgrammingMalyszKZ@gmail.com",-1,0,10,410,20,DT_RIGHT);
			RYS->drawText(mem,"Wersja:",-1,150,90,500,20,DT_SINGLELINE|DT_VCENTER);
			RYS->drawText(mem,"1.0",-1,257,110,40,20,DT_SINGLELINE|DT_VCENTER);
			RYS->drawText(mem,"Data kompilacji:",-1,150,130,500,20,DT_SINGLELINE|DT_VCENTER);
			RYS->drawText(mem,"23 lipca 2006 godz. 18:07",-1,257,150,200,20,DT_SINGLELINE|DT_VCENTER);
			RYS->StretchMaskBmp(mem,Close.left,Close.top,Close.right-Close.left,Close.bottom-Close.top,hClose,hCloseM);
			BitBlt(hdc,0,0,WndSize.right,WndSize.bottom,mem,0,0,SRCCOPY);
			DeleteDC(mem);
			EndPaint(hDlg,&ps);
		}
		break;
	case WM_COMMAND:

		break;
	case WM_DESTROY:
		
		break;
	}
	return DefWindowProc(hDlg,message,wParam,lParam);
}

//Procedura obs³ugi okna
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	static RECT WndSize;
	static POINT MousePos;
	static UINT MK;
	static COLORREF kTla=RGB(0,0,0);
	static LOGFONT lf;
    static OPENFILENAME ofn;
    static TCHAR szFileName [MAX_PATH], szTitleName [MAX_PATH] ;
    static TCHAR szFilter[] = TEXT ("dane (*.dn)\0*.dn\0");
	static HBITMAP hBmp;
	switch(msg){
		case WM_CREATE:{
				ofn.lStructSize       = sizeof (OPENFILENAME);
				ofn.hwndOwner         = hWnd;
				ofn.hInstance         = NULL;
				ofn.lpstrFilter       = szFilter;
				ofn.lpstrCustomFilter = NULL;
				ofn.nMaxCustFilter    = 0;
				ofn.nFilterIndex      = 0;
				ofn.lpstrFile         = szFileName;
				ofn.nMaxFile          = MAX_PATH;
				ofn.lpstrFileTitle    = szTitleName;
				ofn.nMaxFileTitle     = MAX_PATH;
				ofn.lpstrInitialDir   = NULL;
				ofn.lpstrTitle        = NULL;
				ofn.Flags             = 0;
				ofn.nFileOffset       = 0;
				ofn.nFileExtension    = 0;
				ofn.lpstrDefExt       = TEXT ("dn");
				ofn.lCustData         = 0;
				ofn.lpfnHook          = NULL;
				ofn.lpTemplateName    = NULL;
				HDC hdc=GetDC(hWnd);
				HFONT hFont=CreateFont(12,0,0,0,0,FW_NORMAL,0,0,DEFAULT_CHARSET,0,0,0,0,NULL);
				GetObject(hFont,sizeof(lf),&lf);
				DeleteObject(hFont);
				ReleaseDC(hWnd,hdc);
				//SetScrollRange(hWnd,SB_VERT,0,100,true);
				hLogo=LoadBitmap(hInst,(LPCTSTR)LOGO);
				hLogoMask=LoadBitmap(hInst,(LPCTSTR)LOGO_MASK);
				//tlok.StartTimer(hWnd);
				W.tlok->StartTimer(hWnd);
				std::string file;
				Application::StartupFile(file,(std::string) "t³o.bmp");
				hBmp=RYS->GetBitmapFile(file.c_str());
				/*PostScript *p=new PostScript("C:\\bmp.ps");
				p->DrawBitmap(0,0,hBmp);
				delete p;*/
				zeg=new Zegar(650,590,0,0,hWnd,TimerProcZeg,2,RGB(255,255,0),RGB(100,100,0));
			}
			break;
		case WM_TIMER:

			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case ID_FUNC_DRAW:
					DialogBox(hInst,(LPCTSTR)IDD_DIALOG2,hWnd,(DLGPROC)DlgFuncProc);
					break;
				case ID_ZAMKNIJ:
					SendMessage(hWnd,WM_DESTROY,NULL,NULL);
					break;
				case ID_OTW:
					W.OpenFile(&ofn);
					break;
				case ID_ZAPISZ:
					W.SaveToFile(&ofn);
					break;
				case ID_WYCZ:
					W.ClearW();
					break;
				case ID_KOLORY_KOLOROSIX:
					W.SetkOsiX();
					break;
				case ID_KOLORY_KOLOROSIY:
					W.SetkOsiY();
					break;
				case ID_KOLORY_KOLORSIATKI:
					W.SetkSiatki();
					break;
				case ID_KOLORY_KOLORWYKRESU:
					W.SetkWykresu();
					break;
				case ID_KOLORY_RYSOWANIA:
					W.SetkWykresu2();
					break;
				case ID_KOLORY_WAB:
					W.SetkWab();
					break;
				case ID_KOLORY_WT:
					W.SetkWt();
					break;
				case ID_KOLORY_RL:
					W.SetkrLegendy();
					break;
				case ID_KOLORY_GL:
					W.SetkgLegendy();
					break;
				case ID_KOLORY_CS_OT:
					W.SetkCsOtoczenia();
					break;
				case ID_W40023://W³¹cz/Wy³¹cz prowadnice
					CheckMenuItem(GetMenu(hWnd),ID_W40023,W.SetProwOnOff()?MF_CHECKED:MF_UNCHECKED);
					break;
				case ID_W40014://W³¹cz/Wy³¹cz siatka
					CheckMenuItem(GetMenu(hWnd),ID_W40014,W.SetSiatkaOnOff()?MF_CHECKED:MF_UNCHECKED);
					break;
				case ID_W40029://W³¹cz/Wy³¹cz oznaczenia
					CheckMenuItem(GetMenu(hWnd),ID_W40029,W.SetOznOnOff()?MF_CHECKED:MF_UNCHECKED);
					break;
				case ID_ANIMACJA:
					if(W.tlok->GetTimerOnOff()){
						CheckMenuItem(GetMenu(hWnd),ID_ANIMACJA,MF_UNCHECKED);
						W.tlok->StopTimer(hWnd);
					}else{
						CheckMenuItem(GetMenu(hWnd),ID_ANIMACJA,MF_CHECKED);
						W.tlok->StartTimer(hWnd);
					}
					break;
				case ID_W40032:
					CheckMenuItem(GetMenu(hWnd),ID_W40032,W.SetSkokOnOff()?MF_CHECKED:MF_UNCHECKED);
					break;
				case ID_OSIEXY:
					DialogBox(hInst,(LPCTSTR)IDD_DIALOG1,hWnd,(DLGPROC)DlgProc);
					break;
				case O_PROGRAMIE:
					hOProgramie=CreateWindow("oProgramie","",WS_POPUP,GetSystemMetrics(SM_CXSCREEN)/2-250,GetSystemMetrics(SM_CYSCREEN)/2-85,500,170,okno,NULL,hInst,NULL);
					ShowWindow(hOProgramie,SW_SHOWNORMAL);
					UpdateWindow(hOProgramie);
					break;
				case ID_POSTSCRIPT:
					{
						TCHAR szFileName [MAX_PATH], szTitleName [MAX_PATH];
						szFileName[0]=NULL;
						szTitleName[0]=NULL;
						TCHAR szFilter[] = TEXT ("wykres (*.ps)\0*.ps\0");
						OPENFILENAME of;
						of.lStructSize       = sizeof (OPENFILENAME);
						of.hwndOwner         = hWnd;
						of.hInstance         = NULL;
						of.lpstrFilter       = szFilter;
						of.lpstrCustomFilter = NULL;
						of.nMaxCustFilter    = 0;
						of.nFilterIndex      = 0;
						of.lpstrFile         = szFileName;
						of.nMaxFile          = MAX_PATH;
						of.lpstrFileTitle    = szTitleName;
						of.nMaxFileTitle     = MAX_PATH;
						of.lpstrInitialDir   = NULL;
						of.lpstrTitle        = NULL;
						of.Flags             = 0;
						of.nFileOffset       = 0;
						of.nFileExtension    = 0;
						of.lpstrDefExt       = TEXT ("dn");
						of.lCustData         = 0;
						of.lpfnHook          = NULL;
						of.lpTemplateName    = NULL;
						W.SaveToPostScript(&of);
					}
					break;
			}
			break;
		case WM_KEYDOWN:
			switch((int)wParam){
				case VK_RETURN:
					W.SetRysOnOff();
					InvalidateRect(hWnd,NULL,false);
					break;
				case VK_DELETE:
					W.Delete();
					InvalidateRect(hWnd,NULL,false);
					break;
			}
			break;
		case WM_SIZE:
			{
				GetClientRect(hWnd,&WndSize);
				DeleteObject(tlo);
				tlo=CreateCompatibleBitmap(GetDC(NULL),WndSize.right,WndSize.bottom);
			}
			break;
		case WM_VSCROLL:{
			int vScrollPos=-1;
				switch(LOWORD(wParam)){
					case SB_LINEUP:
						vScrollPos=GetScrollPos(hWnd,SB_VERT)-1;
						break;
					case SB_LINEDOWN:
						vScrollPos=GetScrollPos(hWnd,SB_VERT)+1;
						break;
					case SB_PAGEUP:
						vScrollPos=GetScrollPos(hWnd,SB_VERT)-5;
						break;
					case SB_PAGEDOWN:
						vScrollPos=GetScrollPos(hWnd,SB_VERT)+5;
						break;
					case SB_THUMBPOSITION:
						vScrollPos=HIWORD(wParam);
						break;
					case SB_TOP:

						break;
					case SB_BOTTOM:
						break;
					case SB_ENDSCROLL:
						break;
				}
				if(vScrollPos!=-1){
					SetScrollPos(hWnd,SB_VERT,vScrollPos,true);
					W.vScroll(vScrollPos);
					InvalidateRect(hWnd,NULL,false);
				}
			}
			break;
		case WM_LBUTTONDOWN:
			MK=(UINT)MK_LBUTTON;
			if(W.lButtonDown(MousePos))
				InvalidateRect(hWnd,NULL,false);
			break;
		case WM_LBUTTONUP:
			MK=(UINT)wParam;
			W.lButtonUp();
			break;
		case WM_RBUTTONDOWN:
			MK=(UINT)MK_RBUTTON;
			break;
		case WM_RBUTTONUP:
			MK=(UINT)wParam;
			break;
		case WM_MOUSEMOVE:
			MousePos.x = LOWORD(lParam); 
			MousePos.y = HIWORD(lParam);
            MK=(UINT)wParam;
			W.MouseMove(hWnd,hInst,MousePos,MK);
			zeg->Timer();
			W.Timer(hWnd);
			InvalidateRect(hWnd,NULL,false);
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc=BeginPaint(hWnd,&ps);
				HDC mem=CreateCompatibleDC(hdc);
				SelectObject(mem,tlo);
				RYS->fillRectangle(mem,WndSize,kTla);
				RYS->bitBltBmpOnHdc(mem,hBmp,0,0);
				SetBkMode(mem,TRANSPARENT);
				W.Rysuj(mem,MousePos,MK);
				zeg->Timer();
				zeg->Rysuj(mem);
				BitBlt(hdc,0,0,WndSize.right,WndSize.bottom,mem,0,0,SRCCOPY);
				DeleteDC(mem);
				EndPaint(hWnd,&ps);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE ,LPSTR CmdLine,int Show){
	hInst = hInstance;
	
	WNDCLASS wnd;
	
	wnd.cbClsExtra		= NULL;
	wnd.cbWndExtra		= NULL;
	wnd.hbrBackground	= 0;
	wnd.hCursor			= LoadCursor(NULL,IDC_ARROW);
	wnd.hIcon			= LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	wnd.hInstance		= hInstance;
	wnd.lpfnWndProc		= WndProc;
	wnd.lpszClassName	= "okno";
	wnd.lpszMenuName	= (LPCSTR)IDR_MENU1;
	wnd.style			= CS_VREDRAW|CS_HREDRAW;

	if(!RegisterClass(&wnd)) return 0;

	wnd.lpfnWndProc		= oProgramie;
	wnd.lpszClassName	= "oProgramie";
	wnd.lpszMenuName	= NULL;

	if(!RegisterClass(&wnd)) return 0;

	//StartupFile(Path,"\\ini.ini");

	std::string temp;

	Application::StartupFile(temp,(std::string)"ini.ini");

	Path = new char[temp.size()];

	strcpy(Path,temp.c_str());

	okno = CreateWindow("okno","Praca - obliczenia",WS_POPUP,0,0,800,600,NULL,NULL,hInstance,NULL);

	if(!okno) return 0;

	W.ReadFromFile(Path);	//Wczytywanie ustawieñ z pliku

	ShowWindow(okno,SW_SHOWMAXIMIZED);
	UpdateWindow(okno);

	PostScript *p = new PostScript("C:\\a.ps");
	//p->SetRGBColor(255,100,100);
	int gr[] = {1,3,1};
	int ods[] = {0,4,10};
	p->gsave();
	p->SetRGBColor(0,205,255);
	p->grestore();
	p->SetFont(TimesNewRoman,40);
	p->frameRect(0,0,A3_WIDTH_DPI,A3_HEIGHT_DPI,gr,ods,3,3,RGB(0,205,155));
	p->showDrawText("Strona tytulowa",p->frameRect(10,A3_HEIGHT_DPI-110,A3_WIDTH_DPI-20,100,gr,&ods[1],2,1,RGB(255,255,255)),RGB(0,205,255),RGB(255,255,0),DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	p->stroke();
	p->gsave();
	p->SetRGBColor(255,155,0);
	p->roundRect(20,A3_HEIGHT_DPI-220,UINT(A3_WIDTH_DPI-40),UINT(100),UINT(50),UINT(50),FILL);
	p->grestore();
	p->roundRect(20,A3_HEIGHT_DPI-220,UINT(A3_WIDTH_DPI-40),UINT(100),UINT(50),UINT(50),STROKE);
	p->frameCircle(70,A3_HEIGHT_DPI-170,50,gr,ods,3,2,RGB(0,205,255));
	p->showDrawText("A",70,A3_HEIGHT_DPI-170-int(p->GetFontSize()/4),DT_CENTER);
	delete p;

	//Ustawienie zaznaczenia pozycji w menu wzglêdem wczytanych danych z pliku
	CheckMenuItem(GetMenu(okno),ID_W40023,W.GetProwOnOff()?MF_CHECKED:MF_UNCHECKED);	//W³¹cz/Wy³¹cz prowadnice
	CheckMenuItem(GetMenu(okno),ID_W40014,W.GetSiatkaOnOff()?MF_CHECKED:MF_UNCHECKED);	//W³¹cz/Wy³¹cz siatkê
	CheckMenuItem(GetMenu(okno),ID_W40029,W.GetOznOnOff()?MF_CHECKED:MF_UNCHECKED);		//W³¹cz/Wy³¹cz oznaczenia

	MSG msg;

	while(GetMessage(&msg,0,0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	W.WriteToFile(Path);	//Zapisywanie ustawieñ w pliku

	return 0;
}