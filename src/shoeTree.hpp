#include <map>
#include <vector>
#include <string>
#include "shoe.hpp"
#include <memory>

class ShoeTree {
    private:
        //utility functions
        float convert_to_number(std::string str);
        std::vector<std::string> convert_to_list(std::string str, char limit);
        //pairing attributes with the different shoes
        std::shared_ptr<Shoe> root = nullptr; //start off with null ptr
        //methods to run the comparison code on to get results
        //standard functions of a kd tree that we are modifying for this system
        std::shared_ptr<Shoe>  insertRecursive(std::shared_ptr<Shoe>, std::shared_ptr<Shoe>, int);
        //search for the closest point on the kd tree 

        //print out the tree for debugging purposes early on 
        void printRecursive(std::shared_ptr<Shoe>, int);


    public:
        ShoeTree(std::string);
};