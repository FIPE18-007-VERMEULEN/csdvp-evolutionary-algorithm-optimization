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

    CSDVP::generateProblem(pb, CSDVP::GenerationType::RANDOM, 7777);
    assert(pb.seed() == 7777);
    assert(pb.checkConfig() == false);

    CSDVP::generateProblem(pb, CSDVP::GenerationType::RANDOM, -1);
    assert(pb.seed() == -1);
    assert(pb.checkConfig() == false);
        std::cout << "generateProblem does not trigger generation if pb is not config" << std::endl;

    pb.set_cfg_quantityCompetencies(50);
    assert(pb.cfg_quantityCompetencies() == 50);
    assert(pb.checkConfig() == false);

    pb.set_cfg_quantityCourses(25);
    assert(pb.cfg_quantityCourses() == 25);
    assert(pb.checkConfig() == false);

    pb.set_cfg_minimalTimeFrames(5);
    assert(pb.cfg_minimalTimeFrame() == 5);
    assert(pb.checkConfig() == false);

    pb.set_cfg_maximalTimeFrames(10);
    assert(pb.cfg_maximalTimeFrame() == 10);
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

    pb.set_cfg_minimalMagnitude(0.2);
    pb.set_cfg_maximalMagnitude(0.5);
    assert(pb.cfg_magnitudeMin().value() == 0.2);
    assert(pb.cfg_magnitudeMax().value() == 0.5);
    assert(pb.checkConfig() == false );
        std::cout << "Config ok -- excepting seed" << std::endl;

    pb.set_cfg_courseByTFMin(9);
    
    try
    {
        CSDVP::generateProblem(pb, CSDVP::GenerationType::RANDOM, 7777);
    }
    catch(CSDVPOverlapingBoundariesException & e)
    {
        std::cout << "Overlaping protection 1/2 OK" << std::endl;
    }

    pb.set_cfg_courseByTFMin(4);
    pb.set_cfg_ectsMax(2);

    try
    {
        CSDVP::generateProblem(pb, CSDVP::GenerationType::RANDOM, 7777);
    }
    catch(CSDVPOverlapingBoundariesException & e)
    {
        std::cout << "Overlaping protection 2/2 OK" << std::endl;
    }

    pb.set_cfg_ectsMax(5);

    pb.addTimeFrame(2);
    pb.addTimeFrame(5);
    assert(pb.timeFrames().size() == 2);

    pb.addTimeFrame(5);
    assert(pb.timeFrames().size() == 2);
        std::cout << "duplicata protection TF ok" << std::endl;
    
    Course c = Course::build();
    Course c2 = Course::build();

    assert( ! (c == c2) );

    pb.addCourseToCatalogue(c);
    pb.addCourseToCatalogue(c2);
    assert(pb.coursesCatalogue().size() == 2);
    
    pb.addCourseToCatalogue(c);
    assert(pb.coursesCatalogue().size() == 2);
        std::cout << "duplicata protection for course ok" << std::endl;

    Competency comp1 = Competency::build(0.5);
    Competency comp2 = Competency::build(0.7);
    assert(!(comp1 == comp2));

    pb.addCompetencyToCatalogue(comp1);
    pb.addCompetencyToCatalogue(comp2);
    assert(pb.competencyCatalogue().size() == 2);

    pb.addCompetencyToCatalogue(comp1);
    assert(pb.competencyCatalogue().size() == 2);
        std::cout << "duplicata protection for competency ok" << std::endl;

    std::cout << pb << std::endl;

    CSDVP::generateProblem(pb, CSDVP::GenerationType::RANDOM, 7777);
    assert(pb.checkConfig());
        std::cout << "CSDVP has been correctly configurated" << std::endl;

    assert(pb.timeFrames().at(0) == pb.cfg_minimalTimeFrame());
    assert(pb.timeFrames().at(1) == pb.cfg_minimalTimeFrame()+1);
    assert(pb.timeFrames().at(pb.timeFrames().size()-1) == pb.cfg_maximalTimeFrame());
        std::cout << "TimeFrames vector correctly init" << std::endl;

    return EXIT_SUCCESS;
}