#include "stdafx.h"
#include "IppDib.h"

#define BITMAP_MARKER ((WORD) ('M' << 8) | 'B')


IppDib::IppDib()
	: m_nWidth(0), m_nHeight(0), m_nBitCount(0), m_nDibSize(0), m_pDib(NULL)
{
}


IppDib::IppDib(const IppDib& dib) 
	: m_nWidth(dib.m_nWidth), m_nHeight(dib.m_nHeight), m_nBitCount(dib.m_nBitCount), m_nDibSize(dib.m_nDibSize), m_pDib(NULL)
{
	if (dib.m_pDib != NULL) {
		m_pDib = new BYTE[m_nBitCount];
		memcpy(m_pDib, dib.m_pDib, m_nDibSize);
	}
}


IppDib::~IppDib() {
	if (m_pDib)
		delete[] m_pDib;
}



BOOL IppDib::CreateGrayBitmap(LONG nWidth, LONG nHeight) {
	if (m_pDib)
		DestroyBitmap();

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nBitCount = 8;

	DWORD dwWidthStep = (m_nWidth * m_nBitCount / 8 + 3) & ~3;
	DWORD dwSizeImage = dwWidthStep * m_nHeight;

	m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * GetPaletteNums() + dwSizeImage;

	m_pDib = new BYTE[m_nDibSize];
	LPBITMAPINFOHEADER lpBitmapInfoHeader = (LPBITMAPINFOHEADER)m_pDib;
	lpBitmapInfoHeader->biWidth = m_nWidth;
	lpBitmapInfoHeader->biHeight = m_nHeight;
	lpBitmapInfoHeader->biBitCount = m_nBitCount;
	lpBitmapInfoHeader->biPlanes = 1;
	lpBitmapInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	lpBitmapInfoHeader->biSizeImage = dwSizeImage;
	lpBitmapInfoHeader->biXPelsPerMeter = 0;
	lpBitmapInfoHeader->biYPelsPerMeter = 0;
	lpBitmapInfoHeader->biClrUsed = 0;
	lpBitmapInfoHeader->biClrImportant = 0;
	lpBitmapInfoHeader->biCompression = BI_RGB;

	RGBQUAD* pPal = (RGBQUAD*) ((BYTE*) m_pDib + sizeof(BITMAPINFOHEADER));

	for (int i = 0; i < 256; i++) {
		pPal->rgbBlue = i;
		pPal->rgbGreen = i;
		pPal->rgbRed = i;
		pPal->rgbReserved = 0;
		pPal++;
	}

	BYTE* pData = (BYTE*)GetDIBitsAddr();
	memset(pData, 0, dwSizeImage);

	return TRUE;
}



BOOL IppDib::CreateRgbBitmap(LONG nWidth, LONG nHeight) {
	if (m_pDib)
		DestroyBitmap();

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nBitCount = 24;

	DWORD dwWidthStep = (m_nWidth * m_nBitCount / 8 + 3) & ~3;
	DWORD dwSizeImage = dwWidthStep * m_nHeight;

	m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
	m_pDib = new BYTE[m_nDibSize];

	LPBITMAPINFOHEADER infoHeader = (LPBITMAPINFOHEADER)m_pDib;
	infoHeader->biSize = sizeof(BITMAPINFOHEADER);
	infoHeader->biWidth = m_nWidth;
	infoHeader->biHeight = m_nHeight;
	infoHeader->biPlanes = 1;
	infoHeader->biSizeImage = dwSizeImage;
	infoHeader->biCompression = BI_RGB;
	infoHeader->biBitCount = m_nBitCount;
	infoHeader->biClrUsed = 0;
	infoHeader->biClrImportant = 0;
	infoHeader->biXPelsPerMeter = 0;
	infoHeader->biYPelsPerMeter = 0;

	BYTE* pData = m_pDib + sizeof(BITMAPINFOHEADER);
	memset(pData, 0, dwSizeImage);

	return TRUE;
}


void IppDib::DestroyBitmap() {
	if (m_pDib) {
		delete[] m_pDib;
		m_pDib = NULL;
	}
	
	m_nBitCount = 0;
	m_nDibSize = 0;
	m_nHeight = 0;
	m_nWidth = 0;
}


BOOL IppDib::Load(const char* filename) {
	const char* ext = strrchr(filename, '.');
	if (!_strcmpi(ext, ".bmp"))
		return LoadBMP(filename);
	return FALSE;
}


BOOL IppDib::Save(const char* filename) {
	const char* ext = strrchr(filename, '.');
	if (!_strcmpi(ext, ".bmp"))
		return SaveBMP(filename);
	return FALSE;
}


BOOL IppDib::LoadBMP(const char* filename) {
	FILE* fp = NULL;
	fopen_s(&fp, filename, "rb");
	
	if (!fp)
		return FALSE;

	if (m_pDib)
		DestroyBitmap();

	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	if (fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp) == -1) {
		fclose(fp);
		return FALSE;
	}

	if (fileHeader.bfType != BITMAP_MARKER) {
		fclose(fp);
		return FALSE;
	}

	if (fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp) == -1) {
		fclose(fp);
		return FALSE;
	}

	m_nBitCount = infoHeader.biBitCount;
	m_nWidth = infoHeader.biWidth;
	m_nHeight = infoHeader.biHeight;
	
	DWORD dwWidthStep = (m_nWidth * m_nBitCount / 8 + 3) & ~3;
	DWORD dwSizeImage = dwWidthStep * m_nHeight;

	if (m_nBitCount == 24)
		m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
	else
		m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << m_nBitCount) + dwSizeImage;

	fseek(fp, sizeof(BITMAPFILEHEADER), SEEK_SET);

	m_pDib = new BYTE[m_nDibSize];
	if (fread(m_pDib, sizeof(BYTE), m_nDibSize, fp) == -1) {
		fclose(fp);
		return TRUE;
	}

	fclose(fp);

	return TRUE;
}


