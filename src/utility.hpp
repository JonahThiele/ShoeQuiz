#pragma once
#include <sstream>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>

class Utility {
    public:
        static float convert_to_number(std::string str)
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
        static std::vector<std::string> convert_to_list(std::string str, char limit)
        {
            //list
            std::vector<std::string> l;
            std::string el;
            std::stringstream ss(str);
            while(std::getline(ss, el, limit))
            {
                //trim the strings
                el.erase(el.begin(), std::find_if(el.begin(), el.end(), [](char c) {
                    return c != ' ';
                }));
                el.erase(std::find_if(el.rbegin(), el.rend(), [](char c) {
                    return c != ' ';
                }).base(), el.end());                
                l.push_back(el);
            }
            return l;
        }

        static float parseWeight(std::string str)
        {
            std::regex pattern("R(\\d+(?:\\.\\d+)?)");
            std::smatch match;
            if(std::regex_search(str, match, pattern))
            {
                return std::stof(match[1]);
            }

            //error
            return -1;
        }
};
