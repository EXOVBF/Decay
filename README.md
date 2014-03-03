Decay
=====

 * hadronize events with Pythia8
 * decay and hadronize events with Pythia8
 

_be sure not to have CMSSW environment loaded for the following operations, it is screwing up librairies for runtime_

* Get [Pythia8](http://home.thep.lu.se/~torbjorn/Pythia.html) from the website

        NOT wget http://home.thep.lu.se/~torbjorn/pythia8/pythia8180.tgz
        NOT wget http://home.thep.lu.se/~torbjorn/pythia8/pythia8176.tgz
        wget http://home.thep.lu.se/~torbjorn/pythia8/pythia8153.tgz   -> as in CMS

* Untar it:

        tar -xvzf pythia8153.tgz     -> as in CMS


* Compile pythia (works out of the box on lxplus *slc6*, custom makefiles are needed for linking boost librairies [compulsory for `.lhe.gz` format]):

        cd pythia8153/
        ./configure --enable-gzip  --with-hepmcversion=2.06.08
        make -j 8

* Go in the examples directory and link our hadronization stuff, and compile it


# Hadronize only

 * main98

        cd /afs/cern.ch/user/a/amassiro/work/Generation/EXOVBF/Decay/
        cd pythia8153/examples
        ln -s ../../Makefile
        ln -s ../../main98.cc
        make main98
        source config.sh
        ./main98.exe   input.lhe     output.hepmc

        ./main98.exe   /tmp/amassiro/TT_CT10_13TeV-powheg-reduced.lhe     /tmp/amassiro/test-ttbar.lhe.hepmc
        ./main98.exe   /tmp/amassiro/TT_CT10_13TeV-powheg-reduced.lhe     /tmp/amassiro/test-ttbar.lhe.hepmc   13    0     1000


# Run with the precompiled version of Pythia8 present on lxplus

  * setup_slc6.sh: to be sourced to get the Pythia8 environment
  * then to compile look at the compilation command at the top of unit_01.cpp as an example
  * execute an example with: ```./unit_01.exe test_file.lhe test_output.hepmc```
