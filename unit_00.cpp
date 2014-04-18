//****************************************************************************************************************************

// base program with Pythia8
// compile with -----> c++ -O2 -ansi -pedantic -W -Wall `pythia8-config --cxxflags --libs --ldflags` -o unit_00.exe unit_00.cpp

//****************************************************************************************************************************

#include "Pythia.h"
using namespace Pythia8; 

int main() {

  // Number of events to print.
  int nPrint = 10;             

  // Generator           
  Pythia pythia;                            

  // Stick with default values, so do not bother with a separate file
  // for changes. However, do one change, to show readString in action.
  pythia.readString("PartonLevel:ISR = off"); 
  pythia.readString("PartonLevel:FSR = off"); 
  pythia.readString("PartonLevel:MI = off"); 
  pythia.readString("HadronLevel:Hadronize = on"); 

  // Initialize Les Houches Event File run. List initialization information.
  pythia.init("../MC_data/signal_lvj/MGraviton_2500.lhe");      

  // Allow for possibility of a few faulty events.
  int nAbort = 10;
  int iAbort = 0;

  // Begin event loop; generate until none left in input file.     
  for (int iEvent = 0; ; ++iEvent) {

    // Generate events, and check whether generation failed.
    if (!pythia.next()) {

      // If failure because reached end of file then exit event loop.
      if (pythia.info.atEndOfFile()) break; 

      // First few failures write off as "acceptable" errors, then quit.
      if (++iAbort < nAbort) continue;
      break;
    }
  
    // List first few events: Les Houches, hard process and complete.
	
	if (iEvent < nPrint) 
	{     
		cout << endl << "########## EVENT:  " << iEvent << endl;
    	pythia.LHAeventList();               
      	pythia.info.list();          
      	pythia.process.list();          
      	pythia.event.list();           
    }                           
  // End of event loop.        
  }                                           

  // Give statistics. Print histogram.
  pythia.statistics();
  // Done.                           
  return 0;
}
