#! /bin/bash

##
# This is the main usage file. Make sure the cfg is written properly. Also, Make sure you have all the permissions to write out to any necessary directories on hadoop. 
# usage:
# ./submitMergeJobs.sh cfg/sample_cfg.sh

configFile=$1
source $configFile

#ensures that there is a directory to stageout to
if [ ! -d $outputDir ]; then
	echo "$outputDir does not exist. Attempting to make."
	mkdir -p $outputDir
	didMakeDir=$?
else
	didMakeDir=0;
fi

if [ "$didMakeDir" == "0" ]; then
	echo "Directory made successfully. "
else
	echo "Directory not made successfully. Exiting."
	exit 1
fi

#this gets the list of lists
filesList=`ls ${inputDir}/merged*`

#chooses how to call the log submission file
dateS=`date '+%Y.%m.%d-%H.%M.%S'`
if [ ! -d /data/tmp/$USER/$dataSet/submit/std_logs/ ]; then
	mkdir -p /data/tmp/$USER/$dataSet/submit/std_logs/
fi

#This loops over the merged file lists and submits a job for each list
for file in $filesList; do

	inputFile=$file
	# inputFile=${inputDir}$file
	inputArguments="$inputFile"
	inputFiles=$executableScript,$workingDirectory/libC/Cms2toSlimCms2macro.C,$workingDirectory/libC/sweepRoot.C,$workingDirectory/libC/Makefile,$workingDirectory/libso/libMiniFWLite_CMSSW_5_3_2_patch4_V05-03-18.so,$workingDirectory/CMSSW_5_3_2_patch4_V05-03-18.tgz

# this takes care of "resubmission. When all your condor jobs are done, this will check that the files are there, and if they are not, it will resubmit a job. If you find a job that has output a corrupted file (which it shouldn't unless stageout fails) just delete the file and run this script again."
	# outFileName=`echo $list | sed 's/list/ntuple/g' | sed 's/txt/root/g'`
	outFileName=$file
	ls -l $outputDir | grep "$outFileName" 
	doesExist=$?
	if [ $doesExist == 0 ]; then
		echo "File exists, will not submit job for $outFileName."
	else
		echo "submitting job for file $outFileName"
		libsh/submit.sh -e $executableScript -a "$inputArguments" -i $inputFiles -u ${workingDirectory}/${dataSet}_cfg -l /data/tmp/$USER/$dataSet/testlog_$dateS.log -L /data/tmp/$USER/$dataSet/submit/std_logs/
	fi
done

echo "All jobs submitted. Check status of jobs with condor_q $USER"
