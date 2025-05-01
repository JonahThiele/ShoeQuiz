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
        //utility function for KD-nearest neighbors
        float distance_squared(std::array<float, ATTR>, std::array<float, ATTR>);
        
        std::shared_ptr<Shoe> root = nullptr; //start off with null ptr

        //standard functions of a kd tree
        std::shared_ptr<Shoe>  insertRecursive(std::shared_ptr<Shoe>, std::shared_ptr<Shoe>, int);
        
        //search for the closest point on the kd tree to the shoe we created
        void kNearestRecursive(std::shared_ptr<Shoe>, std::shared_ptr<Shoe>, int, int, 
            std::priority_queue<std::pair<float, std::shared_ptr<Shoe>>>&);

        //print out the tree for debugging purposes
        void printRecursive(std::shared_ptr<Shoe>, int);


    public:
        //simple constuctor that takes the data file string
        ShoeTree(std::string);
        //public functions that run the recursive functions
        void print() {
            printRecursive(root, 0);
        }
        std::vector<std::shared_ptr<Shoe>> kNearestNeighbors(std::shared_ptr<Shoe>, int);
};