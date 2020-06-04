#ifndef SRC_MODEL_EXCEPTION_COURSE_TEMPORAL_FRAME_EXCEPTION_H_
#define SRC_MODEL_EXCEPTION_COURSE_TEMPORAL_FRAME_EXCEPTION_H_

#include <exception>
#include <iostream>
#include <string>

#include "../course.h"

class CourseTemporalFrameException : public std::exception
{
    private:
        Course * _course;
        int _time;
        std::string _phrase;
    public:
        CourseTemporalFrameException(Course * c, int time) throw()
            : _course(c)
        {
            this->_phrase = "Exception on time frame: value ("+std::to_string(time)+") is uncorrect. Can it be negative?";
            this->_time = time;
        }

        virtual const char* what() const throw()
        {return _phrase.c_str();}

        virtual ~CourseTemporalFrameException() throw()
        {
            //this->_course must not be free by ~this!
        }

        Course & getCourse() const{return *(this->_course);}
        int getTime() const{return this->_time;}
};

#endif // SRC_MODEL_EXCEPTION_COURSE_TEMPORAL_FRAME_EXCEPTION_H_