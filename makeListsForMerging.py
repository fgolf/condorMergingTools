#! /usr/bin/env python

import string, random
import commands, re, os
import sys 
import xml.dom.minidom
from xml.dom.minidom import Node

                      
################### Gets List of good XML files (files corresponding to jobs that have ###################
################### completed successfully                                             ###################

def getGoodXMLFiles(crabpath):
    global goodCrabXMLFiles

    cmd = 'ls ' + crabpath + '/res/*.xml'
    temp = commands.getoutput(cmd).split('\n')
    
    for j in temp:
        print 'Parsing ' + j
        duplicateFile = False
        for k in goodCrabXMLFiles:
            if k.split('/')[len(k.split('/'))-1] == j.split('/')[len(j.split('/'))-1]:
                duplicateFile = True
                break
        if duplicateFile == False:
            try:
                doc = xml.dom.minidom.parse(j) #read xml file to see if the job failed
            except:
                print 'FrameworkJobReport:',j,'could not be parsed and is skipped'
                continue
            jobFailed = False
            for node in doc.getElementsByTagName("FrameworkJobReport"):
                try:
                    key =  node.attributes.keys()[0].encode('ascii')
                    if node.attributes[key].value == "Success":
                        jobFailed = False
                    if node.attributes[key].value == "Failed":
                        print "Job " + j.split('/')[len(j.split('/'))-1].split('.')[0].split('_')[2] + " Failed!!!"
                        jobFailed = True                
                except:	 
                     print "skipping file ", j, " due to parsing failure"	 
                     jobFailed = True	 
                     continue	 
 	 
            if jobFailed == False:
                goodCrabXMLFiles.append(j)


###############################################################################################################
################### Get Number of Events Run ###################

def getNumEventsRun(crabpath):
    global totalNumEventsRun
    global goodCrabXMLFiles
    #get the job number from the crab file
    for i in goodCrabXMLFiles:
        print "New: " + i
        jobNum = i.split("_")[len(i.split("_"))-1]
        jobNum = jobNum.split(".")[0]
        cmd = "ls " +  outpath + "/preprocessing/ntuple_" + jobNum + "_1.root"
        print cmd
        if commands.getstatusoutput(cmd)[0] == 256:
            continue
        #parse the crab file:
        print "Getting Number of Events run for Job: " + i 
        doc = xml.dom.minidom.parse(i)
        for node in doc.getElementsByTagName("EventsRead"):
            s = node.firstChild.data
            #s is in unicode
            #don't need to encode in ascii, but runs faster
            s.encode('ascii')
            totalNumEventsRun = totalNumEventsRun + int(s)


###############################################################################################################
################### Get List Of Root Files which are not corrupted and have the Events tree ###################
                
