#ifndef SRC_MODEL_EXCEPTION_CSDVP_OVERLAPING_BOUNDARIES_EXCEPTION_H_
#define SRC_MODEL_EXCEPTION_CSDVP_OVERLAPING_BOUNDARIES_EXCEPTION_H_

#include <exception>
#include <iostream>
#include <string>

#include "../problem.h"

class CSDVPOverlapingBoundariesException : public std::exception
{
    private:
        CSDVP * _csdvp;
        std::string _msg;

    public:
        CSDVPOverlapingBoundariesException(CSDVP * pb) throw()
            : _csdvp(pb)
        {
            this->_msg = "Boundaries of the CSDVP (id:"+std::to_string(pb->id())+")  are overlaping.";
        }

        virtual const char* what() const throw()
        {return this->_msg.c_str();}

        CSDVP & getCSDVP() {return *(this->_csdvp);}
};

#endif // SRC_MODEL_EXCEPTION_CSDVP_OVERLAPING_BOUNDARIES_EXCEPTION_H_