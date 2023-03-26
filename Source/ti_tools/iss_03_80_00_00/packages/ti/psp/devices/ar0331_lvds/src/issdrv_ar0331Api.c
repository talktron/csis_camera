/** ==================================================================
 *  @file   issdrv_ar0331Api.c
 *
 *  @path   /ti/psp/devices/ar0331_lvds/src/
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

#include <ti/psp/devices/ar0331_lvds/src/issdrv_ar0331Priv.h>
#include <ti/psp/devices/ar0331_lvds/issdrv_ar0331_config.h>
#include <ti/psp/platforms/iss_platform.h>
#include <ti/psp/iss/drivers/alg/2A/inc/issdrv_alg2APriv.h>
#define LONG_EXPO                       0x3012
#define LONG_GAIN                       0x305e

/* Global object storing all information related to all AR0331 driver handles
 */
static Iss_Ar0331CommonObj gIss_Ar0331CommonObj;

static I2c_Ctrl gAr0331_I2c_ctrl;

extern ti2a_output ti2a_output_params;

#define DEF_1080P60FPS
#define PLL_MULTIPLIER (0x4A)
#ifdef DEF_1080P60FPS
	//60fps
	//#define PLL_MULTIPLIER (0x4A)  //0x4A for 60fps
	#define VT_SYS_CLK_DIV	(1)
	#define PLL_P1       1   //vt_sys_clk_div

	#define OP_SYS_CLK_DIV	(1)
#else
	//30fps
	//#define PLL_MULTIPLIER (0x4A)
	#define PLL_P1       2   //vt_sys_clk_div
	#define VT_SYS_CLK_DIV	(2)
	#define OP_SYS_CLK_DIV	(2)
#endif

#define INPUT_CLK    24                                    // EXTCLK
#define PLL_M        (74)                                 // pll_multiplier
#define PLL_pre_div  4                                     // pre_pll_clk_div

#define PLL_P2       6   //vt_pix_clk_div
float loc_RealRowTime =AR0331_ROW_TIME;
#define OUT_CLK ((INPUT_CLK * PLL_M) / (PLL_pre_div * PLL_P1 * PLL_P2) )//74MHz (30fps) or 148Mhz (60fps)

//#define USE_TI
/* ===================================================================
 *  @func     Iss_Ar0331Lock
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
Int32 Iss_Ar0331Lock();

/* ===================================================================
 *  @func     Iss_Ar0331Unlock
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
Int32 Iss_Ar0331Unlock();

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
int Ar0331_Transplant_DRV_imgsSetEshutter()
{

    int status;

    Uint16 regAddr;

    Uint16 regValue;

    // printf("current eshutterInUsec:%d-----\n",eshutterInUsec);
    Int32 devAddr = AR_0331_ADDR;

    regAddr = AR0331_COARSE_IT_TIME_A;

    status = Iss_Ar0331Lock();
    if (status == FVID2_SOK)
    {
        regValue = (Uint16)((float) ti2a_output_params.sensorExposure / loc_RealRowTime);
        status =
            Iss_deviceWrite16(gAr0331_I2c_ctrl.i2cInstId, devAddr, &regAddr,
                              &regValue, 1);
    }
    Iss_Ar0331Unlock();

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
 * handle - AR0331 driver handle cmd - command cmdArgs - command arguments
 * cmdStatusArgs - command status
 *
 * returns error in case of - illegal parameters - I2C command RX/TX error */
