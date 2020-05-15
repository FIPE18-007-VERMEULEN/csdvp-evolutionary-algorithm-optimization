#include <iostream>

#include "competency.h"
#include "magnitude.h"
#include "tools.h"

#include "exception/magnitudeException.h"
#include "exception/competencyEvolvingException.h"
#include "exception/idOverflowException.h"

int Competency::COMPETENCY_COUNTER = 0;
int Competency::COMPETENCY_TMP_COUNTER = ID_RANGE_FOR_OBJECT + 1;

// === FACTORY

Competency Competency::build(Magnitude & m, std::string name)
{
    int id = Competency::assignID();
    if(name.empty())
        name = "Competency#"+std::to_string(id);
    
    return Competency(id, m, name);
}

Competency Competency::build(double d = 0, std::string name)
{
    int id = Competency::assignID();
    if(name.empty())
        name = "Competency#"+std::to_string(id);

    try
    {
        Magnitude m = Magnitude::build(d);
        return Competency(id, m, name); 
    }
    catch(MagnitudeException & e)
    {
        e.getMagnitude().rebase();
        throw CompetencyEvolvingException(new Competency(id, e.getMagnitude(), name)); 
    }  
}

Competency Competency::buildTMP(double d, std::string name)
{
    int id = Competency::assignID4TMP();
    if(name.empty())
        name = "Competency#"+std::to_string(id);

    try
    {
        Magnitude m = Magnitude::build(d);
        return Competency(id, m, name); 
    }
    catch(MagnitudeException & e)
    {
        e.getMagnitude().rebase();
        throw CompetencyEvolvingException(new Competency(id, e.getMagnitude(), name)); 
    }  
}

// === CONSTRUCTOR

Competency::Competency(int id, Magnitude m, std::string s)
    : _id(id), _m(m), _name(s)
{

}

// === STATIC

int Competency::assignID()
{
    if(Competency::COMPETENCY_COUNTER + 1 > ID_RANGE_FOR_OBJECT)
        throw idOverflowException("assignID()@Competency.cpp");
    return ++Competency::COMPETENCY_COUNTER;
}

int Competency::assignID4TMP()
{
    if(Competency::COMPETENCY_TMP_COUNTER + 1 > ID_RANGE_FOR_TEMPORARY_OBJECT)
    {
        std::cout << "INFO: COMPETENCY_TMP_COUNTER was about to overflow: restored to ID_RANGE_OBJECT + 1" << std::endl;
        COMPETENCY_TMP_COUNTER = ID_RANGE_FOR_OBJECT + 1;
    }
    return ++Competency::COMPETENCY_TMP_COUNTER;
}

// === FUNCTION

void Competency::evolveTowards(Magnitude & m)
{
    try
    {
        this->_m+= m;
    }
    catch(MagnitudeException & e)
    {
        e.getMagnitude().rebase();
        this->_m = e.getMagnitude();
        throw CompetencyEvolvingException(this);
    }
}

void Competency::evolveTowards(double d)
{
    try
    {
        this->_m += d;
    }
    catch(MagnitudeException & e)
    {
        e.getMagnitude().rebase();
        this->_m = e.getMagnitude();
        throw CompetencyEvolvingException(this);
    }
}

const double Competency::competencyValue() const
{
    return this->_m.value();
}

// === OPERATOR
std::ostream& operator<<(std::ostream& Stream, const Competency & c) 
{ 
    std::string s = "Competency\n\tid:"+std::to_string(c.id())+"\n\tname:"+c.c_name()+"\n\tvalue:"+std::to_string(c.competencyValue());
    Stream << s ; 
    return Stream; 
}

bool Competency::operator==(const Competency & c) const
{
    return ( this->_id == c.id() || ( this->_name.compare(c.c_name()) == 0 ) );
}