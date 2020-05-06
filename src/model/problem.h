#ifndef SRC_PROBLEM_H_
#define SRC_PROBLEM_H_

#include <vector>

#include "course.h"
#include "competency.h"

/**
 * Model of the CSDVP (Constraint Satisfaction Decaying Variables Problem).
 * 
 * How many compentecies are available, how many courses, etc...
 */ 

class CSDVP
{
    private:
        int _id;

        int _quantityAvailableCompetencies;
        std::vector<Competency> _availableCompentecies;

        int _quantityAvailableCourses;
        std::vector<Course> _availableCourses;

        /// ECTS values, defining the intervale [_minimal,_maximal] for random
        int _minimalECTSValue;
        int _maximalECTSValue;

        int _minimalCoursesByTimeFrame;
        int _maximalCoursesByTimeFrame;

        int _numberOfTimeFrame;
        std::vector<int> _timeFrames;

        ///@todo implements a decay politics
        //DecayPolitics 

        // Static
        static int PROBLEM_COUNTER;
        static int assignID();

    public:
        CSDVP();
};


#endif // SRC_PROBLEM_H_