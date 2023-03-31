/** ==================================================================
 *  @file   issdrv_ar0522Priv.h
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

#ifndef _ISS_AR0522_PRIV_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define _ISS_AR0522_PRIV_H_

#include <string.h>
#include <xdc/runtime/System.h>

#include <ti/psp/vps/common/trace.h>
#include <ti/psp/devices/ar0522/issdrv_ar0522.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include <ti/psp/iss/drivers/alg/2A/inc/issdrv_algTIaewb.h>

// For GPIO register read/write
//#define __raw_readl(a)      (*(volatile Uint32 *)(a))
//#define __raw_writel(v, a)  (*(volatile Uint32 *)(a) = (v))
#define REG32                           *(volatile unsigned int*)

#define TI81XX_GPIO0_BASE	0x48032000
#define TI81XX_GPIO1_BASE	0x4804C000
#define TI81XX_GPIO2_BASE	0x481AC000
#define TI81XX_GPIO3_BASE	0x481AE000

#define GPIO0_OE_REG		(TI81XX_GPIO0_BASE + 0x134)
#define GPIO0_CLRDATA		(TI81XX_GPIO0_BASE + 0x190)
#define GPIO0_SETDATA		(TI81XX_GPIO0_BASE + 0x194)

#define GPIO1_OE_REG		(TI81XX_GPIO1_BASE + 0x134)
#define GPIO1_CLRDATA		(TI81XX_GPIO1_BASE + 0x190)
#define GPIO1_SETDATA		(TI81XX_GPIO1_BASE + 0x194)

#define GPIO2_OE_REG		(TI81XX_GPIO2_BASE + 0x134)
#define GPIO2_CLRDATA		(TI81XX_GPIO2_BASE + 0x190)
#define GPIO2_SETDATA		(TI81XX_GPIO2_BASE + 0x194)

// #define ISS_AR0522_USE_REG_CACHE
// #define ISS_AR0522_USE_AVI_FRAME_INFO

/* Driver object state - NOT IN USE */
#define ISS_AR0522_OBJ_STATE_UNUSED   (0)

/* Driver object state - IN USE and IDLE */
#define ISS_AR0522_OBJ_STATE_IDLE     (1)

/* AR0522 I2C Port1 offset from Port0 */
#define ISS_AR0522_I2C_PORT1_OFFSET   (4)

/* AR0522 Default TMDS Core ID to use */
#define ISS_AR0522_CORE_ID_DEFAULT    (0)


#define     AR_0522_I2C_DEV_ADDR				(AR_0522_ADDR)
#define     AR_0522_I2C_CHANNEL				(1)
#define     AR_0522_MODEL_ID_REG_ADDR			(0x00000002)
#define     AR_0522_MODEL_ID_REG_VALUE			(0x2C01)

/* xtal frequency in Khz */
#define ISS_AR0522_XTAL_KHZ           (10000)

//#define PLL_M        49                                    // pll_multiplier
//#define PLL_pre_div  2                                     // pre_pll_clk_div
//#define PLL_P1       1                                     // vt_sys_clk_div
//#define PLL_P2       8                                     // vt_pix_clk_div

/* color space */
#define ISS_AR0522_AVI_INFO_COLOR_RGB444       (0)
#define ISS_AR0522_AVI_INFO_COLOR_YUV444       (1)
#define ISS_AR0522_AVI_INFO_COLOR_YUV422       (2)
#define AR0522_DIGITAL_GAIN                    0x305E
#define AR0522_ANALOG_GAIN                     0x3028	   // ar0331 0x3060, also different reg bits

#define AR0522_COARSE_IT_TIME_A	        0x3012
//#define ROW_TIME                        29.6297            // 27 //
#define ROW_TIME                        (29.6297) //15.43              // 27
                                                           // (2*LINE_LENGTH
                                                           // /
                                                           // OUT_CLK)//27.2us,two
                                                           // paths readout

/*
 * Driver handle object */
typedef struct {

    UInt32 state;                                          /* handle state */

    UInt32 handleId;                                       /* handle ID,
                                                            * 0..ISS_DEVICE_MAX_HANDLES-1
                                                            */

    Semaphore_Handle lock;                                 /* handle lock */

    Iss_SensorCreateParams createArgs;                     /* create time
                                                            * arguments */

    UInt8 regCache[2][256];                                /* register read
                                                            * cache */

} Iss_Ar0522Obj;

/*
 * Glabal driver object */
typedef struct {
    FVID2_DrvOps fvidDrvOps;                               /* FVID2 driver
                                                            * ops */

    Semaphore_Handle lock;                                 /* global driver
                                                            * lock */

    Iss_Ar0522Obj Ar0522Obj[ISS_DEVICE_MAX_HANDLES];
    /*
     * handle objects
     */

} Iss_Ar0522CommonObj;

extern Iss_Ar0522CommonObj gIss_Ar0522CommonObj;

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
                           Itt_RegisterParams * gItt_RegisterParams);
