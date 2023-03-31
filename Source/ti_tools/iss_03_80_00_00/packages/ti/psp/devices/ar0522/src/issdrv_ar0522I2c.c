/** ==================================================================
 *  @file   issdrv_ar0522I2c.c                                                  
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

//static Int32 g_ar0522_currentmode_rowtime_10ns;

/* ===================================================================
 *  @func     ar_0522_time_to_rows                                               
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
//static Int32 ar_0522_time_to_rows(Int32 time_us);

Int32 Iss_Ar0522I2cRead8(Iss_Ar0522Obj * pObj,
                         UInt32 i2cInstId, UInt32 i2cDevAddr,
                         const UInt8 * regAddr,
                         UInt8 * regValue, UInt32 numRegs)
{
    Int32 status = FVID2_SOK;

#ifdef ISS_AR0522_USE_REG_CACHE
    UInt32 reg, port;

    if (pObj->createArgs.deviceI2cAddr[0] == i2cDevAddr)
        port = 0;
    else
        port = 1;

    for (reg = 0; reg < numRegs; reg++)
    {
        regValue[reg] = pObj->regCache[port][regAddr[reg]];
    }
#else
    status = Iss_deviceRead8(i2cInstId, i2cDevAddr, regAddr, regValue, numRegs);

    if (status != FVID2_SOK)
        return status;
#endif

    return status;
}

Int32 Iss_Ar0522I2cWrite8(Iss_Ar0522Obj * pObj,
                          UInt32 i2cInstId, UInt32 i2cDevAddr,
                          const UInt8 * regAddr,
                          UInt8 * regValue, UInt32 numRegs)
{
    Int32 status = FVID2_SOK;

#ifdef ISS_AR0522_USE_REG_CACHE
    UInt32 reg, port;

    if (pObj->createArgs.deviceI2cAddr[0] == i2cDevAddr)
        port = 0;
    else
        port = 1;

    for (reg = 0; reg < numRegs; reg++)
    {
        pObj->regCache[port][regAddr[reg]] = regValue[reg];
    }
#endif

    status = Iss_deviceWrite8(i2cInstId, i2cDevAddr,
                              regAddr, regValue, numRegs);

    if (status != FVID2_SOK)
        return status;

    return status;
}

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
Int32 Iss_Ar0522ResetRegCache(Iss_Ar0522Obj * pObj)
{
#ifdef ISS_AR0522_USE_REG_CACHE

    memset(pObj->regCache, 0, sizeof(pObj->regCache));

    // for now hard code default values in registers 0x0 to 0x7
    pObj->regCache[0][0] = 0x01;
    pObj->regCache[0][1] = 0x00;
    pObj->regCache[0][2] = 0x35;
    pObj->regCache[0][3] = 0x91;
    pObj->regCache[0][4] = 0x03;
    pObj->regCache[0][5] = 0x00;
    pObj->regCache[0][6] = 0x1F;                           // assume signal
                                                           // is detected
    pObj->regCache[0][7] = 0x00;

#endif

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     Iss_Ar0522UpdateRegCache                                               
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
Int32 Iss_Ar0522UpdateRegCache(Iss_Ar0522Obj * pObj)
{
    Int32 status = FVID2_SOK;

#ifdef ISS_AR0522_USE_REG_CACHE

    Iss_SensorCreateParams *pCreateArgs;

    UInt8 numRegs = 0xf8;

    UInt8 devAddr[2];

    UInt8 regAddr[8];

    UInt8 regValue[8];

    pCreateArgs = &pObj->createArgs;

    devAddr[0] = pCreateArgs->deviceI2cAddr[0];
    devAddr[1] = pCreateArgs->deviceI2cAddr[0] + ISS_AR0522_I2C_PORT1_OFFSET;

    Iss_Ar0522ResetRegCache(pObj);

    regAddr[0] = 0;
    regAddr[1] = 1;

    // dummy read to make next read io work
    status = Iss_deviceRead8(pCreateArgs->deviceI2cInstId,
                             devAddr[0], regAddr, regValue, 2);
    if (status != FVID2_SOK)
        return status;

    status = Iss_deviceRawRead8(pCreateArgs->deviceI2cInstId,
                                devAddr[0], &pObj->regCache[0][8], numRegs);
    if (status != FVID2_SOK)
        return status;

    status = Iss_deviceRawRead8(pCreateArgs->deviceI2cInstId,
                                devAddr[1], &pObj->regCache[1][8], numRegs);

    if (status != FVID2_SOK)
        return status;

#endif

    return status;
}

/* 
 * This API - Disable output - Power-ON of all module - Manual reset of
 * AR0522 and then setup in auto reset mode - Select default TMDS core */
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
Int32 Iss_Ar0522Reset(Iss_Ar0522Obj * pObj)
{
    Int32 status = FVID2_SOK;

#if 0
    UInt32 mask, addr, delay;
    UInt32 val = 0;

    // Set CAM_RESET on GP0.16 high by setting OE reg to input
    // then delay
    // and finally set GP0.16 to low with OE reg set to output

	// Reset AR0522 sensor
	mask = (1<<16);			// CAM_RESET is output
	addr = GPIO0_OE_REG;
//	val = __raw_readl(addr);
//	val = REG32(GPIO0_OE_REG);
	val |= mask;			// Make CAM_RESET (GP0.16) pin an input

        Vps_printf("<<<<<<< %s: Line %d: val= 0x%08X, mask= 0x%08X, addr=  0x%08X >>>>>>>\n",
	    __FUNCTION__, __LINE__,
	    val, mask, addr );

//	__raw_writel(val, addr);	// Pin gets pulled up to do sensor reset
//	REG32(GPIO0_OE_REG) = val;

        delay = 100000;

        while (delay--)
        {
        }

//	__raw_writel((1<<16), GPIO0_CLRDATA);	// Clear CAM_RESET (GP0.16) in GPIO_DATAOUT
//	REG32(GPIO0_CLRDATA) = mask;
	val &= ~mask;
//	__raw_writel(val, addr);	// Make CAM_RESET (GP0.16) pin an output, driving pin low
//	REG32(GPIO0_OE_REG) = val;

#endif

    return status;
}

