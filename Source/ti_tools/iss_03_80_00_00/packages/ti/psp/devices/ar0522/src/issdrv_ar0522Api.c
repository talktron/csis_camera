/** ==================================================================
 *  @file   issdrv_ar0522Api.c
 *
 *  @path   /ti/psp/devices/ar0522/src/
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

#include <ti/psp/devices/ar0522/src/issdrv_ar0522Priv.h>
#include <ti/psp/devices/ar0522/issdrv_ar0522_config.h>
#include <ti/psp/platforms/iss_platform.h>
#include <ti/psp/iss/drivers/alg/2A/inc/issdrv_alg2APriv.h>
#define LONG_EXPO                       0x3012
#define LONG_GAIN                       0x305e

/* Global object storing all information related to all AR0522 driver handles
 */
static Iss_Ar0522CommonObj gIss_Ar0522CommonObj;

static I2c_Ctrl gAr0522_I2c_ctrl;

extern ti2a_output ti2a_output_params;


/**/
#define DEF_1080P60FPS
//#define DEF_1080P30FPS
//#define DEF_3MP30FPS

/*comment out if disable WDR mode*/
//60fps only works in linear mode
//#define IMGS_MICRON_AR0522_WDR

#ifdef WDR_ON
#ifndef IMGS_MICRON_AR0522_WDR
#define IMGS_MICRON_AR0522_WDR
#endif
#define WDR_COMP_MODE 1
#endif

#ifdef DEF_1080P60FPS
	//60fps
//#define DEF_HARDWARE_MOD_DONE
#ifdef DEF_HARDWARE_MOD_DONE
	#define PLL_MULTIPLIER (0x4A)  //0x4A for 60fps
#else
	#define PLL_MULTIPLIER (0x3A)  //0x4A for 60fps
#endif
	#define VT_SYS_CLK_DIV	(1)
	#define OP_SYS_CLK_DIV	(1)
#endif

#ifdef DEF_1080P30FPS
	//30fps
	#define PLL_MULTIPLIER (0x4A)
	#define VT_SYS_CLK_DIV	(2)
	#define OP_SYS_CLK_DIV	(2)
#endif

#ifdef DEF_3MP30FPS
	//30fps
	#define PLL_MULTIPLIER (0x5A)
	#define VT_SYS_CLK_DIV	(2)
	#define OP_SYS_CLK_DIV	(2)
#endif

#if 0
#define PLL_P1       1                                     // vt_sys_clk_div
#define OP_SYS_CLK_DIV 1                                   // OP_SYS_CLK_DIV

#define INPUT_CLK    24                                    // EXTCLK
#define PLL_M        (74)                                 // pll_multiplier
#define PLL_pre_div  4                                     // pre_pll_clk_div

#define PLL_P2       6                                     // vt_pix_clk_div

//original calculation = 74 Mhz
//real clock (24m x 60 / 4x6)= 60 Mhz

#define OUT_CLK ((INPUT_CLK * PLL_M) / (PLL_pre_div * PLL_P1 * PLL_P2) )    // 74.25MHz
#endif
//#define USE_TI
/* ===================================================================
 *  @func     Iss_Ar0522Lock
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Int32 Iss_Ar0522Lock();

/* ===================================================================
 *  @func     Iss_Ar0522Unlock
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Int32 Iss_Ar0522Unlock();

/* ===================================================================
 *  @func     Transplant_DRV_imgsSetEshutter
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
int Transplant_DRV_imgsSetEshutter()
{

    int status;

    Uint16 regAddr;

    Uint16 regValue;

    // printf("current eshutterInUsec:%d-----\n",eshutterInUsec);
    Int32 devAddr = AR_0522_ADDR;

    regAddr = AR0522_COARSE_IT_TIME_A;

    status = Iss_Ar0522Lock();
    if (status == FVID2_SOK)
    {
        regValue = ti2a_output_params.sensorExposure / ROW_TIME;
        status =
            Iss_deviceWrite16(gAr0522_I2c_ctrl.i2cInstId, devAddr, &regAddr,
                              &regValue, 1);
    }
    Iss_Ar0522Unlock();

    if (status != FVID2_SOK)
    {
        Vps_printf(" ERROR %s:%s:%d !!!\n", __FILE__, __FUNCTION__, __LINE__);
        return status;
    }
    return status;
}

/* Control API that gets called when FVID2_control is called
 *
 * This API does handle level semaphore locking
 *
 * handle - AR0522 driver handle cmd - command cmdArgs - command arguments
 * cmdStatusArgs - command status
 *
 * returns error in case of - illegal parameters - I2C command RX/TX error */
/* ===================================================================
 *  @func     Iss_Ar0522Control
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Int32 Iss_Ar0522Control(Fdrv_Handle handle,
                        UInt32 cmd, Ptr cmdArgs, Ptr cmdStatusArgs)
{
    Iss_Ar0522Obj *pObj = (Iss_Ar0522Obj *) handle;

    Int32 status = FVID2_SOK;

    if (pObj == NULL)
        return FVID2_EBADARGS;

    /*
     * lock handle
     */
    Iss_Ar0522LockObj(pObj);

    switch (cmd)
    {
        case FVID2_START:
            // status = Iss_Ar0522Start ( pObj );
            break;

        case FVID2_STOP:
            // status = Iss_Ar0522Stop ( pObj );
            break;

        case IOCTL_ISS_SENSOR_GET_CHIP_ID:
            // status = Iss_Ar0522GetChipId ( pObj, cmdArgs, cmdStatusArgs );
            break;

        case IOCTL_ISS_SENSOR_RESET:
            // status = Iss_Ar0522Reset ( pObj );
            break;

        case IOCTL_ISS_SENSOR_REG_WRITE:
            // status = Iss_Ar0522RegWrite ( pObj, cmdArgs );
            break;

        case IOCTL_ISS_SENSOR_REG_READ:
            // status = Iss_Ar0522RegRead ( pObj, cmdArgs );
            break;

        case IOCTL_ISS_SENSOR_UPDATE_EXP_GAIN:
            status = Iss_Ar0522UpdateExpGain(pObj, cmdArgs);
//            Transplant_DRV_imgsSetEshutter();
	    status = FVID2_SOK;
            break;

        case IOCTL_ISS_SENSOR_UPDATE_FRAMERATE:
	{
	    Ptr createArgs;
	    Iss_CaptFrameRate *framerateParams = (Iss_CaptFrameRate *)cmdArgs;
	    createArgs = &(framerateParams->FrameRate);
    	    status = Iss_Ar0522FrameRateSet(pObj, createArgs, cmdStatusArgs);
            break;
	}

        case IOCTL_ISS_SENSOR_FRAME_RATE_SET:
            status = Iss_Ar0522FrameRateSet(pObj, cmdArgs, cmdStatusArgs);
            break;

	case IOCTL_ISS_SENSOR_UPDATE_ITT:
            status = Iss_Ar0522UpdateItt(pObj, cmdArgs);
            break;

        case IOCTL_ISS_SENSOR_PWM_CONFIG:
        {
        	Iss_SensorPwmParm *pPwmParm = (Iss_SensorPwmParm *)cmdArgs;
		if( pPwmParm->Id == ISS_SENSOR_PWM_VIDEO )
		{
			Iss_Ar0522PwmVideoSet(pPwmParm->period, pPwmParm->duty);
        	}
		else if( pPwmParm->Id == ISS_SENSOR_PWM_DC )
		{
			Iss_Ar0522PwmDcSet(pPwmParm->period, pPwmParm->duty);
        	}

        	break;
	}

	case IOCTL_ISS_SENSOR_SET_VIDEO_MODE:
	{
		Iss_SensorVideoModeParams *modePrms = (Iss_SensorVideoModeParams *)cmdArgs;
																
		if (NULL != modePrms)
		{
			status = Iss_Ar0522SetMode(pObj, (FVID2_Standard)modePrms->standard, AR_0522_ADDR, 1);
		}
		else
		{
			status = FVID2_EBADARGS;
		}

		if (FVID2_SOK == status)
			pObj->createArgs.InputStandard = modePrms->standard;

		break;
	}

        default:
            status = FVID2_EUNSUPPORTED_CMD;
            break;

    }

    /*
     * unlock handle
     */
    Iss_Ar0522UnlockObj(pObj);

    return status;
}

/*
 * Create API that gets called when FVID2_create is called
 *
 * This API does not configure the AR0522is any way.
 *
 * This API - validates parameters - allocates driver handle - stores create
 * arguments in its internal data structure.
 *
 * Later the create arguments will be used when doing I2C communcation with
 * AR0522
 *
 * drvId - driver ID, must be FVID2_ISS_VID_DEC_AR0522_DRV instanceId - must
 * be 0 createArgs - create arguments createStatusArgs - create status
 * fdmCbParams - NOT USED, set to NULL
 *
 * returns NULL in case of any error */
/* ===================================================================
 *  @func     Iss_Ar0522Create
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Fdrv_Handle Iss_Ar0522Create(UInt32 drvId,
                             UInt32 instanceId,
                             Ptr createArgs,
                             Ptr createStatusArgs,
                             const FVID2_DrvCbParams * fdmCbParams)
{
    Iss_Ar0522Obj *pObj;

    Iss_SensorCreateParams *sensorCreateArgs
        = (Iss_SensorCreateParams *) createArgs;

    Iss_SensorCreateStatus *sensorCreateStatus
        = (Iss_SensorCreateStatus *) createStatusArgs;

    /*
     * check parameters
     */
    if (sensorCreateStatus == NULL)
        return NULL;

    sensorCreateStatus->retVal = FVID2_SOK;

    if (drvId != FVID2_ISS_SENSOR_AR0522_DRV
        || instanceId != 0 || sensorCreateArgs == NULL)
    {
        sensorCreateStatus->retVal = FVID2_EBADARGS;
        return NULL;
    }

    if (sensorCreateArgs->deviceI2cInstId >= ISS_DEVICE_I2C_INST_ID_MAX)
    {
        sensorCreateStatus->retVal = FVID2_EINVALID_PARAMS;
        return NULL;
    }

    /*
     * allocate driver handle
     */
    pObj = Iss_Ar0522AllocObj();
    if (pObj == NULL)
    {
        sensorCreateStatus->retVal = FVID2_EALLOC;
        return NULL;
    }

    Iss_Ar0522Lock();

    // Transplant_AR0522_HDR_Enable(0);
    Iss_Ar0522Unlock();
    /*
     * copy parameters to allocate driver handle
     */
    memcpy(&pObj->createArgs, sensorCreateArgs, sizeof(*sensorCreateArgs));

    Iss_Ar0522ResetRegCache(pObj);

    /*
     * return driver handle object pointer
     */
    return pObj;
}

/*
 * Delete function that is called when FVID2_delete is called
 *
 * This API - free's driver handle object
 *
 * handle - driver handle deleteArgs - NOT USED, set to NULL
 *
 */
