#include <iostream>
#include <utility>

#include "course.h"
#include "competency.h"

#include "exception/courseECTSException.h"

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
    void Course::setAvailabilities(std::vector<int> times)
    {}
    void Course::setPrerequisites(std::vector<Competency> prereqs)
    {}
    void Course::setTeachedComps(std::vector<std::pair<Competency,double>> wComps)
    {}

    // ADDER
    void Course::addTeachedComp(std::pair<Competency,double> wComp)
    {}
    void Course::addPrerequisite(Competency prereq)
    {}
    void Course::addTemporalFrame(int time)
    {}

    // DELETER
    Competency * Course::rmPrerequisite(Competency prereq)
    {return NULL;}
    std::pair<Competency,double> * Course::rmTeachedComp(Competency teached)
    {return NULL;}
    int * Course::rmTemporalFrameIndex(int index)
    {
        return NULL;
    }
    int * Course::rmTemporalFrameValue(int value)
    {
        return NULL;
    }
// === END MUTATOR


// === OPERATOR
std::ostream& operator<<(std::ostream& Stream, const Course & c)
{
    std::string s = "Competency\n\tid:"+std::to_string(c.id())+"\n\tname:"+c.name()+"\n\tECTS: "+std::to_string(c.ects());
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