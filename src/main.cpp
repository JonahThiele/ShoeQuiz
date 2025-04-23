#include "crow.h"
#include "crow/middlewares/session.h"
#include "crow/middlewares/cors.h"
//this might need to be changed for cmake builds
#include "../includes/inja.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
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


//I have no idea what the get_context returns so I am using auto
void storeResponses(auto &store, std::string metrics)
{
    //how are we going to grab both the metric name and value
    //break on ":"
    std::stringstream metrics_ss(metrics);
    std::string values;
    std::vector<std::string> parsed_metrics;
    std::vector<std::string> metric_titles;
    std::vector<std::string> metric_values;

    while(std::getline(metrics_ss, values, ':'))
    {
        parsed_metrics.push_back(values);
    }
    values.clear();
    std::stringstream metric_titles_ss(parsed_metrics[0]);
    while(std::getline(metric_titles_ss, values, ','))
    {
        metric_titles.push_back(values);
    }
    values.clear();
    std::stringstream metric_values_ss(parsed_metrics[0]);
    while(std::getline(metric_values_ss, values, ','))
    {
        metric_values.push_back(values);
    }
    values.clear();

    //there probaly should be some error handling to make sure the vectors match up in lenght or I should have used a simple map
    for( long unsigned int i = 0; i < metric_titles.size(); i++)
    {
        store.set(metric_titles[i], metric_values[i]);
    }

    return;
}

int main()
{

    //define app to handle all the session data
    using Session = crow::SessionMiddleware<crow::InMemoryStore>;
    crow::App<crow::CookieParser, Session> app{Session{
        crow::CookieParser::Cookie("session")
            .max_age(/*one day*/ 24 * 60 * 60)
            .path("/")
            .httponly(),

        4,
        crow::InMemoryStore{}
    }};

    //maybe turning off cors will fix the cookie issues
    // auto &cors = app.get_middleware<crow::CORSHandler>();


    // cors
    //   .global()
    //     .headers("X-Custom-Header", "Upgrade-Insecure-Requests")
    //     .methods("POST"_method, "GET"_method)
    //   .prefix("/cors")
    //     .origin("localhost:18080")
    //   .prefix("/nocors")
    //     .ignore();

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

    //create some route to serve the css, we should make this more clean and extendable later
    CROW_ROUTE(app, "/static/style.css")([&]{
        inja::Template temp = env.parse_template("../templates/style.css");
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
        auto& session = app.get_context<Session>(req);
        session.set("init", "true");

        //return the amount of answers and map them to the shoe class

        auto body = req.get_body_params();
        long int q = std::strtol(body.get("question_num"),NULL, 10);

        std::string metrics;

        //this works but causes the screen to flash a bit when the redirect doesn't go immediately, Idk how to fix that
        crow::response  res = crow::response(302, "redirect");
        switch(q)
        {
            case 1:
                res.set_header("Location", "http://localhost:18080/q2");
                metrics = body.get("q1");
                break;
            case 2:
                res.set_header("Location", "http://localhost:18080/q3");
                metrics = body.get("q2");
                break;
            case 3:
                res.set_header("Location", "http://localhost:18080/q4");
                metrics = body.get("q3");
                break;
            case 4:
                res.set_header("Location", "http://localhost:18080/q5");
                metrics = body.get("q4");
                break;
            case 5:
                res.set_header("Location", "http://localhost:18080/q6");
                metrics = body.get("q5");    
                break;
            case 6:
                res.set_header("Location", "http://localhost:18080/q7");
                metrics = body.get("q6");    
                break;
            case 7:
                res.set_header("Location", "http://localhost:18080/q8");
                metrics = body.get("q7"); 
                break;
            case 8:
                res.set_header("Location", "http://localhost:18080/q9");
                metrics = body.get("q8"); 
                break;
            case 9:
                res.set_header("Location", "http://localhost:18080/q10");
                metrics = body.get("q9"); 
                break;
            case 10:
                res.set_header("Location", "http://localhost:18080/results");
                metrics = body.get("q10"); 
            case 0:
                //time for some strange return logic so that we don't have to duplicate all the store response lines for each question page
                std::cout << "";

        }

        storeResponses(session, metrics);
        auto keys = session.keys();

        return res;

    });
    
       
    app.port(18080).run();
}
