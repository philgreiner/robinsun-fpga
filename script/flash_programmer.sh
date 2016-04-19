#!/bin/sh
#
# This file was automatically generated.
#
# It can be overwritten by nios2-flash-programmer-generate or nios2-flash-programmer-gui.
#

#
# Converting SOF File: D:\Documents\Github\robinsun-fpga\robinsun_fpga.sof to: "..\flash/robinsun_fpga_epcs_flash_controller_0.flash"
#
sof2flash --input="D:/Documents/Github/robinsun-fpga/robinsun_fpga.sof" --output="../flash/robinsun_fpga_epcs_flash_controller_0.flash" --epcs --verbose 

#
# Programming File: "..\flash/robinsun_fpga_epcs_flash_controller_0.flash" To Device: epcs_flash_controller_0
#
nios2-flash-programmer "../flash/robinsun_fpga_epcs_flash_controller_0.flash" --base=0x0 --epcs --accept-bad-sysid --device=1 --instance=0 '--cable=USB-Blaster on localhost [USB-0]' --program --verbose 

#
# Converting ELF File: D:\Documents\Github\robinsun-fpga\software\software_LT24\software_LT24.elf to: "..\flash/software_LT24_epcs_flash_controller_0.flash"
#
elf2flash --input="software_LT24.elf" --output="../flash/software_LT24_epcs_flash_controller_0.flash" --epcs --after="../flash/robinsun_fpga_epcs_flash_controller_0.flash" --verbose 

#
# Programming File: "..\flash/software_LT24_epcs_flash_controller_0.flash" To Device: epcs_flash_controller_0
#
nios2-flash-programmer "../flash/software_LT24_epcs_flash_controller_0.flash" --base=0x0 --epcs --accept-bad-sysid --device=1 --instance=0 '--cable=USB-Blaster on localhost [USB-0]' --program --verbose 

