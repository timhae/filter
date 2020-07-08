############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_interface -mode m_axi -depth 8 "gauss_simple" video_in
set_directive_interface -mode axis -register -register_mode both "gauss_simple" video_out
set_directive_array_partition -type complete -dim 0 "gauss_simple" kernel
set_directive_pipeline "gauss_simple/pipeline"
