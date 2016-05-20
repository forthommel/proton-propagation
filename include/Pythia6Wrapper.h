#ifndef Pythia6Wrapper_h
#define Pythia6Wrapper_h

#include <vector>
#include "TLorentzVector.h"
#include "HepMC/PythiaWrapper.h"

extern "C" 
{
  // subroutines
  void pygive_(const char* chin, int sz);
}

bool call_pygive(const std::string &line);
std::vector<TLorentzVector> getPythiaParticles(bool stable_only=false);

#endif
