#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>

#include "profession.h"
#include "competency.h"
#include "competencyDistribution.h"

#include "exception/JobOverlappingBoundariesException.h"

int Profession::PROFESSION_COUNTER = 0;
unsigned int Profession::JOB_SELECTION_TYPE = 0;

// === FACTORY
    // No factory needed
// === END FACTORY

// === CONSTRUCTOR
    Profession::Profession()
    {
        this->_id = assignID();
        this->_name = "Profession#"+std::to_string(this->_id);
    }
    Profession::Profession(std::string name)
    {
        this->_id = assignID();

        if(name.empty())
            name = "Profession#"+std::to_string(this->_id);
        this->_name = name;
    }

    Profession::Profession(std::vector<Competency> & p, std::string n)
        : _prerequisites(p)
    {
        this->_id = assignID();
        if(n.empty())
            n = "Profession#"+std::to_string(this->_id);
        this->_name = n;
    }

// === END CONSTRUCTOR

// === MUTATOR
    // SETTER
    void Profession::setName(std::string name)
    {
        if(name.empty())
            name = "Profession#"+std::to_string(this->_id);
        this->_name = name;
    }

    std::vector<Competency> & Profession::setPrerequisites(std::vector<Competency> & p)
    {
        std::vector<Competency> * old = &this->_prerequisites;
        this->_prerequisites = p;
        return *old;
    }

    void Profession::setRequiredECTS(int nb)
    {
        assert(nb >= 0);

        this->_requiredECTS = nb;
    }

    void Profession::setRequiredECTS(Profession::GenerationType type)
    {
        if(type == Profession::GenerationType::RANDOM)
            this->_isECTSRandom = true;
    }

    void Profession::set_cfg_minimalPrerequisites(int nb)
        {this->_minimalPrerequisites = nb;}
    void Profession::set_cfg_maximalPrerequisites(int nb)
        {this->_maximalPrerequisites = nb;}
    void Profession::set_cfg_minimalMagnitude(double mag)
    {
        this->_minimalMagnitude = Magnitude::build(mag);
    }
    void Profession::set_cfg_maximalMagnitude(double mag)
    {
        this->_maximalMagnitude = Magnitude::build(mag);
    }

    // ADDER
    /** Adds a new competency to the prerequisites of a profession. It is protected against duplicata: it can't have twice the same competency.
     * 
     * @return true if an insertion as been made into the prerequiste, false otherwise;
     * 
     * Warning: does not use shallow copy. This can lead to a potential non constant behaviors if use with Profession(vector<Competency>) regarding the modification of compentecies outside this.
     * @note Remove the referencing? 
     */
    bool Profession::addPrerequisite(Competency & c)
    {
        if(_duplicataProtection(& this->_prerequisites, c))
            return false;
        this->_prerequisites.push_back(c);
        return true;
    }

// === FUNC
    /// _duplicataProtection returns true if the value (2nd param) searched into (1st param) is found
    bool Profession::_duplicataProtection(std::vector<Competency> * prereq, Competency c)
    {
        std::vector<Competency>::iterator it = std::find( prereq->begin(), prereq->end(), c);
        return it != prereq->end();
    }

// === OPERATOR
std::ostream & operator<<(std::ostream & Stream, const Profession & p)
{
    std::string s = "Profession\n\tid:"+std::to_string(p.id())+"\n\tName:"+p.name()+"\n\tECTS: " + std::to_string(p.requiredECTS()) +"\n\t#Prereq:"+std::to_string(p.prerequisites().size())+"\n\t===Details:===\n\t";
    Stream << s ;
    for(unsigned int i = 0; i < p.prerequisites().size(); i++)
        Stream << "("<< i<< "th) " << p.prerequisites().at(i) << "\n\t";
    Stream << "==========" << std::endl;
    return Stream;
}

bool Profession::checkConfig()
{
    if( !this->_isECTSRandom && this->_requiredECTS <= 0)
    {
        this->_isConfig = false;
        std::cout << "INFO: Profession lacks ECTS config" << std::endl;
        return false;
    }

    if(this->_name.empty())
    {
        this->_isConfig = false;
        std::cout << "INFO: Profession lacks name config" << std::endl;
        return false;
    }

    if( this->_minimalPrerequisites     < 0 ||
        this->_maximalPrerequisites     < 0 ||
        this->_minimalMagnitude.value() < 0 ||
        this->_maximalMagnitude.value() < 0 )
    {
        std::cout << "INFO: Config attributes not yet config" << std::endl;
        this->_isConfig = false;
        return false;   
    }
    //verifying overlapping range
    if( this->_minimalPrerequisites > this->_maximalPrerequisites ||
        this->_minimalMagnitude.value() > this->_maximalMagnitude.value())
    {
        throw JobOverlappingBoundariesException(this);
    }

    this->_isConfig = true;
    return true;
}