BOOL IppDib::SaveBMP(const char* filename) {
	FILE* fp;
	fopen_s(&fp, filename, "wb");
	
	if (!fp)
		return FALSE;

	BITMAPFILEHEADER fileHeader;
	
	fileHeader.bfType = BITMAP_MARKER;
	fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + m_nDibSize;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << GetPaletteNums());
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;

	fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(GetDIBitsAddr(), m_nDibSize, 1, fp);

	fclose(fp);

	return TRUE;
}


void IppDib::Draw(HDC hdc, int dx, int dy) {
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpInfo = (LPBITMAPINFO)m_pDib;
	LPVOID lpBits = (LPVOID)GetDIBitsAddr();

	::SetDIBitsToDevice(hdc, dx, dy, m_nWidth, m_nHeight, 0, 0, 0, m_nHeight, lpBits, lpInfo, DIB_RGB_COLORS);
}


void IppDib::Draw(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop) {
	Draw(hdc, dx, dy, dw, dh, 0, 0, m_nWidth, m_nHeight, dwRop);
}


void IppDib::Draw(HDC hdc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, DWORD dwRop) {
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpInfo = (LPBITMAPINFO)m_pDib;
	LPSTR lpBits = (LPSTR)GetDIBitsAddr();

	SetStretchBltMode(hdc, COLORONCOLOR);
	::StretchDIBits(hdc, dx, dy, dw, dh, sx, sy, sw, sh, lpBits, lpInfo, DIB_RGB_COLORS, dwRop);
}


BOOL IppDib::CopyToClipboard() {
	if (!::OpenClipboard(NULL))
		return FALSE;

	// DIB 전체를 전역 메모리 블럭에 복사
	DWORD dwDibSize = GetDibSize();
	HANDLE hDib = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, dwDibSize);
	if (hDib == NULL) {
		::CloseClipboard();
		return FALSE;
	}

	LPVOID lpDib = ::GlobalLock((HGLOBAL)hDib);
	memcpy(lpDib, GetBitmapInfoAddr(), dwDibSize);
	::GlobalUnlock(hDib);

	// 클립보드에 데이터 입력
	::EmptyClipboard();
	::SetClipboardData(CF_DIB, hDib);
	::CloseClipboard();

	return TRUE;
}


BOOL IppDib::PasteFromClipboard() {
	// CF_DIB 타입이 아니면 종료한다.
	if (!::IsClipboardFormatAvailable(CF_DIB))
		return FALSE;

	// 클립보드를 연다.
	if (!::OpenClipboard(NULL))
		return FALSE;

	// 클립보드 내용을 받아 온다.
	HANDLE hDib = ::GetClipboardData(CF_DIB);
	if (hDib == NULL) {
		::CloseClipboard();
		return FALSE;
	}

	// 메모리 블럭의 크기는 DIB 전체 크기와 동일
	DWORD dwSize = (DWORD)::GlobalSize((HGLOBAL)hDib);
	LPVOID lpDib = ::GlobalLock((HGLOBAL)hDib);

	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)lpDib;
	m_nWidth = lpbi->biWidth;
	m_nHeight = lpbi->biHeight;
	m_nBitCount = lpbi->biBitCount;

	DWORD dwWidthStep = (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3);
	DWORD dwSizeImage = m_nHeight * dwWidthStep;

	if (m_nBitCount == 24)
		m_nDibSize = sizeof(BITMAPINFOHEADER) * dwSizeImage;
	else
		m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << m_nBitCount) + dwSizeImage;

	// 현재 설정된 lppDib 객체가 있다면 삭제D
	if (m_pDib)
		DestroyBitmap();

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL) {
		::GlobalUnlock(hDib);
		::CloseClipboard();
		return FALSE;
	}
	memcpy(m_pDib, lpDib, m_nDibSize);

	::GlobalUnlock(hDib);
	::CloseClipboard();

	return TRUE;
}


IppDib& IppDib::operator=(const IppDib& dib) {
	if (this == &dib)
		return *this;

	if (m_pDib)
		delete[] m_pDib;

	m_nWidth = dib.m_nWidth;
	m_nHeight = dib.m_nHeight;
	m_nBitCount = dib.m_nBitCount;
	m_nDibSize = dib.m_nDibSize;
	m_pDib = NULL;

	if (dib.m_pDib != NULL) {
		m_pDib = new BYTE[m_nDibSize];
		memcpy(m_pDib, dib.m_pDib, m_nDibSize);
	}
	return *this;
}


BYTE* IppDib::GetDIBitsAddr() const {
	if (m_pDib == NULL)
		return NULL;

	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	return ((BYTE*)m_pDib + lpbmi->biSize + sizeof(RGBQUAD) * GetPaletteNums());
}


int IppDib::GetPaletteNums() const {
	switch (m_nBitCount) {
	case 1: return 2;
	case 4: return 16;
	case 8: return 256;
	default: return 0;
	}
}