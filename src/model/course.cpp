#include <iostream>
#include <utility>
#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "course.h"
#include "competency.h"
#include "tools.h"

#include "exception/courseECTSException.h"
#include "exception/courseTemporalFrameException.h"
#include "exception/notImplementedException.h"
#include "exception/idOverflowException.h"

int Course::COURSE_COUNTER = 0;
int Course::COURSE_TMP_COUNTER = ID_RANGE_FOR_OBJECT + 1;

// === FACTORY
Course Course::build(int ects, std::string name)
{
    int id = Course::assignID();
    
    if(name.empty())
        name = "Course#"+std::to_string(id);
    
    if(ects < 0)
        throw CourseECTSException(new Course(id, ects, name));
    else
        return Course(id, ects, name);
}

Course Course::buildTMP(int ects, std::string name)
{
    int id = Course::assignID4TMP();
    
    if(name.empty())
        name = "Course#"+std::to_string(id);
    
    if(ects < 0)
        throw CourseECTSException(new Course(id, ects, name));
    else
        return Course(id, ects, name);
}
// === END FACTORY

// === CONSTRUCTOR
Course::Course(int id, int ects, std::string name)
    : _id(id), _ects(ects), _name(name)
{}

// === END CONSTRUCTOR

// === GETTER
    //cf. course.h
// === END GETTER

// === MUTATOR
    // SETTER
    void Course::setECTS(int ects)
    {
        this->_ects = ects;

        if(this->_ects < 0)
            throw CourseECTSException(this);
    }

    void Course::setName(std::string name)
    {
        if(name.empty())
            name = "Course#"+std::to_string(this->_id);
        this->_name = name;
    }
    void Course::setAvailabilities(std::vector<int> & times)
    {this->_temporalAvailability = times;}
    void Course::setPrerequisites(std::vector<Competency> & prereqs)
    {this->_prerequisites = prereqs;}
    void Course::setTeachedComps(std::vector<std::pair<Competency,double>> & wComps)
    {this->_weightedTeached = wComps;}

    // ADDER
    bool Course::addTeachedComp(std::pair<Competency,double> & wComp)
    {
        if( _duplicataProtection(&(this->_weightedTeached), wComp.first))
            return false;
        this->_weightedTeached.push_back(wComp);
        return true;
    }
    bool Course::addPrerequisite(Competency & prereq)
    {
        if( duplicataFlag((this->_prerequisites), prereq) )
            return false;
        this->_prerequisites.push_back(prereq);
        return true;
    }
    bool Course::addTemporalFrame(int time)
    {
        if(time < 0)
            throw CourseTemporalFrameException(this, time);
        if( duplicataFlag((this->_temporalAvailability), time) )
            return false;
        this->_temporalAvailability.push_back(time);
        return true;
    }

    // DELETER
    Competency * Course::rmPrerequisite(Competency prereq)
    {
        std::vector<Competency>::iterator it = 
            std::find(
                this->_prerequisites.begin(),
                this->_prerequisites.end(),
                prereq
            );
        if(it == this->_prerequisites.end())
            return NULL;
        
        int index = std::distance(this->_prerequisites.begin(), it);
        
        Competency * c = &(this->_prerequisites.at(index));
        this->_prerequisites.erase(this->_prerequisites.begin()+index);
        return c;
    }

    std::pair<Competency,double> * Course::rmTeachedComp(Competency teached)
    {
        if(this->_weightedTeached.size()==0)
            return NULL;
        
        std::vector<std::pair<Competency, double>>::iterator it = this->_weightedTeached.begin();
        
        int index = 0;

        Competency current = this->_weightedTeached.at(index).first;

        while(it != this->_weightedTeached.end() && !( this->_weightedTeached.at(index).first == teached) )
        {
            index=std::distance(this->_weightedTeached.begin(), it);
            current = this->_weightedTeached.at(index).first;
            it++;
        }

        if(it == this->_weightedTeached.end() && !( this->_weightedTeached.at(index).first == teached))
            return NULL;
        
        std::pair<Competency, double> * ptr_pair = &(this->_weightedTeached.at(index));
        this->_weightedTeached.erase(this->_weightedTeached.begin()+index);
        return ptr_pair;
    }
    int * Course::rmTemporalFrameIndex(int index)
    {
        int *ptr_time = & (this->_temporalAvailability.at(index)) ;

        this->_temporalAvailability.erase(this->_temporalAvailability.begin()+index);
        return ptr_time;
    }

    int * Course::rmTemporalFrameValue(int value)
    {
        std::vector<int>::iterator it = 
            std::find(
                this->_temporalAvailability.begin(),
                this->_temporalAvailability.end(),
                value
            );
        if(it == this->_temporalAvailability.end())
            return NULL;
        
        int index = std::distance(this->_temporalAvailability.begin(), it);
        int * ptr_time = &(this->_temporalAvailability.at(index));
        this->_temporalAvailability.erase(this->_temporalAvailability.begin()+index);
        return ptr_time;
    }
// === END MUTATOR


