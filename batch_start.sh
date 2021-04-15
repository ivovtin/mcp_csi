#!/bin/bash
# -------------------------------------------
# --          use bash                     --
#$ -S /bin/bash                            ##specifies the interpreting shell for this job to be the Bash shell.
# -------------------------------------------
# --             batch name                --
#$ -N convert
# -------------------------------------------
# --     What to redirect to where         --
# -- working directory --
#$ -cwd             # run the job in the directory specified.
#$ -o $JOB_NAME.o$JOB_ID
# -- Merge the standard out and standard error to one file --
#$ -j y
##$ -shell n
#$ -V                 ##will pass all environment variables to the job
##$ -e /dev/null
# -------------------------------------------
# --             Queue list                --
#$ -soft
##$ -hard
#$ -l time=24:00:00
##$ -q remote
#
# -- Send mail at submission and completion of script --
##$ -m beas
##$ -M kirill8851@yandex.ru

##. conv_from_binFile.sh
##root -l -q 'plotxy_new3_1.C("2021-04-08_00-01-24",100,0.4,80000,0,0,41,46,17,22)'
##root -l -q 'plotxy_new4.C("2021-04-08_02-19-18",100,0.5,78695,0,0,41,46,17,22)'
root -l -q 'plot_runs.C("2021-04-08_09-00-26",0.5,90000,0,0)'


status=$?
if [ $status != 0 ]; then
  echo "Program exited with status $status"
  exit
fi

echo "Job finished\n"
