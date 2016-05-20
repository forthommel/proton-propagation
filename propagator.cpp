#include <iostream>
#include "H_BeamLine.h"
#include "Pythia6.h"
//#include "HepMC/PythiaWrapper.h"

using namespace std;

int main(int argc, char* argv[])
{
  H_BeamLine b1, b2;
  b1.fill("external/hector/data/LHCB1IR5_v6.500.tfs");
  b2.fill("external/hector/data/LHCB2IR5_v6.500.tfs");
  cout << " --> interaction points: " << b1.getIP() << " -- " << b2.getIP() << endl;

  char* test = "haha";
  cout << sizeof(test) << "/" << sizeof(size_t) << endl;

  //cout << pypars_.mstp[2] << endl;
  //cout << pyint6_.proc[0] << endl;
  pygive("MSTP(81)=1"); // multiple interactions
  pygive("MSTP(51)=10042"); // structure function chosen (external PDF CTEQ6L1)
  //pygive("MSTP(52)=2"); // work with LHAPDF
  pygive("MSTP(81)=21"); // MPI 21 is Pythia new set of MPI models
  pygive("MSTP(82)=5"); // MPI model
  pyinit("NONE", "", "", 0.);
  //pyexec();
  pylist(1);

  return 0;
}
