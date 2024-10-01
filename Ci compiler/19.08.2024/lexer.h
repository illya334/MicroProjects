// by zeq52giw 19.08.2024

#ifndef _LEXER
#define _LEXER
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include "defines.h"
	#include "filer.h"

	struct stLexerTocken{
		byte id; // 0 - end; 1 - id/str; 2 - str; 3 - number str;
		uint nt; // nameTable
		uint cols, rows; // pos
	};

	char *tmpMem = 0;
	uint tmpMemSize = 0;

	struct stLexerTocken *lexerTocken = 0;
	uint lexerTockenIndex = 0;

	char **nameTable = 0; // nt
	uint nameTableSize = 0;

	void lexerHandleTmpMem(uint i);
	bool lexerTestSimbol(char c);
	char lexerGetWord(char c, filer file, uint *cols);
	void lexerAddTocken(byte id, char *str, uint cols, uint rows);

	struct stLexerTocken *lexer(filer file){
		if(tmpMem == 0){
			tmpMem = malloc(20);
			tmpMemSize = 20;
		}

		uint cols = 0;
		uint rows = 0;

		char nc = null;
		char c;

		while(1){
			if(nc == null) c = fileGetByte(file);
			else{
				c = nc;
				nc = null;
			}

			cols++;

			if(c == null) break; // EOF

			uint startCols = cols;
			uint startRows = rows;

			if( lexerTestSimbol(c) ) nc = lexerGetWord(c, file, &cols);
			else { // spec simbols
				if( c == '\n' ){
					rows++;
					cols = 0;
				} else if( (c >= 1) && (c <= 32) ) continue;

				else if( c == '\"' ){
					uint i=0;
					while(1){
						c = fileGetByte(file);
						if( c == '\"') break;
						tmpMem[i] = c;
						i++;
						lexerHandleTmpMem(i);
						cols++;
					}
					tmpMem[i] = 0;

					char *str = malloc( strlen(tmpMem) );
					strcpy(str, tmpMem);
					lexerAddTocken(2, str, startCols, startRows);

				} else if( c == '\'' ){
					uint i=0;
					while(1){
						c = fileGetByte(file);
						if( c == '\'') break;
						tmpMem[i] = c;
						i++;
						lexerHandleTmpMem(i);
						cols++;
					}
					tmpMem[i] = 0;

					char *str = malloc( strlen(tmpMem) );
					strcpy(str, tmpMem);
					lexerAddTocken(2, str, startCols, startRows);

				} else if( ( c >= 33 && c <= 47 ) || ( c >= 58 && c <= 64 ) || ( c >= 91 && c <= 95 ) || ( c >= 123 && c <= 126 ) )
						lexerAddTocken(c, 0, cols, rows);

				else { // ERROR
					printf("LEXER ERROR: unknown symbol on %u:%u (line:rows) = \'%c\';\n", cols-1, rows, c);
					exit(1);
	               }
	               continue;
			}

			char *str = malloc( strlen(tmpMem) );
			strcpy(str, tmpMem);
			lexerAddTocken(1, str, startCols, startRows);

		}

		lexerAddTocken(0, 0, cols, rows); // end
		return lexerTocken;


		free(tmpMem);
		tmpMem = 0;
	}



	bool lexerTestSimbol(char c){
		if( ( c >= 48 && c <= 57 ) || ( c >= 65 && c <= 90 ) || ( c >= 97 && c <= 122 ) || ( c == 95 ) ) return true;
		return false;
	}

	char lexerGetWord(char c, filer file, uint *cols){

		tmpMem[0] = c;
		uint index = 1;

		while( (c = fileGetByte(file)) != null ){
			(*cols)++;
			if( !lexerTestSimbol(c) ) break;
			tmpMem[index] = c;
			index++;
			lexerHandleTmpMem(index);
		}
		lexerHandleTmpMem(index+1);
		tmpMem[index] = 0;

        (*cols)--;

		return c;
	}

	void lexerHandleTmpMem(uint i){

		if(i >= tmpMemSize){
			tmpMemSize += 20;
			tmpMem = (char*)realloc(tmpMem, tmpMemSize);
		}
	}

	bool lexerIsNumber(char *str, uint cols, uint rows){
		if(str == 0) return false;

		bool wasBreak = false;
		bool wasHex = false;
		uint i = 0;

		if( (str[i] == '0') && (str[i+1] == 'x') ){
			i+=2;
			wasHex = true;
		}

		while( str[i] != 0 ){
			if( !( ((str[i] >= 48) && (str[i] <= 57)) || ( str[i] == '_' ) ) ){
				wasBreak = true;
				break;
			}
			i++;
        }

        if( wasBreak == true ){
        	if( (wasHex == true) && (str[i] == 'h') ){ // warning
        		printf("LEXER WARNING: Was \'0x\' and \'h\' on end number, on %u:%u;\n", cols, rows);
        	} else if( !( (str[i] == 'b') || (str[i] == 'h') || (str[i] == 'o') ) ) return false;
        }

		return true;
	}

	void lexerAddTocken(byte id, char *str, uint cols, uint rows){
		if( lexerTocken == 0 ){
			lexerTocken = malloc( sizeof( struct stLexerTocken ) );
			lexerTockenIndex = 0;
		} else lexerTocken = realloc( lexerTocken, sizeof( struct stLexerTocken ) * (lexerTockenIndex+1) );

		if( lexerIsNumber(str, cols, rows) ) lexerTocken[lexerTockenIndex].id = 3;
		else lexerTocken[lexerTockenIndex].id = id;
		
		lexerTocken[lexerTockenIndex].cols = cols;
		lexerTocken[lexerTockenIndex].rows = rows;

		if( str == 0 ){
			lexerTocken[lexerTockenIndex].nt = 0;
		} else { // add to nameTable

			if( nameTable == 0 ){
				nameTable = malloc( sizeof(char*) );
				nameTable[0] = str;
				nameTableSize = 1;
			} else {
				uint wasNT = -1;
				for(uint i=0; i<nameTableSize; i++)
					if( strcmp(nameTable[i], str) == 0 ){
						wasNT = i;
						break;
					}

				if(wasNT != -1){
					lexerTocken[lexerTockenIndex].nt = wasNT+1;
					goto lexerPointEnd;
				} else {
					nameTable = realloc( nameTable, (sizeof(char*) * (nameTableSize + 1)) );
					nameTable[nameTableSize] = str;
					nameTableSize++;
				}
			}

			lexerTocken[lexerTockenIndex].nt = nameTableSize;

		}
		lexerPointEnd:

		lexerTockenIndex++;
	}

#endif
