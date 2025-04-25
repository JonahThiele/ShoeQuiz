#include "shoeGraph.hpp"
#include "shoe.hpp"
#include <fstream>
#include <iostream>
#include "../includes/json.hpp"

//some small data handler functions to make the parsing easier
float convert_to_number(std::string str)
{
    //start at the back and remove once we hit first number
    int end_str = 0;
    for(int i = str.size() - 1; i >= 0; i--)
    {
        if(std::isdigit(str[i]))
        {
            end_str = i;
            break;
        }
    }
    if(end_str){
        return std::stof(str.substr(0,1+end_str));
    }
    //return -1 on an error because you will only have positive values anyway
    return -1;
}

//classic splitting of the 
std::vector<std::string> convert_to_list(std::string str, char limit)
{
    //list
    std::vector<std::string> l;
    std::string el;
    std::stringstream ss(str);
    while(std::getline(ss, el, limit))
    {
        //trim the strings
        el.erase(el.begin(), std::find_if(el.begin(), el.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
        el.erase(std::find_if(el.rbegin(), el.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), el.end());
        l.push_back(el);
    }
    return l;
}

void ShoeTree::add_on_attribute(std::string attribute)
{

}

ShoeTree::ShoeTree(std::string filename){
    //load all the shoe info from the data json file
    std::ifstream file(filename);
    if(!file.is_open()){
        //file is not found etc
        std::cerr << "File could not be opened";
        return;
    }

    nlohmann::json shoes_json;
    file >> shoes_json;

    for(const nlohmann::json &jshoe : shoes_json)
    {
        std::string name = jshoe["name"];
        std::string terrain = jshoe["Terrain:"];
        std::string arch_support = jshoe["Arch support:"];
        float heel_height = convert_to_number(jshoe["Heel height:"]);
        float forefoot_height = convert_to_number(jshoe["Forefoot height:"]);
        //Idk about adding this to the constructor it really doesn't seem relevant
        std::string collection = jshoe["Collection:"];
        //this is going to have a more indepth parsing setup
        //std::string weight = jshoe["Weight:"]; //"Men:  8.6 oz / 244g  | Women:  7.6 oz / 216g",
        float weight = 0;
        //float drop = convert_to_number(jshoe["Drop:"]);
        std::vector<std::string> pronation = convert_to_list(jshoe["Pronation:"], '|'); // "Neutral Pronation  |  Supination  |  Underpronation",
        std::string arch_type = jshoe["Arch type:"];
        std::vector<std::string> material = convert_to_list(jshoe["Material:"], '|'); // "Mesh  |  Reflective",
        std::vector<std::string> features = convert_to_list(jshoe["Features:"], '|'); // "Orthotic friendly  |  Cushioned  |  Carbon plate  |  Removable insole  |  Rocker",
        std::vector<std::string> strike_pattern = convert_to_list(jshoe["Strike Pattern:"], '|'); //"Heel strike  |  Forefoot/Midfoot strike",
        std::vector<std::string> season = convert_to_list(jshoe["Season:"], '|'); //"All seasons",
        std::string brand = jshoe["BRAND Brand:"];
        std::vector<std::string> type = convert_to_list(jshoe["Type:"], '|'); // "Maximalist  |  Lightweight",
        //"Widths available:": "Normal", this is too involved to figure out for the 1st iteration, maybe later
        std::vector<std::string> pace = convert_to_list(jshoe["Pace:"], ','); // "Tempo, Competition",
        std::vector<std::string> distance = convert_to_list(jshoe["Race distance:"], '|'); // "Marathon  |  Half marathon",
        // I dont think these are important at all so Im going to disregard them for now"SKUs:": "FN8454002 ,  FN8454104 ,  FN8454601 ,  FN8455001 ,  FN8455101 ,  FN8455102 ,  FN8455701 ,  HQ1718400 ,  HQ3498100 ,  HV4366072",
        float heel_stack = convert_to_number(jshoe["Heel stack"]);
        float forefoot_stack = convert_to_number(jshoe["Forefoot stack"]);
        float drop = convert_to_number(jshoe["Drop"]);
        float midsole_softness = convert_to_number(jshoe["Midsole softness"]);
        //I don't know if this field is shared by all the json file ones 
        float secondary_foam_softness = convert_to_number(jshoe["Secondary foam softness"]);
        float midsole_softness_in_cold = convert_to_number(jshoe["Midsole softness in cold"]);
        float midsole_softness_in_cold_per = convert_to_number(jshoe["Midsole softness in cold (%)"]);
        float insole_thickness = convert_to_number(jshoe["Insole thickness"]);
        std::string size = jshoe["Size"];
        float toebox_width_widest_prt = convert_to_number(jshoe["Toebox width - widest part (new method)"]);
        float toebox_width_big_toe = convert_to_number(jshoe["Toebox width - big toe (new method)"]);
        float toebox_height = jshoe["Toebox height"];
        int torsional_rigidity = (int)convert_to_number(jshoe["Torsional rigidity"]);
        int heel_counter_stiffness = (int)convert_to_number(jshoe["Heel counter stiffness"]);
        float midsole_width_forefoot = convert_to_number(jshoe["Midsole width - forefoot"]);
        float midesole_width_heel = convert_to_number(jshoe["Midsole width - heel"]);
        float flexibility = convert_to_number(jshoe["Flexibility / Stiffness (new method)"]);
        //"Flexibility / Stiffness (old method)": "35.5N",
        float stiffness_in_cold = convert_to_number(jshoe["Stiffness in cold"]);
        float stiffness_in_cold_per = convert_to_number(jshoe["Stiffness in cold (%)"]);
        // IDK how to do this yet "Weight": "8.75 oz (248g)",
        int breathability = (int)convert_to_number(jshoe["Breathability"]);
        int toebox_durability = (int)convert_to_number(jshoe["Toebox durability"]);
        int heel_padding_durability = (int)convert_to_number(jshoe["Heel padding durability"]);
        float outsole_hardness = convert_to_number(jshoe["Outsole hardness"]);
        float outsole_durability = convert_to_number(jshoe["Outsole durability"]);
        float outsole_thickness = convert_to_number(jshoe["Outsole thickness"]);
        //we need to create a customer one for this "Price": "$170",
        std::string price_str = jshoe["Price"];
        price_str.erase(0,1);
        float price = convert_to_number(price_str);
        bool reflective_elements = (jshoe["Reflective elements"] == "Yes") ? true : false;
        float tongue_padding = convert_to_number(jshoe["Tongue padding"]);
        // probaly disregard this as well "Tongue: gusset type": "Both sides (semi)",
        bool heel_tab = (jshoe["Heel tab"] == "None")? false : true;
        bool removable_insole = (jshoe["Removeable insole"] == "Yes") ? true : false;
        
        Shoe shoe = Shoe(name, heel_stack, forefoot_stack, 
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
        
        
    }


}