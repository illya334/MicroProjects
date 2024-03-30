#ifndef _defines
#define _defines

	#include <stdint.h>

	#define uint unsigned int
	#define ushort unsigned short
	#define null NULL
	#define byte uint8_t
	#define word uint16_t
	#define dword uint32_t
	#define bool byte

	#define reg register

	#define true 1
	#define false 0

	#define wchar wchar_t

	#define jmp goto

	// #define indexParametersMax 50
	// ^^^
	// see in file struct.h


	enum{ // struct stLexerElement.type
		typeNone = 0,	// END
		typeInfo,		// 'A', "Hello", 123, 3.14, 0x13, char, int, ...
		typeFunc,		// void func( parameters ) command
		typePreproc,	// #define a 10
		typeOperator,	// if(typeCommandCondition) command, for(command, typeCommandCondition; command) command, dd, db, ...
		typeGroup,		// {
		typeEndGroup,	// }
		typeComent,		// //, /* */
		typeBackBracket,// )
		typeEndComand,	// ;

	};

#endif