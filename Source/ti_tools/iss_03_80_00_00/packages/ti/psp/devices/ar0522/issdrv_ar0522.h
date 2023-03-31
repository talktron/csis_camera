/** ==================================================================
 *  @file   issdrv_ar0522.h
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

#ifndef _ISSDRV_AR0522_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define _ISSDRV_AR0522_H_

#include <ti/psp/devices/iss_sensorDriver.h>
#include <ti/psp/vps/drivers/fvid2_drvMgr.h>

#define CTRL_MODULE_BASE_ADDR	(0x48140000u)


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
	Int32 Iss_Ar0522Init();

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
		const FVID2_DrvCbParams* fdmCbParams);

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
		UInt32 cmd, Ptr cmdArgs, Ptr cmdStatusArgs);

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
	Int32 Iss_Ar0522Delete(Fdrv_Handle handle, Ptr deleteArgs);

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
	Int32 Iss_Ar0522DeInit();

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
	Int32 Iss_Ar0522PinMux();

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
