#include "crow.h"
//this might need to be changed for cmake builds
#include "../includes/inja.hpp"
#include <fstream>
#include <iostream>


crow::response render_question(inja::json vars, inja::Environment env, int question_number) {
    try {
        inja::Template temp = env.parse_template("../templates/question.html");
        std::string result = env.render(temp, vars[question_number]);
        return crow::response{result};
    //this is bad design but it at least catches the exceptions
    } catch (...){
        return crow::response(404, "Template not found");
    }
}

int main()
{
    crow::SimpleApp app; //define your crow application
    
    std::fstream file("../data/questions.json");

    inja::Environment env;
    inja::json vars;
    file >> vars;
    //set the port, set the app to run on multiple threads, and run the app

    //initial home question
    CROW_ROUTE(app, "/")([&]{
        inja::Template temp = env.parse_template("../templates/index.html");
        std::string result = env.render(temp, vars);
        return crow::response{result};
    });

    //end points for all the various questions this was about as clean as I could get it
    CROW_ROUTE(app, "/q1")([&]{
        std::cout << "in q1" << std::endl;
        return render_question(vars, env, 0);
    });
    CROW_ROUTE(app, "/q2")([&]{
        return render_question(vars, env, 1);
    });

    CROW_ROUTE(app, "/q3")([&]{
        return render_question(vars, env, 2);
    });

    CROW_ROUTE(app, "/q4")([&]{
        return render_question(vars, env, 3);
    });
    CROW_ROUTE(app, "/q5")([&]{
        return render_question(vars, env, 4);
    });

    CROW_ROUTE(app, "/q6")([&]{
        return render_question(vars, env, 5);
    });

    CROW_ROUTE(app, "/q7")([&]{
        return render_question(vars, env, 6);
    });
    CROW_ROUTE(app, "/q8")([&]{
        return render_question(vars, env, 7);
    });

    CROW_ROUTE(app, "/q9")([&]{
        return render_question(vars, env, 8);
    });
       
    app.port(18080).run();
}