/* ===================================================================
 *  @func     Iss_Ar0522GetChipId
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
Int32 Iss_Ar0522GetChipId(Iss_Ar0522Obj * pObj,
                          Iss_SensorChipIdParams * pPrm,
                          Iss_SensorChipIdStatus * pStatus);

/* ===================================================================
 *  @func     Iss_Ar0522Reset
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
Int32 Iss_Ar0522Reset(Iss_Ar0522Obj * pObj);

/* ===================================================================
 *  @func     Iss_Ar0522Start
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
Int32 Iss_Ar0522Start(Iss_Ar0522Obj * pObj);

/* ===================================================================
 *  @func     Iss_Ar0522Stop
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
Int32 Iss_Ar0522Stop(Iss_Ar0522Obj * pObj);

/* ===================================================================
 *  @func     Iss_Ar0522RegWrite
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
Int32 Iss_Ar0522RegWrite(Iss_Ar0522Obj * pObj,
                         Iss_VideoDecoderRegRdWrParams * pPrm);

/* ===================================================================
 *  @func     Iss_Ar0522RegRead
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
Int32 Iss_Ar0522RegRead(Iss_Ar0522Obj * pObj,
                        Iss_VideoDecoderRegRdWrParams * pPrm);

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
Int32 Iss_Ar0522LockObj(Iss_Ar0522Obj * pObj);

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
Int32 Iss_Ar0522UnlockObj(Iss_Ar0522Obj * pObj);

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
Iss_Ar0522Obj *Iss_Ar0522AllocObj();

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
Int32 Iss_Ar0522FreeObj(Iss_Ar0522Obj * pObj);

/* ===================================================================
 *  @func     Iss_Ar0522PowerDown
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
Int32 Iss_Ar0522PowerDown(Iss_Ar0522Obj * pObj, UInt32 powerDown);

/* ===================================================================
 *  @func     Iss_Ar0522OutputEnable
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
Int32 Iss_Ar0522OutputEnable(Iss_Ar0522Obj * pObj, UInt32 enable);

/* ===================================================================
 *  @func     Iss_Ar0522ResetRegCache
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
Int32 Iss_Ar0522ResetRegCache(Iss_Ar0522Obj * pObj);

/* ===================================================================
 *  @func     Ar0522_GainTableMap
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
static int Ar0522_GainTableMap(int sensorGain);

/* ===================================================================
 *  @func     Iss_Ar0522UpdateExpGain
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
Int32 Iss_Ar0522UpdateExpGain(Iss_Ar0522Obj * pObj, Ptr createArgs);

/* ===================================================================
 *  @func     AR0522_GainTableMap
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
static Uint16 AR0522_GainTableMap(int again)
{
    /*
     * int Again = 0x1040; // default 1x if(sensorGain < 1000) { return
     * Again; } if(sensorGain < 2000) // 1x to 2x { Again = 0x1040 +
     * (((sensorGain - 1000) * 64) / 1000); return Again; } if(sensorGain <
     * 4000) // 2x to 4x { Again = 0x1840 + (((sensorGain - 2000) * 64) /
     * 2000); return Again; } if(sensorGain < 8000) // 4x to 8x { Again =
     * 0x1C40 + (((sensorGain - 4000) * 64) / 4000); return Again; }
     * if(sensorGain < 16000) // 8x to 16x { Again = 0x1CC0 + (((sensorGain -
     * 8000) * 64) / 8000); return Again; } if(sensorGain < 32000) // // 16x
     * to 32x { Again = 0x1DC0 + (((sensorGain - 16000) * 64) / 16000);
     * return Again; } return Again; */

    Uint16 regValue = 0x30;

    if (again >= 8000)
    {
        regValue = 0x30;
    }
    else if (again >= 6400)
    {
        regValue = 0x2C;
    }
    else if (again >= 5333)
    {
        regValue = 0x28;
    }
    else if (again >= 4560)
    {
        regValue = 0x24;
    }
    else if (again >= 4000)
    {
        regValue = 0x20;
    }
    else if (again >= 2666)
    {
        regValue = 0x18 + (((again - 2666) * 3) / 1333)*2;
    }
    else if (again >= 2000)
    {
        regValue = 0x10 + (((again - 2000) * 3) / 666)*2;
    }
    else if (again >= 1333)
    {
        regValue = 0x08 + ((again - 1333) * 7) / 666;
    }
    else
    {
        regValue = 0x00;
    }

    return regValue;

}

/* ===================================================================
 *  @func     Iss_Ar0522FrameRateSet
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
Int32 Iss_Ar0522FrameRateSet(Iss_Ar0522Obj * pObj, Ptr createArgs,
                              Ptr cmdStatusArgs);

Uint32 Iss_Ar0522PwmVideoSet(Uint32 load_value, Uint32 match_value);
Uint32 Iss_Ar0522PwmDcSet(Uint32 load_value, Uint32 match_value);
Int32 Iss_Ar0522SetMode(Iss_Ar0522Obj *pObj, FVID2_Standard standard, UInt32 devAddr, UInt32 doReset);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif                                                     /* _ISS_AR0522_PRIV_H_
                                                            */
