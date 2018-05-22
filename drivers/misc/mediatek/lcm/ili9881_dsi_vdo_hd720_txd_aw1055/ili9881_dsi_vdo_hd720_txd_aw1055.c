/*
 * Reverser and Author : Yuvraj Saxena
 */
#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mach/mt_gpio.h>
#endif
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
#define LCM_ID										0x9881

#define LCM_DSI_CMD_MODE								0

#define FRAME_WIDTH									(720)
#define FRAME_HEIGHT									(1280)
#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      						        0xFF

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)									(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)				lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)					lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)												lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)								lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)												lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)							lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)    

struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] = {
        {0xFF, 3,{0x98,0x81,0x07}},
	{0x03,1,{0x20}},
	{0x04,1,{0x06}},
	{0x05,1,{0x00}},
	{0x06,1,{0x01}},
	{0x07,1,{0x00}},
	{0x08,1,{0x00}},
	{0x09,1,{0x00}},
	{0x0A,1,{0x01}},
	{0x0B,1,{0x64}},
	{0x0C,1,{0x00}},
	{0x0D,1,{0x00}},
	{0x0E,1,{0x69}},
	{0x0F,1,{0x00}},
	{0x10,1,{0x40}},
	{0x11,1,{0x02}},
	{0x12,1,{0x05}},
	{0x13,1,{0x00}},
	{0x14,1,{0x00}},
	{0x15,1,{0x00}},
	{0x16,1,{0x64}},
	{0x17,1,{0x64}},
	{0x18,1,{0x00}},
	{0x19,1,{0x00}},
	{0x1A,1,{0x00}},
	{0x1B,1,{0x50}},
	{0x1C,1,{0xBC}},
	{0x1D,1,{0x0C}},
	{0x1E,1,{0x00}},
	{0x1F,1,{0x00}},
	{0x20,1,{0x00}},
	{0x21,1,{0x00}},
	{0x22,1,{0x00}},
	{0x23,1,{0x80}},
	{0x24,1,{0x30}},
	{0x25,1,{0x00}},
	{0x26,1,{0x00}},
	{0x27,1,{0x03}},

	{0x30,1,{0x01}},
	{0x31,1,{0x23}},
	{0x32,1,{0x45}},
	{0x33,1,{0x67}},
	{0x34,1,{0x89}},
	{0x35,1,{0xab}},
	{0x36,1,{0x01}},
	{0x37,1,{0x23}},
	{0x38,1,{0x45}},
	{0x39,1,{0x67}},
	{0x3A,1,{0x89}},
	{0x3B,1,{0xab}},
	{0x3C,1,{0xcd}},
	{0x3D,1,{0xef}},

	{0x50,1,{0x11}},
	{0x51,1,{0x06}},
	{0x52,1,{0x0c}},
	{0x53,1,{0x0d}},
	{0x54,1,{0x0e}},
	{0x55,1,{0x0f}},
	{0x56,1,{0x02}},
	{0x57,1,{0x02}},
	{0x58,1,{0x02}},
	{0x59,1,{0x02}},
	{0x5A,1,{0x02}},
	{0x5B,1,{0x02}},
	{0x5C,1,{0x02}},
	{0x5D,1,{0x02}},
	{0x5E,1,{0x02}},
	{0x5F,1,{0x02}},
	{0x60,1,{0x05}},
	{0x61,1,{0x05}},
	{0x62,1,{0x05}},
	{0x63,1,{0x02}},
	{0x64,1,{0x01}},
	{0x65,1,{0x00}},
	{0x66,1,{0x08}},
	{0x67,1,{0x08}},
	{0x68,1,{0x0c}},
	{0x69,1,{0x0d}},
	{0x6A,1,{0x0e}},
	{0x6B,1,{0x0f}},
	{0x6C,1,{0x02}},
	{0x6D,1,{0x02}},
	{0x6E,1,{0x02}},
	{0x6F,1,{0x02}},
	{0x70,1,{0x02}},
	{0x71,1,{0x02}},
	{0x72,1,{0x02}},
	{0x73,1,{0x02}},
	{0x74,1,{0x02}},
	{0x75,1,{0x02}},
	{0x76,1,{0x05}},
	{0x77,1,{0x05}},
	{0x78,1,{0x05}},
	{0x79,1,{0x02}},
	{0x7A,1,{0x01}},
	{0x7B,1,{0x00}},
	{0x7C,1,{0x06}},

	{0xFF, 3,{0x98,0x81,0x08}},
	{0x76,1,{0xB4}},
	{0x78,1,{0x02}},		//VGL pumping ratio 2x	     
	{0x74,1,{0x2B}},		//VGH clamp 15V
	{0x8E,1,{0x15}},		//VGL clamp -10V
	{0x40,1,{0x01}},
	{0x84,1,{0x81}},		//RF improve
	{0x72,1,{0x25}},
	{0xE3,1,{0x45}},		//Performance enhancement
	{0x7D,1,{0xCB}},
	{0x7E,1,{0x49}},
	{0x49,1,{0x10}},

	{0xFF, 3,{0x98,0x81,0x01}},
	{0x22,1,{0x0A}},		//BGR, SS
	{0x53,1,{0x82}},		//VCOM1
	{0x55,1,{0x85}},		//VCOM2
	{0x50,1,{0xB9}},         	//VREG1OUT=4.7V
	{0x51,1,{0xBA}},        	//VREG2OUT=-4.7V
	{0x31,1,{0x00}},		//column inversion
	{0xA0,1,{0x00}},		//VP255	Gamma P
	{0xA1,1,{0x14}},               //VP251        
	{0xA2,1,{0x2B}},               //VP247        
	{0xA3,1,{0x16}},               //VP243        
	{0xA4,1,{0x1C}},               //VP239        
	{0xA5,1,{0x2E}},               //VP231        
	{0xA6,1,{0x18}},               //VP219        
	{0xA7,1,{0x20}},               //VP203        
	{0xA8,1,{0x78}},               //VP175        
	{0xA9,1,{0x20}},               //VP144        
	{0xAA,1,{0x2A}},               //VP111        
	{0xAB,1,{0x83}},               //VP80         
	{0xAC,1,{0x25}},               //VP52   //06      
	{0xAD,1,{0x1B}},               //VP36         
	{0xAE,1,{0x54}},               //VP24         
	{0xAF,1,{0x2B}},               //VP16         
	{0xB0,1,{0x2B}},               //VP12         
	{0xB1,1,{0x4F}},               //VP8          
	{0xB2,1,{0x67}},               //VP4          
	{0xB3,1,{0x39}},               //VP0                      
	{0xC0,1,{0x00}},		//VN255 GAMMA N
	{0xC1,1,{0x14}},               //VN251        
	{0xC2,1,{0x2B}},               //VN247        
	{0xC3,1,{0x16}},               //VN243        
	{0xC4,1,{0x1C}},               //VN239        
	{0xC5,1,{0x2E}},               //VN231        
	{0xC6,1,{0x18}},               //VN219        
	{0xC7,1,{0x20}},               //VN203        
	{0xC8,1,{0x7B}},               //VN175        
	{0xC9,1,{0x20}},               //VN144        
	{0xCA,1,{0x2A}},               //VN111        
	{0xCB,1,{0x83}},               //VN80         
	{0xCC,1,{0x25}},               //VN52 //06        
	{0xCD,1,{0x1B}},               //VN36         
	{0xCE,1,{0x54}},              //VN24         
	{0xCF,1,{0x2B}},               //VN16         
	{0xD0,1,{0x2B}},               //VN12         
	{0xD1,1,{0x4F}},               //VN8          
	{0xD2,1,{0x67}},               //VN4          
	{0xD3,1,{0x39}},               //VN0  

	//CMD_Page 0
	{0xFF, 3,{0x98,0x81,0x00}},
	//{0x35,1,{0x00}},			//TE on
	{0x11,1,{0x00}},
	{REGFLAG_DELAY, 120, {}},
	{0x29,1,{0x00}},	
	{REGFLAG_DELAY, 20, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	{0xFF, 3,{0x98,0x81,0x00}},
	{0x28, 1, {0x00}},
	{REGFLAG_DELAY, 50, {}},
        {0x10, 1, {0x00}},
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
			//MDELAY(2);
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

        params->dsi.word_count = 2160;
        params->dsi.vertical_backporch = 9;
        params->dsi.vertical_frontporch = 15;
        params->dsi.horizontal_sync_active = 20;
        params->dsi.horizontal_backporch = 150;
        params->dsi.horizontal_frontporch = 78;
        params->dsi.horizontal_blanking_pixel = 60;
        params->dsi.PLL_CLOCK = 216;
        params->dsi.ssc_range = 8;
        params->type = 2;
        params->dsi.data_format.format = 2;
        params->dsi.intermediat_buffer_num = 2;
        params->dsi.PS = 2;
        params->width = 720;
        params->dsi.horizontal_active_pixel = 720;
        params->height = 1280;
        params->dsi.vertical_active_line = 1280;
        params->dbi.te_mode = 1;
        params->dsi.mode = 1;
        params->dbi.te_edge_polarity = 0;
        params->dsi.data_format.color_order = 0;
        params->dsi.data_format.trans_seq = 0;
        params->dsi.data_format.padding = 0;
        params->dsi.ssc_disable = 0;
        params->dsi.LANE_NUM = 4;
        params->dsi.vertical_sync_active = 4;
}

static void lcm_init(void)
{
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(120);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
 
}

static void lcm_suspend(void)
{
        push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);

	SET_RESET_PIN(1);
        MDELAY(1);
        SET_RESET_PIN(0);
        MDELAY(5);
        SET_RESET_PIN(1);
        MDELAY(20);
}

static void lcm_resume(void)
{
	lcm_init();
}

static unsigned int lcm_compare_id(void)
{
    return 1;
}

LCM_DRIVER ili9881_dsi_vdo_hd720_txd_aw1055_lcm_drv = 
{
	.name				= "ili9881_dsi_vdo_hd720_txd_aw1055",
	.set_util_funcs			= lcm_set_util_funcs,
	.compare_id			= lcm_compare_id,
	.get_params			= lcm_get_params,
	.init				= lcm_init,
	.suspend			= lcm_suspend,
	.resume         		= lcm_resume,
};
