#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <queue>
#include <cassert>

#include "problem.h"
#include "tools.h"

#include "exception/csdvpOverlapingBoundaryException.h"
#include "exception/notImplementedException.h"
#include "exception/csdvpBadlyConfigException.h"

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
        this->_minimalCompetencyByCourse = -1;
        this->_maximalCompetencyByCourse = -1;
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
        void CSDVP::set_cfg_minimalCompetencyByCourse(int nb)
            {this->_minimalCompetencyByCourse = nb;}
        void CSDVP::set_cfg_maximalCompetencyByCourse(int nb)
            {this->_maximalCompetencyByCourse = nb;}
        void CSDVP::set_cfg_minimalPrerequisiteByCourse(int nb)
            {this->_minimalPrerequisiteByCourse = nb;}
        void CSDVP::set_cfg_maximalPrerequisiteByCourse(int nb)
            {this->_maximalPrerequisiteByCourse = nb;}
        void CSDVP::set_cfg_pickedCoursesByTimeFrame(int nb)
            {this->_pickedCoursesByTimeFrame = nb;}
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
        bool CSDVP::addTimeFrame(int tF)
        {
            if(duplicataFlag(this->_timeFrames, tF))
                return false; // NTD

            this->_timeFrames.push_back(tF);
            return true;
        }
        bool CSDVP::addCourseToCatalogue(Course & c)
        {
            if(duplicataFlag(this->_availableCourses, c))
                return false;;

            this->_availableCourses.push_back(c);
            return true;
        }

        bool CSDVP::addCompetencyToCatalogue(Competency & c)
        {
            if(duplicataFlag(this->_availableCompentecies, c))
                return false;
            this->_availableCompentecies.push_back(c);
            return true;
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
            this->_maximalMagnitude.value() < 0     ||
            this->_maximalCompetencyByCourse < 0    ||
            this->_minimalCompetencyByCourse < 0    ||
            this->_minimalPrerequisiteByCourse < 0  ||
            this->_maximalPrerequisiteByCourse < 0  ||
            this->_pickedCoursesByTimeFrame < 1     )
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

        // verify if the pb has enough courses
        if( this->_minimalCoursesByTimeFrame >= this->_quantityAvailableCourses)
            throw CSDVPBadlyConfiguratedException("this->_minimalCoursesByTimeFrame > this->_quantityAvailableCourses");
        if(this->_quantityAvailableCourses < this->_maximalCoursesByTimeFrame)
            throw CSDVPBadlyConfiguratedException("this->_quantityAvailableCourses < this->_maximalCoursesByTimeFrame");
        // IDEM from competency
        if(this->_minimalCompetencyByCourse >= this->_quantityAvailableCompetencies)
            throw CSDVPBadlyConfiguratedException("this->_minimalCompetencyByCourse >= this->_quantityAvailableCompetencies");
        if(this->_quantityAvailableCompetencies < this->_maximalCompetencyByCourse)
            throw CSDVPBadlyConfiguratedException("this->_quantityAvailableCompetencies < this->_maximalCompetencyByCourse");
        if(this->_pickedCoursesByTimeFrame > this->_minimalCoursesByTimeFrame)
            throw CSDVPBadlyConfiguratedException("this->_pickedCoursesByTimeFrame > this->_minimalCoursesByTimeFrame");
        
        this->_isConfig = true;
        return this->_isConfig;
    }

    void CSDVP::_makeCoursesSortedByTF()
    {
        //Init the vector of the size of the time frames
        for(int i = 0; i < this->_timeFrames.size(); i++)
            this->_coursesSortedByTF.push_back(std::vector<Course>());
        
        int tmpIdx;
        for(int i = 0; i < this->_availableCourses.size(); i++)
        {
            for(int j = 0; j < this->_availableCourses.at(i).timeFrame().size(); j++)
            {
                tmpIdx = this->_availableCourses.at(i).timeFrame().at(j) - this->_minimalTimeFrame;
                this->_coursesSortedByTF.at(tmpIdx).push_back(this->_availableCourses.at(i));
            }
        }
    }
    int CSDVP::mapCourseToPosition(const Course & c)
    {
        for(int i = 0; i << this->coursesCatalogue().size(); i++)
            if(c == this->coursesCatalogue().at(i))
                return i;
        return -1;
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

        switch (type)
        {
        case CSDVP::GenerationType::RANDOM:
            _randomlyGenerated(csdvp);
            break;
        default:
            break;
        }
    }

    /// [x;y[ ?
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

        /* COURSES ASSIGNATION
         * First, we create a tmp sized coursed vector.
         * Then, for each timeframe, we randomly pick between [0,vec.size] n courses (n is random in [_minimalCoursesByTimeFrame ; _ maximalCoursesByTimeFrame])
         * Then, _availableCourses is all the courses that have at least one assigned TF in vec.
         */
        std::vector<Course> tmpCourses;
        for(int i = 0; i < pb._quantityAvailableCourses; i++)
        {
            tmpCourses.push_back(Course::build(CSDVP::_randomizeIn(pb.cfg_ectsMin(), pb.cfg_ectsMax())));
        }

        /* We obtain how many courses n by semester s
         * then we create an idxCourses vector of size n * s
         * then we shuffle it
         * then we distribute the course accordingly. If a course already exists, we repick one randomly until it's ok 
         */
        std::vector<int> idxCourses;
        std::vector<int> nbCoursesByTF;
        for(int i = 0 ; i < pb.timeFrames().size(); i++)
            nbCoursesByTF.push_back(CSDVP::_randomizeIn(pb._minimalCoursesByTimeFrame, pb._maximalCoursesByTimeFrame));
        int idxCoursesCounter = 0;
        for(int i = 0; i < nbCoursesByTF.size(); i++)
        {
            for(int j = 0; j < nbCoursesByTF.at(i); j++)
            {
                idxCourses.push_back(idxCoursesCounter % pb._quantityAvailableCourses);
                idxCoursesCounter++;
            }
        }

        std::random_shuffle(idxCourses.begin(), idxCourses.end());
        bool insertRez;
        int rndIdx;
        idxCoursesCounter = 0;
        for(int i = 0; i < pb.timeFrames().size(); i++)
        {
            for(int j = 0; j < nbCoursesByTF.at(i); j++)
            {
                insertRez = true;
                int cc = idxCourses.at(idxCoursesCounter);
                insertRez = tmpCourses.at(idxCourses.at(idxCoursesCounter)).addTemporalFrame(pb.timeFrames().at(i));

                while(!insertRez)//if duplicataProtection (i.e. course already in this semester)
                {
                    rndIdx = CSDVP::_randomizeIn(0, pb._quantityAvailableCourses);
                    insertRez = tmpCourses.at(rndIdx).addTemporalFrame(pb.timeFrames().at(i));
                }
                idxCoursesCounter++;
            }
        }

        // // OLD WAY
        // bool insertRez;
        // for(int i = 0; i < pb.timeFrames().size(); i++)
        // {
        //     int nbCoursesInThisTF = CSDVP::_randomizeIn(pb._minimalCoursesByTimeFrame, pb._maximalCoursesByTimeFrame);
        //     int courseIdx;

        //     std::cout << "In the TF "+std::to_string(i)+" I plan " << std::to_string(nbCoursesInThisTF) << " courses." << std::endl;

        //     for(int j = 0; j < nbCoursesInThisTF; j++)
        //     {
        //         insertRez = true;
        //         courseIdx = CSDVP::_randomizeIn(0,tmpCourses.size()-1);
        //         insertRez = tmpCourses.at(courseIdx).addTemporalFrame(pb.timeFrames().at(i));

        //         if(!insertRez) //If a duplicata has been prevented, we do not count the attempt
        //             j--;
        //     }
        // }

        for(int i = 0; i < tmpCourses.size(); i++)
            if(tmpCourses.at(i).timeFrame().size() > 0)
                pb.addCourseToCatalogue(tmpCourses.at(i));
        
        //From here, coursesCatalogue can still be < to minCourseTF * nbTF (due to the fact that a same course can belongs to )
        pb._makeCoursesSortedByTF();

        /* COMPETENCY CREATION
         * We create _quantityAvailableCompetency competencies. For each comp, we randomly define it's magnitude.
         */
        double magVal;
        for(int i = 0; i < pb.cfg_quantityCompetencies(); i++)
        {

            magVal = pb.cfg_magnitudeMin().value() + ( (double)rand()/RAND_MAX) * ( pb.cfg_magnitudeMax().value() - pb.cfg_magnitudeMin().value()) ;
            assert(magVal >= pb.cfg_magnitudeMin().value());
            assert(magVal <= pb.cfg_magnitudeMax().value());
            
            Competency c = Competency::build(magVal);
            pb.addCompetencyToCatalogue(c);
            assert(c == pb.competencyCatalogue().at(pb.competencyCatalogue().size()-1));
        }
        /* COMPETENCY ASSIGNATION FOR TEACHED
         * For each course c, we roll x, the nb of competencies associated to c.
         * To assign a competency to c exhaustively, we create a tmp competency vector v, where the competencies are randomly sorted, then create a queue from it.
         * Then, it is equal to take the firsts x competencies from the queue (and put them at the end of the queue) for c.
         */ 
        std::vector<Competency> randomVec(pb.competencyCatalogue());
        std::random_shuffle(randomVec.begin(), randomVec.end());
        std::queue<Competency> queue;
        for(int i = 0 ; i < randomVec.size(); i++)
            queue.push(randomVec.at(i));
        
        int x;
        Competency tmpComp;
        std::pair<Competency, double> teachedComp;

        for(int i = 0; i < pb.coursesCatalogue().size(); i++)
        {
            x = _randomizeIn(pb.cfg_competencyByCourseMin(), pb.cfg_competencyByCourseMax());
            for(int j = 0; j < x; j++)
            {
                tmpComp = queue.front();
                queue.pop();queue.push(tmpComp);
                teachedComp = std::pair<Competency,double>(tmpComp, 1.0);
                pb.unlocked_coursesCatalogue().at(i).addTeachedComp(teachedComp);
            }
        }

        /* COMPETENCY ASSIGNATION FOR PREREQ
         * IDEM AS ABOVE
         */
        std::random_shuffle(randomVec.begin(), randomVec.end());
        queue = std::queue<Competency>();
        for(int i = 0 ; i < randomVec.size(); i++)
            queue.push(randomVec.at(i));
        
        for(int i = 0; i < pb.coursesCatalogue().size(); i++)
        {
            x = _randomizeIn(pb.cfg_prerequisiteByCourseMin(), pb.cfg_prerequisiteByCourseMax());
            for(int j = 0; j < x; j++)
            {
                tmpComp = queue.front();
                queue.pop(); queue.push(tmpComp);
                pb.unlocked_coursesCatalogue().at(i).addPrerequisite(tmpComp);
            }
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
        s+="\n\tMagnitude min: "+std::to_string(c.cfg_magnitudeMin().value())+"\n\tMagnitude max: "+std::to_string(c.cfg_magnitudeMax().value());

        Stream << s;
        return Stream;
    }
// === END OPERATOR