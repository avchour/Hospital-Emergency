
#include "../include/Utils.h"

#include <iostream>
#include <limits>
#include <algorithm>
#include <cctype>
#include <ctime>

namespace Utils
{
    void clearInputBuffer()
    {
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n');
    }

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
}
