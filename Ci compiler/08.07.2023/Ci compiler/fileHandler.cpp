// by Parfonov Illia
// З цим постійні проблеми, тому я виділив його в окремий файл.

#include "includer.h"

FILE *inFile = 0;
FILE *outFile = 0;

fpos_t fileInPos;

void openFileIn( char *path ){
	if(inFile != 0) fclose(inFile);
	inFile = fopen((const char*)path, "rt");
	if(inFile == null) errorFatal("can`t load the input file");
}

void openFileOut( char *path ){
	if(outFile != 0) fclose(outFile);
	outFile = fopen((const char*)path, "wt");
	if(inFile == null) errorFatal("can`t load the output file");
}

char getCharIn(){ // null - end file
	reg char c = fgetc(inFile); // В кінці файла чомусь з'являється -1
	if(feof(inFile)) return null;
	return c;
}

void backCharIn(){
	fgetpos(inFile, &fileInPos);
	fileInPos--;
	fsetpos(inFile, &fileInPos);
}

void putCharFileOut( char ch ){
	fputc(ch, outFile);
}

void closeFiles(){
	fclose(inFile);
	fclose(outFile);
}