#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"
#include "AST.h"
#include <stdexcept>

class Parser {
private:
    Scanner& scanner;
    QSToken lookahead;

    void consume(QSTokenType type, std::string message);
    std::unique_ptr<Node> parseObjective();

public:
    Parser(Scanner& s);
    std::unique_ptr<QuestNode> parseQuest();
};

#endif