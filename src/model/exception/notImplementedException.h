#ifndef SRC_MODEL_EXCEPTION_NOT_IMPLEMENTED_EXCEPTION_H_
#define SRC_MODEL_EXCEPTION_NOT_IMPLEMENTED_EXCEPTION_H_

#include <exception>
#include <string>

class NotImplementedException : public std::exception
{
    private:
        std::string _msg;

    public:
        NotImplementedException(std::string funcName)
        {
            this->_msg = "Function "+funcName+" not yet implemented.";
        }
        virtual const char* what() const throw()
        {
            return this->_msg.c_str();
}
};

#endif // SRC_MODEL_EXCEPTION_NOT_IMPLEMENTED_EXCEPTION_H_