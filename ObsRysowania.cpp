#include <windows.h>
#include <math.h>

#include "ObsRysowania.h"

RECT GetRect(int x,int y,UINT szer,UINT wys){
	RECT rect;
	SetRect(&rect,x,y,x+szer,y+wys);
	return rect;
}

HBITMAP ObsRys::GetBitmapFile(std::string szFileName)
{
     BITMAPFILEHEADER bmfh ;
     BITMAPINFO     * pbmi ;
     BYTE           * pBits ;
     BOOL             bSuccess ;
     DWORD            dwInfoSize, dwBytesRead ;
     HANDLE           hFile ;
     HBITMAP          hBitmap ;

     // Otwó¿ plik: z dostêpem do odczytu, zkakaz dostêpu do zapisu w pliku

	 char temp[1000];
	 strcpy(temp,szFileName.c_str());

     hFile = CreateFile (temp, GENERIC_READ, FILE_SHARE_READ,
                         NULL, OPEN_EXISTING, 0, NULL) ;

     if (hFile == INVALID_HANDLE_VALUE)
          return NULL ;

     // Czytanie w BITMAPFILEHEADER

     bSuccess = ReadFile (hFile, &bmfh, sizeof (BITMAPFILEHEADER), 
                          &dwBytesRead, NULL) ;

     if (!bSuccess || (dwBytesRead != sizeof (BITMAPFILEHEADER))         
                   || (bmfh.bfType != * (WORD *) "BM"))
     {
          CloseHandle (hFile) ;
          return NULL ;
     }

	// Zarezerwowanie pamiêci dla BITMAPINFO struktury i czytanie jej w

     dwInfoSize = bmfh.bfOffBits - sizeof (BITMAPFILEHEADER) ;

     pbmi =(BITMAPINFO*) malloc (dwInfoSize) ;

     bSuccess = ReadFile (hFile, pbmi, dwInfoSize, &dwBytesRead, NULL) ;

     if (!bSuccess || (dwBytesRead != dwInfoSize))
     {
          free (pbmi) ;
          CloseHandle (hFile) ;
          return NULL ;
     }

     // Tworzenie DIB sekcji

     hBitmap = CreateDIBSection (NULL, pbmi, DIB_RGB_COLORS,(void**) &pBits, NULL, 0) ;

     if (hBitmap == NULL)
     {
          free (pbmi) ;
          CloseHandle (hFile) ;
          return NULL ;
     }

     // Czytanie w bitmapie bitów

     ReadFile (hFile, pBits, bmfh.bfSize - bmfh.bfOffBits, &dwBytesRead, NULL) ;

     free (pbmi) ;
     CloseHandle (hFile) ;

     return hBitmap ;
}

void ObsRys::CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,HBITMAP hBMP, HDC hDC){
    HANDLE hf;					// uchwyt pliku 
    BITMAPFILEHEADER hdr;		// pliku bitmapy nag³ówek
    PBITMAPINFOHEADER pbih;		// bitmapy info-nag³ówek
    LPBYTE lpBits;				// wskaŸnik pamiêci
    DWORD dwTotal;				// ca³kowity licznik bajtów (ang bytes)
    DWORD cb;					// incremental licznik bajtów 
    BYTE *hp;					// wskaŸnik bajtów
    DWORD dwTmp; 

    pbih = (PBITMAPINFOHEADER) pbi; 
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    // Odzyskiwanie tablicy kolorów (RGBQUAD szyk) oraz bitów
    // (szyk palety indeksów ang palette indices) z DIB. 
	GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS);

    // Two¿enie .BMP pliku. 
    hf = CreateFile(pszFile, GENERIC_READ | GENERIC_WRITE, (DWORD) 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL); 

    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
    // Obliczenie (ang compute) rozmiaru ca³ego (ang entire) pliku. 
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage); 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 

    // Obliczenie przesuniêcia (offset) do szyku (ang array) indeksów koloru. 
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof (RGBQUAD); 

    // Kopiuje BITMAPFILEHEADER do .BMP pliku. 
    WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp,  NULL);

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
	WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER)+ pbih->biClrUsed * sizeof (RGBQUAD), (LPDWORD) &dwTmp, ( NULL));

    // Copy the array of color indices into the .BMP file. 
    dwTotal = cb = pbih->biSizeImage; 
    hp = lpBits; 
    WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL);

    // Close the .BMP file. 
	CloseHandle(hf);


    // Free memory. 
    GlobalFree((HGLOBAL)lpBits);
}

PBITMAPINFO ObsRys::CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{ 
    BITMAP bmp; 
    PBITMAPINFO pbmi; 
    WORD    cClrBits; 

    // Retrieve the bitmap color format, width, and height. 
    GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp);

    // Convert the color format to a count of bits. 
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
    if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4) 
        cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else cClrBits = 32; 

    // Allocate memory for the BITMAPINFO structure. (This structure 
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
    // data structures.) 

     if (cClrBits != 24) 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER) + 
                    sizeof(RGBQUAD) * (1<< cClrBits)); 

     // There is no RGBQUAD array for the 24-bit-per-pixel format. 

     else 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER)); 

    // Initialize the fields in the BITMAPINFO structure. 

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

    // If the bitmap is not compressed, set the BI_RGB flag. 
    pbmi->bmiHeader.biCompression = BI_RGB; 

    // Compute the number of bytes in the array of color 
    // indices and store the result in biSizeImage. 
    // For Windows NT, the width must be DWORD aligned unless 
    // the bitmap is RLE compressed. This example shows this. 
    // For Windows 95/98/Me, the width must be WORD aligned unless the 
    // bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8 * pbmi->bmiHeader.biHeight; 
    // Set biClrImportant to 0, indicating that all of the 
    // device colors are important. 
     pbmi->bmiHeader.biClrImportant = 0; 
     return pbmi; 
 } 

void ObsRys::Pion(HDC &hdc,int dx,int dy,COLORREF kolor,double skala){
	if(skala<0.5)
		skala=.5;
	POINT punkty[]={(int)((1+dx)*skala),(int)(dy*skala),(int)((2+dx)*skala),(int)((dy+1)*skala),(int)((2+dx)*skala),(int)((dy+6)*skala),(int)((dx+1)*skala),(int)((dy+7)*skala),(int)(dx*skala),(int)((dy+6)*skala),(int)(dx*skala),(int)((dy+1)*skala)};
	HPEN hP=hPen;
	HBRUSH hBr=hBrush;
	SetPen(hdc,(HPEN)GetStockObject(NULL_PEN));
	SetBrush(hdc,CreateSolidBrush(kolor));
	polygon(hdc,punkty,6);
	SetPen(hdc,hP);
	SetBrush(hdc,hBr);
}

