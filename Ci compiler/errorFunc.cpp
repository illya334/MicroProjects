// by Parfonov Illia

#include "includer.h"

void errorFatal(char *errorText){
	printf("Fatal error: %s.\n", errorText);
	exit(1);
}

void errorWarning(char *errorText){
	printf("Warning: %s.\n", errorText);
}