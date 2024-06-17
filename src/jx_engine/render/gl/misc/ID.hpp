#ifndef ID_CLASS_HPP
#define ID_CLASS_HPP

#include<string>
#include <string>
#include <functional> // For std::hash
#include <sstream>

class ID {
    public:
    static std::string createHashedID(const std::string& input) {
        // Use std::hash to hash the input string
        std::hash<std::string> hasher;
        auto hashedValue = hasher(input);

        // Convert the hashed value to a hexadecimal string
        std::stringstream ss;
        ss << std::hex << hashedValue;
        return ss.str();
    }
        std::string hashID;

};


#endif 