/* ===================================================================
 *  @func     Iss_Ar0522Delete
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Int32 Iss_Ar0522Delete(Fdrv_Handle handle, Ptr deleteArgs)
{
    Iss_Ar0522Obj *pObj = (Iss_Ar0522Obj *) handle;

    if (pObj == NULL)
        return FVID2_EBADARGS;

    /*
     * free driver handle object
     */
    Iss_Ar0522FreeObj(pObj);

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     Transplant_DRV_imgsSetRegs
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
int Transplant_DRV_imgsSetRegs()
{

    int status;
    int i = 0;
    int totalI = 0;

    int j;

    gAr0522_I2c_ctrl.i2cInstId = Iss_platformGetI2cInstId();
    Int32 devAddr = AR_0522_ADDR;

    int delay = 100000;

    Vps_printf("<<<<<<<<< %s:%s:%d >>>>>>>>>\n", __FILE__, __FUNCTION__, __LINE__);	// 7/11/22 DAT

    Uint16 *regAddr = gAr0522_I2c_ctrl.regAddr;
    Uint16 *regValue = gAr0522_I2c_ctrl.regValue;
    gAr0522_I2c_ctrl.numRegs = 1;

    ///////////////////////////////////////////////////////////////
    // Here we set the non-default ar0522 regs for initialization
    ///////////////////////////////////////////////////////////////

	/*
	[Native 1080p@60FPS 1928x1088 10 bit]
	LOAD= Reset
	LOAD=stream_1080p_60fps_10bit

	*/
	regAddr[0] = 0x0103; regValue[0] = 0x0001;  	// SOFTWARE_RESET (alias 0X301A B[0])
        status = Iss_deviceWrite16(gAr0522_I2c_ctrl.i2cInstId, devAddr,
                 &gAr0522_I2c_ctrl.regAddr[0],
                 &gAr0522_I2c_ctrl.regValue[0],
                 gAr0522_I2c_ctrl.numRegs);

        if (status != FVID2_SOK)
        {
	    Vps_printf("<<<<<<< Line %d: Error %d writing AR0522 (0x%02X) reg 0x%04X=0x%04X i2c=%d >>>>>>>\n",
		 __LINE__,
		 status,
		devAddr,
                 gAr0522_I2c_ctrl.regAddr[0],
                 gAr0522_I2c_ctrl.regValue[0],
                 gAr0522_I2c_ctrl.i2cInstId );		// 9/5/22 DAT

//		return FVID2_SOK;			// Abort 9/5/22 DAT
        }

	totalI++;	// 10/17/22 DAT

	/*
	[Hidden: stream_1080p_60fps_10bit]
	LOAD=powerup
	LOAD=corrections_recommended
	LOAD=pixel_timing_recommended
	LOAD=analog_setup_recommended_12bit
	LOAD=analog_setup_recommended_10bit
	LOAD=mipi_timing_recommended_10bit
	LOAD=pll_setup_max_10bit
	LOAD=1080p60fps_array_setup_1928_1088_skip1
	LOAD=Data_pedestal_12bit
	LOAD=new RNC 10bit
	REG=0x300C, 0xBE0 //line_length_pck
	REG=0x300A, 0x8DC //frame_length_lines
	LOAD=enable_streaming
	*/

//        Vps_printf("<<<<<<< Line %d: do CAM_RESET... >>>>>>>\n", __LINE__ );
//	Iss_Ar0522Reset(NULL);
//        Vps_printf("<<<<<<< Line %d: CAM_RESET done >>>>>>>\n", __LINE__ );


#if 1
	Vps_printf("<<<<<<< Line %d: 'corrections_recommended_mono'>>>>>>>\n", __LINE__ );

	/*
	[Hidden: corrections_recommended_mono]
	REG=0x3042, 0x0004     // dark_control2
	REG=0x3044, 0x4580     // RESERVED
	REG=0x30EE, 0x1136     // RESERVED
	REG=0x3120, 0x0001     // RESERVED
	REG=0x3F2C, 0x442E     // RESERVED
	REG=0x30D2, 0x0000     // RESERVED
	REG=0x30D4, 0x0000     // RESERVED
	REG=0x30D6, 0x2FFF     // RESERVED
	REG=0x30DA, 0x0FFF     // RESERVED
	REG=0x30DC, 0x0FFF     // RESERVED
	REG=0x30DE, 0x0000     // RESERVED
	REG=0x31E0, 0x0781     // RESERVED
	REG=0x3180, 0x9434     // fine_dig_correction_control
	REG=0x3172, 0x0206     // analog_control2
	REG=0x3F00, 0x0017     // RESERVED
	REG=0x3F02, 0x02DD     // RESERVED
	REG=0x3F04, 0x0020     // RESERVED
	REG=0x3F06, 0x0040     // RESERVED
	REG=0x3F08, 0x0070     // RESERVED
	REG=0x3F0A, 0x0101     // RESERVED
	REG=0x3F0C, 0x0302     // RESERVED
	REG=0x3F1E, 0x0022     // RESERVED
	REG=0x3F1A, 0x0103     // RESERVED
	REG=0x3F14, 0x0505     // RESERVED
	REG=0x3F44, 0x1515     // RESERVED
	REG=0x3F18, 0x0103     // RESERVED
	REG=0x3F12, 0x0505     // RESERVED
	REG=0x3F42, 0x1515     // RESERVED
	REG=0x3F16, 0x0103     // RESERVED
	REG=0x3F10, 0x0505     // RESERVED
	REG=0x3F40, 0x1515     // RESERVED
	*/

	i = 0;
	regAddr[i] = 0x3042; regValue[i] = 0x0004; i++;  	// dark_control2
	regAddr[i] = 0x3044; regValue[i] = 0x4580; i++;  	// 
	regAddr[i] = 0x30EE; regValue[i] = 0x1136; i++;  	// 
	regAddr[i] = 0x3120; regValue[i] = 0x0001; i++;  	// 
	regAddr[i] = 0x3F2C; regValue[i] = 0x442E; i++;  	// 
	regAddr[i] = 0x30D2; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x30D4; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x30D6; regValue[i] = 0x2FFF; i++;  	// 
	regAddr[i] = 0x30DA; regValue[i] = 0x0FFF; i++;  	// 
	regAddr[i] = 0x30DC; regValue[i] = 0x0FFF; i++;  	// 
	regAddr[i] = 0x30DE; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x31E0; regValue[i] = 0x0781; i++;  	// 
	regAddr[i] = 0x3180; regValue[i] = 0x9434; i++;  	// fine_dig_correction_control
	regAddr[i] = 0x3172; regValue[i] = 0x0206; i++;  	// analog_control2
	regAddr[i] = 0x3F00; regValue[i] = 0x0017; i++;  	// 
	regAddr[i] = 0x3F02; regValue[i] = 0x02DD; i++;  	// 
	regAddr[i] = 0x3F04; regValue[i] = 0x0020; i++;  	// 
	regAddr[i] = 0x3F06; regValue[i] = 0x0040; i++;  	// 
	regAddr[i] = 0x3F08; regValue[i] = 0x0070; i++;  	// 
	regAddr[i] = 0x3F0A; regValue[i] = 0x0101; i++;  	// 
	regAddr[i] = 0x3F0C; regValue[i] = 0x0302; i++;  	// 
	regAddr[i] = 0x3F1E; regValue[i] = 0x0022; i++;  	// 
	regAddr[i] = 0x3F1A; regValue[i] = 0x0103; i++;  	// 
	regAddr[i] = 0x3F14; regValue[i] = 0x0505; i++;  	// 
	regAddr[i] = 0x3F44; regValue[i] = 0x1515; i++;  	// 
	regAddr[i] = 0x3F18; regValue[i] = 0x0103; i++;  	// 
	regAddr[i] = 0x3F12; regValue[i] = 0x0505; i++;  	// 
	regAddr[i] = 0x3F42; regValue[i] = 0x1515; i++;  	// 
	regAddr[i] = 0x3F16; regValue[i] = 0x01FF; i++;  	// 
	regAddr[i] = 0x3F10; regValue[i] = 0x0505; i++;  	// 
	regAddr[i] = 0x3F40; regValue[i] = 0x1511; i++;  	// 

    for (j = 0; j < i; j++)
    {
        status = Iss_deviceWrite16(gAr0522_I2c_ctrl.i2cInstId, devAddr,
                 &gAr0522_I2c_ctrl.regAddr[j],
                 &gAr0522_I2c_ctrl.regValue[j],
                 gAr0522_I2c_ctrl.numRegs);

	totalI++;	// 10/17/22 DAT

        if (status != FVID2_SOK)
        {
	    Vps_printf("<<<<<<< Line %d: Err writing AR0522 (0x%02X) reg 0x%04X=0x%04X\n",
		 __LINE__,
		 status,
		devAddr,
                 gAr0522_I2c_ctrl.regAddr[j],
                 gAr0522_I2c_ctrl.regValue[j]);		// 6/28/22 DAT
        }

        if (gAr0522_I2c_ctrl.regAddr[j] == 0x301A)
        {
#ifdef IMGS_MICRON_AR0522_WDR
            delay = 500000;
#else
            delay = 100000;
#endif
            while (delay--)
            {
            }

        }
        else
        {
            delay = 1000;
            while (delay--)
            {
            }

        }

    }
#endif

#if 1
	Vps_printf("<<<<<<< Line %d: 'pixel_timing_recommended'>>>>>>>\n", __LINE__ );
	/*
	[Hidden: pixel_timing_recommended]
	REG=0x3D00, 0x043E    // RESERVED
	REG=0x3D02, 0x4760    // RESERVED
	REG=0x3D04, 0xFFFF    // RESERVED
	REG=0x3D06, 0xFFFF    // RESERVED
	REG=0x3D08, 0x8000    // RESERVED
	REG=0x3D0A, 0x0510    // RESERVED
	REG=0x3D0C, 0xAF08    // RESERVED
	REG=0x3D0E, 0x0252    // RESERVED
	REG=0x3D10, 0x486F    // RESERVED
	REG=0x3D12, 0x5D5D    // RESERVED
	REG=0x3D14, 0x8056    // RESERVED
	REG=0x3D16, 0x8313    // RESERVED
	REG=0x3D18, 0x0087    // RESERVED
	REG=0x3D1A, 0x6A48    // RESERVED
	REG=0x3D1C, 0x6982    // RESERVED
	REG=0x3D1E, 0x0280    // RESERVED
	REG=0x3D20, 0x8359    // RESERVED
	REG=0x3D22, 0x8D02    // RESERVED
	REG=0x3D24, 0x8020    // RESERVED
	REG=0x3D26, 0x4882    // RESERVED
	REG=0x3D28, 0x4269    // RESERVED
	REG=0x3D2A, 0x6A95    // RESERVED
	REG=0x3D2C, 0x5988    // RESERVED
	REG=0x3D2E, 0x5A83    // RESERVED
	REG=0x3D30, 0x5885    // RESERVED
	REG=0x3D32, 0x6280    // RESERVED
	REG=0x3D34, 0x6289    // RESERVED
	REG=0x3D36, 0x6097    // RESERVED
	REG=0x3D38, 0x5782    // RESERVED
	REG=0x3D3A, 0x605C    // RESERVED
	REG=0x3D3C, 0xBF18    // RESERVED
	REG=0x3D3E, 0x0961    // RESERVED
	REG=0x3D40, 0x5080    // RESERVED
	REG=0x3D42, 0x2090    // RESERVED
	REG=0x3D44, 0x4390    // RESERVED
	REG=0x3D46, 0x4382    // RESERVED
	REG=0x3D48, 0x5F8A    // RESERVED
	REG=0x3D4A, 0x5D5D    // RESERVED
	REG=0x3D4C, 0x9C63    // RESERVED
	REG=0x3D4E, 0x8063    // RESERVED
	REG=0x3D50, 0xA960    // RESERVED
	REG=0x3D52, 0x9757    // RESERVED
	REG=0x3D54, 0x8260    // RESERVED
	REG=0x3D56, 0x5CFF    // RESERVED
	REG=0x3D58, 0xBF10    // RESERVED
	REG=0x3D5A, 0x1681    // RESERVED
	REG=0x3D5C, 0x0802    // RESERVED
	REG=0x3D5E, 0x8000    // RESERVED
	REG=0x3D60, 0x141C    // RESERVED
	REG=0x3D62, 0x6000    // RESERVED
	REG=0x3D64, 0x6022    // RESERVED
	REG=0x3D66, 0x4D80    // RESERVED
	REG=0x3D68, 0x5C97    // RESERVED
	REG=0x3D6A, 0x6A69    // RESERVED
	REG=0x3D6C, 0xAC6F    // RESERVED
	REG=0x3D6E, 0x4645    // RESERVED
	REG=0x3D70, 0x4400    // RESERVED
	REG=0x3D72, 0x0513    // RESERVED
	REG=0x3D74, 0x8069    // RESERVED
	REG=0x3D76, 0x6AC6    // RESERVED
	REG=0x3D78, 0x5F95    // RESERVED
	REG=0x3D7A, 0x5F70    // RESERVED
	REG=0x3D7C, 0x8040    // RESERVED
	REG=0x3D7E, 0x4A81    // RESERVED
	REG=0x3D80, 0x0300    // RESERVED
	REG=0x3D82, 0xE703    // RESERVED
	REG=0x3D84, 0x0088    // RESERVED
	REG=0x3D86, 0x4A83    // RESERVED
	REG=0x3D88, 0x40FF    // RESERVED
	REG=0x3D8A, 0xFFFF    // RESERVED
	REG=0x3D8C, 0xFD70    // RESERVED
	REG=0x3D8E, 0x8040    // RESERVED
	REG=0x3D90, 0x4A85    // RESERVED
	REG=0x3D92, 0x4FA8    // RESERVED
	REG=0x3D94, 0x4F8C    // RESERVED
	REG=0x3D96, 0x0070    // RESERVED
	REG=0x3D98, 0xBE47    // RESERVED
	REG=0x3D9A, 0x8847    // RESERVED
	REG=0x3D9C, 0xBC78    // RESERVED
	REG=0x3D9E, 0x6B89    // RESERVED
	REG=0x3DA0, 0x6A80    // RESERVED
	REG=0x3DA2, 0x6986    // RESERVED
	REG=0x3DA4, 0x6B8E    // RESERVED
	REG=0x3DA6, 0x6B80    // RESERVED
	REG=0x3DA8, 0x6980    // RESERVED
	REG=0x3DAA, 0x6A88    // RESERVED
	REG=0x3DAC, 0x7C9F    // RESERVED
	REG=0x3DAE, 0x866B    // RESERVED
	REG=0x3DB0, 0x8765    // RESERVED
	REG=0x3DB2, 0x46FF    // RESERVED
	REG=0x3DB4, 0xE365    // RESERVED
	REG=0x3DB6, 0xA679    // RESERVED
	REG=0x3DB8, 0x4A40    // RESERVED
	REG=0x3DBA, 0x4580    // RESERVED
	REG=0x3DBC, 0x44BC    // RESERVED
	REG=0x3DBE, 0x7000    // RESERVED
	REG=0x3DC0, 0x8040    // RESERVED
	REG=0x3DC2, 0x0802    // RESERVED
	REG=0x3DC4, 0x10EF    // RESERVED
	REG=0x3DC6, 0x0104    // RESERVED
	REG=0x3DC8, 0x3860    // RESERVED
	REG=0x3DCA, 0x5D5D    // RESERVED
	REG=0x3DCC, 0x5682    // RESERVED
	REG=0x3DCE, 0x1300    // RESERVED
	REG=0x3DD0, 0x8648    // RESERVED
	REG=0x3DD2, 0x8202    // RESERVED
	REG=0x3DD4, 0x8082    // RESERVED
	REG=0x3DD6, 0x598A    // RESERVED
	REG=0x3DD8, 0x0280    // RESERVED
	REG=0x3DDA, 0x2048    // RESERVED
	REG=0x3DDC, 0x3060    // RESERVED
	REG=0x3DDE, 0x8042    // RESERVED
	REG=0x3DE0, 0x9259    // RESERVED
	REG=0x3DE2, 0x865A    // RESERVED
	REG=0x3DE4, 0x8258    // RESERVED
	REG=0x3DE6, 0x8562    // RESERVED
	REG=0x3DE8, 0x8062    // RESERVED
	REG=0x3DEA, 0x8560    // RESERVED
	REG=0x3DEC, 0x9257    // RESERVED
	REG=0x3DEE, 0x8221    // RESERVED
	REG=0x3DF0, 0x10FF    // RESERVED
	REG=0x3DF2, 0xB757    // RESERVED
	REG=0x3DF4, 0x9361    // RESERVED
	REG=0x3DF6, 0x1019    // RESERVED
	REG=0x3DF8, 0x8020    // RESERVED
	REG=0x3DFA, 0x9043    // RESERVED
	REG=0x3DFC, 0x8E43    // RESERVED
	REG=0x3DFE, 0x845F    // RESERVED
	REG=0x3E00, 0x835D    // RESERVED
	REG=0x3E02, 0x805D    // RESERVED
	REG=0x3E04, 0x8163    // RESERVED
	REG=0x3E06, 0x8063    // RESERVED
	REG=0x3E08, 0xA060    // RESERVED
	REG=0x3E0A, 0x9157    // RESERVED
	REG=0x3E0C, 0x8260    // RESERVED
	REG=0x3E0E, 0x5CFF    // RESERVED
	REG=0x3E10, 0xFFFF    // RESERVED
	REG=0x3E12, 0xFFE5    // RESERVED
	REG=0x3E14, 0x1016    // RESERVED
	REG=0x3E16, 0x2048    // RESERVED
	REG=0x3E18, 0x0802    // RESERVED
	REG=0x3E1A, 0x1C60    // RESERVED
	REG=0x3E1C, 0x0014    // RESERVED
	REG=0x3E1E, 0x0060    // RESERVED
	REG=0x3E20, 0x2205    // RESERVED
	REG=0x3E22, 0x8120    // RESERVED
	REG=0x3E24, 0x908F    // RESERVED
	REG=0x3E26, 0x6A80    // RESERVED
	REG=0x3E28, 0x6982    // RESERVED
	REG=0x3E2A, 0x5F9F    // RESERVED
	REG=0x3E2C, 0x6F46    // RESERVED
	REG=0x3E2E, 0x4544    // RESERVED
	REG=0x3E30, 0x0005    // RESERVED
	REG=0x3E32, 0x8013    // RESERVED
	REG=0x3E34, 0x8069    // RESERVED
	REG=0x3E36, 0x6A80    // RESERVED
	REG=0x3E38, 0x7000    // RESERVED
	REG=0x3E3A, 0x0000    // RESERVED
	REG=0x3E3C, 0x0000    // RESERVED
	REG=0x3E3E, 0x0000    // RESERVED
	REG=0x3E40, 0x0000    // RESERVED
	REG=0x3E42, 0x0000    // RESERVED
	REG=0x3E44, 0x0000    // RESERVED
	REG=0x3E46, 0x0000    // RESERVED
	REG=0x3E48, 0x0000    // RESERVED
	REG=0x3E4A, 0x0000    // RESERVED
	REG=0x3E4C, 0x0000    // RESERVED
	REG=0x3E4E, 0x0000    // RESERVED
	REG=0x3E50, 0x0000    // RESERVED
	REG=0x3E52, 0x0000    // RESERVED
	REG=0x3E54, 0x0000    // RESERVED
	REG=0x3E56, 0x0000    // RESERVED
	REG=0x3E58, 0x0000    // RESERVED
	REG=0x3E5A, 0x0000    // RESERVED
	REG=0x3E5C, 0x0000    // RESERVED
	REG=0x3E5E, 0x0000    // RESERVED
	REG=0x3E60, 0x0000    // RESERVED
	REG=0x3E62, 0x0000    // RESERVED
	REG=0x3E64, 0x0000    // RESERVED
	REG=0x3E66, 0x0000    // RESERVED
	REG=0x3E68, 0x0000    // RESERVED
	REG=0x3E6A, 0x0000    // RESERVED
	REG=0x3E6C, 0x0000    // RESERVED
	REG=0x3E6E, 0x0000    // RESERVED
	REG=0x3E70, 0x0000    // RESERVED
	REG=0x3E72, 0x0000    // RESERVED
	REG=0x3E74, 0x0000    // RESERVED
	REG=0x3E76, 0x0000    // RESERVED
	REG=0x3E78, 0x0000    // RESERVED
	REG=0x3E7A, 0x0000    // RESERVED
	REG=0x3E7C, 0x0000    // RESERVED
	REG=0x3E7E, 0x0000    // RESERVED
	REG=0x3E80, 0x0000    // RESERVED
	REG=0x3E82, 0x0000    // RESERVED
	REG=0x3E84, 0x0000    // RESERVED
	REG=0x3E86, 0x0000    // RESERVED
	REG=0x3E88, 0x0000    // RESERVED
	REG=0x3E8A, 0x0000    // RESERVED
	REG=0x3E8C, 0x0000    // RESERVED
	REG=0x3E8E, 0x0000    // RESERVED
	REG=0x3E90, 0x0000    // RESERVED
	REG=0x3E92, 0x0000    // RESERVED
	REG=0x3E94, 0x0000    // RESERVED
	REG=0x3E96, 0x0000    // RESERVED
	REG=0x3E98, 0x0000    // RESERVED
	REG=0x3E9A, 0x0000    // RESERVED
	REG=0x3E9C, 0x0000    // RESERVED
	REG=0x3E9E, 0x0000    // RESERVED
	REG=0x3EA0, 0x0000    // RESERVED
	REG=0x3EA2, 0x0000    // RESERVED
	REG=0x3EA4, 0x0000    // RESERVED
	REG=0x3EA6, 0x0000    // RESERVED
	REG=0x3EA8, 0x0000    // RESERVED
	REG=0x3EAA, 0x0000    // RESERVED
	REG=0x3EAC, 0x0000    // RESERVED
	REG=0x3EAE, 0x0000    // RESERVED
	REG=0x3EB0, 0x0000    // RESERVED
	REG=0x3EB2, 0x0000    // RESERVED
	REG=0x3EB4, 0x0000    // RESERVED
	*/

	i = 0;
	regAddr[i] = 0x3D00; regValue[i] = 0x043E; i++;  	// 
	regAddr[i] = 0x3D02; regValue[i] = 0x4760; i++;  	// 
	regAddr[i] = 0x3D04; regValue[i] = 0xFFFF; i++;  	// 
	regAddr[i] = 0x3D06; regValue[i] = 0xFFFF; i++;  	// 
	regAddr[i] = 0x3D08; regValue[i] = 0x8000; i++;  	// 
	regAddr[i] = 0x3D0A; regValue[i] = 0x0510; i++;  	// 
	regAddr[i] = 0x3D0C; regValue[i] = 0xAF08; i++;  	// 
	regAddr[i] = 0x3D0E; regValue[i] = 0x0252; i++;  	// 

	regAddr[i] = 0x3D10; regValue[i] = 0x486F; i++;  	// 
	regAddr[i] = 0x3D12; regValue[i] = 0x5D5D; i++;  	// 
	regAddr[i] = 0x3D14; regValue[i] = 0x8056; i++;  	// 
	regAddr[i] = 0x3D16; regValue[i] = 0x8313; i++;  	// 
	regAddr[i] = 0x3D18; regValue[i] = 0x0087; i++;  	// 
	regAddr[i] = 0x3D1A; regValue[i] = 0x6A48; i++;  	// 
	regAddr[i] = 0x3D1C; regValue[i] = 0x6982; i++;  	// 
	regAddr[i] = 0x3D1E; regValue[i] = 0x0280; i++;  	// 

	regAddr[i] = 0x3D20; regValue[i] = 0x8359; i++;  	// 
	regAddr[i] = 0x3D22; regValue[i] = 0x8D02; i++;  	// 
	regAddr[i] = 0x3D24; regValue[i] = 0x8020; i++;  	// 
	regAddr[i] = 0x3D26; regValue[i] = 0x4882; i++;  	// 
	regAddr[i] = 0x3D28; regValue[i] = 0x4269; i++;  	// 
	regAddr[i] = 0x3D2A; regValue[i] = 0x6A95; i++;  	// 
	regAddr[i] = 0x3D2C; regValue[i] = 0x5988; i++;  	// 
	regAddr[i] = 0x3D2E; regValue[i] = 0x5A83; i++;  	// 

	regAddr[i] = 0x3D30; regValue[i] = 0x5885; i++;  	// 
	regAddr[i] = 0x3D32; regValue[i] = 0x6280; i++;  	// 
	regAddr[i] = 0x3D34; regValue[i] = 0x6289; i++;  	// 
	regAddr[i] = 0x3D36; regValue[i] = 0x6097; i++;  	// 
	regAddr[i] = 0x3D38; regValue[i] = 0x5782; i++;  	// 
	regAddr[i] = 0x3D3A; regValue[i] = 0x605C; i++;  	// 
	regAddr[i] = 0x3D3C; regValue[i] = 0xBF18; i++;  	// 
	regAddr[i] = 0x3D3E; regValue[i] = 0x0961; i++;  	// 

	regAddr[i] = 0x3D40; regValue[i] = 0x5080; i++;  	// 
	regAddr[i] = 0x3D42; regValue[i] = 0x2090; i++;  	// 
	regAddr[i] = 0x3D44; regValue[i] = 0x4390; i++;  	// 
	regAddr[i] = 0x3D46; regValue[i] = 0x4382; i++;  	// 
	regAddr[i] = 0x3D48; regValue[i] = 0x5F8A; i++;  	// 
	regAddr[i] = 0x3D4A; regValue[i] = 0x5D5D; i++;  	// 
	regAddr[i] = 0x3D4C; regValue[i] = 0x9C63; i++;  	// 
	regAddr[i] = 0x3D4E; regValue[i] = 0x8063; i++;  	// 

	regAddr[i] = 0x3D50; regValue[i] = 0xA960; i++;  	// 
	regAddr[i] = 0x3D52; regValue[i] = 0x9757; i++;  	// 
	regAddr[i] = 0x3D54; regValue[i] = 0x8260; i++;  	// 
	regAddr[i] = 0x3D56; regValue[i] = 0x5CFF; i++;  	// 
	regAddr[i] = 0x3D58; regValue[i] = 0xBF10; i++;  	// 
	regAddr[i] = 0x3D5A; regValue[i] = 0x1681; i++;  	// 
	regAddr[i] = 0x3D5C; regValue[i] = 0x0802; i++;  	// 
	regAddr[i] = 0x3D5E; regValue[i] = 0x8000; i++;  	// 

	regAddr[i] = 0x3D60; regValue[i] = 0x141C; i++;  	// 
	regAddr[i] = 0x3D62; regValue[i] = 0x6000; i++;  	// 
	regAddr[i] = 0x3D64; regValue[i] = 0x6022; i++;  	// 
	regAddr[i] = 0x3D66; regValue[i] = 0x4D80; i++;  	// 
	regAddr[i] = 0x3D68; regValue[i] = 0x5C97; i++;  	// 
	regAddr[i] = 0x3D6A; regValue[i] = 0x6A69; i++;  	// 
	regAddr[i] = 0x3D6C; regValue[i] = 0xAC6F; i++;  	// 
	regAddr[i] = 0x3D6E; regValue[i] = 0x4645; i++;  	// 

	regAddr[i] = 0x3D70; regValue[i] = 0x4400; i++;  	// 
	regAddr[i] = 0x3D72; regValue[i] = 0x0513; i++;  	// 
	regAddr[i] = 0x3D74; regValue[i] = 0x8069; i++;  	// 
	regAddr[i] = 0x3D76; regValue[i] = 0x6AC6; i++;  	// 
	regAddr[i] = 0x3D78; regValue[i] = 0x5F95; i++;  	// 
	regAddr[i] = 0x3D7A; regValue[i] = 0x5F70; i++;  	// 
	regAddr[i] = 0x3D7C; regValue[i] = 0x8040; i++;  	// 
	regAddr[i] = 0x3D7E; regValue[i] = 0x4A81; i++;  	// 

	regAddr[i] = 0x3D80; regValue[i] = 0x0300; i++;  	// 
	regAddr[i] = 0x3D82; regValue[i] = 0xE703; i++;  	// 
	regAddr[i] = 0x3D84; regValue[i] = 0x0088; i++;  	// 
	regAddr[i] = 0x3D86; regValue[i] = 0x4A83; i++;  	// 
	regAddr[i] = 0x3D88; regValue[i] = 0x40FF; i++;  	// 
	regAddr[i] = 0x3D8A; regValue[i] = 0xFFFF; i++;  	// 
	regAddr[i] = 0x3D8C; regValue[i] = 0xFD70; i++;  	// 
	regAddr[i] = 0x3D8E; regValue[i] = 0x8040; i++;  	// 

	regAddr[i] = 0x3D90; regValue[i] = 0x4A85; i++;  	// 
	regAddr[i] = 0x3D92; regValue[i] = 0x4FA8; i++;  	// 
	regAddr[i] = 0x3D94; regValue[i] = 0x4F8C; i++;  	// 
	regAddr[i] = 0x3D96; regValue[i] = 0x0070; i++;  	// 
	regAddr[i] = 0x3D98; regValue[i] = 0xBE47; i++;  	// 
	regAddr[i] = 0x3D9A; regValue[i] = 0x8847; i++;  	// 
	regAddr[i] = 0x3D9C; regValue[i] = 0xBC78; i++;  	// 
	regAddr[i] = 0x3D9E; regValue[i] = 0x6B89; i++;  	// 

	regAddr[i] = 0x3DA0; regValue[i] = 0x6A80; i++;  	// 
	regAddr[i] = 0x3DA2; regValue[i] = 0x6986; i++;  	// 
	regAddr[i] = 0x3DA4; regValue[i] = 0x6B8E; i++;  	// 
	regAddr[i] = 0x3DA6; regValue[i] = 0x6B80; i++;  	// 
	regAddr[i] = 0x3DA8; regValue[i] = 0x6980; i++;  	// 
	regAddr[i] = 0x3DAA; regValue[i] = 0x6A88; i++;  	// 
	regAddr[i] = 0x3DAC; regValue[i] = 0x7C9F; i++;  	// 
	regAddr[i] = 0x3DAE; regValue[i] = 0x866B; i++;  	// 

	regAddr[i] = 0x3DB0; regValue[i] = 0x8765; i++;  	// 
	regAddr[i] = 0x3DB2; regValue[i] = 0x46FF; i++;  	// 
	regAddr[i] = 0x3DB4; regValue[i] = 0xE365; i++;  	// 
	regAddr[i] = 0x3DB6; regValue[i] = 0xA679; i++;  	// 
	regAddr[i] = 0x3DB8; regValue[i] = 0x4A40; i++;  	// 
	regAddr[i] = 0x3DBA; regValue[i] = 0x4580; i++;  	// 
	regAddr[i] = 0x3DBC; regValue[i] = 0x44BC; i++;  	// 
	regAddr[i] = 0x3DBE; regValue[i] = 0x7000; i++;  	// 

	regAddr[i] = 0x3DC0; regValue[i] = 0x8040; i++;  	// 
	regAddr[i] = 0x3DC2; regValue[i] = 0x0802; i++;  	// 
	regAddr[i] = 0x3DC4; regValue[i] = 0x10EF; i++;  	// 
	regAddr[i] = 0x3DC6; regValue[i] = 0x0104; i++;  	// 
	regAddr[i] = 0x3DC8; regValue[i] = 0x3860; i++;  	// 
	regAddr[i] = 0x3DCA; regValue[i] = 0x5D5D; i++;  	// 
	regAddr[i] = 0x3DCC; regValue[i] = 0x5682; i++;  	// 
	regAddr[i] = 0x3DCE; regValue[i] = 0x1300; i++;  	// 

	regAddr[i] = 0x3DD0; regValue[i] = 0x8648; i++;  	// 
	regAddr[i] = 0x3DD2; regValue[i] = 0x8202; i++;  	// 
	regAddr[i] = 0x3DD4; regValue[i] = 0x8082; i++;  	// 
	regAddr[i] = 0x3DD6; regValue[i] = 0x598A; i++;  	// 
	regAddr[i] = 0x3DD8; regValue[i] = 0x0280; i++;  	// 
	regAddr[i] = 0x3DDA; regValue[i] = 0x2048; i++;  	// 
	regAddr[i] = 0x3DDC; regValue[i] = 0x3060; i++;  	// 
	regAddr[i] = 0x3D4E; regValue[i] = 0x8042; i++;  	// 

	regAddr[i] = 0x3DE0; regValue[i] = 0x9259; i++;  	// 
	regAddr[i] = 0x3DE2; regValue[i] = 0x865A; i++;  	// 
	regAddr[i] = 0x3DE4; regValue[i] = 0x8258; i++;  	// 
	regAddr[i] = 0x3DE6; regValue[i] = 0x8562; i++;  	// 
	regAddr[i] = 0x3DE8; regValue[i] = 0x8062; i++;  	// 
	regAddr[i] = 0x3DEA; regValue[i] = 0x8560; i++;  	// 
	regAddr[i] = 0x3DEC; regValue[i] = 0x9257; i++;  	// 
	regAddr[i] = 0x3DEE; regValue[i] = 0x8221; i++;  	// 

	regAddr[i] = 0x3DF0; regValue[i] = 0x10FF; i++;  	// 
	regAddr[i] = 0x3DF2; regValue[i] = 0xB757; i++;  	// 
	regAddr[i] = 0x3DF4; regValue[i] = 0x9361; i++;  	// 
	regAddr[i] = 0x3DF6; regValue[i] = 0x1019; i++;  	// 
	regAddr[i] = 0x3DF8; regValue[i] = 0x8020; i++;  	// 
	regAddr[i] = 0x3DFA; regValue[i] = 0x9043; i++;  	// 
	regAddr[i] = 0x3DFC; regValue[i] = 0x8E43; i++;  	// 
	regAddr[i] = 0x3DFE; regValue[i] = 0x845F; i++;  	// 

	regAddr[i] = 0x3E80; regValue[i] = 0x835D; i++;  	// 
	regAddr[i] = 0x3E82; regValue[i] = 0x805D; i++;  	// 
	regAddr[i] = 0x3E84; regValue[i] = 0x8163; i++;  	// 
	regAddr[i] = 0x3E86; regValue[i] = 0x8063; i++;  	// 
	regAddr[i] = 0x3E88; regValue[i] = 0xA060; i++;  	// 
	regAddr[i] = 0x3E8A; regValue[i] = 0x9157; i++;  	// 
	regAddr[i] = 0x3E8C; regValue[i] = 0x8260; i++;  	// 
	regAddr[i] = 0x3E8E; regValue[i] = 0x5CFF; i++;  	// 

	regAddr[i] = 0x3E10; regValue[i] = 0xFFFF; i++;  	// 
	regAddr[i] = 0x3E12; regValue[i] = 0xFFE5; i++;  	// 
	regAddr[i] = 0x3E14; regValue[i] = 0x1016; i++;  	// 
	regAddr[i] = 0x3E16; regValue[i] = 0x2048; i++;  	// 
	regAddr[i] = 0x3E18; regValue[i] = 0x0802; i++;  	// 
	regAddr[i] = 0x3E1A; regValue[i] = 0x1C60; i++;  	// 
	regAddr[i] = 0x3E1C; regValue[i] = 0x0014; i++;  	// 
	regAddr[i] = 0x3E1E; regValue[i] = 0x0060; i++;  	// 

	regAddr[i] = 0x3E20; regValue[i] = 0x2205; i++;  	// 
	regAddr[i] = 0x3E22; regValue[i] = 0x8120; i++;  	// 
	regAddr[i] = 0x3E24; regValue[i] = 0x908F; i++;  	// 
	regAddr[i] = 0x3E26; regValue[i] = 0x6A80; i++;  	// 
	regAddr[i] = 0x3E28; regValue[i] = 0x6982; i++;  	// 
	regAddr[i] = 0x3E2A; regValue[i] = 0x5F9F; i++;  	// 
	regAddr[i] = 0x3E2C; regValue[i] = 0x6F46; i++;  	// 
	regAddr[i] = 0x3E2E; regValue[i] = 0x4544; i++;  	// 

	regAddr[i] = 0x3E30; regValue[i] = 0x0005; i++;  	// 
	regAddr[i] = 0x3E32; regValue[i] = 0x8013; i++;  	// 
	regAddr[i] = 0x3E34; regValue[i] = 0x8069; i++;  	// 
	regAddr[i] = 0x3E36; regValue[i] = 0x6A80; i++;  	// 
	regAddr[i] = 0x3E38; regValue[i] = 0x7000; i++;  	// 
	regAddr[i] = 0x3E3A; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E3C; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E3E; regValue[i] = 0x0000; i++;  	// 

	regAddr[i] = 0x3E40; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E42; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E44; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E46; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E48; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E4A; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E4C; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E4E; regValue[i] = 0x0000; i++;  	// 

	regAddr[i] = 0x3E50; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E52; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E54; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E56; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E58; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E5A; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E5C; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E5E; regValue[i] = 0x0000; i++;  	// 

	regAddr[i] = 0x3E60; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E62; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E64; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E66; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E68; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E6A; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E6C; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E6E; regValue[i] = 0x0000; i++;  	// 

	regAddr[i] = 0x3E70; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E72; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E74; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E76; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E78; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E7A; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E7C; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E7E; regValue[i] = 0x0000; i++;  	// 

	regAddr[i] = 0x3E80; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E82; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E84; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E86; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E88; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E8A; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E8C; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E8E; regValue[i] = 0x0000; i++;  	// 

	regAddr[i] = 0x3E90; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E92; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E94; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E96; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E98; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E9A; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E9C; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3E9E; regValue[i] = 0x0000; i++;  	// 

	regAddr[i] = 0x3EA0; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3EA2; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3EA4; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3EA6; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3EA8; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3EAA; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3EAC; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3EAE; regValue[i] = 0x0000; i++;  	// 

	regAddr[i] = 0x3EB0; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3EB2; regValue[i] = 0x0000; i++;  	// 
	regAddr[i] = 0x3EB4; regValue[i] = 0x0000; i++;  	// 

    for (j = 0; j < i; j++)
    {
        status = Iss_deviceWrite16(gAr0522_I2c_ctrl.i2cInstId, devAddr,
                 &gAr0522_I2c_ctrl.regAddr[j],
                 &gAr0522_I2c_ctrl.regValue[j],
                 gAr0522_I2c_ctrl.numRegs);

	totalI++;	// 10/17/22 DAT

        if (status != FVID2_SOK)
        {
	    Vps_printf("<<<<<<< Line %d: Error %d writing AR0522 (0x%02X) reg 0x%04X=0x%04X >>>>>>>\n",
		 __LINE__,
		 status,
		devAddr,
                 gAr0522_I2c_ctrl.regAddr[j],
                 gAr0522_I2c_ctrl.regValue[j]);		// 6/28/22 DAT
        }

        if (gAr0522_I2c_ctrl.regAddr[j] == 0x301A)
        {
#ifdef IMGS_MICRON_AR0522_WDR
            delay = 500000;
#else
            delay = 100000;
#endif
            while (delay--)
            {
            }

        }
        else
        {
            delay = 1000;
            while (delay--)
            {
            }

        }

    }
#endif

#if 1
	Vps_printf("<<<<<<< Line %d: 'snslog_setup_recommended_12bit'>>>>>>>\n", __LINE__ );
	/*
	[Hidden: analog_setup_recommended_12bit]
	REG=0x3EB6, 0x004C     // RESERVED
	REG=0x3EBA, 0xAAAA    // RESERVED
	REG=0x3EBC, 0x0086     // RESERVED
	REG=0x3EC0, 0x1E00     // RESERVED
	REG=0x3EC2, 0x100B     // RESERVED
	REG=0x3EC4, 0x3300     // RESERVED
	REG=0x3EC6, 0xEA44     // RESERVED
	REG=0x3EC8, 0x6F6F     // RESERVED
	REG=0x3ECA, 0x2F4A     // RESERVED
	REG=0x3ECC, 0x0506     // RESERVED
	REG=0x3ECE, 0x203B     // RESERVED
	REG=0x3ED0, 0x13F0     // RESERVED
	REG=0x3ED2, 0x9A3D     // RESERVED
	REG=0x3ED4, 0x862F     // RESERVED
	REG=0x3ED6, 0x4081     // RESERVED
	REG=0x3ED8, 0x4003     // RESERVED
	REG=0x3EDA, 0x9A80     // RESERVED
	REG=0x3EDC, 0xC000     // RESERVED
	REG=0x3EDE, 0xC303     // RESERVED
	REG=0x3426, 0x1600     // RESERVED
	REG=0x342A, 0x0038     // RESERVED
	REG=0x3F3E, 0x0001 //Switch ADC from 10 bit to 12 bit mode
	REG=0x341A, 0x6051     // RESERVED
	REG=0x3420, 0x6051     // RESERVED
	*/

	i = 0;
	regAddr[i] = 0x3EB6; regValue[i] = 0x004C; i++;  	// 
	regAddr[i] = 0x3EBA; regValue[i] = 0xAAAA; i++;  	// 
	regAddr[i] = 0x3EBC; regValue[i] = 0x0086; i++;  	// 
	regAddr[i] = 0x3EC0; regValue[i] = 0x1E00; i++;  	// 
	regAddr[i] = 0x3EC2; regValue[i] = 0x100B; i++;  	// 
	regAddr[i] = 0x3EC4; regValue[i] = 0x3300; i++;  	// 
	regAddr[i] = 0x3EC6; regValue[i] = 0xEA44; i++;  	// 
	regAddr[i] = 0x3EC8; regValue[i] = 0x6F6F; i++;  	// 
	regAddr[i] = 0x3ECA; regValue[i] = 0x2F4A; i++;  	// 
	regAddr[i] = 0x3ECC; regValue[i] = 0x0506; i++;  	// 
	regAddr[i] = 0x3ECE; regValue[i] = 0x203B; i++;  	// 
	regAddr[i] = 0x3ED0; regValue[i] = 0x13F0; i++;  	// 
	regAddr[i] = 0x3ED2; regValue[i] = 0x9A3D; i++;  	// 
	regAddr[i] = 0x3ED4; regValue[i] = 0x862F; i++;  	// 
	regAddr[i] = 0x3ED6; regValue[i] = 0x4081; i++;  	// 
	regAddr[i] = 0x3ED8; regValue[i] = 0x4003; i++;  	// 
	regAddr[i] = 0x3EDA; regValue[i] = 0x9A80; i++;  	// 
	regAddr[i] = 0x3EDC; regValue[i] = 0xC000; i++;  	// 
	regAddr[i] = 0x3EDE; regValue[i] = 0xC303; i++;  	// 
	regAddr[i] = 0x3426; regValue[i] = 0x1600; i++;  	// 
	regAddr[i] = 0x342A; regValue[i] = 0x0038; i++;  	// 
	regAddr[i] = 0x3F3E; regValue[i] = 0x0001; i++;  	// Switch ADC from 10 bit to 12 bit mode
	regAddr[i] = 0x341A; regValue[i] = 0x6051; i++;  	// 
	regAddr[i] = 0x3420; regValue[i] = 0x6051; i++;  	// 

	Vps_printf("<<<<<<< Line %d: 'snslog_setup_recommended_10bit'>>>>>>>\n", __LINE__ );
	/*
	[Hidden: analog_setup_recommended_10bit]
	REG=0x3EC2, 0x100A     // RESERVED
	REG=0x3ED8, 0x8003     // RESERVED
	REG=0x341A, 0x4735     // RESERVED
	REG=0x3420, 0x4735     // RESERVED
	REG=0x3426, 0x8A1A     // RESERVED
	REG=0x342A, 0x018     // RESERVED
	REG=0x3ED2, 0xA53D    // RESERVED
	REG=0x3EDA, 0xA580    // RESERVED
	REG=0x3EBA, 0xAAAD     // RESERVED
	REG=0x3EB6, 0x004C     // RESERVED
	REG=0x3EDE, 0xC403     // RESERVED
	REG=0x3F3E, 0x0 //Switch ADC from 12 bit to 10 bit mode
	*/

	regAddr[i] = 0x3EC2; regValue[i] = 0x100A; i++;  	// 
	regAddr[i] = 0x3ED8; regValue[i] = 0x8003; i++;  	// 
	regAddr[i] = 0x341A; regValue[i] = 0x4735; i++;  	// 
	regAddr[i] = 0x3420; regValue[i] = 0x4735; i++;  	// 
	regAddr[i] = 0x3426; regValue[i] = 0x8A1A; i++;  	// 
	regAddr[i] = 0x342A; regValue[i] = 0x0018; i++;  	// 
	regAddr[i] = 0x3ED2; regValue[i] = 0xA53D; i++;  	// 
	regAddr[i] = 0x3EDA; regValue[i] = 0xA580; i++;  	// 
	regAddr[i] = 0x3EBA; regValue[i] = 0xAAAD; i++;  	// 
	regAddr[i] = 0x3EB6; regValue[i] = 0x004C; i++;  	// 
	regAddr[i] = 0x3EDE; regValue[i] = 0xC403; i++;  	// 
	regAddr[i] = 0x3F3E; regValue[i] = 0x0000; i++;  	// 

    for (j = 0; j < i; j++)
    {
        status = Iss_deviceWrite16(gAr0522_I2c_ctrl.i2cInstId, devAddr,
                 &gAr0522_I2c_ctrl.regAddr[j],
                 &gAr0522_I2c_ctrl.regValue[j],
                 gAr0522_I2c_ctrl.numRegs);

	totalI++;	// 10/17/22 DAT

        if (status != FVID2_SOK)
        {
	    Vps_printf("<<<<<<< Line %d: Error %d writing AR0522 reg 0x%04X=0x%04X >>>>>>>\n",
		 __LINE__,
		 status,
                 gAr0522_I2c_ctrl.regAddr[j],
                 gAr0522_I2c_ctrl.regValue[j]);		// 6/28/22 DAT
        }

        if (gAr0522_I2c_ctrl.regAddr[j] == 0x301A)
        {
#ifdef IMGS_MICRON_AR0522_WDR
            delay = 500000;
#else
            delay = 100000;
#endif
            while (delay--)
            {
            }

        }
        else
        {
            delay = 1000;
            while (delay--)
            {
            }

        }

    }
#endif


	i = 0;
#if 1
	Vps_printf("<<<<<<< Line %d: 'mipi_timing_recommended_10bit'>>>>>>>\n", __LINE__ );
	/*
	[Hidden: mipi_timing_recommended_10bit]
	REG=0x31B0, 0x0058 //
	REG=0x31B2, 0x002C //
	REG=0x31B4, 0x23D2 //
	REG=0x31B6, 0x13E9 //
	REG=0x31B8, 0x2413 //
	REG=0x31BA, 0x1C68 //
	REG=0x31BC, 0x860B //
	REG=0x112, 0x0A0A //
	*/

	regAddr[i] = 0x31B0; regValue[i] = 0x0058; i++;  	// Frame preamble 58
	regAddr[i] = 0x31B2; regValue[i] = 0x002C; i++;  	// Line preamble 2C
	regAddr[i] = 0x31B4; regValue[i] = 0x23D2; i++;  	// MIPI timing0
	regAddr[i] = 0x31B6; regValue[i] = 0x13E9; i++;  	// MIPI timing1
	regAddr[i] = 0x31B8; regValue[i] = 0x2413; i++;  	// MIPI timing2
	regAddr[i] = 0x31BA; regValue[i] = 0x1C68; i++;  	// MIPI timing3
	regAddr[i] = 0x31BC; regValue[i] = 0x860B; i++;  	// MIPI timing4
	regAddr[i] = 0x0112; regValue[i] = 0x0A0A; i++;  	// 

	Vps_printf("<<<<<<< Line %d: 'pll_setup_max_10bit'>>>>>>>\n", __LINE__ );
	/*
	[Hidden: pll_setup_max_10bit]
	REG=0x304, 0x0303 //pre_pll_clk_div//PRE_PLL_CLK_DIV2=3// PRE_PLL_CLK_DIV1=3
	REG=0x306, 0x7359 //pll_multiplier//PLL_MULTIPLIER2=115// PLL_MULTIPLIER1=89
	REG=0x302, 0x0001 //vt_sys_clk_div=1
	REG=0x30A, 0x0001 //op_sys_clk_div=1
	REG=0x300, 0x0005 //vt_pix_clk_div=5
	REG=0x308, 0x000A //op_pix_clk_div=10
	REG=0x3016, 0x0101     // RESERVED
	*/

	regAddr[i] = 0x0304; regValue[i] = 0x0303; i++;  	// PRE_PLL_CLK_DIV2=3// PRE_PLL_CLK_DIV1=3
	regAddr[i] = 0x0306; regValue[i] = 0x7359; i++;  	// PLL_MULTIPLIER2=115// PLL_MULTIPLIER1=89
	regAddr[i] = 0x0302; regValue[i] = 0x0001; i++;  	// VT_SYS_CLK_DIV=1
	regAddr[i] = 0x030A; regValue[i] = 0x0001; i++;  	// OP_SYS_CLK_DIV=1
	regAddr[i] = 0x0300; regValue[i] = 0x0005; i++;  	// VT_PIX_CLK_DIV=5
	regAddr[i] = 0x0308; regValue[i] = 0x000A; i++;  	// OP_PIX_CLK_DIV=10
	regAddr[i] = 0x3016; regValue[i] = 0x0101; i++;  	// 

	Vps_printf("<<<<<<< Line %d: '1080p60fps_array_setup_1928_1088_skip1'>>>>>>>\n", __LINE__ );
	/*
	[Hidden: 1080p60fps_array_setup_1928_1088_skip1]
	REG=0x3004, 0x154 //x_addr_start 340
	REG=0x3008, 0x8DB //x_addr_end 2267
	REG=0x3002, 0x1B4 //y_addr_start 436
	REG=0x3006, 0x5F3 //y_addr_end 1523
	REG=0x3040, 0x041     // read_mode
	REG=0x3172, 0x206     // analog_control2
	REG=0x317A, 0x416E     // analog_control6
	REG=0x3F3C, 0x3     // RESERVED
	REG=0x34C, 0x788 //x_output_size 1928
	REG=0x34E, 0x440 //y_output_size 1088
	REG=0x300C, 0x1178 //line_length_pck
	REG=0x300A, 0x4E4 //frame_length_lines
	*/

	regAddr[i] = 0x3004; regValue[i] = 0x0154; i++;  	// x_addr_start 340
	regAddr[i] = 0x3008; regValue[i] = 0x08DB; i++;  	// x_addr_end 2267
	regAddr[i] = 0x3002; regValue[i] = 0x01B4; i++;  	// y_addr_start 436
	regAddr[i] = 0x3006; regValue[i] = 0x05F3; i++;  	// y_addr_end 1523
	regAddr[i] = 0x3040; regValue[i] = 0x0041; i++;  	// read_mode
	regAddr[i] = 0x3172; regValue[i] = 0x0206; i++;  	// analog_control2
	regAddr[i] = 0x317A; regValue[i] = 0x416E; i++;  	// analog_control6
	regAddr[i] = 0x3F3C; regValue[i] = 0x0003; i++;  	// RESERVED
	regAddr[i] = 0x034C; regValue[i] = 0x078B; i++;  	// x_output_size 1928
	regAddr[i] = 0x034E; regValue[i] = 0x0440; i++;  	// y_output_size 1088
	regAddr[i] = 0x300C; regValue[i] = 0x1178; i++;  	// line_length_pck
	regAddr[i] = 0x300A; regValue[i] = 0x04E4; i++;  	// frame_length_lines

	Vps_printf("<<<<<<< Line %d: 'Data_pedestal_12bit'>>>>>>>\n", __LINE__ );
	/*
	[Hidden: Data_pedestal_12bit]
	REG=0x301A, 0x210     // reset_register
	REG=0x301E, 0x0A8 //
	REG=0x301A, 0x218     // reset_register
	*/

	regAddr[i] = 0x301A; regValue[i] = 0x0210; i++;  	// reset_register
	regAddr[i] = 0x301E; regValue[i] = 0x00A8; i++;  	// 
	regAddr[i] = 0x301A; regValue[i] = 0x0218; i++;  	// reset_register

	Vps_printf("<<<<<<< Line %d: 'new RNC 10bit+ *+65944'>>>>>>>\n", __LINE__ );
	/*
	[Hidden: new RNC 10bit]
	REG = 0x30EE, 0x1136     // RESERVED
	REG = 0x3F2C, 0x442E     // RESERVED
	//for 10bit mode
	REG = 0x301A, 0x0210    // reset_register
	REG = 0x301E, 0x00AA // PEDESTAL+2 :+2 is a workaround for 10bit mode +0.5 Rounding
	REG = 0x301A, 0x0218     // reset_register
	REG = 0x3120, 0x0005     // RESERVED
	*/

	regAddr[i] = 0x30EE; regValue[i] = 0x1136; i++;  	// RESERVED
	regAddr[i] = 0x3F2C; regValue[i] = 0x442E; i++;  	// RESERVED
	regAddr[i] = 0x301A; regValue[i] = 0x0210; i++;  	// reset_register
	regAddr[i] = 0x301E; regValue[i] = 0x00AA; i++;  	// PEDESTAL+2 :+2 is a workaround for 10bit mode +0.5 Rounding
	regAddr[i] = 0x301A; regValue[i] = 0x0218; i++;  	// reset_register
	regAddr[i] = 0x3120; regValue[i] = 0x0005; i++;  	// 

	/*
	REG=0x300C, 0x0BE0 //line_length_pck
	REG=0x300A, 0x08DC //frame_length_lines
	*/

	regAddr[i] = 0x300C; regValue[i] = 0x0BE0; i++;  	// line_length_pck
	regAddr[i] = 0x300A; regValue[i] = 0x08DC; i++;  	// frame_length_lines

	// MIPI signal test
//	regAddr[i] = 0x3066; regValue[i] = 0x03D4; i++;  	// (0x0F<<6)+(5<<2)

#endif
	/*
	[Hidden: enable_streaming]
	REG=0x301A, 0x21C     // reset_register
	*/

	regAddr[i] = 0x301A; regValue[i] = 0x021C; i++;  	// reset_register, enable_streaming


    for (j = 0; j < i; j++)
    {
        status = Iss_deviceWrite16(gAr0522_I2c_ctrl.i2cInstId, devAddr,
                 &gAr0522_I2c_ctrl.regAddr[j],
                 &gAr0522_I2c_ctrl.regValue[j],
                 gAr0522_I2c_ctrl.numRegs);

	totalI++;	// 10/17/22 DAT

        if (status != FVID2_SOK)
        {
	    Vps_printf("<<<<<<< Line %d: Error %d writing AR0522 reg 0x%04X=0x%04X >>>>>>>\n",
		 __LINE__,
		 status,
                 gAr0522_I2c_ctrl.regAddr[j],
                 gAr0522_I2c_ctrl.regValue[j]);		// 6/28/22 DAT
        }

        if (gAr0522_I2c_ctrl.regAddr[j] == 0x301A)
        {
#ifdef IMGS_MICRON_AR0522_WDR
            delay = 500000;
#else
            delay = 100000;
#endif
            while (delay--)
            {
            }

        }
        else
        {
            delay = 1000;
            while (delay--)
            {
            }

        }

    }

    // printf("Finished Demo Init with AR0522\n");
    Vps_printf("<<<<<<<<<< %s:%s:%d: Finished Init for AR0522, total parms=%d >>>>>>>\n", __FILE__, __FUNCTION__, __LINE__, totalI);	// 10/17/22 DAT
    return FVID2_SOK;
}

