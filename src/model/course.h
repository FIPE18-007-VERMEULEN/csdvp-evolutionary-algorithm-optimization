#ifndef SRC_COURSE_H_
#define SRC_COURSE_H_

#include <vector>
#include <utility>

#include "competency.h"

/**
 * Represents a course in an academic structure.
 * 
 * Based on a non exhaustive WiP standard from APACHES project. It includes several element of a french course teached in universities.
 * This representation is subject to change.
 * 
 * @author alexis.lebis
 * @version 1 
 */
class Course
{
    private:
        // === COMPETENCIES RELATED
        
        std::vector<Competency> _prerequisites;

        /** weightTeached represents how a compentency i is teached. This means that a competency i is transfered to a student with a weight w (a double). A course has a vector of competency, each one with a specific weight.
         */
        std::vector<std::pair<Competency,double>> _weightedTeached;
        // std::vector<Competency> teached;
        // std::vector<double> diffusionWeight;
        
        // === OTHER
        /// European Credit Transfer and Accumulation System
        int _ects;
        /** Timeframe availablity of a course.
         *  @note For now, it discretiez time into semester
         *  @version 1.0
         */ 
        std::vector<int> _temporalAvailability;
        std::string _name;
        int _id;

        // Static
        static int COURSE_COUNTER;
        static int assignID();

        // Constructor
        //Course();
        Course(int id, int ects, std::string name);
    public:
        static Course build(int ects = 0, std::string name = "");

        // === GETTER
        const int id() const{return this->_id;};
        const std::string name() const{return this->_name;};
        const int ects() const{return this->_ects;}
        const std::vector<Competency> prerequisites() const {return this->_prerequisites;}

        // === MUTATOR
            // SETTER
            void setECTS(int ects);
            void setName(std::string);
            void setAvailabilities(std::vector<int>);
            void setPrerequisites(std::vector<Competency>);
            void setTeachedComps(std::vector<std::pair<Competency,double>>);

            // ADDER
            void addTeachedComp(std::pair<Competency,double>);
            void addPrerequisite(Competency);
            void addTemporalFrame(int);

            // DELETER
            /// rmPrerequisite return a pointer to the removed competency from the _prerequisite, or NULL if not found.
            Competency * rmPrerequisite(Competency);
            /// rmTeachedComp removes the given Competency. It returns a pointer to the std::pair<Competency,double> removed in _weightedTeached, or NULL.
            std::pair<Competency,double> * rmTeachedComp(Competency);
            /// rmTemporalFrame removes the temporal frame stored at the index. It returns the value or NULL;
            int* rmTemporalFrameIndex(int index);
            int* rmTemporalFrameValue(int value);
        
        // === FUNC
            void fixECTS();

};

// === OPERATOR
std::ostream & operator<<(std::ostream& Stream, const Course & c);



#endif // SRC_COURSE_H_