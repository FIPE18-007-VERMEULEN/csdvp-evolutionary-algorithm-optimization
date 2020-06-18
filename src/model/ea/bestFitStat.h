#ifndef SRC_MODEL_EA_BESTFITSTAT_H_
#define SRC_MODEL_EA_BESTFITSTAT_H_

#include <eo>
#include <fstream>
#include <iostream>
#include "cursus.h"

template <class EOT>
class bestFitStat: public eoStat<EOT, typename EOT::Fitness>
{
 public:
  
  typedef typename EOT::Fitness Fitness;

 bestFitStat(int _interval, std::string _filename, std::string _description="best "):eoStat<EOT, Fitness>(Fitness(), _description), interval(_interval),filename(_filename){
    cpt=0;
  }

  void operator()(const eoPop<EOT>& _pop){
    if((cpt%interval)==0){
      std::ofstream outputfile(filename.c_str(), std::ios::app);
      outputfile << cpt << ":\t" << std::endl;
      _pop.best_element().printOn(outputfile);
      outputfile << std::endl;
      outputfile.close();
    }
    cpt++;
  }

  virtual void lastCall(eoPop<EOT>& _pop){
    std::ofstream outputfile(filename.c_str(), std::ios::app);
    outputfile << cpt << ":\t" << std::endl;
    _pop.best_element().printOn(outputfile);
    outputfile << std::endl;
    outputfile.close();
  }
  
  int interval;
  std::string filename;
  int cpt;
};

#endif // SRC_MODEL_EA_BESTFITSTAT_H_
