#include "Scanner.h"
#include <cctype>

Scanner::Scanner(std::string src) : source(src) {}

bool Scanner::isAtEnd() { return current >= source.length(); }

char Scanner::advance() {
    col++;
    return source[current++];
}

char Scanner::peek() { return isAtEnd() ? '\0' : source[current]; }

void Scanner::skipWhitespace() {
    while (true) {
        char c = peek();
        switch (c) {
            case ' ': case '\r': case '\t': advance(); break;
            case '\n': line++; col = 1; advance(); break;
            default: return;
        }
    }
}

QSToken Scanner::makeToken(QSTokenType type) {
    std::string text = source.substr(start, current - start);
    return {type, text, line, col - (int)text.length()};
}

QSToken Scanner::nextToken() {
    skipWhitespace();
    start = current;
    if (isAtEnd()) return makeToken(QSTokenType::TS_EOF);

    char c = advance();

    if (isalpha(c)) {
        while (isalnum(peek())) advance();
        std::string text = source.substr(start, current - start);
        
        if (text == "MISSION") return makeToken(QSTokenType::TS_QUEST); 
        if (text == "OBJECTIVE") return makeToken(QSTokenType::TS_OBJETIVO);
        if (text == "XP") return makeToken(QSTokenType::TS_RECOMPENSA);
        
        return makeToken(QSTokenType::TS_STRING); 
    }

    if (isdigit(c)) {
        while (isdigit(peek())) advance();
        return makeToken(QSTokenType::TS_NUMBER);
    }

    if (c == '"') {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') { line++; col = 1; }
            advance();
        }
        
        if (isAtEnd()) return {QSTokenType::TS_ERROR, "Cadena no cerrada", line, col};
        
        std::string text = source.substr(start + 1, current - start - 1);
        
        advance();
        return {QSTokenType::TS_STRING, text, line, col - (int)text.length()};
    }

    switch (c) {
        case '{': return makeToken(QSTokenType::TS_LBRACE);
        case '}': return makeToken(QSTokenType::TS_RBRACE);
        case ';': return makeToken(QSTokenType::TS_SEMICOLON);
        default: return {QSTokenType::TS_ERROR, std::string(1, c), line, col};
    }

    return {QSTokenType::TS_ERROR, std::string(1, c), line, col};
}