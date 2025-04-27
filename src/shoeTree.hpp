#include <map>
#include <vector>
#include <string>
#include "shoe.hpp"
#include "utility.hpp"
#include <memory>
#include <queue>
#include <array>

class ShoeTree {
    private:
        //utility function
        float distance_squared(std::array<float, ATTR>, std::array<float, ATTR>);
        //pairing attributes with the different shoes
        std::shared_ptr<Shoe> root = nullptr; //start off with null ptr
        //methods to run the comparison code on to get results
        //standard functions of a kd tree that we are modifying for this system
        std::shared_ptr<Shoe>  insertRecursive(std::shared_ptr<Shoe>, std::shared_ptr<Shoe>, int);
        //search for the closest point on the kd tree 
        void kNearestRecursive(std::shared_ptr<Shoe>, std::shared_ptr<Shoe>, int, int, 
            std::priority_queue<std::pair<float, std::shared_ptr<Shoe>>>);

        //print out the tree for debugging purposes early on 
        void printRecursive(std::shared_ptr<Shoe>, int);


    public:
        ShoeTree(std::string);
        void print() {
            printRecursive(root, 0);
        }
        std::vector<std::shared_ptr<Shoe>> kNearestNeighbors(std::shared_ptr<Shoe>, int);
};