/* ===================================================================
 *  @func     Iss_Ar0331Control
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
Int32 Iss_Ar0331Control(Fdrv_Handle handle,
                        UInt32 cmd, Ptr cmdArgs, Ptr cmdStatusArgs)
{
    Iss_Ar0331Obj *pObj = (Iss_Ar0331Obj *) handle;

    Int32 status = FVID2_SOK;

    if (pObj == NULL)
        return FVID2_EBADARGS;

    /*
     * lock handle
     */
    Iss_Ar0331LockObj(pObj);

    switch (cmd)
    {
        case FVID2_START:
            // status = Iss_Ar0331Start ( pObj );
            break;

        case FVID2_STOP:
            // status = Iss_Ar0331Stop ( pObj );
            break;

        case IOCTL_ISS_SENSOR_GET_CHIP_ID:
            // status = Iss_Ar0331GetChipId ( pObj, cmdArgs, cmdStatusArgs );
            break;

        case IOCTL_ISS_SENSOR_RESET:
            // status = Iss_Ar0331Reset ( pObj );
            break;

        case IOCTL_ISS_SENSOR_REG_WRITE:
            // status = Iss_Ar0331RegWrite ( pObj, cmdArgs );
            break;

        case IOCTL_ISS_SENSOR_REG_READ:
            // status = Iss_Ar0331RegRead ( pObj, cmdArgs );
            break;

        case IOCTL_ISS_SENSOR_UPDATE_EXP_GAIN:
            status = Iss_Ar0331UpdateExpGain(pObj, cmdArgs);
//            Transplant_DRV_imgsSetEshutter();
		status = FVID2_SOK;
            break;

        case IOCTL_ISS_SENSOR_UPDATE_FRAMERATE:
		{
	        	Ptr createArgs;
	        	Iss_CaptFrameRate *framerateParams = (Iss_CaptFrameRate *)cmdArgs;
	        	createArgs = &(framerateParams->FrameRate);
			status = Iss_Ar0331FrameRateSet(pObj, createArgs, cmdStatusArgs);
		}
            break;

        case IOCTL_ISS_SENSOR_FRAME_RATE_SET:
            status = Iss_Ar0331FrameRateSet(pObj, cmdArgs, cmdStatusArgs);
            break;

		case IOCTL_ISS_SENSOR_UPDATE_ITT:
            status = Iss_Ar0331UpdateItt(pObj, cmdArgs);
            break;
        case IOCTL_ISS_SENSOR_PWM_CONFIG:
        {
		Iss_SensorPwmParm *pPwmParm = (Iss_SensorPwmParm *)cmdArgs;
			if( pPwmParm->Id == ISS_SENSOR_PWM_VIDEO )
			{
			Iss_Ar0331PwmVideoSet(pPwmParm->period, pPwmParm->duty);
			}else if( pPwmParm->Id == ISS_SENSOR_PWM_DC ){
				Iss_Ar0331PwmDcSet(pPwmParm->period, pPwmParm->duty);
			}

		break;
		}
	/*	case IOCTL_ISS_SENSOR_SET_VIDEO_MODE:
		{
			Iss_SensorVideoModeParams *modePrms = (Iss_SensorVideoModeParams *)cmdArgs;

			if (NULL != modePrms)
			{
				status = Iss_Ar0331SetMode(pObj, (FVID2_Standard)modePrms->standard, AR_0331_ADDR, 1);
			}
			else
			{
				status = FVID2_EBADARGS;
			}

			if (FVID2_SOK == status)
				pObj->createArgs.InputStandard = modePrms->standard;

			break;
		}*/
        default:
            status = FVID2_EUNSUPPORTED_CMD;
            break;
    }

    /*
     * unlock handle
     */
    Iss_Ar0331UnlockObj(pObj);

    return status;
}

/*
 * Create API that gets called when FVID2_create is called
 *
 * This API does not configure the AR0331is any way.
 *
 * This API - validates parameters - allocates driver handle - stores create
 * arguments in its internal data structure.
 *
 * Later the create arguments will be used when doing I2C communcation with
 * AR0331
 *
 * drvId - driver ID, must be FVID2_ISS_VID_DEC_AR0331_DRV instanceId - must
 * be 0 createArgs - create arguments createStatusArgs - create status
 * fdmCbParams - NOT USED, set to NULL
 *
 * returns NULL in case of any error */
