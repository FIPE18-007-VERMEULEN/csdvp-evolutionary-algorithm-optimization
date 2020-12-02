#ifndef SRC_COMPETENCY_DISTRIBUTION_H_
#define SRC_COMPETENCY_DISTRIBUTION_H_

#include <vector>

#include "problem.h"
#include "competency.h"

/**
 * Represents the distribution of prerequisites from competencies.
 *  
 * This is used to implement some hierarchy between competencies.
 * CompetencyDistribution must be called after the competencies has been created within the problem
 * and preferably before the compentecies have been affected to the courses.
 * 
 * @author alexis.lebis
 * @version 1
 */
class CompetencyDistribution
{
    private:
        static bool hlevelSanitized;

    public:
        static std::vector<int> HLEVEL;

        void linearDistribution(CSDVP &);
        /**
         * distribute realises the competency distribution according to the HLEVEL
         */ 
        void distribute(CSDVP &);

        // === STATIC
        static int HLevelRange(CSDVP &); //return the range max (starting from 0) of the HL
        static std::vector<Competency> getHLevel(CSDVP &, int); // returns all the competency of a given HL
        static std::vector<Competency> upToHLevel(CSDVP &, int); // retrieves all the comp comprised between [0;int] (and NOT [0;int[)
        /*
         * sanitizeHLEVEL performs two task:
         * * it verifies that the sum of all HLEVEL is equal to 100
         * * it resize the HLEVEL array, eliminating all unasigned value
         */
        static void sanitizeHLEVEL();
        static void displayHLevel();
};


#endif // SRC_COMPETENCY_DISTRIBUTION_H_