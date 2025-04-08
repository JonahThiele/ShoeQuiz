#include "crow.h"
//this might need to be changed for cmake builds
#include "../includes/inja.hpp"
#include <fstream>

int main()
{
    crow::SimpleApp app; //define your crow application
    
    inja::Environment env;

    //define your endpoint at the root directory
    CROW_ROUTE(app, "/")([&](const crow::request& req){
        //set some vars in the inja template via json
        inja::json vars;
        vars["title"] = "Inja + Crow";
        vars["message"] = "Hello, welcome to Crow + Inja!";

        //load the template from a file
        //remember this is going to be running from the build dir
        inja::Template temp = env.parse_template("../templates/index.html");
        std::string result = env.render(temp, vars);
        return crow::response{result};
    });
    //set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
}
