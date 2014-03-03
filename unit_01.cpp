//****************************************************************************************************************************

// on slc6: compile with -----> c++ -O2 -ansi -pedantic -W -Wall -I /afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc6-gcc46-opt/include/ -L /afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc6-gcc46-opt/lib -lHepMC -lpythia8tohepmc `pythia8-config --cxxflags --libs --ldflags` -o unit_01.exe unit_01.cpp

//****************************************************************************************************************************

// just for back-compatibility

#ifdef PYTHIA8176
#include "Pythia.h"
#include "HepMCInterface.h"
#define HepMCConverter HepMC::I_Pythia8
#else
#include "Pythia8/Pythia.h"
#include "Pythia8/Pythia8ToHepMC.h"
#define HepMCConverter HepMC::Pythia8ToHepMC
#endif

#include "HepMC/GenEvent.h"
#include "HepMC/IO_GenEvent.h"
#include "HepMC/IO_AsciiParticles.h"

using namespace Pythia8; 

int main(int argc, char **argv) 
{
    if (argc < 3) 
    {
        std::cerr << " Not enough input information!  no input/output file!" << std::endl;
        exit (0);
    }
    int startEntry = 0;
    if (argc >= 4) startEntry = atoi(argv[3]);
    int endEntry = -1;
    if (argc >= 5) endEntry   = atoi(argv[4]);
 
    std::string namefile_in;
    namefile_in = argv[1];
    std::string namefile_out;
    namefile_out = argv[2];
    std::string namefile_easy_out = "human_";
    namefile_easy_out += (namefile_out);
    

	// Initialize Les Houches Event File run. List initialization information.
	Pythia pythia;                           
    std::string sfile = "Beams:LHEF ="+namefile_in;    
    pythia.readString("Beams:frameType = 4");
   	pythia.readString("HadronLevel:Hadronize = on");
	pythia.readString(sfile.c_str());      
    pythia.init();

    // Interface for conversion from Pythia8::Event to HepMC event. 
    //HepMC::I_Pythia8 ToHepMC;
    HepMCConverter ToHepMC;
    // Specify file where HepMC events will be stored.
    HepMC::IO_GenEvent hepmc_file_out(namefile_out.c_str(), std::ios::out);
    // Alternative output format -----> just for test 
//    HepMC::IO_AsciiParticles human_file_out(namefile_easy_out.c_str(), std::ios::out);

	// Allow for possibility of a few faulty events.
	int iAbort = 0;
	int totEvent = 0;

	// Begin event loop; generate until none left in input file.     
	for (int iEvent = 0; ; ++iEvent)
	{
                //---- subrange of events ----
        if (endEntry!=-1 && iEvent>=endEntry) break;
        if (iEvent >= startEntry)
        {
        	if (!(iEvent%500)) std::cout<<"####### EVENT = " << iEvent << std::endl;	
        			// Generate events, and check whether generation failed.
            if (!pythia.next()) 
		    {
		    // If failure because reached end of file then exit event loop.
	    	    if (pythia.info.atEndOfFile()) break; 
		    // First few failures write off as "acceptable" errors, then quit.
			    //if (++iAbort < nAbort) continue;
		        //break;
		        iAbort++;
            }

                // construct new HepMC event setting units.
                HepMC::GenEvent* hepmcevt = new HepMC::GenEvent(HepMC::Units::GEV, HepMC::Units::MM);
                // fill the event including PDF infos
                ToHepMC.fill_next_event( pythia, hepmcevt );
  
                // Write the HepMC event to file. Done with it.
                hepmc_file_out << hepmcevt;
                //human_file_out << hepmcevt;
    
                delete hepmcevt;
                totEvent++;
        }
 	    // End of event loop.        
  	}                                           
    cout << "############### total simulated event:   " << totEvent << "   Fail:   " << iAbort << "   ###############" << endl;
	// Done.                           
	return 0;
}
