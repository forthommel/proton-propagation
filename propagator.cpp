#include <iostream>
#include "H_BeamLine.h"
#include "Pythia6.h"

using namespace std;

int main(int argc, char* argv[])
{
  H_BeamLine b1, b2;
  b1.fill("external/hector/data/LHCB1IR5_v6.500.tfs");
  b2.fill("external/hector/data/LHCB2IR5_v6.500.tfs");
  cout << " --> interaction points: " << b1.getIP() << " -- " << b2.getIP() << endl;

  pylist(1);

  return 0;
}
