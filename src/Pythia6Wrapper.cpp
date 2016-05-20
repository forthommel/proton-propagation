#include "Pythia6Wrapper.h"

bool
call_pygive(const std::string &line)
{
  int numWarn = pydat1.mstu[26]; // # warnings
  int numErr = pydat1.mstu[22];	// # errors
  pygive_(line.c_str(), line.length());
  return pydat1.mstu[26] == numWarn && pydat1.mstu[22] == numErr;
}

std::vector<TLorentzVector>
getPythiaParticles(bool stable_only)
{
  std::vector<TLorentzVector> out;
  for (unsigned int i=0; i<pyjets.n; i++) {
    if (stable_only and pyjets.k[0][i]!=0) continue;
    out.push_back(TLorentzVector(pyjets.p[0][i], pyjets.p[1][i], pyjets.p[2][i], pyjets.p[3][i]));
  }
  return out;
}
