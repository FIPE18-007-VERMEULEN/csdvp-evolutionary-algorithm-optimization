#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdlib>

#include "problem.h"
#include "tools.h"

#include "exception/csdvpOverlapingBoundaryException.h"
#include "exception/notImplementedException.h"

int CSDVP::CSDVP_COUNTER = 0;

// === FACTORY
    // No factory. Only one constructor : all config has to be manually set
// === END FACTORY

// === CONSTRUCTOR
    CSDVP::CSDVP()
    {
        this->_id = CSDVP::assignID();

        this->_isConfig = false;
        this-> _seed = -1;

        this->_quantityAvailableCompetencies = -1;
        this->_quantityAvailableCourses = -1;
        this->_minimalTimeFrame = -1;
        this->_maximalTimeFrame = -1;
        this->_minimalECTSValue = -1;
        this->_maximalECTSValue = -1;
        this->_minimalCoursesByTimeFrame = -1;
        this->_maximalCoursesByTimeFrame = -1;
    }
// === END CONSTRUCTOR

// === MUTATOR
        // SETTER 
        void CSDVP::set_cfg_quantityCompetencies(int nb)
            {this->_quantityAvailableCompetencies = nb;}
        void CSDVP::set_cfg_quantityCourses(int nb)
            {this->_quantityAvailableCourses = nb;}
        void CSDVP::set_cfg_minimalTimeFrames(int nb)
            {this->_minimalTimeFrame = nb;}
        void CSDVP::set_cfg_maximalTimeFrames(int nb)
            {this->_maximalTimeFrame = nb;}
        void CSDVP::set_cfg_ectsMin(int nb)
            {this->_minimalECTSValue = nb;}
        void CSDVP::set_cfg_ectsMax(int nb)
            {this->_maximalECTSValue = nb;}
        void CSDVP::set_cfg_courseByTFMin(int nb)
            {this->_minimalCoursesByTimeFrame = nb;}
        void CSDVP::set_cfg_courseByTFMax(int nb)
            {this->_maximalCoursesByTimeFrame = nb;}
        void CSDVP::set_cfg_minimalMagnitude(double m)
        {
            this->_minimalMagnitude = Magnitude::build(m);
        }
        void CSDVP::set_cfg_maximalMagnitude(double m)
        {
            this->_maximalMagnitude = Magnitude::build(m);
        }
        
        void CSDVP::setTimeFrames(std::vector<int> & v)
            {this->_timeFrames = v;}
        void CSDVP::setCoursesCatalogue(std::vector<Course> & c)
            {this->_availableCourses;}
        void CSDVP::setCompetenciesCatalogue(std::vector<Competency> & c)
            {this->_availableCompentecies;}
        // ADDER
        void CSDVP::addTimeFrame(int tF)
        {
            if(duplicataFlag(this->_timeFrames, tF))
                return; // NTD

            this->_timeFrames.push_back(tF);
        }
        void CSDVP::addCourseToCatalogue(Course & c)
        {
            if(duplicataFlag(this->_availableCourses, c))
                return;

            this->_availableCourses.push_back(c);
        }

        void CSDVP::addCompetencyToCatalogue(Competency & c)
        {
            if(duplicataFlag(this->_availableCompentecies, c))
                return;
            this->_availableCompentecies.push_back(c);
        }


// === END MUTATOR

// === FUNC
    bool CSDVP::checkConfig()
    {
        //If one of the following is true, CSDVP is not config
        if( this->_seed < 0                         ||
            this->_quantityAvailableCompetencies < 0||
            this->_quantityAvailableCourses < 0     ||
            this->_minimalTimeFrame < 0             ||
            this->_maximalTimeFrame < 0             ||
            this->_minimalECTSValue < 0             ||
            this->_maximalECTSValue < 0             ||
            this->_minimalCoursesByTimeFrame < 0    ||
            this->_maximalCoursesByTimeFrame < 0    ||
            this->_minimalMagnitude.value() < 0     ||
            this->_maximalMagnitude.value() < 0     )
        {
            this->_isConfig = false;
            return this->_isConfig;
        }

        // verify overlaping range
        if( this->_minimalTimeFrame > this->_maximalTimeFrame ||
            this->_minimalECTSValue > this->_maximalECTSValue ||
            this->_minimalCoursesByTimeFrame > this->_maximalCoursesByTimeFrame ||
            this->_minimalMagnitude > this->_maximalMagnitude)
        {
            throw CSDVPOverlapingBoundariesException(this);
        }

        this->_isConfig = true;
        return this->_isConfig;
    }