/* ===================================================================
 *  @func     Transplant_AR0522_HDR_Enable
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
int Transplant_AR0522_HDR_Enable(int enable)
{
    int status = 0;

    // Following N/A for ar0522 CSIS  6/28/22 DAT
    // Commented out in 'Iss_Ar0522Create' func

#if 0
    Uint16 regAddr[32];

    Uint16 regValue[32];


    int i = 0;

    int j = 0;

    int delay = 100000;

    gAr0522_I2c_ctrl.i2cInstId = Iss_platformGetI2cInstId();
    Int32 devAddr = AR_0522_ADDR;

    if (enable)
    {                                                      /* rev1,32X */
        /*
         * regAddr[i] = 0x3082; regValue[i] = 0x000c; i++; regAddr[i] =
         * 0x31d0; regValue[i] = 0x0001; i++; regAddr[i] = 0x301a;
         * regValue[i] = 0x10de; i++; regAddr[i] = 0x318c; regValue[i] =
         * 0xc001; i++; regAddr[i] = 0x31d0; regValue[i] = 0x0000; i++;
         * regAddr[i] = 0x2400; regValue[i] = 0x0002; i++; regAddr[i] =
         * 0x3064; regValue[i] = 0x1882; i++; */
        /* rev2 4x */
        regAddr[i] = 0x3082;
        regValue[i] = 0x0004;
        i++;
        regAddr[i] = 0x305E;
        regValue[i] = 0x0080;
        i++;

        // Delay=60

        regAddr[i] = 0x318C;
        regValue[i] = 0xC001;
        i++;
        regAddr[i] = 0x3198;
        regValue[i] = 0x061E;
        i++;

        // [ALTM Enabled]
        regAddr[i] = 0x301A;
        regValue[i] = 0x10D8;
        i++;                                               // stopping
                                                           // streaming

        regAddr[i] = 0x2442;
        regValue[i] = 0x0080;
        i++;
        regAddr[i] = 0x2444;
        regValue[i] = 0x0000;
        i++;
        regAddr[i] = 0x2446;
        regValue[i] = 0x0004;
        i++;
        regAddr[i] = 0x2440;
        regValue[i] = 0x0002;
        i++;
        regAddr[i] = 0x2438;
        regValue[i] = 0x0010;
        i++;
        regAddr[i] = 0x243A;
        regValue[i] = 0x0020;
        i++;
        regAddr[i] = 0x243C;
        regValue[i] = 0x0000;
        i++;
        regAddr[i] = 0x243E;
        regValue[i] = 0x0200;
        i++;
        regAddr[i] = 0x31D0;
        regValue[i] = 0x0000;
        i++;
        regAddr[i] = 0x301E;
        regValue[i] = 0x0000;
        i++;
        /* LOAD= ALTM Enabled Devware Color Setup */
        regAddr[i] = 0x2400;
        regValue[i] = 0x0002;
        i++;
        // regAddr[i]= 0x2400; regValue[i]=0x0000;i++;

        regAddr[i] = 0x301A;
        regValue[i] = 0x10DC;
        i++;                                               // Enable
                                                           // Streaming

        regAddr[i] = 0x30FE;
        regValue[i] = 0x0000;
        i++;
        regAddr[i] = 0x31E0;
        regValue[i] = 0x0200;
        i++;
        regAddr[i] = 0x320A;
        regValue[i] = 0x0000;
        i++;
        regAddr[i] = 0x2450;
        regValue[i] = 0x0000;
        i++;
        regAddr[i] = 0x301E;
        regValue[i] = 0x0000;
        i++;
        regAddr[i] = 0x318A;
        regValue[i] = 0x0E10;
        i++;
        regAddr[i] = 0x3064;
        regValue[i] = 0x1982;
        i++;
        regAddr[i] = 0x3064;
        regValue[i] = 0x1982;
        i++;

        regAddr[i] = 0x301A;
        regValue[i] = 0x10DE;
        i++;

        // [ADACD Enabled]
        regAddr[i] = 0x3202;
        regValue[i] = 0x00A0;
        i++;
        regAddr[i] = 0x3206;
        regValue[i] = 0x1810;
        i++;
        regAddr[i] = 0x3208;
        regValue[i] = 0x2820;
        i++;
        regAddr[i] = 0x3200;
        regValue[i] = 0x0002;
        i++;
    }
    else
    {
        regAddr[i] = 0x3082;
        regValue[i] = 0x0009;
        i++;
        regAddr[i] = 0x31d0;
        regValue[i] = 0x0000;
        i++;
        regAddr[i] = 0x301a;
        regValue[i] = 0x10dc;
        i++;
        regAddr[i] = 0x318c;
        regValue[i] = 0x0000;
        i++;
        regAddr[i] = 0x31d0;
        regValue[i] = 0x0000;
        i++;
        regAddr[i] = 0x2400;
        regValue[i] = 0x0003;
        i++;
        regAddr[i] = 0x3064;
        regValue[i] = 0x1082;
        i++;
    }

    gAr0522_I2c_ctrl.numRegs = 1;
    for (j = 0; j < i; j++)
    {
        status =
            Iss_deviceWrite16(gAr0522_I2c_ctrl.i2cInstId, devAddr, &regAddr[j],
                              &regValue[j], gAr0522_I2c_ctrl.numRegs);

        if (status != FVID2_SOK)
        {
            // printf("I2C write Error,index:%d\n",j);
            return status;
        }
        if (regAddr[j] == 0x301A)
        {
            while (delay--)
            {
            }
            delay = 100000;
        }

    }

