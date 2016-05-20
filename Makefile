FC = gfortran
CC = g++
CFLAGS = -g
LDFLAGS = -g -lgfortran

HECTOR_DIR = external/hector
HECTOR_LIB_DIR = $(HECTOR_DIR)/lib/

all: propagator

hector:
	cd $(HECTOR_DIR) && $(MAKE)

propagator: propagator.o | hector
	$(CC) $(LDFLAGS) $^ -o $@ -L$(HECTOR_LIB_DIR) -lHector -Wl,-R$(HECTOR_LIB_DIR)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^ -o $@
