#include <map>
#include <vector>
#include <string>
#include "shoe.hpp"
#include <memory>

class ShoeGraph {
    public:
        //pointers to hopefully minimize the overhead of all this
        //pairing attributes with the different shoes
        std::map<std::shared_ptr<Shoe>, std::vector<std::pair<std::string , std::shared_ptr<Shoe>>>> adjacencyList;
        ShoeGraph(std::string);
        //methods to run the comparison code on to get results
    private:
        void add_on_attribute(std::string);
};