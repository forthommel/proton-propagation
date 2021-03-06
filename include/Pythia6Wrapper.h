#ifndef Pythia6Wrapper_h
#define Pythia6Wrapper_h

#include <vector>
#include "TLorentzVector.h"
#include "HepMC/PythiaWrapper.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenEvent.h"
#include "HepMC/Units.h"
//#include "HepPID/ParticleIDTranslations.hh"


extern "C"
{
  // subroutines
  void pygive_(const char* chin, int sz);
  int pychge_(int& pdgid);
}

bool call_pygive(const std::string &line);
inline int call_pychge(int pdgid) { return pychge_(pdgid); }
std::vector<TLorentzVector> getPythiaParticles(bool stable_only=false);
int getStatusFromPythia(int py, int fallback=-1);
HepMC::GenEvent getEvent();

#endif
