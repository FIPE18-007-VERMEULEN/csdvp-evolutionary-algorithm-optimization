#ifndef DEF_COURSE
#define DEF_COURSE

#include <vector>

#include <competency.h>

/**
 * Represents a course in an academic structure.
 * 
 * Based on a non exhaustive WiP standard from APACHES project. It includes several element of a french course teached in universities.
 * This representation is subject to change.
 * 
 * @param diffusionWeight The weight each {@link teached} competencies are transfered to the student (presumably from a teacher).
 * @author alexis.lebis
 * @version 1 
 */
class Course
{
    private:
        // Competencies related
        std::vector<Competency> prerequisites;
        std::vector<Competency> teached;
        std::vector<double> diffusionWeight;
        
        // Other
        int ects;
        std::vector<int> temporalAvailability;

        // Static
        static int COURSE_COUNTER;
    public:
        Course();
};




#endif