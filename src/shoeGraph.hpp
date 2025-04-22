#include <map>
#include <vector>
#include <string>
#include "shoe.hpp"

class ShoeGraph {
    public:
        //pointers to hopefully minimize the overhead of all this
        std::map<std::shared_ptr<Shoe>, std::vector<std::shared_ptr<Shoe>>> adjacencyList;
        ShoeGraph(std::string);
        //methods to run the comparison code on to get results
};