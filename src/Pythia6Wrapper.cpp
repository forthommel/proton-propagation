#include "Pythia6Wrapper.h"

bool
call_pygive(const std::string &line)
{
  int numWarn = pydat1.mstu[26]; // # warnings
  int numErr = pydat1.mstu[22]; // # errors
  pygive_(line.c_str(), line.length());
  return pydat1.mstu[26] == numWarn && pydat1.mstu[22] == numErr;
}

std::vector<TLorentzVector>
getPythiaParticles(bool stable_only)
{
  std::vector<TLorentzVector> out;
  for (unsigned int i=0; i<pyjets.n; i++) {
    if (stable_only and pyjets.k[0][i]!=1) continue;
    out.push_back(TLorentzVector(pyjets.p[0][i], pyjets.p[1][i], pyjets.p[2][i], pyjets.p[3][i]));
  }
  return out;
}

HepMC::GenEvent
getEvent()
{
  HepMC::GenEvent evt;
  evt.set_signal_process_id(pypars.msti[0]);
  evt.set_event_scale(pypars.pari[22]);
  evt.set_alphaQED(pyint1.vint[56]);
  evt.set_alphaQCD(pyint1.vint[57]);

  HepMC::GenCrossSection xs;
  const double cs_pb = pypars.pari[0]*1.e9, err_cs_pb = cs_pb/sqrt(pypars.msti[4]);
  xs.set_cross_section(cs_pb, err_cs_pb);
  evt.set_cross_section(xs);

  //FIXME
  evt.weights().push_back(pyint1.vint[96]*1.e9);
  evt.weights().push_back(1./(pyint1.vint[98]));
call_pylist(2);
  std::vector<HepMC::GenParticle*> hepmc_particles;
  for (int iprt=0; iprt<2; iprt++) {
    std::cout << iprt << " --> " << pyjets.k[2][iprt] << std::endl;
    HepMC::FourVector pmom(pyjets.p[0][iprt], pyjets.p[1][iprt], pyjets.p[2][iprt], pyjets.p[3][iprt]);
    int dstatus = getStatusFromPythia(pyjets.k[0][iprt]);
    HepMC::GenParticle* primary = new HepMC::GenParticle(pmom, pyjets.k[1][iprt], dstatus); //FIXME
    primary->suggest_barcode(iprt+1);
    //std::cout << pyjets.p[4][iprt] << std::endl;
    hepmc_particles.push_back(primary);
    /*HepMC::GenVertex* primVtx = new HepMC::GenVertex(HepMC::FourVector(pyjets.v[0][iprt], pyjets.v[1][iprt], pyjets.v[2][iprt], pyjets.v[3][iprt]));
    primVtx->add_particle_in(primary);
    evt.add_vertex(primVtx);*/
  }

  HepMC::GenVertex* v2 = new HepMC::GenVertex(HepMC::FourVector(pyjets.v[0][0], pyjets.v[1][0], pyjets.v[2][0], pyjets.v[3][0]));
  HepMC::GenVertex* v1 = new HepMC::GenVertex(HepMC::FourVector(pyjets.v[0][1], pyjets.v[1][1], pyjets.v[2][1], pyjets.v[3][1]));
  v1->add_particle_in(hepmc_particles[0]);
  v2->add_particle_in(hepmc_particles[1]);
  evt.add_vertex(v1);
  evt.add_vertex(v2);
  evt.set_beam_particles(hepmc_particles[0], hepmc_particles[1]);

  HepMC::GenVertex* DecVtx = 0;

  for (int iprt=2; iprt<pyjets.n; iprt++) {
    int parent = pyjets.k[2][iprt];

    HepMC::FourVector pmom(pyjets.p[0][iprt], pyjets.p[1][iprt], pyjets.p[2][iprt], pyjets.p[3][iprt]);
    int dstatus = getStatusFromPythia(pyjets.k[0][iprt]);
    //HepMC::GenParticle* daughter = new HepMC::GenParticle(pmom, HepPID::translatePythiatoPDT(pyjets.k[1][iprt]), dstatus);
    HepMC::GenParticle* daughter = new HepMC::GenParticle(pmom, pyjets.k[1][iprt], dstatus); //FIXME
    daughter->suggest_barcode(iprt+1);

    if (parent==0) {
      HepMC::GenVertex* v21 = new HepMC::GenVertex(HepMC::FourVector(pyjets.v[0][iprt], pyjets.v[1][iprt], pyjets.v[2][iprt], pyjets.v[3][iprt]));
      v21->add_particle_in(daughter);
      evt.add_vertex(v21);
    }
    else if (parent==1) { v1->add_particle_out(daughter); }
    else if (parent==2) { v2->add_particle_out(daughter); }
    else {
      HepMC::GenParticle* parentPart = evt.barcode_to_particle(parent);
      parentPart->set_status(2); // reset status, to mark that it's decayed
      DecVtx = new HepMC::GenVertex(HepMC::FourVector(pyjets.v[0][iprt], pyjets.v[1][iprt], pyjets.v[2][iprt], pyjets.v[3][iprt]));
      DecVtx->add_particle_in(parentPart);
      DecVtx->add_particle_out(daughter);
      evt.add_vertex(DecVtx);
    }

    int iprt1;
    for (iprt1=iprt+1; iprt1<pyjets.n; iprt1++) { // the pointer is shifted by -1, c++ style
      if (pyjets.k[2][iprt1]!=parent) break; // another parent particle, break the loop
      HepMC::FourVector pmomN(pyjets.p[0][iprt1], pyjets.p[1][iprt1], pyjets.p[2][iprt1], pyjets.p[3][iprt1]);
      dstatus = getStatusFromPythia(pyjets.k[0][iprt1], pyjets.k[0][iprt]);
      //HepMC::GenParticle* daughterN = new HepMC::GenParticle(pmomN, HepPID::translatePythiatoPDT(pyjets.k[1][iprt1]), dstatus);
      HepMC::GenParticle* daughterN = new HepMC::GenParticle(pmomN, pyjets.k[1][iprt1], dstatus); //FIXME
      daughterN->suggest_barcode(iprt1+1);
      if (parent==1) { v1->add_particle_out(daughterN); }
      else if (parent==2) { v2->add_particle_out(daughterN); }
      else { DecVtx->add_particle_out(daughterN); }
    }
    iprt = iprt1-1; // reset counter such that it doesn't go over the same child more than once
                    // don't forget to offset back into c++ counting, as it's already +1 forward
  }
  evt.set_signal_process_vertex(*(v1->vertices_begin()));
  return evt;
}

int
getStatusFromPythia(int py, int fallback)
{
  if (py<0) return 0;
  else if (py<=10) return 1;
  else if (py<=20) return 2;
  else if (py<=30) return 3;
  else if (py<=100) return fallback;
  else return -1;
}
