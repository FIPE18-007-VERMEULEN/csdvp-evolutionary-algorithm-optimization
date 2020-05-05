#include <eo>
#include <iostream>
#include <string>
#include <cassert>

#include "model/course.h"
#include "model/competency.h"

#include "model/exception/courseECTSException.h"
#include "model/exception/courseTemporalFrameException.h"
#include "model/exception/competencyEvolvingException.h"

int main(int argc, char* argv[])
{
    Course c = Course::build();

    std::cout << c << std::endl;

    assert(c.prerequisites().size() == 0);
        std::cout << "Length of prereq: " << c.prerequisites().size() << std::endl;

    c = Course::build();
    std::cout << c << std::endl;

    assert(c.prerequisites().size() == 0);
        std::cout << "Length of prereq still : " << c.prerequisites().size() << " after redefining." << std::endl;

    c = Course::build(5);
    assert(c.ects() == 5);
        std::cout << "C's ects is equal to five" << std::endl;

    c = Course::build(7,"Testdel'oeuf");
    std::cout << c << std::endl;
    assert(c.ects() == 7 && (c.name().compare("Testdel'oeuf") == 0) );
        std::cout << "Comp constructor ok" << std::endl;

    

    try
    {
        c = Course::build(-1, "lol");
    }
    catch(CourseECTSException & e)
    {
        assert(e.getCourse().ects() == -1);
            std::cout << "Last ects set correctly taken into account" << std::endl;

        std::cout << e.what() << std::endl;
        std::cout << c << std::endl;
    }

    Competency comp1 = Competency::build(0.2);
    Competency comp2 = Competency::build(0.4);
    Competency comp3 = Competency::build(0.7);

    c.addPrerequisite(comp1);

    assert(c.prerequisites().size() == 1);
    assert(c.prerequisites().at(0) == comp1 );
        std::cout << "Prereq add OK" << std::endl;
    
    c.addPrerequisite(comp2);

    Competency * ptr_c = c.rmPrerequisite(comp1);
    assert(ptr_c != NULL);
    assert(c.prerequisites().size() == 1);
    assert(c.prerequisites().at(0) == comp2);
        std::cout << "Prereq rm 1/2 OK" << std::endl ;

    c.addPrerequisite(comp1);
    ptr_c = c.rmPrerequisite(comp3);

    assert(ptr_c == NULL);
    assert(c.prerequisites().size() == 2);
    assert(c.prerequisites().at(0) == comp2 );
    assert(c.prerequisites().at(1) == comp1);
        std::cout << "Prereq rm 2/2 OK" << std::endl ;

    c.addTemporalFrame(0);
    c.addTemporalFrame(2);
    assert(c.timeFrame().size() == 2);
    c.addTemporalFrame(2);
    assert(c.timeFrame().size() == 2);
        std::cout << "_temporalAvailability is protected against duplicate" << std::endl;

    try{
        c.addTemporalFrame(-2);
    }
    catch(CourseTemporalFrameException & e)
    {
        std::cout << e.what() << std::endl;
        assert(e.getTime() == -2);
        assert(c.timeFrame().size() == 2);
            std::cout << "Excepetion handling ok" << std::endl;
    }

    // int * ptr_time = c.rmTemporalFrameValue(2);
    // assert(ptr_time != NULL && c.timeFrame().size()==1);
    //     std::cout << "Temporal rm by value 1/2  ok" << std::endl;;
    // ptr_time = c.rmTemporalFrameValue(2);
    // assert(ptr_time == NULL && c.timeFrame().size() == 1);
    //     std::cout << "Temp rm by val 2/2 ok" << std::endl;
    
    // c.addTemporalFrame(7);
    // ptr_time = c.rmTemporalFrameIndex(1);
    // assert(ptr_time != NULL && c.timeFrame().size()==2 && c.timeFrame().at(0) == 0);
    //     std::cout << "Temporal rm by index ok" << std::endl;

    return EXIT_SUCCESS;
}