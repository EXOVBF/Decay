/****************************************************************************************************************************

        -this program create a sample on minimum bias events for SLHC-14TeV
        -reference for the Tune:pp option used can be found here: https://cds.cern.ch/record/1697700/files/GEN-14-001-pas.pdf 

        -source setup_slc6, then:
        -compile with ---> gcc -O2 -lm -I /afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc6-gcc46-opt/include/ -L /afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc6-gcc46-opt/lib -I /afs/cern.ch/cms/slc6_amd64_gcc472/external/pythia8/185-cms/include/ -L /afs/cern.ch/cms/slc6_amd64_gcc472/external/pythia8/185-cms/lib/ -I /afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.8.9/x86_64-slc6-gcc46-opt/include/ -L /afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.8.9/x86_64-slc6-gcc46-opt/lib/ -lHepMC -lpythia8tohepmc -lpythia8 -lLHAPDF -o genMinBias_14TeV genMinBias_14TeV.cpp
****************************************************************************************************************************/

#include "Pythia8/Pythia.h"
#include "Pythia8/Pythia8ToHepMC.h"
#include "Pythia8/LHAPDFInterface.h"

#include "HepMC/GenEvent.h"
#include "HepMC/IO_GenEvent.h"
#include "HepMC/IO_AsciiParticles.h"

using namespace Pythia8;  
using namespace std;

//********************************************************************************************

int main(int argc, char** argv) 
{
    int nEvents = atoi(argv[1]);
    
    //-----Pythia setup-----
    Pythia pythia;     
    //---general setup
    pythia.readString("Random:seed = 0");
    pythia.readString("Random:setSeed = on"); 
    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eA = 7000.");
    pythia.readString("Beams:eB = 7000.");
    pythia.readString("Beams:eCM = 14000.");
    pythia.readString("HadronLevel:Hadronize = on");
    //---CMS tuned min bias
    pythia.readString("SoftQCD:nonDiffractive = on");
    pythia.readString("SoftQCD:singleDiffractive = on");
    pythia.readString("SoftQCD:doubleDiffractive = on");
    pythia.readString("Tune:ee = 3");
    pythia.readString("Tune:pp = 15");
    //---Pythia initialization
    pythia.init();

    string outFile = "/afs/cern.ch/user/s/spigazzi/work/VBS4TP/MC_data/minBias/minBias_14TeV_";
    outFile += argv[1];
    outFile += "_";
    outFile += argv[2];
    outFile += ".hepmc";
    HepMC::IO_GenEvent hepmc_file_out(outFile, std::ios::out);
    HepMC::Pythia8ToHepMC ToHepMC;
	
    for (int iEvent = 0; iEvent < nEvents; ++iEvent)
    {
	if(iEvent%(nEvents/10) == 0) 
	{
	    cout << "Events:  " << iEvent << endl;
	}
	if(!pythia.next())
	{
	    cout << "CRASH! ---> skip" << endl;
	    continue;
	}
	// construct new HepMC event setting units.	
	HepMC::GenEvent* hepmcevt = new HepMC::GenEvent(HepMC::Units::GEV, HepMC::Units::MM);
	// fill the event including PDF infos
	ToHepMC.fill_next_event( pythia, hepmcevt );
	hepmc_file_out << hepmcevt;
	    delete hepmcevt;
    }
    // Done.                           
    return 0;
}
