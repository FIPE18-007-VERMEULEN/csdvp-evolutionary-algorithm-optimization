#ifndef SRC_MODEL_EXCEPTION_COURSE_ECTS_EXCEPTION_H_
#define SRC_MODEL_EXCEPTION_COURSE_ECTS_EXCEPTION_H_

#include <exception>
#include <iostream>
#include <string>

#include "../course.h"

class CourseECTSException : public std::exception
{
    private:
        Course * _course;
        std::string _phrase;
    public:
        CourseECTSException(Course * c) throw()
            : _course(c)
        {
            this->_phrase = "Exception on ECTS: value ("+std::to_string(c->ects())+") uncorrect. Can it be negative?";
        }

        virtual const char* what() const throw()
        {return _phrase.c_str();}

        virtual ~CourseECTSException() throw()
        {
            //this->_course must not be free by ~this!
        }

        Course & getCourse() const{return *(this->_course);}
};

#endif // SRC_MODEL_EXCEPTION_COURSE_ECTS_EXCEPTION_H_