void Profession::_randomlyGenerate(Profession & job, CSDVP & pb)
{
    srand(job.seed());

    job.unlocked_prerequisites().clear();

    int howManyPrereq = job.cfg_minimalPrerequisites() + ( rand() % ( job.cfg_maximalPrerequisites() - job.cfg_minimalPrerequisites() + 1) );

    std::vector<Competency> tmpComp = pb.competencyCatalogue();

    Competency ctmp;
    double magVal;

    std::random_shuffle(tmpComp.begin(), tmpComp.end());

    int i;
    std::vector<Competency> compHigherHL;
    
    switch (Profession::JOB_SELECTION_TYPE)
    {
    case 1: //at least one comp in the higher HL
        compHigherHL = CompetencyDistribution::getHLevel(pb, CompetencyDistribution::HLevelRange(pb)-1);
        // std::cout << "compHigherHL size :" << compHigherHL.size() << std::endl;
        std::random_shuffle(compHigherHL.begin(), compHigherHL.end());
        
        assert(compHigherHL.size() > 0); //if no comp retrieved in the higher hlevel (hhl), there is a pb here !

        magVal = job.cfg_minimalMagnitude().value() + ( (double)rand()/RAND_MAX) * ( job.cfg_maximalMagnitude().value() - job.cfg_minimalMagnitude().value()) ;
        ctmp = Competency::buildTMP(magVal,compHigherHL.at(0).name());
        ctmp.setHL(compHigherHL.at(0).hLevel());
        job.addPrerequisite(ctmp);

        // !! No duplicata protection: we can insert another time the competency above from the HHL
        for(i = 0; i < (int)tmpComp.size() && i < howManyPrereq-1; i++) //cp/paste from default case
        {
            magVal = job.cfg_minimalMagnitude().value() + ( (double)rand()/RAND_MAX) * ( job.cfg_maximalMagnitude().value() - job.cfg_minimalMagnitude().value()) ;
            ctmp = Competency::buildTMP(magVal,tmpComp.at(i).name());
            ctmp.setHL(tmpComp.at(i).hLevel());
            job.addPrerequisite(ctmp);
        }
        break;
    case 2: // emphasis on higher HL
        _pickWithHLWeighting(howManyPrereq, job, pb);
        break;
    default: //classic behavior
        for(i = 0; i < (int)tmpComp.size() && i < howManyPrereq; i++)
        {
            magVal = job.cfg_minimalMagnitude().value() + ( (double)rand()/RAND_MAX) * ( job.cfg_maximalMagnitude().value() - job.cfg_minimalMagnitude().value()) ;
            ctmp = Competency::buildTMP(magVal,tmpComp.at(i).name());
            ctmp.setHL(tmpComp.at(i).hLevel());
            job.addPrerequisite(ctmp);
        }

        if(i != howManyPrereq) //Warning need to check if still in range because not enought courses
        {
            if(i < job.cfg_minimalPrerequisites() || i > job.cfg_maximalPrerequisites())
            {
                //considering as a fail during generation
                assert(i < job.cfg_minimalPrerequisites() || i > job.cfg_maximalPrerequisites());
            }
        }
        break;
    }

    // If ECTS is set to be random, then calculating it
    if(!job._isECTSRandom)
        return;
    
    // Formula of ECTS calcul is : SIGMA_{i=0}^{maxTF}(SIGMA_{j=0}^{nbCourseAvg}(rand(pb.minECTS,pb.maxECTS)))
    int ects= 0;
    
    // ects = pb.timeFrames().size()  * 30; //30 is the default european value 

    for(i = 0; i < (int)pb.timeFrames().size(); i++)
    {
        for(int j = 0; j < pb.cfg_pickedCoursesByTimeFrame(); j++)
        {
            if( (rand() % 100) > 35) //Take the average in 100-35 % of the cases
                ects += ( pb.cfg_ectsMin() + rand() % (pb.cfg_ectsMax() - pb.cfg_ectsMin() +1) );
            else
                ects += ( pb.cfg_ectsMin() + rand() % ( (pb.cfg_ectsMax() -1) - pb.cfg_ectsMin() +1) );
        }
    }

    job.setRequiredECTS(ects);
}

// Here we weight where we pick the comp, the higher the HL, more likely a comp is to be pick
void Profession::_pickWithHLWeighting(int nbToPick, Profession & job, CSDVP & pb)
{
    std::vector<int> range;
    int sumInterval = 0;
    int x; unsigned int currentHL;
    double magVal;
    Competency ctmp;
    const int hLRange = CompetencyDistribution::HLevelRange(pb);
    std::vector<Competency> hlComp;

    for(int i = 0; i < hLRange ; i++)
    {
        // sumInterval+=i;
        // sumInterval = i * 2;
        sumInterval = i * i;
        range.push_back(sumInterval);
    }

    for(int i = 0; i < nbToPick; i++)
    {
        x = rand() % ( sumInterval + 1);
        assert(x <= sumInterval);

        currentHL = 0;

        // std::cout << "sumInterval: " << sumInterval << " & x: " << x << std::endl;

        while(x > range[currentHL] && currentHL < range.size())
        {
            currentHL++;
        }
        
        hlComp = CompetencyDistribution::getHLevel(pb, currentHL);//we get the correspond hl level
        assert(hlComp.size() > 0);
        std::random_shuffle(hlComp.begin(), hlComp.end());
        magVal = job.cfg_minimalMagnitude().value() + ( (double)rand()/RAND_MAX) * ( job.cfg_maximalMagnitude().value() - job.cfg_minimalMagnitude().value()) ;
        ctmp = Competency::buildTMP(magVal,hlComp.at(0).name());
        ctmp.setHL(hlComp.at(0).hLevel());
        job.addPrerequisite(ctmp);
    }
}

// === STATIC
int Profession::assignID(){return ++Profession::PROFESSION_COUNTER;}

    // ---------- GENERATION FUNCTIONS ----------
    void  Profession::generateProfession(Profession & job, Profession::GenerationType type, CSDVP & csdvp, int seed)
    {
        job._seed = seed;

        if( !job.checkConfig() || !csdvp.checkConfig())
        {
            std::cout << "Can't generate a profession with non configurated problem and profession !" << std::endl;
            return;
        }

        switch (type)
        {
        case Profession::GenerationType::RANDOM:
            _randomlyGenerate(job, csdvp);
            break;
        
        default:
            break;
        }
    }