/* 
 * Power-ON All modules */
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
Int32 Iss_Ar0522PowerDown(Iss_Ar0522Obj * pObj, UInt32 powerDown)
{
    Int32 status = FVID2_SOK;

    // Following is N/A for ar0522 CSIS  6/28/22 DAT
    // TODO:  may want to provide this feature for CSIS
    // Note that this func is commented out in 'Iss_Ar0522Reset' above

#if 0

    Iss_SensorCreateParams *pCreateArgs;

    UInt8 regAddr[8];

    UInt8 regValue[8];

    UInt8 numRegs;

    UInt8 devAddr;

    pCreateArgs = &pObj->createArgs;

    devAddr = pCreateArgs->deviceI2cAddr[0];

    numRegs = 0;
    regAddr[numRegs] = ISS_AR0522_REG_SYS_CTRL_1;
    regValue[numRegs] = 0;
    numRegs++;

    status = Iss_Ar0522I2cRead8(pObj,
                                pCreateArgs->deviceI2cInstId,
                                devAddr, regAddr, regValue, numRegs);

    if (status != FVID2_SOK)
        return FVID2_ETIMEOUT;

    if (powerDown)
        regValue[0] = 0x04;                                /* power down */
    else
        regValue[0] = 0x05;                                /* normal
                                                            * operation */

    status = Iss_Ar0522I2cWrite8(pObj, pCreateArgs->deviceI2cInstId,
                                 devAddr, regAddr, regValue, numRegs);

    if (status != FVID2_SOK)
        return FVID2_ETIMEOUT;

#endif

    return status;
}

