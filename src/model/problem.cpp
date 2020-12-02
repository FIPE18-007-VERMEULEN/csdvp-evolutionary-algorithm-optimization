#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <queue>
#include <cassert>
#include <tuple>

#include "problem.h"
#include "tools.h"
#include "competencyDistribution.h"

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
            {this->_availableCourses = c;}
        void CSDVP::setCompetenciesCatalogue(std::vector<Competency> & c)
            {this->_availableCompentecies = c;}
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
        if(this->_pickedCoursesByTimeFrame * ( (this->_maximalTimeFrame - this->_minimalTimeFrame) + 1) > this->_quantityAvailableCourses)
            throw CSDVPBadlyConfiguratedException("this->_pickedCoursesByTimeFrame * ( (this->_maximalTimeFrame - this->_minimalTimeFrame) + 1) > this->_quantityAvailableCourses");
        this->_isConfig = true;
        return this->_isConfig;
    }

    void CSDVP::_makeCoursesSortedByTF()
    {
        //Init the vector of the size of the time frames
        for(unsigned int i = 0; i < this->_timeFrames.size(); i++)
            this->_coursesSortedByTF.push_back(std::vector<Course>());
        
        int tmpIdx;
        for(unsigned int i = 0; i < this->_availableCourses.size(); i++)
        {
            for(unsigned int j = 0; j < this->_availableCourses.at(i).timeFrame().size(); j++)
            {
                tmpIdx = this->_availableCourses.at(i).timeFrame().at(j) - this->_minimalTimeFrame;
                this->_coursesSortedByTF.at(tmpIdx).push_back(this->_availableCourses.at(i));
            }
        }
    }
    int CSDVP::mapCourseToPosition(const Course & c)
    {
        for(unsigned int i = 0; i < this->coursesCatalogue().size(); i++)
            if(c == this->coursesCatalogue().at(i))
                return i;
        return -1;
    }

    int CSDVP::mapCompToPosition(const Competency & comp)
    {
        for(unsigned int i = 0; i < this->competencyCatalogue().size();i++)
        {
            if(comp == this->competencyCatalogue().at(i))
                return i;
        }
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

    // double CSDVP::_randomizeIn(const double min, const double max)
    // {
    //     throw NotImplementedException("CSDVP::_randomizeIn");
    // }

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
        for(unsigned int i = 0 ; i < pb.timeFrames().size(); i++)
            nbCoursesByTF.push_back(CSDVP::_randomizeIn(pb._minimalCoursesByTimeFrame, pb._maximalCoursesByTimeFrame));
        int idxCoursesCounter = 0;
        for(unsigned int i = 0; i < nbCoursesByTF.size(); i++)
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
        for(unsigned int i = 0; i < pb.timeFrames().size(); i++)
        {
            for(int j = 0; j < nbCoursesByTF.at(i); j++)
            {
                insertRez = true;
                insertRez = tmpCourses.at(idxCourses.at(idxCoursesCounter)).addTemporalFrame(pb.timeFrames().at(i));

                while(!insertRez)//if duplicataProtection (i.e. course already in this semester)
                {
                    rndIdx = CSDVP::_randomizeIn(0, pb._quantityAvailableCourses - 1 );
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

        for(unsigned int i = 0; i < tmpCourses.size(); i++)
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

        /* Creating _distributedCompetencies array for assignation */
        for(int i = 0; i < pb.competencyCatalogue().size(); i++)
        {
            pb._distributedCompetencies.push_back(-1);
        }

        assert (pb._distributedCompetencies.size() == pb.competencyCatalogue().size());

        /* Assigning Hierachy Level (HL) for each comp
         * HL is used to improve the average quality of the course catalogue compared to random
         */
        CompetencyDistribution distr = CompetencyDistribution();
        //distr.linearDistribution(pb);
        distr.distribute(pb);

        /* COMPETENCY ASSIGNATION FOR TEACHED
         * For each course c, we roll x, the nb of competencies associated to c.
         * To assign a competency to c exhaustively, we create a tmp competency vector v, where the competencies are randomly sorted, then create a queue from it.
         * Then, it is equal to take the firsts x competencies from the queue (and put them at the end of the queue) for c.
         */ 
        std::vector<Competency> randomVec(pb.competencyCatalogue());
        std::random_shuffle(randomVec.begin(), randomVec.end());
        std::queue<Competency> queue;
        for(unsigned int i = 0 ; i < randomVec.size(); i++)
            queue.push(randomVec.at(i));
        /*
         * ADDENDUM:
         * We use the HierarchyLevel to condition the assignation of competency.
         * A course at level i (defined by its biggest TF) cannot have a comp with HL > i*HLrange / NbTF 
         */
        int lastTF;
        int nbTF = pb.timeFrames().size();
        int hLevelR = CompetencyDistribution::HLevelRange(pb);
        int maxLevel; //used to identify which HL are authorized;
        std::vector<Competency> HLComp;

        int x;
        Competency tmpComp;
        std::pair<Competency, double> teachedComp;

        for(unsigned int i = 0; i < pb.coursesCatalogue().size(); i++)
        {
            x = _randomizeIn(pb.cfg_competencyByCourseMin(), pb.cfg_competencyByCourseMax());
            lastTF = pb.coursesCatalogue().at(i).lastTimeFrame();
            maxLevel = lastTF * hLevelR / nbTF;
            HLComp = CompetencyDistribution::upToHLevel(pb, maxLevel);
            std::random_shuffle(HLComp.begin(), HLComp.end());

            std::cout << "SIZE OF HLCOMP : " << HLComp.size() << std::endl;

            for(int j = 0; j < x && HLComp.size() > 0 && j < HLComp.size(); j++)
            {
                // tmpComp = queue.front();
                tmpComp = HLComp.at(j);
                pb._sourceCourseTeachedComp(pb, i, tmpComp);
            }
        }

        /* COMPETENCY ASSIGNATION FOR PREREQ
         * IDEM AS ABOVE
         */
        /*
         * ADDENDUM :
         * Specific behavior for cour level 0: no prereq
         * Prereq for level i only draw from HL-1
         * 
         * WARNING :
         * Such a behavior induces a strong hypothesis on how courses draw their comp!
         * 
         */
        /*
         * NEXT TO DO :
         * Array of boolean : true comp assignée ; évolution un tableau de -1 si pas assigné et [0;1] pour les magn des comp à chaque case puis assigné decay sur l'ensemble du tab -> utile pour borner les mag max du job
         * POur prereq juste prendre les comp a true
         * +
         * Vecteur pour le HLevel en entree en % [20,30,10,10,30] (x cases) a passer en dur
         * +
         * Comp prereq metric comme job metric continue
         */
        std::random_shuffle(randomVec.begin(), randomVec.end());
        queue = std::queue<Competency>();
        for(unsigned int i = 0 ; i < randomVec.size(); i++)
            queue.push(randomVec.at(i));
        
        for(unsigned int i = 0; i < pb.coursesCatalogue().size(); i++)
        {
            x = _randomizeIn(pb.cfg_prerequisiteByCourseMin(), pb.cfg_prerequisiteByCourseMax());
            lastTF = pb.coursesCatalogue().at(i).lastTimeFrame();
            maxLevel = lastTF * hLevelR / nbTF;
            HLComp = CompetencyDistribution::upToHLevel(pb, maxLevel-1);
            std::random_shuffle(HLComp.begin(), HLComp.end());

            if(x == 0)
                std::cout << "X is 0! for " << pb.coursesCatalogue().at(i).name() << std::endl;
            if(HLComp.size() == 0)
                std::cout << "HLComp size is 0! for " << pb.coursesCatalogue().at(i).name() << std::endl;

            for(int j = 0; j < x && HLComp.size() > 0 && j < HLComp.size(); j++)
            {
                tmpComp = HLComp.at(j);
                //we change mag value for prereq
                magVal = pb.cfg_magnitudeMin().value() + ( (double)rand()/RAND_MAX) * ( pb.cfg_magnitudeMax().value() - pb.cfg_magnitudeMin().value()) ;
                Competency cpt = Competency::build(magVal,tmpComp.c_name());
                pb.unlocked_coursesCatalogue().at(i).addPrerequisite(cpt);
                queue.push(tmpComp);
            }
        }

    }

    void CSDVP::_sourceCourseTeachedComp(CSDVP & pb, unsigned int idx, Competency & c)
    {
        double magVal;
        magVal = pb.cfg_magnitudeMin().value() + ( (double)rand()/RAND_MAX) * ( pb.cfg_magnitudeMax().value() - pb.cfg_magnitudeMin().value()) ;
        Competency cpt = Competency::build(magVal,c.c_name());
        std::pair<Competency, double> teachedComp = std::pair<Competency,double>(cpt, 1.0);
        pb.unlocked_coursesCatalogue().at(idx).addTeachedComp(teachedComp);
        _updateDistribComp(pb, cpt);
    }

    void CSDVP::_updateDistribComp(CSDVP & pb, Competency & cpt)
    {
        int idx = pb.mapCompToPosition(cpt);
        assert (idx >= 0);

        if(pb._distributedCompetencies.at(idx) == -1) //not yet assigned? Then first 0
            pb._distributedCompetencies.at(idx) = 0;
        pb._distributedCompetencies.at(idx) += cpt.magnitude().value();
        
        // Overflow in distributedCompetencies is not an issue since it tracks how many magnitude has been put into a comp
        // if(pb._distributedCompetencies.at(idx) > 1) //if range overflow --> rebase to one
        //     pb._distributedCompetencies.at(idx) = 1;
    }
    // --------- END GENERATION RELATED FUNCTIONS ---------

// === END STATIC

void const CSDVP::displayDistribution(){
    std::cout << "------------------------------" << std::endl;
    std::cout << "| PB Competency distribution |" << std::endl;
    std::cout << "------------------------------" << std::endl;

    std::tuple<int, int, double, double> stats = this->distributionStats();
    std::cout << "Stats: " << std::endl ;
    std::cout << "\n\t# of Comp: " << this->_distributedCompetencies.size();
    std::cout << "\n\t# of Unassigned: " << std::get<0>(stats);
    std::cout << "\n\t# of above 0.5: " << std::get<1>(stats);
    std::cout << "\n\tDistrib mean: " << std::get<2>(stats);
    std::cout << "\n\tDistrib median: " << std::get<3>(stats) << std::endl;

    for(int i = 0; i < CompetencyDistribution::HLEVEL.size(); i++)
    {
        std::cout << "\tAssigned in HL#" << i <<": " << CompetencyDistribution::getHLevel(*this, i).size() << std::endl;
    }

    std::cout << "Distrib:" << std::endl;
    std::cout << "[";
    for(int i = 0; i < this->_distributedCompetencies.size(); i++)
    {
        std::cout << this->_distributedCompetencies.at(i);
        if(i < this->_distributedCompetencies.size() -1 )
            std::cout << "|";
    }
    std::cout << "]" << std::endl;
    std::cout << "------------------------------" << std::endl << std::endl;
}

std::tuple<int, int, double, double> CSDVP::distributionStats()
{
    std::tuple<int, int, double, double> stats;
    int unassigned = 0;
    int aboveFive = 0;// >= 0.5
    double mean = 0;
    double median = 0;

    for(int i = 0; i < this->_distributedCompetencies.size(); i++)
    {
        if(this->_distributedCompetencies.at(i) == -1)
            unassigned++;
        else
        {
            mean += this->_distributedCompetencies.at(i);
            if(this->_distributedCompetencies.at(i) >= 0.5)
                aboveFive++;
        }
    }

    mean = mean / ( (double)(this->_distributedCompetencies.size() - unassigned));

    std::get<0>(stats) = unassigned;
    std::get<1>(stats) = aboveFive; 
    std::get<2>(stats) = mean; //todo
    std::get<3>(stats) = -1; //todo
    return stats;
}

// === OPERATOR
    std::ostream & operator<<(std::ostream & Stream, const CSDVP & c)
    {
        std::string s = "--------------\n| Problem n°"+std::to_string(c.id())+"|\n--------------\n| Configuration:";
        s+= "\n|\tseed: "+std::to_string(c.seed())+"\n|\tNb comp: "+std::to_string(c.cfg_quantityCompetencies())+"\n|\tNb courses: "+std::to_string(c.cfg_quantityCourses())+"\n|\tMin TimeF: "+std::to_string(c.cfg_minimalTimeFrame())+"\n|\tMax TimeF: "+std::to_string(c.cfg_maximalTimeFrame());
        s+= "\n|\tECTS Min: "+std::to_string(c.cfg_ectsMin())+"\n|\tECTS Max: "+std::to_string(c.cfg_ectsMax())+"\n|\tCourse by TF min: "+std::to_string(c.cfg_courseByTFMin())+"\n|\tCourse by TF max: "+std::to_string(c.cfg_courseByTFMax());
        s+="\n|\tMagnitude min: "+std::to_string(c.cfg_magnitudeMin().value())+"\n|\tMagnitude max: "+std::to_string(c.cfg_magnitudeMax().value());
        s+= "\n| Detail:\n";

        Stream << s;

        std::vector<Course> courses = c.coursesCatalogue();
        for(int i = 0; i < courses.size(); i++)
            Stream << courses[i] << "\n";

        Stream << "===Competencies:";
        std::vector<Competency> comp = c.competencyCatalogue();
        for(int i = 0; i < comp.size(); i++)
            Stream << comp[i] << "\n";
       
        return Stream;
    }
// === END OPERATOR