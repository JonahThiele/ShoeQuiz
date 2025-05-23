#pragma once
#include <string>
#include <map>
#include <array>
#include <memory>
#define ATTR 37

//convert the different options into a integer number
enum Size {
    TRUE_TO_SIZE,
    SLIGHTLY_SMALL,
    SLIGHTLY_LARGE,
    HALF_SIZE_SMALL,
    HALF_SIZE_LARGE
};

//convert the brands into a different integer number
enum Brand {
    ADIDAS,
    ALLBIRDS,
    ALTRA,
    APL,
    ASICS,
    BROOKS,
    HOKA,
    INOV8,
    JORDAN,
    KAILAS,
    LA_SPORTIVA,
    MERRELL,
    MIZUNO,
    NEW_BALANCE,
    NIKE,
    NNORMAL,
    NOBULL,
    ON,
    PUMA,
    REEBOK,
    SALOMON,
    SAUCONY,
    SCARPA,
    SKETCHERS,
    THE_NORTH_FACE,
    TOPO_ATHLETIC,
    UNDER_ARMOR,
    XERO_SHOES
};
//use these to map the string of the json file to the enum representation
extern std::map<std::string, Brand> BrandLabels;
extern std::map<std::string, Size> SizeLabels;

//this is functionaly a node for the KD tree, purely a data class
class Shoe {
        public:
            std::string name;
            float price;
            std::array<float , ATTR> points;
            std::shared_ptr<Shoe> right;
            std::shared_ptr<Shoe> left;
        //these are data points we are doing the KD-tree search on
        // 1. double  heel_stack;
        // 2. double forefoot_stack;
        // //both of these in HA units
        // //this is shore hardness scale using Shore A for both of these I think
        // 3. double midsole_softness;
        // 4. double midsole_softness_cold;
        // //percent increase in stiffness
        // 5. double midsole_softness_cold_per;
        // //in milimeters
        // 6. double insole_thickness;
        // //what is the fit compare to other sizes
        // 7. enum Size size;
        // //both of these in millimeters
        // 8. double toebox_width_widest_prt;
        // 9. double toebox_width_big_toe;
        // 10. double toebox_height;
        // //both of these in subjective 1-5 rating
        // 11. double torsional_rigidity;
        // 12. double heel_counter_stiffness;
        // //both of these in millimeters
        // 13. double midsole_width_forefoot;
        // 14. double midesole_width_heel;
        // //in newtons of force
        // 15. double flexibility;
        // //in newtons
        // 16. double stiffness_in_cold;
        // //percent incrase in stiffness when cold
        // 17. double stiffness_in_cold_per;
        // //in oz
        // 18. double weight;
        // //both subjective scale 1-5 
        // 19. double breathability;
        // 20. double toebox_durability;
        // //more shore hardness scale using Shore C
        // 21. double outsole_hardness;
        // //in milimeters
        // 22. double outsole_thickness;
        // //forget the change to make is easier for now
        // 23. double price;
        // 24. double reflective_elements;
        // //in millimeters of padding
        // 25. float tongue_padding;
        // //I might add the gusset type padding later, idk
        // 26. double heel_tab;
        // 27. double removable_insole;
        // 28. int terrain; //0 road or 1 trail
        // 29. int Arch_support; // 0 Neutral or 1 Stability 
        // 30. float heel_height;
        // 31. float forefoot_height;
        // 32. int arch_type; // 0 high arch or 1 low arch
        // 33. int brand;
        // 34. float drop;
        // 35. float secondary_foam_softness;
        // 36. int heel_padding_durability;
        // 37. int outsole_durability; 
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
            bool removable_insole, bool road_terrain, bool arch_stablility_support,
            float heel_height, float forefoot_height, bool high_arch_type, std::string brand,
            float drop, float secondary_foam_softness, float heel_padding_durability,
            float outsole_durability)
            {

                //load in the attributes into the points array to use for the KD-tree algorithm
                this->name = name;
                points[0] = heel_stack;
                points[1] = forefoot_stack;
                points[2] = midsole_softness;
                points[3] = midsole_softness_cold;
                points[4] = midsole_softness_cold_per;
                points[5] = insole_thickness;
                points[6] = SizeLabels[size];
                points[7] = toebox_width_widest_prt;
                points[8] = toebox_width_big_toe;
                points[9] = toebox_height;
                points[10] = torsional_rigidity;
                points[11] = heel_counter_stiffness;
                points[12] = midsole_width_forefoot;
                points[13] = midesole_width_heel;
                points[14] = flexibility;
                points[15] = stiffness_in_cold;
                points[16] = stiffness_in_cold_per;
                points[17] = weight;
                points[18] = breathability;
                points[19] = toebox_durability;
                points[20] = outsole_hardness;
                points[21] = outsole_thickness;
                points[22] = price;
                this->price = price;
                points[23] = (reflective_elements)? 1: 0;
                points[24] = tongue_padding;
                points[25] = (heel_tab)? 1: 0;
                points[26] = (removable_insole)? 1 : 0;
                points[27] = (road_terrain)? 0 : 1;
                points[28] = (arch_stablility_support)? 1: 0;
                points[29] = heel_height;
                points[30] = forefoot_height;
                points[31] = (high_arch_type)? 0 : 1;
                points[32] = BrandLabels[brand];
                points[33] = drop;
                points[34] = secondary_foam_softness;
                points[35] = heel_padding_durability;
                points[36] = outsole_durability;
            }
};