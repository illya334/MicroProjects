// by zeq52giw 25.08.2024

#ifndef _PARSER
#define _PARSER

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include "defines.h"
	#include "lexer.h"

	/*
		stParserTocken.com:
			none = 0
			lexer = 1 ?
			group = 2 -> ( )
			groupLTock = 3 -> "int a"
			groupArr = 4 -> [ ]
	*/

	struct stParserTocken{
		byte com;

		bool aIsL;

		union{
			struct stLexerTocken *la;
			struct stParserTocken *pa;
		};

		bool bIsL;

		union{
			struct stLexerTocken *lb;
			struct stParserTocken *pb;
			uint laSize;
		};
	};

	void parserError(char *str, struct stLexerTocken *ltock);

	// обробляє тільки одну команду (до ; або до кінця файлу)
	struct stParserTocken *parserCmd( struct stLexerTocken *ltock, uint group, uint *offset ){

		// TODO: ( ) [ ] та обробка помилок. Все через group

		void *tmpTock;

		tmpTock = 0;

		struct stParserTocken *parserTocken = malloc( sizeof( struct stParserTocken ) );

		struct stParserTocken *nowPTock = parserTocken;
		struct stParserTocken *backPTock = 0;

		//bool waitQuit = false;

		bool tmpIsP = false;

		bool wasCom = false;

		uint i = 0;
		while(1){
			if( (ltock[i].id == ')') && (offset == 0) ) parserError("Not expected \')\'", &( ltock[i] ));
			if( (ltock[i].id == ']') && (offset == 0) ) parserError("Not expected \']\'", &( ltock[i] ));

			if( (ltock[i].id == 0) || (ltock[i].id == ';') || ( (group == true) && (ltock[i].id == ')') ) || ( (group == true) && (ltock[i].id == ']') ) ){

				if( (tmpTock != 0) ){
                    if(backPTock != 0){
                    	free(nowPTock);
						backPTock->bIsL = !tmpIsP;
						backPTock->lb = tmpTock;
                    } else {
                        free(parserTocken);
                        parserTocken = tmpTock;
                    }

				}

				break;
			}

			// if( waitQuit == true )
			//	 parserError("Unknown command", &( ltock[i] ));

			if( (ltock[i].id == '(') || (ltock[i].id == '[') ){
				i++;
				uint j = 0;

				struct stParserTocken *tmp = malloc( sizeof( struct stParserTocken ) );

				tmp->com = (ltock[i].id == '(') ? 2 : 4; // group
				tmp->aIsL = false;
				tmp->pa = parserCmd( &( ltock[i] ), true, &j );
				i += j;

				tmpIsP = true;
                tmpTock = tmp;

			} else if( ltock[i].id < 20 ){

				if(tmpTock == 0)
					tmpTock = &( ltock[i] );

				else if( ltock[i].id != 3 ){
					uint j=1;
					while(1){
						if( (ltock[j-1].id < 20) || (ltock[j-1].id == 0) ) break;
						j++;
					}

					struct stParserTocken *tmp = malloc( sizeof( struct stParserTocken ) );

					tmp->com = 3; // groupLTock
					tmp->aIsL = true;
					tmp->la = tmpTock;
					tmp->bIsL = false;
					tmp->laSize = j+1;

					tmpIsP = true;
					tmpTock = tmp;

					i += j-1;
				} else {
					parserError( "Unknown simbol/command", &(ltock[i]) );
				}

			} else {


				nowPTock->com = ltock[i].id;
				nowPTock->aIsL = !tmpIsP;
				nowPTock->la = tmpTock;

				tmpTock = 0;
				tmpIsP = false;

				nowPTock->bIsL = false;

				backPTock = nowPTock;

				struct stParserTocken *tmp = malloc( sizeof( struct stParserTocken ) );
				nowPTock->pb = tmp;
				nowPTock = tmp;

				wasCom = true;
			}

			i++;
		}

        if( offset != 0 ) *offset = i;

		return parserTocken;
	}


	// виставляє * / в правильному порядку
	struct stParserTocken *parserCmdMulDiv( struct stParserTocken *ptock ){

		while(1){
			if( (ptock->com == '*') || ptock->com == '/' ){

				if(ptock->bIsL == false){
					if( (ptock->pb->com == '+') || (ptock->pb->com == '-') ){
						struct stParserTocken *mulDiv = ptock;
						ptock = ptock->pb;

						mulDiv->pb = ptock->pa;
						mulDiv->bIsL = ptock->aIsL;

                        ptock->aIsL = false;
						ptock->pa = mulDiv;
						break;
					}
				}
			}

			if( (ptock->com > ' ') && (ptock->aIsL == false) ){
				struct stParserTocken *tmp = parserCmdMulDiv(ptock->pa);
				if(tmp != ptock->pa){
                    ptock->pa = tmp;
					continue;
				}
			}

			if( (ptock->com > ' ') && (ptock->bIsL == false) ){
				struct stParserTocken *tmp = parserCmdMulDiv(ptock->pb);
				if(tmp != ptock->pb){
                    ptock->pb = tmp;
                    continue;
                }
			}

            break;
		}

        return ptock;
	}

	void parserError(char *str, struct stLexerTocken *ltock){
		printf("ERROR Parser: %s on %u:%u;\n", str, ltock->cols, ltock->rows);
		exit(1);
	}


#endif
