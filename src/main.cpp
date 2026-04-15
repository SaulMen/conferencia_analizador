#include "Scanner.h"
#include "Parser.h"

#include "httplib.h"

#ifdef DELETE
#undef DELETE
#endif
#ifdef ERROR
#undef ERROR
#endif

#include <iostream>

int main() {
    httplib::Server svr;
    std::cout << "Servidor listo en http://localhost:18080" << std::endl;

    svr.Options(R"(/analyze)", [](const httplib::Request& /*req*/, httplib::Response& res) {
        std::cout << "[LOG] Recibida petición OPTIONS (Pre-flight)" << std::endl;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    svr.Post("/analyze", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "[LOG] Recibida petición POST - Analizando..." << std::endl;
        
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");

        try {
            Scanner scanner(req.body);
            Parser parser(scanner);
            auto result = parser.parseQuest();
            res.set_content(result->toJSON(), "application/json");
            std::cout << "[LOG] Análisis exitoso" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "[ERROR] " << e.what() << std::endl;
            res.status = 400;
            res.set_content("{\"error\": \"" + std::string(e.what()) + "\"}", "application/json");
        }
    });

    svr.listen("0.0.0.0", 18080);
    return 0;
}