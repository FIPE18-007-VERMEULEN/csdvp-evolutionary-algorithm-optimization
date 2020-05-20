#ifndef SRC_MODEL_EXCEPTION_JOB_OVERLAPING_BOUNDARIES_EXCEPTION_H_
#define SRC_MODEL_EXCEPTION_JOB_OVERLAPING_BOUNDARIES_EXCEPTION_H_

#include <exception>
#include <iostream>
#include <string>

#include "../profession.h"

class JobOverlappingBoundariesException : public std::exception
{
    private:
        Profession * _job;
        std::string _msg;

    public:
        JobOverlappingBoundariesException(Profession * pb) throw()
            : _job(pb)
        {
            this->_msg = "Boundaries of the Profession (aka Job) (id:"+std::to_string(pb->id())+")  are overlaping.";
        }

        ~JobOverlappingBoundariesException() throw()
        { delete(this->_job); }

        virtual const char* what() const throw()
        {return this->_msg.c_str();}

        Profession & getCSDVP() {return *(this->_job);}
};

#endif // SRC_MODEL_EXCEPTION_JOB_OVERLAPING_BOUNDARIES_EXCEPTION_H_