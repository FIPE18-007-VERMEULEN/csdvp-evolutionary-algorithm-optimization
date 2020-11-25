#include "prerequisitesConstraints.h"

#include <string>
#include <utility>
#include <tuple>

#include "model/course.h"
#include "model/competency.h"
#include "model/tools.h"

#include "model/exception/competencyEvolvingException.h"

int ConstraintsPrerequisites::DISCRETE_METRIC = 1;

std::pair<bool, double> ConstraintsPrerequisites::integrityCheck(Cursus indiv)
{
    int currentTF = 0;
    int notFound = 0;
    int notRespected = 0;
    int score = 0;
    int nbPrereq = 0;

    int magDivisor = 0;
    double magDiff = 0;

    //Each comp availble at a specific TF. Decay can be applied between i and i+1
    std::vector<std::vector<Competency>> compByTF(this->_pb.timeFrames().size());

    Course currentCourse;
    Competency currentCompetency;
    std::tuple<int, int, double, int> prereqFound;
        std::get<0>(prereqFound) = 0; std::get<1>(prereqFound) = 0;
        std::get<2>(prereqFound) = 0; std::get<3>(prereqFound) = 0;
    std::pair<int, Competency> alreadyExists;

    bool changedTF = false;

    //std::cout << "=========START========" << std::endl;
    for(int i = 0; i < indiv.size() ; i++)
    {
        if(currentTF != i / this->_pb.cfg_pickedCoursesByTimeFrame())
        {
            //std::cout << "I've changed of TF" << std::endl;
            changedTF = true;
        }
        else
            changedTF = false;
        
        currentTF = i / this->_pb.cfg_pickedCoursesByTimeFrame();
        //std::cout << "Current TF: " << std::to_string(currentTF) << std::endl;
        //If changedTF is set to true, then we have changed of TF, we need to make available all the Comp in TF-1 here in TF, in addition to the new that'll be discovered in TF 

        for(int j = 0 ; changedTF && j < compByTF.at(currentTF-1).size(); j++)
        {
            // HERE, VARIABLE DECAY CAN BE APPLIED!!!!!!!!
            compByTF.at(currentTF).push_back(compByTF.at(currentTF-1).at(j));
            compByTF.at(currentTF).at(compByTF.at(currentTF).size()-1).increaseDecay();
        }

        // Then, we explore the current TF for new Comp
        currentCourse = this->_pb.coursesCatalogue().at(indiv.at(i));
        //std::cout << "\t" << currentCourse << std::endl;

        //std::cout << "\tPrereq: " << std::to_string(currentCourse.prerequisites().size()) << std::endl;
        nbPrereq += currentCourse.prerequisites().size();


        //Handling prereq
        // Check if prerequisites exists in TF-1
        if(currentTF > 0)
        {
            prereqFound = this->_prereqsInPreviousTF(compByTF.at(currentTF-1), currentCourse.prerequisites());
        }
        else
        {
            prereqFound = this->_prereqsInPreviousTF(std::vector<Competency>(0), currentCourse.prerequisites());
        }
        notFound        += std::get<0>(prereqFound);
        notRespected    += std::get<1>(prereqFound);
        magDiff         += std::get<2>(prereqFound);
        magDivisor      += std::get<3>(prereqFound);


        // Handling teached comp
        for(int j = 0; j < currentCourse.teachedCompetenciesWeighted().size() ; j++)
        {
            currentCompetency = currentCourse.teachedCompetenciesWeighted().at(j).first;

            //Comp already exists in the array ?
            alreadyExists = findInVector(compByTF.at(currentTF), currentCompetency);

            if(alreadyExists.first >= 0) //Already Exists in the array
            {
	      //std::cout << currentCompetency.c_name() << " already exists" <<std::endl;
                try
                {
                    compByTF.at(currentTF).at(alreadyExists.first).saveDecay();
                    compByTF.at(currentTF).at(alreadyExists.first).evolveTowards(currentCompetency.magnitude());
                }
                catch(CompetencyEvolvingException & e) //if CEE is thrown, then magnitude has been auto rebased
                {
                    //Should has NTD here
                    //compToAnswer.at(posFound.first) = e.getCompetency();
                    //std::cout << "INFO:\n(during ConstraintsProfession)\n\n Compentecy evolution throw an exception. Auto rebase. New value is " << e.getCompetency() << std::endl;
                    //std::cout << "Comp rebased val is" << compByTF.at(currentTF).at(alreadyExists.first) << std::endl;
                    //std::cout << "exception.Rebased";
                }
            }
            else
            {
                compByTF.at(currentTF).push_back(Competency::buildTMP(currentCompetency.c_magnitude().value(), currentCompetency.c_name()));
            }
        }
    }

    // //std::cout << "==EXPLORING COMP BY TF" << std::endl;
    // for(int i = 0; i < compByTF.size(); i++)
    // {
    //   //std::cout << "TF#" << std::to_string(i) << std::endl;
    //     for(int j = 0; j < compByTF.at(i).size() ; j++)
    //     {
	//   //std::cout << compByTF.at(i).at(j) << std::endl;
    //     }
    // }

    bool isOK = ((notFound == 0) && (notRespected == 0));
    /*
      std::cout << "========== PREREQ CSTR RES ==========" << std::endl;
    std::cout << "Not Found: " << std::to_string(notFound) << std::endl;
    std::cout << "Not Respected: " << std::to_string(notRespected) << std::endl;
    std::cout << "Nb Prereq: " << std::to_string(nbPrereq) << std::endl;
    */
    switch (ConstraintsPrerequisites::DISCRETE_METRIC)
    {
    case 0/* constant-expression */:
        std::cout << "MagDiff: " << magDiff << " for " << (double)magDivisor << " prereqs. (1 - " << ( magDiff / (double)magDivisor ) << std::endl;
        assert(magDivisor != 0);
        return std::pair<bool, double>(isOK, 1 - ( magDiff / (double)magDivisor ) );    
    default:
        double metric = 0;
        if(nbPrereq > 0)
        {
            metric = 1.0 - ( (((double)2 * (double)notFound) + (double)notRespected ) / (2 * (double) nbPrereq) );
        }
        else //can't divide by 0
        {
            if(isOK)
                metric = 1;
            else
                metric = 0;
        }
        //std::cout << "Metric: " << std::to_string(metric) << std::endl;
        //std::cout << "====================" << std::endl;
        return std::pair<bool, double>(isOK, metric);
    }
}

