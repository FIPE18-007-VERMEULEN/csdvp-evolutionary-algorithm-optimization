#include <queenEval.h>

void queenEval::operator()(QUEEN & _queen){
  double fit=0.0;
  for(int i=0; i<_queen.size()-1; i++)
    for(int j=i+1; j< _queen.size(); j++)
      if((_queen[i]+j-i == _queen[j]) || (_queen[i]+i-j == _queen[j]))
	fit+=1.0;
  _queen.fitness(fit);
}
