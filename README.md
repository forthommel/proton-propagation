# proton-propagation

### How to build?
The main dependencies are:
 * for `Hector`: `root` (available [here](https://root.cern.ch/downloading-root))
 * for `pythia6`: a fortran77-capable compiler (`gfortran` is used here)
 * for this tool: `HepMC`, `root`

Once everything is set up, run the following commands:
```sh
sh init.sh   # retrieve the dependencies (Hector, pythia6)
make
```
