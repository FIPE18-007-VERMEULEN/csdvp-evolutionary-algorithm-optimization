#ifndef SRC_PROBLEM_H_
#define SRC_PROBLEM_H_

#include <vector>

#include "course.h"
#include "competency.h"
#include "magnitude.h"

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

        int _minimalTimeFrame;
        int _maximalTimeFrame;
        /// ECTS values, defining the intervale [_minimal,_maximal] for random
        int _minimalECTSValue;
        int _maximalECTSValue;

        int _minimalCoursesByTimeFrame;
        int _maximalCoursesByTimeFrame;

        int _minimalCompetencyByCourse;
        int _maximalCompetencyByCourse;

        int _minimalPrerequisiteByCourse;
        int _maximalPrerequisiteByCourse;

        int _pickedCoursesByTimeFrame;

        Magnitude _minimalMagnitude;
        Magnitude _maximalMagnitude;
        // ---------- END CONFIGURATION ATTRIBUTES ----------

        // ---------- PROBLEM SPECIFIC ATTRIBUTES ----------
        // Theses attributes represent the CSDVP
        std::vector<int> _timeFrames; // Time frames stocks the VALUE, not the index
        std::vector<Course> _availableCourses;
        std::vector<std::vector<Course>> _coursesSortedByTF; //sorted by standard index. e.g. TF[4;6] -> [0]=4; [1]=5 ; [2] = 6
        std::vector<Competency> _availableCompentecies; //The competency's magnitude should not be used here. 

        ///@todo implements a decay politics
        //DecayPolitics 
        // --------- END PROBLEM SPECIFIC ATTRIBUTES ---------

        // === STATIC
        static int CSDVP_COUNTER;
        static int assignID();

        static const int RANDOM_MAX_ECTS_VALUE = 10;
        static const int RANDOM_MAX_QTE_COMPETENCIES = 50;
        static const int RANDOM_MAX_QTE_COURSES = 50;
        static const int RANDOM_MAX_TIME_FRAMES = 15;
        static const int RANDOM_MAX_COURSE_TIME_FRAMES = 10;

        // --------- GENERATION RELATED FUNCTION ---------
        static void _randomlyGenerated(CSDVP & pb);
        
        static int _randomizeIn(const int min, const int max);
        static double _randomizeIn(const double min, const double max);
        // --------- END GENERATION RELATED FUNCTION ---------

        /// It sources _coursesSortedByTF, which is another view of _availableCourses, sorted by TF
        void _makeCoursesSortedByTF();

    public:
        // --------- GENERATION RELATED FUNCTION ---------
        /// allows a random attribution of pb's attributes
        static void randomizeProblem(CSDVP & pb, int seed);
        // --------- END GENERATION RELATED FUNCTION ---------

        // === ENUM
        enum GenerationType
        {
            RANDOM
            //PRESET
        };

        // === CONSTRUCTOR
        CSDVP();
        /**Generate an instance of the CSDVP iff isConfig is true. Thus, seed != -1;
         * @developper each kind of generation must sources correctly the main arrays (_availableCours/comp) TF and _sortedCoursesByTF
         */
        static void generateProblem(CSDVP & csdvp, CSDVP::GenerationType type, int seed= -1 );

        // === GETTER
        const int id() const{return this->_id;}
        const int seed() const{return this->_seed;}
        const int cfg_quantityCompetencies() const{return this->_quantityAvailableCompetencies;}
        const int cfg_quantityCourses() const{return this->_quantityAvailableCourses;}
        const int cfg_minimalTimeFrame() const{return this->_minimalTimeFrame;}
        const int cfg_maximalTimeFrame() const{return this->_maximalTimeFrame;}
        const int cfg_ectsMax() const{return this->_maximalECTSValue;}
        const int cfg_ectsMin() const{return this->_minimalECTSValue;}
        const int cfg_courseByTFMax() const{return this->_maximalCoursesByTimeFrame;}
        const int cfg_courseByTFMin() const{return this->_minimalCoursesByTimeFrame;}
        const int cfg_competencyByCourseMin() const {return this->_minimalCompetencyByCourse;}
        const int cfg_competencyByCourseMax() const {return this->_maximalCompetencyByCourse;}
        const int cfg_prerequisiteByCourseMin() const {return this->_minimalPrerequisiteByCourse;}
        const int cfg_prerequisiteByCourseMax() const {return this->_maximalPrerequisiteByCourse;}
        const int cfg_pickedCoursesByTimeFrame() const {return this->_pickedCoursesByTimeFrame;}
        const Magnitude & cfg_magnitudeMin() const{return this->_minimalMagnitude;}
        const Magnitude & cfg_magnitudeMax() const{return this->_maximalMagnitude;}

        const std::vector<int> & timeFrames() const{return this->_timeFrames;}
        const std::vector<Course> & coursesCatalogue() const{return this->_availableCourses;}
        /**CompetencyCatalogue should be used to know which competency exists in the current pb. However, it should not
         * be used to know the magnitude of each comp in the pb, since these mag changes accordingly to the courses (and if it is teached or a prereq)
         */ 
        const std::vector<Competency> & competencyCatalogue() const{return this->_availableCompentecies;}
        const std::vector<std::vector<Course>> & coursesSortedByTF() const {return this->_coursesSortedByTF;}
        std::vector<int> & unlocked_timeFrames(){return this->_timeFrames;}
        std::vector<Course> & unlocked_coursesCatalogue(){return this->_availableCourses;}
        std::vector<Competency> & unlocked_competenciesCatalogue(){return this->_availableCompentecies;}
        
        const int getQuantityCoursesToPick() const{
            if(this->_isConfig)
                return this->_timeFrames.size() * this->_pickedCoursesByTimeFrame;
            return -1;//if not config
        }
        /** Maps a course into its position inside the this->courseCatalogue().
         * returns the index of the course within the coursesCatalogue [0;size[  ; otherwise return -1 if the course is not found.
         */
        int mapCourseToPosition(const Course & c);
        ///@todo getDecayPolitic

        // === MUTATOR
            // SETTER
            /*setSeed is deactivated. The only way to attribute a seed to CSDVP is by generateProblem.*/
            //void setSeed(int s);
            void set_cfg_quantityCompetencies(int nb);
            void set_cfg_quantityCourses(int nb);
            void set_cfg_minimalTimeFrames(int nb);
            void set_cfg_maximalTimeFrames(int nb);
            void set_cfg_ectsMin(int nb);
            void set_cfg_ectsMax(int nb);
            void set_cfg_courseByTFMax(int nb);
            void set_cfg_courseByTFMin(int nb);
            void set_cfg_minimalMagnitude(double mag);
            void set_cfg_maximalMagnitude(double mag);
            void set_cfg_minimalCompetencyByCourse(int nb);
            void set_cfg_maximalCompetencyByCourse(int nb);
            void set_cfg_minimalPrerequisiteByCourse(int nb);
            void set_cfg_maximalPrerequisiteByCourse(int nb);
            void set_cfg_pickedCoursesByTimeFrame(int nb);

            void setTimeFrames(std::vector<int> & v);
            void setCoursesCatalogue(std::vector<Course> &);
            void setCompetenciesCatalogue(std::vector<Competency> &);
            // ADDER
            //Any adder has a duplicata protection. Tend to favor addX instead of setX to prevent duplicata
            /**Returns true if the element has been inserted. Returns false if the insertion has not been performed to prevent a duplicata*/
            bool addTimeFrame(int t);
            bool addCourseToCatalogue(Course & c);
            bool addCompetencyToCatalogue(Competency & c);

        // === FUNC
        /// Checks all configuration attributes. If they have been all set, then isConfig is set to true
        bool checkConfig();
};
std::ostream & operator<<(std::ostream & Stream, const CSDVP & c);



#endif // SRC_PROBLEM_H_