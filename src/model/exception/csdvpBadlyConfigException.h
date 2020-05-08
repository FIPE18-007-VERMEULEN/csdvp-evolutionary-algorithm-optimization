#ifndef SRC_MODEL_EXCEPTION_CSDVP_BADLY_CONFIG_EXCEPTION_H_
#define SRC_MODEL_EXCEPTION_CSDVP_BADLY_CONFIG_EXCEPTION_H_

#include <exception>
#include <string>

class CSDVPBadlyConfiguratedException : public std::exception
{
    private:
        std::string _msg;
    
    public:
        CSDVPBadlyConfiguratedException(std::string failedCondDescr) throw()
        {
            _msg = "The problem is badly configurated and thus, likely to 1) be overconstrained or 2) can't be generated.\nThe failed condition is: "+failedCondDescr;
        }

        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }
};

#endif //SRC_MODEL_EXCEPTION_CSDVP_BADLY_CONFIG_EXCEPTION_H_