#include "crow.h"
#include "crow/middlewares/session.h"
#include "crow/middlewares/cors.h"
#include "../includes/inja.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <bits/stdc++.h>
#include "shoeTree.hpp"
#include "utility.hpp" 



//create dynamic routes based on the q#.json files
void register_question_routes(crow::App<crow::CookieParser, crow::SessionMiddleware<crow::InMemoryStore> >& app, inja::Environment& env, inja::json& vars, std::string questions_dir)
{
    std::regex question_pattern(R"(q(\d+)\.json)"); //select q#.json file
    
    for(auto file : std::filesystem::directory_iterator(questions_dir))
    {
        // it hits a folder a link etc, skip
        if(!file.is_regular_file()) continue;
        std::string filename = file.path().filename().string();
        std::smatch match;
        if(std::regex_match(filename, match, question_pattern)) {
            std::string question_num = match[1];
            std::string route_path = "/q" + question_num;
            std::string full_path = file.path().string();
            
            //add the route and render the q#.json file if the route is called
            app.route_dynamic(route_path)
            .methods(crow::HTTPMethod::GET)
            ([&, full_path](const crow::request& req, crow::response& res){
                std::string result = env.render_file_with_json_file("../templates/question.html", full_path);
                res.write(result);
                res.end();
            });
        }
    }
      
}

//probably should not be using auto
//this grabs the selected response and takes the meterics it effects and stores it into the session variables
void storeResponses(auto &store, std::string metrics)
{
    //how are we going to grab both the metric name and value
    //break on ":"
    std::stringstream metrics_ss(metrics);
    std::string values;
    std::vector<std::string> parsed_metrics;
    std::vector<std::string> metric_titles;
    std::vector<std::string> metric_values;

    //break the meteric string into names and  values of the metrics that are being used
    while(std::getline(metrics_ss, values, ':'))
    {
        parsed_metrics.push_back(values);
    }
    values.clear();
    //split the names of the metrics
    std::stringstream metric_titles_ss(parsed_metrics[0]);
    while(std::getline(metric_titles_ss, values, ','))
    {
        metric_titles.push_back(values);
    }
    values.clear();
    //split the names of the values for the metrics
    std::stringstream metric_values_ss(parsed_metrics[1]);
    while(std::getline(metric_values_ss, values, ','))
    {
        metric_values.push_back(values);
    }
    values.clear();

    // load the metrics into the session store
    for( long unsigned int i = 0; i < metric_titles.size(); i++)
    {
        store.set(metric_titles[i], metric_values[i]);
    }

    return;
}

int main()
{
    //load the KD-tree of Shoes from the shoes json
    ShoeTree tree("../scraping/shoes.json");
    //tree.print();

    //define app to handle all the session data via cookies
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

    //initial home splash screen
    CROW_ROUTE(app, "/")([&](const crow::request& req){
        inja::Template temp = env.parse_template("../templates/index.html");
        std::string result = env.render(temp, vars);
        return crow::response{result};
    });

    //create some route to serve the css
    CROW_ROUTE(app, "/static/style.css")([&]{
        inja::Template temp = env.parse_template("../templates/style.css");
        std::string result = env.render(temp, vars);
        return crow::response{result};
    });

    //set up the dynamic question routes
    register_question_routes(app, env, vars, "../data");

    //show the perfect shoe for the user
    CROW_ROUTE(app, "/results")([&](const crow::request &req){
        
        //get the session data and dump it into a new shoe object
        auto& session = app.get_context<Session>(req);

        float weight =  Utility::parseWeight(session.get<std::string>("Weight", "None"));
        bool terrain = (session.get<std::string>("Terrain:", "None") == "Road")? true : false;
        bool arch_support = (session.get<std::string>("Arch support:", "None") == "Neutral")? true : false;
        float heel_height = Utility::convert_to_number(session.get<std::string>("Heel height:", "None"));
        float forefoot_height = Utility::convert_to_number(session.get<std::string>("Forefoot height:", "None"));
       
        bool arch_type = (session.get<std::string>("Arch type:", "None") == "High arch")? true : false;

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
        bool heel_tab = (session.get<std::string>("Heel tab", "None") == "None")? false : true;
        bool removable_insole = (session.get<std::string>("Removeable insole", "None") == "Yes") ? true : false;

        //create a new shoe object to run the search on
        Shoe perfect_shoe = Shoe("perfect_shoe", heel_stack, forefoot_stack, 
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
            removable_insole, terrain, arch_support, heel_height,
            forefoot_height, arch_type, brand, drop, secondary_foam_softness,
            heel_padding_durability, outsole_durability);

        //get the list of shoes back in order from closest
        //run knearestneighbors on the KD-Tree of running shoes
        std::vector<std::shared_ptr<Shoe>> results = tree.kNearestNeighbors(std::make_shared<Shoe>(perfect_shoe), ATTR);
        vars["shoes"] = nlohmann::json::array(); //json to use with rendering results
        for(std::shared_ptr<Shoe> s : results)
        {
            nlohmann::json shoe;
            shoe["name"] = s->name;
            shoe["price"] = s->price;
            vars["shoes"].push_back(shoe);
        }

        //load the vector into a map to parse the results
        inja::Template temp = env.parse_template("../templates/results.html");
        std::string result = env.render(temp, vars);
        return crow::response{result};
    });

    //after each question store the results into session data
    CROW_ROUTE(app, "/store").methods(crow::HTTPMethod::POST)([&](const crow::request &req){
        //get the session data
        auto& session = app.get_context<Session>(req);

        auto body = req.get_body_params();
        long int q = std::strtol(body.get("question_num"),NULL, 10);
        long int questions = std::strtol(body.get("questions"), NULL, 10);
        
        std::string metrics;
        crow::response res;
        std::string redir;

        //figure out what next question to redirect to or just to the results page
        if(q < questions){
            metrics = body.get("q" + std::to_string(q));
            redir = "http://localhost:18080/q" + std::to_string(q+1);
        }else {
            metrics = body.get("q" + std::to_string(q));
            redir = "http://localhost:18080/results";
        }
        //store the response string into the session data
        storeResponses(session, metrics);
        //go to the next question
        res.set_header("Location", redir);
        res.code = 302; 
        return res;

    });
    
       
    app.port(18080).run();
}
