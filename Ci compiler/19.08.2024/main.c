// by zeq52giw (illya334) 19.08.2024

#include <stdio.h>
#include <stdlib.h>

#include "filer.h"
#include "lexer.h"
#include "parser.h"
#include "dbg.h"

int main(){

    filer file = fileOpen("testIn.txt");

    struct stLexerTocken *lt = lexer(file);

    dbgLexer(lt, false, 0);

    struct stParserTocken *pt = parserCmdMulDiv(parserCmd(lt, false, 0));

    dbgParser(pt, 0);

    return 0;
}