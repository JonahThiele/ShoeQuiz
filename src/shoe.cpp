#include <string>
#include <map>

//these were the only size 
enum Size {
    TRUE_TO_SIZE,
    SLIGHTLY_SMALL,
    SLIGHTLY_LARGE,
    HALF_SIZE_SMALL,
    HALF_SIZE_LARGE
};

std::map<std::string, Size> SizeLabels = {
    {"True to size", TRUE_TO_SIZE},
    {"Slightly small", SLIGHTLY_SMALL},
    {"Slightly large", SLIGHTLY_LARGE},
    {"Half size small", HALF_SIZE_SMALL},
    {"Half size large", HALF_SIZE_LARGE}
};

//this is mainly a data class to hold the shoe attributes for search
class Shoe {
    private:
        std::string name;
        float heel_stack;
        float forefoot_stack;
        //both of these in HA units
        //this is shore hardness scale using Shore A for both of these I think
        float midsole_softness;
        float midsole_softness_cold;
        //percent increase in stiffness
        float midsole_softness_cold_per;
        //in milimeters
        float insole_thickness;
        //what is the fit compare to other sizes
        enum Size size;
        //both of these in millimeters
        float toebox_width_widest_prt;
        float toebox_width_big_toe;
        float toebox_height;
        //both of these in subjective 1-5 rating
        int torsional_rigidity;
        int heel_counter_stiffness;
        //both of these in millimeters
        float midsole_width_forefoot;
        float midesole_width_heel;
        //in newtons of force
        float flexibility;
        //in newtons
        float stiffness_in_cold;
        //percent incrase in stiffness when cold
        float stiffness_in_cold_per;
        //in oz
        float weight;
        //both subjective scale 1-5 
        int breathability;
        int toebox_durability;
        //more shore hardness scale using Shore C
        float outsole_hardness;
        //in milimeters
        float outsole_thickness;
        //forget the change to make is easier for now
        int price;
        bool reflective_elements;
        //in millimeters of padding
        float tongue_padding;
        //I might add the gusset type padding later, idk
        bool heel_tab;
        bool removable_insole;
    public:
        Shoe(std::string name, float heel_stack, float forefoot_stack, 
            float midsole_softness, float midsole_softness_cold, 
            float midsole_softness_cold_per, float insole_thickness, 
            std::string size, float toebox_width_widest_prt, 
            float toebox_width_big_toe, float toebox_height, int torsional_rigidity,
            int heel_counter_stiffness, float midsole_width_forefoot,
            float midesole_width_heel, float flexibility,
            float stiffness_in_cold, float stiffness_in_cold_per,
            float weight,int breathability,int toebox_durability,
            float outsole_hardness,float outsole_thickness,
            int price, bool reflective_elements,
            float tongue_padding, bool heel_tab,
            bool removable_insole )
            {

                this->name = name;
                this->heel_stack = heel_stack;
                this->forefoot_stack = forefoot_stack;
                this->midsole_softness = midsole_softness;
                this->midsole_softness_cold = midsole_softness_cold;
                this->midsole_softness_cold_per = midsole_softness_cold_per;
                this->insole_thickness = insole_thickness;
                this->size = SizeLabels[size];
                this->toebox_width_widest_prt = toebox_width_widest_prt;
                this->toebox_width_big_toe = toebox_width_big_toe;
                this->toebox_height = toebox_height;
                this->torsional_rigidity = torsional_rigidity;
                this->heel_counter_stiffness = heel_counter_stiffness;
                this->midsole_width_forefoot = midsole_width_forefoot;
                this->midesole_width_heel = midesole_width_heel;
                this->flexibility = flexibility;
                this->stiffness_in_cold = stiffness_in_cold;
                this->stiffness_in_cold_per = stiffness_in_cold_per;
                this->weight = weight;
                this->breathability = breathability;
                this->toebox_durability = toebox_durability;
                this->outsole_hardness = outsole_hardness;
                this->outsole_thickness = outsole_thickness;
                this->price = price;
                this->reflective_elements = reflective_elements;
                this->tongue_padding = tongue_padding;
                this->heel_tab = heel_tab;
                this->removable_insole = removable_insole;
            }
};