void ObsRys::Poziom(HDC &hdc,int dx,int dy,COLORREF kolor,double skala){
	if(skala<0.5)
		skala=0.5;
	POINT punkty[]={(int)(dx*skala),(int)((dy+1)*skala),(int)((dx+1)*skala),(int)(dy*skala),(int)((dx+6)*skala),(int)(dy*skala),(int)((dx+7)*skala),(int)((dy+1)*skala),(int)((dx+6)*skala),(int)((dy+2)*skala),(int)((dx+1)*skala),(int)((dy+2)*skala)};
	HPEN hP=hPen;
	HBRUSH hBr=hBrush;
	SetPen(hdc,(HPEN)GetStockObject(NULL_PEN));
	SetBrush(hdc,CreateSolidBrush(kolor));
	polygon(hdc,punkty,6);
	SetPen(hdc,hP);
	SetBrush(hdc,hBr);
}

void ObsRys::CyfryWysEl(HBITMAP &hBitmap,UINT nrLiczby,COLORREF kTla,COLORREF On,COLORREF Off,double skala){
	if(skala<0.5)
		skala=0.5;
	HDC hdc=GetDC(NULL);
	DeleteObject(hBitmap);
	hBitmap=CreateCompatibleBitmap(hdc,(int)(12*skala),(int)(21*skala));
	HDC mem=CreateCompatibleDC(hdc);
	SelectObject(mem,hBitmap);
	fillRectangle(mem,0,0,(int)(12*skala),(int)(21*skala),kTla);
	if(nrLiczby>10)
		nrLiczby=10;
    switch(nrLiczby){
		case 0:
			Poziom(mem,2,0,On,skala);
			Poziom(mem,2,9,Off,skala);
			Poziom(mem,2,18,On,skala);
			Pion(mem,0,2,On,skala);
			Pion(mem,9,2,On,skala);
			Pion(mem,0,11,On,skala);
			Pion(mem,9,11,On,skala);
			break;
		case 1:
			Poziom(mem,2,0,Off,skala);
			Poziom(mem,2,9,Off,skala);
			Poziom(mem,2,18,Off,skala);
			Pion(mem,0,2,Off,skala);
			Pion(mem,9,2,On,skala);
			Pion(mem,0,11,Off,skala);
			Pion(mem,9,11,On,skala);
			break;
		case 2:
			Poziom(mem,2,0,On,skala);
			Poziom(mem,2,9,On,skala);
			Poziom(mem,2,18,On,skala);
			Pion(mem,0,2,Off,skala);
			Pion(mem,9,2,On,skala);
			Pion(mem,0,11,On,skala);
			Pion(mem,9,11,Off,skala);
			break;
		case 3:
			Poziom(mem,2,0,On,skala);
			Poziom(mem,2,9,On,skala);
			Poziom(mem,2,18,On,skala);
			Pion(mem,0,2,Off,skala);
			Pion(mem,9,2,On,skala);
			Pion(mem,0,11,Off,skala);
			Pion(mem,9,11,On,skala);
			break;
		case 4:
			Poziom(mem,2,0,Off,skala);
			Poziom(mem,2,9,On,skala);
			Poziom(mem,2,18,Off,skala);
			Pion(mem,0,2,On,skala);
			Pion(mem,9,2,On,skala);
			Pion(mem,0,11,Off,skala);
			Pion(mem,9,11,On,skala);
			break;
		case 5:
			Poziom(mem,2,0,On,skala);
			Poziom(mem,2,9,On,skala);
			Poziom(mem,2,18,On,skala);
			Pion(mem,0,2,On,skala);
			Pion(mem,9,2,Off,skala);
			Pion(mem,0,11,Off,skala);
			Pion(mem,9,11,On,skala);
			break;
		case 6:
			Poziom(mem,2,0,On,skala);
			Poziom(mem,2,9,On,skala);
			Poziom(mem,2,18,On,skala);
			Pion(mem,0,2,On,skala);
			Pion(mem,9,2,Off,skala);
			Pion(mem,0,11,On,skala);
			Pion(mem,9,11,On,skala);
			break;
		case 7:
			Poziom(mem,2,0,On,skala);
			Poziom(mem,2,9,Off,skala);
			Poziom(mem,2,18,Off,skala);
			Pion(mem,0,2,Off,skala);
			Pion(mem,9,2,On,skala);
			Pion(mem,0,11,Off,skala);
			Pion(mem,9,11,On,skala);
			break;
		case 8:
			Poziom(mem,2,0,On,skala);
			Poziom(mem,2,9,On,skala);
			Poziom(mem,2,18,On,skala);
			Pion(mem,0,2,On,skala);
			Pion(mem,9,2,On,skala);
			Pion(mem,0,11,On,skala);
			Pion(mem,9,11,On,skala);
			break;
		case 9:
			Poziom(mem,2,0,On,skala);
			Poziom(mem,2,9,On,skala);
			Poziom(mem,2,18,On,skala);
			Pion(mem,0,2,On,skala);
			Pion(mem,9,2,On,skala);
			Pion(mem,0,11,Off,skala);
			Pion(mem,9,11,On,skala);
			break;
		case 10:
			Poziom(mem,2,0,Off,skala);
			Poziom(mem,2,9,On,skala);
			Poziom(mem,2,18,Off,skala);
			Pion(mem,0,2,Off,skala);
			Pion(mem,9,2,Off,skala);
			Pion(mem,0,11,Off,skala);
			Pion(mem,9,11,Off,skala);
			break;
	}
	DeleteDC(hdc);
	DeleteDC(mem);    
}

void ObsRys::StretchMaskBmp(HDC hdc,int x,int y,int szer,int wys,HBITMAP Image,HBITMAP Mask){
	COLORREF kTekstu=GetTextColor(hdc);
	SetTextColor(hdc,RGB(0,0,0));
	HDC hdcImage=CreateCompatibleDC(hdc);
	HDC hdcMask=CreateCompatibleDC(hdc);
	SelectObject(hdcImage,Image);
	SelectObject(hdcMask,Mask);
	BITMAP bitmap;
	GetObject(Image,sizeof(bitmap),&bitmap);
	BitBlt(hdcImage,0,0,szer,wys,hdcMask,0,0,SRCAND);
	StretchBlt(hdc,x,y,szer,wys,hdcMask,0,0,bitmap.bmWidth,bitmap.bmHeight,0x220326);
	StretchBlt(hdc,x,y,szer,wys,hdcImage,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCPAINT);
	DeleteDC(hdcImage);
	DeleteDC(hdcMask);
	SetTextColor(hdc,kTekstu);
}

