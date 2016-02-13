#pragma once

#include "bencode/type/element.h"

/*allows you to find necessary item in the bencode::map
 *using the string as comparison
 */

namespace bencode{
    class string_comparator : public std::unary_function<std::string, bool>{
        private:
            std::string _baseline;

        public:
            string_comparator(const std::string &baseline) : _baseline(baseline){}

            bool operator()(std::pair<bencode::element*, bencode::element*> value){
                bencode::element* key = value.first;

                try{
                    std::vector<char> first_flow = bencode::type::string::decode(key);
                    std::string first_value(first_flow.begin(), first_flow.end());

                    return first_value.compare(_baseline) == 0;

                }catch(std::exception e){
                    return false;
                }
            }
    };
}
