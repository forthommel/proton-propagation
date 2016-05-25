#include <iostream>
#include "H_BeamLine.h"
#include "Pythia6Wrapper.h"

using namespace std;

int main(int argc, char* argv[])
{
  cout << endl << ">>> First step: Creating the event with Pythia..." << endl << endl;
  call_pygive("MSTU(13)=0"); // silence printouts from PYGIVE, unless display requested
  call_pygive("MSTP(81)=1"); // multiple interactions
  //call_pygive("MSTP(51)=10042"); // structure function chosen (external PDF CTEQ6L1)
  call_pygive("MSTP(52)=1");
  //call_pygive("MSTP(52)=2"); // work with LHAPDF
  call_pygive("MSTP(81)=21"); // MPI 21 is Pythia new set of MPI models
  call_pygive("MSTP(82)=5"); // MPI model
  call_pyinit("CMS", "p", "p", 14000.);
  //call_pystat();
  call_pyevnt();
  //call_pylist(1);

  HepMC::GenEvent ev = getEvent();
  //ev.print();

  //FIXME populate me!!

  cout << endl << ">>> Second step: Propagating the output event through the beamline..." << endl << endl;
  // initialize the beamlines
  H_BeamLine b1, b2;
  b1.fill("external/hector/data/LHCB1IR5_v6.500.tfs", +1, "IP5");
  b2.fill("external/hector/data/LHCB2IR5_v6.500.tfs", -1, "IP5");
  //FIXME from CMSSW
  b1.offsetElements(120, -0.097); b1.calcMatrix();
  b2.offsetElements(120, +0.097); b2.calcMatrix();
  cout << " --> interaction points: " << b1.getIP() << " -- " << b2.getIP() << endl
       << " --> number of elements: " << b1.getNumberOfElements() << " -- " << b2.getNumberOfElements() << endl;

  for(HepMC::GenEvent::particle_const_iterator p=ev.particles_begin(); p!=ev.particles_end(); p++) {
    if ((*p)->status()!=1) continue; // we only keep stable, final state particles
    unsigned int id = (*p)->barcode();
  }

  return 0;
}
