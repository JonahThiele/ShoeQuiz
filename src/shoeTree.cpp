#include "shoeTree.hpp"
#include "shoe.hpp"
#include <fstream>
#include <iostream>
#include "../includes/json.hpp"
#include <regex>

//helper function for the knn nearest neighbor searchs, get distance between two shoes based on metrics
float ShoeTree::distance_squared(std::array<float, ATTR> a, std::array<float, ATTR> b)
{
    float dist = 0;
    for(int i = 0; i < ATTR; i++)
    {
        float d = a[i] - b[i];
        dist += (d * d);
    }
    return dist;
}

void ShoeTree::kNearestRecursive(std::shared_ptr<Shoe> node, std::shared_ptr<Shoe> perfect_shoe,
    int depth, int k, std::priority_queue<std::pair<float, std::shared_ptr<Shoe>>> &max_heap)
{
    //check if node is not leaf
    if(!node)
        return;

    //get distance from the shoe we want and the shoe in the tree
    float dist = distance_squared(perfect_shoe->points, node->points);

    //if we can have k suggestions just add to heap
    if(max_heap.size() < static_cast<size_t>(k))
    {
        max_heap.emplace(dist, node);
    //if we have max suggestions remove the frist one and add it instead
    } else if (dist < max_heap.top().first)
    {
        max_heap.pop();
        max_heap.emplace(dist, node);
    }

    int cd = depth % ATTR;
    //check if the right or left branch is closer 
    bool goLeft = perfect_shoe->points[cd] < node->points[cd];
    auto next = goLeft ? node->left : node->right;
    auto other = goLeft ? node->right : node->left;

    //continue with the branch chosen
    kNearestRecursive(next, perfect_shoe, depth + 1, k, max_heap);

    //check other side is actually closer later on
    float axis_diff = perfect_shoe->points[cd] - node->points[cd];
    if(max_heap.size() < static_cast<size_t>(k) || axis_diff * axis_diff < max_heap.top().first)
    {   
        //continue down that path if it is
        kNearestRecursive(other, perfect_shoe, depth + 1, k, max_heap);
    }
}

//store it in a vector and reverse it so it has the closest matches first
std::vector<std::shared_ptr<Shoe>> ShoeTree::kNearestNeighbors(std::shared_ptr<Shoe> perfect_shoe, int k)
{
    std::priority_queue<std::pair<float, std::shared_ptr<Shoe>>> max_heap;
    kNearestRecursive(root, perfect_shoe, 0, k, max_heap);

    std::vector<std::shared_ptr<Shoe>> result;
    while(!max_heap.empty())
    {
        result.push_back(max_heap.top().second);
        max_heap.pop();
    }
    //closeest shoes are first
    std::reverse(result.begin(), result.end());
    return result;
}

//use this to build the tree in constructor
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
void ShoeTree::printRecursive(std::shared_ptr<Shoe> node, int depth)
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
            printRecursive(node->right, depth +1);
        }

ShoeTree::ShoeTree(std::string filename){
    //load all the shoe info from the data json file
    std::ifstream file(filename);
    if(!file.is_open()){
        //file is not found etc
        std::cerr << "File could not be opened";
        return;
    }

    //load the json/hashmap structure
    nlohmann::json shoes_json;
    file >> shoes_json;

    for(const nlohmann::json &jshoe : shoes_json)
    {
        //convert all the json map variables into something that can be places into a shoe 
        std::string name = jshoe["name"];
        bool terrain = (jshoe.value("Terrain:", "None") == "Road")? true : false;
        bool arch_support = (jshoe.value("Arch support:", "None") == "Neutral")? true : false;
        float heel_height = Utility::convert_to_number(jshoe.value("Heel height:", "None"));
        float forefoot_height = Utility::convert_to_number(jshoe.value("Forefoot height:", "None"));
        bool arch_type = (jshoe.value("Arch type:", "None") == "High arch")? true : false;
        std::string brand = jshoe.value("BRAND Brand:", "None");
        float heel_stack = Utility::convert_to_number(jshoe.value("Heel stack", "None"));
        float forefoot_stack = Utility::convert_to_number(jshoe.value("Forefoot stack", "None"));
        float drop = Utility::convert_to_number(jshoe.value("Drop", "None"));
        float midsole_softness = Utility::convert_to_number(jshoe.value("Midsole softness", "None"));
        float secondary_foam_softness = Utility::convert_to_number(jshoe.value("Secondary foam softness", "None"));
        float midsole_softness_in_cold = Utility::convert_to_number(jshoe.value("Midsole softness in cold", "None"));
        float midsole_softness_in_cold_per = Utility::convert_to_number(jshoe.value("Midsole softness in cold (%)", "None"));
        float insole_thickness = Utility::convert_to_number(jshoe.value("Insole thickness", "None"));
        std::string size = jshoe.value("Size", "None");
        float toebox_width_widest_prt = Utility::convert_to_number(jshoe.value("Toebox width - widest part (new method)", "None"));
        float toebox_width_big_toe = Utility::convert_to_number(jshoe.value("Toebox width - big toe (new method)", "None"));
        float toebox_height = Utility::convert_to_number(jshoe.value("Toebox height", "None"));
        int torsional_rigidity = (int)Utility::convert_to_number(jshoe.value("Torsional rigidity", "None"));
        int heel_counter_stiffness = (int)Utility::convert_to_number(jshoe.value("Heel counter stiffness", "None"));
        float midsole_width_forefoot = Utility::convert_to_number(jshoe.value("Midsole width - forefoot", "None"));
        float midesole_width_heel = Utility::convert_to_number(jshoe.value("Midsole width - heel", "None"));
        float flexibility = Utility::convert_to_number(jshoe.value("Flexibility / Stiffness (old method)", "None"));
        float stiffness_in_cold = Utility::convert_to_number(jshoe.value("Stiffness in cold", "None"));
        float stiffness_in_cold_per = Utility::convert_to_number(jshoe.value("Stiffness in cold (%)", "None"));
        float weight = Utility::parseWeight(jshoe.value("Weight", "None"));
        int breathability = (int)Utility::convert_to_number(jshoe.value("Breathability", "None"));
        int toebox_durability = (int)Utility::convert_to_number(jshoe.value("Toebox durability", "None"));
        int heel_padding_durability = (int)Utility::convert_to_number(jshoe.value("Heel padding durability", "None"));
        float outsole_hardness = Utility::convert_to_number(jshoe.value("Outsole hardness", "None"));
        float outsole_durability = Utility::convert_to_number(jshoe.value("Outsole durability", "None"));
        float outsole_thickness = Utility::convert_to_number(jshoe.value("Outsole thickness", "None"));
        //handle converting the price into a number
        std::string price_str = jshoe.value("Price", "None");
        price_str.erase(0,1);
        float price = Utility::convert_to_number(price_str);
        bool reflective_elements = (jshoe.value("Reflective elements", "None") == "Yes") ? true : false;
        float tongue_padding = Utility::convert_to_number(jshoe.value("Tongue padding", "None"));
        bool heel_tab = (jshoe.value("Heel tab", "None") == "None")? false : true;
        bool removable_insole = (jshoe.value("Removeable insole", "None") == "Yes") ? true : false;
        
        //create the shoe object from the json values
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
            removable_insole, terrain, arch_support,
            heel_height, forefoot_height, arch_type, brand,
            drop, secondary_foam_softness, heel_padding_durability, outsole_durability);
        
        //add the shoe to the tree
        root = insertRecursive(root, std::make_shared<Shoe>(shoe), 0);
    }
}