/* ===================================================================
 *  @func     Iss_Ar0331Create
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
Fdrv_Handle Iss_Ar0331Create(UInt32 drvId,
                             UInt32 instanceId,
                             Ptr createArgs,
                             Ptr createStatusArgs,
                             const FVID2_DrvCbParams * fdmCbParams)
{
    Iss_Ar0331Obj *pObj;
    Iss_SensorCreateParams *sensorCreateArgs
        = (Iss_SensorCreateParams *) createArgs;

    Iss_SensorCreateStatus *sensorCreateStatus
        = (Iss_SensorCreateStatus *) createStatusArgs;

    /*
     * check parameters
     */
    if (sensorCreateStatus == NULL) {
        return NULL;
}
    sensorCreateStatus->retVal = FVID2_SOK;

    if (instanceId != 0 || sensorCreateArgs == NULL)
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
    pObj = Iss_Ar0331AllocObj();
    if (pObj == NULL)
    {
        sensorCreateStatus->retVal = FVID2_EALLOC;
        return NULL;
    }

    Iss_Ar0331Lock();

    // Transplant_AR0331_HDR_Enable(0);
    Iss_Ar0331Unlock();
    /*
     * copy parameters to allocate driver handle
     */
    memcpy(&pObj->createArgs, sensorCreateArgs, sizeof(*sensorCreateArgs));

    Iss_Ar0331ResetRegCache(pObj);

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
 *  @func     Iss_Ar0331Delete
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
Int32 Iss_Ar0331Delete(Fdrv_Handle handle, Ptr deleteArgs)
{
    Iss_Ar0331Obj *pObj = (Iss_Ar0331Obj *) handle;

    if (pObj == NULL)
        return FVID2_EBADARGS;

    /*
     * free driver handle object
     */
    Iss_Ar0331FreeObj(pObj);

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
int AR0331_Transplant_DRV_imgsSetRegs()
{

	int status;
	int i = 0;

	int j;
	gAr0331_I2c_ctrl.i2cInstId = Iss_platformGetI2cInstId();
	Int32 devAddr = AR_0331_ADDR;

	Uint16 *regAddr = gAr0331_I2c_ctrl.regAddr;

	Uint16 *regValue = gAr0331_I2c_ctrl.regValue;

	regAddr[i] = 0x301A; regValue[i] = 0x0001; i++;  	// RESET_REGISTER
	regAddr[i] = 0x301A; regValue[i] = 0x10D8; i++;  	// RESET_REGISTER

	//[AR0331 Linear sequencer load - 1.0]
	regAddr[i] = 0x3088; regValue[i] = 0x8000; i++;  	// SEQ_CTRL_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4A03; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4316; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x0443; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1645; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4045; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x6017; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5045; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x404B; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1244; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x6134; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4A31; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4342; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4560; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2714; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x3DFF; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x3DFF; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x3DEA; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2704; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x3D10; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2705; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x3D10; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2715; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x3527; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x053D; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1045; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4027; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x0427; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x143D; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0xFF3D; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0xFF3D; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0xEA62; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2728; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x3627; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x083D; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x6444; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2C2C; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2C2C; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4B01; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x432D; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4643; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1647; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x435F; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4F50; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2604; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2694; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2027; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0xFC53; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x0D5C; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x0D57; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5417; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x0955; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5649; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5307; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5303; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4D28; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x6C4C; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x0928; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2C28; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x294E; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5C09; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4500; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4580; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x26B6; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x27F8; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1702; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x27FA; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5C0B; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1718; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x26B2; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5C03; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1744; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x27F2; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1702; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2809; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1710; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1628; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x084D; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1A26; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x9316; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1627; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0xFA45; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0xA017; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x0727; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0xFB17; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2945; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x8017; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x0827; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0xFA17; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x285D; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x170E; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2691; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5301; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1740; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5302; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1710; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2693; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2692; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x484D; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4E28; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x094C; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x0B17; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5F27; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0xF217; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1428; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x0816; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4D1A; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1616; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x27FA; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2603; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5C01; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4540; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2798; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x172A; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4A0A; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4316; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x0B43; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x279C; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4560; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1707; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x279D; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1725; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4540; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1708; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2798; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5D53; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x0D26; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x455C; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x014B; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1244; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5251; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1702; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x6018; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4A03; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4316; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x0443; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1658; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4316; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5943; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x165A; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4316; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x5B43; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4540; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x279C; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4560; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1707; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x279D; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1725; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x4540; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1710; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2798; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1720; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x224B; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x1244; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2C2C; i++;  	// SEQ_DATA_PORT
	regAddr[i] = 0x3086; regValue[i] = 0x2C2C; i++;  	// SEQ_DATA_PORT

	regAddr[i] = 0x301A; regValue[i] = 0x0058; i++;  	// RESET_REGISTER
	regAddr[i] = 0x30B0; regValue[i] = 0x0000; i++;  	// DIGITAL_TEST
	regAddr[i] = 0x30BA; regValue[i] = 0x06EC; i++;  	// DIGITAL_CTRL
	regAddr[i] = 0x31AC; regValue[i] = 0x0C0C; i++; 		//DATA_FORMAT_BITS = 3084
	//PLL_settings - 4 Lane 12-bit HiSPi

	regAddr[i] = 0x302A; regValue[i] = PLL_P2 ;	i++;// VT_PIX_CLK_DIV
	regAddr[i] = 0x302C; regValue[i] = PLL_P1 ;	i++;// VT_SYS_CLK_DIV
	regAddr[i] = 0x302E; regValue[i] = PLL_pre_div ;	i++;// PRE_PLL_CLK_DIV
	regAddr[i] = 0x3030; regValue[i] = PLL_M ; 	i++;// PLL_MULTIPLIER
	regAddr[i] =0x3036; regValue[i] = 0x000C;  i++;  // OP_PIX_CLK_DIV
	regAddr[i] = 0x3038; regValue[i] = OP_SYS_CLK_DIV; 	i++;// OP_SYS_CLK_DIV
	regAddr[i] =0x3002; regValue[i] = 0x00E4;  i++;  // Y_ADDR_START
	regAddr[i] =0x3004; regValue[i] = 0x0042;  i++;  // X_ADDR_START
	regAddr[i] =0x3006; regValue[i] = 0x0523;  i++;  // Y_ADDR_END
	regAddr[i] = 0x3008; regValue[i] = 0x07c9; i++;  //regAddr[i] = 0x3008; regValue[i] = 0x07C0; i++; 	// X_ADDR_END
	regAddr[i] =0x300A; regValue[i] = 0x0465;  i++;  // FRAME_LENGTH_LINES
	regAddr[i] =0x300C; regValue[i] = 0x044C;  i++;  // LINE_LENGTH_PCK
	regAddr[i] =0x3012; regValue[i] = 0x0438;  i++;  // COARSE_INTEGRATION_TIME
	regAddr[i] =0x30A2; regValue[i] = 0x0001;  i++;  // X_ODD_INC
	regAddr[i] =0x30A6; regValue[i] = 0x0001;  i++;  // Y_ODD_INC

	regAddr[i] =0x3040; regValue[i] = 0x0000;  i++;  // READ_MODE
	//regAddr[i] =0x3040; regValue[i] = 0x0000;  i++;  // READ_MODE

	//Linear Mode
	regAddr[i] =0x3082; regValue[i] = 0x0009;  i++;  // OPERATION_MODE_CTRL
	//2D motion compensation OFF
	regAddr[i] =0x318C; regValue[i] = 0x0000;  i++;  // HDR_MC_CTRL2
	regAddr[i] =0x3190; regValue[i] = 0x0000;  i++;  // HDR_MC_CTRL4
	regAddr[i] =0x30FE; regValue[i] = 0x0080;  i++;  // NOISE_PEDESTAL
	regAddr[i] =0x320A; regValue[i] = 0x0080;  i++;  // ADACD_PEDESTAL
	//ALTM Bypassed
	regAddr[i] =0x301A; regValue[i] = 0x0058;  i++;  // RESET_REGISTER
	regAddr[i] =0x2400; regValue[i] = 0x0003;  i++;  // ALTM_CONTROL
	regAddr[i] =0x2450; regValue[i] = 0x0000;  i++;  // ALTM_OUT_PEDESTAL
	regAddr[i] =0x301E; regValue[i] = 0x00A8;  i++;  // DATA_PEDESTAL
	regAddr[i] =0x301A; regValue[i] = 0x005C;  i++;  // RESET_REGISTER
	//ADACD Disabled
	regAddr[i] =0x3200; regValue[i] = 0x0000;  i++;  // ADACD_CONTROL
	regAddr[i] =0x31D0; regValue[i] = 0x0000;  i++;  // COMPANDING
	regAddr[i] =0x31E0; regValue[i] = 0x0200;  i++;  // PIX_DEF_ID
	//regAddr[i] =0x3060; regValue[i] = 0x0030;  i++;  // ANALOG_GAIN

	//regAddr[i] =0x305E; regValue[i] = 0x04FF;  i++;  // Digital_GAIN , by windsor
	//Disable Embedded Data and Stats
	regAddr[i] =0x3064; regValue[i] = 0x1802;  i++;  // SMIA_TEST
	regAddr[i] =0x31AE; regValue[i] = 0x0304;  i++;  // SERIAL_FORMAT
	regAddr[i] = 0x31C6; regValue[i] = 0x8400; 	i++; // HISPI_CONTROL_STATUS
	regAddr[i] =0x306E; regValue[i] = 0x9010;  i++;  // DATAPATH_SELECT
	//Analog Settings
	regAddr[i] =0x3180; regValue[i] = 0x8089;  i++;  // DELTA_DK_CONTROL
	regAddr[i] =0x30F4; regValue[i] = 0x4000;  i++;  // ADC_COMMAND3_HS
	regAddr[i] =0x3ED4; regValue[i] = 0x8F6C;  i++;  // DAC_LD_8_9
	regAddr[i] =0x3ED6; regValue[i] = 0x6666;  i++;  // DAC_LD_10_11
	regAddr[i] =0x3EDA; regValue[i] = 0x8899;  i++;  // DAC_LD_14_15
	regAddr[i] =0x3EE6; regValue[i] = 0x00F0;  i++;  // DAC_LD_26_27
	regAddr[i] =0x3ED2; regValue[i] = 0x9F46;  i++;  // DAC_LD_6_7
	regAddr[i] =0x301A; regValue[i] = 0x005C;  i++;  // RESET_REGISTER


    gAr0331_I2c_ctrl.numRegs = 1;
    for (j = 0; j < i; j++)
    {
	status =
		Iss_deviceWrite16(gAr0331_I2c_ctrl.i2cInstId, devAddr,
				&gAr0331_I2c_ctrl.regAddr[j],
				&gAr0331_I2c_ctrl.regValue[j],
				gAr0331_I2c_ctrl.numRegs);
        if (status != FVID2_SOK)
        {
            Vps_printf("I2C write Error,index:%d\n",j);
            //return status;
        }

        /*
         * if(status != FVID2_SOK) { //printf("I2C write
         * Error,index:%d\n",j); return status; } */
	if (gAr0331_I2c_ctrl.regAddr[j] == 0x301A)
	{
		/*TODO: DM38X expects more delay in I2C compare to DM8127
		 * This delay works good for both. Need to root cause.
		 */
		Task_sleep(200);
	}
	else
	{
		Task_sleep(10);
	}

    }

    Vps_printf("DONE %s:%s:%d !!!\n", __FILE__, __FUNCTION__, __LINE__);
    return FVID2_SOK;
}

/* ===================================================================
 *  @func     Transplant_AR0331_HDR_Enable
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
int Transplant_AR0331_HDR_Enable(int enable)
{
    Uint16 regAddr[32];

    Uint16 regValue[32];

    int status = 0;

    int i = 0;

    int j = 0;

//    int delay = 100000;

    gAr0331_I2c_ctrl.i2cInstId = Iss_platformGetI2cInstId();
    Int32 devAddr = AR_0331_ADDR;

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

    gAr0331_I2c_ctrl.numRegs = 1;
    for (j = 0; j < i; j++)
    {
        status =
            Iss_deviceWrite16(gAr0331_I2c_ctrl.i2cInstId, devAddr, &regAddr[j],
                              &regValue[j], gAr0331_I2c_ctrl.numRegs);

        if (status != FVID2_SOK)
	{
		// printf("I2C write Error,index:%d\n",j);
		return status;
	}
	if (regAddr[j] == 0x301A)
	{
		Task_sleep(200);
	}


    }

    return status;
}


/* All register config for the camera sensor */
int ISS_Ar0331LVDS324_SetReg()
{
	unsigned char lvdsRegAddr[12];
	unsigned char lvdsRegValue[12];

	gAr0331_I2c_ctrl.i2cInstId = Iss_platformGetI2cInstId();
	int ii=0;

	Iss_deviceRead8(gAr0331_I2c_ctrl.i2cInstId,0x2d,lvdsRegAddr,lvdsRegValue, 8);

	for(ii=0;ii<12;ii++)
	{
		lvdsRegAddr[ii]=ii;
		lvdsRegValue[ii]=0;
		Iss_deviceRead8(gAr0331_I2c_ctrl.i2cInstId,0x2d,&lvdsRegAddr[ii],&lvdsRegValue[ii], 1);
		Task_sleep(100);
	}

	lvdsRegAddr[0]=0x09; lvdsRegValue[0]=0x2c;
	lvdsRegAddr[1]=0x0A; lvdsRegValue[1]=0x22;
	Task_sleep(100);
	Iss_deviceWrite8(gAr0331_I2c_ctrl.i2cInstId,0x2d,&lvdsRegAddr[0],&lvdsRegValue[0], 1);
	Task_sleep(100);
	Iss_deviceWrite8(gAr0331_I2c_ctrl.i2cInstId,0x2d,&lvdsRegAddr[1],&lvdsRegValue[1], 1);
	Task_sleep(100);
	Iss_deviceRead8(gAr0331_I2c_ctrl.i2cInstId,0x2d,&lvdsRegAddr[0],&lvdsRegValue[1], 1);
	Task_sleep(100);
	Iss_deviceRead8(gAr0331_I2c_ctrl.i2cInstId,0x2d,&lvdsRegAddr[1],&lvdsRegValue[1], 1);
	Task_sleep(100);
	lvdsRegValue[0]=0x0;
	lvdsRegValue[1]=0x0;
	Iss_deviceRead8(gAr0331_I2c_ctrl.i2cInstId,0x2d,&lvdsRegAddr[0],&lvdsRegValue[0], 1);
	Task_sleep(100);
	Iss_deviceRead8(gAr0331_I2c_ctrl.i2cInstId,0x2d,&lvdsRegAddr[1],&lvdsRegValue[1], 1);
	Task_sleep(100);
	return FVID2_SOK;
}

/*
 * System init for AR0331 driver
 *
 * This API - create semaphore locks needed - registers driver to FVID2
 * sub-system - gets called as part of Iss_deviceInit() */

/* ===================================================================
 *  @func     Iss_Ar0331Init
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
Int32 Iss_Ar0331Init()
{
    Semaphore_Params semParams;

    Int32 status = FVID2_SOK;
    /*
     * Set to 0's for global object, descriptor memory
     */
    memset(&gIss_Ar0331CommonObj, 0, sizeof(gIss_Ar0331CommonObj));

    /*
     * Create global AR0331 lock
     */
    Semaphore_Params_init(&semParams);

    semParams.mode = Semaphore_Mode_BINARY;

    gIss_Ar0331CommonObj.lock = Semaphore_create(1u, &semParams, NULL);

    if (gIss_Ar0331CommonObj.lock == NULL)
        status = FVID2_EALLOC;


    if (status == FVID2_SOK)
    {
    	ISS_Ar0331LVDS324_SetReg();

	AR0331_Transplant_DRV_imgsSetRegs();

        Iss_Ar0331PwmVideoSet(83320, 83320);
        Iss_Ar0331PwmDcSet(83320, 41660);

        gIss_Ar0331CommonObj.fvidDrvOps.create =
            (FVID2_DrvCreate) Iss_Ar0331Create;
        gIss_Ar0331CommonObj.fvidDrvOps.delete = Iss_Ar0331Delete;
        gIss_Ar0331CommonObj.fvidDrvOps.control = Iss_Ar0331Control;
        gIss_Ar0331CommonObj.fvidDrvOps.queue = NULL;
        gIss_Ar0331CommonObj.fvidDrvOps.dequeue = NULL;
        gIss_Ar0331CommonObj.fvidDrvOps.processFrames = NULL;
        gIss_Ar0331CommonObj.fvidDrvOps.getProcessedFrames = NULL;
        gIss_Ar0331CommonObj.fvidDrvOps.drvId = FVID2_ISS_SENSOR_AR0331_DRV;
        status = FVID2_registerDriver(&gIss_Ar0331CommonObj.fvidDrvOps);

        if (status != FVID2_SOK)
        {
            /*
             * Error - free acquired resources
             */
            Semaphore_delete(&gIss_Ar0331CommonObj.lock);
        }
    }

    if (status != FVID2_SOK)
    {
        Vps_printf(" ERROR %s:%s:%d !!!\n", __FILE__, __FUNCTION__, __LINE__);
    }

    return status;
}

