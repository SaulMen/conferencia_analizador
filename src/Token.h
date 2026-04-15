#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class QSTokenType {
    TS_QUEST, 
    TS_OBJETIVO, 
    TS_RECOMPENSA, 
    TS_STRING, 
    TS_NUMBER, 
    TS_LBRACE, 
    TS_RBRACE, 
    TS_SEMICOLON, 
    TS_EOF,
    TS_ERROR
}; 

struct QSToken {
    QSTokenType type; 
    std::string lexeme;
    int line;
    int col;
};

#endif