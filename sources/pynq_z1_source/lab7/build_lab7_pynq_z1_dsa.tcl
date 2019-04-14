set design_name "lab7_pynq_z1"

# open project and block design
open_project -quiet ./${design_name}/${design_name}.xpr
open_bd_design ./${design_name}/${design_name}.srcs/sources_1/bd/${design_name}/${design_name}.bd

# set sdx platform properties
set_property PFM_NAME "xilinx.com:${design_name}:${design_name}:1.0" \
        [get_files ./${design_name}/${design_name}.srcs/sources_1/bd/${design_name}/${design_name}.bd]
set_property PFM.CLOCK { \
    clk_out1 {id "0" is_default "true" proc_sys_reset "proc_sys_reset_0" } \
    clk_out2 {id "1" is_default "false" proc_sys_reset "proc_sys_reset_1" } \
    clk_out3 {id "2" is_default "false" proc_sys_reset "proc_sys_reset_2" } \
    clk_out4 {id "3" is_default "false" proc_sys_reset "proc_sys_reset_3" } \
    } [get_bd_cells /clk_wiz_0]
set_property PFM.AXI_PORT { \
 	M_AXI_GP0 {memport "M_AXI_GP"} \
 	M_AXI_GP1 {memport "M_AXI_GP"} \
	S_AXI_ACP {memport "S_AXI_ACP" sptag "ACP" memory "ps7_0 ACP_DDR_LOWOCM"} \
	S_AXI_HP0 {memport "S_AXI_HP" sptag "HP0" memory "ps7_0 HP0_DDR_LOWOCM"} \
	S_AXI_HP1 {memport "S_AXI_HP" sptag "HP1" memory "ps7_0 HP1_DDR_LOWOCM"} \
	S_AXI_HP2 {memport "S_AXI_HP" sptag "HP2" memory "ps7_0 HP2_DDR_LOWOCM"} \
	S_AXI_HP3 {memport "S_AXI_HP" sptag "HP3" memory "ps7_0 HP3_DDR_LOWOCM"} \
    } [get_bd_cells /ps7_0]

set intVar []
for {set i 0} {$i < 16} {incr i} {
    lappend intVar In$i {}
}
set_property PFM.IRQ $intVar [get_bd_cells /xlconcat_0]

generate_target all \
[get_files ./${design_name}/${design_name}.srcs/sources_1/bd/${design_name}/${design_name}.bd]

# generate dsa
write_dsa -force ./${design_name}.dsa
validate_dsa ./${design_name}.dsa
