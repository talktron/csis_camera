# ============================================================================
# Copyright (c) Texas Instruments Inc 2013
#
# Use of this software is controlled by the terms and conditions found in the
# license agreement under which this software has been supplied or provided.
# ============================================================================

# Default build environment, windows or linux
ifeq ($(OS), )
  OS := Linux
endif

#####################################################################################
#CONFIGURATION
#####################################################################################

SYSTEM_PLATFORM  := CSK


# Build configuration
#APP_BUILD_CFG    := debug
APP_BUILD_CFG   := release

# Binary to boot from NAND or SDCARD  , default is sd
#BINARY_MODE   := nand
BINARY_MODE   := sd

# Choose the target platform that is needed
# Values are "DM388" and "DM8127"
IPNC_DEVICE    := DM388
#IPNC_DEVICE    := DM8127


# Values are "LOW_POWER" and "FULL_FEATURE"
IPNC_CONFIG := FULL_FEATURE
#IPNC_CONFIG := LOW_POWER

# Memory foot print	7/28/22 DAT
MEMORY_CONFIG := 512MB
#MEMORY_CONFIG := 256MB

IMGS_ID := IMGS_MICRON_AR0522

# TILER Memory enable
IPNC_TILER_ENABLE := NO
CAMERA_TILER_ENABLE  := NO
BTE_ENABLE := NO

# Codec Setting Parameters
# Mjpeg Rate Control Mode ( VBR, CBR, QPMODE)
MJPEG_RC_MODE := MJPEG_QPMODE
#MJPEG_RC_MODE := MJPEG_VBR_MODE
#MJPEG_RC_MODE := MJPEG_CBR_MODE

# WDR mode: If sensor supports WDR mode output then it will be turn on
# WDR link will replace GLBCE link in the full feature use case
#WDR_ON := YES
WDR_ON := NO

# Build warnings will be treated as errors for ipnc_mcfw folder
#BUILD_WARNINGS_AS_ERROR := NO
BUILD_WARNINGS_AS_ERROR := YES

# Noise Filter mode supported for DMVA3 platform
# Options are TNF3 or MCTNF
#DMVA3_VNF_MODE := TNF3
DMVA3_VNF_MODE := MCTNF

# MCTNF mode supported for DM388 platform
# Set this flag to "NO" will disable MCTNF for DM388
DM388_MCTNF_ON := YES

# GUI type needed can be specified here
# GODB_DEFAULT vendor can be used if Default GUI screens are to be selected
GUI_ENGINE_MODE := GODB_DEFAULT

# Sensor Board Vendor - LIMG: since Leopard Imaging Board is used
SENSOR_BOARD_VENDOR := LIMG

# Special usecase flags are given here, will override the normal IPNC setting
# User need to ensure that IPNC_CONFIG is in FULL_FEATURE mode for this usecase
################################################################################

# This usecase enables capture + encode + decode + display dataflow
# this  is normally useful for video-conf usecase who use IPNCRDK as codebase
ENC_A8_DEC_USE_CASE := NO

# This is Quad stream usecase of H264 1080p@30fps+H264 D1@30fps+H264 CIF@30fps+MJPEG 1080p@5fps
# In this mode noisefilter is applied only once on 1080p stream and all other resolution are derived from it
QUAD_STREAM_NF_USE_CASE := NO

# This usecase enables Capture + Display mode, normally used for sensor bringup and during IPIPE tunnning
# Set this flag FRAMES_TO_A8 to "YUV" will send secondary stream only with a round trip M3-A8-M3 to reach the SD display
# Set this flag FRAMES_TO_A8 to "RAW" will send RAW frames to A8 and then back to M3
# This flag FRAMES_TO_A8 can be set only if CAPTURE_DISPLAY_MODE_ON is set to YES
# Set SPARSE_OPTICAL_FLOW_ON to "YES" to enable the Sparse Optical Flow demo in the Capture Display use case.
CAPTURE_DISPLAY_MODE_ON := NO
FRAMES_TO_A8 := NO
SPARSE_OPTICAL_FLOW_ON := NO

# for MCTNF demo: Does Capture->MCTNF->VNF->Display side-by-side: filtered+non-filtered
CAPTURE_MCTNF_DISP_MODE_ON := NO

# for RVM demo: 
RVM_DEMO_MODE := NO