#endif

    return status;
}

/*
 * System init for AR0522 driver
 *
 * This API - create semaphore locks needed - registers driver to FVID2
 * sub-system - gets called as part of Iss_deviceInit() */

/* ===================================================================
 *  @func     Iss_Ar0522Init
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Int32 Iss_Ar0522Init()
{
    Semaphore_Params semParams;

    Int32 status = FVID2_SOK;

    Vps_printf("<<<<<<<<<< %s:%s:%d: Initializing AR0522 regs >>>>>>>>>>\n", __FILE__, __FUNCTION__, __LINE__);	// 7/11/22 DAT

    /*
     * Set to 0's for global object, descriptor memory
     */
    memset(&gIss_Ar0522CommonObj, 0, sizeof(gIss_Ar0522CommonObj));

    /*
     * Create global AR0522 lock
     */
    Semaphore_Params_init(&semParams);

    semParams.mode = Semaphore_Mode_BINARY;

    gIss_Ar0522CommonObj.lock = Semaphore_create(1u, &semParams, NULL);

    if (gIss_Ar0522CommonObj.lock == NULL)
        status = FVID2_EALLOC;

    if (status == FVID2_SOK)
    {
        /*
         * Register AR0522 driver with FVID2 sub-system
         */
        /*
         * gAr0522_I2c_ctrl.i2cInstId = Iss_platformGetI2cInstId(); devAddr =
         * AR_0522_ADDR;
         *
         * for(count=0; count<353; count++){ gAr0522_I2c_ctrl.regAddr[count]
         * = SensorConfigScript[count][0]; gAr0522_I2c_ctrl.regValue[count] =
         * SensorConfigScript[count][2]; gAr0522_I2c_ctrl.numRegs = 1;
         * if(SensorConfigScript[count][1] == 2){ status =
         * Iss_deviceWrite16(gAr0522_I2c_ctrl.i2cInstId,devAddr,&gAr0522_I2c_ctrl.regAddr[count],&gAr0522_I2c_ctrl.regValue[count],gAr0522_I2c_ctrl.numRegs);
         * if(gAr0522_I2c_ctrl.regAddr[count] == 0x301A){ while(delay--){} delay = 100000; } } else if(SensorConfigScript[count][2] ==1){ // status =
         * Iss_deviceWrite8(gAr0522I2c_ctrl.i2cInstId,devAddr,&gAr0522_I2c_ctrl.regAddr[count],&gAr0522_I2c_ctrl.regValue[count],gAr0522_I2c_ctrl.numRegs); }
         * else if(SensorConfigScript[count][2] ==0){ // DM814x_usecDelay(SensorConfigScript[count][2]); } } */

        Transplant_DRV_imgsSetRegs();

        Iss_Ar0522PwmVideoSet(83320, 83320);
        Iss_Ar0522PwmDcSet(83320, 41660);

        gIss_Ar0522CommonObj.fvidDrvOps.create = (FVID2_DrvCreate) Iss_Ar0522Create;
        gIss_Ar0522CommonObj.fvidDrvOps.delete = Iss_Ar0522Delete;
        gIss_Ar0522CommonObj.fvidDrvOps.control = Iss_Ar0522Control;
        gIss_Ar0522CommonObj.fvidDrvOps.queue = NULL;
        gIss_Ar0522CommonObj.fvidDrvOps.dequeue = NULL;
        gIss_Ar0522CommonObj.fvidDrvOps.processFrames = NULL;
        gIss_Ar0522CommonObj.fvidDrvOps.getProcessedFrames = NULL;
        gIss_Ar0522CommonObj.fvidDrvOps.drvId = FVID2_ISS_SENSOR_AR0522_DRV;

        status = FVID2_registerDriver(&gIss_Ar0522CommonObj.fvidDrvOps);

        if (status != FVID2_SOK)
        {
            /*
             * Error - free acquired resources
             */
            Semaphore_delete(&gIss_Ar0522CommonObj.lock);
        }
    }

    if (status != FVID2_SOK)
    {
        Vps_printf(" ERROR %s:%s:%d !!!\n", __FILE__, __FUNCTION__, __LINE__);
    }

    return status;
}

