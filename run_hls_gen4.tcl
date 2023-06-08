# create a project
open_project -reset "proj_gen4"
# specify the name of the function to synthetize
set_top gen4
# load source code for synthesis
set cflags "-std=c++11"
add_files firmware/gen4.cc -cflags ${cflags}
# load source code for the testbench
add_files -tb gen4_test.cpp -cflags ${cflags}

# create a solution (i.e. a hardware configuration for synthesis)
open_solution -reset "solution"
# set the FPGA (VU9P), and a 320 MHz clock
set_part {xcku15p-ffva1760-2-e}
create_clock -period 2.5 

csim_design
if { [info exists env(DO_SYNTH)] && $env(DO_SYNTH) == "1"  } {
    csynth_design
    cosim_design  -trace_level all 
}
exit