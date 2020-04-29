#include <eo>
#include <queen.h>
#include <queenInit.h>
#include <queenMutation.h>
#include <queenCrossover.h>
#include <queenEval.h>

int main(int argc, char* argv[]){

  //Define a QUEEN -> 1 line
  QUEEN s1;

  //Define an initializer -> 1 line
  queenInit init(8,1);

  //Define the evaluation function -> 1 line
  queenEval eval;

  //Define mutation -> 1 line
  queenMutation mut;

  //Define crossover -> 1 line
  queenCrossover cross;

  //Define a generational continuator (put 100 generation for example) -> 1 line
  eoGenContinue<QUEEN> cont(100);

  //Define the transformation object (it contains, the crossover, the crossover rate, the mutation and the mutation rate) -> 1 line
  eoSGATransform<QUEEN> transform(cross, 0.1, mut, 0.1);

  //Define a selection method that selects ONE individual by deterministic tournament(put the tournament size at 2 for example) -> 1 line 
  eoDetTournamentSelect<QUEEN> selectOne(2);

  //Define a "eoSelectPerc" with the tournament with default parameter (allow to select the good size of individuals) -> 1 line
  eoSelectPerc<QUEEN> select(selectOne);

  //Define a generational replacement strategy -> 1 line
  eoGenerationalReplacement<QUEEN> replace;

  //Define a pop of QUEEN -> 1 line
  eoPop<QUEEN> pop;

  //Fill the pop with 100 initialized and evaluated QUEEN
  //Use the initializer, the evaluation function and the push_back operator's vector -> A "for" included three insrtuctions
  for(unsigned int i=0; i<100; i++){
    init(s1);
    eval(s1);
    pop.push_back(s1);
  }
  
  //Print the pop -> 1 line
  pop.printOn(std::cout);

  //HERE you can test whether you succeded in initializing the population by compiling and executing this part of the program.
  
  //Print end of line (endl)
  std::cout << std::endl;
  
  /*Define an eoEasyEA with good parameter:
    - continuator
    - evaluation function
    - eoSelectPerc
    - transformation object
    - replacement
   */
  // -> 1 line
  eoEasyEA<QUEEN> algo(cont,eval,select,transform,replace);

  //run the algorithm on the initialized population -> 1 line
  algo(pop);

  //Print the best element -> 1 line
  pop.best_element().printOn(std::cout);

  //If the fitness value is equal to 0, the best solution is found. Else try again.

  std::cout << std::endl;  
  return EXIT_SUCCESS;
}