void ObsRys::MaskBmp(HDC hdc,int x,int y,int szer,int wys,HBITMAP Image,HBITMAP Mask){
	COLORREF kTekstu=GetTextColor(hdc);
	SetTextColor(hdc,RGB(0,0,0));
	HDC hdcImage=CreateCompatibleDC(hdc);
	HDC hdcMask=CreateCompatibleDC(hdc);
	SelectObject(hdcImage,Image);
	SelectObject(hdcMask,Mask);
	BitBlt(hdcImage,0,0,szer,wys,hdcMask,0,0,SRCAND);
	BitBlt(hdc,x,y,szer,wys,hdcMask,0,0,0x220326);
	BitBlt(hdc,x,y,szer,wys,hdcImage,0,0,SRCPAINT);
	DeleteDC(hdcImage);
	DeleteDC(hdcMask);
	SetTextColor(hdc,kTekstu);
}

void ObsRys::bitBltBmpOnHdc(HDC &hdc,HBITMAP hBmp,int x,int y,UINT szer,UINT wys){
	HDC mem=CreateCompatibleDC(hdc);
	SelectObject(mem,hBmp);
	BitBlt(hdc,x,y,szer,wys,mem,0,0,SRCCOPY);
	DeleteDC(mem);
}

void ObsRys::bitBltBmpOnHdc(HDC &hdc,HBITMAP hBmp,int x,int y){
	BITMAP bmp;
	GetObject(hBmp,sizeof(bmp),&bmp);
	bitBltBmpOnHdc(hdc,hBmp,x,y,bmp.bmWidth,bmp.bmHeight);
}

