#include "prerequisitesConstraints.h"

#include <string>
#include <utility>

#include "model/course.h"
#include "model/competency.h"
#include "model/tools.h"

#include "model/exception/competencyEvolvingException.h"

std::pair<bool, double> ConstraintsPrerequisites::integrityCheck(Cursus indiv)
{
    int currentTF = 0;
    int notFound = 0;
    int notRespected = 0;
    int score = 0;
    int nbPrereq = 0;

    //Each comp availble at a specific TF. Decay can be applied between i and i+1
    std::vector<std::vector<Competency>> compByTF(this->_pb.timeFrames().size());

    Course currentCourse;
    Competency currentCompetency;
    std::pair<int, int> prereqFound; prereqFound.first = 0; prereqFound.second = 0;
    std::pair<int, Competency> alreadyExists;

    bool changedTF = false;

    for(int i = 0; i < indiv.size() ; i++)
    {
        if(currentTF != i / this->_pb.cfg_pickedCoursesByTimeFrame())
        {
            std::cout << "I've changed of TF" << std::endl;
            changedTF = true;
        }
        else
            changedTF = false;

        currentTF = i / this->_pb.cfg_pickedCoursesByTimeFrame();
        std::cout << "Current TF: " << std::to_string(currentTF) << std::endl;
        //If changedTF is set to true, then we have changed of TF, we need to make available all the Comp in TF-1 here in TF, in addition to the new that'll be discovered in TF 

        for(int j = 0 ; changedTF && j < compByTF.at(currentTF-1).size(); j++)
        {
            // HERE, VARIABLE DECAY CAN BE APPLIED!!!!!!!!
            compByTF.at(currentTF).push_back(compByTF.at(currentTF-1).at(j));
        }

        // Then, we explore the current TF for new Comp
        currentCourse = this->_pb.coursesCatalogue().at(indiv.at(i));
        std::cout << "\tPrereq: " << std::to_string(currentCourse.prerequisites().size()) << std::endl;
        nbPrereq += currentCourse.prerequisites().size();

        for(int j = 0; j < currentCourse.teachedCompetenciesWeighted().size() ; j++)
        {
            currentCompetency = currentCourse.teachedCompetenciesWeighted().at(j).first;

            //Comp already exists in the array ?
            alreadyExists = findInVector(compByTF.at(currentTF), currentCompetency);

            if(alreadyExists.first >= 0) //Already Exists in the array
            {
                try
                {
                    compByTF.at(currentTF).at(alreadyExists.first).evolveTowards(currentCompetency.magnitude());
                }
                catch(CompetencyEvolvingException & e) //if CEE is thrown, then magnitude has been auto rebased
                {
                    //Should has NTD here
                    //compToAnswer.at(posFound.first) = e.getCompetency();
                    std::cout << "INFO:\n(during ConstraintsProfession)\n\n Compentecy evolution throw an exception. Auto rebase. New value is " << e.getCompetency() << std::endl;
                }
            }
            else
            {
                // Check if prerequisites exists in TF-1
                if(currentTF > 0)
                prereqFound = this->_prereqsInPreviousTF(compByTF.at(currentTF-1), currentCourse.prerequisites());
                
                notFound += prereqFound.first;
                notRespected += prereqFound.second;

                compByTF.at(currentTF).push_back(Competency::build(currentCompetency.c_magnitude().value(), currentCompetency.c_name()));
            }
        }
    }

    bool isOK = ((notFound == 0) && (notRespected == 0));
    std::cout << "Not Found: " << std::to_string(notFound) << std::endl;
    std::cout << "Not Respected: " << std::to_string(notRespected) << std::endl;
    std::cout << "Nb Prereq: " << std::to_string(nbPrereq) << std::endl;
    double metric = 1.0 - ( (((double)2 * (double)notFound) + (double)notRespected ) / (2 * (double) nbPrereq) );
    std::cout << "Metric: " << std::to_string(metric) << std::endl;
    return std::pair<bool, double>(isOK, metric);
}

std::pair<int, int> ConstraintsPrerequisites::_prereqsInPreviousTF(std::vector<Competency> cInTF, std::vector<Competency> prereqs)
{
    int notFound = 0;
    int notRespected = 0;
    bool found = false;

    for(int i = 0; i < prereqs.size(); i++)
    {
        found = false;

        for(int j = 0 ; j < cInTF.size() && !found; j++)
        {
            if(prereqs.at(i)==cInTF.at(j))
            {
                found = true;
                if(prereqs.at(i).c_magnitude().value() > cInTF.at(j).c_magnitude().value())
                 notRespected++;
            }
        }

        if(!found)
            notFound++;
    }

    return std::pair<int, int>(notFound, notRespected);
}