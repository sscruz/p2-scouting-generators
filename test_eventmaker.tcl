# create a project


set cflags "-std=c++11 "

open_project -reset "proj_simplia_luts"
set_top dummy

add_files -tb simplia_luts.cc -cflags ${cflags}
open_solution -reset "solution"
set_part {xcku15p-ffva1760-2-e}
create_clock -period 2.777

csim_design
close_project

file copy -force "proj_simplia_luts/solution/csim/build/luts.h" "firmware/luts.h"


open_project -reset "proj_eventmaker"
# specify the name of the function to synthetize
set_top bitpattern_pileup
# load source code for synthesis

add_files firmware/data.cc -cflags ${cflags}
add_files firmware/simplia.cc -cflags ${cflags}
add_files firmware/pileup.cc -cflags ${cflags}
add_files firmware/eventmaker.cc -cflags ${cflags}

# load source code for the testbench
add_files -tb pattern_serializer.cpp -cflags ${cflags}
add_files -tb test_eventmaker.cpp -cflags ${cflags}

# creae a solution (i.e. a hardware configuration for synthesis)
open_solution -reset "solution"
# set the FPGA (VU9P), and a 360 MHz clock
set_part {xcku15p-ffva1760-2-e}
create_clock -period 2.777

csim_design
csynth_design

export_design

exit
