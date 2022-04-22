// #include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"


// you can declare prototypes of parser functions below
ParserInfo classDeclare();
ParserInfo memberDeclare();
ParserInfo classVarDeclare();
ParserInfo type();
ParserInfo subroutineDeclare();
ParserInfo paramList();
ParserInfo subroutineBody();
ParserInfo statement();
ParserInfo varDeclareStatement();
ParserInfo letStatement();
ParserInfo ifStatement();
ParserInfo whileStatement();
ParserInfo doStatement();
ParserInfo subroutineCall();
ParserInfo expressionList();
ParserInfo returnStatement();
ParserInfo expression();
ParserInfo relationalExpression();
ParserInfo arithmeticExpression();
ParserInfo term();
ParserInfo factor();
ParserInfo operand();

// File name for self text
/*char *fileName = "Ball.jack";
const char* file_source = "E:\\Compiler Design and Construction\\Parser_Program\\Parser Files\\";*/

// File name for final text
extern char fileName[1000];

extern FILE *ioFile;
extern int line;

int InitParser (char* file_name)
{
    InitLexer(file_name);
    return 1;
}

ParserInfo classDeclare () {
    Token next_token = GetNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if (next_token.tp == RESWORD && strcmp(next_token.lx, "class") == 0) {}
    else {
        parserInfo.er = classExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = GetNextToken();
    if (next_token.tp == ID);
    else {
        parserInfo.er = idExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = GetNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, "{") == 0) {
        next_token = PeekNextToken();
        while (next_token.tp == RESWORD && (strcmp(next_token.lx, "static") == 0 ||
                strcmp(next_token.lx, "field") == 0 || strcmp(next_token.lx, "constructor") == 0 ||
                strcmp(next_token.lx, "function") == 0 || strcmp(next_token.lx, "method") == 0)) {
            parserInfo = memberDeclare();
            if (parserInfo.er == none) {}
            else {
                return parserInfo;
            }
            next_token = PeekNextToken();
        }

        next_token = GetNextToken();
        if (strcmp(next_token.lx, "}") == 0) {
            parserInfo.er = none;
            return parserInfo;
        }
        else {
            parserInfo.er = closeBraceExpected;
            //parserInfo.er = none;
            parserInfo.tk = next_token;
            return parserInfo;
        }
    } else {
        parserInfo.er = openBraceExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    return parserInfo;
}

ParserInfo memberDeclare () {
    Token next_token = PeekNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if (next_token.tp == RESWORD && (strcmp(next_token.lx, "static") == 0 || strcmp(next_token.lx, "field") == 0)) {
        parserInfo = classVarDeclare();
    } else if (next_token.tp == RESWORD && (strcmp(next_token.lx, "constructor") == 0 ||
            strcmp(next_token.lx, "function") == 0|| strcmp(next_token.lx, "method") == 0)) {
        parserInfo = subroutineDeclare();
    } else {
        parserInfo.er = memberDeclarErr;
        parserInfo.tk = next_token;
    }

    return parserInfo;
}

ParserInfo classVarDeclare() {
    Token next_token = GetNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if (next_token.tp == RESWORD && (strcmp(next_token.lx, "static") == 0 || strcmp(next_token.lx, "field") == 0)) {
        parserInfo = type();
    } else {
        parserInfo.er = classVarErr;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    if (parserInfo.er == none) {}
    else {
        return parserInfo;
    }

    next_token = GetNextToken();
    if (next_token.tp == ID) {}
    else {
        parserInfo.er = idExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = PeekNextToken();
    while (next_token.tp == SYMBOL && strcmp(next_token.lx, ",") == 0) {
        GetNextToken();
        next_token = GetNextToken();
        if (next_token.tp != ID) {
            parserInfo.er = idExpected;
            parserInfo.tk = next_token;
            return parserInfo;
        }
        next_token = PeekNextToken();
    }

    next_token = GetNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, ";") == 0) {}
    else {
        parserInfo.er = semicolonExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    return parserInfo;
}

ParserInfo type () {
    Token next_token = GetNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if ((next_token.tp == RESWORD && (strcmp(next_token.lx, "int") == 0 || strcmp(next_token.lx, "char") == 0 ||
            strcmp(next_token.lx, "boolean") == 0)) || next_token.tp == ID) {}
    else {
        parserInfo.er = illegalType;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    return parserInfo;
}

ParserInfo subroutineDeclare () {
    Token next_token = GetNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if (next_token.tp == RESWORD && (strcmp(next_token.lx, "constructor") == 0 ||
            strcmp(next_token.lx, "function") == 0 || strcmp(next_token.lx, "method") == 0)) {}
    else {
        parserInfo.er = subroutineDeclarErr;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = PeekNextToken();
    if (next_token.tp == RESWORD && (strcmp(next_token.lx, "void") == 0)) {
        GetNextToken();
    } else {
        parserInfo = type();
        if (parserInfo.er == none) {}
        else {
            return parserInfo;
        }
    }

    next_token = GetNextToken();
    if (next_token.tp == ID) {}
    else {
        parserInfo.er = idExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = GetNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, "(") == 0) {
        parserInfo = paramList();
        if (parserInfo.er != none) {
            return parserInfo;
        }
        next_token = GetNextToken();
        if (strcmp(next_token.lx, ")") == 0) {}
        else {
            parserInfo.er = closeParenExpected;
            parserInfo.tk = next_token;
            return parserInfo;
        }
    } else {
        parserInfo.er = openParenExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    parserInfo = subroutineBody();
    if (parserInfo.er == none) {}
    else {
        return parserInfo;
    }

    return parserInfo;
}

ParserInfo paramList () {
    Token next_token = PeekNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if ((next_token.tp == RESWORD && (strcmp(next_token.lx, "int") == 0 ||
            strcmp(next_token.lx, "char") == 0 || strcmp(next_token.lx, "boolean") == 0)) ||
            next_token.tp == ID){
        parserInfo = type();
        if (parserInfo.er == none) {}
        else {
            return parserInfo;
        }

        next_token = GetNextToken();
        if (next_token.tp == ID) {}
        else {
            parserInfo.er = idExpected;
            parserInfo.tk = next_token;
            return parserInfo;
        }

        next_token = PeekNextToken();
        while (next_token.tp == SYMBOL && strcmp(next_token.lx, ",") == 0) {
            GetNextToken();
            parserInfo = type();
            if (parserInfo.er == none) {}
            else {
                return parserInfo;
            }
            next_token = GetNextToken();
            if (next_token.tp == ID) {}
            else {
                parserInfo.er = idExpected;
                parserInfo.tk = next_token;
                return parserInfo;
            }
            next_token = PeekNextToken();
        }
    }

    return parserInfo;
}

ParserInfo subroutineBody () {
    Token next_token = GetNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if (next_token.tp == SYMBOL && strcmp(next_token.lx, "{") == 0) {
        next_token = PeekNextToken();
        while (next_token.tp == RESWORD &&
                (strcmp(next_token.lx, "var") == 0 || strcmp(next_token.lx, "while") == 0 ||
                strcmp(next_token.lx, "let") == 0 || strcmp(next_token.lx, "if") == 0 ||
                strcmp(next_token.lx, "do") == 0 || strcmp(next_token.lx, "return") == 0)) {
            parserInfo = statement();
            if (parserInfo.er == none) {}
            else {
                return parserInfo;
            }
            next_token = PeekNextToken();
        }
        next_token = GetNextToken();
        if (strcmp(next_token.lx, "}") == 0) {}
        else {
            parserInfo.er = closeBraceExpected;
            //parserInfo.er = none;
            parserInfo.tk = next_token;
            return parserInfo;
        }
    } else {
        parserInfo.er = openBraceExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    return parserInfo;
}

ParserInfo statement () {
    Token next_token = PeekNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if (next_token.tp == RESWORD && strcmp(next_token.lx, "var") == 0) {
        parserInfo = varDeclareStatement();
        if (parserInfo.er != none) {
            return parserInfo;
        }
    } else if (next_token.tp == RESWORD && strcmp(next_token.lx, "let") == 0) {
        parserInfo = letStatement();
        if (parserInfo.er != none) {
            return parserInfo;
        }
    } else if (next_token.tp == RESWORD && strcmp(next_token.lx, "if") == 0) {
        parserInfo = ifStatement();
        if (parserInfo.er != none) {
            return parserInfo;
        }
    } else if (next_token.tp == RESWORD && strcmp(next_token.lx, "while") == 0) {
        parserInfo = whileStatement();
        if (parserInfo.er != none) {
            return parserInfo;
        }
    } else if (next_token.tp == RESWORD && strcmp(next_token.lx, "do") == 0) {
        parserInfo = doStatement();
        if (parserInfo.er != none) {
            return parserInfo;
        }
    } else if (next_token.tp == RESWORD && strcmp(next_token.lx, "return") == 0){
        parserInfo = returnStatement();
        if (parserInfo.er != none) {
            return parserInfo;
        }
    } else {
        parserInfo.er = syntaxError;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    return parserInfo;
}

ParserInfo varDeclareStatement () {
    Token next_token = GetNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if (next_token.tp == RESWORD && strcmp(next_token.lx, "var") == 0) {
        parserInfo = type();
        if (parserInfo.er != none) {
            return parserInfo;
        }
    } else {
        parserInfo.er = syntaxError;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = GetNextToken();
    if (next_token.tp == ID) {}
    else {
        parserInfo.er = idExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = PeekNextToken();
    while (next_token.tp == SYMBOL && strcmp(next_token.lx, ",") == 0) {
        GetNextToken();
        next_token = GetNextToken();
        if (next_token.tp == ID) {}
        else {
            parserInfo.er = idExpected;
            parserInfo.tk = next_token;
            return parserInfo;
        }
        next_token = PeekNextToken();
    }

    next_token = GetNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, ";") == 0) {}
    else {
        parserInfo.er = semicolonExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }
    return parserInfo;
}

ParserInfo letStatement () {
    Token next_token = GetNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if (next_token.tp == RESWORD && strcmp(next_token.lx, "let") == 0 ) {}
    else {
        parserInfo.er = syntaxError;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = GetNextToken();
    if (next_token.tp == ID) {}
    else {
        parserInfo.er = idExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = PeekNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, "[") == 0) {
        GetNextToken();
        parserInfo = expression();
        if (parserInfo.er == none) {}
        else {
            return parserInfo;
        }
        next_token = GetNextToken();
        if (strcmp(next_token.lx, "]") == 0) {}
        else {
            parserInfo.er = closeBracketExpected;
            parserInfo.tk = next_token;
            return parserInfo;
        }
    }

    next_token = GetNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, "=") == 0) {
        parserInfo = expression();
        if (parserInfo.er != none) {
            return parserInfo;
        }
    } else {
        parserInfo.er = equalExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = GetNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, ";") == 0) {}
    else {
        parserInfo.er = semicolonExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    return parserInfo;
}

ParserInfo ifStatement () {
    Token next_token = GetNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if (next_token.tp == RESWORD && strcmp(next_token.lx, "if") == 0) {}
    else {
        parserInfo.er = syntaxError;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = GetNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, "(") == 0) {
        parserInfo = expression();
        if (parserInfo.er != none) {
            return parserInfo;
        }
        next_token = GetNextToken();
        if (strcmp(next_token.lx, ")") == 0) {}
        else {
            parserInfo.er = closeParenExpected;
            parserInfo.tk = next_token;
            return parserInfo;
        }
    } else {
        parserInfo.er = openParenExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = GetNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, "{") == 0) {
        next_token = PeekNextToken();
        while (next_token.tp == RESWORD && (strcmp(next_token.lx, "var") == 0 || strcmp(next_token.lx, "let") == 0 ||
                strcmp(next_token.lx, "if") == 0 || strcmp(next_token.lx, "while") == 0 ||
                strcmp(next_token.lx, "do") == 0 || strcmp(next_token.lx, "return") == 0)) {
            parserInfo = statement();
            if (parserInfo.er != none) {
                return parserInfo;
            }
            next_token = PeekNextToken();
        }
        next_token = GetNextToken();
        if (strcmp(next_token.lx, "}") == 0) {}
        else {
            parserInfo.er = closeBraceExpected;
            //parserInfo.er = none;
            parserInfo.tk = next_token;
            return parserInfo;
        }
    } else {
        parserInfo.er = openBraceExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = PeekNextToken();
    if (next_token.tp == RESWORD && strcmp(next_token.lx, "else") == 0) {
        GetNextToken();
        next_token = GetNextToken();
        if (next_token.tp == SYMBOL && strcmp(next_token.lx, "{") == 0) {
            next_token = PeekNextToken();
            while (next_token.tp == RESWORD && (strcmp(next_token.lx, "var") == 0 || strcmp(next_token.lx, "let") == 0 ||
                    strcmp(next_token.lx, "if") == 0 || strcmp(next_token.lx, "while") == 0 ||
                    strcmp(next_token.lx, "do") == 0 || strcmp(next_token.lx, "return") == 0)) {
                parserInfo = statement();
                if (parserInfo.er == none) {}
                else {
                    return parserInfo;
                }
                next_token = PeekNextToken();
            }
            next_token = GetNextToken();
            if (strcmp(next_token.lx, "}") == 0) {}
            else {
                parserInfo.er = closeBraceExpected;
                //parserInfo.er = none;
                parserInfo.tk = next_token;
                return parserInfo;
            }
        } else {
            parserInfo.er = openBraceExpected;
            parserInfo.tk = next_token;
            return parserInfo;
        }
    }
    return parserInfo;
}

ParserInfo whileStatement () {
    Token next_token = GetNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if (next_token.tp == RESWORD && strcmp(next_token.lx, "while") == 0) {}
    else {
        parserInfo.er = syntaxError;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = GetNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, "(") == 0) {
        parserInfo = expression();
        if (parserInfo.er == none) {}
        else {
            return parserInfo;
        }
        next_token = GetNextToken();
        if (strcmp(next_token.lx, ")") == 0) {}
        else {
            parserInfo.er = closeParenExpected;
            parserInfo.tk = next_token;
            return parserInfo;
        }
    } else {
        parserInfo.er = openParenExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = GetNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, "{") == 0) {
        next_token = PeekNextToken();
        while (next_token.tp == RESWORD && (strcmp(next_token.lx, "var") == 0 || strcmp(next_token.lx, "let") == 0 ||
                strcmp(next_token.lx, "if") == 0 || strcmp(next_token.lx, "while") == 0 ||
                strcmp(next_token.lx, "do") == 0 || strcmp(next_token.lx, "return") == 0)) {
            parserInfo = statement();
            if (parserInfo.er == none) {}
            else {
                return parserInfo;
            }
            next_token = PeekNextToken();
        }
        next_token = GetNextToken();
        if (strcmp(next_token.lx, "}") == 0) {}
        else {
            parserInfo.er = closeBraceExpected;
            //parserInfo.er = none;
            parserInfo.tk = next_token;
            return parserInfo;
        }
    } else {
        parserInfo.er = openBraceExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    return parserInfo;
}

ParserInfo doStatement () {
    Token next_token = GetNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;
    if (next_token.tp == RESWORD && strcmp(next_token.lx, "do") == 0) {
        parserInfo = subroutineCall();
        if (parserInfo.er != none) {
            return parserInfo;
        }
    } else {
        parserInfo.er = syntaxError;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = GetNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, ";") == 0) {}
    else {
        parserInfo.er = semicolonExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    return parserInfo;
}

ParserInfo subroutineCall () {
    Token next_token = GetNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if (next_token.tp == ID) {}
    else {
        parserInfo.er = idExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = PeekNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, ".") == 0) {
        GetNextToken();
        next_token = GetNextToken();
        if (next_token.tp == ID) {}
        else {
            parserInfo.er = idExpected;
            parserInfo.tk = next_token;
            return parserInfo;
        }
    }

    next_token = GetNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, "(") == 0) {
        parserInfo = expressionList();
        if (parserInfo.er == none) {}
        else {
            return parserInfo;
        }
        next_token = GetNextToken();
        if (strcmp(next_token.lx, ")") == 0) {}
        else {
            parserInfo.er = closeParenExpected;
            parserInfo.tk = next_token;
            return parserInfo;
        }
    } else {
        parserInfo.er = openParenExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    return parserInfo;
}

