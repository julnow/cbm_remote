#!/bin/bash

source /lustre/cbm/users/lubynets/soft/root-6/install-cpp11/bin/thisroot.sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/lustre/cbm/users/$USER/AnalysisTree/install/lib/
export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:/lustre/cbm/users/$USER/AnalysisTree/install/include/AnalysisTree/
#for Pf_simple
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/lustre/cbm/users/$USER/pf_simple/install/external/lib/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/lustre/cbm/users/$USER/pf_simple/install/lib/

WORK=/lustre/cbm/users/$USER/pf_results
#slurm taks id
INDEX=${SLURM_ARRAY_TASK_ID}
#compare files in list
mkdir -p $PWD/comparison
paths=(/lustre/cbm/users/jnowak/pf_results/SetCutCos/*/analysis_tree.root)
path=${paths[$INDEX-1]}
#create root histograms file with folder name from pfinput
folder=${path#/lustre/cbm/users/jnowak/pf_results/SetCutCos/}
folder=${folder%/analysis_tree.root}
output=$PWD/comparison/$folder".root"
root -l -b -q "$WORK/pfRead.c(\"$path\", \"$output\")" >& log_${INDEX}.txt
echo $folder >> log_${INDEX}.txt
#run compareRootFiles
#~/comparerootfiles/build/compareRootFiles -i comparison/*
   
