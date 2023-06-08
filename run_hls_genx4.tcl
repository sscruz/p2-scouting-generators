# create a project
open_project -reset "proj_genx4"
# specify the name of the function to synthetize
set_top genx4
# load source code for synthesis
set cflags "-std=c++11"
add_files firmware/genx4.cc -cflags ${cflags}
# load source code for the testbench
add_files -tb genx4_test.cpp -cflags ${cflags}

# create a solution (i.e. a hardware configuration for synthesis)
open_solution -reset "solution"
# set the FPGA (VU9P), and a 320 MHz clock
set_part {xcku15p-ffva1760-2-e}
create_clock -period 2.6 

csim_design
if { [info exists env(DO_SYNTH)] && $env(DO_SYNTH) == "1"  } {
    csynth_design
    cosim_design  -trace_level all 
}
exit