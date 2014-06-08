#! /bin/bash

inputFile=$1

CMSSWRelease=CMSSW_5_3_2_patch4
CMS2Tag=V05-03-18

mkdir tempdir
cd tempdir
cp ../* .
echo "current files in directory:"
echo `ls -l`

inputDir=`dirname $inputFile`
inputFileName=`basename $inputFile`
outputDir="${inputDir}_slim/"

source /code/osgcode/cmssoft/cms/cmsset_default.sh > /dev/null 2>&1
export SCRAM_ARCH=slc5_amd64_gcc462
echo $SCRAM_ARCH

scram project -n ${CMSSWRelease}_$CMS2Tag CMSSW $CMSSWRelease
mv $CMS2Tar ${CMSSWRelease}_$CMS2Tag/
cd ${CMSSWRelease}_$CMS2Tag
tar -xzf $CMS2Tar #this is an overkill for now. we can experiment with dropping parts of it.
eval `scram runtime -sh`
cd -

echo "Compiling sweepRoot macro."
#compiles sweeproot
make

echo "Attempting to slim file: $inputFileName"

root -b -q -l "Cms2toSlimCms2macro.C+ (\"$inputDir\",\"$inputFileName\")"

./sweepRoot -o "Events" $inputFileName
didSlim=$?

if [ $didSlim != 0 ]; then
	
	echo "Error, Slimming unsuccessful. Attempting to slim one more time.."
	
	if [ -e "$inputFileName" ]; then
		
		rm $inputFileName
	fi
	
	root -b -q -l "Cms2toSlimCms2macro.C+ (\"$inputDir\",\"$inputFileName\")"
	./sweepRoot -o "Events" $inputFileName
	didSlim=$?

fi

if [ $didSlim != 0 ]; then
	
	echo "Error in second merging attempt. Exiting."
	
	if [ -e "$inputFileName" ]; then
		
		rm $inputFileName
		
	fi
	
	exit 2	
fi

outFileName=$inputFileName

#use lcgcp to stageout
echo "filename = $outFileName"
echo "Outputting file to $outputDir from worker node."
localFile=`pwd`/${outFileName}

if [ $didSlim == 0 ]; then

	echo -e "copying file now from : \n$localFile \nto:\n$outputDir/$outFileName"
	lcg-cp -b -D srmv2 --vo cms -t 2400 --verbose file:`pwd`/${outFileName} srm://bsrm-1.t2.ucsd.edu:8443/srm/v2/server?SFN=$outputDir/${outFileName}
	stageout_error=$?

	if [ $stageout_error != 0 ]; then

		echo "Error slimming file: $inputFileName. Job exit code $stageout_error. Stageout with lcg-cp failed."

	fi

	if [ $stageout_error == 0 ]; then

		echo "Slimming of file: $inputFileName successful. Job exit code $stageout_error." # Error occurred while running makeSkime.C."

	fi
fi

echo "Cleaning up."
rm *.so $outFileName
echo "End of Slimming."
echo "Final file located in $outFileName"

#add white space.
