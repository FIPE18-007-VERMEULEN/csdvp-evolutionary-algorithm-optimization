#include <eo>
#include <queen.h>
#include <queenInit.h>
#include <queenMutation.h>
#include <queenCrossover.h>
#include <queenEval.h>

#include <model/problem.h>
#include <model/profession.h>
#include <model/magnitude.h>
#include <model/tools.h>
#include <model/competency.h>

#include <model/ea/cursus.h>
#include <model/ea/initializer.h>
#include <model/ea/initConstraint.h>
#include <model/ea/mutation.h>
#include <model/ea/crossover.h>
#include <model/ea/evaluator.h>

#include <model/constraints/ectsConstraints.h>
#include <model/constraints/repetitionConstraints.h>
#include <model/constraints/professionConstraints.h>
#include <model/constraints/prerequisitesConstraints.h>

#include <model/exception/magnitudeException.h>
#include <model/exception/competencyEvolvingException.h>

void make_help(eoParser & _parser);

int main(int argc, char* argv[]){
    
    /*** init ***/
    eoParser parser(argc, argv);  // for user-parameter reading  
    eoState state;                // to keep all things allocate

    

    // ================================= CEAO ZONE ===================================

    //GENERAL PARAMATERS
    eoValueParam<uint32_t>& _seedParam = parser.createParam(uint32_t(0), "seed", "Random number seed", 'S');
    std::string _outputFile = parser.createParam(std::string("/scratch"), "outputfile", "", '\0', "Representation", true).value();

    //PROBLEM PARAMETERS
    unsigned int NBCOURSES = parser.createParam((unsigned int)(15), "nbCourses", "Nb of courses available in the cursus",'C',"Param").value();
    unsigned int NBCOMP = parser.createParam((unsigned int)(10), "nbComp", "Nb of competencies available in the cursus",'c',"Param").value();
    unsigned int MINTF = parser.createParam((unsigned int)(1), "minTF", "id of the first time frame",'t',"Param").value();
    unsigned int MAXTF = parser.createParam((unsigned int)(6), "maxTF", "id of the last time frame",'T',"Param").value();
    unsigned int MINECTS = parser.createParam((unsigned int)(1), "minECTS", "minimum number ECTS by course",'e',"Param").value();
    unsigned int MAXECTS = parser.createParam((unsigned int)(5), "maxECTS", "maxmimum number ECTS by course",'E',"Param").value();
    unsigned int MINCBYTF = parser.createParam((unsigned int)(3), "minCbyTF", "minimal course by time frame",'n',"Param").value();
    unsigned int MAXCBYTF = parser.createParam((unsigned int)(8), "maxCbyTF", "maximal course by time frame",'N',"Param").value();
    double MINMAGN = parser.createParam((double)(0.2), "minMagn", "miminal magnitude", 'm', "Param").value();
    double MAXMAGN = parser.createParam((double)(0.75), "maxMagn", "maximal magnitude", 'M', "Param").value();
    unsigned int MINCBYC = parser.createParam((unsigned int)(1), "minCbyC", "minimal competency by course",'r',"Param").value();
    unsigned int MAXCBYC = parser.createParam((unsigned int)(5), "maxCbyC", "maximal competency by course",'R',"Param").value();
    unsigned int MINPRE = parser.createParam((unsigned int)(0), "minPre", "minimal competency by course",'p',"Param").value();
    unsigned int MAXPRE = parser.createParam((unsigned int)(3), "maxPre", "maximal competency by course",'P',"Param").value();
    unsigned int CBYTF = parser.createParam((unsigned int)(2), "cbyTF", "course by time frame to pick",'A',"Param").value();

    //EVOLUTION ENGINE PARAMETERS
    unsigned int POPSIZE = parser.createParam((unsigned int)(100), "popSize", "Population size", 'P', "Evolution Engine").value();
    double PMUT = parser.createParam((double)(0.5), "pMut", "mutation rate", 'x', "Evolution Engine").value();
    double PCROSS = parser.createParam((double)(0.5), "pCross", "crossover rate", 'X', "Evolution Engine").value();    
    unsigned int NBGEN = parser.createParam((unsigned int)(100), "nbGen", "Number of generation",'G',"Param").value();
    unsigned int SIZET = parser.createParam((unsigned int)(7), "sizeT", "Tournament Size",'S',"Param").value();
        
    // ===== PB CONFIG ZONE =====
    CSDVP pb;
    Profession job;
    pb.set_cfg_quantityCourses(NBCOURSES);
    pb.set_cfg_quantityCompetencies(NBCOMP);
    pb.set_cfg_minimalTimeFrames(MINTF);
    pb.set_cfg_maximalTimeFrames(MAXTF); //Just "Licence"
    pb.set_cfg_ectsMin(MINECTS);
    pb.set_cfg_ectsMax(MAXECTS);
    pb.set_cfg_courseByTFMin(MINCBYTF);
    pb.set_cfg_courseByTFMax(MAXCBYTF);
    pb.set_cfg_minimalMagnitude(MINMAGN);
    pb.set_cfg_maximalMagnitude(MAXMAGN);
    pb.set_cfg_minimalCompetencyByCourse(MINCBYC);
    pb.set_cfg_maximalCompetencyByCourse(MAXCBYC);
    pb.set_cfg_minimalPrerequisiteByCourse(MINPRE);
    pb.set_cfg_maximalPrerequisiteByCourse(MAXPRE);
    pb.set_cfg_pickedCoursesByTimeFrame(CBYTF);

    CSDVP::generateProblem(pb, CSDVP::GenerationType::RANDOM, 7777);
    assert(pb.checkConfig());
    
    job.setRequiredECTS(Profession::GenerationType::RANDOM);
    job.set_cfg_minimalPrerequisites(2);
    job.set_cfg_maximalPrerequisites(5);
    job.set_cfg_minimalMagnitude(0.8);
    job.set_cfg_maximalMagnitude(1.0);

    Profession::generateProfession(job, Profession::GenerationType::RANDOM, pb, 7777);
    assert(job.checkConfig());
    std::cout << "JOB IS : \n" << job << std::endl;

    // job.setRequiredECTS(4 * 6);
    // Competency tmpC = pb.competencyCatalogue().at(0);
    // tmpC = Competency::buildTMP(0.8, tmpC.c_name());
    // job.addPrerequisite(tmpC);
    // tmpC = pb.competencyCatalogue().at(1);
    // tmpC = Competency::buildTMP(0.8, tmpC.c_name());
    // job.addPrerequisite(tmpC);
    // tmpC = pb.competencyCatalogue().at(2);
    // tmpC = Competency::buildTMP(0.8, tmpC.c_name());
    // job.addPrerequisite(tmpC);

    //tmpC = Competency::build(0.5,"Wesh");
    //job.addPrerequisite(tmpC);
    //tmpC = pb.competencyCatalogue().at(8);
    //job.addPrerequisite(tmpC);
    // ===== END PB CONFIG =====

    ConstraintsECTS ctrECTS(pb, job);
    ConstraintsRepetition ctrRep(pb, job);
    ConstraintsProfession ctrJob(pb, job);
    ConstraintsPrerequisites ctrPrq(pb, job);

    
    //std::cout << "getQuantityCoursesToPick : " << std::to_string(pb.getQuantityCoursesToPick()) << std::endl;
    //std::cout << "cfg_quantityCourses() : " << std::to_string(pb.cfg_quantityCourses()) << std::endl;
    /**@todo make size of the pb accessible as well as size of an individu*/

    
    // ======================== AG CONFIG ZONE ========================
    
    //PROBLEM DEPENDANT OPERATOR
    CursusInitConstraint init(pb);    
    CursusEval eval(ctrPrq, ctrRep, ctrJob, ctrECTS);
    CursusCrossover cross(pb, ctrRep, init);
    CursusMutation mut(pb, ctrRep);
    
    //PROBLEM INDEPENDANT OPERATOR
    eoGenContinue<Cursus> cont(NBGEN);
    eoSGATransform<Cursus> transform(cross, PCROSS, mut, PMUT);
    eoDetTournamentSelect<Cursus> selectOne(SIZET);
    eoSelectPerc<Cursus> select(selectOne);
    eoGenerationalReplacement<Cursus> replace;

    //POPULATION INITIALISATION
    eoPop<Cursus> pop;
    Cursus c1;
    for(int i = 0; i < POPSIZE; i++){
      init(c1);
      eval(c1);
      pop.push_back(c1);
    }

    // ======================== AG CONFIG ZONE END ========================

    
    // ================================ TEST ZONE ===========================================
    /*
    std::cout << "MAGNITUDE TEST ZONE" << std::endl;
    Magnitude m = Magnitude::build(0.5);
    Magnitude n = Magnitude::build(0.3);
    Magnitude o = Magnitude::build(0.2);

    o = m;

    std::cout << "Magnitude" << o.value() << std::endl;

    std::cout << "Magnitude" << m.value() << std::endl;

    m += n;

    std::cout << "New magnitude is " << m.value() << std::endl;

    try{
      std::cout << "After addition mag is : " << (m + m).value() << std::endl;
    }
    catch(MagnitudeException & e)
      {
	std::cout << "\nEXCEPTION CATCH !\n";
	std::cout << "Memory adr of m is : " << &m << std::endl;
	std::cout << "Memory adr of e is : " << &e.getMagnitude() << std::endl;
	e.getMagnitude().rebase();
	std::cout << "REBASE! New magnitude value is" << e.getMagnitude().rebase() << std::endl;
	std::cout << "Accessing magnitude value : " << e.getMagnitude().value() << std::endl;
	std::cout << "magnitude value of M : " << m.value() << std::endl;
	m = e.getMagnitude();
	std::cout << "before end catch" << m.value() << std::endl;
      }

    std::cout << "Inspect m value:" << m.value() << std::endl;
    std::cout << "END MAGNITUDE TEST\n\n\n" << std::endl;
    */

    /*
      std::cout << "CONSTRAINT TEST ZONE" << std::endl;
    std::pair<bool,double> res;
    for(int i = 0; i < pop.size(); i++){
      res = ctrECTS.integrityCheck(pop[i]);
      std::cout << "ECTS Metric" << std::to_string(res.second) << std::endl;
      //res = ctrRep.integrityCheck(pop[i]);
      //res = ctrJob.integrityCheck(pop[i]);
      res = ctrPrq.integrityCheck(pop[i]);
      std::cout << "IND#" << std::to_string(i) << "\nFirst: " << res.first << "\nSecond: " << std::to_string((double)res.second) << std::endl;
      if(res.first)
	{
	  std::cout << "*************** OK PREREQ !! ***********" << std::endl;
	}
      else
	{
	  std::cout << "$$$$$$$$$$$$$$$$ NON OK PREREQ !! $$$$$$$$$$$$$$$$$$$" << std::endl;
	}
      for(int i = 0 ; i < pop[i].size(); i++)
	{
	  std::cout << pb.coursesCatalogue().at(pop[i].at(i)) << std::endl;
	}
    }
    */
    
    //MUTATION TEST
    /*
      std::cout << "MUTATION TEST ZONE" << std::endl;
      pop[0].printOn(std::cout);
      std::cout << std::endl;
      for(int i=0; i<50; i++){
      mut(pop[0]);
      pop[0].printOn(std::cout);
      std::cout << std::endl;
      }
    */
  
    //CROSS TEST
    /*
      std::cout << "CROSSOVER TEST ZONE" << std::endl;
      pop[0].printOn(std::cout);
      std::cout << std::endl;
      pop[1].printOn(std::cout);
      std::cout << std::endl;
      for(int i=0; i<50; i++){
      cross(pop[0],pop[1]);
      pop[0].printOn(std::cout);
      std::cout << std::endl;
      pop[1].printOn(std::cout);
      std::cout << std::endl;
      std::cout << std::endl;
      }
    */

    /*
    for(int i = 0; i < pb.coursesCatalogue().size(); i++)
      {
	std::cout << pb.coursesCatalogue().at(i) << std::endl;
      }
    */
    // ======================== TEST ZONE END========================


    
    // ================================= RUN ZONE ===============================
    
    std::cout << "===== CURRENT POP =====" << std::endl;
    pop.best_element().printOn(std::cout);
    std::cout << " fitness:" << pop.best_element().fitness() << std::endl;
    std::cout << "Stats & metrics: \n" << std::endl;
    std::pair<bool, double> resECTS = ctrECTS.integrityCheck(pop.best_element());
    std::pair<bool, double> resRep = ctrRep.integrityCheck(pop.best_element());
    std::pair<bool, double> resJob = ctrJob.integrityCheck(pop.best_element());
    std::pair<bool, double> resPrq = ctrPrq.integrityCheck(pop.best_element());
    
    std::cout << "ECTS: ";
    if(resECTS.first)
      std::cout << "succeed";
    else
      std::cout << "failed";
    std::cout << " | value: " << resECTS.second << std::endl;

    std::cout << "Repetition: ";
    if(resRep.first)
      std::cout << "succeed";
    else
      std::cout << "failed";
    std::cout << " | value: " << resRep.second << std::endl;

    std::cout << "Job: ";
    if(resJob.first)
      std::cout << "succeed";
    else
      std::cout << "failed";
    std::cout << " | value: " << resJob.second << std::endl;

    std::cout << "Prereq: ";
    if(resPrq.first)
      std::cout << "succeed";
    else
      std::cout << "failed";
    std::cout << " | value: " << resPrq.second << std::endl;
    std::cout << "\n==========" << std::endl;

    eoEasyEA<QUEEN> algo(cont,eval,select,transform,replace);

    algo(pop);

    std::cout << "\n===== BEST INDIVIDU =====" << std::endl;
    pop.best_element().printOn(std::cout);
    std::cout << " fitness:" << pop.best_element().fitness() << std::endl;
    std::cout << "Stats & metrics: \n" << std::endl;
    resECTS = ctrECTS.integrityCheck(pop.best_element());
    resRep = ctrRep.integrityCheck(pop.best_element());
    resJob = ctrJob.integrityCheck(pop.best_element());
    resPrq = ctrPrq.integrityCheck(pop.best_element());
    
    std::cout << "ECTS: ";
    if(resECTS.first)
      std::cout << "succeed";
    else
      std::cout << "failed";
    std::cout << " | value: " << resECTS.second << std::endl;

    std::cout << "Repetition: ";
    if(resRep.first)
      std::cout << "succeed";
    else
      std::cout << "failed";
    std::cout << " | value: " << resRep.second << std::endl;

    std::cout << "Job: ";
    if(resJob.first)
      std::cout << "succeed";
    else
      std::cout << "failed";
    std::cout << " | value: " << resJob.second << std::endl;

    std::cout << "Prereq: ";
    if(resPrq.first)
      std::cout << "succeed";
    else
      std::cout << "failed";
    std::cout << " | value: " << resPrq.second << std::endl;

    std::cout << "===============" << std::endl;

    std::cout << "cpt: " << cross.cpt << std::endl;
  
    // ================================= END RUN ZONE ===============================
 
  return EXIT_SUCCESS;
}
