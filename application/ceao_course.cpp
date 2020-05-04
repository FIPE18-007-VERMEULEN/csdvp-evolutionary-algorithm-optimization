#include <eo>
#include <iostream>
#include <string>

#include "model/course.h"

int main(int argc, char* argv[])
{
    Course c = Course::build();

    std::cout << c << std::endl;
    std::cout << "Length of prereq: " << c.prerequisites().size() << std::endl;

    c = Course::build();
    std::cout << c << std::endl;
    std::cout << "Length of prereq: " << c.prerequisites().size() << std::endl;

    c = Course::build(5);
    std::cout << c << std::endl;

    c = Course::build(7,"Testdel'oeuf");
    std::cout << c << std::endl;

    c = Course::build(-1, "lol");
    std::cout << c << std::endl;

    return EXIT_SUCCESS;
}