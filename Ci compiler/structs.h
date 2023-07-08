#include "defines.h"

#define indexParametersMax 50

struct stLexerElement{
	byte type;
	char *name;
	struct stLexerElement *parameters;
	byte indexParameters; // кількість параметрів
	
};