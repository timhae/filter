############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_interface -mode axis -register -register_mode both "gauss_simple" video_in
set_directive_interface -mode axis -register -register_mode both "gauss_simple" video_out
set_directive_array_partition -type complete -dim 0 "gauss_simple" kernel
set_directive_array_partition -type complete -dim 1 "gauss_simple" line_buf
set_directive_array_partition -type complete "gauss_simple" right_side
set_directive_array_partition -type complete -dim 0 "gauss_simple" window
set_directive_pipeline "gauss_simple/pipe1"
set_directive_pipeline "gauss_simple/pipe2"
set_directive_pipeline "gauss_simple/pipe3"
set_directive_pipeline "gauss_simple/pipe4"