/*
 * System de-init for AR0331 driver
 *
 * This API - de-registers driver from FVID2 sub-system - delete's allocated
 * semaphore locks - gets called as part of Iss_deviceDeInit() */
/* ===================================================================
 *  @func     Iss_Ar0331DeInit
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
Int32 Iss_Ar0331DeInit()
{
    /*
     * Unregister FVID2 driver
     */
    FVID2_unRegisterDriver(&gIss_Ar0331CommonObj.fvidDrvOps);

    /*
     * Delete semaphore's
     */
    Semaphore_delete(&gIss_Ar0331CommonObj.lock);

    return 0;
}

/* ===================================================================
 *  @func     Iss_Ar0331PinMux
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
Int32 Iss_Ar0331PinMux()
{
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
    return 0;
}

/*
 * Handle level lock */
/* ===================================================================
 *  @func     Iss_Ar0331LockObj
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
Int32 Iss_Ar0331LockObj(Iss_Ar0331Obj * pObj)
{
    Semaphore_pend(pObj->lock, BIOS_WAIT_FOREVER);

    return FVID2_SOK;
}

/*
 * Handle level unlock */
/* ===================================================================
 *  @func     Iss_Ar0331UnlockObj
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
Int32 Iss_Ar0331UnlockObj(Iss_Ar0331Obj * pObj)
{
    Semaphore_post(pObj->lock);

    return FVID2_SOK;
}

/*
 * Global driver level lock */

