#include "evaluator.h"

void CursusEval::operator()(Cursus & _cursus){
  double fit=0.0;

  int pCE, pCP, pCR;

  pCE=1;
  pCR=1;
  pCP=1;

  std::pair<bool, double> resCE;
  std::pair<bool, double> resCP;
  std::pair<bool, double> resCR;
  
  resCE=ce.integrityCheck(_cursus);
  resCP=cp.integrityCheck(_cursus);
  resCR=cr.integrityCheck(_cursus);
  //std::cout << resCP.first << " " << resCP.second << std::endl;
  
  double sum=pCE*resCE.second + pCR*resCR.second + pCP*resCP.second;
  fit=1.0/(1+sum)*100;

  _cursus.fitness(fit);
}
