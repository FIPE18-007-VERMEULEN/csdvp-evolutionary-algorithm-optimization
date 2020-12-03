#include "evaluator.h"

#include "../tools.h"

double CursusEval::WEIGHT_ECTS = 1;
double CursusEval::WEIGHT_REPETION = 1;
double CursusEval::WEIGHT_JOB = 1;
double CursusEval::WEIGHT_PREREQ = 1;

void CursusEval::operator()(Cursus & _cursus){
  double fit=0.0;

  double pCE, pCP, pCR, pCPR;

  pCE = WEIGHT_ECTS;
  pCR = WEIGHT_REPETION;
  pCP = WEIGHT_JOB;
  pCPR= WEIGHT_PREREQ;
  
  std::pair<bool, double> resCE;
  std::pair<bool, double> resCP;
  std::pair<bool, double> resCR;
  std::pair<bool, double> resCPR;				  
  
  resCE=ce.integrityCheck(_cursus);
  resCP=cp.integrityCheck(_cursus);
  resCR=cr.integrityCheck(_cursus);
  resCPR=cpr.integrityCheck(_cursus);
  
  /*
  std::cout << "EVAL: ";
  std::cout << resCE.first << " " << resCE.second << std::endl;
  std::cout << resCP.first << " " << resCP.second << std::endl;
  std::cout << resCR.first << " " << resCR.second << std::endl;
  std::cout << resCPR.first << " " << resCPR.second << std::endl;
  std::cout << std::endl << std::endl;
  */
  double sum=pCE*resCE.second + pCR*resCR.second + pCP*resCP.second + pCPR*resCPR.second;
  fit=1.0/(1+sum)*100;

  _cursus.fitness(fit);
}
