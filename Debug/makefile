################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-armllvm_2.1.2.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./dashboard_task.o" \
"./ethernet_task.o" \
"./html.o" \
"./syscfg/ti_devices_config.o" \
"./syscfg/ti_radio_config.o" \
"./syscfg/ti_drivers_config.o" \
"./syscfg/ti_sysbios_config.o" \
"./main.o" \
"./radio_queue.o" \
"./radio_task.o" \
"./ethernet/ArduinoCompatibility.o" \
"./ethernet/Dhcp.o" \
"./ethernet/Dns.o" \
"./ethernet/Ethernet.o" \
"./ethernet/EthernetBonjour.o" \
"./ethernet/EthernetClient.o" \
"./ethernet/EthernetServer.o" \
"./ethernet/EthernetUdp.o" \
"./ethernet/EthernetUtil.o" \
"./ethernet/IPAddress.o" \
"./ethernet/Printfg.o" \
"./ethernet/SPIfg.o" \
"./ethernet/Serial.o" \
"./ethernet/UnitTest.o" \
"./ethernet/socket.o" \
"./ethernet/w5500.o" \
"./websocket/WebsocketServer.o" \
"../cc13x2_cc26x2_tirtos7.cmd" \
$(GEN_CMDS__FLAG) \
-Wl,-l"ti/grlib/lib/ticlang/m4f/grlib.a" \
-Wl,-l"third_party/spiffs/lib/ticlang/m4f/spiffs_cc26xx.a" \
-Wl,-l"ti/drivers/lib/ticlang/m4f/drivers_cc26x2.a" \
-Wl,-lti_utils_build_linker.cmd.genlibs \
-Wl,-l"ti/devices/cc13x2_cc26x2/driverlib/bin/ticlang/driverlib.lib" \
-Wl,-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include ethernet/subdir_vars.mk
-include websocket/subdir_vars.mk
-include subdir_rules.mk
-include ethernet/subdir_rules.mk
-include websocket/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
mSniff_LP_CC2652RB_tirtos7_ticlang.out \

EXE_OUTPUTS__QUOTED += \
"mSniff_LP_CC2652RB_tirtos7_ticlang.out" \

BIN_OUTPUTS += \
mSniff_LP_CC2652RB_tirtos7_ticlang.hex \

BIN_OUTPUTS__QUOTED += \
"mSniff_LP_CC2652RB_tirtos7_ticlang.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "mSniff_LP_CC2652RB_tirtos7_ticlang.out"

# Tool invocations
mSniff_LP_CC2652RB_tirtos7_ticlang.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-armllvm_2.1.2.LTS/bin/tiarmclang.exe" -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mlittle-endian -mthumb -Oz -DDeviceFamily_CC26X2 -gdwarf-3 -march=armv7e-m -Wl,-m"mSniff_LP_CC2652RB_tirtos7_ticlang.map" -Wl,-i"C:/ti/simplelink_cc13xx_cc26xx_sdk_6_30_01_03/source" -Wl,-i"C:/ti/simplelink_cc13xx_cc26xx_sdk_6_30_01_03/kernel/tirtos7/packages" -Wl,-i"C:/Users/Administrator/workspace_v12/mSniff_LP_CC2652RB_tirtos7_ticlang/Debug/syscfg" -Wl,-i"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-armllvm_2.1.2.LTS/lib" -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="mSniff_LP_CC2652RB_tirtos7_ticlang_linkInfo.xml" -Wl,--rom_model -o "mSniff_LP_CC2652RB_tirtos7_ticlang.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

mSniff_LP_CC2652RB_tirtos7_ticlang.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: Arm Hex Utility'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-armllvm_2.1.2.LTS/bin/tiarmhex.exe" --diag_wrap=off -o "mSniff_LP_CC2652RB_tirtos7_ticlang.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "dashboard_task.o" "ethernet_task.o" "html.o" "syscfg\ti_devices_config.o" "syscfg\ti_radio_config.o" "syscfg\ti_drivers_config.o" "syscfg\ti_sysbios_config.o" "main.o" "radio_queue.o" "radio_task.o" "ethernet\ArduinoCompatibility.o" "ethernet\Dhcp.o" "ethernet\Dns.o" "ethernet\Ethernet.o" "ethernet\EthernetBonjour.o" "ethernet\EthernetClient.o" "ethernet\EthernetServer.o" "ethernet\EthernetUdp.o" "ethernet\EthernetUtil.o" "ethernet\IPAddress.o" "ethernet\Printfg.o" "ethernet\SPIfg.o" "ethernet\Serial.o" "ethernet\UnitTest.o" "ethernet\socket.o" "ethernet\w5500.o" "websocket\WebsocketServer.o" 
	-$(RM) "dashboard_task.d" "ethernet_task.d" "html.d" "syscfg\ti_devices_config.d" "syscfg\ti_radio_config.d" "syscfg\ti_drivers_config.d" "syscfg\ti_sysbios_config.d" "main.d" "radio_queue.d" "radio_task.d" "ethernet\ArduinoCompatibility.d" "ethernet\Dhcp.d" "ethernet\Dns.d" "ethernet\Ethernet.d" "ethernet\EthernetBonjour.d" "ethernet\EthernetClient.d" "ethernet\EthernetServer.d" "ethernet\EthernetUdp.d" "ethernet\EthernetUtil.d" "ethernet\IPAddress.d" "ethernet\Printfg.d" "ethernet\SPIfg.d" "ethernet\Serial.d" "ethernet\UnitTest.d" "ethernet\socket.d" "ethernet\w5500.d" "websocket\WebsocketServer.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

