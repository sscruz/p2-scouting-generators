# create a project


set cflags "-std=c++11 "

open_project -reset "proj_tkem"
# specify the name of the function to synthetize
set_top tkem_generator
# load source code for synthesis

add_files firmware/data.cc -cflags ${cflags}
add_files firmware/tkem.cc -cflags ${cflags}

# load source code for the testbench
add_files -tb tkem_test.cpp -cflags ${cflags}

# create a solution (i.e. a hardware configuration for synthesis)
open_solution -reset "solution"
# set the FPGA (VU9P), and a 360 MHz clock
set_part {xcku15p-ffva1760-2-e}
create_clock -period 2.777 

csim_design
csynth_design

exit
