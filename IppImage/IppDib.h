#pragma once

#include <Windows.h>

class IppDib
{
public:
	// 생성자와 소멸자
	IppDib();
	IppDib(const IppDib& dib);
	~IppDib();

	// 비트맵 생성과 소멸
	BOOL CreateGrayBitmap(LONG nWidth, LONG nHeight);
	BOOL CreateRgbBitmap(LONG nWidth, LONG nHeight);
	void DestroyBitmap();

	// 파일 입출력
	BOOL Load(const char* filename);
	BOOL Save(const char* filename);

	// 비트맵 화면 출력
	void Draw(HDC hdc, int dx = 0, int dy = 0);
	void Draw(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop = SRCCOPY);
	void Draw(HDC hdc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, DWORD dwRop = SRCCOPY);

	// Windows 환경에서 클립보드로 복사하기 및 붙여넣기
	BOOL CopyToClipboard();
	BOOL PasteFromClipboard();

	// 대입 연산자 재정의와 비트맵 복사
	IppDib& operator=(const IppDib& dib);
	void Copy(IppDib* pImage);

	// 비트맵 정보 반환 함수
	LONG GetHeight() const { return m_nHeight; }
	LONG GetWidth() const { return m_nWidth; }
	WORD GetBitCount() const { return m_nBitCount; }
	DWORD GetDibSize() const { return m_nDibSize; }
	LPBITMAPINFO GetBitmapInfoAddr() const { return (LPBITMAPINFO)m_pDib; }
	BYTE* GetDIBitsAddr() const;
	int GetPaletteNums() const;
	BOOL IsValid() const { return (m_pDib != NULL); }

private:
	BOOL LoadBMP(const char* filename);
	BOOL SaveBMP(const char* filename);


private:
	LONG m_nWidth;
	LONG m_nHeight;
	WORD m_nBitCount;
	DWORD m_nDibSize;
	BYTE* m_pDib;
};

