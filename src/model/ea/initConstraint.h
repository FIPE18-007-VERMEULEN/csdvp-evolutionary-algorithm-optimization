#ifndef SRC_MODEL_EA_INITCONSTRAINT_H_
#define SRC_MODEL_EA_INITCONSTRAINT_H_

#include<queue>
#include<vector>
#include<cassert>
#include<algorithm>
#include<random>
#include<functional>

#include <eoInit.h>

#include "model/problem.h"
#include "cursus.h"

template <class EOT>
class eoInitConstraintCSDVP: public eoInit<EOT>
{
 public:

  typedef typename EOT::AtomType AtomType;

 eoInitConstraintCSDVP(CSDVP& _pb): pb(_pb){
    std::vector<int> tmp;
    chromSize=pb.getQuantityCoursesToPick();
    maxVal=pb.coursesCatalogue().size();
    seed=pb.seed();
    TF = pb.timeFrames();
    nbTF=TF.size();
    sizeTF=chromSize/nbTF;
    catalogue = pb.coursesCatalogue();
    possibleIDbyTF.resize(nbTF);
    for(int i=0; i<maxVal; i++){
      tmp=catalogue[i].timeFrame();
      for(int j=0; j<tmp.size(); j++){
	possibleIDbyTF[tmp[j]-pb.cfg_minimalTimeFrame()].push_back(i);
      }
    }
  //   for(int i=0; i<possibleIDbyTF.size(); i++){
  //     std::cout << "Possible course in TF " << i+pb.cfg_minimalTimeFrame() << ": ";
  //     for(int j=0; j<possibleIDbyTF[i].size(); j++){
	// std::cout << possibleIDbyTF[i][j] << " ";
  //     }
  //     std::cout << std::endl;
  //   }
  }
  
  virtual void operator()(EOT& chrom){
    int cpt=0;
    //std::cout << "Enter init" << std::endl;
    unsigned int r=eo::rng.random(possibleIDbyTF[0].size());
    chrom.resize(0);
    chrom.push_back(possibleIDbyTF[0][r]);
    //std::cout << "push " << possibleIDbyTF[0][r] << std::endl;
    for(int i = 1; i < chromSize; i++){
      cpt=0;
      r=eo::rng.random(possibleIDbyTF[i/sizeTF].size());
      while(!notin(chrom, possibleIDbyTF[i/sizeTF][r], i) && cpt<maxVal){
	r=eo::rng.random(possibleIDbyTF[i/sizeTF].size());
	cpt++;
      }
      if(cpt<maxVal){
	//std::cout << i << " " << r << " ";
	chrom.push_back(possibleIDbyTF[i/sizeTF][r]);
	//std::cout << "push " << possibleIDbyTF[i/sizeTF][r] << std::endl;
      }
      else{
	r=eo::rng.random(maxVal);
	while(!notin(chrom, r, i))
	  r=eo::rng.random(maxVal);
	chrom.push_back(r);
	std::cout << "push with error " << r << std::endl;
      }

    }
    chrom.invalidate();
    // chrom.printOn(std::cout);
    //std::cout << std::endl;

    //std::cout << "End init" << std::endl;
  }

  bool notin(EOT& _chrom, int _id, int _size){
    bool res=true;
    int i=0;
    while(res && i<_size){
      if(_chrom[i]==_id)
	res=false;
      i++;
    }
    return res;
  }

 private :
  std::vector< std::vector<int> > possibleIDbyTF;
  std::vector<Course> catalogue;
  std::vector<int> TF;
  unsigned int nbTF;
  unsigned int sizeTF;
  CSDVP pb;
  unsigned chromSize;
  unsigned maxVal;
  UF_random_generator<unsigned int> gen;
  int seed;
};

typedef eoInitConstraintCSDVP<Cursus> CursusInitConstraint;

#endif // SRC_MODEL_EA_INITIALIZER_H_
