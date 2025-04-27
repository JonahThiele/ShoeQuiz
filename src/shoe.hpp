#pragma once
#include <string>
#include <map>
#include <array>
#include <memory>
#define ATTR 27

//these were the only sizes I saw after briefly skimming the file 
enum Size {
    TRUE_TO_SIZE,
    SLIGHTLY_SMALL,
    SLIGHTLY_LARGE,
    HALF_SIZE_SMALL,
    HALF_SIZE_LARGE
};

 extern std::map<std::string, Size> SizeLabels;

//this is functionaly a node for the KD tree 
class Shoe {
        public:
            std::string name;
            float price;
            std::array<float , ATTR> points;
            std::shared_ptr<Shoe> right;
            std::shared_ptr<Shoe> left;

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
            }
};