void ObsRys::stretchBltBmpOnHdc(HDC &hdc,HBITMAP hBmp,int x,int y,UINT szer,UINT wys){
	BITMAP bmp;
	GetObject(hBmp,sizeof(bmp),&bmp);
	HDC mem=CreateCompatibleDC(hdc);
	SelectObject(mem,hBmp);
	StretchBlt(hdc,0,0,szer,wys,mem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	DeleteDC(mem);
}

HBITMAP ObsRys::CreateMask(HBITMAP hBmp,COLORREF kMaski){
	HBITMAP hMask;
	BITMAP bmp;
	GetObject(hBmp,sizeof(bmp),&bmp);
	HDC hdc=GetDC(NULL);
	hMask=CreateBitmap(bmp.bmWidth,bmp.bmHeight,1,1,NULL);
	HDC hdc2=CreateCompatibleDC(hdc);
	HDC hdc3=CreateCompatibleDC(hdc);
	SelectObject(hdc3,hBmp);
	SelectObject(hdc2,hMask);
	for(int i=0;i<bmp.bmWidth;i++)
		for(int j=0;j<bmp.bmHeight;j++)
			if(GetPixel(hdc3,i,j)==kMaski)
				SetPixel(hdc2,i,j,RGB(0,0,0));
			else
				SetPixel(hdc2,i,j,RGB(255,255,255));
	DeleteDC(hdc2);
	DeleteDC(hdc3);
	DeleteDC(hdc);
	return hMask;
}

void ObsRys::textOut(HDC &hdc,int x,int y,LPSTR tekst,int dlTekstu){
	if(dlTekstu>0)
		TextOut(hdc,x,y,tekst,dlTekstu);
	else
		TextOut(hdc,x,y,tekst,(int)strlen(tekst));
}

void ObsRys::textOut(HDC &hdc,int x,int y,int liczba,int ilCyfr){
	char *p;
	if(ilCyfr<3)
		ilCyfr=3;
	p=new char[ilCyfr+10];
	_gcvt(liczba,ilCyfr,p);
	if(p[strlen(p)-1]=='.')
		p[strlen(p)-1]=NULL;
	textOut(hdc,x,y,p,-1);
	delete[] p;
}

void ObsRys::textOut(HDC &hdc,int x,int y,double liczba,int ilCyfr){
	char *p;
	if(ilCyfr<3)
		ilCyfr=3;
	p=new char[ilCyfr+10];
	_gcvt(liczba,ilCyfr,p);
	if(p[strlen(p)-1]=='.')
		p[strlen(p)-1]=NULL;
	textOut(hdc,x,y,p,-1);
	delete[] p;
}

void ObsRys::textOut(HDC &hdc,int x,int y,LPSTR tekst,int dlTekstu,COLORREF kTekstu,int bkMode){
	SetTextColor(hdc,kTekstu);
	SetBkMode(hdc,bkMode);
	textOut(hdc,x,y,tekst,dlTekstu);
}

void ObsRys::textOut(HDC &hdc,int x,int y,int liczba,int ilCyfr,COLORREF kTekstu,int bkMode){
	SetTextColor(hdc,kTekstu);
	SetBkMode(hdc,bkMode);
	textOut(hdc,x,y,liczba,ilCyfr);
}

void ObsRys::textOut(HDC &hdc,int x,int y,double liczba,int ilCyfr,COLORREF kTekstu,int bkMode){
	SetTextColor(hdc,kTekstu);
	SetBkMode(hdc,bkMode);
	textOut(hdc,x,y,liczba,ilCyfr);
}

void ObsRys::drawText(HDC &hdc,int liczba,int ilCyfr,int x,int y,UINT szer,UINT wys,UINT uFormat){
	char* p;
	if(ilCyfr<3)
		ilCyfr=3;
	p=new char[ilCyfr+10];
	_gcvt(liczba,ilCyfr,p);
	if(p[strlen(p)-1]=='.')
		p[strlen(p)-1]=NULL;
	drawText(hdc,p,-1,x,y,szer,wys,uFormat);
	delete[] p;
}

void ObsRys::drawText(HDC &hdc,double liczba,int ilCyfr,int x,int y,UINT szer,UINT wys,UINT uFormat){
	char* p;
	if(ilCyfr<3)
		ilCyfr=3;
	p=new char[ilCyfr+10];
	_gcvt(liczba,ilCyfr,p);
	if(p[strlen(p)-1]=='.')
		p[strlen(p)-1]=NULL;
	drawText(hdc,p,-1,x,y,szer,wys,uFormat);
	delete[] p;
}

void ObsRys::drawText(HDC &hdc,LPSTR tekst,int dlTekstu,RECT *rect,UINT uFormat){
	DrawText(hdc,tekst,dlTekstu,rect,uFormat);
}

void ObsRys::drawText(HDC &hdc,LPSTR tekst,int dlTekstu,int x,int y,UINT szer,UINT wys,UINT uFormat){
	DrawText(hdc,tekst,dlTekstu,&GetRect(x,y,szer,wys),uFormat);
}

void ObsRys::SetBrush(HDC &hdc,COLORREF kBrush){
	DeleteObject(hBrush);
	hBrush=CreateSolidBrush(kBrush);
	SelectObject(hdc,hBrush);
}

void ObsRys::SetBrush(HDC &hdc,HBRUSH _hBrush){
	DeleteObject(hBrush);
	hBrush=_hBrush;
	SelectObject(hdc,hBrush);
}

void ObsRys::SetPen(HDC &hdc,HPEN _hPen){
	DeleteObject(hPen);
	hPen=_hPen;
	SelectObject(hdc,hPen);
}

void ObsRys::SetPen(HDC &hdc,COLORREF kPen,int Styl, int Gr){
	DeleteObject(hPen);
	hPen=CreatePen(Styl,Gr,kPen);
	SelectObject(hdc,hPen);
}

void ObsRys::rectangle(HDC &hdc,int x,int y,UINT szer,UINT wys){
	Rectangle(hdc,x,y,x+szer,y+wys);
}

void ObsRys::rectangle(HDC &hdc,RECT rect){
	Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
}

void ObsRys::rectangle(HDC &hdc,RECT rect,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	rectangle(hdc,rect);
}

void ObsRys::rectangle(HDC &hdc,int x,int y,UINT szer,UINT wys,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	rectangle(hdc,x,y,szer,wys);
}

void ObsRys::fillRectangle(HDC &hdc,int x,int y,UINT szer,UINT wys,COLORREF kBrush){
	SetBrush(hdc,kBrush);
	RECT rect;
	SetRect(&rect,x,y,x+szer,y+wys);
	FillRect(hdc,&rect,hBrush);
}

void ObsRys::fillRectangle(HDC &hdc,RECT rect,COLORREF kBrush){
	SetBrush(hdc,kBrush);
	FillRect(hdc,&rect,hBrush);
}

void ObsRys::fillRectangle(HDC &hdc,RECT rect){
	FillRect(hdc,&rect,hBrush);
}

void ObsRys::fillRectangle(HDC &hdc,int x,int y,UINT szer,UINT wys){
	RECT rect;
	SetRect(&rect,x,y,x+szer,y+wys);
	FillRect(hdc,&rect,hBrush);
}

void ObsRys::frameRectangle(HDC &hdc,int x,int y,UINT szer,UINT wys){
	HBRUSH hBr=hBrush;
	SetBrush(hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
	Rectangle(hdc,x,y,x+szer,y+wys);
	SetBrush(hdc,hBr);
}

void ObsRys::frameRectangle(HDC &hdc,RECT rect){
	HBRUSH hBr=hBrush;
	SetBrush(hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
	Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
	SetBrush(hdc,hBr);
}

void ObsRys::frameRectangle(HDC &hdc,int x,int y,UINT szer,UINT wys,COLORREF kPen,int Styl,int Gr){
	HBRUSH hBr=hBrush;
	SetBrush(hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
	SetPen(hdc,kPen,Styl,Gr);
	Rectangle(hdc,x,y,x+szer,y+wys);
	SetBrush(hdc,hBr);
}

void ObsRys::frameRectangle(HDC &hdc,RECT rect,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	HBRUSH br=hBrush;
	SetBrush(hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
	Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
	SetBrush(hdc,br);	
}

void ObsRys::circle(HDC &hdc,int xS,int yS,UINT Ray){
	Ellipse(hdc,xS-(int)Ray,yS-(int)Ray,xS+Ray,yS+Ray);
}

void ObsRys::circle(HDC &hdc,int xS,int yS,UINT Ray,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	circle(hdc,xS,yS,Ray);
}

void ObsRys::fillCircle(HDC &hdc,int xS,int yS,UINT Ray){
	HPEN hP=(HPEN)GetStockObject(NULL_PEN);
	SelectObject(hdc,hP);
	Ellipse(hdc,xS-(int)Ray,yS-(int)Ray,xS+Ray,yS+Ray);
	DeleteObject(hP);
	SelectObject(hdc,hPen);
}

void ObsRys::fillCircle(HDC &hdc,int xS,int yS,UINT Ray,COLORREF kBrush){
	SetBrush(hdc,kBrush);
	fillCircle(hdc,xS,yS,Ray);
}

void ObsRys::frameCircle(HDC &hdc,int xS,int yS,UINT Ray){
	HBRUSH hBr=hBrush;
	SetBrush(hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
	Ellipse(hdc,xS-(int)Ray,yS-(int)Ray,xS+Ray,yS+Ray);
	SetBrush(hdc,hBr);
}

void ObsRys::frameCircle(HDC &hdc,int xS,int yS,UINT Ray,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	frameCircle(hdc,xS,yS,Ray);
}

void ObsRys::ellipse(HDC &hdc,int x,int y,UINT xRay,UINT yRay){
	Ellipse(hdc,x-(int)xRay,y-(int)yRay,x+xRay,y+yRay);
}

void ObsRys::ellipse(HDC &hdc,RECT rect){
	Ellipse(hdc,rect.left,rect.top,rect.right,rect.bottom);
}

void ObsRys::ellipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	ellipse(hdc,xS,yS,xRay,yRay);
}

void ObsRys::ellipse(HDC &hdc,RECT rect,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	ellipse(hdc,rect);
}

void ObsRys::fillEllipse(HDC &hdc,RECT rect){
	HPEN hP=(HPEN)GetStockObject(NULL_PEN);
	SelectObject(hdc,hP);
	Ellipse(hdc,rect.left,rect.top,rect.right,rect.bottom);
	DeleteObject(hP);
	SelectObject(hdc,hPen);
}

void ObsRys::fillEllipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay){
	HPEN hP=(HPEN)GetStockObject(NULL_PEN);
	SelectObject(hdc,hP);
	Ellipse(hdc,xS-(int)xRay,yS-(int)yRay,xS+xRay,yS+yRay);
	DeleteObject(hP);
	SelectObject(hdc,hPen);
}

void ObsRys::fillEllipse(HDC &hdc,RECT rect,COLORREF kBrush){
	SetBrush(hdc,kBrush);
	fillEllipse(hdc,rect);
}

void ObsRys::fillEllipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,COLORREF kBrush){
	SetBrush(hdc,kBrush);
	fillEllipse(hdc,xS,yS,xRay,yRay);
}

void ObsRys::frameEllipse(HDC &hdc,RECT rect){
	HBRUSH hBr=(HBRUSH)GetStockObject(NULL_BRUSH);
	SelectObject(hdc,hBr);
	Ellipse(hdc,rect.left,rect.top,rect.right,rect.bottom);
	DeleteObject(hBr);
	SelectObject(hdc,hBrush);
}

void ObsRys::frameEllipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay){
	HBRUSH hBr=(HBRUSH)GetStockObject(NULL_BRUSH);
	SelectObject(hdc,hBr);
	Ellipse(hdc,xS-(int)xRay,yS-(int)yRay,xS+xRay,yS+yRay);
	DeleteObject(hBr);
	SelectObject(hdc,hBrush);
}

void ObsRys::frameEllipse(HDC &hdc,RECT rect,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	frameEllipse(hdc,rect);
}

void ObsRys::frameEllipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	frameEllipse(hdc,xS,yS,xRay,yRay);
}

void ObsRys::roundRect(HDC &hdc,RECT rect,UINT Ray){
	RoundRect(hdc,rect.left,rect.top,rect.right,rect.bottom,Ray,Ray);
}

void ObsRys::roundRect(HDC &hdc,RECT rect,UINT xRay,UINT yRay){
	RoundRect(hdc,rect.left,rect.top,rect.right,rect.bottom,xRay,yRay);
}

void ObsRys::roundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT Ray){
	RoundRect(hdc,x,y,x+szer,y+wys,Ray,Ray);
}

