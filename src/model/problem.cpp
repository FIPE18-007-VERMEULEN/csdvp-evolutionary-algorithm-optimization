#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>

#include "problem.h"
#include "tools.h"

#include "exception/csdvpOverlapingBoundaryException.h"

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
        this->_quantityOfTimeFrame = -1;
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
        void CSDVP::set_cfg_quantityTimeFrames(int nb)
            {this->_quantityOfTimeFrame = nb;}
        void CSDVP::set_cfg_ectsMin(int nb)
            {
                this->_minimalECTSValue = nb;}
        void CSDVP::set_cfg_ectsMax(int nb)
            {this->_maximalECTSValue = nb;}
        void CSDVP::set_cfg_courseByTFMin(int nb)
            {this->_minimalCoursesByTimeFrame = nb;}
        void CSDVP::set_cfg_courseByTFMax(int nb)
            {this->_maximalCoursesByTimeFrame = nb;}
        
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
            this->_quantityOfTimeFrame < 0          ||
            this->_minimalECTSValue < 0             ||
            this->_maximalECTSValue < 0             ||
            this->_minimalCoursesByTimeFrame < 0    ||
            this->_maximalCoursesByTimeFrame < 0     )
        {
            this->_isConfig = false;
            return this->_isConfig;
        }

        // verify overlaping range
        if( this->_minimalECTSValue > this->_maximalECTSValue ||
            this->_minimalCoursesByTimeFrame > this->_maximalCoursesByTimeFrame)
        {
            throw CSDVPOverlapingBoundariesException(this);
        }

        this->_isConfig = true;
        return this->_isConfig;
    }
// === END FUNC

// === STATIC
    void CSDVP::generateProblem(CSDVP & csdvp, int seed)
    {
        csdvp._seed = seed;

        if(! csdvp.checkConfig() ) //if csdvp is not configurated, aborting generation
            return; //aborting pb generation

        std::cout << "generateProblem TODO" << std::endl;
    }

    int CSDVP::assignID()
    { return ++CSDVP::CSDVP_COUNTER;}