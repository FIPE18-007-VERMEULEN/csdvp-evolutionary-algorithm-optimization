#include <eo>
#include <iostream>
#include <string>

#include <cassert>

#include "model/magnitude.h"
#include "model/competency.h"

#include "model/exception/magnitudeException.h"
#include "model/exception/competencyEvolvingException.h"

int main(int argc, char* argv[])
{
    Magnitude m = Magnitude::build(0.7);
    Competency c = Competency::build(0.5, "");
    Competency c2 = Competency::build(m,"");
    Competency c3 = Competency::build(0.5);

    assert(c3.name().compare("Competency#"+std::to_string(c3.id())) == 0);
        std::cout << "Default naming ok" << std::endl;

    std::cout << std::to_string(c.c_magnitude().value()) << std::endl ;
    std::cout << c2.competencyValue() << std::endl;

    c.evolveTowards(0.2);
    std::cout << c << std::endl;
    c.evolveTowards(-0.2);
    std::cout << c << std::endl;
    c.evolveTowards(-0.3);
    std::cout << c << std::endl;
    c.evolveTowards(m);
    std::cout << c << std::endl;
    try
    {
        c.evolveTowards(2);
    }
    catch(CompetencyEvolvingException & e)
    {
        std::cout << e.what() << std::endl;

        e.getCompetency().setName("Oeuf!");
        std::cout << e.getCompetency() << std::endl;
        std::cout << c << std::endl;
    }
    
    c.evolveTowards(-0.5);
    std::cout << c << std::endl;

    assert( c == c);
        std::cout << "c == c OK" << std::endl ;
    std::cout << "renaming c2 as c.name()" << std::endl;
    c2.setName(c.name());
    assert( c == c2);
        std::cout << "c == c2 OK" << std::endl ;

    assert ( !(c == c3) );
        std::cout << "c != c3 OK" << std::endl;

    return EXIT_SUCCESS;
}