ifeq ($(ENC_A8_DEC_USE_CASE), YES)
    CAPTURE_DISPLAY_MODE_ON := NO
    CAPTURE_MCTNF_DISP_MODE_ON := NO
    QUAD_STREAM_NF_USE_CASE := NO
    IPNC_CONFIG := FULL_FEATURE
endif
ifeq ($(QUAD_STREAM_NF_USE_CASE), YES)
    CAPTURE_DISPLAY_MODE_ON := NO
    CAPTURE_MCTNF_DISP_MODE_ON := NO
    ENC_A8_DEC_USE_CASE := NO
    IPNC_CONFIG := FULL_FEATURE
endif
ifeq ($(CAPTURE_DISPLAY_MODE_ON), YES)
    QUAD_STREAM_NF_USE_CASE := NO
    ENC_A8_DEC_USE_CASE := NO
    IPNC_CONFIG := FULL_FEATURE
endif
    
ifeq ($(CAPTURE_MCTNF_DISP_MODE_ON), YES)
    QUAD_STREAM_NF_USE_CASE := NO
    ENC_A8_DEC_USE_CASE := NO
    IPNC_CONFIG := FULL_FEATURE
endif
################################################################################
# All build tools paths are set in this section
################################################################################

BASE_INSTALL_DIR    := $(shell pwd)/..

# Defining the install base directory for IPNC RDK
IPNC_INSTALL_DIR    := $(BASE_INSTALL_DIR)/ipnc_rdk
TOOLS_INSTALL_DIR   := $(BASE_INSTALL_DIR)/ti_tools

# Defining the build tools directory for IPNC RDK
BUILD_TOOL_DIR      := $(TOOLS_INSTALL_DIR)/linux_devkit
BUILD_TOOL_PREFIX   := $(BUILD_TOOL_DIR)/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
CODEGEN_PATH_A8     := $(BUILD_TOOL_DIR)

# Defining all the tools that are required for IPNC RDK
bios_PATH         := $(TOOLS_INSTALL_DIR)/bios_6_37_01_24
CODEGEN_PATH_DSP  := $(TOOLS_INSTALL_DIR)/cgt6x_7_4_6
CODEGEN_PATH_M3   := $(TOOLS_INSTALL_DIR)/cgt470_5_1_3
edma3lld_PATH     := $(TOOLS_INSTALL_DIR)/edma3_lld_02_11_10_09
fc_PATH           := $(TOOLS_INSTALL_DIR)/framework_components_3_30_00_06
ipc_PATH          := $(TOOLS_INSTALL_DIR)/ipc_1_25_03_15
hdvicplib_PATH    := $(TOOLS_INSTALL_DIR)/ivahd_hdvicp20api_01_00_00_23
linuxdevkit_PATH  := $(TOOLS_INSTALL_DIR)/linux_devkit/arm-arago-linux-gnueabi
xdais_PATH        := $(TOOLS_INSTALL_DIR)/xdais_7_24_00_04
xdc_PATH          := $(TOOLS_INSTALL_DIR)/xdctools_3_25_05_94
linuxutils_PATH   := $(TOOLS_INSTALL_DIR)/linuxutils_3_23_00_01
syslink_PATH      := $(TOOLS_INSTALL_DIR)/syslink_2_21_02_10
hdvpss_PATH       := $(TOOLS_INSTALL_DIR)/hdvpss_01_00_01_37
iss_PATH       	  := $(TOOLS_INSTALL_DIR)/iss_03_80_00_00
vlib_PATH         := $(TOOLS_INSTALL_DIR)/vlib_c674x_3_3_0_3