/* ===================================================================
 *  @func     Iss_Ar0331Lock
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
Int32 Iss_Ar0331Lock()
{
    Semaphore_pend(gIss_Ar0331CommonObj.lock, BIOS_WAIT_FOREVER);

    return FVID2_SOK;
}

/*
 * Global driver level unlock */
/* ===================================================================
 *  @func     Iss_Ar0331Unlock
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
Int32 Iss_Ar0331Unlock()
{
    Semaphore_post(gIss_Ar0331CommonObj.lock);

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
 *  @func     Iss_Ar0331AllocObj
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
Iss_Ar0331Obj *Iss_Ar0331AllocObj()
{
    UInt32 handleId;

    Iss_Ar0331Obj *pObj;

    Semaphore_Params semParams;

    UInt32 found = FALSE;

    /*
     * Take global lock to avoid race condition
     */
    Iss_Ar0331Lock();

    /*
     * find a unallocated object in pool
     */
    for (handleId = 0; handleId < ISS_DEVICE_MAX_HANDLES; handleId++)
    {

        pObj = &gIss_Ar0331CommonObj.Ar0331Obj[handleId];

        if (pObj->state == ISS_AR0331_OBJ_STATE_UNUSED)
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
            pObj->state = ISS_AR0331_OBJ_STATE_IDLE;
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
                pObj->state = ISS_AR0331_OBJ_STATE_UNUSED;
            }

            break;
        }
    }

    /*
     * Release global lock
     */
    Iss_Ar0331Unlock();

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
 *  @func     Iss_Ar0331FreeObj
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
Int32 Iss_Ar0331FreeObj(Iss_Ar0331Obj * pObj)
{
    /*
     * take global lock
     */
    Iss_Ar0331Lock();

    if (pObj->state != ISS_AR0331_OBJ_STATE_UNUSED)
    {
        /*
         * mark state as unused
         */
        pObj->state = ISS_AR0331_OBJ_STATE_UNUSED;

        /*
         * delete object locking semaphore
         */
        Semaphore_delete(&pObj->lock);
    }

    /*
     * release global lock
     */
    Iss_Ar0331Unlock();

    return FVID2_SOK;
}

