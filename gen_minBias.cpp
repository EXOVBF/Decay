/****************************************************************************************************************************

        this program create a sample on minimum bias events for LHC-8TeV
        compile with ----->c++ -O2 -lm -I /afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc6-gcc46-opt/include/ -L /afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc6-gcc46-opt/lib -I /afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.8.9/x86_64-slc6-gcc46-opt/include/ -L /afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.8.9/x86_64-slc6-gcc46-opt/lib/ -lHepMC -lpythia8tohepmc -lLHAPDF `pythia8-config --cxxflags --libs --ldflags`  -o gen_minBias.exe gen_minBias.cpp

****************************************************************************************************************************/

#ifdef pythia176
#include "Pythia.h"
#include "HepMCInterface.h"
#define HepMCConverter HepMC::I_Pythia
#else
#include "Pythia8/Pythia.h"
#include "Pythia8/Pythia8ToHepMC.h"
#define HepMCConverter HepMC::Pythia8ToHepMC
#endif

#include "HepMC/GenEvent.h"
#include "HepMC/IO_GenEvent.h"
#include "HepMC/IO_AsciiParticles.h"

using namespace Pythia8;  
using namespace std;

//********************************************************************************************

int main(int argc, char** argv) 
{
	Pythia pythia;

// Configure                                                                                                                                           
	pythia.readString("SoftQCD:inelastic = on");
	pythia.readString("HadronLevel:Hadronize = on");
//	pythia.readString("SoftQCD:nonDiffractive = on");

	pythia.readString("Random:seed = 0");
	pythia.readString("Beams:idA = 2212");
	pythia.readString("Beams:idB = 2212");
	pythia.readString("Beams:eA = 4000.");
	pythia.readString("Beams:eB = 4000.");
	pythia.readString("PDF:useLHAPDF = on");
	pythia.readString("PDF:LHAPDFset = cteq61.LHpdf");

// Initialize                                                                                                                                          

	pythia.init();

    string outFile = "/afs/cern.ch/user/s/spigazzi/work/EXOVBF/MC_Data/minBias/minBias_8TeV_500K_";
	outFile += argv[1];
	outFile += ".hepmc";
	HepMC::IO_GenEvent hepmc_file_out(outFile, std::ios::out);
	HepMCConverter ToHepMC;
	
	for (int iEvent = 0; iEvent < 5000; ++iEvent)
	{
		if(iEvent%1000 == 0) 
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
