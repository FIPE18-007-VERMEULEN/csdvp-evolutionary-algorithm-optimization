#ifndef SRC_MODEL_EA_MUTATION_H_
#define SRC_MODEL_EA_MUTATION_H_

#include <algorithm>

#include <eoSwapMutation.h>
#include <eoShiftMutation.h>

#include "cursus.h"
#include "model/course.h"
#include "model/competency.h"
#include "model/tools.h"
#include "model/magnitude.h"

#include "model/constraints/repetitionConstraints.h"

#include "model/exception/competencyEvolvingException.h"

template <class EOT>
class mutCSDVP: public eoMonOp<EOT>
{
 public:
 mutCSDVP(CSDVP& _pb, ConstraintsRepetition& _ctr):pb(_pb),ctr(_ctr){}

  //_CourseID subastraction from _chrom
  void notin(std::vector<int>& _courseID, std::vector<int>& _chrom){
    unsigned int tmp;
    for (unsigned int i=0; i<_chrom.size(); i++){
      tmp=0;
      while(tmp<_courseID.size() && _chrom[i]!=_courseID[tmp])
	tmp++;
      if(tmp<=_courseID.size()){
	_courseID[tmp]=_courseID[_courseID.size()-1];
	_courseID.pop_back();
      }
    }
  }

  /** @version 1.0
   * @author Jérem
   */
  // virtual bool operator()(EOT& _chrom){
  //   bool res=false;    
  //   std::pair<bool, double> ctrRes= ctr.integrityCheck(_chrom);

  //   //Integrity Check AFTER CROSS
  //   assert(ctrRes.first);
    
  //   //_chrom.printOn(std::cout);
  //   //std::cout << std::endl;
  //   //On cherche une mutation
  //   unsigned int i;
  //   const std::vector<int> TF = pb.timeFrames();
  //   unsigned int nbTF=TF.size();
  //   unsigned int sizeTF=_chrom.size()/nbTF;
  //   const std::vector<Course> catalogue = pb.coursesCatalogue();
  //   //std::cout << "Catalogue Size: " << catalogue.size() << ", nbTF: " << nbTF << ", sizeTF: " << sizeTF << std::endl;
  //   //std::cout << "TF min: " << pb.cfg_courseByTFMin() << std::endl;
    
  //     std::vector<int> courseID;
  //     std::vector<int> TFavailabilities;
  //     for(i=0; i<catalogue.size(); i++)
	// courseID.push_back(i);
  //     //on garde que les cours pas encore choisi
  //     notin(courseID,_chrom);

  //     /*
  //     //std::cout << "id des cours dispos: ";
  //     for(int i=0; i<courseID.size(); i++)
	// std::cout << courseID[i] << " ";
  //     std::cout << std::endl;
  //     */

  //     //on en prend un au hasard dedans
  //     unsigned int newCourse=eo::rng.random(courseID.size());
  //     //std::cout << "newCourse: " << courseID[newCourse] << std::endl;      
  //     //on regarde les time frame dispo pour ce cours
  //     TFavailabilities=catalogue[courseID[newCourse]].timeFrame();

  //     //on tape au hazard dans une de ces time frame
  //     unsigned int replaceTF=eo::rng.random(TFavailabilities.size());
  //     unsigned int oldCourse=eo::rng.random(sizeTF);
  //     //std::cout << "Try to Replace " << _chrom[((TFavailabilities[replaceTF]-pb.cfg_minimalTimeFrame())*sizeTF) + oldCourse] << " by " << courseID[newCourse] << " in TF: " << TFavailabilities[replaceTF] << std::endl;
  //     _chrom[((TFavailabilities[replaceTF]-pb.cfg_minimalTimeFrame())*sizeTF) + oldCourse]=courseID[newCourse];

  //     //std::cout << std::endl << std::endl;
      
  //     ctrRes= ctr.integrityCheck(_chrom);
  //     res=true;
  //     _chrom.invalidate();
  //   return res;
  // }