/*
 * System de-init for AR0522 driver
 *
 * This API - de-registers driver from FVID2 sub-system - delete's allocated
 * semaphore locks - gets called as part of Iss_deviceDeInit() */
/* ===================================================================
 *  @func     Iss_Ar0522DeInit
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Int32 Iss_Ar0522DeInit()
{
    /*
     * Unregister FVID2 driver
     */
    FVID2_unRegisterDriver(&gIss_Ar0522CommonObj.fvidDrvOps);

    /*
     * Delete semaphore's
     */
    Semaphore_delete(&gIss_Ar0522CommonObj.lock);

    return 0;
}

/* ===================================================================
 *  @func     Iss_Ar0522PinMux
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Int32 Iss_Ar0522PinMux()
{
    // Following N/A for ar0522 CSIS  6/28/22 DAT

#if 0
    /* setup CAM input pin mux */
    *PINCNTL156 = 0x00050002;                              // select function
                                                           // 2 with receiver
                                                           // enabled and
                                                           // pullup/down
                                                           // disabled - only
                                                           // works in
                                                           // supervisor mode
    *PINCNTL157 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL158 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL159 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL160 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL161 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL162 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL163 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL164 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL165 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL166 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL167 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL168 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL169 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL170 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL171 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL172 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL173 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL174 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    *PINCNTL175 = 0x00050002;                              // select function
                                                           // 2 - only works
                                                           // in supervisor
                                                           // mode
    #if defined(TI_8107_BUILD)
    *PINCNTL135 = 0xE0040;   /* i2c2_scl_mux0 */
    *PINCNTL136 = 0xE0040;   /* i2c2_sda_mux0 */
    #endif
    #if defined(TI_814X_BUILD)
    *PINCNTL74 = 0x00020;
    *PINCNTL75 = 0x00020;
    #endif

