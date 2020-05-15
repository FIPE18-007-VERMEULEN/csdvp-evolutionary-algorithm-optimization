#ifndef SRC_MODEL_EA_INITIALIZER_H_
#define SRC_MODEL_EA_INITIALIZER_H_

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
class eoInitCSDVP: public eoInit<EOT>
{
    public:

    typedef typename EOT::AtomType AtomType;

        eoInitCSDVP(unsigned _chromSize, unsigned _maxVal, int _seed)
	  : chromSize(_chromSize), maxVal(_maxVal), seed(_seed)
        {}

        virtual void operator()(EOT& chrom)
        {
	  chrom.resize(0);
	  for(int i = 0; i < maxVal; i++)
	    chrom.push_back(i);
	  std::random_shuffle(chrom.begin(), chrom.end());
	  chrom.resize(chromSize);
          chrom.invalidate();
        }

    private :
        unsigned chromSize;
        unsigned maxVal;
        UF_random_generator<unsigned int> gen;
        int seed;
};

typedef eoInitCSDVP<Cursus> CursusInit;

#endif // SRC_MODEL_EA_INITIALIZER_H_
