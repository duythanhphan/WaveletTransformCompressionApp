/*
 * main.cpp
 *
 *  Created on: 31-12-2011
 *      Author: Michal
 */

#include <cstdio>

#include "WaveletCompressor.h"

int main(int argc, char* argv[]) {

	WaveletCompressor compressor;
	compressor.init("input.png", "output");
	compressor.compress(WaveletCompressor::Haar);

	return 0;
}


