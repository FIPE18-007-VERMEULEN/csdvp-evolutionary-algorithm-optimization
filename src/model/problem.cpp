#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>

#include "problem.h"

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

            // ADDER

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

        this->_isConfig = true;
        return this->_isConfig;
    }
// === END FUNC

// === STATIC
    void CSDVP::generateProblem(CSDVP & csdvp, int seed)
    {
        csdvp._seed = seed;

        if(! csdvp.checkConfig() ) //if csdvp is not configurated, aborting generation
            return;

        std::cout << "generateProblem TODO" << std::endl;
    }

    int CSDVP::assignID()
    { return ++CSDVP::CSDVP_COUNTER;}