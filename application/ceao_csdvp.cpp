#include <eo>
#include <iostream>
#include <string>
#include <cassert>
#include <utility>
#include <vector>

#include "model/problem.h"
#include "model/course.h"
#include "model/competency.h"

#include "model/exception/csdvpOverlapingBoundaryException.h"

int main(int argc, char* argv[])
{
    CSDVP pb;

    assert(pb.id() == 1);
    assert(pb.seed() == -1);
    assert(pb.cfg_quantityCompetencies() == -1);
    std::cout << "CSDVP constructor OK" << std::endl;

    assert(pb.checkConfig() == false);
        std::cout << "OK! CSDVP shoukd not be config yet here" << std::endl;

    CSDVP::generateProblem(pb, 7777);
    assert(pb.seed() == 7777);
    assert(pb.checkConfig() == false);

    CSDVP::generateProblem(pb, -1);
    assert(pb.seed() == -1);
    assert(pb.checkConfig() == false);
        std::cout << "generateProblem does not trigger generation if pb is not config" << std::endl;

    pb.set_cfg_quantityCompetencies(50);
    assert(pb.cfg_quantityCompetencies() == 50);
    assert(pb.checkConfig() == false);

    pb.set_cfg_quantityCourses(25);
    assert(pb.cfg_quantityCourses() == 25);
    assert(pb.checkConfig() == false);

    pb.set_cfg_quantityTimeFrames(10);
    assert(pb.cfg_quantityTimeFrames() == 10);
    assert(pb.checkConfig() == false);

    pb.set_cfg_ectsMin(3);
    assert(pb.cfg_ectsMin() == 3);
    assert(pb.checkConfig() == false);

    pb.set_cfg_ectsMax(5);
    assert(pb.cfg_ectsMax() == 5);
    assert(pb.checkConfig() == false);

    pb.set_cfg_courseByTFMin(5);
    assert(pb.cfg_courseByTFMin() == 5);
    assert(pb.checkConfig() == false);

    pb.set_cfg_courseByTFMax(8);
    assert(pb.cfg_courseByTFMax() == 8);
    assert(pb.checkConfig() == false);
        std::cout << "Config ok -- excepting seed" << std::endl;

    pb.set_cfg_courseByTFMin(9);
    
    try
    {
        CSDVP::generateProblem(pb, 7777);
    }
    catch(CSDVPOverlapingBoundariesException & e)
    {
        std::cout << "Overlaping protection 1/2 OK" << std::endl;
    }

    pb.set_cfg_courseByTFMin(4);
    pb.set_cfg_ectsMax(2);

    try
    {
        CSDVP::generateProblem(pb, 7777);
    }
    catch(CSDVPOverlapingBoundariesException & e)
    {
        std::cout << "Overlaping protection 2/2 OK" << std::endl;
    }

    pb.set_cfg_ectsMax(5);

    CSDVP::generateProblem(pb, 7777);
    assert(pb.checkConfig());
        std::cout << "CSDVP has been correctly configurated" << std::endl;

    return EXIT_SUCCESS;
}