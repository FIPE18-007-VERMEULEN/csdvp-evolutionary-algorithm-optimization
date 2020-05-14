#ifndef SRC_MODEL_EA_MUTATION_H_
#define SRC_MODEL_EA_MUTATION_H_

#include <eoSwapMutation.h>
#include <eoShiftMutation.h>

#include "cursus.h"
#include "model/constraints/repetitionConstraints.h"

template <class EOT>
class mutCSDVP: public eoMonOp<EOT>
{
 public:
  
 mutCSDVP(CSDVP& _pb, ConstraintsRepetition& _ctr):pb(_pb),ctr(_ctr){}

  //_CourseID subastraction from _chrom
  void notin(std::vector<int>& _courseID, std::vector<int>& _chrom){
    int tmp;
    for (int i=0; i<_chrom.size(); i++){
      tmp=0;
      while(tmp<_courseID.size() && _chrom[i]!=_courseID[tmp])
	tmp++;
      if(tmp<=_courseID.size()){
	_courseID[tmp]=_courseID[_courseID.size()-1];
	_courseID.pop_back();
      }
    }
  }

  
  virtual bool operator()(EOT& _chrom){
    bool res=false;    
    std::pair<bool, double> ctrRes= ctr.integrityCheck(_chrom);

    //Integrity Check AFTER CROSS
    assert(ctrRes.first);
      
    //_chrom.printOn(std::cout);
    //std::cout << std::endl;
    //On cherche une mutation
    unsigned int i;
    const std::vector<int> TF = pb.timeFrames();
    unsigned int nbTF=TF.size();
    unsigned int sizeTF=_chrom.size()/nbTF;
    const std::vector<Course> catalogue = pb.coursesCatalogue();
    //std::cout << "Catalogue Size: " << catalogue.size() << ", nbTF: " << nbTF << ", sizeTF: " << sizeTF << std::endl;
    //std::cout << "TF min: " << pb.cfg_courseByTFMin() << std::endl;
    
      std::vector<int> courseID;
      std::vector<int> TFavailabilities;
      for(i=0; i<catalogue.size(); i++)
	courseID.push_back(i);
      //on garde que les cours pas encore choisi
      notin(courseID,_chrom);

      /*
      //std::cout << "id des cours dispos: ";
      for(int i=0; i<courseID.size(); i++)
	std::cout << courseID[i] << " ";
      std::cout << std::endl;
      */

      //on en prend un au hasard dedans
      unsigned int newCourse=eo::rng.random(courseID.size());
      //std::cout << "newCourse: " << courseID[newCourse] << std::endl;      
      //on regarde les time frame dispo pour ce cours
      TFavailabilities=catalogue[courseID[newCourse]].timeFrame();

      //on tape au hazard dans une de ces time frame
      unsigned int replaceTF=eo::rng.random(TFavailabilities.size());
      unsigned int oldCourse=eo::rng.random(sizeTF);
      //std::cout << "Try to Replace " << _chrom[((TFavailabilities[replaceTF]-pb.cfg_minimalTimeFrame())*sizeTF) + oldCourse] << " by " << courseID[newCourse] << " in TF: " << TFavailabilities[replaceTF] << std::endl;
      _chrom[((TFavailabilities[replaceTF]-pb.cfg_minimalTimeFrame())*sizeTF) + oldCourse]=courseID[newCourse];

      //std::cout << std::endl << std::endl;
      
      ctrRes= ctr.integrityCheck(_chrom);
      res=true;
      _chrom.invalidate();
    return res;
  }

 private:
  CSDVP pb;
  ConstraintsRepetition ctr;
  
};

typedef mutCSDVP<Cursus> CursusMutation;




#endif // SRC_MODEL_EA_MUTATION_H_