Uint32 Iss_Ar0331GainCalc(int gain)
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

	return (Uint32)GainSend;
}

Int32 Iss_Ar0331UpdateExpGain(Iss_Ar0331Obj * pObj, Ptr createArgs)
{
    Int32 status = FVID2_SOK;
	Int32 count = 0;
	Int32 aewbVendor;


    UInt32 i2cInstId = Iss_platformGetI2cInstId();

    Int32 devAddr = AR_0331_ADDR;

    gAr0331_I2c_ctrl.i2cInstId = Iss_platformGetI2cInstId();
    devAddr = AR_0331_ADDR;

    /*
     * take global lock
     */
    Iss_Ar0331Lock();
	count = 0;
	aewbVendor = IssAlg_capt2AGetAEWBVendor();

	if( aewbVendor == 1 )
	{
		if (ti2a_output_params.mask)
		{
			unsigned int GainSend = Iss_Ar0331GainCalc(ti2a_output_params.sensorGain);
			gAr0331_I2c_ctrl.regAddr[count] = AR0331_ANALOG_GAIN;
			gAr0331_I2c_ctrl.regValue[count] =  (GainSend&0xFF);
			count++;

			gAr0331_I2c_ctrl.regAddr[count] = AR0331_DIGITAL_GAIN;
			gAr0331_I2c_ctrl.regValue[count] =  ((GainSend>>8)&0xFFFF);
			count++;
			//Vps_printf (" sensorGain = 0x%x \r\n", ti2a_output_params.sensorGain);
		}

		if (ti2a_output_params.mask)
		{
			gAr0331_I2c_ctrl.regAddr[count] = AR0331_COARSE_IT_TIME_A;
			gAr0331_I2c_ctrl.regValue[count] =  ti2a_output_params.sensorExposure;
			count++;
			//Vps_printf (" sensorExposure = %d \r\n", ti2a_output_params.sensorExposure);
		}


		gAr0331_I2c_ctrl.numRegs = count;
	}
  else if(aewbVendor == 2)
  {
		if (ti2a_output_params.mask & 3)
		{
			//Ar0331_Transplant_DRV_imgsSetEshutter();
			Uint16 shutter = (Uint16)((float) ti2a_output_params.sensorExposure / loc_RealRowTime);
			//Uint16 shutter = ti2a_output_params.sensorExposure / AR0331_ROW_TIME;
			Uint16 aGain   = AR0331_GainTableMap(ti2a_output_params.sensorGain);



			gAr0331_I2c_ctrl.regAddr[count]  = AR0331_ANALOG_GAIN;
			gAr0331_I2c_ctrl.regValue[count] =  aGain;
			count++;
			//Vps_printf (" sensorGain = %d, reg value 0x%x \r\n", ti2a_output_params.sensorGain,aGain);

			gAr0331_I2c_ctrl.regAddr[count]  = AR0331_COARSE_IT_TIME_A;
			gAr0331_I2c_ctrl.regValue[count] =  shutter;
			count++;


			//Vps_printf (" sensorExposure = %d, reg value %d\r\n", ti2a_output_params.sensorExposure,shutter);
    }
    gAr0331_I2c_ctrl.numRegs = count;
  }

    status = Iss_deviceWrite16(gAr0331_I2c_ctrl.i2cInstId, devAddr,
    		&gAr0331_I2c_ctrl.regAddr[0],
    		&gAr0331_I2c_ctrl.regValue[0],
    		gAr0331_I2c_ctrl.numRegs);


    /*
     * release global lock
     */
    Iss_Ar0331Unlock();

    return status;
}