#endif

    return 0;
}

/*
 * Handle level lock */
/* ===================================================================
 *  @func     Iss_Ar0522LockObj
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Int32 Iss_Ar0522LockObj(Iss_Ar0522Obj * pObj)
{
    Semaphore_pend(pObj->lock, BIOS_WAIT_FOREVER);

    return FVID2_SOK;
}

/*
 * Handle level unlock */
/* ===================================================================
 *  @func     Iss_Ar0522UnlockObj
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Int32 Iss_Ar0522UnlockObj(Iss_Ar0522Obj * pObj)
{
    Semaphore_post(pObj->lock);

    return FVID2_SOK;
}

/*
 * Global driver level lock */

/* ===================================================================
 *  @func     Iss_Ar0522Lock
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Int32 Iss_Ar0522Lock()
{
    Semaphore_pend(gIss_Ar0522CommonObj.lock, BIOS_WAIT_FOREVER);

    return FVID2_SOK;
}

/*
 * Global driver level unlock */
/* ===================================================================
 *  @func     Iss_Ar0522Unlock
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Int32 Iss_Ar0522Unlock()
{
    Semaphore_post(gIss_Ar0522CommonObj.lock);

    return FVID2_SOK;
}

/*
 * Allocate driver object
 *
 * Searches in list of driver handles and allocate's a 'NOT IN USE' handle
 * Also create's handle level semaphore lock
 *
 * return NULL in case handle could not be allocated */
