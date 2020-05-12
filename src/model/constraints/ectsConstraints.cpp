#include "ectsConstraints.h"

#include "model/course.h"

std::pair<bool, double> ConstraintsECTS::integrityCheck(Cursus indiv)
{
    std::vector<Course> courses = this->_pb.coursesCatalogue();
    int tmpECTS = 0;
    std::cout << "courses size : " << std::to_string(courses.size()) << std::endl;
    std::cout << "courses catl : " << std::to_string(this->_pb.cfg_quantityCourses()) << std::endl;
    for(int i = 0; i < indiv.size(); i++)
    {
        tmpECTS += courses.at(indiv[i]).ects();
        std::cout << std::to_string(courses.at(indiv[i]).ects()) << " + ";
    }
    std::cout << " = " << std::to_string(tmpECTS) << std::endl;
    bool isCheckOK = false;
    if(tmpECTS >= this->_job.requiredECTS())
        isCheckOK = true;
    std::cout << "Required: " << std::to_string(this->_job.requiredECTS());
    double metric = (double)tmpECTS / this->_job.requiredECTS();

    return std::pair<bool, double>(isCheckOK, metric);
}