#include "crow.h"
#include "crow/middlewares/session.h"
//this might need to be changed for cmake builds
#include "../includes/inja.hpp"
#include <fstream>
#include <iostream>
#include "shoe.hpp"

//some annoying gets from the query string request 
#include <bits/stdc++.h>


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

    //define app to handle all the session data
    crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>> app{crow::SessionMiddleware<crow::FileStore>{
        crow::FileStore{"/tmp/sessiondata"}
    }};

    //set up a session to handle the questions storing the results
    

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
    //make these endpoints somehow dynamically
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

    CROW_ROUTE(app, "/store").methods(crow::HTTPMethod::POST)([&](const crow::request &req){

        //get the session data
        auto& session = app.get_context<crow::SessionMiddleware<crow::FileStore>>(req);

        //return the amount of answers and map them to the shoe class

        auto body = req.get_body_params();
        //std::cout << body.get("question_num");
        long int q = std::strtol(body.get("question_num"),NULL, 10);

        //this works but causes the screen to flash a bit when the redirect doesn't go immediately, Idk how to fix that
        crow::response  res = crow::response(302, "redirect");
        switch(q)
        {
            case 1:
                res.set_header("Location", "http://localhost:18080/q2");
                break;
            case 2:
                res.set_header("Location", "http://localhost:18080/q3");
                break;
            case 3:
                res.set_header("Location", "http://localhost:18080/q4");
                break;
            case 4:
                res.set_header("Location", "http://localhost:18080/q5");
                break;
            case 5:
                res.set_header("Location", "http://localhost:18080/q6");    
                break;
            case 6:
                res.set_header("Location", "http://localhost:18080/q7");    
                break;
            case 7:
                res.set_header("Location", "http://localhost:18080/q8");
                break;
            case 8:
                res.set_header("Location", "http://localhost:18080/q9");
                break;
            case 9:
                res.set_header("Location", "http://localhost:18080/q10");
                break;
            case 10:
                res.set_header("Location", "http://localhost:18080/results");
                break;

        }
            return res;
    });
    
       
    app.port(18080).run();
}
