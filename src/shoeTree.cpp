#include "shoeTree.hpp"
#include "shoe.hpp"
#include <fstream>
#include <iostream>
#include "../includes/json.hpp"

//some small data handler functions to make the parsing easier
float ShoeTree::convert_to_number(std::string str)
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
std::vector<std::string> ShoeTree::convert_to_list(std::string str, char limit)
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

std::shared_ptr<Shoe> ShoeTree::insertRecursive(std::shared_ptr<Shoe> node, std::shared_ptr<Shoe> shoe, int depth)
{
            //its  in a new spot that is can be added to
            if (node == nullptr){
                return shoe; 
            }

            //which dimension to use rn
            int cd = depth % ATTR;

            if(shoe->points[cd] < node->points[cd])
            {
                node->left = insertRecursive(node->left,  shoe, depth +1);
            }
            else {
                node->right = insertRecursive(node->right, shoe, depth + 1);
            }

            return node;
}

//purely for debugging
void printRecursive(std::shared_ptr<Shoe> node, int depth)
        {
            if(node == nullptr)
                return;

            for(int i = 0; i < depth; i++)
            {
                std::cout << "  ";
            }
            std::cout << "(";
            for(int i = 0; i < ATTR; i++)
            {
                std::cout << node->points[i];
                if(i < ATTR -1) 
                {
                    std::cout << ", ";
                }
            }
            std::cout << ")\n";

            printRecursive(node->left, depth +1);
            printRecursive(node->left, depth +1);
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
        //using default values for the company stats because they seem to have variable ones
        std::string terrain = jshoe.value("Terrain:", "None");
        std::string arch_support = jshoe.value("Arch support:", "None");
        float heel_height = convert_to_number(jshoe.value("Heel height:", "None"));
        float forefoot_height = convert_to_number(jshoe.value("Forefoot height:", "None"));
        //Idk about adding this to the constructor it really doesn't seem relevant
        std::string collection = jshoe.value("Collection:", "None");
        //this is going to have a more indepth parsing setup
        //std::string weight = jshoe["Weight:"]; //"Men:  8.6 oz / 244g  | Women:  7.6 oz / 216g",
        float weight = 0;
        //float drop = convert_to_number(jshoe["Drop:"]);
        std::vector<std::string> pronation = convert_to_list(jshoe.value("Pronation:", "None"), '|'); // "Neutral Pronation  |  Supination  |  Underpronation",
        std::string arch_type = jshoe.value("Arch type:", "None");
        std::vector<std::string> material = convert_to_list(jshoe.value("Material:", "None"), '|'); // "Mesh  |  Reflective",
        std::vector<std::string> features = convert_to_list(jshoe.value("Features:", "None"), '|'); // "Orthotic friendly  |  Cushioned  |  Carbon plate  |  Removable insole  |  Rocker",
        std::vector<std::string> strike_pattern = convert_to_list(jshoe.value("Strike Pattern:", "None"), '|'); //"Heel strike  |  Forefoot/Midfoot strike",
        std::vector<std::string> season = convert_to_list(jshoe.value("Season:", "None"), '|'); //"All seasons",
        std::string brand = jshoe.value("BRAND Brand:", "None");
        std::vector<std::string> type = convert_to_list(jshoe.value("Type:", "None"), '|'); // "Maximalist  |  Lightweight",
        //"Widths available:": "Normal", this is too involved to figure out for the 1st iteration, maybe later
        std::vector<std::string> pace = convert_to_list(jshoe.value("Pace:", "None"), ','); // "Tempo, Competition",
        std::vector<std::string> distance = convert_to_list(jshoe.value("Race distance:", "None"), '|'); // "Marathon  |  Half marathon",
        // I dont think these are important at all so Im going to disregard them for now"SKUs:": "FN8454002 ,  FN8454104 ,  FN8454601 ,  FN8455001 ,  FN8455101 ,  FN8455102 ,  FN8455701 ,  HQ1718400 ,  HQ3498100 ,  HV4366072",
        float heel_stack = convert_to_number(jshoe.value("Heel stack", "None"));
        float forefoot_stack = convert_to_number(jshoe.value("Forefoot stack", "None"));
        float drop = convert_to_number(jshoe.value("Drop", "None"));
        float midsole_softness = convert_to_number(jshoe.value("Midsole softness", "None"));
        //I don't know if this field is shared by all the json file ones 
        float secondary_foam_softness = convert_to_number(jshoe.value("Secondary foam softness", "None"));
        float midsole_softness_in_cold = convert_to_number(jshoe.value("Midsole softness in cold", "None"));
        float midsole_softness_in_cold_per = convert_to_number(jshoe.value("Midsole softness in cold (%)", "None"));
        float insole_thickness = convert_to_number(jshoe.value("Insole thickness", "None"));
        std::string size = jshoe.value("Size", "None");
        float toebox_width_widest_prt = convert_to_number(jshoe.value("Toebox width - widest part (new method)", "None"));
        float toebox_width_big_toe = convert_to_number(jshoe.value("Toebox width - big toe (new method)", "None"));
        float toebox_height = convert_to_number(jshoe.value("Toebox height", "None"));
        int torsional_rigidity = (int)convert_to_number(jshoe.value("Torsional rigidity", "None"));
        int heel_counter_stiffness = (int)convert_to_number(jshoe.value("Heel counter stiffness", "None"));
        float midsole_width_forefoot = convert_to_number(jshoe.value("Midsole width - forefoot", "None"));
        float midesole_width_heel = convert_to_number(jshoe.value("Midsole width - heel", "None"));
        float flexibility = convert_to_number(jshoe.value("Flexibility / Stiffness (new method)", "None"));
        //"Flexibility / Stiffness (old method)": "35.5N",
        float stiffness_in_cold = convert_to_number(jshoe.value("Stiffness in cold", "None"));
        float stiffness_in_cold_per = convert_to_number(jshoe.value("Stiffness in cold (%)", "None"));
        // IDK how to do this yet "Weight": "8.75 oz (248g)",
        int breathability = (int)convert_to_number(jshoe.value("Breathability", "None"));
        int toebox_durability = (int)convert_to_number(jshoe.value("Toebox durability", "None"));
        int heel_padding_durability = (int)convert_to_number(jshoe.value("Heel padding durability", "None"));
        float outsole_hardness = convert_to_number(jshoe.value("Outsole hardness", "None"));
        float outsole_durability = convert_to_number(jshoe.value("Outsole durability", "None"));
        float outsole_thickness = convert_to_number(jshoe.value("Outsole thickness", "None"));
        //we need to create a customer one for this "Price": "$170",
        std::string price_str = jshoe.value("Price", "None");
        price_str.erase(0,1);
        float price = convert_to_number(price_str);
        bool reflective_elements = (jshoe.value("Reflective elements", "None") == "Yes") ? true : false;
        float tongue_padding = convert_to_number(jshoe.value("Tongue padding", "None"));
        // probaly disregard this as well "Tongue: gusset type": "Both sides (semi)",
        bool heel_tab = (jshoe.value("Heel tab", "None") == "None")? false : true;
        bool removable_insole = (jshoe.value("Removeable insole", "None") == "Yes") ? true : false;
        
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
        
        //add the shoe to the tree
        root = insertRecursive(root, std::make_shared<Shoe>(shoe), 0);
    
    }


}