void ObsRys::roundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT xRay,UINT yRay){
	RoundRect(hdc,x,y,x+szer,y+wys,xRay,yRay);
}

void ObsRys::roundRect(HDC &hdc,RECT rect,UINT Ray,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	roundRect(hdc,rect,Ray);
}

void ObsRys::roundRect(HDC &hdc,RECT rect,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	roundRect(hdc,rect,xRay,yRay);
}
void ObsRys::roundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT Ray,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	roundRect(hdc,x,y,szer,wys,Ray);
}
void ObsRys::roundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	roundRect(hdc,x,y,szer,wys,xRay,yRay);
}
void ObsRys::fillRoundRect(HDC &hdc,RECT rect,UINT Ray,COLORREF kBrush){
	SetBrush(hdc,kBrush);
	HPEN hP=hPen;
	SetPen(hdc,(HPEN)GetStockObject(NULL_PEN));
	RoundRect(hdc,rect.left,rect.top,rect.right,rect.bottom,Ray,Ray);
	SetPen(hdc,hPen);
}
void ObsRys::fillRoundRect(HDC &hdc,RECT rect,UINT xRay,UINT yRay,COLORREF kBrush){
	SetBrush(hdc,kBrush);
	HPEN hP=hPen;
	SetPen(hdc,(HPEN)GetStockObject(NULL_PEN));
	RoundRect(hdc,rect.left,rect.top,rect.right,rect.bottom,xRay,yRay);
	SetPen(hdc,hPen);
}
void ObsRys::fillRoundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT Ray,COLORREF kBrush){
	SetBrush(hdc,kBrush);
	HPEN hP=hPen;
	SetPen(hdc,(HPEN)GetStockObject(NULL_PEN));
	RoundRect(hdc,x,y,x+szer,y+wys,Ray,Ray);
	SetPen(hdc,hPen);
}
void ObsRys::fillRoundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT xRay,UINT yRay,COLORREF kBrush){
	SetBrush(hdc,kBrush);
	HPEN hP=hPen;
	SetPen(hdc,(HPEN)GetStockObject(NULL_PEN));
	RoundRect(hdc,x,y,x+szer,y+wys,xRay,yRay);
	SetPen(hdc,hPen);
}

void ObsRys::frameRoundRect(HDC &hdc,RECT rect,UINT Ray,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	HBRUSH hBr=hBrush;
	SetBrush(hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
	RoundRect(hdc,rect.left,rect.top,rect.right,rect.bottom,Ray,Ray);
	SetBrush(hdc,hBr);
}
void ObsRys::frameRoundRect(HDC &hdc,RECT rect,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	HBRUSH hBr=hBrush;
	SetBrush(hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
	RoundRect(hdc,rect.left,rect.top,rect.right,rect.bottom,xRay,yRay);

}
void ObsRys::frameRoundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT Ray,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	HBRUSH hBr=hBrush;
	SetBrush(hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
	RoundRect(hdc,x,y,x+szer,y+wys,Ray,Ray);
	SetBrush(hdc,hBr);
}
void ObsRys::frameRoundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	HBRUSH hBr=hBrush;
	SetBrush(hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
	RoundRect(hdc,x,y,x+szer,y+wys,xRay,yRay);
	SetBrush(hdc,hBr);
}

void ObsRys::line(HDC &hdc,POINT p1,POINT p2){
	line(hdc,p1.x,p1.y,p2.x,p2.y);
}

void ObsRys::line(HDC &hdc,int x1,int y1,int x2,int y2){
	MoveToEx(hdc,x1,y1,NULL);
	LineTo(hdc,x2,y2);
}

void ObsRys::line(HDC &hdc,int X,int Y,int kt,UINT R){
	double _kt=asin(1.)*kt/90;
	line(hdc,X,Y,X+(int)((double)R*cos(_kt)),Y+(int)((double)R*sin(_kt)));
}
void ObsRys::line(HDC &hdc,int X,int Y,double kt, UINT R){
	line(hdc,X,Y,X+(int)((double)R*cos(kt)),Y+(int)((double)R*sin(kt)));
}

void ObsRys::line(HDC &hdc,POINT p1,POINT p2,COLORREF kPen,int Styl,int Gr){
	line(hdc,p1.x,p1.y,p2.x,p2.y,kPen,Styl,Gr);
}

void ObsRys::line(HDC &hdc,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	line(hdc,x1,y1,x2,y2);
}

void ObsRys::line(HDC &hdc,int X,int Y,int kt,UINT R,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	line(hdc,X,Y,kt,R);
}

void ObsRys::line(HDC &hdc,int X,int Y,double kt,UINT R,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	line(hdc,X,Y,kt,R);
}

void ObsRys::circleArc(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2){
	Arc(hdc,xS-(int)Ray,yS-(int)Ray,xS+Ray,yS+Ray,x1,y1,x2,y2);
}
void ObsRys::circleArc(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2){
	double _kt1=asin(1.)*kt1/90;
	double _kt2=asin(1.)*kt2/90;
	Arc(hdc,xS-(int)Ray,yS-(int)Ray,xS+Ray,yS+Ray,xS+(int)((double)(Ray)*cos(_kt1)),yS+(int)((double)(Ray)*sin(_kt1)),xS+(int)((double)(Ray)*cos(_kt2)),yS+(int)((double)(Ray)*sin(_kt2)));
}
void ObsRys::circleArc(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2){
	Arc(hdc,xS-(int)Ray,yS-(int)Ray,xS+Ray,yS+Ray,xS+(int)((double)Ray*cos(kt1)),yS+(int)((double)Ray*sin(kt1)),xS+(int)((double)Ray*cos(kt2)),yS+(int)((double)Ray*sin(kt2)));
}

void ObsRys::circleArc(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	circleArc(hdc,xS,yS,Ray,x1,y1,x2,y2);
}

void ObsRys::circleArc(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	circleArc(hdc,xS,yS,Ray,kt1,kt2);
}
void ObsRys::circleArc(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	circleArc(hdc,xS,yS,Ray,kt1,kt2);
}

void ObsRys::ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2){
	Arc(hdc,xS-(int)xRay,yS-(int)yRay,xS+xRay,yS+yRay,x1,y1,x2,y2);
}

void ObsRys::ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2){
	double _kt1=asin(1.)*(double)kt1/90;
	double _kt2=asin(1.)*(double)kt2/90;
	Arc(hdc,xS-(int)xRay,yS-(int)yRay,xS+xRay,yS+yRay,xS+(int)((double)xRay*cos(_kt1)),yS+(int)((double)xRay*sin(_kt1)),xS+(int)((double)xRay*cos(_kt2)),yS+(int)((double)xRay*sin(_kt2)));
}