def getGoodRootFiles(datapath,outpath):
    global goodCrabXMLFiles
    global goodRootFiles
    global dcachePrefix
    global crabpath
    tempXMLFileList = []
    fileNumber = 1
    totalFileSize = 0
    singleFileSize = 0
    f1 = open("%s/mergeFiles/listOfAllGoodFiles.txt" %(crabpath), "w")
    fileiter = open("%s/mergeFiles/mergeLists/merged_list_%s.txt" %(crabpath,str(fileNumber)), "w")

    #this is where we parse the crab fjrs to find the file size, path to the file, and if it is good or not.
    for ntuple in goodCrabXMLFiles:
        doc = xml.dom.minidom.parse(ntuple)
        path = '/cms'+doc.getElementsByTagName("LFN")[0].firstChild.data.strip().rstrip()
        fullPath = '/hadoop/cms'+doc.getElementsByTagName("LFN")[0].firstChild.data.strip().rstrip()
        fname = path.split('/')[len(path.split('/'))-1]
                
        #checks the files with sweeproot before copying. If it finds a bad file, It skips the file and does not add the events to the total # of events
        print 'Checking File /hadoop' + dcachePrefix + path + ' for integrity'
        cmd = "./sweepRoot -o Events /hadoop/" + dcachePrefix + path #outpath + '/temp/' + fname + ' 2> /dev/null'
        output = commands.getoutput(cmd).split('\n')

        for sweepRootCode in output:

            if sweepRootCode.find('SUMMARY') != -1:
                print sweepRootCode

            if sweepRootCode.find('SUMMARY: 1 bad, 0 good') != -1:
                print 'File: /hadoop' + dcachePrefix + path + ' does not seem to be good!!!!\n'
                commands.getoutput('rm ' + outpath+'/temp/' + fname)

            elif sweepRootCode.find('SUMMARY: 0 bad, 1 good') != -1:
                print 'File: /hadoop' + dcachePrefix + path + ' looks just fine!\n'
                cmd = "du /hadoop%s%s | awk \'{print $1}\'" %(dcachePrefix,path)
                singleFileSize = os.path.getsize(fullPath)# commands.getoutput(cmd)
                # totalFileSize+=os.path.getsize(path)
                totalFileSize += int(singleFileSize)
                print"single file size: %s" %(singleFileSize)
                print "total file size: %s" %(totalFileSize)
                
                # checks to see if the files are > ~4.85GB, and if they are, creates a new list
                if totalFileSize < 5200000000:
                # #checks to see if the files are > ~7.2GB for the cms2->slim, and if they are, creates a new list
                # if totalFileSize < 7730900000:
                    f1.write( '/hadoop%s%s\n' %(dcachePrefix, path) )
                    fileiter.write( '/hadoop%s%s\n' %(dcachePrefix, path) )
                else:
                    fileiter.close()
                    fileNumber += 1
                    # totalFileSize = os.path.getsize(path)
                    totalFileSize = int(singleFileSize)
                    fileiter = open("%s/mergeFiles/mergeLists/merged_list_%s.txt" %(crabpath,str(fileNumber)), "w");
                    f1.write( '/hadoop%s%s\n' %(dcachePrefix, path) )
                    fileiter.write( '/hadoop%s%s\n' %(dcachePrefix, path) )

                tempXMLFileList.append(ntuple)

    fileiter.close()
    goodCrabXMLFiles = tempXMLFileList



###########################################################################
## Compare total space used by ntuples to available space on post processing disk. don't post process if not enough space
############################################################################
def checkForSpace(outpath):
    global goodCrabXMLFiles
    totalFileSize=0
    for i in goodCrabXMLFiles:
        #no more resubmission dir. Use <LFN> tag, never assume ntuple file name.
        #many lfn's, but first should always be right, and start with '/store/user'
        doc = xml.dom.minidom.parse(i)
        path = '/hadoop/cms'+doc.getElementsByTagName("LFN")[0].firstChild.data.strip().rstrip()
        totalFileSize+=os.path.getsize(path)

    outdisk = os.statvfs(outpath)
    freeSpace = outdisk.f_bsize * outdisk.f_bavail
    print '\nPostprocessing ' + str(totalFileSize) + ' bytes on ' + str(freeSpace) + ' bytes of available disk space.'
    threshold = 5  #change this threshold if this is too conservative.
    # if( (freeSpace/totalFileSize) < threshold ):
    if( True ):
        print 'The threshold of %s times as much available disk space as ntuple size has not been met. Will not post process.' % str(threshold)
        sys.exit();
    print '\n'

if( len(sys.argv)!=15 ):
    print 'Usage: postProcessing.py '
    print '                              -c [name of crab directory]'
    print '                              -d [directory where root files are]'
    print '                              -o [output directory for files on hadoop]'
    print '                              -s [name of sample]'
    print '                              -k kFactor'
    print '                              -e filter Efficiency'
    print '                              -x x-Section'

    sys.exit()

##global variables here
crabpath = ''
datapath = ''
outpath = ''
dcachePrefix = ''     

    
localdirectory = commands.getstatusoutput('pwd')[1]
for i in range (0, len(sys.argv)):
    if(sys.argv[i] == "-c"):
        crabpath   = sys.argv[i+1] + "/"
    if(sys.argv[i] == "-d"):
        datapath    = sys.argv[i+1] + "/"
    if(sys.argv[i] == "-o"):
        outpath     = sys.argv[i+1] + "/"
    if(sys.argv[i] == "-s"):
        samplename  = sys.argv[i+1]
    if(sys.argv[i] == "-k"):
        kFactor     = sys.argv[i+1] 
    if(sys.argv[i] == "-e"):
        filtEff     = sys.argv[i+1] 
    if(sys.argv[i] == "-x"):
        xSection     = sys.argv[i+1] 
                         