// === END FUNC

// === STATIC
    int CSDVP::assignID()
    { return ++CSDVP::CSDVP_COUNTER;}

    // ---------- GENERATION RELATED FUNCTIONS ----------    
    void CSDVP::generateProblem(CSDVP & csdvp, CSDVP::GenerationType type, int seed)
    {
        csdvp._seed = seed;

        if(! csdvp.checkConfig() ) //if csdvp is not configurated, aborting generation
            return; //aborting pb generation

        std::cout << "generateProblem TODO" << std::endl;
        switch (type)
        {
        case CSDVP::GenerationType::RANDOM:
            _randomlyGenerated(csdvp);
            break;
        default:
            break;
        }
    }

    int CSDVP::_randomizeIn(const int min, const int max)
    {
        return min + ( rand() % (max - min + 1) );
    }

    double CSDVP::_randomizeIn(const double min, const double max)
    {
        throw NotImplementedException("CSDVP::_randomizeIn");
    }

    void CSDVP::randomizeProblem(CSDVP & pb, int seed)
    {
        pb._seed = seed;
        srand(pb.seed());

        pb.set_cfg_ectsMin(_randomizeIn(1, CSDVP::RANDOM_MAX_ECTS_VALUE));
        pb.set_cfg_ectsMax(_randomizeIn(pb.cfg_ectsMin(), CSDVP::RANDOM_MAX_ECTS_VALUE));

        throw NotImplementedException("_randomizeProblem");

        //SKILL_MAX_VALUE
    }

    void CSDVP::_randomlyGenerated(CSDVP & pb)
    {
        srand(pb.seed());

        pb.unlocked_timeFrames().clear();
        pb.unlocked_competenciesCatalogue().clear();
        pb.unlocked_coursesCatalogue().clear();
        
        for(int i = 0; i < (pb.cfg_maximalTimeFrame() - pb.cfg_minimalTimeFrame()) + 1 ; i++)
        {
            pb.addTimeFrame(pb.cfg_minimalTimeFrame()+i);
        }
    }
    // --------- END GENERATION RELATED FUNCTIONS ---------

// === END STATIC

// === OPERATOR
    std::ostream & operator<<(std::ostream & Stream, const CSDVP & c)
    {
        std::string s = "--------------\n| Problem n°"+std::to_string(c.id())+"|\n---------------\n| Configuration:";
        s+= "\n\tseed: "+std::to_string(c.seed())+"\n\tNb comp: "+std::to_string(c.cfg_quantityCompetencies())+"\n\tNb courses: "+std::to_string(c.cfg_quantityCourses())+"\n\tMin TimeF: "+std::to_string(c.cfg_minimalTimeFrame())+"\n\tMax TimeF: "+std::to_string(c.cfg_maximalTimeFrame());
        s+= "\n\tECTS Min: "+std::to_string(c.cfg_ectsMin())+"\n\tECTS Max: "+std::to_string(c.cfg_ectsMax())+"\n\tCourse by TF min: "+std::to_string(c.cfg_courseByTFMin())+"\n\tCourse by TF max: "+std::to_string(c.cfg_courseByTFMax());
        s+="\n\tMagnitude min: "+std::to_string(c.cfg_magnitudeMax().value())+"\n\tMagnitude max: "+std::to_string(c.cfg_magnitudeMax().value());

        Stream << s;
        return Stream;
    }
// === END OPERATOR