// by zeq52giw (illya334) 04.08.2024

// Ціль: Відкрити файл, прочитати по байтово та зактири файл

#ifndef _FILER_
#define _FILER_

	#include <stdio.h>
	#include <stdlib.h>

	#define filer FILE*

	filer fileOpen(char *path){
		filer file = (filer)fopen(path, "r+");

		if(file == 0){
            printf("ERROR filer: cant open this file \'%s\'\n", path);
            exit(1);
		}

		return file;
	}

	char fileGetByte(filer file){
		int c;
		if( (c = fgetc(file) ) != EOF) return (char)c;
		else return 0;
	}

#endif
