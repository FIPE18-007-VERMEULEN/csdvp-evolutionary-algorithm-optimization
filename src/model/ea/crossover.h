#ifndef SRC_MODEL_EA_CROSSOVER_H_
#define SRC_MODEL_EA_CROSSOVER_H_


template <class EOT>
class crossCSDVP: public eoQuadOp<EOT>
{

 public:
  
 crossCSDVP(CSDVP& _pb, ConstraintsRepetition& _ctr, CursusInit& _init):pb(_pb),ctr(_ctr), init(_init), cpt(0){}

  
  virtual bool operator()(EOT& _chrom1, EOT& _chrom2){
    cpt++;
    //Integrity Check BEFORE CROSS
    std::pair<bool, double> ctrRes1= ctr.integrityCheck(_chrom1);
    std::pair<bool, double> ctrRes2= ctr.integrityCheck(_chrom2);
    assert(ctrRes1.first && ctrRes2.first);

    //Cross by using tmp solution
    EOT tmp;
    tmp.resize(0);
    for(int i=0; i<_chrom1.size(); i++)
      tmp.push_back(_chrom1[i]);
    cross(_chrom1,_chrom2);
    cross(_chrom2, tmp);

    //Integrity Check AFTER CROSS
    ctrRes1= ctr.integrityCheck(_chrom1);
    ctrRes2= ctr.integrityCheck(_chrom2);
    assert(ctrRes1.first && ctrRes2.first);
    
    return true;
  }

  //Cross modify _chrom1
  void cross(EOT& _chrom1,const EOT& _chrom2){
    bool needRepair=false;
    bool errorFound=false;
    unsigned int i, tmp;
    const std::vector<int> TF = pb.timeFrames();
    unsigned int nbTF=TF.size();
    unsigned int sizeTF=_chrom1.size()/nbTF;
    std::vector<Course> catalogue = pb.coursesCatalogue();
    std::vector<int> courseID;
    //Create a vector with all position
    for(i=0; i<catalogue.size(); i++)
      courseID.push_back(i);      

    //Define a cut point between time frames
    unsigned int cut = eo::rng.random(nbTF-1)+1;
    //copie right part from cut with chrom2, -1 if impossible
    for(i=cut*sizeTF; i<_chrom1.size(); i++){
      if(notin(_chrom1, _chrom2[i],i)){
	_chrom1[i]=_chrom2[i];
      }
      else{
	_chrom1[i]=-1;
	needRepair=true;
      }
    }

    //Repair
    if(needRepair){
      //search a course available in the TF for each -1 (impossible position of previous step)
      coursenotin(courseID, _chrom1);
      for(i=0; i<_chrom1.size(); i++){
	if(_chrom1[i]==-1){
	  tmp=0;	  
	  while(!isInTF(catalogue[courseID[tmp]], (i/sizeTF)+pb.cfg_minimalTimeFrame()) && tmp < courseID.size()){
	    tmp++;
	    }
	  if(tmp<courseID.size()){
	    _chrom1[i]=courseID[tmp];
	    remove(courseID, _chrom1[i]);
	  }
	  else{
	    //no possibility
	    errorFound=true;
	  }
	}
      }
      
    }
    //If no possibility, reinit the sol
    if(errorFound){
      std::cout << "Impossibility in Cross > Reinit sol" << std::endl;
      init(_chrom1);
    }
    
  }

  //Return if a course can be in a TF
  bool isInTF(Course& _c, int _TF){
    bool res=false;
    std::vector<int> tf=_c.timeFrame();
    for(int i=0; i<tf.size(); i++)
      if(_TF=tf[i])
	res=true;
    return res;
  }

  //remove an _id from a vector
  void remove(std::vector<int>& _chrom, int _id){
    int i=0;
    while(_chrom[i]!= _id)
      i++;
    _chrom[i]=_chrom[_chrom.size()-1];
    _chrom.pop_back();
  }

  //return if a id is not in the left side of a sol (side depend of _size)
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

  //_CourseID subastraction from _chrom
  void coursenotin(std::vector<int>& _courseID, std::vector<int>& _chrom){
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

  int cpt;
 private:
  
  CSDVP pb;
  ConstraintsRepetition ctr;
  CursusInit init;
};

typedef crossCSDVP<Cursus> CursusCrossover;

#endif // SRC_MODEL_EA_CROSSOVER_H_
