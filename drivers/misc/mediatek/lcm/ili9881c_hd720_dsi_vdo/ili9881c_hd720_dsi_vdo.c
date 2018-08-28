/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/


#ifdef BUILD_LK
#include "platform/mt_gpio.h"
#else
    #include <linux/string.h>
    #if defined(BUILD_UBOOT)
        #include <asm/arch/mt_gpio.h>
    #else
        #include <mt-plat/mt_gpio.h>
    #endif
#endif

#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1280)

#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0xFFF   // END OF REGISTERS MARKER

#define LCM_ID_ili9806c	0x8009

#define LCM_DSI_CMD_MODE									0

#ifndef TRUE
    #define   TRUE     1
#endif
 
#ifndef FALSE
    #define   FALSE    0
#endif

static unsigned int lcm_esd_test = FALSE; 

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

 struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};
static  int lcm_is_init=0xff; 
//extern int IMM_GetOneChannelValue(int dwChannel, int data[4], int* rawdata);

//static unsigned int  vcomadj=0x93;
static void init_lcm_registers(void)
{
	unsigned int data_array[16];

data_array[0] = 0x00043902;
data_array[1] = 0x7983FFB9;
dsi_set_cmdq(data_array, 2, 1);
MDELAY(2);

data_array[0] = 0x00113902;
data_array[1] = 0x161644B1;
data_array[2] = 0xD0503131;
data_array[3] = 0x388054EE;
data_array[4] = 0x2222F838;
data_array[5] = 0x00000022;
dsi_set_cmdq(data_array, 6, 1);
MDELAY(2);

data_array[0] = 0x000A3902;
data_array[1] = 0x0dFE80B2;
data_array[2] = 0x1150000a;
data_array[3] = 0x00001D42;
dsi_set_cmdq(data_array, 4, 1);
MDELAY(2);

data_array[0] = 0x000B3902;
data_array[1] = 0x027C02B4;
data_array[2] = 0x227C027C;
data_array[3] = 0x00862386;
dsi_set_cmdq(data_array, 4, 1);
MDELAY(2);

data_array[0] = 0x00023902;
data_array[1] = 0x000002CC;
dsi_set_cmdq(data_array, 2, 1);
MDELAY(2);

data_array[0] = 0x00023902;
data_array[1] = 0x000011D2;
dsi_set_cmdq(data_array, 2, 1);
MDELAY(2);

data_array[0] = 0x001E3902;
data_array[1] = 0x000700D3;
data_array[2] = 0x08081f3C;
data_array[3] = 0x00041032;
data_array[4] = 0x03700304;
data_array[5] = 0x00080070;
data_array[6] = 0x06333708;
data_array[7] = 0x06063706;
data_array[8] = 0x00000b37;
dsi_set_cmdq(data_array, 9, 1);
MDELAY(2);

data_array[0] = 0x00233902;
data_array[1] = 0x181919D5;
data_array[2] = 0x1b1a1a18;
data_array[3] = 0x0003021b;
data_array[4] = 0x04070601;
data_array[5] = 0x22212005;
data_array[6] = 0x18181823;
data_array[7] = 0x18181818;
data_array[8] = 0x18181818;
data_array[9] = 0x00000018;
dsi_set_cmdq(data_array, 10, 1);
MDELAY(2);

data_array[0] = 0x00213902;
data_array[1] = 0x191818D6;
data_array[2] = 0x1b1a1a19;
data_array[3] = 0x0502031b;
data_array[4] = 0x01060704;
data_array[5] = 0x21222300;
data_array[6] = 0x18181820;
data_array[7] = 0x18181818;
data_array[8] = 0x18181818;
data_array[9] = 0x00000018;
dsi_set_cmdq(data_array, 10, 1);
MDELAY(2);

data_array[0] = 0x002B3902;
data_array[1] = 0x070400E0;
data_array[2] = 0x1B230c0b;
data_array[3] = 0x0c0b082D;
data_array[4] = 0x17140f17;
data_array[5] = 0x13081615;
data_array[6] = 0x04001815;
data_array[7] = 0x230c0b07;
data_array[8] = 0x0b082D1B;
data_array[9] = 0x140f170c;
data_array[10] = 0x08161517;
data_array[11] = 0x00181513;
dsi_set_cmdq(data_array, 12, 1);
MDELAY(2);

data_array[0] = 0x00033902;
data_array[1] = 0x005656B6;
dsi_set_cmdq(data_array, 2, 1);
MDELAY(2);

data_array[0] = 0x00053902;
data_array[1] = 0x000000C7;
data_array[2] = 0x000000C0;
dsi_set_cmdq(data_array, 3, 1);
MDELAY(2);

data_array[0] = 0x00110500;
dsi_set_cmdq(data_array, 1, 1);
MDELAY(120);
data_array[0] = 0x00290500;
dsi_set_cmdq(data_array, 1, 1);
MDELAY(20);

}