# Defining all the codec components that are required for IPNC RDK
codecs_DIR        := $(TOOLS_INSTALL_DIR)/codecs-dm814x
h264enc_DIR       := $(codecs_DIR)/REL.500.V.H264AVC.E.IVAHD.02.00.09.01
h264dec_DIR       := $(codecs_DIR)/REL.500.V.H264AVC.D.HP.IVAHD.02.00.21.01
mpeg4enc_DIR      := $(codecs_DIR)/REL.500.V.MPEG4.E.SP.IVAHD.01.00.02.01
mpeg4dec_DIR      := $(codecs_DIR)/REL.500.V.MPEG4.D.IVAHD.01.00.15.01
jpegenc_DIR       := $(codecs_DIR)/REL.500.V.MJPEG.E.IVAHD.01.00.16.01
jpegdec_DIR       := $(codecs_DIR)/REL.500.V.MJPEG.D.IVAHD.01.00.16.01
mctnf_DIR         := $(codecs_DIR)/REL.500.V.MCTNF.IVAHD.00.13.01.00
h264enc_PATH      := $(h264enc_DIR)/500.V.H264AVC.E.IVAHD.02.00/IVAHD_001
h264dec_PATH      := $(h264dec_DIR)/500.V.H264AVC.D.HP.IVAHD.02.00/IVAHD_001
jpegenc_PATH      := $(jpegenc_DIR)/500.V.MJPEG.E.IVAHD.01.00/IVAHD_001
jpegdec_PATH      := $(jpegdec_DIR)/500.V.MJPEG.D.IVAHD.01.00/IVAHD_001
mpeg4enc_PATH     := $(mpeg4enc_DIR)/500.V.MPEG4.E.SP.IVAHD.01.00/IVAHD_001
mpeg4dec_PATH     := $(mpeg4dec_DIR)/500.V.MPEG4.D.IVAHD.01.00.15.01/IVAHD_001
mctnf_PATH        := $(mctnf_DIR)/500.V.MCTNF.IVAHD.00.13/IVAHD_001

# The directory that points to the Linux Support Package
lsp_PATH	  := $(TOOLS_INSTALL_DIR)/ipnc_psp_arago
KERNELDIR         := $(lsp_PATH)/kernel
UBOOTDIR          := $(lsp_PATH)/u-boot

# The directory that points to where filesystem is mounted
FILESYS_INSTALL_DIR  := $(IPNC_INSTALL_DIR)/target
TARGET_FS            := $(FILESYS_INSTALL_DIR)/filesys
TARGET_FS_DIR        := $(TARGET_FS)
MTD_UTILS            := $(FILESYS_INSTALL_DIR)/mtd-utils

# The directory that points IPNC RDK source code
MCFW_ROOT_PATH    := $(IPNC_INSTALL_DIR)/ipnc_mcfw
IPNC_DIR          := $(IPNC_INSTALL_DIR)/ipnc_app
TFTP_HOME         := $(IPNC_INSTALL_DIR)/tftp/$(IPNC_DEVICE)/$(BINARY_MODE)

# SYSLINK related options/paths are set here
ifeq ($(IPNC_DEVICE), DM8127)
    syslink_DEVICE := TI814X
else
    syslink_DEVICE := TI813X
endif
syslink_INCLUDE := $(syslink_PATH)/packages
syslink_LIB_DIR := $(syslink_INCLUDE)/ti/syslink/lib
syslink_OUT_DIR := $(syslink_PATH)/packages/ti/syslink/bin/$(syslink_DEVICE)

# The directory to MCFW binaries
# Hardware config and Platform config setting being done here
# For REV2 , dm388 config is used and for REV1, DM385 config is used
ifeq ($(IPNC_DEVICE), DM8127)
    TARGET_MCFW_DIR 	:= $(MCFW_ROOT_PATH)/bin/ti814x
    PLATFORM_CFG    	:= dm8127
else
    TARGET_MCFW_DIR := $(MCFW_ROOT_PATH)/bin/ti810x
   	PLATFORM_CFG    := dm38x
endif

# The directory to application interface
GLOBAL_INCLUDE_DIR := $(BUILD_TOOL_DIR)/rootfs/include/
IPNC_INTERFACE_DIR := $(IPNC_DIR)/interface

# The directory to application interface include files
PUBLIC_INCLUDE_DIR := $(IPNC_INTERFACE_DIR)/inc

# The directory to application interface library files
APP_LIB_DIR := $(IPNC_INTERFACE_DIR)/lib

# The directory to CMEM library
CMEM_LIB_DIR := $(linuxutils_PATH)/packages/ti/sdo/linuxutils/cmem/lib
CMEM_INC_DIR := $(linuxutils_PATH)/packages/ti/sdo/linuxutils/cmem/include

# The directory to root file system
ROOT_FILE_SYS:= $(TARGET_FS)

# Where to copy the resulting executables and data to (when executing 'make
# install') in a proper file structure. This EXEC_DIR should either be visible
# from the target, or you will have to copy this (whole) directory onto the
# target filesystem.
EXEC_DIR:=$(TARGET_FS)/opt/ipnc

################################################################################
# Combinations checks for the options set above are done here 
################################################################################

# Board type can be one of the following
#   1. DM814X_IPNC
#   2. DM8107_IPNC
ifeq ($(IPNC_DEVICE), DM8127)
    DVR_RDK_BOARD_TYPE := DM814X_IPNC