/* ===================================================================
 *  @func     Iss_Ar0522AllocObj
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Iss_Ar0522Obj *Iss_Ar0522AllocObj()
{
    UInt32 handleId;

    Iss_Ar0522Obj *pObj;

    Semaphore_Params semParams;

    UInt32 found = FALSE;

    /*
     * Take global lock to avoid race condition
     */
    Iss_Ar0522Lock();

    /*
     * find a unallocated object in pool
     */
    for (handleId = 0; handleId < ISS_DEVICE_MAX_HANDLES; handleId++)
    {

        pObj = &gIss_Ar0522CommonObj.Ar0522Obj[handleId];

        if (pObj->state == ISS_AR0522_OBJ_STATE_UNUSED)
        {
            /*
             * free object found
             */

            /*
             * init to 0's
             */
            memset(pObj, 0, sizeof(*pObj));

            /*
             * init state and handle ID
             */
            pObj->state = ISS_AR0522_OBJ_STATE_IDLE;
            pObj->handleId = handleId;

            /*
             * create driver object specific semaphore lock
             */
            Semaphore_Params_init(&semParams);

            semParams.mode = Semaphore_Mode_BINARY;

            pObj->lock = Semaphore_create(1u, &semParams, NULL);

            found = TRUE;

            if (pObj->lock == NULL)
            {
                /*
                 * Error - release object
                 */
                found = FALSE;
                pObj->state = ISS_AR0522_OBJ_STATE_UNUSED;
            }

            break;
        }
    }

    /*
     * Release global lock
     */
    Iss_Ar0522Unlock();

    if (found)
        return pObj;                                       /* Free object
                                                            * found return it
                                                            */

    /*
     * free object not found, return NULL
     */
    return NULL;
}

/*
 * De-Allocate driver object
 *
 * Marks handle as 'NOT IN USE' Also delete's handle level semaphore lock */
/* ===================================================================
 *  @func     Iss_Ar0522FreeObj
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Int32 Iss_Ar0522FreeObj(Iss_Ar0522Obj * pObj)
{
    /*
     * take global lock
     */
    Iss_Ar0522Lock();

    if (pObj->state != ISS_AR0522_OBJ_STATE_UNUSED)
    {
        /*
         * mark state as unused
         */
        pObj->state = ISS_AR0522_OBJ_STATE_UNUSED;

        /*
         * delete object locking semaphore
         */
        Semaphore_delete(&pObj->lock);
    }

    /*
     * release global lock
     */
    Iss_Ar0522Unlock();

    return FVID2_SOK;
}

UInt32 Iss_Ar0522GainCalc(int gain)
{
	int Gain1 = 0;
	unsigned int Gain2 = 0;
	int GainSend = 0;

	if( gain >= 640000 )
		gain = 635000;

	if( gain >= 320000 )
	{
		Gain1 = 0x30;
		Gain2 = (unsigned int )gain*1000/8;
		Gain2 /= 78125;
	}
	else if( gain >= 160000 )
	{
		Gain1 = 0x30;
		Gain2 = (unsigned int )gain*1000/8;
		Gain2 /= 78125;
	}
	else if( gain >= 80000 )
	{
		Gain1 = 0x30;
		Gain2 = (unsigned int )gain*1000/8;
		Gain2 /= 78125;
	}
	else if( gain >= 40000 )
	{
		Gain1 = 0x20;
		Gain2 = (unsigned int )gain*1000/4;
		Gain2 /= 78125;

	}
	else if( gain >= 20000 )
	{
		Gain1 = 0x10;
		Gain2 = (unsigned int )gain*1000/2;
		Gain2 /= 78125;

	}
	else if( gain >= 10000 )
	{
		Gain1 = 0x00;
		Gain2 = (unsigned int )gain*1000/1;
		Gain2 /= 78125;
	}

	GainSend = (Gain2<<8)|Gain1;

	return (UInt32)GainSend;
}

Int32 Iss_Ar0522UpdateExpGain(Iss_Ar0522Obj * pObj, Ptr createArgs)
{
    Int32 status = FVID2_SOK;
	Int32 count = 0;
	Int32 aewbVendor;


    UInt32 i2cInstId = Iss_platformGetI2cInstId();

    Int32 devAddr = AR_0522_ADDR;

    gAr0522_I2c_ctrl.i2cInstId = Iss_platformGetI2cInstId();
    devAddr = AR_0522_ADDR;

    /*
     * take global lock
     */
    Iss_Ar0522Lock();
	count = 0;
	aewbVendor = IssAlg_capt2AGetAEWBVendor();

	if( aewbVendor == 1 )
	{
		if (ti2a_output_params.mask)
		{
			unsigned int GainSend = Iss_Ar0522GainCalc(ti2a_output_params.sensorGain);
			gAr0522_I2c_ctrl.regAddr[count] = AR0522_ANALOG_GAIN;
			gAr0522_I2c_ctrl.regValue[count] =  (GainSend&0xFF);
			count++;

			gAr0522_I2c_ctrl.regAddr[count] = AR0522_DIGITAL_GAIN;
			gAr0522_I2c_ctrl.regValue[count] =  ((GainSend>>8)&0xFFFF);
			count++;
			//Vps_printf (" sensorGain = 0x%x \r\n", ti2a_output_params.sensorGain);
		}

		if (ti2a_output_params.mask)
		{
			gAr0522_I2c_ctrl.regAddr[count] = AR0522_COARSE_IT_TIME_A;
			gAr0522_I2c_ctrl.regValue[count] =  ti2a_output_params.sensorExposure;
			count++;
			//Vps_printf (" sensorExposure = %d \r\n", ti2a_output_params.sensorExposure);
		}


		gAr0522_I2c_ctrl.numRegs = count;
	}
  else if(aewbVendor == 2)
  {
		if (ti2a_output_params.mask & 3)
		{

      Uint16 shutter = ti2a_output_params.sensorExposure / ROW_TIME;
      Uint16 aGain   = AR0522_GainTableMap(ti2a_output_params.sensorGain);

      gAr0522_I2c_ctrl.regAddr[count]  = AR0522_ANALOG_GAIN;
			gAr0522_I2c_ctrl.regValue[count] =  aGain;
			count++;
			//Vps_printf (" sensorGain = %d, reg value 0x%x \r\n", ti2a_output_params.sensorGain,aGain);

			gAr0522_I2c_ctrl.regAddr[count]  = AR0522_COARSE_IT_TIME_A;
			gAr0522_I2c_ctrl.regValue[count] =  shutter;
			count++;
      //Vps_printf (" sensorExposure = %d, reg value %d\r\n", ti2a_output_params.sensorExposure,shutter);
    }
    gAr0522_I2c_ctrl.numRegs = count;
  }

    status = Iss_deviceWrite16(gAr0522_I2c_ctrl.i2cInstId, devAddr,
    		&gAr0522_I2c_ctrl.regAddr[0],
    		&gAr0522_I2c_ctrl.regValue[0],
    		gAr0522_I2c_ctrl.numRegs);


    /*
     * release global lock
     */
    Iss_Ar0522Unlock();

    return status;
}