if( commands.getstatusoutput('ls ' + crabpath)[0] == 256):
    print 'The crab path does not exist. Please supply a valid path'
    sys.exit()

if( commands.getstatusoutput('ls ' + datapath)[0] == 256):
    print 'The directory containing the root files does not exist. Please supply a valid path'
    sys.exit()

if( commands.getstatusoutput('ls ' + outpath)[0] == 256):
    print '*******************************************'
    print 'The directory where you want your final root files to end up does not exist'
    sys.exit()

# try:
#     kFactor
# except NameError:
#     print  "kFactor is not set. exiting."
#     exit(1)

##get list of fjrs, then look through those for output files
##sum the space used by root files, and compare that to the free space on the output disk
goodCrabXMLFiles = []
getGoodXMLFiles(crabpath)
# checkForSpace(outpath)


if datapath.find("hadooop") != -1:
        dcachePrefix = ''
    

if datapath.find("pnfs") != -1 or datapath.find("hadoop") != -1:
    print "Creating folders for logs and merged file lists in %s/mergeFiles" %(outpath)
    cmd = "mkdir -p " + crabpath + "/mergeFiles/mergeLists"
    # print commands.getstatusoutput("ls %s/mergeFiles/mergeLists" %(crabpath))[1]
    if commands.getstatusoutput(cmd)[0] == 256 and commands.getstatusoutput("ls %s/mergeFiles/mergeLists" %(crabpath))[1]!="":
        print "The directory %s/mergeFiles/mergeLists already exists and is not empty!. Exiting!" %(crabpath)
        sys.exit()

        

##now make the sweeproot macro
cmd = 'cp libC/Makefile .'
commands.getstatusoutput(cmd)
cmd = 'cp libC/sweepRoot.C .'
commands.getstatusoutput(cmd)
cmd = 'make '
commands.getstatusoutput(cmd)


goodRootFiles = []
totalNumEventsRun = 0
rootFilesToMerge = []


getGoodRootFiles(datapath,outpath)        
getNumEventsRun(crabpath)

# ##copy crab directory to output directory
# commands.getstatusoutput('mkdir ' + outpath + '/crab_logs')
# commands.getstatusoutput('cp -r ' + crabpath + '* ' + outpath + 'crab_logs')
# ##copy hadoop copy script to output directory
# commands.getstatusoutput('cp %s/src/CMS2/NtupleMacros/NtupleTools/copyToHadoop.sh %s' %(CMSSWpath,outpath))

print '+++++++++++++++++++++++++++++'
print 'Total number of events that were run over to produce ntuples: ' + str(totalNumEventsRun)


print "creating file metaData.txt for postprocessing..."
f = open("%smergeFiles/metaData.txt" %(crabpath),"w")
f.write("n: %s\nk: %s\nf: %s\nx: %s\n" %(str(totalNumEventsRun),kFactor,filtEff,xSection))
cmd = "ls %s/mergeFiles/mergeLists/ | awk '{print $1}'" %(crabpath) 
output = commands.getoutput(cmd).split('\n')
for listName in output:
    f.write("file: %s\n" %(listName))
    
f.close()

print "Writing cfg file to cfg/%s.sh" %(samplename)
f_cfg = open("cfg/%s_cfg.sh" %(samplename),"w")
f_cfg.write("export inputListDirectory=%s/mergeFiles/mergeLists/\n" %(crabpath))
f_cfg.write("export mData=%s/mergeFiles/metaData.txt\n" %(crabpath))
f_cfg.write("export outputDir=%s\n" %(outpath))
f_cfg.write("export dataSet=%s\n" %(samplename))
f_cfg.write("export workingDirectory=%s\n" %(localdirectory))
f_cfg.write("export executableScript=%s/libsh/mergeScript.sh\n" %(localdirectory))
f_cfg.close()

print "All file lists are created and stored in %smergeFiles/mergeLists/\n preparing to create jobs to postprocess ntuples..." %(crabpath)
