/*
 * main.cpp
 *
 *  Created on: 31-12-2011
 *      Author: Michal
 */

#include <cstdio>
#include <cstring>
#include <fstream>

#include "WaveletCompressor.h"
#include "WaveletDecompressor.h"

using namespace std;

enum Mode {
	NotSet,
	Compress,
	Decompress
};

void printHelp() {
	printf("-c Compress\n");
	printf("-d Decompress\n");
	printf("-Wavelet (wavelet_type)\n");
	printf("\t Haar\n");
	printf("\t D4\n");
	printf("-Input inputFile - for compression or decompression\n");
	printf("-Output outputFile - for compression or decompression\n");
}

int getArgPosition(const char* key, int argc, char* argv[]) {
	for(int i = 0; i < argc; ++i) {
		if(strcmp(key, argv[i]) == 0) {
			return i;
		}
	}
	return -1;
}

char* getNextArg(int pos, int argc, char* argv[]) {
	if( (pos + 1) > argc) {
		fprintf(stderr, "Wrong number of program arguments.\n");
		printHelp();
		exit(1);
	}
	return argv[pos + 1];
}

void loadCompressorSettings(WaveletCompressor* pCompressor) {
	ifstream settings("../settings.ini");
	if(!settings.good()) {
		return;
	}

	unsigned int quantizerIntervals[3] = {8192, 8192, 8192};
	const char* quantizerOptionNames[] = {"QuantizerIntervalsY:", "QuantizerIntervalsU:", "QuantizerIntervalsV:"};
	char setting[256];
	char* optionName = 0;
	char* optionValue = 0;

	for(int i = 0; i < 3; ++i) {
		settings.getline(setting, 256);
		optionName = strtok(setting, " ");
		optionValue = strtok(NULL, " ");
		if(strcmp(optionName, quantizerOptionNames[i]) == 0) {
			quantizerIntervals[i] = atoi(optionValue);
		}
	}
	pCompressor->setQuantizationIntervalsY(quantizerIntervals[0], quantizerIntervals[1], quantizerIntervals[2]);
}

bool compress(const char* inputFilename, const char* outputFilename, WaveletCompressor::WaveletType waveletType) {
	printf("compressing...\n");

	WaveletCompressor compressor;
	loadCompressorSettings(&compressor);
	if(!compressor.init(inputFilename, outputFilename)) {
		return false;
	}
	if(!compressor.compress(waveletType)) {
		return false;
	}

	printf("done\n");

	return true;
}

bool decompress(const char* inputFilename, const char* outputFilename) {
	printf("decompressing...\n");

	WaveletDecompressor decompressor;
	if(!decompressor.init(inputFilename, outputFilename)) {
		return false;
	}
	if(!decompressor.decompress()) {
		return false;
	}

	printf("done\n");

	return true;
}

int main(int argc, char* argv[]) {
	const char* inputFilename = "";
	const char* outputFilename = "";
	WaveletCompressor::WaveletType waveletType = WaveletCompressor::Haar;
	Mode mode = NotSet;

	//Set Mode
	if(getArgPosition("-c", argc, argv) != -1) {
		mode = Compress;
	} else if(getArgPosition("-d", argc, argv) != -1) {
		mode = Decompress;
	} else {
		printf("Mode not set, set option -c for compression or -d for decompression\n");
		printHelp();
		exit(1);
	}

	//Input Filename
	int argPos = -1;
	argPos = getArgPosition("-Input", argc, argv);
	if(argPos == -1) {
		printf("Set input file (-InputFile filename)\n");
		printHelp();
		exit(1);
	}
	inputFilename = getNextArg(argPos, argc, argv);

	//Output Filename
	argPos = getArgPosition("-Output", argc, argv);
	if(argPos == -1) {
		printf("Set output file (-OutputFile filename)\n");
		printHelp();
		exit(1);
	}
	outputFilename = getNextArg(argPos, argc, argv);

	//Set Wavelet type
	argPos = getArgPosition("-Wavelet", argc, argv);
	if(argPos != -1) {
		char* waveletStr = getNextArg(argPos, argc, argv);
		if(strcmp("Haar", waveletStr) == 0) {
			waveletType = WaveletCompressor::Haar;
		} else if(strcmp("D4", waveletStr) == 0) {
			waveletType = WaveletCompressor::D4;
		}
	}

	//Do the job
	if(mode == Compress) {
		if(!compress(inputFilename, outputFilename, waveletType)) {
			fprintf(stderr, "Compression Failed\n");
		}
	} else if(mode == Decompress) {
		if(!decompress(inputFilename, outputFilename)) {
			fprintf(stderr, "Decompression Failed\n");
		}
	}

	return 0;
}


