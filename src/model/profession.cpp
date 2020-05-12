#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>

#include "profession.h"
#include "competency.h"

int Profession::PROFESSION_COUNTER = 0;

// === FACTORY
    // No factory needed
// === END FACTORY

// === CONSTRUCTOR
    Profession::Profession()
    {
        this->_id = assignID();
        this->_name = "Profession#"+std::to_string(this->_id);
    }
    Profession::Profession(std::string name)
    {
        this->_id = assignID();

        if(name.empty())
            name = "Profession#"+std::to_string(this->_id);
        this->_name = name;
    }

    Profession::Profession(std::vector<Competency> & p, std::string n)
        : _prerequisites(p)
    {
        this->_id = assignID();
        if(n.empty())
            n = "Profession#"+std::to_string(this->_id);
        this->_name = n;
    }

// === END CONSTRUCTOR

// === MUTATOR
    // SETTER
    void Profession::setName(std::string name)
    {
        if(name.empty())
            name = "Profession#"+std::to_string(this->_id);
        this->_name = name;
    }

    std::vector<Competency> & Profession::setPrerequisites(std::vector<Competency> & p)
    {
        std::vector<Competency> * old = &this->_prerequisites;
        this->_prerequisites = p;
        return *old;
    }

    void Profession::setRequiredECTS(int nb)
    {
        assert(nb >= 0);

        this->_requiredECTS = nb;
    }

    // ADDER
    /** Adds a new competency to the prerequisites of a profession. It is protected against duplicata: it can't have twice the same competency.
     * 
     * @return true if an insertion as been made into the prerequiste, false otherwise;
     * 
     * Warning: does not use shallow copy. This can lead to a potential non constant behaviors if use with Profession(vector<Competency>) regarding the modification of compentecies outside this.
     * @note Remove the referencing? 
     */
    bool Profession::addPrerequisite(Competency & c)
    {
        if(_duplicataProtection(& this->_prerequisites, c))
            return false;
        this->_prerequisites.push_back(c);
        return true;
    }

// === FUNC
    /// _duplicataProtection returns true if the value (2nd param) searched into (1st param) is found
    bool Profession::_duplicataProtection(std::vector<Competency> * prereq, Competency c)
    {
        std::vector<Competency>::iterator it = std::find( prereq->begin(), prereq->end(), c);
        return it != prereq->end();
    }

// === OPERATOR
std::ostream & operator<<(std::ostream & Stream, const Profession & p)
{
    std::string s = "Profession\n\tid:"+std::to_string(p.id())+"\n\tname:"+p.name()+"\n\t#Prereq:"+std::to_string(p.prerequisites().size());
    Stream << s;
    return Stream;
}

// === STATIC
int Profession::assignID(){return ++Profession::PROFESSION_COUNTER;}