Int32 Iss_Ar0331FrameRateSet(Iss_Ar0331Obj * pObj, Ptr createArgs,
                              Ptr cmdStatusArgs)
{
    Int32 status = FVID2_SOK;

    UInt32 i2cInstId = Iss_platformGetI2cInstId();

    Int32 devAddr = AR_0331_ADDR;

    Int32 count = 0;

    Int32 framerate = *(Int32 *) createArgs;

    gAr0331_I2c_ctrl.i2cInstId = Iss_platformGetI2cInstId();
    devAddr = AR_0331_ADDR;

    if (framerate == 60)
    {
	loc_RealRowTime=AR0331_ROW_TIME/2;

	gAr0331_I2c_ctrl.regAddr[count] = 0x300C;
	gAr0331_I2c_ctrl.regValue[count] = 0x44C;
        count++;
        *(Int32 *) cmdStatusArgs = (1100 *PLL_MULTIPLIER)/0x4a;
    }
    else if (framerate == 50)
    {
	loc_RealRowTime=AR0331_ROW_TIME*3/5;
        gAr0331_I2c_ctrl.regAddr[count] = 0x300C;
        gAr0331_I2c_ctrl.regValue[count] = 0x528;
        count++;
        *(Int32 *) cmdStatusArgs = (1320 *PLL_MULTIPLIER)/0x4a;
    }
    else if (framerate == 30)
    {
	loc_RealRowTime=AR0331_ROW_TIME;
        gAr0331_I2c_ctrl.regAddr[count] = 0x300C;
        gAr0331_I2c_ctrl.regValue[count] = (0x898 *PLL_MULTIPLIER)/0x4a;
        count++;
        *(Int32 *) cmdStatusArgs = (2200 *PLL_MULTIPLIER)/0x4a;
    }
    else if (framerate == 25)
    {
	loc_RealRowTime=AR0331_ROW_TIME*30/25;
        gAr0331_I2c_ctrl.regAddr[count] = 0x300C;
        gAr0331_I2c_ctrl.regValue[count] = (0xA50 *PLL_MULTIPLIER)/0x4a;
        count++;
        *(Int32 *) cmdStatusArgs = (2640 *PLL_MULTIPLIER)/0x4a;
    }

    else
    {
        status = FVID2_EFAIL;
        return status;
    }
    /** take global lock **/
    Iss_Ar0331Lock();

    gAr0331_I2c_ctrl.numRegs = count;
    status =
        Iss_deviceWrite16(gAr0331_I2c_ctrl.i2cInstId, devAddr,
                          &gAr0331_I2c_ctrl.regAddr[0],
                          &gAr0331_I2c_ctrl.regValue[0],
                          gAr0331_I2c_ctrl.numRegs);

    /** release global lock **/
    Iss_Ar0331Unlock();

    return status;
}

