#ifndef SRC_MODEL_EXCEPTION_COMPETENCY_EVOLVING_EXCEPTION_H_
#define SRC_MODEL_EXCEPTION_COMPETENCY_EVOLVING_EXCEPTION_H_

#include <exception>
#include <iostream>
#include <string>

#include "../competency.h"

class CompetencyEvolvingException : public std::exception
{
    public:
        CompetencyEvolvingException(Competency * c)
            : _c(c)
        {
            this->_phrase = "Exception while evolving the competency. Magnitude value was OOB and has been auto-corrected.";
        }

        virtual const char* what() const throw()
        {
            return _phrase.c_str();
        }

        virtual ~CompetencyEvolvingException() throw()
        {}

        Competency & getCompetency() const {return *(this->_c);}
    private:
        Competency * _c;
        std::string _phrase;
};

#endif // SRC_MODEL_EXCEPTION_COMPETENCY_EVOLVING_EXCEPTION_H_