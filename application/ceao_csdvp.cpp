#include <eo>
#include <iostream>
#include <string>
#include <cassert>
#include <utility>
#include <vector>

#include "model/problem.h"
#include "model/course.h"
#include "model/competency.h"

int main(int argc, char* argv[])
{
    CSDVP pb;

    assert(pb.id() == 1);
    assert(pb.seed() == -1);
    assert(pb.cfg_quantityCompetencies() == -1);
    std::cout << "CSDVP constructor OK" << std::endl;

    assert(pb.checkConfig() == false);
        std::cout << "OK! CSDVP is not cofig yet here" << std::endl;

    return EXIT_SUCCESS;
}