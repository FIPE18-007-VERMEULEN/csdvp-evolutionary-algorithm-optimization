#ifndef SRC_PROFESSION_H_
#define SRC_PROFESSION_H_

#include <vector>

#include "competency.h"

/**
 * A profession is a job sought by a student.
 * 
 * It has a set a prerequisted competencies for the student to perform it correctly.
 */
class Profession
{
    private:
        std::vector<Competency> _prerequisites;
        std::string _name;
        int _id;

        int _requiredECTS;

        // === FUNC
        /** _duplicataProtection returns true if the value (2nd param) searched into (1st param) is found*/
        bool _duplicataProtection(std::vector<Competency> *, Competency);

        // Static
        static int PROFESSION_COUNTER;
        static int assignID();

    public:
        Profession();
        Profession(std::string name);
        Profession(std::vector<Competency> & p, std::string n = "");
        
        // === GETTER
        const int id() const{return this->_id;}
        const int requiredECTS() const{return this->_requiredECTS;}
        const std::string name() const{return this->_name;}
        const std::vector<Competency> & prerequisites() const{return this->_prerequisites;}
        /// return a modifiable reference to _prerequisite;
        std::vector<Competency> & unlocked_prerequisites(){return this->_prerequisites;}

        // === MUTATOR
            // SETTER
            /// Set name of the profession. If name empty, creates a default name based on ID
            void setName(std::string name);
            /// Set the prerequisites of a profession. The old prereq is returned.
            std::vector<Competency> & setPrerequisites(std::vector<Competency> & v);
            void setRequiredECTS(int ects);
            // ADDER
            bool addPrerequisite(Competency &);

};

// === OPERATOR
std::ostream & operator<<(std::ostream & Stream, const Profession & p);

#endif // SRC_PROFESSION_H_