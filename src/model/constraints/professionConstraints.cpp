#include "professionConstraints.h"

#include <string>
#include <utility>

#include "model/course.h"
#include "model/competency.h"
#include "model/tools.h"

#include "model/exception/competencyEvolvingException.h"

int ConstraintsProfession::DISCRETE_METRIC = 1;

std::pair<bool, double> ConstraintsProfession::integrityCheck(Cursus indiv)
{
    std::vector<Competency> compToAnswer;

    for(unsigned int i = 0 ; i < this->_job.prerequisites().size(); i++)
    {
        std::string name = this->_job.prerequisites().at(i).c_name();
        compToAnswer.push_back(Competency::buildTMP(0, name)); //same name to exploit the Competency::operator== on name equality
    }

    Course current;
    Competency currentComp;
    std::pair<int, Competency> posFound;

    bool changedTF = false;
    int currentTF = 0;

    for(unsigned int i = 0 ; i < indiv.size(); i++)
    {
        current = this->_pb.coursesCatalogue().at(indiv.at(i));
        
        if(currentTF != i / this->_pb.cfg_pickedCoursesByTimeFrame())
            changedTF = true;
        else
            changedTF = false;
        currentTF = i / this->_pb.cfg_pickedCoursesByTimeFrame();

        for(unsigned int j = 0; j < compToAnswer.size(); j++)
        {
            compToAnswer.at(j).increaseDecay();
        }

        for(unsigned int j = 0 ; j < current.teachedCompetenciesWeighted().size() ; j++)
        {
            currentComp = current.teachedCompetenciesWeighted().at(j).first;

            posFound = findInVector(compToAnswer, currentComp);
            if(posFound.first != -1)
            {
                try
                {
                    compToAnswer.at(posFound.first).saveDecay();
                    compToAnswer.at(posFound.first).evolveTowards(currentComp.magnitude());
                }
                catch(CompetencyEvolvingException & e) //if CEE is thrown, then magnitude has been auto rebased
                {
                    //Should has NTD here
                    //compToAnswer.at(posFound.first) = e.getCompetency();
                    //std::cout << "INFO:\n(during ConstraintsProfession)\n\n Compentecy evolution throw an exception. Auto rebase. New value is " << e.getCompetency() << std::endl;
                }
            }
        }
    }

    for(unsigned int i = 0; i < compToAnswer.size(); i++)
    {
        compToAnswer.at(i).saveDecay();
        //std::cout << compToAnswer.at(i) << std::endl;
    }

    //Now that we have evolve all the tmp competency, we compate their mag to the requirement. We count how many is not met to define the metric

    int score = 0;
    double magDiff = 0; // addendum from HL
    for(unsigned int i = 0; i < this->_job.prerequisites().size(); i++)
    {
        if(compToAnswer.at(i).magnitude().value() < this->_job.prerequisites().at(i).c_magnitude().value())
        {
            score++;
            magDiff += ( this->_job.prerequisites().at(i).c_magnitude().value() - compToAnswer.at(i).magnitude().value() ) / this->_job.prerequisites().at(i).c_magnitude().value() ;
        }
        
    }

    //std::cout << "Score: " << std::to_string(score) << std::endl;
    //std::cout << "Size: " << std::to_string(compToAnswer.size()) << std::endl;
    bool res = score == 0;

    switch (ConstraintsProfession::DISCRETE_METRIC) //whether we use discrete or continue metrics
    {
    case 0:
        return std::pair<bool, double>(res, 1 - ( magDiff / (double)compToAnswer.size() ) );    
    default:
        return std::pair<bool, double>(res, 1 - ( (double)score / (double)compToAnswer.size()));
    }
}