else
    DVR_RDK_BOARD_TYPE := DM8107_IPNC
endif   

# Values are "NULL", "FPGA" or "LVDS324"
SENSOR_DATA_OP := SENSOR_DATA_OP_NULL
ifeq ($(IPNC_DEVICE), DM8127)
  ifeq ($(IMGS_ID), IMGS_PANASONIC_MN34041)
    SENSOR_DATA_OP:= MN34041_DATA_OP_FPGA
  endif
  ifeq ($(SYSTEM_PLATFORM), CSK)
	ifeq ($(IMGS_ID), IMGS_MICRON_AR0331)
	 SENSOR_DATA_OP:= AR0331_DATA_OP_LVDS324
	endif
  endif
else
  ifeq ($(IMGS_ID), IMGS_SONY_IMX136)
    SENSOR_DATA_OP:= IMX136_DATA_OP_LVDS324
  endif
  ifeq ($(IMGS_ID), IMGS_PANASONIC_MN34041)
    SENSOR_DATA_OP:= MN34041_DATA_OP_LVDS324
  endif
  ifeq ($(IMGS_ID), IMGS_SONY_IMX104)
    SENSOR_DATA_OP:= IMX104_LVDS324
  endif
  ifeq ($(SYSTEM_PLATFORM), CSK)
	ifeq ($(IMGS_ID), IMGS_MICRON_AR0331)
	 SENSOR_DATA_OP:= AR0331_DATA_OP_LVDS324
	endif
  endif
endif

ifeq ($(IMGS_ID), IMGS_MICRON_AR0330)
    CAPTURE_MODE:= MIPI_CSI2
endif
ifeq ($(IMGS_ID), IMGS_SONY_IMX140)
    CAPTURE_MODE:= MIPI_CSI2
endif

# 10/15/22 DAT
ifeq ($(IMGS_ID), IMGS_MICRON_AR0522)
    CAPTURE_MODE:= MIPI_CSI2
endif

ifeq ($(IPNC_CONFIG), FULL_FEATURE)
    # Values are "NO" and "YES"
    ifeq ($(IPNC_DEVICE), DM8127)
        POWER_OPT_DSP_OFF := NO
        POWER_OPT_DSS_OFF := NO
    else
        POWER_OPT_DSP_OFF := YES
        POWER_OPT_DSS_OFF := NO 
    endif
else
    # Values are "NO" and "YES"
    POWER_OPT_DSP_OFF := YES
    POWER_OPT_DSS_OFF := YES
    CAPTURE_DISPLAY_MODE_ON := NO
    CAPTURE_MCTNF_DISP_MODE_ON := NO
endif

ifeq ($(CAPTURE_DISPLAY_MODE_ON), YES)
    POWER_OPT_DSP_OFF := YES
    ifeq ($(SPARSE_OPTICAL_FLOW_ON), YES)
        POWER_OPT_DSP_OFF := NO
    endif
endif
ifeq ($(CAPTURE_MCTNF_DISP_MODE_ON), YES)
    POWER_OPT_DSP_OFF := YES
endif
ifeq ($(MEMORY_CONFIG), 256MB)
    POWER_OPT_DSP_OFF := YES
    IPNC_TILER_ENABLE := NO  
    CAMERA_TILER_ENABLE := NO
    BTE_ENABLE := NO
    MEMORY_256MB := YES
    DMVA3_VNF_MODE := TNF3
else
    MEMORY_256MB := NO
endif

# Default platform
ifeq ($(IPNC_DEVICE), DM8127)
  PLATFORM := ti814x-evm
else
  PLATFORM := ti810x-evm
endif

# Default profile
ifeq ($(PROFILE_m3video), )
  PROFILE_m3video := $(APP_BUILD_CFG)
endif

ifeq ($(PROFILE_m3vpss), )
  PROFILE_m3vpss := $(APP_BUILD_CFG)
endif

ifeq ($(PROFILE_c6xdsp), )
  PROFILE_c6xdsp := $(APP_BUILD_CFG)
ifeq ($(APP_BUILD_CFG), whole_program_debug)
  PROFILE_c6xdsp := release
endif  
endif

# Default klockwork build flag
ifeq ($(KW_BUILD), )
  KW_BUILD := no
endif

#Teardown method for reloading usecases
TEARDOWN_LOAD_UNLOAD := NO

# Option to speed up GLBCE/SALDRE by offloading tonalcurve calculation on A8
USE_A8_FOR_TC_CALC := YES

