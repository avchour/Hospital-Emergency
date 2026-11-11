
#include "../include/utils.h"

#include <algorithm>
#include <cctype>
#include <ctime>
#include <climits>

namespace Utils
{

    std::string getCurrentDateTime()
    {
        std::time_t now = std::time(nullptr);
        std::tm *localTime = std::localtime(&now);

        char buffer[30];

        std::strftime(
            buffer,
            sizeof(buffer),
            "%Y-%m-%d %H:%M:%S",
            localTime);

        return buffer;
    }

    bool isValidPhone(const std::string &phone)
    {
        if (phone.empty())
            return false;

        for (char c : phone)
        {
            if (!std::isdigit(static_cast<unsigned char>(c)) && c != '+')
                return false;
        }

        return true;
    }

    std::string toLowerCase(std::string text)
    {
        std::transform(
            text.begin(),
            text.end(),
            text.begin(),
            [](unsigned char c)
            {
                return std::tolower(c);
            });

        return text;
    }
    bool isValidNonNegativeInteger(const std::string &value)
    {
        if (value.empty())
            return false;

        int number = 0;

        for (char c : value)
        {
            if (c < '0' || c > '9')
                return false;

            int digit = c - '0';

            // Prevent integer overflow
            if (number > (INT_MAX - digit) / 10)
                return false;

            number = number * 10 + digit;
        }

        return true;
    }
}