  /**
   * This mutation choose a course randomly, then replace it with a course available in the current TF.
   * The course selection is made as follow :
   *  Select a course (at random) where its prerequisite are respected by the previous assignation
   *  Otherwise, pick the least constrained course in 60% of cases, otherwise random
   * Note that even if the select course's prerequisite may be respected, the _chrom is not guaranteed to be a solution, since the previous
   * assignement of course could be wrong.
   * @author Alex
   * @version 2.0
   * @todo optimize + define a global variable for random
   */ 
  virtual bool operator()(EOT& _chrom)
  {
    // CFG
    unsigned int cmptAlert = 0;
    unsigned int tresholdAlert = 10;
    unsigned int tresholdAbort = 100;
    // -----
    bool success=false;    
    std::pair<bool, double> ctrRes= ctr.integrityCheck(_chrom);

    //Integrity Check AFTER CROSS
    assert(ctrRes.first);

    // Mutation set up
      unsigned int i;
      const std::vector<int> TF = pb.timeFrames();
      unsigned int nbTF=TF.size();
      unsigned int sizeTF=_chrom.size()/nbTF;
      const std::vector<Course> catalogue = pb.coursesCatalogue();
      std::vector<int> courseID;
      std::vector<Course> availableC;
      std::vector<std::vector<Course>> sortedC;
      std::vector<int> availableTF;
      std::vector<Course> coursesOfTF;
      std::vector<Competency> statesOfComp;
      int rngTF = 0;
      int rngCourseToSwap;
      int tfPositionOfRngCourse;
      std::vector<Course> gfiCourse; //go for it courses : they pass the prereq
      Course fbBestCourse; //fb : Fallback

    // //we repeat here because mutation may fail if bad pick happens;
    // do
    // {
      for(i=0; i<catalogue.size(); i++)
        courseID.push_back(i);
      
      //Keeping only not taken courses
      notin(courseID,_chrom);
      
      //Filling availables courses
      for(i = 0; i < courseID.size(); i++) 
        availableC.push_back(catalogue.at(courseID.at(i)));

      // Sorting courses by TF
      sortedC = Course::organiseByTF(availableC, pb.timeFrames());  
      for(i = 0; i < sortedC.size(); i++)
        if(sortedC.at(i).size() > 0)
          availableTF.push_back(i);
      
      //Defining which course should be mutated (i.e. swapped with another non taken)
      rngCourseToSwap = eo::rng.random(_chrom.size());
      
      tfPositionOfRngCourse = rngCourseToSwap / sizeTF;
      coursesOfTF = sortedC.at(tfPositionOfRngCourse);

      if(coursesOfTF.size() == 0) //No mutation is possible !
      {
        std::random_shuffle(courseID.begin(), courseID.end());
        _chrom[rngCourseToSwap] = courseID.at(0);
        _chrom.invalidate();
        return false; //No mutation is possible !
      }

      //Defining the status of competencies mastery until the specified TF
      statesOfComp = _getCompStatusAtTF(pb.coursesCatalogue(), _chrom, tfPositionOfRngCourse, sizeTF);

      //Taking the best course OR the one with the less of prereq
      gfiCourse = _getOKCourse(coursesOfTF, statesOfComp);

      if(gfiCourse.size() > 0)
      {
        std::random_shuffle(gfiCourse.begin(), gfiCourse.end());
        _chrom[rngCourseToSwap] = pb.mapCourseToPosition(gfiCourse.at(0));        
      }
      else //least constraint courses in 25% of the cases, otherwise full rand
      {
        if(eo::rng.random(100) > RATIO_RANDOM_VS_BEST)
        {
          fbBestCourse = coursesOfTF.at(0);
          for(i = 1; i < coursesOfTF.size(); i++)
          {
            if(fbBestCourse.prerequisites().size() > coursesOfTF.at(i).prerequisites().size())
              fbBestCourse = coursesOfTF.at(i);
          }
          _chrom[rngCourseToSwap] = pb.mapCourseToPosition(fbBestCourse);
        }
        else
        {
          std::random_shuffle(coursesOfTF.begin(), coursesOfTF.end());
          _chrom[rngCourseToSwap] = pb.mapCourseToPosition(coursesOfTF.at(0));
        }
      }
      


    //   if(!success)
    //   {
    //     cmptAlert++;
    //     if(cmptAlert % tresholdAlert == 0)
    //       std::cout << "INFO! Several mutation do not success currently" << std::endl;
    //     if(cmptAlert >= tresholdAbort)
    //     {
    //       std::cout << "WARNING! Max mutation reroll reached. We crash the program." << std::endl;
    //       assert(false);
    //     }
    //   }
    // } while (!success);
    _chrom.invalidate();
    return false;

  }

