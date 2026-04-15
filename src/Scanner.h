#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include "Token.h"

class Scanner {
private:
    std::string source;
    size_t start = 0;
    size_t current = 0;
    int line = 1;
    int col = 1;

    bool isAtEnd();
    char advance();
    char peek();
    void skipWhitespace();
    QSToken makeToken(QSTokenType type); 

public:
    Scanner(std::string src);
    QSToken nextToken();
};

#endif