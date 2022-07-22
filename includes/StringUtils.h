#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include "SystemFiles.h"


namespace StringUtils {


// Trims whitespace from start of the string
inline string trimHead(string str)
{
    string::size_type trimLength = 0;
    for (string::size_type i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ' || str[i] == '\t')
        {
            trimLength++;
        }
        else break;
    }
    return str.erase(0, trimLength);
}


// Trims whitespace from end of the string
inline string trimTail(string str)
{
    while (str.rfind(" ") == str.length() - 1 || str.rfind("\t") == str.length() - 1)
    {
        if (str.length() == 1)
        {
            str.erase(0);
            break;
        }
        str.erase(str.length() - 1);
    }
    return str;
}


// Trims whitespace from start and end of the string
inline string trim(string str)
{
    str = trimHead(str);
    return trimTail(str);
}


// Split string based on separator
inline vector<string> split(string str, string separator)
{
    vector<string> fields;
    size_t pos = 0;
    string token;
    while ((pos = str.find(separator)) != string::npos)
    {
        token = str.substr(0, pos);
        fields.push_back(token);
        str.erase(0, pos + separator.length());
    }
    fields.push_back(str);
    return fields;
}


// Split string based on whitespace
inline vector<string> splitFields(string str)
{
    vector<string> fields;
    string token;
    stringstream ss(str);
    while (ss >> token)
        fields.push_back(token);
    return fields;
}


// Convert string to int
inline int toInt(string str)
{
    return std::stoi(str);
}

}

#endif // STRINGUTILS_H