/* 
 * Enable output port */
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
Int32 Iss_Ar0522OutputEnable(Iss_Ar0522Obj * pObj, UInt32 enable)
{
    Int32 status = FVID2_SOK;

    // Following is N/A for ar0522 CSIS  6/28/22 DAT
    // Note that this func is commented out in 'Iss_Ar0522Start' and 'Iss_Ar0522Stop' above

#if 0

    Iss_SensorCreateParams *pCreateArgs;

    UInt8 regAddr[8];

    UInt8 regValue[8];

    UInt8 numRegs;

    UInt8 devAddr;

    pCreateArgs = &pObj->createArgs;

	// TODO:  verify the use of 'port1' re: I2C
    devAddr = pCreateArgs->deviceI2cAddr[0] + ISS_AR0522_I2C_PORT1_OFFSET;

	// TODO:  note values set in these regs -- need corresponding AR0522 usage
	numRegs = 0;
    regAddr[numRegs] = ISS_AR0522_REG_SYS_PWR_DWN_2;
    regValue[numRegs] = 0xC3;                              /* Enable all
                                                            * expect outputs */
    if (enable)
        regValue[numRegs] |= (1 << 2);                     /* Enable outputs */

    regAddr[numRegs] = ISS_AR0522_REG_SYS_PWR_DWN;
    regValue[numRegs] = 0xAD;                              /* Enable all
                                                            * expect outputs */
    if (enable)
        regValue[numRegs] |= (1 << 6);                     /* Enable outputs */

    numRegs++;

    status = Iss_Ar0522I2cWrite8(pObj, pCreateArgs->deviceI2cInstId,
                                 devAddr, regAddr, regValue, numRegs);

    if (status != FVID2_SOK)
        return FVID2_ETIMEOUT;
#endif

    return status;
}

/* 
 * Stop output port */
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
Int32 Iss_Ar0522Stop(Iss_Ar0522Obj * pObj)
{
    Int32 status = FVID2_SOK;

    // status = Iss_Ar0522OutputEnable ( pObj, FALSE );

    return status;
}

/* 
 * Start output port */
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
Int32 Iss_Ar0522Start(Iss_Ar0522Obj * pObj)
{
    Int32 status = FVID2_SOK;

    // status = Iss_Ar0522OutputEnable ( pObj, TRUE );
    //g_ar0522_currentmode_rowtime_10ns = ((1000000 * 100) / 60) / 1080;

    return status;
}

/* ===================================================================
 *  @func     ar_0522_time_to_rows                                               
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
//static Int32 ar_0522_time_to_rows(Int32 time_us)
//{
//    if (time_us > 429496000)
//        return 0xffffffff;
//
//    return (ROUND(time_us * 100, g_ar0522_currentmode_rowtime_10ns));   // us_to_rows;
//}

/* 
 * Get Chip ID and revision ID */
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
                          Iss_SensorChipIdStatus * pStatus)
{
    Int32 status = FVID2_SOK;

    //Iss_SensorCreateParams *pCreateArgs;

    //UInt8 regAddr[8];

    //UInt8 regValue[8];

    //UInt8 numRegs;

    return status;
}

/* 
 * write to device registers */
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
                         Iss_VideoDecoderRegRdWrParams * pPrm)
{
    Int32 status = FVID2_SOK;

    //Iss_SensorCreateParams *pCreateArgs;

    return status;
}

/* 
 * read from device registers */
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
                        Iss_VideoDecoderRegRdWrParams * pPrm)
{
    Int32 status = FVID2_SOK ;

    // Following is N/A for ar0522 CSIS  6/28/22 DAT
    // Commented out in 'issdrv_ar0522Api.c'

#if 0
    Iss_SensorCreateParams *pCreateArgs;

    if (pPrm == NULL)
        return FVID2_EBADARGS;

    if (pPrm->regAddr == NULL || pPrm->regValue8 == NULL || pPrm->numRegs == 0)
        return FVID2_EBADARGS;

    pCreateArgs = &pObj->createArgs;

    if (pPrm->deviceNum > pCreateArgs->numDevicesAtPort)
        return FVID2_EBADARGS;

    memset(pPrm->regValue8, 0, pPrm->numRegs);

    status = Iss_Ar0522I2cRead8(pObj,
                                 pCreateArgs->deviceI2cInstId,
                                 pCreateArgs->deviceI2cAddr[pPrm->deviceNum],
                                 pPrm->regAddr, pPrm->regValue8, pPrm->numRegs);
#endif

    return status;

}