void ObsRys::ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2){
	Arc(hdc,xS-(int)xRay,yS-(int)yRay,xS+xRay,yS+yRay,xS+(int)((double)xRay*cos(kt1)),yS+(int)((double)xRay*sin(kt1)),xS+(int)((double)xRay*cos(kt2)),yS+(int)((double)xRay*sin(kt2)));
}

void ObsRys::ellipseArc(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2){
	Arc(hdc,rect.left,rect.top,rect.right,rect.bottom,x1,y1,x2,y2);
}

void ObsRys::ellipseArc(HDC &hdc,RECT rect,int kt1,int kt2){
	int xS=(rect.right+rect.left)/2;
	int yS=(rect.bottom+rect.top)/2;
	ellipseArc(hdc,xS,yS,(rect.right-rect.left)/2,(rect.bottom-rect.top)/2,kt1,kt2);
}

void ObsRys::ellipseArc(HDC &hdc,RECT rect,double kt1,double kt2){
	int xS=(rect.right+rect.left)/2;
	int yS=(rect.bottom+rect.top)/2;
	ellipseArc(hdc,xS,yS,(rect.right-rect.left)/2,(rect.bottom-rect.top)/2,kt1,kt2);
}

void ObsRys::ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipseArc(hdc,xS,yS,xRay,yRay,x1,y1,x2,y2);
}

void ObsRys::ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipseArc(hdc,xS,yS,xRay,yRay,kt1,kt2);
}

void ObsRys::ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipseArc(hdc,xS,yS,xRay,yRay,kt1,kt2);
}

void ObsRys::ellipseArc(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipseArc(hdc,rect,x1,y1,x2,y2);
}

void ObsRys::ellipseArc(HDC &hdc,RECT rect,int kt1,int kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipseArc(hdc,rect,kt1,kt2);
}

void ObsRys::ellipseArc(HDC &hdc,RECT rect,double kt1,double kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipseArc(hdc,rect,kt1,kt2);
}

void ObsRys::circleChord(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2){
	Chord(hdc,xS-(int)Ray,yS-(int)Ray,xS+Ray,yS+Ray,x1,y1,x2,y2);
}
void ObsRys::circleChord(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2){
	double _kt1=asin(1.)*kt1/90;
	double _kt2=asin(1.)*kt2/90;
	Chord(hdc,xS-(int)Ray,yS-(int)Ray,xS+Ray,yS+Ray,xS+(int)((double)(Ray)*cos(_kt1)),yS+(int)((double)(Ray)*sin(_kt1)),xS+(int)((double)(Ray)*cos(_kt2)),yS+(int)((double)(Ray)*sin(_kt2)));
}
void ObsRys::circleChord(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2){
	Chord(hdc,xS-(int)Ray,yS-(int)Ray,xS+Ray,yS+Ray,xS+(int)((double)Ray*cos(kt1)),yS+(int)((double)Ray*sin(kt1)),xS+(int)((double)Ray*cos(kt2)),yS+(int)((double)Ray*sin(kt2)));
}

void ObsRys::circleChord(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	circleChord(hdc,xS,yS,Ray,x1,y1,x2,y2);
}

void ObsRys::circleChord(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	circleChord(hdc,xS,yS,Ray,kt1,kt2);
}
void ObsRys::circleChord(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	circleChord(hdc,xS,yS,Ray,kt1,kt2);
}

void ObsRys::ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2){
	Chord(hdc,xS-(int)xRay,yS-(int)yRay,xS+xRay,yS+yRay,x1,y1,x2,y2);
}

void ObsRys::ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2){
	double _kt1=asin(1.)*(double)kt1/90;
	double _kt2=asin(1.)*(double)kt2/90;
	Chord(hdc,xS-(int)xRay,yS-(int)yRay,xS+xRay,yS+yRay,xS+(int)((double)xRay*cos(_kt1)),yS+(int)((double)xRay*sin(_kt1)),xS+(int)((double)xRay*cos(_kt2)),yS+(int)((double)xRay*sin(_kt2)));
}

void ObsRys::ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2){
	Chord(hdc,xS-(int)xRay,yS-(int)yRay,xS+xRay,yS+yRay,xS+(int)((double)xRay*cos(kt1)),yS+(int)((double)xRay*sin(kt1)),xS+(int)((double)xRay*cos(kt2)),yS+(int)((double)xRay*sin(kt2)));
}

