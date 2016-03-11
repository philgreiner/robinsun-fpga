transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -sv -work work +incdir+D:/Documents/Github/robinsun-fpga {D:/Documents/Github/robinsun-fpga/sonars.sv}
vlog -sv -work work +incdir+D:/Documents/Github/robinsun-fpga {D:/Documents/Github/robinsun-fpga/robinsun_fpga.sv}
vlog -sv -work work +incdir+D:/Documents/Github/robinsun-fpga {D:/Documents/Github/robinsun-fpga/MySPI.sv}
vlog -sv -work work +incdir+D:/Documents/Github/robinsun-fpga {D:/Documents/Github/robinsun-fpga/encoder.sv}

vlog -sv -work work +incdir+D:/Documents/Github/robinsun-fpga {D:/Documents/Github/robinsun-fpga/testbench.sv}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cycloneive_ver -L rtl_work -L work -voptargs="+acc"  testbench

add wave *
view structure
view signals
run 500 ms
