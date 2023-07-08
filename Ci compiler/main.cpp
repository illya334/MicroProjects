// Ci compiller by Illia Parfonov
//

#include "includer.h"
#include <conio.h>

char *dbgGetType(uint type);
void debug(struct stLexerElement *obj, uint t);

int main(int argc, char* argv[]){
	
	// only read
	struct stLexerElement back;
	struct stLexerElement now;
	struct stLexerElement next;

	openFileIn("C:\\testIn.txt");
	openFileOut("testOut.txt");

	lexerInit(&back, &now, &next);

	lexer(&now);
	lexer(&next);

	debug(&now, 0);
	
	debug(&next, 0);

	getch();

	return 0;
}

void debug(struct stLexerElement *obj, uint t){
	for(uint i=0; i<t; i++) printf("\t");

	printf("TYPE: %s\n", dbgGetType(obj->type));

	if(obj->name != null){
		for(uint i=0; i<t; i++) printf("\t");
		printf("NAME: \'%s\'\n", obj->name);
	}

	if(obj->parameters != null){
		for(uint i=0; i<t+1; i++) printf("\t");
		printf("Parameters:\n");
		for(uint i=0; i < obj->indexParameters; i++){
			debug( &(obj->parameters[i]), t+1);
		}
	} else if(obj->indexParameters != 0)
		printf("indexParameters: %d\n", obj->indexParameters);
	printf("\n");
}

char *dbgGetType(uint type){
	switch(type){
		case typeNone:
			return "typeNone";

		case typeInfo:
			return "typeInfo";

		case typeFunc:
			return "typeFunc";

		case typePreproc:
			return "typePreproc";

		case typeOperator:
			return "typeOperator";

		case typeGroup:
			return "typeGroup";

		case typeEndGroup:
			return "typeEndGroup";

		case typeComent:
			return "typeComent";

		case typeBackBracket:
			return "typeBackBracket";

		case typeEndComand:
			return "typeEndComand";

		default:
			reg char* type = (char*)malloc(20);
			sprintf(type, "%d", type);
			return type;
	}
}