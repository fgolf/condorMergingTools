#! /bin/bash

# This is the config needed to run the merging. Maybe in the future the makeListsForMerging.py can write this script for you. For now, you have to write it on your own. The usage will be indicated with an example before each variable required for the job. All the directories you list must be in the form /home/users/username/... 

# This is the location of your samples. They will be output into the crab directory of the sample you are trying to merge. Example:
# export inputListDirectory=/home/users/cwelke/MCNtupling/CMSSW/CMSSW_5_3_2_patch4_V05-03-13/crab/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12_DR53X-PU_S10_START53_V7A-v1/mergeFiles/mergeLists/ 
export inputListDirectory=

# This is the location of the metaData.txt file that was created which contains event information.
# export mData=/home/users/cwelke/MCNtupling/CMSSW/CMSSW_5_3_2_patch4_V05-03-13/crab/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12_DR53X-PU_S10_START53_V7A-v1/mergeFiles/metaData.txt 
export mData=

# this is the output directory on hadoop where you want your job to stage out.
# export outputDir=/hadoop/cms/store/group/snt/papers2012/Summer12_53X_MC/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12_DR53X-PU_S10_START53_V7A-v1/V05-03-18_slim/
export outputDir=

# This is the name of the dataset you are trying to merge. It is best to just use the same one as the name of the crab folder.
# export dataSet=DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12_DR53X-PU_S10_START53_V7A-v1
export dataSet=

# This is the directory where you stored all these scripts
# export workingDirectory=/home/users/cwelke/slimcfg/condorMergingTools
export workingDirectory=

# this is the script you will be executing. Usually just mergeScript.sh
# export executableScript=$workingDirectory/libsh/mergeScript.sh
export executableScript=

