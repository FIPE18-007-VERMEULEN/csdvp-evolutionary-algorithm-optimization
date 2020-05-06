#include <iostream>
#include <utility>
#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "course.h"
#include "competency.h"

#include "exception/courseECTSException.h"
#include "exception/courseTemporalFrameException.h"

int Course::COURSE_COUNTER = 0;

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
    void Course::addTeachedComp(std::pair<Competency,double> & wComp)
    {
        if( _duplicataProtection(&(this->_weightedTeached), wComp.first))
            return;
        this->_weightedTeached.push_back(wComp);
    }
    void Course::addPrerequisite(Competency & prereq)
    {
        if( _duplicataProtection(&(this->_prerequisites), prereq) )
            return;
        this->_prerequisites.push_back(prereq);
    }
    void Course::addTemporalFrame(int time)
    {
        if(time < 0)
            throw CourseTemporalFrameException(this, time);
        if( _duplicataProtection(&(this->_temporalAvailability), time) )
            return;
        this->_temporalAvailability.push_back(time);
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
bool Course::_duplicataProtection(std::vector<int> * timeFrame, int time)
{
    std::vector<int>::iterator it = std::find( timeFrame->begin(), timeFrame->end(), time);

    return it != timeFrame->end();
}

bool Course::_duplicataProtection(std::vector<Competency> * prereq, Competency c)
{
    std::vector<Competency>::iterator it = std::find( prereq->begin(), prereq->end(), c);
    return it != prereq->end();
}

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

// === OPERATOR
std::ostream& operator<<(std::ostream& Stream, const Course & c)
{
    std::string s = "Course\n\tid:"+std::to_string(c.id())+"\n\tname:"+c.name()+"\n\tECTS: "+std::to_string(c.ects());
    Stream << s;
    return Stream;
}
// === END OPERATOR


// === STATIC
/// Course counter
int Course::assignID()
{
    return ++Course::COURSE_COUNTER;
}
// === END STATIC