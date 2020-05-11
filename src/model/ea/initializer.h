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

        eoInitCSDVP(unsigned chromSize, unsigned maxVal, int _seed)
            : _chromSize(chromSize), _maxVal(maxVal)
        {}

        virtual void operator()(EOT& chrom)
        {
            chrom.resize(_chromSize);

            std::vector<int> rdVec;
            for(int i = 0; i < this->_maxVal; i++)
                rdVec.push_back(i);
            std::random_shuffle(rdVec.begin(), rdVec.end());

            std::vector<int> r2; //trying tmp struct
            for(int i = 0; i < rdVec.size(); i++)
                r2.push_back(rdVec.at(i));

            for(unsigned idx=0;idx <chrom.size();idx++)
            {
                unsigned int tmp = r2.at(idx);
                // chrom[idx]=tmp; // DOES NOT WORK
                chrom[idx] = idx; //DOES WORK;
            }

            //std::random_shuffle(chrom.begin(), chrom.end(),gen);
            chrom.invalidate();
        }

    private :
        unsigned _chromSize;
        unsigned _maxVal;
        UF_random_generator<unsigned int> gen;
        int _seed;
};

typedef eoInitCSDVP<Cursus> CursusInit;

#endif // SRC_MODEL_EA_INITIALIZER_H_