#ifndef SRD_MODEL_EXCEPTION_ID_OVERFLOW_EXCEPTION_H_
#define SRD_MODEL_EXCEPTION_ID_OVERFLOW_EXCEPTION_H_

#include <exception>
#include <string>

class idOverflowException : public std::exception
{
    private:
        std::string _msg;

    public:
        idOverflowException(std::string buildInfo) throw()
        {
            this->_msg = "The id of an object has overflow the maximal value. Where: "+buildInfo;
        }

        virtual const char* what() const throw()
        {
            return this->_msg.c_str();
        }
};


#endif //SRD_MODEL_EXCEPTION_ID_OVERFLOW_EXCEPTION_H_