Int32 Iss_Ar0522FrameRateSet(Iss_Ar0522Obj * pObj, Ptr createArgs,
                              Ptr cmdStatusArgs)
{
    Int32 status = FVID2_SOK;

    UInt32 i2cInstId = Iss_platformGetI2cInstId();

    Int32 devAddr = AR_0522_ADDR;

    Int32 count = 0;

    Int32 framerate = *(Int32 *) createArgs;

    gAr0522_I2c_ctrl.i2cInstId = Iss_platformGetI2cInstId();
    devAddr = AR_0522_ADDR;

    if (framerate == 60)
    {

        gAr0522_I2c_ctrl.regAddr[count] = 0x300C;
        gAr0522_I2c_ctrl.regValue[count] = 0x44C;
        count++;
        *(Int32 *) cmdStatusArgs = (1100 *PLL_MULTIPLIER)/0x4a;
    }
    else if (framerate == 50)
    {
        gAr0522_I2c_ctrl.regAddr[count] = 0x300C;
        gAr0522_I2c_ctrl.regValue[count] = 0x528;
        count++;
        *(Int32 *) cmdStatusArgs = (1320 *PLL_MULTIPLIER)/0x4a;
    }
    else if (framerate == 30)
    {
        gAr0522_I2c_ctrl.regAddr[count] = 0x300C;
        gAr0522_I2c_ctrl.regValue[count] = (0x898 *PLL_MULTIPLIER)/0x4a;
        count++;
        *(Int32 *) cmdStatusArgs = (2200 *PLL_MULTIPLIER)/0x4a;
    }
    else if (framerate == 25)
    {
        gAr0522_I2c_ctrl.regAddr[count] = 0x300C;
        gAr0522_I2c_ctrl.regValue[count] = (0xA50 *PLL_MULTIPLIER)/0x4a;
        count++;
        *(Int32 *) cmdStatusArgs = (2640 *PLL_MULTIPLIER)/0x4a;
    }

    else
    {
        status = FVID2_EFAIL;
        return status;
    }
    /** take global lock **/
    Iss_Ar0522Lock();

    gAr0522_I2c_ctrl.numRegs = count;
    status =
        Iss_deviceWrite16(gAr0522_I2c_ctrl.i2cInstId, devAddr,
                          &gAr0522_I2c_ctrl.regAddr[0],
                          &gAr0522_I2c_ctrl.regValue[0],
                          gAr0522_I2c_ctrl.numRegs);

    /** release global lock **/
    Iss_Ar0522Unlock();

    return status;
}

/*
 * Update ITT Values */
/* ===================================================================
 *  @func     Iss_Ar0522UpdateItt
 *
 *  @desc     Function does the following
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Int32 Iss_Ar0522UpdateItt(Iss_Ar0522Obj * pObj,
                           Itt_RegisterParams * gItt_RegisterParams)
{
    int status = 0;

    int devAddr, count = 0;

    I2c_Ctrl gAr0522_I2c_ctrl;

    gAr0522_I2c_ctrl.i2cInstId = Iss_platformGetI2cInstId();
    devAddr = AR_0522_ADDR;
    if (gItt_RegisterParams->Control == 1)
    {
        gAr0522_I2c_ctrl.regAddr[count] = gItt_RegisterParams->regAddr;
        gAr0522_I2c_ctrl.regValue[count] = gItt_RegisterParams->regValue;
        gAr0522_I2c_ctrl.numRegs = 1;
        status =
            Iss_deviceWrite16(gAr0522_I2c_ctrl.i2cInstId, devAddr,
                              &gAr0522_I2c_ctrl.regAddr[count],
                              &gAr0522_I2c_ctrl.regValue[count],
                              gAr0522_I2c_ctrl.numRegs);
    }
    else if (gItt_RegisterParams->Control == 0)
    {
        gAr0522_I2c_ctrl.regAddr[count] = gItt_RegisterParams->regAddr;
        gAr0522_I2c_ctrl.regValue[count] = 0;
        gAr0522_I2c_ctrl.numRegs = 1;
        status =
            Iss_deviceRead16(gAr0522_I2c_ctrl.i2cInstId, devAddr,
                             &gAr0522_I2c_ctrl.regAddr[count],
                             &gAr0522_I2c_ctrl.regValue[count],
                             gAr0522_I2c_ctrl.numRegs);
        count = 0;
        gItt_RegisterParams->regValue = gAr0522_I2c_ctrl.regValue[count];
    }
    return status;
}
#define DM_TIMER_LOAD_MIN		0xFFFFFFFE
inline Uint32 TimerBaseAddr( Int32 timerId )
{
	Uint32 timerAddr[] = {
		0x4802E000,// 1
		0x48040000,// 2
		0x48042000,// 3
		0x48044000,// 4
		0x48046000,// 5
		0x48048000,// 6
		0x4804A000,// 7
		0x481C1000 // 8
	};
	return timerAddr[(timerId-1)];
}
Uint32 Iss_Ar0522PwmVideoSet(Uint32 load_value, Uint32 match_value)
{
	Uint32	TimerID = 6;
	Uint32  *TimerLoadReg = (Uint32  *)(TimerBaseAddr(TimerID)+0x40);
	Uint32  *TimerMatchReg = (Uint32  *)(TimerBaseAddr(TimerID)+0x4C);

	*TimerLoadReg = DM_TIMER_LOAD_MIN-load_value;
	*TimerMatchReg = DM_TIMER_LOAD_MIN-(load_value-match_value);

	return 0;
}


Uint32 Iss_Ar0522PwmDcSet(Uint32 load_value, Uint32 match_value)
{
	Uint32	TimerID = 7;
	Uint32  *TimerLoadReg = (Uint32  *)(TimerBaseAddr(TimerID)+0x40);
	Uint32  *TimerMatchReg = (Uint32  *)(TimerBaseAddr(TimerID)+0x4C);

	*TimerLoadReg = DM_TIMER_LOAD_MIN-load_value;
	*TimerMatchReg = DM_TIMER_LOAD_MIN-(load_value-match_value);

	return 0;
}

Int32 Iss_Ar0522SetMode(Iss_Ar0522Obj *pObj, FVID2_Standard standard, UInt32 devAddr, UInt32 doReset)
{
    Int32 status = FVID2_SOK;
	UInt16 count = 0;
	
    //Iss_Ar0522Stop(pObj);

    Uint16 *regAddr = gAr0522_I2c_ctrl.regAddr;
    Uint16 *regValue = gAr0522_I2c_ctrl.regValue;

    switch (standard)
    {
    case 0x253: //SYSTEM_STD_3MP_2048_1536: //FVID2_STD_3MP_2048_1536:
		regAddr[count] = 0x301A; regValue[count] = 0x10D8; count++;
		regAddr[count] = 0x302A; regValue[count] = 0x0006; count++;         //VT_PIX_CLK_DIV = 6
		regAddr[count] = 0x302C; regValue[count] = 0x2; count++;         //VT_SYS_CLK_DIV = 1
		regAddr[count] = 0x302E; regValue[count] = 0x3; count++;         //PRE_PLL_CLK_DIV = 4
		regAddr[count] = 0x3030; regValue[count] = 90; count++;        //PLL_MULTIPLIER = 74
		regAddr[count] = 0x3036; regValue[count] = 0x000C; count++;         //OP_PIX_CLK_DIV = 12
		regAddr[count] = 0x3038; regValue[count] = 0x2; count++;         //OP_SYS_CLK_DIV = 1

		regAddr[count] = 0x3002; regValue[count] = 0x4 ; count++;         //Y_ADDR_START = 4
		regAddr[count] = 0x3004; regValue[count] = 0x6 ; count++;         //X_ADDR_START = 6
		regAddr[count] = 0x3006; regValue[count] = 0x603 ; count++;       //Y_ADDR_END = 1539
		regAddr[count] = 0x3008; regValue[count] = 0x805 ; count++;       //X_ADDR_END = 2053
		regAddr[count] = 0x300A; regValue[count] = 0x625 ; count++;       //FRAME_LENGTH_LINES = 1573
		regAddr[count] = 0x300C; regValue[count] = 0x488 ; count++;       //LINE_LENGTH_PCK = 1573
		regAddr[count] = 0x301A; regValue[count] = 0x0058; count++;
		gAr0522_I2c_ctrl.numRegs = 1;
        for (count = 0; count < 14; count++)
        {
			status = Iss_deviceWrite16(gAr0522_I2c_ctrl.i2cInstId, devAddr,
                                      &gAr0522_I2c_ctrl.regAddr[count],
                                      &gAr0522_I2c_ctrl.regValue[count],
                                      gAr0522_I2c_ctrl.numRegs);
			if (regAddr[count] == 0x301A)
			{
				Task_sleep(500);
			}
        }
        break;
	default:
		regAddr[count] = 0x301A; regValue[count] = 0x10D8; count++;
		regAddr[count] = 0x302A; regValue[count] = 0x0006; count++;         //VT_PIX_CLK_DIV = 6
		regAddr[count] = 0x302C; regValue[count] = 0x2; count++;         //VT_SYS_CLK_DIV = 1
		regAddr[count] = 0x302E; regValue[count] = 0x3; count++;         //PRE_PLL_CLK_DIV = 4
		regAddr[count] = 0x3030; regValue[count] = 90; count++;        //PLL_MULTIPLIER = 74
		regAddr[count] = 0x3036; regValue[count] = 0x000C; count++;         //OP_PIX_CLK_DIV = 12
		regAddr[count] = 0x3038; regValue[count] = 0x2; count++;         //OP_SYS_CLK_DIV = 1

		regAddr[count] = 0x3002; regValue[count] = 0x4 ; count++;         //Y_ADDR_START = 4
		regAddr[count] = 0x3004; regValue[count] = 0x6 ; count++;         //X_ADDR_START = 6
		regAddr[count] = 0x3006; regValue[count] = 0x603 ; count++;       //Y_ADDR_END = 1539
		regAddr[count] = 0x3008; regValue[count] = 0x805 ; count++;       //X_ADDR_END = 2053
		regAddr[count] = 0x300A; regValue[count] = 0x625 ; count++;       //FRAME_LENGTH_LINES = 1573
		regAddr[count] = 0x300C; regValue[count] = 0x488 ; count++;       //LINE_LENGTH_PCK = 1573
		regAddr[count] = 0x301A; regValue[count] = 0x0058; count++;
		gAr0522_I2c_ctrl.numRegs = 1;
        for (count = 0; count < 14; count++)
        {
			status = Iss_deviceWrite16(gAr0522_I2c_ctrl.i2cInstId, devAddr,
                                      &gAr0522_I2c_ctrl.regAddr[count],
                                      &gAr0522_I2c_ctrl.regValue[count],
                                      gAr0522_I2c_ctrl.numRegs);
			if (regAddr[count] == 0x301A)
			{
				Task_sleep(500);
			}
        }
        break;
    }

    return (status);
}




