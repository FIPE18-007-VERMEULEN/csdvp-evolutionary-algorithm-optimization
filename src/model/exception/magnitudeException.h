#ifndef SRC_EXCEPTION_MAGNITUDE_EXCEPTION_H_
#define SRC_EXCEPTION_MAGNITUDE_EXCEPTION_H_

#include <exception>
#include <string>
#include <iostream>

#include "../magnitude.h"

class MagnitudeException : public std::exception
{
    private:
        Magnitude & _triedValue;
        std::string _phrase;

    public:
        MagnitudeException(Magnitude & m) throw()
            : _triedValue(m)
        {
            this->_phrase = "Magnitude Exception: value (" + std::to_string(m.value()) + ") is out of bound [0;1].";
        }

        virtual const char* what() const throw()
        {            
            return _phrase.c_str();  
        }

        virtual ~MagnitudeException() throw()
        {}

        Magnitude & getMagnitude() const {return this->_triedValue;}
};

#endif // SRC_EXCEPTION_MAGNITUDE_EXCEPTION_H_