#!/bin/sh

PYTHIA_VERSION=6428

PYTHIA_DIR=external/
PYTHIA_FILE=pythia${PYTHIA_VERSION}.f
if [ ! -f ${PYTHIA_DIR}${PYTHIA_FILE} ]; then
  curl "http://home.thep.lu.se/~torbjorn/pythia6/${PYTHIA_FILE}" > ${PYTHIA_DIR}${PYTHIA_FILE}
fi

# submodules initialisation
git submodule update --init --recursive # fetch the latest version of xpedaq
git submodule foreach git pull origin master