// === FUNC
/// @deprecated
// bool Course::_duplicataProtection(std::vector<int> * timeFrame, int time)
// {
//     std::vector<int>::iterator it = std::find( timeFrame->begin(), timeFrame->end(), time);

//     return it != timeFrame->end();
// }

/// @deprecated
// bool Course::_duplicataProtection(std::vector<Competency> * prereq, Competency c)
// {
//     std::vector<Competency>::iterator it = std::find( prereq->begin(), prereq->end(), c);
//     return it != prereq->end();
// }

bool Course::_duplicataProtection(std::vector<std::pair<Competency,double>> *teached, Competency c)
{
    if(teached->size() == 0)
        return false;
    
    std::vector<std::pair<Competency, double>>::iterator it = teached->begin();
    int index = 0;
    Competency current = teached->at(index).first;

    while(it != teached->end() && !( teached->at(index).first == c) )
    {
        index=std::distance(teached->begin(), it);
        current = teached->at(index).first;
        it++;
    }

    return ( it != teached->end() && !( teached->at(index).first == c) );
}

// template<typename T>
// std::pair<int, T> Course::findInVector(const std::vector<T> & vec, const T & findMe)
// {
//     std::pair<int, T> res;

//     typename std::vector<T>::iterator it = std::find( vec.begin(), vec.end(), findMe);
//     if(it == vec.end())
//     {
//         res.first = -1;
//         res.second; //NTD, -1 SHOULD BE USED TO DETECT THAT NOTHING HAS BEEN FOUND
//     }
//     else
//     {
//         res.first = std::distance(vec.begin(), it);
//         res.second = vec.at(res.first);
//     }

//     return res;
    
// }

bool Course::_lazyEquality(const Course & c) const
{
    return ( (this->_id == c.id() ||
            this->_name.compare(c.name()) == 0 ) &&
            this->_ects == c.ects()
            );
}

///@todo
bool Course::_fullEquality(const Course & c) const
{
    throw NotImplementedException("Course::_fullEquality");
}

// === OPERATOR
std::ostream& operator<<(std::ostream& Stream, const Course & c)
{
    std::string s = "Course\n\tid:"+std::to_string(c.id())+"\n\tname:"+c.name()+"\n\tECTS: "+std::to_string(c.ects());
    std::string tf;
    if(c.timeFrame().size() > 0)
    {
        for(int i = 0; i < c.timeFrame().size()-1; i++)
            tf+=std::to_string(c.timeFrame().at(i))+" ; ";
        tf+=std::to_string(c.timeFrame().at(c.timeFrame().size()-1));
        s+="\n\tTimeFrames: ["+tf+"]";
    }

    s+="\n\tRequirement: [";
    for(int i = 0; i < c.prerequisites().size(); i++)
    {
        s+="" + c.prerequisites().at(i).c_name()+ "("+ std::to_string(c.prerequisites().at(i).c_magnitude().value()) + ") ; ";
    }
    s+="]";
    s+="\n\tTeaches: [";
    for(int i = 0 ; i < c.teachedCompetenciesWeighted().size(); i++)
    {
        s+= "" + c.teachedCompetenciesWeighted().at(i).first.c_name() + "("+ std::to_string(c.teachedCompetenciesWeighted().at(i).first.c_magnitude().value())+") ; ";
    }
    s+="]";
    Stream << s;
    return Stream;
}

bool Course::operator==(const Course & c) const
{
    return _lazyEquality(c);
}
// === END OPERATOR


// === STATIC
/// Course counter
int Course::assignID()
{
    if(Course::COURSE_COUNTER + 1 > ID_RANGE_FOR_OBJECT)
        throw idOverflowException("assignID()@Course.cpp");
    return ++Course::COURSE_COUNTER;
}

int Course::assignID4TMP()
{
    if(Course::COURSE_TMP_COUNTER + 1 > ID_RANGE_FOR_TEMPORARY_OBJECT)
    {
        std::cout << "INFO: COURSE_TMP_COUNTER was about to overflow: restored to ID_RANGE_OBJECT + 1" << std::endl;
        COURSE_TMP_COUNTER = ID_RANGE_FOR_OBJECT + 1;
    }
    return ++COURSE_TMP_COUNTER;
}

/** It produces a new vector, which is another view of courses in param, sorted by TF
 * Duplicates the behaviour of CSDVP::_makeCoursesSortedByTF() except that it applies to any vector of courses
 * size of timeFrames must be equal to the maximal TF value in courses.timeFrames !
 */ 
std::vector<std::vector<Course>> Course::organiseByTF(std::vector<Course> courses, std::vector<int> timeFrames)
{
    std::vector<std::vector<Course>> coursesByTF(timeFrames.size());
    
    int tmpIdx;
    for(int i = 0; i < courses.size(); i++)
    {
        for(int j = 0; j < courses.at(i).timeFrame().size(); j++)
        {
            tmpIdx = courses.at(i).timeFrame().at(j) - timeFrames.at(0);
            coursesByTF.at(tmpIdx).push_back(courses.at(i));
        }
    }

    return coursesByTF;
}
// === END STATIC