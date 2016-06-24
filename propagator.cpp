#include <iostream>
#include "H_BeamLine.h"
#include "H_BeamParticle.h"
#include "Pythia6Wrapper.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"

using namespace std;

int main(int argc, char* argv[])
{
  const unsigned int nevents = 10000;
  //const double pps_z_position = 204.;
  const double pps_z_position = 215.5;
  const bool debug = false;

  // initialize Pythia
  call_pygive("MSTU(13)=0"); // silence printouts from PYGIVE, unless display requested
  call_pygive("MSTU(12)=12345"); // silence the Pythia banner
  call_pygive("MRPY(1)=1987"); // change the seed
  call_pygive("MRPY(1)=0"); // reset the generator
  call_pygive("MSTP(81)=1"); // multiple interactions
  //call_pygive("MSTP(51)=10042"); // structure function chosen (external PDF CTEQ6L1)
  call_pygive("MSTP(52)=1");
  //call_pygive("MSTP(52)=2"); // work with LHAPDF
  call_pygive("MSTP(81)=21"); // MPI 21 is Pythia new set of MPI models
  call_pygive("MSTP(82)=5"); // MPI model
  call_pyinit("CMS", "p", "p", 14000.);
  //call_pystat();

  // initialize the beamlines
  H_BeamLine b1, b2;
  b1.fill("external/hector/data/LHCB1IR5_v6.500.tfs", +1, "IP5");
  b2.fill("external/hector/data/LHCB2IR5_v6.500.tfs", -1, "IP5");
  //FIXME from CMSSW
  b1.offsetElements(120, -0.097); b1.calcMatrix();
  b2.offsetElements(120, +0.097); b2.calcMatrix();
  cout << " --> interaction points: " << b1.getIP() << " -- " << b2.getIP() << endl
       << " --> number of elements: " << b1.getNumberOfElements() << " -- " << b2.getNumberOfElements() << endl;

  TH1D proton_multiplicities("proton_multiplicities", "", 10, 0, 10.);
  TH2D hitmap_xy("hitmap_xy", "", 10, -5., 5., 10, -5., 5.); // in mm

  int propagated_protons;
  for (unsigned int i=0; i<nevents; i++) {
    if (i%100==0)
      cout << ">>>>>>>>>> STARTING EVENT " << i << " <<<<<<<<<<" << endl;
    if (debug)
      cout << ">>> EVENT " << i << ": First step: Creating the event with Pythia..." << endl;

    call_pyevnt();
    //call_pylist(1);

    HepMC::GenEvent ev = getEvent();
    //ev.print();

    if (debug)
      cout << ">>> EVENT " << i << ": Second step: Propagating the output event through the beamline..." << endl;

    propagated_protons = 0;
    for(HepMC::GenEvent::particle_const_iterator ip=ev.particles_begin(); ip!=ev.particles_end(); ip++) {
      HepMC::GenParticle* p = (*ip);
      if (p->status()!=1) continue; // we only keep stable, final state particles
      unsigned int id = p->barcode();
      if (p->pdg_id()!=2212) continue; // we only propagate protons
      H_BeamParticle* hp = new H_BeamParticle(p->generated_mass(), call_pychge(p->pdg_id()));
      hp->setPosition(-p->production_vertex()->position().x()*1e4, //FIXME units?!
                       p->production_vertex()->position().y()*1e4,
                       0.0, 0.0, // urad
                       p->production_vertex()->position().z()*1e-2);
      hp->set4Momentum(-p->momentum().px(),
                        p->momentum().py(),
                       -p->momentum().pz(),
                        p->momentum().e());
      int direction = (p->momentum().z()>0) ? 1 : -1;
      H_BeamLine* prop_beamline = (direction>0) ? &b1 : &b2; //FIXME need to check the direction
      hp->computePath(*prop_beamline);
      bool stopped = hp->stopped(prop_beamline);
      if (debug) {
        cout << "Particle " << id << " (" << p->pdg_id()
             << ", E=" << p->momentum().e() << " GeV, eta=" << p->momentum().eta() << ")";
        cout << " is stopped? " << (stopped ? "yes" : "no");
      }
      if (stopped) {
        if (debug)
          cout << " -> stopping element: " << hp->getStoppingElement()->getTypeString()
               << " with name " << hp->getStoppingElement()->getName()
               << " at s=" << hp->getStoppingElement()->getS() << endl;
        continue;
      }
      if (debug) cout << endl;

      double z_position = direction*pps_z_position;
      hp->propagate(z_position);

      double x = -hp->getX(), y = hp->getY(); // in um
      //cout << x << " -- " << y << endl;
      hitmap_xy.Fill(x*1.e-3, y*1.e-3);
      propagated_protons++;

      delete hp;
    }
    proton_multiplicities.Fill(propagated_protons, 1./nevents);
  }
  TCanvas c_hitmap("c_hitmap", "", 500, 500);
  hitmap_xy.Draw("colz");
  hitmap_xy.GetXaxis()->SetTitle("x (mm)");
  hitmap_xy.GetYaxis()->SetTitle("y (mm)");
  c_hitmap.SaveAs("hitmap.png");

  TCanvas c_mult("c_hitmap", "", 500, 500);
  proton_multiplicities.Draw();
  proton_multiplicities.GetXaxis()->SetTitle("Number of protons propagated");
  proton_multiplicities.GetYaxis()->SetTitle("Events fraction");
  c_mult.SaveAs("multiplicities.png");

  return 0;
}
