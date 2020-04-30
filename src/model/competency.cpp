#include "competency.h"
#include "magnitude.h"
#include "exception/magnitudeException.h"

// === FACTORY

Competency Competency::build(Magnitude & m, std::string name = "")
{
    int id = Competency::assignID();
    if(name.empty())
        name = "Competency#"+id;
    
    return Competency(id, m, name);
}

Competency Competency::build(double d = 0, std::string name = "")
{
    int id = Competency::assignID();
    if(name.empty())
        name = "Competency#"+id;

    try
    {
        Magnitude m = Magnitude::build(d);
        return Competency(id, m, name); 
    }
    catch(MagnitudeException & e)
    {
        e.getMagnitude().rebase();
        Magnitude m(e.getMagnitude());
        return Competency(id, m, name); 
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
    return ++Competency::COMPETENCY_COUNTER;
}

// === FUNCTION

/// @TODO
void Competency::evolveTowards(Magnitude & m)
{

}

/// @TODO
void Competency::evolveTowards(double d)
{

}