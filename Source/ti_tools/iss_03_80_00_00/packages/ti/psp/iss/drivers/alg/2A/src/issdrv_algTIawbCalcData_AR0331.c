/** ==================================================================
 *  @file   issdrv_algTIawbCalcData.c                                                  
 *                                                                    
 *  @path    /proj/vsi/users/venu/DM812x/IPNetCam_rel_1_8/ti_tools/iss_02_bkup/packages/ti/psp/iss/drivers/alg/2A/src/                                                 
 *                                                                    
 *  @desc   This  File contains.                                      
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012                    
 *                                                                    
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/
#include <alg_ti_aewb_priv.h>

awb_calc_data_t awb_calc_data = {
   
// Sensor specific calibration data

   17,			// num_of_ref_1
   4,			// num_of_ref_2
   4,			// num_of_gray

   // color_temp_1[num_of_ref_1]
   {2850,3029,3207,3386,3564,3743,3921,4100,4280,4460,4640,4820,5000,5375,5750,6125,6500},

   // wbReferenceCb[num_of_ref_1][num_of_gray]
   {
       {-92,-92,-92,-92},
       {-91,-92,-90,-90},
       {-90,-90,-90,-88},
       {-89,-89,-88,-86},
       {-87,-87,-87,-85},
       {-86,-86,-85,-83},
       {-84,-84,-84,-80},
       {-83,-83,-82,-78},
       {-77,-76,-76,-71},
       {-71,-70,-69,-67},
       {-64,-64,-63,-60},
       {-57,-57,-56,-56},
       {-50,-49,-49,-48},
       {-43,-42,-41,-41},
       {-37,-37,-37,-35},
       {-31,-31,-30,-29},
       {-26,-26,-26,-23}
   },

   // wbReferenceCr[num_of_ref_1][num_of_gray]
   {
       {6,5,6,5},
       {1,0,1,1},
       {-5,-5,-5,-6},
       {-11,-11,-10,-10},
       {-18,-19,-16,-18},
       {-25,-25,-23,-23},
       {-32,-33,-30,-33},
       {-40,-40,-38,-39},
       {-41,-42,-40,-41},
       {-43,-43,-42,-42},
       {-45,-45,-42,-44},
       {-47,-47,-45,-45},
       {-49,-48,-48,-47},
       {-51,-51,-51,-51},
       {-54,-54,-54,-50},
       {-56,-56,-57,-53},
       {-59,-59,-59,-57}
   },

   // ref_gray_R_1[num_of_ref_1][num_of_gray]
   {
       {262,162,84,39},
       {247,153,79,37},
       {232,143,74,34},
       {217,134,69,32},
       {201,124,65,30},
       {186,115,60,28},
       {171,105,55,25},
       {156,96,50,23},
       {150,93,48,22},
       {144,89,46,22},
       {138,86,45,21},
       {132,82,43,21},
       {126,79,41,20},
       {131,82,42,20},
       {135,84,43,21},
       {140,87,44,21},
       {144,89,45,21}
   },

   // ref_gray_G_1[num_of_ref_1][num_of_gray]
   {
       {280,174,90,42},
       {275,171,88,41},
       {269,167,86,40},
       {264,164,84,39},
       {259,161,83,39},
       {254,158,81,38},
       {248,154,79,37},
       {243,151,77,36},
       {236,147,75,35},
       {229,142,73,35},
       {221,138,71,34},
       {214,133,69,34},
       {207,129,67,33},
       {219,136,70,34},
       {231,144,74,35},
       {242,151,77,36},
       {254,158,80,37}
   },

   // ref_gray_B_1[num_of_ref_1][num_of_gray]
   {
       {92,57,29,13},
       {91,56,29,13},
       {90,56,28,13},
       {89,55,28,13},
       {89,55,28,13},
       {88,54,28,13},
       {87,54,27,13},
       {86,53,27,13},
       {91,57,29,14},
       {97,60,31,15},
       {102,64,33,16},
       {108,67,35,17},
       {113,71,37,18},
       {129,81,42,20},
       {144,90,46,22},
       {160,100,51,24},
       {175,109,55,26}
   },

   // ref_index_2[num_of_ref_2]
   {0,7,12,16},

   // color_temp_2[num_of_ref_2]
   {2850,4100,5000,6500},

   // img_ref[NUM_OF_REF_2*1120]
   {
       //2850
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,1,2,2,
       	1,1,1,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	1,1,1,1,
       	1,3,5,4,
       	3,3,1,1,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,1,
       	1,3,6,7,
       	5,5,6,6,
       	5,3,2,3,
       	3,2,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,1,2,
       	2,5,12,18,
       	13,8,7,6,
       	5,3,2,4,
       	5,3,1,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,1,2,3,
       	4,8,16,24,
       	19,10,6,4,
       	3,1,1,3,
       	4,3,1,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,1,2,3,
       	5,8,14,21,
       	16,9,4,2,
       	1,0,0,1,
       	1,1,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,1,2,
       	3,6,9,12,
       	10,5,2,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,1,
       	2,4,5,6,
       	4,2,1,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	1,3,3,3,
       	2,1,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	1,2,2,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	1,2,2,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,1,2,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,1,1,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	

       //4100
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	1,2,2,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,1,1,1,
       	2,4,4,3,
       	2,1,1,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	1,1,2,2,
       	3,5,5,5,
       	4,2,1,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,1,
       	2,2,4,6,
       	6,5,6,6,
       	4,2,2,2,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,2,
       	4,4,8,14,
       	13,8,5,5,
       	3,3,4,4,
       	2,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,2,
       	4,6,11,19,
       	19,11,5,3,
       	2,3,5,4,
       	2,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,1,
       	2,5,10,17,
       	19,12,5,2,
       	1,2,3,3,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	1,3,7,12,
       	15,10,4,1,
       	1,1,1,1,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,2,5,7,
       	8,6,3,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,2,3,3,
       	3,2,1,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,1,2,1,
       	1,1,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,1,1,1,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,1,2,2,
       	2,1,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,1,1,2,
       	2,1,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,1,
       	1,1,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	

       //5000
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	1,2,2,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	1,1,1,0,
       	1,3,3,2,
       	2,2,1,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	1,2,1,1,
       	1,3,3,3,
       	3,3,2,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,1,
       	1,1,1,1,
       	1,3,3,3,
       	2,2,1,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,1,1,
       	1,2,2,2,
       	2,3,4,4,
       	2,1,1,2,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,1,2,
       	2,5,9,8,
       	6,5,5,5,
       	3,2,3,4,
       	2,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,1,2,
       	4,8,15,17,
       	12,7,5,3,
       	2,2,4,4,
       	2,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,1,3,
       	4,8,14,19,
       	15,8,4,2,
       	1,1,2,3,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,1,2,
       	4,6,10,14,
       	13,8,3,1,
       	0,1,1,2,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,1,
       	2,5,7,9,
       	9,6,2,0,
       	0,0,1,1,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	2,4,5,5,
       	4,3,1,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	1,3,2,2,
       	1,1,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	1,2,1,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,1,1,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	1,1,1,1,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	1,2,2,2,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	1,1,1,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,1,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	

       //6500
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,1,
       	2,2,2,1,
       	1,1,1,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	1,1,1,1,
       	3,4,3,2,
       	2,2,1,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,1,
       	2,2,1,1,
       	3,3,2,2,
       	1,1,1,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,1,1,
       	2,1,1,1,
       	2,3,2,1,
       	1,1,1,1,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,1,2,
       	1,1,1,1,
       	2,4,3,2,
       	1,1,2,2,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,1,1,
       	2,3,3,3,
       	4,6,5,3,
       	1,1,3,3,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,1,
       	4,8,10,9,
       	7,6,5,2,
       	1,2,3,3,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,1,3,
       	6,13,18,16,
       	9,4,2,1,
       	1,1,2,2,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,3,4,
       	6,11,16,15,
       	9,4,1,1,
       	1,1,2,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,3,4,
       	5,7,9,9,
       	7,3,1,0,
       	0,1,2,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,1,3,
       	5,6,5,4,
       	3,2,0,0,
       	0,1,1,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,1,2,
       	4,5,4,2,
       	1,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,1,2,
       	4,4,2,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,1,2,
       	2,2,1,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,1,
       	1,1,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	1,1,1,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,1,
       	2,2,2,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,1,
       	2,2,1,1,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,1,
       	1,1,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	

   },

   // referencesCb_2[num_of_ref_2][num_of_gray]
   {
       {-92,-92,-92,-91},
       {-82,-82,-82,-80},
       {-50,-49,-49,-45},
       {-26,-25,-25,-22}
   },

   // referencesCr_2[num_of_ref_2][num_of_gray]
   {
       {6,5,6,5},
       {-40,-40,-39,-40},
       {-49,-49,-47,-48},
       {-59,-59,-58,-60}
   },

   // ref_gray_R_2[num_of_ref_2][num_of_gray]
   {
       {262,162,84,39},
       {156,96,50,23},
       {126,79,41,20},
       {144,89,45,21}
   },

   // ref_gray_G_2[num_of_ref_2][num_of_gray]
   {
       {280,174,90,42},
       {243,151,77,36},
       {207,129,67,33},
       {254,158,80,37}
   },

   // ref_gray_B_2[num_of_ref_2][num_of_gray]
   {
       {92,57,29,13},
       {86,53,27,13},
       {113,71,37,18},
       {175,109,55,26}
   },

   
// AWB tuning parameters

   50,     // radius

   20,     // luma_awb_min
   1000,     // luma_awb_max

   3500,     // low_color_temp_thresh
   0,     // apply_rgb_adjust
   100,     // R_adjust(%)
   90,     // B_adjust(%)

   92,     // SB_1
   185,     // SB_2
   0,     // SB_low_bound

   6500,     // default_T_Hh
   5000,     // default_T_MH
   4100,     // default_T_ML
   2850,     // default_T_L

   16,		// default_T_Hh_index
   12,		// default_T_MH_index
   7,		// default_T_ML_index
   0,		// default_T_L_index

   1,     // best_gray_index_default

};
