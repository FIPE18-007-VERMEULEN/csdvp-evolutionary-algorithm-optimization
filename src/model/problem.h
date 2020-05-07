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

        /// Seed used to generate the problem
        int _seed;
        /// Use to determine if CSDVP has been config by the user.
        bool _isConfig;

        // ----------- CONFIGURATION ATTRIBUTES ----------
        // Config attributes are used to generate a problem instance. This way, two differents problems can be generated within the same definition space. Random is made by _seed.

        int _quantityAvailableCompetencies;
        int _quantityAvailableCourses;

        int _quantityOfTimeFrame;
        /// ECTS values, defining the intervale [_minimal,_maximal] for random
        int _minimalECTSValue;
        int _maximalECTSValue;

        int _minimalCoursesByTimeFrame;
        int _maximalCoursesByTimeFrame;
        // ---------- END CONFIGURATION ATTRIBUTES ----------

        // ---------- PROBLEM SPECIFIC ATTRIBUTES ----------
        // Theses attributes represent the CSDVP
        std::vector<int> _timeFrames;
        std::vector<Course> _availableCourses;
        std::vector<Competency> _availableCompentecies;

        ///@todo implements a decay politics
        //DecayPolitics 
        // --------- END PROBLEM SPECIFIC ATTRIBUTES ---------

        // Static
        static int CSDVP_COUNTER;
        static int assignID();

    public:
        // === CONSTRUCTOR
        CSDVP();
        /// Generate an instance of the CSDVP iff isConfig is true. Thus, seed != -1;
        void static generateProblem(CSDVP & csdvp, int seed= -1 );

        // === GETTER
        const int id() const{return this->_id;}
        const int seed() const{return this->_seed;}
        const int cfg_quantityCompetencies() const{return this->_quantityAvailableCompetencies;}
        const int cfg_quantityCourses() const{return this->_quantityAvailableCourses;}
        const int cfg_quantityTimeFrames() const{return this->_quantityOfTimeFrame;}
        const int cfg_ectsMax() const{return this->_maximalECTSValue;}
        const int cfg_ectsMin() const{return this->_minimalECTSValue;}
        const int cfg_courseByTFMax() const{return this->_maximalCoursesByTimeFrame;}
        const int cfg_courseByTFMin() const{return this->_minimalCoursesByTimeFrame;}

        const std::vector<int> timeFrames() const{return this->_timeFrames;}
        const std::vector<Course> coursesCatalogue() const{return this->_availableCourses;}
        const std::vector<Competency> competencyCatalogue() const{return this->_availableCompentecies;}
        std::vector<int> unlocked_timeFrames(){return this->_timeFrames;}
        std::vector<Course> unlocked_coursesCatalogue(){return this->_availableCourses;}
        std::vector<Competency> unlocked_competenciesCatalogue(){return this->_availableCompentecies;}
        ///@todo getDecayPolitic

        // === MUTATOR
            // SETTER
            /*setSeed is deactivated. The only way to attribute a seed to CSDVP is by generateProblem.*/
            //void setSeed(int s);
            void set_cfg_quantityCompetencies(int nb);
            void set_cfg_quantityCourses(int nb);
            void set_cfg_quantityTimeFrames(int nb);
            void set_cfg_ectsMin(int nb);
            void set_cfg_ectsMax(int nb);
            void set_cfg_courseByTFMax(int nb);
            void set_cfg_courseByTFMin(int nb);

            void setTimeFrames(std::vector<int> & v);
            void setCoursesCatalogue(std::vector<Course> &);
            void setCompetenciesCatalogue(std::vector<Competency> &);
            // ADDER
            //Any adder has a duplicata protection. Tend to favor addX instead of setX to prevent duplicata
            void addTimeFrame(int t);
            void addCourseToCatalogue(Course & c);
            void addCompetencyToCatalogue(Competency & c);

        // === FUNC
        /// Checks all configuration attributes. If they have been all set, then isConfig is set to true
        bool checkConfig();
};


#endif // SRC_PROBLEM_H_