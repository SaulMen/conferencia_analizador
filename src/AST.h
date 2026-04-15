#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

class Node {
public:
    virtual ~Node() = default;
    virtual std::string toJSON() = 0; 
};

class ObjectiveNode : public Node {
public:
    std::string desc;
    int reward;
    ObjectiveNode(std::string d, int r) : desc(d), reward(r) {}
    std::string toJSON() override {
        return "{\"descripcion\":\"" + desc + "\", \"recompensa\":" + std::to_string(reward) + "}";
    }
};

class QuestNode : public Node {
public:
    std::string title;
    std::vector<std::unique_ptr<Node>> objectives;
    QuestNode(std::string t) : title(t) {}
    std::string toJSON() override {
        std::string json = "{\"mision\":\"" + title + "\", \"objetivos\":[";
        for (size_t i = 0; i < objectives.size(); ++i) {
            json += objectives[i]->toJSON() + (i == objectives.size() - 1 ? "" : ",");
        }
        return json + "]}";
    }
};

#endif