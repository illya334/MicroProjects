// by Parfonov Illia

#include "includer.h"
#include "fileHandler.h"

struct stLexerElement *back;
struct stLexerElement *now;
struct stLexerElement *next;

const char operatorCharsEnd[] = {'(', ')', '[', ']', ',', '.', ';', ':', '\'', '\"', '`', '~', '?', '/', '\\', '*', '&', '^', '%', '$', '#', '@', '!', '-', '>', '<', '+', '=', '{', '}', '|', ' ' };
const byte operatorCharsEndLen = sizeof(operatorCharsEnd);

const char *operatorsWithB[] = {"if", "for", "while", "switch"};
const byte operatorsWithBLen = sizeof(operatorsWithB) / sizeof(char*);

void lexerInit( struct stLexerElement *backElement, struct stLexerElement *nowElement, struct stLexerElement *nextElement ){
	back = backElement;
	now = nowElement;
	next = nextElement;

}

void lexer( struct stLexerElement *elementOut ){
	char *buf = (char*)malloc(51);
	uint buflen = 50;
	uint bufIndex = 0;
	
	char c;
	bool btmp = false;
	bool btmp2 = false;

	while(1){
		c = getCharIn();
		if(c == null && bufIndex == 0){
			elementOut->type = typeNone; // END
			elementOut->name = null;
			elementOut->parameters = null;
			elementOut->indexParameters = null;
			jmp ret;
		}

		if( (c == ' ' || c == '\n' || c == '\t' ) && btmp2 == false) continue;
		else btmp2 = true;

		for(byte i=0; i < operatorCharsEndLen; i++){
			if(operatorCharsEnd[i] == c){
				btmp = true;
				break;
			}
		}

		if(btmp) break;

		if(bufIndex >= buflen){
			buflen += 50;
			buf = (char*)realloc(buf, buflen + 1);
		}

		buf[bufIndex] = c;
		buf[bufIndex + 1] = 0;
		bufIndex++;

	}

	if(c == ' ' || c == '\n' || c == '\t'){
		while(1){
			c = getCharIn();
			if(c == null && bufIndex == 0){
				elementOut->type = null;
				elementOut->name = null;
				elementOut->parameters = null;
				elementOut->indexParameters = null;
				jmp ret;
			} else if(c == null) break;

			if(c != ' ' && c != '\n' && c != '\t') break;
		}
	}

	if(c == '/'){
		while(1){
			c = getCharIn();
			if(c == null) break;
			if( c == ' ' || c == '\n' || c == '\t' ) continue;
			else break;
		}
		if(c == '/'){

			btmp = false;
			while(1){
				c = getCharIn();

				if(c == null) break;

				if(c == ' ' && btmp == false) continue;
				else btmp = true;

				buf[bufIndex] = c;
				bufIndex++;
				buf[bufIndex] = '\0';

				if(bufIndex >= buflen)
					buf = (char*)realloc(buf, buflen + 50);

				if( c == '\n') break;
			}

			elementOut->type = typeComent;
			elementOut->name = copystr(buf);
			elementOut->parameters = null;
			elementOut->indexParameters = null;
			jmp ret;
		} else if(c == '*'){
			btmp = false;
			btmp2 = false;
			while(1){
				c = getCharIn();

				if(c == null) break;

				if(c == ' ' && btmp == false) continue;
				else btmp = true;

				buf[bufIndex] = c;
				bufIndex++;
				buf[bufIndex] = '\0';

				if(bufIndex >= buflen)
					buf = (char*)realloc(buf, buflen + 50);
 
				if( c == '*' ) btmp2 = true;
				if( c == '/' && btmp2){
					buf[bufIndex--] = 0;
					buf[bufIndex--] = 0;
					buf[bufIndex--] = 0;
					
					break;
				}
			}

			elementOut->type = typeComent;
			elementOut->name = copystr(buf);
			elementOut->parameters = null;
			elementOut->indexParameters = null;
			jmp ret;
		} else {
			c = '/';
			jmp safeChar;
		}
	}

	if(c == '{'){
		elementOut->type = typeGroup;
		elementOut->name = null;
		elementOut->parameters = null;
		elementOut->indexParameters = null;
		jmp ret;
	}

	if(c == '}'){
		elementOut->type = typeEndGroup;
		elementOut->name = null;
		elementOut->parameters = null;
		elementOut->indexParameters = null;
		jmp ret;
	}

	if(c == ';'){
		elementOut->type = typeEndComand;
		elementOut->name = null;
		elementOut->parameters = null;
		elementOut->indexParameters = null;
		jmp ret;
	}

	if(c == ')' && bufIndex == 0){
		elementOut->type = typeBackBracket;
		elementOut->name = null;
		elementOut->parameters = null;
		elementOut->indexParameters = null;
		jmp ret;
	}

	if(c == '('){
		
		btmp = false;
		for(byte i=0; i < operatorsWithBLen; i++){
			if(cmpstr(buf, (char*)operatorsWithB[i])){
				btmp = true;
				break;
			}
		}

		if(btmp) // if, for, while, switch
			elementOut->type = typeOperator;
		else 
			elementOut->type = typeFunc;

		elementOut->name = copystr(buf);
		elementOut->indexParameters = 0;
		elementOut->parameters = (struct stLexerElement*)malloc(sizeof(struct stLexerElement) * 5);
		reg uint tmpMaxLen = 5;
		while(1){
			lexer( &(elementOut->parameters[elementOut->indexParameters]) );
			elementOut->indexParameters++;

			if(elementOut->indexParameters >= indexParametersMax) errorFatal("too many parameters");

			reg uint tmp = elementOut->parameters[elementOut->indexParameters-1].type;
			if(tmp == null || (tmp == typeBackBracket)) break;

			if(elementOut->indexParameters >= tmpMaxLen){
				tmpMaxLen += 5;
				elementOut->parameters = (struct stLexerElement*)realloc( elementOut->parameters, sizeof(struct stLexerElement) * tmpMaxLen );
			}
		}
		jmp ret;

	}

safeChar:

	if(bufIndex != 0){
		elementOut->name = copystr(buf);
		backCharIn();
	} else {
		elementOut->name = (char*)malloc(2);
		elementOut->name[0] = c;
		elementOut->name[1] = '\0';
	}

	elementOut->type = typeInfo;
	elementOut->parameters = null;
	elementOut->indexParameters = null;

ret:

	free(buf);

}