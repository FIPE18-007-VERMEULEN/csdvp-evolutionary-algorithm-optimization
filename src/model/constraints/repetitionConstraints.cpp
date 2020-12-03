#include "repetitionConstraints.h"

#include "model/course.h"

std::pair<bool, double> ConstraintsRepetition::integrityCheck(Cursus indiv)
{
    int nbOfRepetition = 0;

    for(unsigned int i = 0; i < indiv.size(); i++)
    {
        for(unsigned int j = i+1; j < indiv.size(); j++)
        {
            if(indiv.at(i) == indiv.at(j))
                nbOfRepetition++;
        }
    }
    bool isOK = nbOfRepetition == 0;
    double metric = 1 - ( ( (double) nbOfRepetition * 2) / (double)indiv.size() );
    return std::pair<bool, double>(isOK, metric);
}