  /** _getCompStatusAtTF returns the status of the magnitude of all the competencies sawn in all the timeframes below TF.
   */ 
  std::vector<Competency> _getCompStatusAtTF(const std::vector<Course>& catalogue, std::vector<int>& _chrom, int TF, int nbCbyTF)
  {
    std::vector<Competency> compStatus;
    std::vector<Competency> prereqs;
    Course tmpCourse = Course::buildTMP();
    Course currentCourse;
    Competency tmpComp;
    bool addStatus = false;
    int pos=0;

    bool changedTF = false;
    int currentTF = 0;

    for(unsigned int i = 0; i < _chrom.size() && (i / nbCbyTF < TF); i++)
    {
      currentCourse = catalogue.at(_chrom.at(i));

      if(currentTF != i / nbCbyTF)
        changedTF = true;
      else
        changedTF = false;
      currentTF = i / nbCbyTF;

      if(changedTF) //if we have changed of tf, lets improve decay for all comp
      {
        for(unsigned int j = 0; j < tmpCourse.prerequisites().size(); j++)
        {
          tmpCourse.unlocked_prerequisites().at(j).increaseDecay();
        }
      }

      for(unsigned int j = 0; j < currentCourse.teachedCompetenciesWeighted().size(); j++)
      {
        tmpComp = currentCourse.teachedCompetenciesWeighted().at(j).first;
        addStatus = tmpCourse.addPrerequisite(tmpComp);

        if(!addStatus) // comp already exist, then lets evolve it instead
        {
          prereqs = tmpCourse.prerequisites();
          std::pair<int, Competency> fiv= findInVector(prereqs, tmpComp);
          pos = fiv.first;

          try
          {
            Magnitude mag = currentCourse.teachedCompetenciesWeighted().at(j).first.c_magnitude();
            //tmpCourse.unlocked_prerequisites().at(pos).saveDecay();
            tmpCourse.unlocked_prerequisites().at(pos).evolveTowards(mag);
          }
          catch(CompetencyEvolvingException & e)
          {
            //Should has NTD here
            //auto rebased
          }
        }
      } 
    }

    compStatus = tmpCourse.prerequisites();
    for(unsigned int i = 0; i < compStatus.size(); i++)
      compStatus.at(i).saveDecay();

    return compStatus;
  }

  std::vector<Course> _getOKCourse(std::vector<Course> availableC, std::vector<Competency> state)
  {
    std::vector<Course> res;
    bool stop = false;
    bool found = false;
    bool isPrereqOK = true;
    unsigned int j = 0;
    unsigned int itState = 0;

    Competency checkCmp;

    for(unsigned int i = 0; i < availableC.size(); i++)
    {
      isPrereqOK = true;

      for(j = 0; j < availableC.at(i).prerequisites().size() && isPrereqOK; j++)
      {
        checkCmp = availableC.at(i).prerequisites().at(j);
        stop = false;
        found = false;

        for(itState = 0; itState < state.size() && !stop; itState++)
        {
          if(checkCmp == state.at(itState))
          {
            if(checkCmp.c_magnitude().value() > state.at(itState).decay())
            { stop = true; found = false; }
            else
            { stop = true; found = true;  }
          } 
        }

        if(found)
          isPrereqOK = true;
        else
          isPrereqOK = false;
      }

      if(isPrereqOK)
        res.push_back(availableC.at(i));
    }

    return res;
  }

 private:
  CSDVP pb;
  ConstraintsRepetition ctr;
  
};

typedef mutCSDVP<Cursus> CursusMutation;




#endif // SRC_MODEL_EA_MUTATION_H_