# Option where from the BoxCar comes from ISS or calculated by iMX
ifeq ($(WDR_ON), YES)
    USE_ISS_BOXCAR := NO
else 
ifeq ($(USE_A8_FOR_TC_CALC), YES)
    USE_ISS_BOXCAR := YES
else
    USE_ISS_BOXCAR := NO
    endif
endif

ifeq ($(BUILD_WARNINGS_AS_ERROR), YES)
TREAT_WARNINGS_AS_ERROR := yes
endif

ifeq ($(SYSTEM_PLATFORM), IPNC)
	export SYSTEM_CFG := ipnc
else
ifeq ($(SYSTEM_PLATFORM), CSK)
	export SYSTEM_CFG := csk
else
	export SYSTEM_CFG := evm
	endif
endif

ifeq (, $(findstring $(UBOOTDIR)/tools, $(PATH)))
export PATH:=$(UBOOTDIR)/tools:$(PATH)
endif

PLATFORM_NAME := IPNC_RDK_PLATFORM
FS_CFG  := ubifs
ROOTDIR := $(MCFW_ROOT_PATH)

SC_SCRIPTS_BASE_DIR     := $(TOOLS_BASE_DIR)/coding_guidelines/scripts

XDCPATH = $(bios_PATH)/packages;$(xdc_PATH)/packages;$(ipc_PATH)/packages;$(hdvpss_PATH)/packages;$(fc_PATH)/packages;$(MCFW_ROOT_PATH);$(syslink_PATH)/packages;$(xdais_PATH)/packages;$(edma3lld_PATH)/packages

################################################################################
# All EXPORT symbols are defined here
################################################################################
export OS
export PLATFORM
export CORE
export PROFILE_m3vpss
export PROFILE_m3video
export PROFILE_c6xdsp
export CODEGEN_PATH_M3
export CODEGEN_PATH_A8
export CODEGEN_PATH_DSP
export ROOTDIR
export bios_PATH
export xdc_PATH
export iss_PATH
export vlib_PATH
export hdvpss_PATH
export ipc_PATH
export fc_PATH
export xdais_PATH
export h264dec_PATH
export h264enc_PATH
export jpegenc_PATH
export jpegdec_PATH
export mctnf_PATH
export hdvicplib_PATH
export linuxdevkit_PATH
export edma3lld_PATH
export XDCPATH
export KW_BUILD
export syslink_PATH
export KERNELDIR
export TARGET_FS
export TARGET_MCFW_DIR
export ROOT_FILE_SYS
export MCFW_ROOT_PATH
export UBOOTDIR
export DVR_RDK_BOARD_TYPE
export DEST_ROOT
export IPNC_INSTALL_DIR
export TFTP_HOME
export LINUX_MEM
export BUILD_TOOL_DIR
export BUILD_TOOL_PREFIX
export IPNC_DIR
export PUBLIC_INCLUDE_DIR
export GLOBAL_INCLUDE_DIR
export APP_LIB_DIR
export EXEC_DIR
export syslink_INCLUDE
export syslink_LIB_DIR
export ipc_INCLUDE
export linuxutils_PATH
export IPNC_CONFIG
export POWER_OPT_DSP_OFF 
export POWER_OPT_DSS_OFF 
export IMGS_ID
export TREAT_WARNINGS_AS_ERROR
export SC_SCRIPTS_BASE_DIR
export CMEM_LIB_DIR
export CMEM_INC_DIR
export mpeg4enc_PATH
export mpeg4dec_PATH
export CAPTURE_DISPLAY_MODE_ON
export SPARSE_OPTICAL_FLOW_ON
export CAPTURE_MCTNF_DISP_MODE_ON
export FRAMES_TO_A8
export MEMORY_256MB
export IPNC_TILER_ENABLE
export CAMERA_TILER_ENABLE
export BTE_ENABLE
export IPNC_DEVICE
export TARGET_FS_DIR
export USE_A8_FOR_TC_CALC
export USE_ISS_BOXCAR
export TEARDOWN_LOAD_UNLOAD
export SENSOR_DATA_OP
export ENC_A8_DEC_USE_CASE
export QUAD_STREAM_NF_USE_CASE
export CAPTURE_MODE
export WDR_ON
export MJPEG_RC_MODE
export PLATFORM_NAME
export RVM_DEMO_MODE
export DMVA3_VNF_MODE
export SENSOR_BOARD_VENDOR
export DM388_MCTNF_ON
