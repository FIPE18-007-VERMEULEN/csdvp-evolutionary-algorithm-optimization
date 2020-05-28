using namespace std;
// temp
#include "moBitNeighbor.h"
// paradiseo
#include <eo>
#include <ga.h>
#include <mo>
#include <problems/bitString/moBitNeighbor.h>
#include <moeo>
#include <utils/moeoFullEvalByCopy.h>
// problem
#include<rmnkEval.h>
//MPI
#include <mpi.h>

// checks for help demand, and writes the status file and make_help; in libutils
void make_help(eoParser & _parser);

// objective vector of real values
typedef moeoRealObjectiveVector < moeoObjectiveVectorTraits > rmnkObjectiveVector;
// problem representation
typedef moeoBitVector < rmnkObjectiveVector >  RMNK;
// neighbor
typedef moBitNeighbor<RMNK,rmnkObjectiveVector> rmnkNeighbor;

// go
int main(int argc, char* argv[]){
  try{
    
    /*** init ***/
    eoParser parser(argc, argv);  // for user-parameter reading
    eoState state;                // to keep all things allocated
    
    // parameters
    eoValueParam<uint32_t>& _seedParam = parser.createParam(uint32_t(0), "seed", "Random number seed", 'S');
    std::string _inputFile = parser.createParam(std::string(), "inputfile", "", '\0', "Representation", true).value();
    std::string _outputFile = parser.createParam(std::string("/scratch"), "outputfile", "", '\0', "Representation", true).value();
    unsigned int PAS = parser.createParam((unsigned int)(1), "pas", "Nb of iteration between 2 print",'p',"Param").value();      
    unsigned int BEGIN = parser.createParam((unsigned int)(0), "begin", "Nb of iteration between 2 print",'p',"Param").value();      

    unsigned int _popSize = parser.createParam((unsigned int)(100), "popSize", "Population size", 'P', "Evolution Engine").value();
    unsigned int NBCORES = parser.createParam((unsigned int)(32), "nbCores", "Number of cores used",'C',"Param").value();    
    std::string STRAT = parser.createParam(std::string(""), "strat", "Strategy for the choice of neighbors",'S',"Param").value();
    unsigned int NBMUT = parser.createParam((unsigned int)(100), "nbMut", "Number of mutation",'X',"Param").value();
    double _pMut = parser.createParam((double)(1.0), "pMut", "mut rate", 'M', "Evolution Engine").value();
    unsigned int NBITER = parser.createParam((unsigned int)(100), "nbIter", "Number of iteration",'I',"Param").value();
    
    //for MPI    
    int numtasks, rank, tag1=1, tag2=2, prev, next;
    MPI_Request reqs[4];
    MPI_Status stats[4];
    double tmp, max, x, y, newx=1.0, newy=1.0, lambda0, lambda1, xref=0, yref=0;

    // seed
    if (_seedParam.value() == 0) _seedParam.value() = time(0);
    rng.reseed(_seedParam.value());

    //copy of objective vector for MPI
    double front[2][NBCORES];

    // eval function
    rmnkEval<RMNK,rmnkObjectiveVector> fEval(_inputFile.c_str());
    eoEvalFuncCounter<RMNK> eval(fEval);

    // number of objectives
    std::vector <bool> bObj(fEval.getM(), false);
    moeoObjectiveVectorTraits::setup(fEval.getM(), bObj);

    // initialiser
    eoUniformGenerator<bool> uGen;
    eoInitFixedLength<RMNK> init(fEval.getN(),uGen);

    // mutation (prob = _pMut/N)
    eoBitMutation<RMNK> mut (_pMut, true);

    // generate initial population
    RMNK sol;
    moeoUnboundedArchive<RMNK> archive;
    moeoUnboundedArchive<RMNK> finalArchive;

    while(archive.size()<NBCORES){
      init(sol);
      eval(sol);
      archive.push_back(sol);
    }

    //Print initial population
    /*
    std::cout << "POINTS INITIALS" << std::endl;
    for (unsigned int i=0; i<archive.size(); i++){
      cout << archive[i].objectiveVector() << " " << arch[i].fitness() << endl;
    }
    */

    // Copie des valeurs des objectifs      
    for(int i=0; i<NBCORES; i++){	  
      front[0][i]=archive[i].objectiveVector()[0];
      front[1][i]=archive[i].objectiveVector()[1];
    }

    // stockage local des objectif vector sur les coeurs P, P+1, P-1
    double local[3][2];

    //Init MPI
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    
    //On repartie les valeurs sur chaque coeurs
    MPI_Scatter(&front[0][0],1,MPI_DOUBLE,&local[0][0],1,MPI_DOUBLE,1,MPI_COMM_WORLD);
    MPI_Scatter(&front[1][0],1,MPI_DOUBLE,&local[0][1],1,MPI_DOUBLE,1,MPI_COMM_WORLD);

    //Print du front de départ
    //std::cout << rank << " " << local[0][0] << " " << local[0][1] << std::endl;
    
    //define the neighbor (P+1 et P-1)
    prev=rank-1;
    next=rank+1;

    // Keep best
    RMNK best;

    max=0;
	
    // main loop
    for(unsigned int i=0;i<NBITER; i++){

      // Communication with prev and next
      if(rank==0){	
	//prev n'existe pas
	local[2][0]=0;
	local[2][1]=0;
	
	//envoie des valeurs objectif au next
	MPI_Isend(&local[0][0], 2, MPI_DOUBLE, next, tag1, MPI_COMM_WORLD, &reqs[0]);
	
	//Reception des valeurs objectif du next
	MPI_Irecv(&local[1][0], 2, MPI_DOUBLE, next, tag2, MPI_COMM_WORLD, &reqs[1]);
	  
	MPI_Waitall(2, reqs, stats);
      }//endif(rank==0)
      else if(rank==(NBCORES-1)){
	//next n'existe pas
	local[1][0]=0;
	local[1][1]=0;
	
	//envoie des valeurs objectif au prev
	MPI_Isend(&local[0][0], 2, MPI_DOUBLE, prev, tag2, MPI_COMM_WORLD, &reqs[0]);
	
	//Reception des valeurs objectif du prev
	MPI_Irecv(&local[2][0], 2, MPI_DOUBLE, prev, tag1, MPI_COMM_WORLD, &reqs[1]);
	
	MPI_Waitall(2, reqs, stats);
      }//endif(rank==NBCORES-1)
      else{
	//envoie des valeurs objectif au next
	MPI_Isend(&local[0][0], 2, MPI_DOUBLE, next, tag1, MPI_COMM_WORLD, &reqs[0]);
	
	//Reception des valeurs objectif du next
	MPI_Irecv(&local[1][0], 2, MPI_DOUBLE, next, tag2, MPI_COMM_WORLD, &reqs[1]);	    
	
	//envoie des valeurs objectif au prev
	MPI_Isend(&local[0][0], 2, MPI_DOUBLE, prev, tag2, MPI_COMM_WORLD, &reqs[2]);
	
	//Reception des valeurs objectif du prev
	MPI_Irecv(&local[2][0], 2, MPI_DOUBLE, prev, tag1, MPI_COMM_WORLD, &reqs[3]);
	
	MPI_Waitall(4, reqs, stats);
      }
      
      // find a new solution
      bool better=false;

      //on calcul le score de la solution actuelle par rapport à ces nouveaux voisins
      if(rank!=0 && rank!=(NBCORES-1)){
	x=archive[rank].objectiveVector()[0];
	y=archive[rank].objectiveVector()[1];
	if(STRAT=="hyp"){	      
	  // si on apporte rien à l'hypervolume on prend une direction ortogonal par rapport aux voisins
	  if((x<=local[2][0]) || (y<=local[1][1])){
	    lambda0=std::abs(local[1][1]-local[2][1]);
	    lambda1=std::abs(local[1][0]-local[2][0]);
	    tmp=(lambda0*x + lambda1*y) * 0.000001;     
	  }
	  // sinon on prend l'hypervolume
	  else{
	    tmp=(x-local[2][0]) * (y-local[1][1]);	    
	  }
	}	  
	else if(STRAT=="ortho"){
	  lambda0=std::abs(local[1][1]-local[2][1]);
	  lambda1=std::abs(local[1][0]-local[2][0]);
	  tmp=lambda0*x + lambda1*y;     
	}
	max=tmp;	
      }
      else{
	max=-2;
      }

      // mutations 1/l
      for(unsigned int j=0; j<NBMUT; j++){     
	
	// copie de la solution 
	RMNK tmpSol;	
	tmpSol=archive[rank];      	
	double tmp1;
	double tmp2;
	
	// mutation de la copie
	mut(tmpSol);
	// on invalide la copie
	tmpSol.invalidate();
	// on evalue les valeurs objectifs de la copie
	eval(tmpSol);
	
	//on copie ces valeurs dans x et y (plus simple a ecrire quand mm)
	x=tmpSol.objectiveVector()[0];
	y=tmpSol.objectiveVector()[1];
	
	// extreme
	if(rank==0){
	  if((y >local[0][1]) || ((y==local[0][1])&& (x>local[0][0]))){
	    if(y > local[0][1])
	      tmp=y;
	    else
	      tmp=-1*1/x;
	    if(i==0 || tmp>max){
	      max=tmp;
	      best=tmpSol;
	      newx=x;
	      newy=y;
	      better=true;
	    }
	  }
	}
	else if(rank==(NBCORES-1)){
	  if((x >local[0][0]) || ((x ==local[0][0]) && (y >local[0][0])) ){
	    if(x >local[0][0])
	      tmp=x;
	    else
	      tmp=-1*1/y;
	    if(i==0 || tmp>max){
	      max=tmp;
	      best=tmpSol;
	      newx=x;
	      newy=y;
	      better=true;
	    }
	  }	  
	}
	//cas général
	else{
	  if(STRAT=="hyp"){	      
	    // si on apporte rien à l'hypervolume on prend une direction ortogonal par rapport aux voisins
	    if((x<=local[2][0]) || (y<=local[1][1])){
	      lambda0=std::abs(local[1][1]-local[2][1]);
	      lambda1=std::abs(local[1][0]-local[2][0]);
	      tmp=(lambda0*x + lambda1*y) * 0.000001;     
	    }
	    // sinon on prend l'hypervolume
	    else{
	      tmp=(x-local[2][0]) * (y-local[1][1]);	    
	    }
	  }	  
	  else if(STRAT=="ortho"){
	    lambda0=std::abs(local[1][1]-local[2][1]);
	    lambda1=std::abs(local[1][0]-local[2][0]);
	    tmp=lambda0*x + lambda1*y;     
	  }	
	  if(tmp>max){
	    // si on a trouvé une meilleur solution qu'avant on la sauvegarde	    
	    better=true;
	    max=tmp;
	    best=tmpSol;
	    newx=x;
	    newy=y;
	  }	  	
	}
      }
      // on affecte la meilleur solution trouvé
      if(better){
	local[0][0]=newx;
	local[0][1]=newy;
	archive[rank]=best;
      }


      //on attend tout le monde
      MPI_Barrier(MPI_COMM_WORLD);
      //on save le front régulierement
      if((i%PAS==0 && i>=BEGIN) || i==NBITER-1){
	std::string filename=_outputFile + "/rank.";
	std::ostringstream oss;
	if(rank<10)
	  oss << "00" << rank;
	else if(rank<100)
	  oss << "0" << rank;
	else
	  oss << rank;
	filename+=oss.str();
	std::ofstream outputfile(filename.c_str(), std::ios::app);
	outputfile << local[0][0] << " " << local[0][1] << std::endl;
	outputfile.close();
      }      
    }

    
    std::string filename=_outputFile + "/finalpop.";
    std::ostringstream oss;
    oss << rank;
    filename+=oss.str();
    std::ofstream outputfile(filename.c_str(), std::ios::app);
    archive[rank].printOn(outputfile);
    outputfile << std::endl;
    outputfile.close();
    filename=_outputFile + "/finalpoint.";
    oss << rank;
    filename+=oss.str();
    std::ofstream outputfile2(filename.c_str(), std::ios::app);
    outputfile2 << archive[rank].objectiveVector()[0] << " " << archive[rank].objectiveVector()[1] << std::endl;
    outputfile2.close();

    make_help(parser);
    
    MPI_Finalize();
    

  }
  catch (exception& e){
        cout << e.what() << endl;
  }
  return EXIT_SUCCESS;
}
