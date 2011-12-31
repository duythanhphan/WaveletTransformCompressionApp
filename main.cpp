/*
 * main.cpp
 *
 *  Created on: 31-12-2011
 *      Author: Michal
 */

#include <cstdio>

#include "WaveletCompressor.h"
#include "WaveletDecompressor.h"

int main(int argc, char* argv[]) {

	WaveletCompressor compressor;
	compressor.init("input.png", "output");
	compressor.compress(WaveletCompressor::Haar);

	WaveletDecompressor decompressor;
	decompressor.init("output", "decompressed.bmp");
	decompressor.decompress();

	/*
	Image image;
	image.load("input.png");

	unsigned int width = FreeImage_GetWidth(image.getDib());
	unsigned int height = FreeImage_GetHeight(image.getDib());
	BYTE* bits;
	BYTE test;
	unsigned int BytesPerPixel = FreeImage_GetBPP(image.getDib()) / 8;

	for(unsigned int y = 0; y < height; ++y) {
		bits = FreeImage_GetScanLine(image.getDib(), y);
		for(unsigned int x = 0; x < width; ++x) {
			test = bits[FI_RGBA_RED];
			bits += BytesPerPixel;
		}
	}

	printf("test: %d\n", test);
	*/

	return 0;
}


