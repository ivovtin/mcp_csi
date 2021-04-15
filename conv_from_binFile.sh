#!/bin/bash/

##indirs=( "2021-04-08_00-01-24" "2021-04-08_00-31-38" )
indirs=( "2021-04-08_03-01-22" "2021-04-08_03-10-31" "2021-04-08_03-20-53" "2021-04-08_03-34-54" "2021-04-08_03-59-57" "2021-04-08_08-24-24" "2021-04-08_08-34-43" "2021-04-08_08-42-18" "2021-04-08_08-52-29" "2021-04-08_09-00-26")

for indir in ${indirs[@]}
do
  mkdir "$indir"
  outfile="$indir.root"

  cd "$indir"
  nice -n 19 ../proc2021tbeamnogem /home/ovtin/mcp_csi_070421/$indir/wave_0.dat /home/ovtin/mcp_csi_070421/$indir/wave_4.dat /home/ovtin/mcp_csi_070421/$indir/wave_2.dat $outfile -1000 20 0.0 0.0 2
  cd ../
done 

