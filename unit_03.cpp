/****************************************************************************************************************************

        this program reads lhe file, select some events and then do the hadronization ---> time saving!
        compile with -----> c++ -O2 -ansi -pedantic -W -Wall `pythia8-config --cxxflags --libs --ldflags` -o unit_03.exe unit_03.cpp

****************************************************************************************************************************/

#include "Pythia8/Pythia.h"
#include <vector>
#include <string>
#include <fstream>
#include <string.h>

using namespace Pythia8; 
using namespace std;

//**************************************************************************************************

bool lhe_event_preselection(vector< vector<float> >* LHE_event)
{
    vector< vector<float> > outQuark;
    
    for(int iPart = 0; iPart < LHE_event->size(); iPart++)
    {
        vector<float> particle = LHE_event->at(iPart);
        if(particle.at(1) == 1 && abs(particle.at(0)) > 0 && abs(particle.at(0)) < 7)
        {
            cout << particle.at(0) << endl;
            outQuark.push_back(particle);
        }
    }
    if(outQuark.size() < 1)
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

    // Generators                     
    Pythia pythia;

    // Stick with default values, so do not bother with a separate file
    // for changes. However, do one change, to show readString in action.
    pythia.readString("HadronLevel:Hadronize = on");

    // Initialize Les Houches Event File run. List initialization information.
    //pythia.init("../MC_data/signal_lvj/MGraviton_2500.lhe");             
    pythia.init("main72.lhe");             


    // Allow for possibility of a few faulty events.
    int skippedEvents = 0;
    int iAbort = 0;
    
//    ifstream inputLHE ("../MC_data/signal_lvj/MGraviton_2500.lhe", ios::in);
    ifstream inputLHE ("main72.lhe", ios::in);
    char buffer[256];
    vector<float> LHE_particle;
    vector< vector<float> > LHE_event; 

    // Begin event loop; generate until none left in input file.         
    for (int iEvent = 0; ; ++iEvent) 
    {      
        LHE_event.erase(LHE_event.begin(), LHE_event.end());  
        inputLHE.getline(buffer,256);
        //--- end of file check
        if(strcmp(buffer, "</LesHouchesEvents>") == 0)        
        {
            cout << "### end of LHE file reached! ### " << endl;
            break;
        } 
        //--- reads and store the event
        else if(strcmp(buffer, "<event>") == 0)
        {   
            int nPart;
            float info_tmp;
            inputLHE >> nPart;
            for(int i=0; i<5; i++)
            {
                inputLHE >> info_tmp;
            }
            for(int i=0; i<nPart; i++)
            {
                for(int j=0; j<13; j++)
                {
                    inputLHE >> info_tmp;
                    cout << " " << info_tmp << " ";
                    LHE_particle.push_back(info_tmp);
                }
                cout << endl;
                LHE_event.push_back(LHE_particle);
                LHE_particle.erase(LHE_particle.begin(), LHE_particle.end());
            }
            if(lhe_event_preselection(&LHE_event))
            {
                if(!pythia.next())
                {
                    // If failure because reached end of file then exit event loop.
                    if (pythia.info.atEndOfFile()) break;
                    else
                    {
                        iAbort++;
                    }
                }     
                cout << endl << "########## EVENT:    " << iEvent << endl;
                pythia.LHAeventList();                             
             	pythia.info.list();                    
             	pythia.process.list();                    
            }
            else
            {
                if(pythia.LHAeventSkip(1))
                {
                    skippedEvents++;
                }
            }
            cout << "### skipped:    " << skippedEvents << endl;         

            // End of event loop. do event by event.                
            int help;
            cin >> help;
        }
    }                                                    
    cout << "### failed:    " << iAbort << endl;                                                                                     

    // Give statistics. Print histogram.
    pythia.statistics();
    // Done.                                                     
    return 0;
}
