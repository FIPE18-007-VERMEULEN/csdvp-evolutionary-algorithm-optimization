#include <iostream>
#include <assert.h>

#include "competencyDistribution.h"

std::vector<int> CompetencyDistribution::HLEVEL(10);

void CompetencyDistribution::linearDistribution(CSDVP &pb)
{
    int interval = CompetencyDistribution::HLevelRange(pb);
    if(pb.cfg_competencyByCourseMin() != 0) //if the min borne is not 0, we add another interval for comp with no prereq
        interval++;
    
    int nbCompByHL = pb.cfg_quantityCompetencies() / interval;

    std::cout << "Interval" << interval << " -- nbCompByHL:" << nbCompByHL << std::endl;

    for(int i = 0; i < pb.cfg_quantityCompetencies() && i < nbCompByHL * interval; i++)
    {
        pb.unlocked_competenciesCatalogue().at(i).setHL(i/nbCompByHL);
    }

    //Taking into account comp missed with the division
    int diff = pb.cfg_quantityCompetencies() - nbCompByHL*interval;
    int idxInterval = 0;

    for(int i = pb.cfg_quantityCompetencies()-1; i >= nbCompByHL*interval; i--)
    {
        pb.unlocked_competenciesCatalogue().at( i ).setHL(idxInterval%interval);
        idxInterval++;
    }
}

// === STATIC
    int CompetencyDistribution::HLevelRange(CSDVP & pb)
    {
        int interval = pb.cfg_competencyByCourseMax() - pb.cfg_competencyByCourseMin();
        return interval;
    }

    std::vector<Competency> CompetencyDistribution::getHLevel(CSDVP & pb, int level)
    {
        std::vector<Competency> comp = std::vector<Competency>();

        if(level < 0)
            return comp;
        if(level > CompetencyDistribution::HLevelRange(pb))
            return comp;
        
        std::vector<Competency> pbComp = pb.competencyCatalogue();
        for(int i = 0; i < pbComp.size(); i++)
            if(pbComp[i].hLevel() == level)
                comp.push_back(pbComp[i]);
        
        return comp;
    }

    std::vector<Competency> CompetencyDistribution::upToHLevel(CSDVP & pb, int level)
    {
        std::vector<Competency> res = std::vector<Competency>();
        std::vector<Competency> tmp; //dry prog 
        int start;

        for(start = 0; start <= level ; start++)
        {
            tmp = CompetencyDistribution::getHLevel(pb, start);
            for(int i = 0; i < tmp.size(); i++)
                res.push_back(tmp[i]);
        }

        return res;
    }

    void CompetencyDistribution::sanitizeHLEVEL()
    {
        int sum = 0;
        std::vector<int> tmp;

        for(int i = 0; i < CompetencyDistribution::HLEVEL.size(); i++)
        {
            if(CompetencyDistribution::HLEVEL.at(i) >= 0) //ignoring all negative value
            {
                sum += CompetencyDistribution::HLEVEL.at(i);
                tmp.push_back(CompetencyDistribution::HLEVEL.at(i));
            }
        }

        assert(sum==100);

        CompetencyDistribution::HLEVEL = tmp;
    }

    void CompetencyDistribution::displayHLevel()
    {
        std::cout << "HLEVEL:" << std::endl << "[";
        for(int i = 0 ; i < CompetencyDistribution::HLEVEL.size()-1; i++)
        {
            std::cout << CompetencyDistribution::HLEVEL.at(i) << "|";
        }
        std::cout << CompetencyDistribution::HLEVEL.at(CompetencyDistribution::HLEVEL.size()-1) << "]" << std::endl << std::endl;
    }