void ObsRys::ellipseChord(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2){
	Chord(hdc,rect.left,rect.top,rect.right,rect.bottom,x1,y1,x2,y2);
}

void ObsRys::ellipseChord(HDC &hdc,RECT rect,int kt1,int kt2){
	int xS=(rect.right+rect.left)/2;
	int yS=(rect.bottom+rect.top)/2;
	ellipseChord(hdc,xS,yS,(rect.right-rect.left)/2,(rect.bottom-rect.top)/2,kt1,kt2);
}

void ObsRys::ellipseChord(HDC &hdc,RECT rect,double kt1,double kt2){
	int xS=(rect.right+rect.left)/2;
	int yS=(rect.bottom+rect.top)/2;
	ellipseChord(hdc,xS,yS,(rect.right-rect.left)/2,(rect.bottom-rect.top)/2,kt1,kt2);
}

void ObsRys::ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipseChord(hdc,xS,yS,xRay,yRay,x1,y1,x2,y2);
}

void ObsRys::ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipseChord(hdc,xS,yS,xRay,yRay,kt1,kt2);
}

void ObsRys::ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipseChord(hdc,xS,yS,xRay,yRay,kt1,kt2);
}

void ObsRys::ellipseChord(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipseChord(hdc,rect,x1,y1,x2,y2);
}

void ObsRys::ellipseChord(HDC &hdc,RECT rect,int kt1,int kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipseChord(hdc,rect,kt1,kt2);
}

void ObsRys::ellipseChord(HDC &hdc,RECT rect,double kt1,double kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipseChord(hdc,rect,kt1,kt2);
}

void ObsRys::circlePie(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2){
	Pie(hdc,xS-(int)Ray,yS-(int)Ray,xS+Ray,yS+Ray,x1,y1,x2,y2);
}
void ObsRys::circlePie(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2){
	double _kt1=asin(1.)*kt1/90;
	double _kt2=asin(1.)*kt2/90;
	Pie(hdc,xS-(int)Ray,yS-(int)Ray,xS+Ray,yS+Ray,xS+(int)((double)(Ray)*cos(_kt1)),yS+(int)((double)(Ray)*sin(_kt1)),xS+(int)((double)(Ray)*cos(_kt2)),yS+(int)((double)(Ray)*sin(_kt2)));
}
void ObsRys::circlePie(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2){
	Pie(hdc,xS-(int)Ray,yS-(int)Ray,xS+Ray,yS+Ray,xS+(int)((double)Ray*cos(kt1)),yS+(int)((double)Ray*sin(kt1)),xS+(int)((double)Ray*cos(kt2)),yS+(int)((double)Ray*sin(kt2)));
}

void ObsRys::circlePie(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	circlePie(hdc,xS,yS,Ray,x1,y1,x2,y2);
}

void ObsRys::circlePie(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	circlePie(hdc,xS,yS,Ray,kt1,kt2);
}
void ObsRys::circlePie(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	circlePie(hdc,xS,yS,Ray,kt1,kt2);
}

void ObsRys::ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2){
	Pie(hdc,xS-(int)xRay,yS-(int)yRay,xS+xRay,yS+yRay,x1,y1,x2,y2);
}

void ObsRys::ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2){
	double _kt1=asin(1.)*(double)kt1/90;
	double _kt2=asin(1.)*(double)kt2/90;
	Pie(hdc,xS-(int)xRay,yS-(int)yRay,xS+xRay,yS+yRay,xS+(int)((double)xRay*cos(_kt1)),yS+(int)((double)xRay*sin(_kt1)),xS+(int)((double)xRay*cos(_kt2)),yS+(int)((double)xRay*sin(_kt2)));
}

void ObsRys::ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2){
	Pie(hdc,xS-(int)xRay,yS-(int)yRay,xS+xRay,yS+yRay,xS+(int)((double)xRay*cos(kt1)),yS+(int)((double)xRay*sin(kt1)),xS+(int)((double)xRay*cos(kt2)),yS+(int)((double)xRay*sin(kt2)));
}

void ObsRys::ellipsePie(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2){
	Pie(hdc,rect.left,rect.top,rect.right,rect.bottom,x1,y1,x2,y2);
}

void ObsRys::ellipsePie(HDC &hdc,RECT rect,int kt1,int kt2){
	int xS=(rect.right+rect.left)/2;
	int yS=(rect.bottom+rect.top)/2;
	ellipsePie(hdc,xS,yS,(rect.right-rect.left)/2,(rect.bottom-rect.top)/2,kt1,kt2);
}

void ObsRys::ellipsePie(HDC &hdc,RECT rect,double kt1,double kt2){
	int xS=(rect.right+rect.left)/2;
	int yS=(rect.bottom+rect.top)/2;
	ellipsePie(hdc,xS,yS,(rect.right-rect.left)/2,(rect.bottom-rect.top)/2,kt1,kt2);
}

void ObsRys::ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipsePie(hdc,xS,yS,xRay,yRay,x1,y1,x2,y2);
}

void ObsRys::ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipsePie(hdc,xS,yS,xRay,yRay,kt1,kt2);
}

void ObsRys::ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipsePie(hdc,xS,yS,xRay,yRay,kt1,kt2);
}

void ObsRys::ellipsePie(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipsePie(hdc,rect,x1,y1,x2,y2);
}

void ObsRys::ellipsePie(HDC &hdc,RECT rect,int kt1,int kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipsePie(hdc,rect,kt1,kt2);
}

void ObsRys::ellipsePie(HDC &hdc,RECT rect,double kt1,double kt2,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	ellipsePie(hdc,rect,kt1,kt2);
}

void ObsRys::polyline(HDC &hdc,CONST POINT* Punkty,int ilP){
	Polyline(hdc,Punkty,ilP);
}

void ObsRys::polyline(HDC &hdc,CONST POINT* Punkty,int ilP,COLORREF kPen,int Styl,int Gr){
	SetPen(hdc,kPen,Styl,Gr);
	Polyline(hdc,Punkty,ilP);
}

void ObsRys::polygon(HDC &hdc,CONST POINT* Punkty,int ilP){
	Polygon(hdc,Punkty,ilP);
}

void ObsRys::polygon(HDC &hdc,CONST POINT* Punkty,int ilP,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	Polygon(hdc,Punkty,ilP);
}

