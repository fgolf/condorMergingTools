#! /bin/bash

#replace this later with $1
#inputList=~/MCNtupling/CMSSW/CMSSW_5_3_2_patch4_V05-03-13/crab/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12_DR53X-PU_S10_START53_V7A-v1/mergeFiles/mergeLists/merged_list_1.txt
inputList=$1
#replace with mdata input $2
# mData=~/MCNtupling/CMSSW/CMSSW_5_3_2_patch4_V05-03-13/crab/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12_DR53X-PU_S10_START53_V7A-v1/mergeFiles/metaData.txt
mData=$2

#output
outputDir=$3

CMSSWRelease=CMSSW_5_3_2_patch4
CMS2Tag=V05-03-18
CMS2Tar=CMSSW_5_3_2_patch4_V05-03-18.tgz

mkdir tempdir
cd tempdir
cp ../* .
echo "current files in directory:"
echo `ls -l`
inputList=`basename $inputList`
echo "Attempting to merge files from $inputList"
mData=`basename $mData`
echo "using metadata file: $mData"

source /code/osgcode/cmssoft/cms/cmsset_default.sh > /dev/null 2>&1
export SCRAM_ARCH=slc5_amd64_gcc462
echo $SCRAM_ARCH

scram list
scram project -n ${CMSSWRelease}_$CMS2Tag CMSSW $CMSSWRelease
# scramv1 p -n CMSSW_5_3_2_patch4 CMSSW CMSSW_5_3_2_patch4
mv $CMS2Tar ${CMSSWRelease}_$CMS2Tag/
cd ${CMSSWRelease}_$CMS2Tag
tar -xzf $CMS2Tar #this is an overkill for now. we can experiment with dropping parts of it.
eval `scram runtime -sh`
cd -

echo "Compiling sweepRoot macro."
#compiles sweeproot
make

echo "Attempting to merge and slim files in list: $inputList"
# this returns the name of the list and turns it into a merged_ntuple name
inputList=`basename $inputList`
outFileName=`echo $inputList | sed 's/list/ntuple/g' | sed 's/txt/root/g'`
echo "outFileName: $outFileName"

csvFiles=
while read file; do
echo "Adding files to list for the slim_cfg"
	if [ "$csvFiles" == "" ]; then
		# echo "adding first file to csv list"
		csvFiles="\"file:$file\""
		# echo $file
	else 
		# echo "adding file to csv list"
		csvFiles="$csvFiles,\n\"file:$file\""
		# echo $file
	fi
	
done < "$inputList"

csvFiles=`echo -e "$csvFiles"`
echo "csv files list:"
echo "$csvFiles"


echo -e "Writing cms2Slim_cfg.py:\n"

echo "import FWCore.ParameterSet.Config as cms" > cms2Slim_cfg.py
echo >> cms2Slim_cfg.py
echo "process = cms.Process(\"CMS2\")" >> cms2Slim_cfg.py
echo >> cms2Slim_cfg.py
echo "process.source = cms.Source(\"PoolSource\"," >> cms2Slim_cfg.py
echo "    skipEvents = cms.untracked.uint32(0)," >> cms2Slim_cfg.py
echo "    fileNames  = cms.untracked.vstring(" >> cms2Slim_cfg.py
for file in $csvFiles; do
	echo "        $file" >> cms2Slim_cfg.py
done
        # "file:/hadoop/cms/store/group/snt/papers2012/Summer12_53X_MC/TprimeTprimeToBWBW_M-300_TuneZ2star_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1/V05-03-13/merged_ntuple.root"
#        "file:/hadoop/cms/store/user/cwelke/CMS2_V05-03-13/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12_DR53X-PU_S10_START53_V7A-v1/ntuple_1742_1_RTi.root"        # "file:/home/users/fgolf/devel/CMSSW_5_3_2_patch4/src/CMS2/NtupleMaker/test/full_cms2.root"
echo "    )," >> cms2Slim_cfg.py
echo ")" >> cms2Slim_cfg.py
echo >> cms2Slim_cfg.py         
echo "process.out = cms.OutputModule(" >> cms2Slim_cfg.py
echo "    \"PoolOutputModule\"," >> cms2Slim_cfg.py
echo "    fileName     = cms.untracked.string(\"merged_ntuple.root\")" >> cms2Slim_cfg.py
echo ")" >> cms2Slim_cfg.py
echo "process.outpath      = cms.EndPath(process.out)" >> cms2Slim_cfg.py
echo >> cms2Slim_cfg.py
echo "process.out.outputCommands = cms.untracked.vstring(\"keep *\")" >> cms2Slim_cfg.py
echo >> cms2Slim_cfg.py
echo "from CMS2.NtupleMaker.SlimCms2_cff import slimcms2" >> cms2Slim_cfg.py
echo "process.out.outputCommands.extend(slimcms2)" >> cms2Slim_cfg.py

echo "Finished writing cms2Slim_cfg.py"

while read line; do
	echo $line
done < "cms2Slim_cfg.py"


echo -e "Attempting to slim and merge:\n"
cmsRun cms2Slim_cfg.py > cmsRunLog.txt 2>&1

didSlim=$?
# didSlim="0"

if [ "$didSlim" == "0" ]; then 
	echo "Files merged successfully!"
else
	echo "Something went wrong with slimming. Exiting."
	exit 1
fi

echo "checking File with sweepRoot"
./sweepRoot -o "Events" `pwd`"/merged_ntuple.root"
isGoodFile=$?

if [ $isGoodFile == 0 ]; then
	echo "File looks fine."
else
	echo "File did not pass sweepRoot. Exiting."
	exit 2
fi

echo -e "Attempting to add cms2 branches now:"

#Adds cms2 branches to the file. If it fails, it tries again.
root -b -q -l "runAddBranches.C (\"$mData\",\"merged_ntuple.root\",\"$outFileName\")"
./sweepRoot -o "Events" `pwd`/$outFileName
didAddBranches=$?
if [ $didAddBranches != 0 ]; then
	echo "Error in adding cms2 branches. Attempting one more time.."

	if [ -e $outFileName ]; then
		echo "removing $outFileName"
		rm $outFileName
	fi
	
	root -b -q -l "runAddBranches.C (\"$mData\",\"merged_ntuple.root\",\"$outFileName\")"
	./sweepRoot -o "Events" `pwd`/$outFileName
	didAddBranches=$?
fi

echo "filename = $outFileName"
echo "Outputting file to $outputDir from worker node."
localFile=`pwd`/${outFileName}
if [ $didAddBranches == 0 ]; then
	echo -e "copying file now from : \n$localFile\nTo:\n$outputDir"
	lcg-cp -b -D srmv2 --vo cms -t 2400 --verbose file:`pwd`/${outFileName} srm://bsrm-1.t2.ucsd.edu:8443/srm/v2/server?SFN=$outputDir/${outFileName}
	stageout_error=$?
	if [ $stageout_error != 0 ]; then
		echo "Error merging files in $inputList. Job exit code $stageout_error. Stageout with lcg-cp failed."
	fi
	if [ $stageout_error == 0 ]; then
		echo "Merging files in $inputList successful. Job exit code $error." # Error occurred while running makeSkime.C."
	fi
else
	echo "Something went wrong adding branches. Exiting."
	exit 12	
fi

echo "Cleaning up..."

rm merged_ntuple* *.so cms2Slim_cfg.py cmsRunLog.txt

echo "End of Script."
