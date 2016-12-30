#include "stdafx.h"
#include <math.h>
#include "IppEnhance.h"

void IppInverse(IppByteImage& img) {
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p[i] = 255 - p[i];
	}
}


void IppBrightness(IppByteImage& img, int n) {
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p[i] = limit(p[i] + n);
	}
}


void IppContrast(IppByteImage& img, int n) {
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++) {
		p[i] = static_cast<BYTE>(limit(p[i] + (p[i] - 128) * n / 100));
	}
}


void IppGammaCorrection(IppByteImage& img, float gamma) {
	int invGamma = 1.f / gamma;

	float gamma_table[256];
	for (int i = 0; i < 256; i++)
	{
		gamma_table[i] = pow((i / 255.f), invGamma);
	}

	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p[i] = static_cast<BYTE>(limit(gamma_table[p[i]] * 255 + 0.5f));
	}
}


void IppHistogram(IppByteImage& img, float histo[256]) {
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	// ������׷� ���
	int cnt[256];
	memset(cnt, 0, sizeof(int) * 256);
	for (int i = 0; i < size; i++)
	{
		cnt[p[i]]++;
	}

	// ������׷� ����ȭ
	for (int i = 0; i < 256; i++)
	{
		histo[i] = static_cast<float>(cnt[i]) / size;
	}
}


void IppHistogramStretching(IppByteImage& img) {
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	// �ִ�, �ּ� �׷��̽����� �� ���
	BYTE grayMax, grayMin;
	grayMax = grayMin = p[0];
	for (int i = 1; i < size; i++)
	{
		if (grayMax < p[i]) grayMax = p[i];
		if (grayMin > p[i]) grayMin = p[i];
	}

	if (grayMax == grayMin)
		return;

	// ������׷� ��Ʈ��Ī
	for (int i = 0; i < size; i++)
	{
		p[i] = (p[i] - grayMin) * 255 / (grayMax - grayMin);
	}
}


void IppHistogramEqualization(IppByteImage& img) {
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	// ������׷� ���
	float histo[256];
	IppHistogram(img, histo);

	float cdf[256] = { 0.0, };
	cdf[0] = histo[0];
	for (int i = 1; i < 256; i++)
		cdf[i] = cdf[i - 1] + histo[i];

	// ������׷� �յ�ȭ
	for (int i = 0; i < size; i++)
		p[i] = static_cast<BYTE>(limit(cdf[p[i]] * 255));
}


bool IppAdd(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3) {
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i] = limit(p1[i] + p2[i]);
	}

	return true;
}


bool IppSub(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3) {
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i] = limit(p1[i] - p2[i]);
	}
	return true;
}


bool IppAve(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3) {
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i] = (p1[i] + p2[i]) / 2;
	}
	return true;
}


bool IppDiff(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3) {
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i] = (p1[i] > p2[i] ? p1[i] - p2[i] : p2[i] - p1[i]);
	}
	return true;
}


bool IppAND(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3) {
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i] = p1[i] & p2[i];
	}
	return true;
}


bool IppOR(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3) {
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i] = p1[i] | p2[i];
	}
	return true;
}


void IppBitPlane(IppByteImage& img1, IppByteImage& img2, int bit) {
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	img2.CreateImage(w, h);

	int size = img1.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p2[i] = (p1[i] & (1 << bit))?255:0;
	}
}