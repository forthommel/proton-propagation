FC = gfortran
CC = g++
CFLAGS = -g
LDFLAGS = -g -lgfortran
ROOT_CFLAGS = `root-config --cflags`
ROOT_LDFLAGS = `root-config --glibs`

HECTOR_DIR = external/hector
HECTOR_LIB_DIR = $(HECTOR_DIR)/lib/
HECTOR_INC_DIR = $(HECTOR_DIR)/include/

OBJ_DIR = obj/
INC_DIR = include/

all: propagator

clean:
	rm -rf $(OBJ_DIR) propagator

hector:
	@cd $(HECTOR_DIR) && $(MAKE) -s

propagator: $(OBJ_DIR)propagator.o $(OBJ_DIR)pythia6428.fo | hector
	$(CC) $(LDFLAGS) $^ -o $@ -L$(HECTOR_LIB_DIR) -lHector -Wl,-R$(HECTOR_LIB_DIR) $(ROOT_LDFLAGS)

$(OBJ_DIR)%.o: %.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $^ -o $@ -I$(INC_DIR) -I$(HECTOR_INC_DIR) $(ROOT_CFLAGS)

$(OBJ_DIR)%.fo: external/%.f | $(OBJ_DIR)
	$(FC) $(FFLAGS) -c $^ -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)