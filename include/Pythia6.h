#ifndef Pythia6_h
#define Pythia6_h

extern "C" 
{
  // subroutines
  void pylist_(int& mlist);
  void pygive_(const char* chin, int sz);
  void pyexec_();
  void pyinit_(const char* frame, const char* beam, const char* target, double& win, int sz_frame, int sz_beam, int sz_target);

  // common blocks
  extern struct {
    int mstp[200];
    double parp[200];
    int msti[200];
    double pari[200];
  } pypars_;
  /*extern struct {
    char* proc[500];
  } pyint6_;*/

}

inline void pylist(int mlist) { pylist_(mlist); }
inline void pygive(std::string chin) { pygive_(chin.c_str(), (int)chin.size()); }
inline void pyinit(std::string frame, std::string beam, std::string target, double win) {
  pyinit_(frame.c_str(), beam.c_str(), target.c_str(), win,
          (int)frame.size(), (int)beam.size(), (int)target.size());
}
inline void pyexec() { pyexec_(); }

#endif
