
#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils
{
    void clearInputBuffer();

    std::string getCurrentDateTime();

    bool isValidPhone(const std::string &phone);

    std::string toLowerCase(std::string text);

    bool isValidNonNegativeInteger(const std::string &value);
}

#endif
