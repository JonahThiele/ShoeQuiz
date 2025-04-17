#include "crow.h"
//this might need to be changed for cmake builds
#include "../includes/inja.hpp"
#include <fstream>
#include <iostream>


crow::response render_question(inja::json &vars, inja::Environment &env, std::string path) {
    try {
        std::string result = env.render_file_with_json_file("../templates/question.html", path);
        return crow::response{result};
    
    } catch (const std::exception& e) {
        return crow::response(404, std::string("Template or data error: ") + e.what());
    }
}

int main()
{
    crow::SimpleApp app; //define your crow application

    inja::Environment env;
    inja::json vars;
    //set the port, set the app to run on multiple threads, and run the app

    //initial home question
    CROW_ROUTE(app, "/")([&]{
        inja::Template temp = env.parse_template("../templates/index.html");
        std::string result = env.render(temp, vars);
        return crow::response{result};
    });

    //end points for all the various questions this was about as clean as I could get it
    CROW_ROUTE(app, "/q1")([&]{
        return render_question(vars, env, "../data/q1.json");
    });
    CROW_ROUTE(app, "/q2")([&]{
        return render_question(vars, env, "../data/q2.json");
    });

    CROW_ROUTE(app, "/q3")([&]{
        return render_question(vars, env, "../data/q3.json");
    });

    CROW_ROUTE(app, "/q4")([&]{
        return render_question(vars, env, "../data/q4.json");
    });
    CROW_ROUTE(app, "/q5")([&]{
        return render_question(vars, env, "../data/q5.json");
    });

    CROW_ROUTE(app, "/q6")([&]{
        return render_question(vars, env, "../data/q6.json");
    });

    CROW_ROUTE(app, "/q7")([&]{
        return render_question(vars, env, "../data/q7.json");
    });
    CROW_ROUTE(app, "/q8")([&]{
        return render_question(vars, env, "../data/q8.json");
    });

    CROW_ROUTE(app, "/q9")([&]{
        return render_question(vars, env, "../data/q9.json");
    });
       
    app.port(18080).run();
}
