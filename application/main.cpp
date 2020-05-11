#include <eo>
#include <queen.h>
#include <queenInit.h>
#include <queenMutation.h>
#include <queenCrossover.h>
#include <queenEval.h>

#include <model/magnitude.h>
#include <model/competency.h>

#include <model/ea/cursus.h>
#include <model/ea/initializer.h>
#include <model/ea/mutation.h>
#include <model/ea/crossover.h>
#include <model/ea/evaluator.h>

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
  //CSDVP pb;

  Cursus c1;
  CursusInit init(8,1); //init(pb.getQuantityCoursesToPick(),1); 
  CursusEval eval;
  CursusMutation mut;
  CursusCrossover xOver;

  eoGenContinue<Cursus> cont(100); // runs for 100 gen
  
  //xOver, xOver rate, mutation, mutation rate
  eoSGATransform<Cursus> transform(xOver, 0.1, mut, 0.1);
  eoDetTournamentSelect<Cursus> selectOne(2); //selection method by tournament, here against 2
  eoSelectPerc<Cursus> select(selectOne);
  eoGenerationalReplacement<Cursus> replace;
  eoPop<Cursus> pop;

  /**@todo make size of the pb accessible as well as size of an individu*/
  int size_of_the_pb = 100;
  for(int i = 0; i < size_of_the_pb; i++)
  {
    init(c1);
    eval(c1);
    pop.push_back(c1);
  }

  std::cout << "===== CURRENT POP =====" << std::endl;
  pop.printOn(std::cout);
  std::cout << "=====             =====" << std::endl;

  eoEasyEA<QUEEN> algo(cont,eval,select,transform,replace);

  algo(pop);

  std::cout << "===== BEST INDIVIDU =====" << std::endl;
  pop.best_element().printOn(std::cout);
  std::cout << " fitness:" << pop.best_element().fitness() << std::endl;
  std::cout << "===============" << std::endl;

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
