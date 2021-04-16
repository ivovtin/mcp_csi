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
#$ -m beas
#$ -M kirill8851@yandex.ru

##. conv_from_binFile.sh
##root -l -q 'plot_runs.C("2021-04-08_08-42-18",0.5,125819,0,0)'   ##Umcp=2600V; Upc=2900V;
##root -l -q 'plot_runs.C("2021-04-08_01-45-33",0.5,74125,0,0)'    ##Umcp=2200V; Upc=2100V;
##root -l -q 'plot_runs.C("2021-04-08_09-00-26",0.5,90000,0,0)'    ##Umcp=2600V; Upc=3200V;
root -l -q 'plot_runs.C("2021-04-08_03-59-57",0.5,76002,0,0)'      ##Umcp=2400V; Upc=3000V;


status=$?
if [ $status != 0 ]; then
  echo "Program exited with status $status"
  exit
fi

echo "Job finished\n"
