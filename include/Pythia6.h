#ifndef Pythia6_h
#define Pythia6_h

extern "C" 
{
  void pylist_(int& mlist);
}

inline void pylist(int mlist) { pylist_(mlist); }

#endif
