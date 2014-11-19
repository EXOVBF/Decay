#!/usr/bin/python

import sys
import os
import commands
from commands import getstatusoutput
import datetime
import argparse


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

def submitJob (jobID, nEvents, queue):
    jobname = 'jPyt_'+queue+'_'+jobID+'.sh'
    f = open (jobname, 'w')
    f.write ('#!/bin/sh' + '\n\n')
    f.write ('export SCRAM_ARCH=slc6_amd64_gcc472 \n')    
    f.write ('source /afs/cern.ch/user/s/spigazzi/work/EXOVBF/Decay/setup_slc6.sh \n')
    f.write ('/afs/cern.ch/user/s/spigazzi/work/EXOVBF/Decay/genMinBias_14TeV ' + nEvents + ' ' + jobID + ' \n\n')
    f.close ()
    getstatusoutput ('chmod 755 ' + jobname)
    getstatusoutput ('bsub -q ' + queue + ' ' + '-u simone.pigazzini@cern.ch ' + jobname)


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


if __name__ == '__main__':

    parser = argparse.ArgumentParser (description = 'boh')
    parser.add_argument('-j', '--jobsNum' , default = 1, help='number of sub job')
    parser.add_argument('-n', '--eventsNum' , default = 10000, help='number of events per job')
    parser.add_argument('-q', '--queue' , default = '1nh', help='batch queue (1nh)')
    
    args = parser.parse_args ()

    #getstatusoutput('cmsMkdir /store/user/govoni/Delphes/'+args.lheDir)

    print 'submitting', args.jobsNum, 'jobs to queue', args.queue
    for i in range (0, int(args.jobsNum)):
        submitJob (str (i), args.eventsNum, args.queue) 