/*
 * Update ITT Values */
/* ===================================================================
 *  @func     Iss_Ar0331UpdateItt
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
Int32 Iss_Ar0331UpdateItt(Iss_Ar0331Obj * pObj,
                           Itt_RegisterParams * gItt_RegisterParams)
{
    int status = 0;

    int devAddr, count = 0;

    I2c_Ctrl gAr0331_I2c_ctrl;

    gAr0331_I2c_ctrl.i2cInstId = Iss_platformGetI2cInstId();
    devAddr = AR_0331_ADDR;
    if (gItt_RegisterParams->Control == 1)
    {
        gAr0331_I2c_ctrl.regAddr[count] = gItt_RegisterParams->regAddr;
        gAr0331_I2c_ctrl.regValue[count] = gItt_RegisterParams->regValue;
        gAr0331_I2c_ctrl.numRegs = 1;
        status =
            Iss_deviceWrite16(gAr0331_I2c_ctrl.i2cInstId, devAddr,
                              &gAr0331_I2c_ctrl.regAddr[count],
                              &gAr0331_I2c_ctrl.regValue[count],
                              gAr0331_I2c_ctrl.numRegs);
    }
    else if (gItt_RegisterParams->Control == 0)
    {
        gAr0331_I2c_ctrl.regAddr[count] = gItt_RegisterParams->regAddr;
        gAr0331_I2c_ctrl.regValue[count] = 0;
        gAr0331_I2c_ctrl.numRegs = 1;
        status =
            Iss_deviceRead16(gAr0331_I2c_ctrl.i2cInstId, devAddr,
                             &gAr0331_I2c_ctrl.regAddr[count],
                             &gAr0331_I2c_ctrl.regValue[count],
                             gAr0331_I2c_ctrl.numRegs);
        count = 0;
        gItt_RegisterParams->regValue = gAr0331_I2c_ctrl.regValue[count];
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
Uint32 Iss_Ar0331PwmVideoSet(Uint32 load_value, Uint32 match_value)
{
	Uint32	TimerID = 6;
	Uint32  *TimerLoadReg = (Uint32  *)(TimerBaseAddr(TimerID)+0x40);
	Uint32  *TimerMatchReg = (Uint32  *)(TimerBaseAddr(TimerID)+0x4C);

	*TimerLoadReg = DM_TIMER_LOAD_MIN-load_value;
	*TimerMatchReg = DM_TIMER_LOAD_MIN-(load_value-match_value);

	return 0;
}


Uint32 Iss_Ar0331PwmDcSet(Uint32 load_value, Uint32 match_value)
{
	Uint32	TimerID = 7;
	Uint32  *TimerLoadReg = (Uint32  *)(TimerBaseAddr(TimerID)+0x40);
	Uint32  *TimerMatchReg = (Uint32  *)(TimerBaseAddr(TimerID)+0x4C);

	*TimerLoadReg = DM_TIMER_LOAD_MIN-load_value;
	*TimerMatchReg = DM_TIMER_LOAD_MIN-(load_value-match_value);

	return 0;
}

Int32 Iss_Ar0331SetMode(Iss_Ar0331Obj *pObj, FVID2_Standard standard, UInt32 devAddr, UInt32 doReset)
{
    Int32 status = FVID2_SOK;
	UInt16 count = 0;

    //Iss_Ar0331Stop(pObj);

    Uint16 *regAddr = gAr0331_I2c_ctrl.regAddr;
    Uint16 *regValue = gAr0331_I2c_ctrl.regValue;

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
		gAr0331_I2c_ctrl.numRegs = 1;
        for (count = 0; count < 14; count++)
        {
			status = Iss_deviceWrite16(gAr0331_I2c_ctrl.i2cInstId, devAddr,
                                      &gAr0331_I2c_ctrl.regAddr[count],
                                      &gAr0331_I2c_ctrl.regValue[count],
                                      gAr0331_I2c_ctrl.numRegs);
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
		gAr0331_I2c_ctrl.numRegs = 1;
        for (count = 0; count < 14; count++)
        {
			status = Iss_deviceWrite16(gAr0331_I2c_ctrl.i2cInstId, devAddr,
                                      &gAr0331_I2c_ctrl.regAddr[count],
                                      &gAr0331_I2c_ctrl.regValue[count],
                                      gAr0331_I2c_ctrl.numRegs);
			if (regAddr[count] == 0x301A)
			{
				Task_sleep(500);
			}
        }
        break;
    }

    return (status);
}

