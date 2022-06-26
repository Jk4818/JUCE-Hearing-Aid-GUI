
#pragma once

#include <JuceHeader.h>
#include <math.h>

class HelperFunctions : public juce::Component
{
public:

    ///HELPER FUNCTION FOR CONVERTING NEGATIVE DEGREE VALUES TO BETWEEN [0, 360]
    int convertPositiveDegree(int value) {
        if (value < 0) value = (value % 360) + 360;
        return value;
    }



    ////HELPER FUNCTIONS FOR VALID IP
    //==============================================================================

    // check if a given string is a numeric string or not
    bool isNumber(const std::string& str)
    {
        // `std::find_first_not_of` searches the string for the first character
        // that does not match any of the characters specified in its arguments
        return !str.empty() &&
            (str.find_first_not_of("[0123456789]") == std::string::npos);
    }

    // Function to split string `str` using a given delimiter
    std::vector<std::string> split(const std::string& str, char delim)
    {
        auto i = 0;
        std::vector<std::string> list;

        auto pos = str.find(delim);

        while (pos != std::string::npos)
        {
            list.push_back(str.substr(i, pos - i));
            i = ++pos;
            pos = str.find(delim, pos);
        }

        list.push_back(str.substr(i, str.length()));

        return list;
    }

    // Function to validate an IP address
    bool validateIP(std::string ip)
    {
        // split the string into tokens
        std::vector<std::string> list = split(ip, '.');

        // if the token size is not equal to four
        if (list.size() != 4) {
            return false;
        }

        // validate each token
        for (std::string str : list)
        {
            // verify that the string is a number or not, and the numbers
            // are in the valid range
            if (!isNumber(str) || stoi(str) > 255 || stoi(str) < 0) {
                return false;
            }
        }

        return true;
    }


private:


};
