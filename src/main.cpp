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
#include "shoeTree.hpp"
#include "utility.hpp"

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
    //fingers crossed
    ShoeTree tree("../scraping/shoes.json");
    //tree.print();

    //define app to handle all the session data
    using Session = crow::SessionMiddleware<crow::InMemoryStore>;
    crow::App<crow::CookieParser, Session> app{Session{
        crow::CookieParser::Cookie("session")
            .max_age(/*one day*/ 24 * 60 * 60)
            .path("/")
            .httponly()
            .same_site(crow::CookieParser::Cookie::SameSitePolicy::Lax),

        4,
        crow::InMemoryStore{}
    }};

    inja::Environment env;
    inja::json vars;
    //set the port, set the app to run on multiple threads, and run the app

    //initial home question
    CROW_ROUTE(app, "/")([&](const crow::request& req){
        inja::Template temp = env.parse_template("../templates/index.html");
        std::string result = env.render(temp, vars);

        //set up the session 
        auto& session = app.get_context<Session>(req);
        session.set("hi", 1);

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
    
    CROW_ROUTE(app, "/q10")([&]{
        return render_question(vars, env, "../data/q10.json");
    });

    //just process all the data in this step aggregated from all the question pages
    CROW_ROUTE(app, "/results")([&](const crow::request &req){
        
        //create the perfect shoe from all the stored answers of the questions in the session data
        auto& session = app.get_context<Session>(req);

        std::string name = session.get<std::string>("name");
        std::string terrain = session.get<std::string>("Terrain:", "None");
        std::string arch_support = session.get<std::string>("Arch support:", "None");
        float heel_height = Utility::convert_to_number(session.get<std::string>("Heel height:", "None"));
        float forefoot_height = Utility::convert_to_number(session.get("Forefoot height:", "None"));
        std::string collection = session.get<std::string>("Collection:", "None");
        float weight = 0;
        std::vector<std::string> pronation = Utility::convert_to_list(session.get<std::string>("Pronation:", "None"), '|');
        std::string arch_type = session.get<std::string>("Arch type:", "None");
        std::vector<std::string> material = Utility::convert_to_list(session.get<std::string>("Material:", "None"), '|');
        std::vector<std::string> features = Utility::convert_to_list(session.get<std::string>("Features:", "None"), '|');
        std::vector<std::string> strike_pattern = Utility::convert_to_list(session.get<std::string>("Strike Pattern:", "None"), '|');
        std::vector<std::string> season = Utility::convert_to_list(session.get<std::string>("Season:", "None"), '|');
        std::string brand = session.get<std::string>("BRAND Brand:", "None");
        std::vector<std::string> type = Utility::convert_to_list(session.get<std::string>("Type:", "None"), '|');
        std::vector<std::string> pace = Utility::convert_to_list(session.get<std::string>("Pace:", "None"), ',');
        std::vector<std::string> distance = Utility::convert_to_list(session.get<std::string>("Race distance:", "None"), '|');
        float heel_stack = Utility::convert_to_number(session.get<std::string>("Heel stack", "None"));
        float forefoot_stack = Utility::convert_to_number(session.get<std::string>("Forefoot stack", "None"));
        float drop = Utility::convert_to_number(session.get<std::string>("Drop", "None"));
        float midsole_softness = Utility::convert_to_number(session.get<std::string>("Midsole softness", "None"));
        float secondary_foam_softness = Utility::convert_to_number(session.get<std::string>("Secondary foam softness", "None"));
        float midsole_softness_in_cold = Utility::convert_to_number(session.get<std::string>("Midsole softness in cold", "None"));
        float midsole_softness_in_cold_per = Utility::convert_to_number(session.get<std::string>("Midsole softness in cold (%)", "None"));
        float insole_thickness = Utility::convert_to_number(session.get<std::string>("Insole thickness", "None"));
        std::string size = session.get<std::string>("Size", "None");
        float toebox_width_widest_prt = Utility::convert_to_number(session.get<std::string>("Toebox width - widest part (new method)", "None"));
        float toebox_width_big_toe = Utility::convert_to_number(session.get<std::string>("Toebox width - big toe (new method)", "None"));
        float toebox_height = Utility::convert_to_number(session.get<std::string>("Toebox height", "None"));
        int torsional_rigidity = (int)Utility::convert_to_number(session.get<std::string>("Torsional rigidity", "None"));
        int heel_counter_stiffness = (int)Utility::convert_to_number(session.get<std::string>("Heel counter stiffness", "None"));
        float midsole_width_forefoot = Utility::convert_to_number(session.get<std::string>("Midsole width - forefoot", "None"));
        float midesole_width_heel = Utility::convert_to_number(session.get<std::string>("Midsole width - heel", "None"));
        float flexibility = Utility::convert_to_number(session.get<std::string>("Flexibility / Stiffness (new method)", "None"));
        float stiffness_in_cold = Utility::convert_to_number(session.get<std::string>("Stiffness in cold", "None"));
        float stiffness_in_cold_per = Utility::convert_to_number(session.get<std::string>("Stiffness in cold (%)", "None"));
        int breathability = (int)Utility::convert_to_number(session.get<std::string>("Breathability", "None"));
        int toebox_durability = (int)Utility::convert_to_number(session.get<std::string>("Toebox durability", "None"));
        int heel_padding_durability = (int)Utility::convert_to_number(session.get<std::string>("Heel padding durability", "None"));
        float outsole_hardness = Utility::convert_to_number(session.get<std::string>("Outsole hardness", "None"));
        float outsole_durability = Utility::convert_to_number(session.get<std::string>("Outsole durability", "None"));
        float outsole_thickness = Utility::convert_to_number(session.get<std::string>("Outsole thickness", "None"));
        std::string price_str = session.get<std::string>("Price", "None");
        price_str.erase(0,1);
        float price = Utility::convert_to_number(price_str);
        bool reflective_elements = (session.get<std::string>("Reflective elements", "None") == "Yes") ? true : false;
        float tongue_padding = Utility::convert_to_number(session.get<std::string>("Tongue padding", "None"));
        // probaly disregard this as well "Tongue: gusset type": "Both sides (semi)",
        bool heel_tab = (session.get<std::string>("Heel tab", "None") == "None")? false : true;
        bool removable_insole = (session.get<std::string>("Removeable insole", "None") == "Yes") ? true : false;


        Shoe perfect_shoe = Shoe(name, heel_stack, forefoot_stack, 
            midsole_softness, midsole_softness_in_cold, 
            midsole_softness_in_cold_per, insole_thickness, 
            size, toebox_width_widest_prt, 
            toebox_width_big_toe, toebox_height, torsional_rigidity,
            heel_counter_stiffness, midsole_width_forefoot,
            midesole_width_heel, flexibility,
            stiffness_in_cold, stiffness_in_cold_per,
            weight, breathability, toebox_durability,
            outsole_hardness, outsole_thickness,
            price, reflective_elements,
            tongue_padding, heel_tab,
            removable_insole );

        //get the list of shoes back 
        std::vector<std::shared_ptr<Shoe>> results = tree.kNearestNeighbors(std::make_shared<Shoe>(perfect_shoe), ATTR);
        vars["shoes"] = nlohmann::json::array();
        for(std::shared_ptr<Shoe> s : results)
        {
            nlohmann::json shoe;
            shoe["name"] = s->name;
            shoe["price"] = s->price;
            vars["shoes"].push_back(shoe);
        }

        //load the vector into a map to parse the results
        inja::Template temp = env.parse_template("../templates/results.html");
        std::string result = env.render(temp, vars); // this should brick that is okay
        return crow::response{result};
    });

    CROW_ROUTE(app, "/store").methods(crow::HTTPMethod::POST)([&](const crow::request &req){
        //get the session data
        auto& session = app.get_context<Session>(req);

        //return the amount of answers and map them to the shoe class

        auto body = req.get_body_params();
        long int q = std::strtol(body.get("question_num"),NULL, 10);

        std::string metrics;

        //this works but causes the screen to flash a bit when the redirect doesn't go immediately, Idk how to fix that
        crow::response res;
        std::string redir;
        //later once this is working clean this up because it is unnecessary and ugly
        switch(q)
        {
            case 1:
                redir = "http://localhost:18080/q2";
                metrics = body.get("q1");
                break;
            case 2:
                redir = "http://localhost:18080/q3";
                metrics = body.get("q2");
                break;
            case 3:
                redir = "http://localhost:18080/q4";
                metrics = body.get("q3");
                break;
            case 4:
                redir = "http://localhost:18080/q5";
                metrics = body.get("q4");
                break;
            case 5:
                redir = "http://localhost:18080/q6";
                metrics = body.get("q5");    
                break;
            case 6:
                redir = "http://localhost:18080/q7";
                metrics = body.get("q6");    
                break;
            case 7:
                redir = "http://localhost:18080/q8";
                metrics = body.get("q7"); 
                break;
            case 8:
                redir = "http://localhost:18080/q9";
                metrics = body.get("q8"); 
                break;
            case 9:
                redir = "http://localhost:18080/q10";
                metrics = body.get("q9"); 
                break;
            case 10:
                redir = "http://localhost:18080/results";
                metrics = body.get("q10"); 
            case 0:
                //time for some strange return logic so that we don't have to duplicate all the store response lines for each question page
                std::cout << "";

        }

        storeResponses(session, metrics);
        auto keys = session.keys();
        res.set_header("Location", redir);
        res.code = 302; 
        return res;

    });
    
       
    app.port(18080).run();
}
