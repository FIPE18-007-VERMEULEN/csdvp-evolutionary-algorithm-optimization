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

int main(int argc, char* argv[]){

  // ================================ TEST ZONE ===========================================
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

  // ================================= END TEST ZONE =====================================

  // ================================= CEAO ZONE ===================================
  CSDVP pb;
  Profession job;
    // ===== PB CONFIG ZONE =====
      pb.set_cfg_quantityCourses(25);
      pb.set_cfg_quantityCompetencies(40);
      pb.set_cfg_minimalTimeFrames(1);
      pb.set_cfg_maximalTimeFrames(6); //Just "Licence"
      pb.set_cfg_ectsMin(1);
      pb.set_cfg_ectsMax(5);
      pb.set_cfg_courseByTFMin(3);
      pb.set_cfg_courseByTFMax(8);
      pb.set_cfg_minimalMagnitude(0.2);
      pb.set_cfg_maximalMagnitude(0.75);
      pb.set_cfg_minimalCompetencyByCourse(1);
      pb.set_cfg_maximalCompetencyByCourse(5);
      pb.set_cfg_minimalPrerequisiteByCourse(0);
      pb.set_cfg_maximalPrerequisiteByCourse(3);

      pb.set_cfg_pickedCoursesByTimeFrame(2);

      CSDVP::generateProblem(pb, CSDVP::GenerationType::RANDOM, 7777);
      assert(pb.checkConfig());
      


      job.setRequiredECTS(4 * 6);
      Competency tmpC = pb.competencyCatalogue().at(0);
      tmpC = Competency::buildTMP(0.8, tmpC.c_name());
      job.addPrerequisite(tmpC);
      tmpC = pb.competencyCatalogue().at(10);
      tmpC = Competency::buildTMP(0.8, tmpC.c_name());
      job.addPrerequisite(tmpC);
      tmpC = pb.competencyCatalogue().at(20);
      tmpC = Competency::buildTMP(0.8, tmpC.c_name());
      job.addPrerequisite(tmpC);
      //tmpC = Competency::build(0.5,"Wesh");
      //job.addPrerequisite(tmpC);
      //tmpC = pb.competencyCatalogue().at(8);
      //job.addPrerequisite(tmpC);
    // ===== END PB CONFIG =====

  Cursus c1;
  
  std::cout << "getQuantityCoursesToPick : " << std::to_string(pb.getQuantityCoursesToPick()) << std::endl;
  std::cout << "cfg_quantityCourses() : " << std::to_string(pb.cfg_quantityCourses()) << std::endl;

  /**@todo make size of the pb accessible as well as size of an individu*/
  int size_of_the_pb = 100;

  ConstraintsECTS ctrECTS(pb, job);
  ConstraintsRepetition ctrRep(pb, job);
  ConstraintsProfession ctrJob(pb, job);
  ConstraintsPrerequisites ctrPrq(pb, job);
  std::pair<bool,double> res;

  //CursusInit init(pb.getQuantityCoursesToPick()-5,0);
  //CursusInit init(pb.getQuantityCoursesToPick(),pb.coursesCatalogue().size(),pb.seed());
  CursusInitConstraint init(pb);
  //pb.cfg_quantityCourses());//pb.getQuantityCoursesToPick(),pb.cfg_quantityCourses(), pb.seed()); 
  CursusEval eval(ctrPrq, ctrRep, ctrJob, ctrECTS);

  CursusCrossover cross(pb, ctrRep, init);
  CursusMutation mut(pb, ctrRep);
  
  eoGenContinue<Cursus> cont(1000); // runs for 100 gen
  
  //xOver, xOver rate, mutation, mutation rate
  eoSGATransform<Cursus> transform(cross, 0.5, mut, 0.8);
  eoDetTournamentSelect<Cursus> selectOne(5); //selection method by tournament, here against 2
  eoSelectPerc<Cursus> select(selectOne);
  eoGenerationalReplacement<Cursus> replace;
  eoPop<Cursus> pop;



  for(int i = 0; i < size_of_the_pb; i++)
  {
    init(c1);
    eval(c1);
    /*
    res = ctrECTS.integrityCheck(c1);
    std::cout << "ECTS Metric" << std::to_string(res.second) << std::endl;
    //res = ctrRep.integrityCheck(c1);
    //res = ctrJob.integrityCheck(c1);
    res = ctrPrq.integrityCheck(c1);
    std::cout << "IND#" << std::to_string(i) << "\nFirst: " << res.first << "\nSecond: " << std::to_string((double)res.second) << std::endl;
    if(res.first)
    {
      std::cout << "*************** OK PREREQ !! ***********" << std::endl;
    }
    else
    {
      std::cout << "$$$$$$$$$$$$$$$$ NON OK PREREQ !! $$$$$$$$$$$$$$$$$$$" << std::endl;
    }
    for(int i = 0 ; i < c1.size(); i++)
      {
        std::cout << pb.coursesCatalogue().at(c1.at(i)) << std::endl;
      }
    */
    pop.push_back(c1);
  }
    
  //MUTATION TEST
  /*
  pop[0].printOn(std::cout);
  std::cout << std::endl;
  for(int i=0; i<50; i++){
    mut(pop[0]);
    pop[0].printOn(std::cout);
    std::cout << std::endl;
    }*/
  
  //CROSS TEST
  /*
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
  
  for(int i = 0; i < pb.coursesCatalogue().size(); i++)
  {
    //std::cout << pb.coursesCatalogue().at(i) << std::endl;
  }

  
  std::cout << "===== CURRENT POP =====" << std::endl;
  pop.best_element().printOn(std::cout);
  std::cout << "=====             =====" << std::endl;

  eoEasyEA<QUEEN> algo(cont,eval,select,transform,replace);

  algo(pop);

  std::cout << "===== BEST INDIVIDU =====" << std::endl;
  pop.best_element().printOn(std::cout);
  std::cout << " fitness:" << pop.best_element().fitness() << std::endl;
  std::cout << "===============" << std::endl;

  std::cout << "cpt: " << cross.cpt << std::endl;
  
  // ================================= END CEAO ZONE ===============================
  
  
  
  // //Define a QUEEN -> 1 line
  // QUEEN s1;

  // //Define an initializer -> 1 line
  // queenInit init(8,1);

  // //Define the evaluation function -> 1 line
  // queenEval eval;

  // //Define mutation -> 1 line
  // queenMutation mut;

  // //Define crossover -> 1 line
  // queenCrossover cross;

  // //Define a generational continuator (put 100 generation for example) -> 1 line
  // eoGenContinue<QUEEN> cont(100);

  // //Define the transformation object (it contains, the crossover, the crossover rate, the mutation and the mutation rate) -> 1 line
  // eoSGATransform<QUEEN> transform(cross, 0.1, mut, 0.1);

  // //Define a selection method that selects ONE individual by deterministic tournament(put the tournament size at 2 for example) -> 1 line 
  // eoDetTournamentSelect<QUEEN> selectOne(2);

  // //Define a "eoSelectPerc" with the tournament with default parameter (allow to select the good size of individuals) -> 1 line
  // eoSelectPerc<QUEEN> select(selectOne);

  // //Define a generational replacement strategy -> 1 line
  // eoGenerationalReplacement<QUEEN> replace;

  // //Define a pop of QUEEN -> 1 line
  // eoPop<QUEEN> pop;

  // //Fill the pop with 100 initialized and evaluated QUEEN
  // //Use the initializer, the evaluation function and the push_back operator's vector -> A "for" included three insrtuctions
  // for(unsigned int i=0; i<100; i++){
  //   init(s1);
  //   eval(s1);
  //   pop.push_back(s1);
  // }
  
  // //Print the pop -> 1 line
  // pop.printOn(std::cout);

  //HERE you can test whether you succeded in initializing the population by compiling and executing this part of the program.
  
  //Print end of line (endl)
  // std::cout << std::endl;
  
  // /*Define an eoEasyEA with good parameter:
  //   - continuator
  //   - evaluation function
  //   - eoSelectPerc
  //   - transformation object
  //   - replacement
  //  */
  // // -> 1 line
  // eoEasyEA<QUEEN> algo(cont,eval,select,transform,replace);

  // //run the algorithm on the initialized population -> 1 line
  // algo(pop);

  // //Print the best element -> 1 line
  // pop.best_element().printOn(std::cout);

  // //If the fitness value is equal to 0, the best solution is found. Else try again.

  // std::cout << std::endl;  
  return EXIT_SUCCESS;
}
