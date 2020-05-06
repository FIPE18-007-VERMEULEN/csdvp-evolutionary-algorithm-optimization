#include <eo>
#include <iostream>
#include <string>
#include <cassert>
#include <utility>
#include <vector>

#include "model/profession.h"
#include "model/competency.h"


int main(int argc, char* argv[])
{

    Competency c1 = Competency::build(0.5);
    Competency c2 = Competency::build(0.7);
    Competency c3 = Competency::build(0.21);

    Profession p1;
    Profession p2("NameTest");
    std::vector<Competency> vC;
    vC.push_back(c1); vC.push_back(c2);
    Profession p3(vC);

    assert(p1.prerequisites().size() == 0);
    assert(p1.name().compare("Profession#1") == 0);

    assert(p2.name().compare("NameTest") == 0);

    assert(p3.prerequisites().size() == 2);
    assert(p3.prerequisites().at(0) == c1);

    std::cout << "Profession constructors OK" << std::endl;

    p2.setPrerequisites(vC);
    assert(p2.prerequisites().size() == 2);
    assert(p2.prerequisites().at(0) == c1);

    bool state;
    state=p2.addPrerequisite(c3);
    assert(state);
    assert(p2.prerequisites().size() == 3);
    assert(p2.prerequisites().at(2) == c3);

    state=p2.addPrerequisite(c3);
    assert(!state);
    assert(p2.prerequisites().size() == 3);
    assert(p2.prerequisites().at(2) == c3);

    std::cout << p2 << std::endl;
    std::cout << p3 << std::endl;

    c3.evolveTowards(0.4);
    std::cout << c3 << std::endl;
    std::cout << p2.prerequisites().at(2) << std::endl;

    p2.unlocked_prerequisites().at(2).evolveTowards(0.3);

    std::cout << c3 << std::endl;
    std::cout << p2.prerequisites().at(2) << std::endl;


    return EXIT_SUCCESS;
}