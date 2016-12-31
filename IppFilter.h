#pragma once

#include "IppImage\IppImage.h"

void IppFilterMean(IppByteImage& imgSrc, IppByteImage& imgDst);
void IppFilterWeightedMean(IppByteImage& imgSrc, IppByteImage& imgDst);
void IppFilterGaussian(IppByteImage& imgSrc, IppFloatImage& imgDst, float sigma);
void IppFilterLaplacian(IppByteImage& imgSrc, IppByteImage& imgDst);
void IppUnSharpMask(IppByteImage& imgSrc, IppByteImage& imgDst);
void IppFilterHighboost(IppByteImage& imgSrc, IppByteImage& imgDst, float alpha);
void IppNoiseGaussian(IppByteImage& imgSrc, IppByteImage& imgDst, int amount);
void IppNoiseSaltNPepper(IppByteImage& imgSrc, IppByteImage& imgDst, int amount);