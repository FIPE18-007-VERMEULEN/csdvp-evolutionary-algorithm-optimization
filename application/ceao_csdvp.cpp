#include <eo>
#include <iostream>
#include <string>
#include <cassert>
#include <utility>
#include <vector>

#include "model/problem.h"
#include "model/course.h"
#include "model/competency.h"
#include "model/tools.h"

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

    assert(pb.getQuantityCoursesToPick() == -1);//'cauz pb is not yet fully config
    pb.set_cfg_pickedCoursesByTimeFrame(3);

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

    pb.set_cfg_minimalCompetencyByCourse(1);
    pb.set_cfg_maximalCompetencyByCourse(5);

    pb.set_cfg_minimalPrerequisiteByCourse(0);
    pb.set_cfg_maximalPrerequisiteByCourse(2);

    CSDVP::generateProblem(pb, CSDVP::GenerationType::RANDOM, 7777);
    assert(pb.checkConfig());
        std::cout << "CSDVP has been correctly configurated" << std::endl;

    assert(pb.getQuantityCoursesToPick() == 3 * 6);//course to pick * number of timeframe max - min +1

    assert(pb.timeFrames().at(0) == pb.cfg_minimalTimeFrame());
    assert(pb.timeFrames().at(1) == pb.cfg_minimalTimeFrame()+1);
    assert(pb.timeFrames().at(pb.timeFrames().size()-1) == pb.cfg_maximalTimeFrame());
        std::cout << "TimeFrames vector correctly init" << std::endl;

    assert(pb.coursesCatalogue().size() > 0);
    for(int i = 0; i < pb.coursesCatalogue().size(); i++)
        assert(pb.coursesCatalogue().at(i).timeFrame().size() > 0);
        std::cout << "Course catalogue randomized on TF OK." << std::endl;
    std::cout << "Displaying size ("+std::to_string(pb.coursesCatalogue().size())+")/ 2 and -1 and +1 course" << std::endl;
    std::cout << pb.coursesCatalogue().at(pb.coursesCatalogue().size()/2 - 1) << std::endl;
    std::cout << pb.coursesCatalogue().at(pb.coursesCatalogue().size()/2) << std::endl;
    std::cout << pb.coursesCatalogue().at(pb.coursesCatalogue().size()/2 + 1) << std::endl;

    int counter;
    std::cout << "Cheking Time Frames integrity..." << std::endl;
    for(int i = 0; i < pb.timeFrames().size(); i++)
    {
        counter = 0;

        for(int j = 0; j < pb.coursesCatalogue().size(); j++)
        {
            for(int k = 0; k < pb.coursesCatalogue().at(j).timeFrame().size(); k++)
            {
                if(pb.coursesCatalogue().at(j).timeFrame().at(k) == pb.timeFrames().at(i))
                    counter++;
                assert(counter <= pb.cfg_courseByTFMax());
            }
        }
        assert(counter >= pb.cfg_courseByTFMin());
    }
    std::cout << "TF Integrity is OK!" << std::endl;

    assert(pb.competencyCatalogue().size() == pb.cfg_quantityCompetencies());

    // for(int i = 0; i < pb.coursesCatalogue().size(); i++)
    // {
    //     std::cout << pb.coursesCatalogue().at(i).name() << "\n\tPrereq: " + std::to_string(pb.coursesCatalogue().at(i).prerequisites().size()) << std::endl;
    //     if(pb.coursesCatalogue().at(i).prerequisites().size() > 0)
    //         std::cout << "Competency 0" << pb.coursesCatalogue().at(i).prerequisites().at(0) << std::endl;  
    //     std::cout << "\n\tTeach: "+ std::to_string(pb.coursesCatalogue().at(i).teachedCompetenciesWeighted().size()) << std::endl;
    // }

    for(int i = 0; i < pb.competencyCatalogue().size() ; i++)
    {
        assert(pb.competencyCatalogue().at(i).c_magnitude().value() >= pb.cfg_magnitudeMin().value());
        assert(pb.competencyCatalogue().at(i).c_magnitude().value() <= pb.cfg_magnitudeMax().value());
    }
        std::cout << "Magnitudes OK" << std::endl;

    //CHECK QTE COMP IN RANGE OK
    for(int i = 0 ; i < pb.coursesCatalogue().size(); i++)
    {
        assert(pb.coursesCatalogue().at(i).prerequisites().size() >= pb.cfg_prerequisiteByCourseMin());
        assert(pb.coursesCatalogue().at(i).prerequisites().size() <= pb.cfg_prerequisiteByCourseMax());
        assert(pb.coursesCatalogue().at(i).teachedCompetenciesWeighted().size() >= pb.cfg_competencyByCourseMin());
        assert(pb.coursesCatalogue().at(i).teachedCompetenciesWeighted().size() <= pb.cfg_competencyByCourseMax());
    }
    std::cout << "Courses prerequistes ok!" << std::endl;
    std::cout << "Courses teached comp ok!" << std::endl;

    std::cout << "CSDVP HAS BEEN CORRECTLY GENERATED!" << std::endl;

    return EXIT_SUCCESS;
}