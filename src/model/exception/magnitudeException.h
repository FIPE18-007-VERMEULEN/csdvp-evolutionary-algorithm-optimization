#ifndef SRC_EXCEPTION_MAGNITUDE_EXCEPTION_H_
#define SRC_EXCEPTION_MAGNITUDE_EXCEPTION_H_

#include <exception>
#include <string>

class MagnitudeException : public std::exception
{
    private:
        double _triedValue;
        std::string _phrase;

    public:
        MagnitudeException(double v) throw()
            : _triedValue(v)
        {
            this->_phrase = "Magnitude Exception: value (" + std::to_string(v) + ") is out of bound [0;1].";
        }

        virtual const char* what() const throw()
        {            
            return _phrase.c_str();  
        }

        virtual ~MagnitudeException() throw()
        {}
};

#endif // SRC_EXCEPTION_MAGNITUDE_EXCEPTION_H_