ParserInfo expressionList () {
    Token next_token = PeekNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if ((next_token.tp == SYMBOL && (strcmp(next_token.lx, "-") == 0 || strcmp(next_token.lx, "~") == 0 ||
            strcmp(next_token.lx, "(") == 0)) || next_token.tp == INT || next_token.tp == ID ||
            next_token.tp == STRING || (next_token.tp == RESWORD && (strcmp(next_token.lx, "true") == 0 ||
            strcmp(next_token.lx, "false") == 0 || strcmp(next_token.lx, "null") == 0 ||
            strcmp(next_token.lx, "this") == 0))) {
        parserInfo = expression();
        if (parserInfo.er == none) {}
        else {
            return parserInfo;
        }
        next_token = PeekNextToken();
        while (next_token.tp == SYMBOL && strcmp(next_token.lx, ",") == 0) {
            GetNextToken();
            parserInfo = expression();
            if (parserInfo.er == none) {}
            else {
                return parserInfo;
            }
            next_token = PeekNextToken();
        }
    }
    return parserInfo;
}

ParserInfo returnStatement () {
    Token next_token = GetNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if (next_token.tp == RESWORD && strcmp(next_token.lx, "return") == 0) {}
    else {
        parserInfo.er = syntaxError;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    next_token = PeekNextToken();
    if ((next_token.tp == SYMBOL && (strcmp(next_token.lx, "-") == 0 || strcmp(next_token.lx, "~") == 0 ||
            strcmp(next_token.lx, "(") == 0)) || next_token.tp == INT || next_token.tp == ID ||
            next_token.tp == STRING || (next_token.tp == RESWORD && (strcmp(next_token.lx, "true") == 0 ||
            strcmp(next_token.lx, "false") == 0 || strcmp(next_token.lx, "null") == 0 ||
            strcmp(next_token.lx, "this") == 0))) {
        parserInfo = expression();
        if (parserInfo.er == none) {}
        else {
            return parserInfo;
        }
    }

    next_token = GetNextToken();
    if (next_token.tp == SYMBOL && strcmp(next_token.lx, ";") == 0) {}
    else {
        parserInfo.er = semicolonExpected;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    return parserInfo;
}

ParserInfo expression () {
    Token next_token;
    ParserInfo parserInfo;
    parserInfo.er = none;

    parserInfo = relationalExpression();
    if (parserInfo.er == none) {}
    else {
        return parserInfo;
    }
    next_token = PeekNextToken();
    while (next_token.tp == SYMBOL && (strcmp(next_token.lx, "&") == 0 || strcmp(next_token.lx, "|") == 0)) {
        GetNextToken();
        parserInfo = relationalExpression();
        if (parserInfo.er == none) {}
        else {
            return parserInfo;
        }
        next_token = PeekNextToken();
    }

    return parserInfo;
}

ParserInfo relationalExpression () {
    Token next_token;
    ParserInfo parserInfo;
    parserInfo.er = none;

    parserInfo = arithmeticExpression();
    if (parserInfo.er == none) {}
    else {
        return parserInfo;
    }
    next_token = PeekNextToken();
    while (next_token.tp == SYMBOL && (strcmp(next_token.lx, "=") == 0 ||
            strcmp(next_token.lx, ">") == 0 || strcmp(next_token.lx, "<") == 0)) {
        GetNextToken();
        parserInfo = arithmeticExpression();
        if (parserInfo.er == none) {}
        else {
            return parserInfo;
        }
        next_token = PeekNextToken();
    }

    return parserInfo;
}

ParserInfo arithmeticExpression () {
    Token next_token;
    ParserInfo parserInfo;
    parserInfo.er = none;

    parserInfo = term();
    if (parserInfo.er == none) {}
    else {
        return parserInfo;
    }
    next_token = PeekNextToken();
    while (next_token.tp == SYMBOL && (strcmp(next_token.lx, "+") == 0 || strcmp(next_token.lx, "-") == 0)) {
        GetNextToken();
        parserInfo = term();
        if (parserInfo.er == none) {}
        else {
            return parserInfo;
        }
        next_token = PeekNextToken();
    }

    return parserInfo;
}

ParserInfo term () {
    Token next_token;
    ParserInfo parserInfo;
    parserInfo.er = none;

    parserInfo = factor();
    if (parserInfo.er == none) {}
    else {
        return parserInfo;
    }
    next_token = PeekNextToken();
    while (next_token.tp == SYMBOL && (strcmp(next_token.lx, "*") == 0 || strcmp(next_token.lx, "/") == 0)) {
        GetNextToken();
        parserInfo = factor();
        if (parserInfo.er == none) {}
        else {
            return parserInfo;
        }
        next_token = PeekNextToken();
    }

    return parserInfo;
}

ParserInfo factor () {
    Token next_token = PeekNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if (next_token.tp == SYMBOL && (strcmp(next_token.lx, "-") == 0 || strcmp(next_token.lx, "~") == 0)) {
        GetNextToken();
        parserInfo = operand();
        if (parserInfo.er == none) {}
        else {
            return parserInfo;
        }
    } else {
        parserInfo = operand();
        if (parserInfo.er == none) {}
        else {
            return parserInfo;
        }
    }

    return parserInfo;
}

ParserInfo operand () {
    Token next_token = GetNextToken();
    ParserInfo parserInfo;
    parserInfo.er = none;

    if (next_token.tp == INT || next_token.tp == STRING ||
            (next_token.tp == RESWORD && (strcmp(next_token.lx, "true") == 0 || strcmp(next_token.lx, "false") == 0 ||
            strcmp(next_token.lx, "null") == 0 || strcmp(next_token.lx, "this") == 0))) {}
    else if (next_token.tp == ID) {
        next_token = PeekNextToken();
        if (next_token.tp == SYMBOL && strcmp(next_token.lx, ".") == 0) {
            GetNextToken();
            next_token = GetNextToken();
            if (next_token.tp == ID) {}
            else {
                parserInfo.er = idExpected;
                parserInfo.tk = next_token;
                return parserInfo;
            }
        }

        next_token = PeekNextToken();
        if (next_token.tp == SYMBOL && strcmp(next_token.lx, "[") == 0) {
            GetNextToken();
            parserInfo = expression();
            if (parserInfo.er == none) {}
            else {
                return parserInfo;
            }
            next_token = GetNextToken();
            if (strcmp(next_token.lx, "]") == 0) {}
            else {
                parserInfo.er = closeBracketExpected;
                parserInfo.tk = next_token;
                return parserInfo;
            }
        } else if (next_token.tp == SYMBOL && strcmp(next_token.lx, "(") == 0) {
            GetNextToken();
            parserInfo = expressionList();
            if (parserInfo.er == none) {}
            else {
                return parserInfo;
            }
            next_token = GetNextToken();
            if (strcmp(next_token.lx, ")") == 0) {}
            else {
                parserInfo.er = closeParenExpected;
                parserInfo.tk = next_token;
                return parserInfo;
            }
        }
    } else if (next_token.tp == SYMBOL && strcmp(next_token.lx, "(") == 0) {
        parserInfo = expression();
        if (parserInfo.er == none) {}
        else {
            return parserInfo;
        }
        next_token = GetNextToken();
        if (strcmp(next_token.lx, ")") == 0) {}
        else {
            parserInfo.er = closeParenExpected;
            parserInfo.tk = next_token;
            return parserInfo;
        }
    } else {
        parserInfo.er = syntaxError;
        parserInfo.tk = next_token;
        return parserInfo;
    }

    return parserInfo;
}

ParserInfo Parse ()
{
    ParserInfo pi;

    // implement the function
    int true = 1;
    while (true) {
        Token next_token = GetNextToken();
        if (next_token.tp == ERR) {
            pi.er = lexerErr;
            pi.tk = next_token;
            return pi;
        } else if (next_token.tp == EOFile) {
            break;
        }
    }
    rewind(ioFile);
    line = 1;

    Token next_token = PeekNextToken();
    if (next_token.tp == RESWORD && strcmp(next_token.lx, "class") == 0) {
        while (next_token.tp == RESWORD && strcmp(next_token.lx, "class") == 0) {
            pi = classDeclare();
            next_token = PeekNextToken();
        }
    } else {
        pi.er = classExpected;
        pi.tk = next_token;
        return pi;
    }

    if (pi.er != none) {
        return pi;
    }

    // pi.er = none;
    return pi;
}


int StopParser () {
    StopLexer();
    return 1;
}

#ifndef TEST_PARSER
int main () {
    printf("Parser begin\n");
    InitParser("Ball.jack");
    ParserInfo parserInfo = Parse();
    printf("The error is: %d\nThe lexeme is: %s\n", parserInfo.er, parserInfo.tk.lx);
    printf("The line number is: %d", parserInfo.tk.ln);
    StopParser();
    return 1;
}
#endif
