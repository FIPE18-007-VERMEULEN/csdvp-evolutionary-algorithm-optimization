#include "evaluator.h"

void CursusEval::operator()(Cursus & _cursus){
  double fit=0.0;
  
  fit = 1.0;
  _cursus.fitness(fit);
}
