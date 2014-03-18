/****************************************************************************************************************************

    this program reads lhe file, select some events and then do the hadronization ---> time saving!
    compile with -----> c++ -O2 -ansi -pedantic -W -Wall `pythia8-config --cxxflags --libs --ldflags` -o unit_02.exe unit_02.cpp

****************************************************************************************************************************/

#include "Pythia8/Pythia.h"
#include <vector>

using namespace Pythia8; 
using namespace std;

//**************************************************************************************************

bool lhe_event_preselection(Event* event)
{
  vector<Particle> outQuark;
  
  for(int iPart = 0; iPart < event->size(); iPart++)
  {
    Particle particle = event->at(iPart);
    cout << particle.status() << endl;
    if(particle.status()>0 && abs(particle.id())>0 && abs(particle.id())<7)
    {
      outQuark.push_back(particle);
    }
  }
  if(outQuark.size() < 5)
  {
    return false;
  }
  else
  {
    return true;
  }      
}
//**************************************************************************************************

int main() {

  // Number of events to print.
  int nPrint = 10;             

  // Generators           
  Pythia pythia;
  Pythia pythiaPreselector;                            

  // Stick with default values, so do not bother with a separate file
  // for changes. However, do one change, to show readString in action.
  pythiaPreselector.readString("PartonLevel:ISR = off"); 
  pythiaPreselector.readString("PartonLevel:FSR = off"); 
  pythiaPreselector.readString("PartonLevel:MI = off"); 
  pythiaPreselector.readString("HadronLevel:Hadronize = off"); 
  pythia.readString("HadronLevel:Hadronize = on");

  // Initialize Les Houches Event File run. List initialization information.
  pythiaPreselector.init("../MC_data/signal_lvj/MGraviton_2500.lhe");
  pythia.init("../MC_data/signal_lvj/MGraviton_2500.lhe");       

  // Allow for possibility of a few faulty events.
  int nAbort = 10;
  int iAbort = 0;

  // Begin event loop; generate until none left in input file.     
  for (int iEvent = 0; ; ++iEvent) 
  {
    // Generate events, and check whether generation failed.
    if (!pythiaPreselector.next()) 
    {
      // If failure because reached end of file then exit event loop.
      if (pythiaPreselector.info.atEndOfFile()) break; 
    }
    cout << "preselction" << endl;
    pythiaPreselector.LHAeventList();               
   	pythiaPreselector.info.list();          
   	pythiaPreselector.process.list();          
   	pythiaPreselector.event.list();

    if(lhe_event_preselection(&pythiaPreselector.event))
    {
      if(!pythia.next())
      {
        // If failure because reached end of file then exit event loop.
        if (pythiaPreselector.info.atEndOfFile()) break;
        else
        {
          iAbort++;
        }
      }
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
  cout << "### failed:  " << iAbort << endl;                                           

  // Give statistics. Print histogram.
  pythia.statistics();
  // Done.                           
  return 0;
}
