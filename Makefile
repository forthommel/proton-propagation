FC = gfortran
CC = g++
CFLAGS = -g
LDFLAGS = -g -lgfortran
ROOT_CFLAGS = `root-config --cflags`
ROOT_LDFLAGS = `root-config --glibs`
HEPMC_PATH = /usr
#HEPMC_PATH = /cvmfs/cms.cern.ch/slc6_amd64_gcc493/external/hepmc/2.06.07 # uncomment if on lxplus
HEPMC_CFLAGS = -I$(HEPMC_PATH)/include/
HEPMC_LDFLAGS = -L$(HEPMC_PATH)/lib/ -lHepMC

HECTOR_DIR = external/hector
HECTOR_LIB_DIR = $(HECTOR_DIR)/lib/
HECTOR_INC_DIR = $(HECTOR_DIR)/include/

OBJ_DIR = obj/
INC_DIR = include/
SRC_DIR = src/

PYTHIA_OBJ = $(OBJ_DIR)pythia6428.fo $(OBJ_DIR)Pythia6Wrapper.o

all: propagator

clean:
	rm -rf $(OBJ_DIR)/*.o propagator

hector:
	@cd $(HECTOR_DIR) && $(MAKE) -s

propagator: $(OBJ_DIR)propagator.o $(PYTHIA_OBJ) | hector
	$(CC) $(LDFLAGS) $^ -o $@ -L$(HECTOR_LIB_DIR) -lHector -Wl,-R$(HECTOR_LIB_DIR) $(ROOT_LDFLAGS) $(HEPMC_LDFLAGS)

$(OBJ_DIR)%.o: %.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $^ -o $@ -I$(INC_DIR) -I$(HECTOR_INC_DIR) $(ROOT_CFLAGS) $(HEPMC_CFLAGS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $^ -o $@ -I$(INC_DIR) -I$(HECTOR_INC_DIR) $(ROOT_CFLAGS) $(HEPMC_CFLAGS)

$(OBJ_DIR)%.fo: external/%.f | $(OBJ_DIR)
	$(FC) $(FFLAGS) -c $^ -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
