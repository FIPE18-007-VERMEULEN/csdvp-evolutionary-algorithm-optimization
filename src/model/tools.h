#ifndef SRC_MODEL_TOOLS_H_
#define SRC_MODEL_TOOLS_H

#include <vector>
#include <utility>
#include <algorithm>

template<typename T>
/** Searches into vec the element findMe. The class T must have T() defined ( T()=default; is OK) */
static std::pair<int, T> findInVector(std::vector<T> & vec, const T & findMe)
{
    std::pair<int, T> res;

    typename std::vector<T>::iterator it = std::find( vec.begin(), vec.end(), findMe);
    if(it == vec.end())
    {
        res.first = -1;
        res.second; //NTD, -1 SHOULD BE USED TO DETECT THAT NOTHING HAS BEEN FOUND
    }
    else
    {
        res.first = std::distance(vec.begin(), it);
        res.second = vec.at(res.first);
    }

    return res;
}

template<typename T>
/** duplicataFlag returns true if the value (2nd param) searched into (1st param) is found */
static bool duplicataFlag(std::vector<T> & toProtect, const T & toAdd)
{
    std::pair<int, T> res = findInVector(toProtect, toAdd);
    if(res.first < 0)
        return false;
    else
        return true;

}

#endif // SRC_MODEL_TOOLS_H_