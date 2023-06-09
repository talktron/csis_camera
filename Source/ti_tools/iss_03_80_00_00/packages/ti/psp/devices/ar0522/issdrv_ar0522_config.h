/** ==================================================================
 *  @file   issdrv_ar0522_config.h
 *
 *  @path   /ti/psp/devices/ar0522/
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/
#ifndef _ISSDRV_AR0522_CONFIG_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define _ISSDRV_AR0522_CONFIG_H_

#define PSP_I2C_MAX_REG_RD_WR       	(0x200)				// 512 regs in AR0522

#define AR_0522_ADDR			(0x36)                     	// AR0521/522 I2C chip address  9/7/22 DAT
#define AR_0522_BOARD_PRECONFIG		(TRUE)                 		// TRUE/FALSE

	typedef struct {
		UInt32 i2cInstId;

		UInt32 numArgs;
		UInt16 regAddr[PSP_I2C_MAX_REG_RD_WR];
		UInt16 regValue[PSP_I2C_MAX_REG_RD_WR];
		UInt32 numRegs;
	} I2c_Ctrl;

	// TODO:  disable all of the following code to see if any link errors
	// 
	// Apparently there was another use of 'SensorConfigScript' where the script is
	// transmitted to the sensor during initialization (other sensors)
	// These scripts are not used in AR0330, AR0331		6/24/22 DAT
	UInt32 SensorConfigScript[353][3] = {
		{0x301A, 2, 0x0001},
		{0x301A, 2, 0x10D8},
		{0x3088, 2, 0x8000},
		{0x3086, 2, 0x4A03},
		{0x3086, 2, 0x4316},
		{0x3086, 2, 0x0443},
		{0x3086, 2, 0x1645},
		{0x3086, 2, 0x4045},
		{0x3086, 2, 0x6017},
		{0x3086, 2, 0x5045},
		{0x3086, 2, 0x404B},
		{0x3086, 2, 0x1244},
		{0x3086, 2, 0x6134},
		{0x3086, 2, 0x4A31},
		{0x3086, 2, 0x4342},
		{0x3086, 2, 0x4560},
		{0x3086, 2, 0x2714},
		{0x3086, 2, 0x3DFF},
		{0x3086, 2, 0x3DFF},
		{0x3086, 2, 0x3DEA},
		{0x3086, 2, 0x2704},
		{0x3086, 2, 0x3D10},
		{0x3086, 2, 0x2705},
		{0x3086, 2, 0x3D10},
		{0x3086, 2, 0x2715},
		{0x3086, 2, 0x3527},
		{0x3086, 2, 0x053D},
		{0x3086, 2, 0x1045},
		{0x3086, 2, 0x4027},
		{0x3086, 2, 0x0427},
		{0x3086, 2, 0x143D},
		{0x3086, 2, 0xFF3D},
		{0x3086, 2, 0xFF3D},
		{0x3086, 2, 0xEA62},
		{0x3086, 2, 0x2728},
		{0x3086, 2, 0x3627},
		{0x3086, 2, 0x083D},
		{0x3086, 2, 0x6444},
		{0x3086, 2, 0x2C2C},
		{0x3086, 2, 0x2C2C},
		{0x3086, 2, 0x4B01},
		{0x3086, 2, 0x432D},
		{0x3086, 2, 0x4643},
		{0x3086, 2, 0x1647},
		{0x3086, 2, 0x435F},
		{0x3086, 2, 0x4F50},
		{0x3086, 2, 0x2604},
		{0x3086, 2, 0x2694},
		{0x3086, 2, 0x2027},
		{0x3086, 2, 0xFC53},
		{0x3086, 2, 0x0D5C},
		{0x3086, 2, 0x0D57},
		{0x3086, 2, 0x5417},
		{0x3086, 2, 0x0955},
		{0x3086, 2, 0x5649},
		{0x3086, 2, 0x5307},
		{0x3086, 2, 0x5303},
		{0x3086, 2, 0x4D28},
		{0x3086, 2, 0x6C4C},
		{0x3086, 2, 0x0928},
		{0x3086, 2, 0x2C28},
		{0x3086, 2, 0x294E},
		{0x3086, 2, 0x5C09},
		{0x3086, 2, 0x4500},
		{0x3086, 2, 0x4580},
		{0x3086, 2, 0x26B6},
		{0x3086, 2, 0x27F8},
		{0x3086, 2, 0x1702},
		{0x3086, 2, 0x27FA},
		{0x3086, 2, 0x5C0B},
		{0x3086, 2, 0x1718},
		{0x3086, 2, 0x26B2},
		{0x3086, 2, 0x5C03},
		{0x3086, 2, 0x1744},
		{0x3086, 2, 0x27F2},
		{0x3086, 2, 0x1702},
		{0x3086, 2, 0x2809},
		{0x3086, 2, 0x1710},
		{0x3086, 2, 0x1628},
		{0x3086, 2, 0x084D},
		{0x3086, 2, 0x1A26},
		{0x3086, 2, 0x9316},
		{0x3086, 2, 0x1627},
		{0x3086, 2, 0xFA45},
		{0x3086, 2, 0xA017},
		{0x3086, 2, 0x0727},
		{0x3086, 2, 0xFB17},
		{0x3086, 2, 0x2945},
		{0x3086, 2, 0x8017},
		{0x3086, 2, 0x0827},
		{0x3086, 2, 0xFA17},
		{0x3086, 2, 0x285D},
		{0x3086, 2, 0x170E},
		{0x3086, 2, 0x2691},
		{0x3086, 2, 0x5301},
		{0x3086, 2, 0x1740},
		{0x3086, 2, 0x5302},
		{0x3086, 2, 0x1710},
		{0x3086, 2, 0x2693},
		{0x3086, 2, 0x2692},
		{0x3086, 2, 0x484D},
		{0x3086, 2, 0x4E28},
		{0x3086, 2, 0x094C},
		{0x3086, 2, 0x0B17},
		{0x3086, 2, 0x5F27},
		{0x3086, 2, 0xF217},
		{0x3086, 2, 0x1428},
		{0x3086, 2, 0x0816},
		{0x3086, 2, 0x4D1A},
		{0x3086, 2, 0x1616},
		{0x3086, 2, 0x27FA},
		{0x3086, 2, 0x2603},
		{0x3086, 2, 0x5C01},
		{0x3086, 2, 0x4540},
		{0x3086, 2, 0x2798},
		{0x3086, 2, 0x172A},
		{0x3086, 2, 0x4A0A},
		{0x3086, 2, 0x4316},
		{0x3086, 2, 0x0B43},
		{0x3086, 2, 0x279C},
		{0x3086, 2, 0x4560},
		{0x3086, 2, 0x1707},
		{0x3086, 2, 0x279D},
		{0x3086, 2, 0x1725},
		{0x3086, 2, 0x4540},
		{0x3086, 2, 0x1708},
		{0x3086, 2, 0x2798},
		{0x3086, 2, 0x5D53},
		{0x3086, 2, 0x0D26},
		{0x3086, 2, 0x455C},
		{0x3086, 2, 0x014B},
		{0x3086, 2, 0x1244},
		{0x3086, 2, 0x5251},
		{0x3086, 2, 0x1702},
		{0x3086, 2, 0x6018},
		{0x3086, 2, 0x4A03},
		{0x3086, 2, 0x4316},
		{0x3086, 2, 0x0443},
		{0x3086, 2, 0x1658},
		{0x3086, 2, 0x4316},
		{0x3086, 2, 0x5943},
		{0x3086, 2, 0x165A},
		{0x3086, 2, 0x4316},
		{0x3086, 2, 0x5B43},
		{0x3086, 2, 0x4540},
		{0x3086, 2, 0x279C},
		{0x3086, 2, 0x4560},
		{0x3086, 2, 0x1707},
		{0x3086, 2, 0x279D},
		{0x3086, 2, 0x1725},
		{0x3086, 2, 0x4540},
		{0x3086, 2, 0x1710},
		{0x3086, 2, 0x2798},
		{0x3086, 2, 0x1720},
		{0x3086, 2, 0x224B},
		{0x3086, 2, 0x1244},
		{0x3086, 2, 0x2C2C},
		{0x3086, 2, 0x2C2C},
		{0x301A, 2, 0x10D8},
		{0x30B0, 2, 0x0000},
		{0x30BA, 2, 0x06EC},
		{0x31AC, 2, 0x0C0C},
		{0x302A, 2, 0x0008},
		{0x302C, 2, 0x0001},
		{0x302E, 2, 0x0002},
		{0x3030, 2, 0x002C},
		{0x3036, 2, 0x000C},
		{0x3038, 2, 0x0001},
		{0x3002, 2, 0x00E4},
		{0x3004, 2, 0x0042},
		{0x3006, 2, 0x0523},
		{0x3008, 2, 0x07C9},
		{0x300A, 2, 0x0465},
		{0x300C, 2, 0x044C},
		{0x3012, 2, 0x0416},
		{0x30A2, 2, 0x0001},
		{0x30A6, 2, 0x0001},
		{0x3040, 2, 0xC000},
		{0x31AE, 2, 0x0301},
		{0x3082, 2, 0x0009},
		{0x318C, 2, 0x0000},
		{0x301A, 2, 0x10D8},
		{0x2400, 2, 0x0003},
		{0x301E, 2, 0x00A8},
		{0x301A, 2, 0x10DC},
		{0x3200, 2, 0x0000},
		{0x31D0, 2, 0x0000},
		{0x30FE, 2, 0x0080},
		{0x31E0, 2, 0x0200},
		{0x320A, 2, 0x0080},
		{0x2450, 2, 0x0000},
		{0x301E, 2, 0x00A8},
		{0x3064, 2, 0x1982},
		{0x3064, 2, 0x1982},
		{0x3180, 2, 0x8089},
		{0x30F4, 2, 0x4000},
		{0x3ED4, 2, 0x8F6C},
		{0x3ED6, 2, 0x6666},
		{0x3EDA, 2, 0x8899},
		{0x3EE6, 2, 0x00F0},
		{0x3ED2, 2, 0x9F46},
		{0x301A, 2, 0x10DC},
		{0x3060, 2, 0x0012},
		{0x305A, 2, 0x009A},
		{0x3056, 2, 0x0083},
		{0x305C, 2, 0x0083},
		{0x3058, 2, 0x00CD},
		{0x3012, 2, 0x0445},
		{0x3060, 2, 0x000E},
		{0x305A, 2, 0x0093},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00D3},
		{0x3060, 2, 0x000C},
		{0x305A, 2, 0x0092},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00D5},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0091},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00D8},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0091},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00D9},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0091},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00DA},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0091},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00D9},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0091},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00D8},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0092},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00D6},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0092},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00D5},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0093},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00D3},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0093},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00D1},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0094},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00D0},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0095},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00CE},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0095},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00CC},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0096},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00CA},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0097},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00C9},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0097},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00C8},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0097},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00C7},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0098},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00C6},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0098},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00C5},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0098},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00C4},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0098},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00C4},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0098},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00C4},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0098},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00C5},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0098},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00C5},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0098},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00C6},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0098},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00C6},
		{0x3060, 2, 0x000B},
		{0x305A, 2, 0x0098},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00C7},
		{0x305E, 2, 0x0080},
		{0x305A, 2, 0x0093},
		{0x3056, 2, 0x0080},
		{0x305C, 2, 0x0080},
		{0x3058, 2, 0x00CF}

	};

	// Apparently there was another use of 'SensorConfigScript' where the script is
	// transmitted to the sensor during initialization (other sensors)
	// These scripts are not used in AR0330, AR0331		6/24/22 DAT
	UInt32 SensorConfigScript_30[65][3] = {
		{0x0100, 2, 0x00},                                     // Mode Select =
															   // 0x0
		{100, 0, 100},
		{0x0300, 2, 0x3},                                      // vt_pix_clk_div
		{0x0302, 2, 0x1},                                      // vt_sys_clk_div
		{0x0304, 2, 0x1},                                      // pre_pll_clk_div
		{0x0306, 2, 0x30},                                     // pll_multiplier
		{0x0308, 2, 0x0C},                                     // op_pix_clk_div
		{0x030A, 2, 0x01},                                     // op_sys_clk_div
		{100, 0, 100},
		{0x0104, 2, 0x01},
		{0x3064, 2, 0x805},
		{0x3178, 2, 0x0000},
		{0x3ED0, 2, 0x1B24},
		{0x3EDC, 2, 0xC3E4},
		{0x3EE8, 2, 0x0000},
		{0x3E00, 2, 0x0010},
		{0x3016, 2, 0x111},                                    // row_speed
		{0x0344, 2, 0x3D8},
		{0x0348, 2, 0xB57},
		{0x0346, 2, 0x34A},
		{0x034A, 2, 0x781},
		{0x3040, 2, 0x0041},
		{0x0400, 2, 0x0000},
		{0x0404, 2, 0x0010},
		{0x034C, 2, 0x800},
		{0x034E, 2, 0x450},
		{0x0342, 2, 0x0880},
		{0x0340, 2, 0x04C9},
		{0x0202, 2, 0x0010},
		{0x3014, 2, 0x03F2},
		{0x3010, 2, 0x009C},
		{0x3018, 2, 0x0000},
		{0x30D4, 2, 0x1080},
		{0x306E, 2, 0x90B0},
		{0x3070, 2, 0x0000},
		{0x31C6, 2, 0x8400},
		{0x3174, 2, 0x8000},
		{0x3E40, 2, 0xDC05},
		{0x3E42, 2, 0x6E22},
		{0x3E44, 2, 0xDC22},
		{0x3E46, 2, 0xFF00},
		{0x3ED4, 2, 0xF998},
		{0x3ED6, 2, 0x9789},
		{0x3EDE, 2, 0xE438},
		{0x3EE0, 2, 0xA43F},
		{0x301A, 2, 0x0010},                                   // Unlock data
															   // pedestal
															   // register
		{0x3064, 2, 0x0805},                                   // Disable
															   // embedded data
		{0x301E, 2, 0x00A8},                                   // Set data
															   // pedestal
		{0x0400, 2, 0x0002},
		{0x306E, 2, 0x90B0},
		{0x31AE, 2, 0x0304},
		{0x301A, 2, 0x001C},
		{0x0104, 2, 0x00},
		{0x0120, 2, 0x00},
		{0x305e, 2, 0x1840},                                   // 2x
		{0x0202, 2, 2450},                                     // 1/30s
		{10, 0, 0}
	};

	// Apparently there was another use of 'SensorConfigScript' where the script is
	// transmitted to the sensor during initialization (other sensors)
	// These scripts are not used in AR0330, AR0331		6/24/22 DAT
	UInt32 SensorConfigScript_30_bin[65][3] = {
		{0x0100, 2, 0x00},                                     // Mode Select =
															   // 0x0
		{100, 0, 100},
		{0x0300, 2, 0x3},                                      // vt_pix_clk_div
															   // = 0x3
		{0x0302, 2, 0x1},                                      // vt_sys_clk_div
															   // = 0x1
		{0x0304, 2, 0x2},                                      // pre_pll_clk_div 
															   // = 0x2
		{0x0306, 2, 96},                                       // pll_multiplier
															   // = 0x60
		{0x0308, 2, 0x0C},                                     // op_pix_clk_div
															   // = 0xC
		{0x030A, 2, 0x01},                                     // op_sys_clk_div
															   // = 0x1
		{100, 0, 100},
		{0x0104, 2, 0x01},                                     // Grouped
															   // Parameter Hold
															   // = 0x1
		{0x3064, 2, 0x805},                                    // RESERVED_MFR_3064 
															   // = 0x805
		{0x3016, 2, 0x121},                                    // Row Speed =
															   // 0x111
		{0x0344, 2, 0x012},                                    // Column Start =
															   // 0x18
		{0x0348, 2, 0xF13},                                    // Column End =
															   // 0xF10
		{0x0346, 2, 0x8},                                      // Row Start =
															   // 0x12E
		{0x034A, 2, 0x879},                                    // Row End = 0x99B
		{0x3040, 2, 0x28C3},                                   // Read Mode =
															   // 0xC3
		{0x0400, 2, 0x0002},                                   // Scaling Mode =
															   // 0x0
		{0x0404, 2, 0x0010},                                   // Scale_M = 0x10
		{0x034C, 2, 0x782},                                    // Output Width =
															   // 0x780
		{0x034E, 2, 0x43A},                                    // Output Height = 
															   // 0x438
		{0x0342, 2, 0x0880},                                   // Line Length =
															   // 0x884
		{0x0340, 2, 0x0992},                                   // Frame Lines =
															   // 0x4C7
		{0x0202, 2, 1212},                                     // Coarse
															   // Integration
															   // Time (OPTIONAL) 
															   // = 0x10
		{0x3014, 2, 0x3F2},                                    // Fine
															   // Integration
															   // Time = 0x3F2
		{0x3010, 2, 0x009C},                                   // Fine Correction 
															   // = 0x9C
		{0x3018, 2, 0x0000},                                   // Extra Delay =
															   // 0x0
		{0x30D4, 2, 0xB080},                                   // Cols Dbl
															   // Sampling =
															   // 0x1080
		{0x306E, 2, 0x90B0},                                   // Scalar
															   // Re-sampling =
															   // 0x90B0
		{0x3E00, 2, 0x0010},                                   // Low Power
															   // On_Off = 0x10
		{0x3070, 2, 0x0000},                                   // 0:Normal
															   // operation:
															   // Generate output 
															   // data from pixel 
															   // array
		{0x31C6, 2, 0x8400},                                   // 01: Packetized
															   // SP
		{0x3E00, 2, 0x0010},                                   // Low Power
															   // On_Off = 0x10
		{0x3EDC, 2, 0xD8E4},                                   // Summing
															   // Procedure 3 =
															   // 0xC3E4
		{0x3178, 2, 0x0070},
		{0x3170, 2, 0x00E5},
		{0x3ECC, 2, 0x0FE4},
		{0x316C, 2, 0x0429},
		{0x3174, 2, 0x8000},
		{0x3E40, 2, 0xDC05},
		{0x3E42, 2, 0x6E22},
		{0x3E44, 2, 0xDC22},
		{0x3E46, 2, 0xFF00},
		{0x3ED4, 2, 0xF998},
		{0x3ED6, 2, 0x9789},
		{0x3EDE, 2, 0xE438},
		{0x3EE0, 2, 0xA43F},
		{0x3EE2, 2, 0xA4BF},
		{0x3EEC, 2, 0x1C21},
		{0x3056, 2, 0x1040},
		{0x3058, 2, 0x1040},
		{0x305A, 2, 0x1040},
		{0x305C, 2, 0x1040},
		{0x301A, 2, 0x0010},                                   // Unlock data
															   // pedestal
															   // register
		{0x3064, 2, 0x0805},                                   // Disable
															   // embedded data
		{0x301E, 2, 0x00A8},                                   // Set data
															   // pedestal
		{0x0400, 2, 0x0002},
		{0x306E, 2, 0x90B0},
		{0x31AE, 2, 0x0304},
		{0x301A, 2, 0x001C},
		{0x0104, 2, 0x00},
		{0x0120, 2, 0x00},
		{0x305e, 2, 0x1040},                                   // 1x
		{0x0202, 2, 2450},                                     // 1/30s
		{10, 0, 0}
	};

	// Apparently there was another use of 'SensorConfigScript' where the script is
	// transmitted to the sensor during initialization (other sensors)
	// These scripts are not used in AR0330, AR0331		6/24/22 DAT
	UInt32 SensorConfigScript_60_bin[65][3] = {
		{0x0100, 2, 0x00},                                     // Mode Select =
															   // 0x0
		{100, 0, 100},
		{0x0300, 2, 0x3},                                      // vt_pix_clk_div
															   // = 0x3
		{0x0302, 2, 0x1},                                      // vt_sys_clk_div
															   // = 0x1
		{0x0304, 2, 0x2},                                      // pre_pll_clk_div 
															   // = 0x2
		{0x0306, 2, 96},                                       // pll_multiplier
															   // = 0x60
		{0x0308, 2, 0x0C},                                     // op_pix_clk_div
															   // = 0xC
		{0x030A, 2, 0x01},                                     // op_sys_clk_div
															   // = 0x1
		{100, 0, 100},
		{0x0104, 2, 0x01},                                     // Grouped
															   // Parameter Hold
															   // = 0x1
		{0x3064, 2, 0x805},                                    // RESERVED_MFR_3064 
															   // = 0x805
		{0x3016, 2, 0x121},                                    // Row Speed =
															   // 0x121	(289)		6/24/22 DAT
		{0x0344, 2, 0x012},                                    // Column Start =
															   // 18				6/24/22 DAT
		{0x0348, 2, 0xF13},                                    // Column End =
															   // 0xF13 (3859)			6/24/22 DAT
		{0x0346, 2, 0x8},                                      // Row Start =
															   // 0x8 (8) 			6/24/22 DAT
		{0x034A, 2, 0x879},                                    // Row End = 0x879 (2169)			6/24/22 DAT
		{0x3040, 2, 0x28C3},                                   // Read Mode =
															   // 0xC3
		{0x0400, 2, 0x0002},                                   // Scaling Mode =
															   // 0x0
		{0x0404, 2, 0x0010},                                   // Scale_M = 0x10
		{0x034C, 2, 0x782},                                    // Output Width =
															   // 0x782	(1922)		6/24/22 DAT
		{0x034E, 2, 0x43A},                                    // Output Height = 
															   // 0x43A (1082)			6/24/22 DAT
		{0x0342, 2, 0x0881},                                   // Line Length =
															   // 0x881 (2177)			6/24/22 DAT
		{0x0340, 2, 0x04c9},                                   // Frame Lines =
															   // 0x4C9 (1225)			6/24/22 DAT
		{0x0202, 2, 1212},                                     // Coarse
															   // Integration
															   // Time (OPTIONAL) 
															   // = 0x10
		{0x3014, 2, 0x0603},                                   // Fine
															   // Integration
															   // Time = 0x3F2
		{0x3010, 2, 0x009C},                                   // Fine Correction 
															   // = 0x9C
		{0x3018, 2, 0x0000},                                   // Extra Delay =
															   // 0x0
		{0x30D4, 2, 0xB080},                                   // Cols Dbl
															   // Sampling =
															   // 0x1080
		{0x306E, 2, 0x90B0},                                   // Scalar
															   // Re-sampling =
															   // 0x90B0
		{0x3E00, 2, 0x0010},                                   // Low Power
															   // On_Off = 0x10
		{0x3070, 2, 0x0000},                                   // 0:Normal
															   // operation:
															   // Generate output 
															   // data from pixel 
															   // array
		{0x31C6, 2, 0x8400},                                   // 01: Packetized
															   // SP
		{0x3E00, 2, 0x0010},                                   // Low Power
															   // On_Off = 0x10
		{0x3EDC, 2, 0xD8E4},                                   // Summing
															   // Procedure 3 =
															   // 0xC3E4
		{0x3178, 2, 0x0070},
		{0x3170, 2, 0x00E5},
		{0x3ECC, 2, 0x0FE4},
		{0x316C, 2, 0x0429},
		{0x3174, 2, 0x8000},
		{0x3E40, 2, 0xDC05},
		{0x3E42, 2, 0x6E22},
		{0x3E44, 2, 0xDC22},
		{0x3E46, 2, 0xFF00},
		{0x3ED4, 2, 0xF998},
		{0x3ED6, 2, 0x9789},
		{0x3EDE, 2, 0xE438},
		{0x3EE0, 2, 0xA43F},
		{0x3EE2, 2, 0xA4BF},
		{0x3EEC, 2, 0x1C21},
		{0x3056, 2, 0x1040},
		{0x3058, 2, 0x1040},
		{0x305A, 2, 0x1040},
		{0x305C, 2, 0x1040},
		{0x301A, 2, 0x0010},                                   // Unlock data
															   // pedestal
															   // register
		{0x3064, 2, 0x0805},                                   // Disable
															   // embedded data
		{0x301E, 2, 0x00A8},                                   // Set data
															   // pedestal
		{0x0400, 2, 0x0002},
		{0x306E, 2, 0x90B0},
		{0x31AE, 2, 0x0304},
		{0x301A, 2, 0x001C},
		{0x0104, 2, 0x00},
		{0x0120, 2, 0x00},
		{0x305e, 2, 0x1040},                                   // 1x
		{0x0202, 2, 1175},                                     // 1/60s
		{10, 0, 0}
	};

	// Apparently there was another use of 'SensorConfigScript' where the script is
	// transmitted to the sensor during initialization (other sensors)
	// These scripts are not used in AR0330, AR0331		6/24/22 DAT
	UInt32 SensorConfigScript_60[57][3] = {
		{0x0100, 2, 0x00},                                     // Mode Select =
															   // 0x0
		{100, 0, 100},
		{0x0300, 2, 0x3},                                      // vt_pix_clk_div
		{0x0302, 2, 0x1},                                      // vt_sys_clk_div
		{0x0304, 2, 0x1},                                      // pre_pll_clk_div
		{0x0306, 2, 0x30},                                     // pll_multiplier
		{0x0308, 2, 0x0C},                                     // op_pix_clk_div
		{0x030A, 2, 0x01},                                     // op_sys_clk_div

		{100, 0, 100},
		{0x0104, 2, 0x01},
		{0x3064, 2, 0x805},
		{0x3178, 2, 0x0000},
		{0x3ED0, 2, 0x1B24},
		{0x3EDC, 2, 0xC3E4},
		{0x3EE8, 2, 0x0000},
		{0x3E00, 2, 0x0010},
		{0x3016, 2, 0x111},                                    // row_speed
		{0x0344, 2, 0x3D8},
		{0x0348, 2, 0xB57},
		{0x0346, 2, 0x34A},
		{0x034A, 2, 0x781},
		{0x3040, 2, 0x0041},
		{0x0400, 2, 0x0000},
		{0x0404, 2, 0x0010},
		{0x034C, 2, 0x800},
		{0x034E, 2, 0x450},
		{0x0342, 2, 0x0880},
		{0x0340, 2, 0x04C9},
		{0x0202, 2, 0x0010},
		{0x3014, 2, 0x03F2},
		{0x3010, 2, 0x009C},
		{0x3018, 2, 0x0000},
		{0x30D4, 2, 0x1080},
		{0x306E, 2, 0x90B0},
		{0x3070, 2, 0x0000},
		{0x31C6, 2, 0x8400},
		{0x3174, 2, 0x8000},
		{0x3E40, 2, 0xDC05},
		{0x3E42, 2, 0x6E22},
		{0x3E44, 2, 0xDC22},
		{0x3E46, 2, 0xFF00},
		{0x3ED4, 2, 0xF998},
		{0x3ED6, 2, 0x9789},
		{0x3EDE, 2, 0xE438},
		{0x3EE0, 2, 0xA43F},
		{0x301A, 2, 0x0010},                                   // Unlock data
															   // pedestal
															   // register
		{0x3064, 2, 0x0805},                                   // Disable
															   // embedded data
		{0x301E, 2, 0x00A8},                                   // Set data
															   // pedestal
		{0x0400, 2, 0x0002},
		{0x306E, 2, 0x90B0},
		{0x31AE, 2, 0x0304},
		{0x301A, 2, 0x001C},
		{0x0104, 2, 0x00},
		{0x0120, 2, 0x00},
		{0x305e, 2, 0x1840},                                   // 2x
		{0x0202, 2, 2450},                                     // 1/30s
		{10, 0, 0}
	};
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
