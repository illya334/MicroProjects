// by zeq52giw 10.08.2024
// mov [name], 494C4C5941h

// Simple debuger

#ifndef _DBG
#define _DBG

	#include "lexer.h"
	#include "parser.h"

	// call dbgLexer(lexerTocken, 0, 0);
	void dbgLexer( struct stLexerTocken *tocken, uint size, uint tabs ){

		if( tocken == null ){
			printf("DBG lexer error: input null\n");
			return;
		}

		printf("LTocken (0x%X):\n", tocken);

		uint i=0;
		while( !( (i >= size) && (size != 0) ) ){
			for(uint i=0; i<tabs; i++) printf("\t");
			if(size == 0) printf("[%u]", i);

			printf("\tID: %u ", tocken[i].id);

			switch(tocken[i].id){
				case 0:
					printf("(end)");
					break;
				case 1:
					printf("id/string");
					break;
				case 2:
					printf("(string)");
					break;
				case 3:
					printf("(number)");
					break;
				default:
					if( tocken[i].id > 20 ) printf("(\'%c\')", tocken[i].id);
					else printf("(unknown)");
			}
			printf(";\n");

			for(uint i=0; i<tabs; i++) printf("\t");
			printf("\tNT: %u", tocken[i].nt);
			if(tocken[i].nt == 0) printf(";\n");
			else printf(" (\"%s\");\n", nameTable[ tocken[i].nt - 1 ]);

			for(uint i=0; i<tabs; i++) printf("\t");
			printf("\tPos: %u:%u;\n\n", tocken[i].cols, tocken[i].rows);


			if( (tocken[i].id == 0) || ( (i >= size) && (size != 0) ) ) break; // for watch 'end' tocken

			i++;
		}

		if(size == 0)
	    	printf("Tocken size: %u; summ: %u\n\n", sizeof(struct stLexerTocken), sizeof(struct stLexerTocken)*(i+1));

	}

	void dbgParser(struct stParserTocken *ptock, uint tabs){

		if(ptock == 0){
			printf("DBG parser error: input null\n");
			return;
		}

		if(ptock->com == 1){
            dbgLexer(ptock, 1, tabs + 1);
            return;
        }

		printf("PTocken (0x%X):\n", ptock);

		for(uint i=0; i<tabs; i++) printf("\t");
		printf("COM: %u", ptock->com);
		if(ptock->com > 20) printf(" (%c)", ptock->com);
		if(ptock->com == 2) printf(" (group)");
		if(ptock->com == 3) printf(" (Lexer group)");
		printf(";\n");

        if(ptock->com == 3){ // groupLTock

			printf("\tLTocks (size: %u): ", ptock->laSize);
			dbgLexer(ptock->la, ptock->laSize, tabs + 1);

		} else {
			for(uint i=0; i<tabs; i++) printf("\t");
			if(ptock->aIsL == true){
				printf("\tA is ltock: ");
				if(ptock->la != 0) dbgLexer(ptock->la, 1, tabs + 1);
				else printf("(null);\n");
			} else {
				printf("\tA is ptock: ");
				if(ptock->pa != 0) dbgParser(ptock->pa, tabs+1);
				else printf("(null);\n");
			}

			for(uint i=0; i<tabs; i++) printf("\t");
			if( (ptock->com > 2) ){

				if(ptock->bIsL == true){
					printf("\tB is ltock: ");
					if(ptock->lb != 0) dbgLexer(ptock->lb, 1, tabs + 1);
					else printf("(null);\n");
				} else {
					printf("\tB is ptock: ");
					if(ptock->pb != 0) dbgParser(ptock->pb, tabs+1);
					else printf("(null);\n");
				}
			}
		}

	}

#endif