void ObsRys::framePolygon(HDC &hdc,CONST POINT* Punkty,int ilP,COLORREF kPen,int Styl,int Gr){
	HBRUSH hBr=hBrush;
	SetBrush(hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
	SetPen(hdc,kPen,Styl,Gr);
	Polygon(hdc,Punkty,ilP);
	SetBrush(hdc,hBr);
}

void ObsRys::framePolygon(HDC &hdc,CONST POINT* Punkty,int ilP){
	HBRUSH hBr=hBrush;
	SetBrush(hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
	Polygon(hdc,Punkty,ilP);
	SetBrush(hdc,hBr);
}

void ObsRys::fillPolygon(HDC &hdc,CONST POINT* Punkty,int ilP,COLORREF kBrush){
	HPEN hP=hPen;
	SetPen(hdc,(HPEN)GetStockObject(NULL_PEN));
	SetBrush(hdc,kBrush);
	Polygon(hdc,Punkty,ilP);
	SetPen(hdc,hPen);
}

void ObsRys::fillPolygon(HDC &hdc,CONST POINT* Punkty,int ilP){
	HPEN hP=hPen;
	SetPen(hdc,(HPEN)GetStockObject(NULL_PEN));
	Polygon(hdc,Punkty,ilP);
	SetPen(hdc,hP);
}

void ObsRys::polyPolygon(HDC &hdc,CONST POINT* Punkty,CONST INT *ilP,int ilW){
	PolyPolygon(hdc,Punkty,ilP,ilW);
}

void ObsRys::polyPolygon(HDC &hdc,CONST POINT* Punkty,CONST INT *ilP,int ilW,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	PolyPolygon(hdc,Punkty,ilP,ilW);
}

void ObsRys::wielForemny(HDC &hdc,int xS,int yS,UINT ilBokow,UINT R,int ktPrzes){
	POINT *p;
	p=new POINT[ilBokow];
	double dk=asin(1.)*4/(double)ilBokow;
	double _ktPrzes=asin(1.)*ktPrzes/90;
	for(POINT* pn=p;pn<p+ilBokow;pn++){
		pn->x=xS+(int)((double)R*cos(dk*(pn-p)+_ktPrzes));
		pn->y=yS+(int)((double)R*sin(dk*(pn-p)+_ktPrzes));
	}
	Polygon(hdc,p,ilBokow);
	delete[] p;
}

void ObsRys::wielForemny(HDC &hdc,int xS,int yS,UINT ilBokow,UINT R,double ktPrzes){
	POINT *p;
	p=new POINT[ilBokow];
	double dk=asin(1.)*4/(double)ilBokow;
	for(POINT* pn=p;pn<p+ilBokow;pn++){
		pn->x=xS+(int)((double)R*cos(dk*(pn-p)+ktPrzes));
		pn->y=yS+(int)((double)R*sin(dk*(pn-p)+ktPrzes));
	}
	Polygon(hdc,p,ilBokow);
	delete[] p;
}

void ObsRys::wielForemny(HDC &hdc,int xS,int yS,UINT ilBokow,UINT R,int ktPrzes,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	wielForemny(hdc,xS,yS,ilBokow,R,ktPrzes);
}

void ObsRys::wielForemny(HDC &hdc,int xS,int yS,UINT ilBokow,UINT R,double ktPrzes,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	wielForemny(hdc,xS,yS,ilBokow,R,ktPrzes);
}

void ObsRys::Wektor(HDC &hdc,int X,int Y,double Kat,int dlWektora,bool Arrow){
	POINT punkty[3];
	if(Arrow){
		punkty[0].x=X+(int)(dlWektora*cos(Kat));
		punkty[0].y=Y+(int)(dlWektora*sin(Kat));
		punkty[1].x=punkty[0].x-(int)(10*cos(Kat)+3*sin(Kat));
		punkty[1].y=punkty[0].y-(int)(10*sin(Kat)-3*cos(Kat));
		punkty[2].x=punkty[0].x-(int)(10*cos(Kat)-3*sin(Kat));
		punkty[2].y=punkty[0].y-(int)(10*sin(Kat)+3*cos(Kat));
	}else{
		punkty[0].x=X;
		punkty[0].y=Y;
		punkty[1].x=punkty[0].x+(int)(10*cos(Kat)+3*sin(Kat));
		punkty[1].y=punkty[0].y+(int)(10*sin(Kat)-3*cos(Kat));
		punkty[2].x=punkty[0].x+(int)(10*cos(Kat)-3*sin(Kat));
		punkty[2].y=punkty[0].y+(int)(10*sin(Kat)+3*cos(Kat));
	}
	line(hdc,X,Y,Kat,(UINT)abs(dlWektora-10));
	Polygon(hdc,punkty,3);
}

void ObsRys::Wektor(HDC &hdc,int x1,int y1,int x2,int y2,bool Arrow){
	double odl=pow((double)(pow((double)(x1-x2),2.0)+pow((double)(y1-y2),2.0)),0.5);
	double kat=asin((double)(y2-y1)/odl);
	if(x1>x2)
		kat=asin(1.)*2-kat;
	Wektor(hdc,x1,y1,kat,(int)odl,Arrow);
}

void ObsRys::Wektor(HDC &hdc,int X,int Y,double Kat,int dlWektora,bool Arrow,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	Wektor(hdc,X,Y,Kat,dlWektora,Arrow);
}

void ObsRys::Wektor(HDC &hdc,int x1,int y1,int x2,int y2,bool Arrow,COLORREF kPen,int Styl,int Gr,COLORREF kBrush){
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,kBrush);
	Wektor(hdc,x1,y1,x2,y2,Arrow);
}

void ObsRys::frameWektor(HDC &hdc,int x1,int y1,int x2,int y2,bool Arrow){
	HBRUSH hBr=hBrush;
	SetBrush(hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
	Wektor(hdc,x1,y1,x2,y2,Arrow);
	SetBrush(hdc,hBr);
}

void ObsRys::frameWektor(HDC &hdc,int x1,int y1,int x2,int y2,bool Arrow,COLORREF kPen,int Styl,int Gr){
	HBRUSH hBr=hBrush;
	SetPen(hdc,kPen,Styl,Gr);
	SetBrush(hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
	Wektor(hdc,x1,y1,x2,y2,Arrow);
	SetBrush(hdc,hBr);
}

ObsRys::ObsRys(void){
	hPen=CreatePen(PS_SOLID,1,RGB(0,0,0));
	hBrush=CreateSolidBrush(RGB(255,255,255));
}