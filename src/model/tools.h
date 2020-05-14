#ifndef SRC_MODEL_TOOLS_H_
#define SRC_MODEL_TOOLS_H

#include <vector>
#include <utility>
#include <algorithm>
#include <queue>
#include <random>

template<typename T>
/** Searches into vec the element findMe. The class T must have T() defined ( T()=default; is OK) */
static std::pair<int, T> findInVector(std::vector<T> & vec, T & findMe)
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
static bool duplicataFlag(std::vector<T> & toProtect, T & toAdd)
{
    std::pair<int, T> res = findInVector(toProtect, toAdd);
    if(res.first < 0)
        return false;
    else
        return true;

}

template<typename T>
static std::pair<bool,T> getElementById(const std::vector<T> & elem, int idToFind)
{
    for(int i = 0; i < elem.size(); i++)
    {
        if(elem.at(i).id() == idToFind)
            return std::pair<bool,T>(true, elem.at(i));
    }
    return std::pair<bool, T>(false, T());
}

///Not working properly yet, just for debug
// static std::queue<int> RNG_QUEUE;
// /**Init a queue of size s from 0 to s, then shuffle it*/
// static void initRandomSuite(const unsigned int s)
// {
//     RNG_QUEUE = std::queue<int>();
//     std::vector<int> tmp (s);
//     for(int i = 0; i < s; i++)
//         tmp.push_back(i);
//     // std::random_device rng;
//     // std::mt19937 urng(rng());
//     std::random_shuffle(tmp.begin(), tmp.end());
//     for(int i = 0; i < tmp.size(); i++)
//         RNG_QUEUE.push(tmp.at(i));
// }

// static int getNextRandom()
// {
//     int tmp = RNG_QUEUE.front();
//     RNG_QUEUE.pop();
//     return tmp;
// }

#endif // SRC_MODEL_TOOLS_H_