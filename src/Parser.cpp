#include "Parser.h"

Parser::Parser(Scanner& s) : scanner(s) {
    lookahead = scanner.nextToken();
}

void Parser::consume(QSTokenType type, std::string message) {
    if (lookahead.type == type) {
        lookahead = scanner.nextToken();
    } else {
        throw std::runtime_error("Error: " + message);
    }
}

std::unique_ptr<Node> Parser::parseObjective() {
    consume(QSTokenType::TS_OBJETIVO, "Se esperaba 'OBJECTIVE'");
    std::string desc = lookahead.lexeme;
    consume(QSTokenType::TS_STRING, "Se esperaba la descripcion del objetivo");
    consume(QSTokenType::TS_RECOMPENSA, "Se esperaba 'XP'");
    int reward = std::stoi(lookahead.lexeme);
    consume(QSTokenType::TS_NUMBER, "Se esperaba valor numerico");
    consume(QSTokenType::TS_SEMICOLON, "Se esperaba ';'");
    return std::make_unique<ObjectiveNode>(desc, reward);
}

std::unique_ptr<QuestNode> Parser::parseQuest() {
    consume(QSTokenType::TS_QUEST, "Se esperaba 'MISSION'");
    std::string title = lookahead.lexeme;
    consume(QSTokenType::TS_STRING, "Se esperaba titulo de la mision");
    consume(QSTokenType::TS_LBRACE, "Se esperaba '{'");
    
    auto quest = std::make_unique<QuestNode>(title);
    
    while (lookahead.type == QSTokenType::TS_OBJETIVO) {
        quest->objectives.push_back(parseObjective());
    }
    
    consume(QSTokenType::TS_RBRACE, "Se esperaba '}'");
    return quest;
}