static struct LCM_setting_table lcm_initialization_setting_dj[] = {
{0XFF, 3, {0X98, 0X81, 0X03}},
{0X1, 1, {0X00}},
{0X2, 1, {0X00}},
{0X3, 1, {0X73}},
{0X4, 1, {0X00}},
{0X5, 1, {0X00}},
{0X6, 1, {0X0C}},
{0X7, 1, {0X00}},
{0X8, 1, {0X00}},
{0X9, 1, {0X01}},
{0XA, 1, {0X01}},
{0XB, 1, {0X01}},
{0XC, 1, {0X01}},
{0XD, 1, {0X01}},
{0XE, 1, {0X01}},
{0XF, 1, {0X00}},
{0X10, 1, {0X00}},
{0X11, 1, {0X00}},
{0X12, 1, {0X00}},
{0X13, 1, {0X00}},
{0X14, 1, {0X00}},
{0X15, 1, {0X00}},
{0X16, 1, {0X00}},
{0X17, 1, {0X00}},
{0X18, 1, {0X00}},
{0X19, 1, {0X00}},
{0X1A, 1, {0X00}},
{0X1B, 1, {0X00}},
{0X1C, 1, {0X00}},
{0X1D, 1, {0X00}},
{0X1E, 1, {0X44}},
{0X1F, 1, {0XC0}},
{0X20, 1, {0X0A}},
{0X21, 1, {0X03}},
{0X22, 1, {0X0A}},
{0X23, 1, {0X00}},
{0X24, 1, {0X8C}},
{0X25, 1, {0X8C}},
{0X26, 1, {0X00}},
{0X27, 1, {0X00}},
{0X28, 1, {0X3B}},
{0X29, 1, {0X03}},
{0X2A, 1, {0X00}},
{0X2B, 1, {0X00}},
{0X2C, 1, {0X00}},
{0X2D, 1, {0X00}},
{0X2E, 1, {0X00}},
{0X2F, 1, {0X00}},
{0X30, 1, {0X00}},
{0X31, 1, {0X00}},
{0X32, 1, {0X00}},
{0X33, 1, {0X00}},
{0X34, 1, {0X00}},
{0X35, 1, {0X00}},
{0X36, 1, {0X00}},
{0X37, 1, {0X00}},
{0X38, 1, {0X00}},
{0X39, 1, {0X00}},
{0X3A, 1, {0X00}},
{0X3B, 1, {0X00}},
{0X3C, 1, {0X00}},
{0X3D, 1, {0X00}},
{0X3E, 1, {0X00}},
{0X3F, 1, {0X00}},
{0X40, 1, {0X00}},
{0X41, 1, {0X00}},
{0X42, 1, {0X00}},
{0X43, 1, {0X00}},
{0X44, 1, {0X00}},
{0X50, 1, {0X01}},
{0X51, 1, {0X23}},
{0X52, 1, {0X45}},
{0X53, 1, {0X67}},
{0X54, 1, {0X89}},
{0X55, 1, {0XAB}},
{0X56, 1, {0X01}},
{0X57, 1, {0X23}},
{0X58, 1, {0X45}},
{0X59, 1, {0X67}},
{0X5A, 1, {0X89}},
{0X5B, 1, {0XAB}},
{0X5C, 1, {0XCD}},
{0X5D, 1, {0XEF}},
{0X5E, 1, {0X11}},
{0X5F, 1, {0X0C}},
{0X60, 1, {0X0D}},
{0X61, 1, {0X0E}},
{0X62, 1, {0X0F}},
{0X63, 1, {0X06}},
{0X64, 1, {0X07}},
{0X65, 1, {0X02}},
{0X66, 1, {0X02}},
{0X67, 1, {0X02}},
{0X68, 1, {0X02}},
{0X69, 1, {0X02}},
{0X6A, 1, {0X02}},
{0X6B, 1, {0X02}},
{0X6C, 1, {0X02}},
{0X6D, 1, {0X02}},
{0X6E, 1, {0X02}},
{0X6F, 1, {0X02}},
{0X70, 1, {0X02}},
{0X71, 1, {0X02}},
{0X72, 1, {0X02}},
{0X73, 1, {0X01}},
{0X74, 1, {0X00}},
{0X75, 1, {0X0C}},
{0X76, 1, {0X0D}},
{0X77, 1, {0X0E}},
{0X78, 1, {0X0F}},
{0X79, 1, {0X06}},
{0X7A, 1, {0X07}},
{0X7B, 1, {0X02}},
{0X7C, 1, {0X02}},
{0X7D, 1, {0X02}},
{0X7E, 1, {0X02}},
{0X7F, 1, {0X02}},
{0X80, 1, {0X02}},
{0X81, 1, {0X02}},
{0X82, 1, {0X02}},
{0X83, 1, {0X02}},
{0X84, 1, {0X02}},
{0X85, 1, {0X02}},
{0X86, 1, {0X02}},
{0X87, 1, {0X02}},
{0X88, 1, {0X02}},
{0X89, 1, {0X01}},
{0X8A, 1, {0X00}},
{0XFF, 3, {0X98, 0X81, 0X04}},
{0X0, 1, {0X00}},
{0X6C, 1, {0X15}},
{0X6E, 1, {0X19}},
{0X6F, 1, {0XA5}},
{0X3A, 1, {0X24}},
{0X35, 1, {0X1F}},
{0X8D, 1, {0X1F}},
{0X87, 1, {0XBA}},
{0X26, 1, {0X76}},
{0XB2, 1, {0XD1}},
{0X7A, 1, {0X10}},
{0XFF, 3, {0X98, 0X81, 0X01}},
{0X22, 1, {0X0A}},
{0X31, 1, {0X00}},
{0X42, 1, {0X33}},
{0X50, 1, {0XAF}},
{0X51, 1, {0XAF}},
{0X53, 1, {0X48}},
{0X55, 1, {0X48}},
{0X60, 1, {0X06}},
{0X61, 1, {0X01}},
{0X62, 1, {0X19}},
{0X63, 1, {0X10}},
{0XA0, 1, {0X08}},
{0XA1, 1, {0X14}},
{0XA2, 1, {0X1F}},
{0XA3, 1, {0X11}},
{0XA4, 1, {0X12}},
{0XA5, 1, {0X24}},
{0XA6, 1, {0X18}},
{0XA7, 1, {0X1C}},
{0XA8, 1, {0X6B}},
{0XA9, 1, {0X1C}},
{0XAA, 1, {0X27}},
{0XAB, 1, {0X63}},
{0XAC, 1, {0X19}},
{0XAD, 1, {0X16}},
{0XAE, 1, {0X4C}},
{0XAF, 1, {0X22}},
{0XB0, 1, {0X27}},
{0XB1, 1, {0X56}},
{0XB2, 1, {0X66}},
{0XB3, 1, {0X39}},
{0XC0, 1, {0X08}},
{0XC1, 1, {0X14}},
{0XC2, 1, {0X1F}},
{0XC3, 1, {0X11}},
{0XC4, 1, {0X12}},
{0XC5, 1, {0X24}},
{0XC6, 1, {0X18}},
{0XC7, 1, {0X1C}},
{0XC8, 1, {0X6B}},
{0XC9, 1, {0X1C}},
{0XCA, 1, {0X27}},
{0XCB, 1, {0X63}},
{0XCC, 1, {0X19}},
{0XCD, 1, {0X16}},
{0XCE, 1, {0X4C}},
{0XCF, 1, {0X22}},
{0XD0, 1, {0X27}},
{0XD1, 1, {0X56}},
{0XD2, 1, {0X66}},
{0XD3, 1, {0X39}},
{0XFF, 3, {0X98, 0X81, 0X00}},
{0X11, 1, {0X00}},
{REGFLAG_DELAY, 120, {}},
{0X29, 1, {0X00}},
{REGFLAG_DELAY, 10, {}},
{0X35, 1, {0X00}},
{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_set_window[] = {
	{0x2A,	4,	{0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
	{0x2B,	4,	{0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0xFF,5,{0xFF,0x98,0x06,0x04,0x00}},
	
	{0x11, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
	{0x29, 1, {0x00}},
	{REGFLAG_DELAY, 50, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {

	// Display off sequence
	//{0xFF,5,{0xFF,0x98,0x06,0x04,0x00}},
	
	{0x28, 0, {0x00}},
	{REGFLAG_DELAY, 50, {}},

    // Sleep Mode On
	{0x10, 0, {0x00}},
	{REGFLAG_DELAY, 120, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

    for(i = 0; i < count; i++) {
		
        unsigned cmd;
        cmd = table[i].cmd;
		
        switch (cmd) {
			
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
				
            case REGFLAG_END_OF_TABLE :
                break;
				
            default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
       	}
    }
	
}


// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{	
	memset(params, 0, sizeof(LCM_PARAMS));
	 
	 params->type	= LCM_TYPE_DSI;
	 
	 params->width	= FRAME_WIDTH;
	 params->height = FRAME_HEIGHT;

	
		// enable tearing-free
		//params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
	//params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
#if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
#else
		params->dsi.mode   = BURST_VDO_MODE;
#endif
	 // DSI
	 /* Command mode setting */
	 params->dsi.LANE_NUM				 = LCM_THREE_LANE;
	 
	 //The following defined the fomat for data coming from LCD engine.
	 params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	 params->dsi.data_format.trans_seq	 = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	 params->dsi.data_format.padding	 = LCM_DSI_PADDING_ON_LSB;
	 params->dsi.data_format.format    = LCM_DSI_FORMAT_RGB888;
	 
	 // Video mode setting		 
	 
	 params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
	 
               // params->dsi.ssc_disable = 1;  // ssc disable control (1: disable, 0: enable, default: 0)  add stone
               // params->dsi.ssc_range = 3;  // ssc range control (1:min, 8:max, default: 4)  add stone 2
	
	 params->dsi.vertical_sync_active				 = 4;//4
		params->dsi.vertical_backporch					= 18;//16;//14;20 stone
		params->dsi.vertical_frontporch					= 18;//16;20 stone
	 params->dsi.vertical_active_line				 = FRAME_HEIGHT; 
	 
		params->dsi.horizontal_sync_active				= 20;//6;//2;
		params->dsi.horizontal_backporch				= 120;//44;//44;//42;
		params->dsi.horizontal_frontporch				= 80;//44;90 stone
	 params->dsi.horizontal_active_pixel					 = FRAME_WIDTH;  
	 
	// Bit rate calculation
	/*params->dsi.pll_div1=1;	 // div1=0,1,2,3;div1_real=1,2,4,4
	params->dsi.pll_div2=1;	 // div2=0,1,2,3;div2_real=1,2,4,4
	params->dsi.fbk_sel=1; 	  // fbk_sel=0,1,2,3;fbk_sel_real=1,2,4,4
	params->dsi.fbk_div =32;//28	 30  //ox1c=28	 // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	 //18--28*/
	//	params->dsi.HS_PRPR=3;
	//params->dsi.CLK_HS_POST=22;
		params->dsi.HS_TRAIL = 12;

		params->dsi.PLL_CLOCK = 204;//dsi clock customization: should config clock value directly 220 stone 210
	//params->dsi.ssc_range=8;
                params->dsi.cont_clock=0;//add stone
	//PLL_CLOCK*2/()/()/2/24
	params->dsi.esd_check_enable = 1;
	params->dsi.customization_esd_check_enable = 1;
	params->dsi.lcm_esd_check_table[0].cmd = 0x0a;  
	params->dsi.lcm_esd_check_table[0].count = 1;
	params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c;  

}




static void lcm_init(void)
{

    SET_RESET_PIN(1);
    MDELAY(5); //20
    SET_RESET_PIN(0);
    MDELAY(10); //20
    SET_RESET_PIN(1);
    MDELAY(120);//150

//init_lcm_registers();
	#if defined(BUILD_LK) 
	 	printf("MYCAT hxili9881c, lcm_init\n");
	 #else
	 	printk("MYCAT hxili881c, lcm_init\n");
	#endif  
	push_table(lcm_initialization_setting_dj, sizeof(lcm_initialization_setting_dj) / sizeof(struct LCM_setting_table), 1);

}


static void lcm_suspend(void)
{

	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(120);

	//push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting)/sizeof(struct LCM_setting_table), 1);
}


static void lcm_resume(void)
{
	lcm_init();
//	push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);

}

static unsigned int lcm_compare_id(void)
{ 

	char data_array[4]; 
	int array[16];
	char buffer[5] = {0xFF,0x98,0x81,0x01}; 
	char id_high=0; 
	char id_low=0; 
	int id=0; 

	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(1);
	SET_RESET_PIN(1);
	MDELAY(10);

	dsi_set_cmdq_V2(0xFF, 5, buffer, 1);//enable to page1  

	array[0] = 0x00013700;  
	dsi_set_cmdq(array, 1, 1);  
	read_reg_v2(0x00, &data_array[0], 1);

	array[0] = 0x00013700; 
	dsi_set_cmdq(array, 1, 1); 
	read_reg_v2(0x01, &data_array[1], 1); 

	id_high = data_array[0]; 
	id_low = data_array[1]; 
	id = (id_high<<8) | id_low; 

#ifdef BUILD_LK 
	printf("%s, LK debug: ili9881c id = 0x%08x\n", __func__, id); 
#else 
	printk("%s, kernel debug: ili9881c id = 0x%08x\n", __func__, id); 
#endif 

	if(id == 0x9881)
		return 1;
	else
		return 0;

}

static unsigned int lcm_esd_check(void)
{
	char  buffer[3];
	int   array[4];
	#ifndef BUILD_LK
 printk("luning ---lcm_esd_check\n");

	if(lcm_esd_test)
	{
		lcm_esd_test = FALSE;
		return TRUE;
	}

	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0x0a, buffer, 1);
	if(buffer[0]==0x9c)
	{
	printk("buffer[0] == %d\n",buffer[0]);
		return FALSE;
	}
	else
	{	
	printk("buffer[0]aaaa == %d\n",buffer[0]);
		return FALSE;
	}
	//return FALSE;
		
 #endif

}

static unsigned int lcm_esd_recover(void)
{
	
	lcm_init();
//	lcm_resume();
	//printk("luning ---lcm_esd_recover\n");

	return TRUE;
}
	
LCM_DRIVER ili9881c_hd720_dsi_vdo_lcm_drv = 
{
	.name		= "ili9881c_hd720_dsi_vdo",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	//.compare_id    = lcm_compare_id,	
	//.esd_check = lcm_esd_check,
	//.esd_recover = lcm_esd_recover,
#if (LCM_DSI_CMD_MODE)
	//.set_backlight	= lcm_setbacklight,
	//.update         = lcm_update,
#endif
};
