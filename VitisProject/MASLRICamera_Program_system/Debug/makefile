#
# This file was created by Xilinx Vitis tool.
#


#
# tools
#

RM = rm -f
RMDIR = rm -rf



#
# variables
#

SDCARD_GEN = sdcard_gen
GENERATE_BIF = ::scw::generate_bif

XPFM_PATH = C:/Users/Korisnik/workspace/MASLRICamera/export/MASLRICamera/MASLRICamera.xpfm
SYS_CONFIG = MASLRICamera
DOMAINS = standalone_domain
BIF_PATH = C:/Users/Korisnik/workspace/MASLRICamera_Program_system/Debug/system.bif

BITSTREAM_PATH = C:/Users/Korisnik/workspace/MASLRICamera_Program/_ide/bitstream/design_1_wrapper.bit
SD_FILES = --elf C:/Users/Korisnik/workspace/MASLRICamera_Program/Debug/MASLRICamera_Program.elf,ps7_cortexa9_0

#
# primary build targets
#

all: package

package:
	-@echo Generating bif file for the system project
	generate_system_bif.bat 62430 ${XPFM_PATH} ${DOMAINS} ${BIF_PATH}
	${SDCARD_GEN} --xpfm ${XPFM_PATH} --sys_config ${SYS_CONFIG} --bif ${BIF_PATH} --bitstream ${BITSTREAM_PATH} ${SD_FILES}

clean:
	$(RMDIR) ./_sds
	${RMDIR} package
	${RM} system.bif

.PHONY: all package clean