std::tuple<int, int, double, int> ConstraintsPrerequisites::_prereqsInPreviousTF(std::vector<Competency> cInTF, std::vector<Competency> prereqs)
{
    int notFound = 0;
    int notRespected = 0;
    int nbFound = 0;
    bool found = false;
    double magDiff = 0;
    int divisor = 0;

    if(cInTF.size() == 0) //if empty, we'll find nothing
        return std::tuple<int, int, double, int>(prereqs.size(), 0, magDiff, divisor);

    for(int i = 0; i < prereqs.size(); i++)
    {
        found = false;
        //std::cout << "Looking for " << prereqs.at(i) << std::endl;

        for(int j = 0 ; j < cInTF.size() && !found; j++)
        {
	  //std::cout << "\n\t" << cInTF.at(j) << std::endl;

            if(prereqs.at(i)==cInTF.at(j))
            {
                nbFound++;
                found = true;
                if(prereqs.at(i).c_magnitude().value() > cInTF.at(j).decay())
                {
                    notRespected++;
                    magDiff += ( prereqs.at(i).c_magnitude().value() - cInTF.at(j).decay() ) / prereqs.at(i).c_magnitude().value();
                    std::cout << "\tMag diff: " << prereqs.at(i).c_magnitude().value() - cInTF.at(j).decay()  << "\t Ratio:" << magDiff << std::endl;
                }
            }
        }

        if(!found)
            notFound++;
    }

    divisor = (notFound + nbFound);
    //std::cout << "NF: " << std::to_string(notFound) << " | NR: " << std::to_string(notRespected) << std::endl;
    return std::tuple<int, int, double, int>(notFound, notRespected, magDiff, divisor);
}
