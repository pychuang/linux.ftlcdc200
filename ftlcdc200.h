/*
 * Faraday FTLCDC200 LCD Controller
 *
 * (C) Copyright 2009 Faraday Technology
 * Po-Yu Chuang <ratbert@faraday-tech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __FTLCDC200_H
#define __FTLCDC200_H

#define FTLCDC200_OFFSET_CTRL		0x00
#define FTLCDC200_OFFSET_PIXEL		0x04
#define FTLCDC200_OFFSET_INT_ENABLE	0x08
#define FTLCDC200_OFFSET_INT_CLEAR	0x0c
#define FTLCDC200_OFFSET_INT_STATUS	0x10
#define FTLCDC200_OFFSET_SCALE		0x14
#define FTLCDC200_OFFSET_FRAME_BASE0	0x18
#define FTLCDC200_OFFSET_FRAME_BASE1	0x24
#define FTLCDC200_OFFSET_FRAME_BASE2	0x30
#define FTLCDC200_OFFSET_FRAME_BASE3	0x3c
#define FTLCDC200_OFFSET_PATGEN		0x48
#define FTLCDC200_OFFSET_FIFO_THRESHOLD	0x4c
#define FTLCDC200_OFFSET_GPIO_CONTROL	0x50
#define FTLCDC200_OFFSET_HTIMING	0x100
#define FTLCDC200_OFFSET_VTIMING0	0x104
#define FTLCDC200_OFFSET_VTIMING1	0x108
#define FTLCDC200_OFFSET_POLARITY	0x10c
#define FTLCDC200_OFFSET_SERIAL		0x200
#define FTLCDC200_OFFSET_CCIR656	0x204
#define FTLCDC200_OFFSET_PIP		0x300
#define FTLCDC200_OFFSET_PIP_POS1	0x304
#define FTLCDC200_OFFSET_PIP_DIM1	0x308
#define FTLCDC200_OFFSET_PIP_POS2	0x30c
#define FTLCDC200_OFFSET_PIP_DIM2	0x310
#define FTLCDC200_OFFSET_CM0		0x400
#define FTLCDC200_OFFSET_CM1		0x404
#define FTLCDC200_OFFSET_CM2		0x408
#define FTLCDC200_OFFSET_CM3		0x40c
#define FTLCDC200_OFFSET_GAMMA_R	0x600
#define FTLCDC200_OFFSET_GAMMA_G	0x700
#define FTLCDC200_OFFSET_GAMMA_B	0x800
#define FTLCDC200_OFFSET_PALETTE	0xa00	/* 0xa00  - 0xbfc */
#define FTLCDC200_OFFSET_SCALE_IN_HRES	0x1100
#define FTLCDC200_OFFSET_SCALE_IN_VRES	0x1104
#define FTLCDC200_OFFSET_SCALE_OUT_HRES	0x1108
#define FTLCDC200_OFFSET_SCALE_OUT_VRES	0x110c
#define FTLCDC200_OFFSET_MISC_CTRL	0x1110
#define FTLCDC200_OFFSET_HHT		0x1114
#define FTLCDC200_OFFSET_HLT		0x1118
#define FTLCDC200_OFFSET_VHT		0x111c
#define FTLCDC200_OFFSET_VLT		0x1120
#define FTLCDC200_OFFSET_SCALE_PAR	0x112c
#ifdef	CONFIG_FTLCDC200_COMPLEX_OSD
/*
 * Complex OSD configuration
 */
#define FTLCDC200_OFFSET_OSD_ENABLE	0x2000
#define FTLCDC200_OFFSET_OSD_FONT0	0x2004
#define FTLCDC200_OFFSET_OSD_FONT1	0x2008
#define FTLCDC200_OFFSET_OSD_W1C1	0x2010
#define FTLCDC200_OFFSET_OSD_W1C2	0x2014
#define FTLCDC200_OFFSET_OSD_W2C1	0x201c
#define FTLCDC200_OFFSET_OSD_W2C2	0x2020
#define FTLCDC200_OFFSET_OSD_W3C1	0x2028
#define FTLCDC200_OFFSET_OSD_W3C2	0x202c
#define FTLCDC200_OFFSET_OSD_W4C1	0x2034
#define FTLCDC200_OFFSET_OSD_W4C2	0x2038
#define FTLCDC200_OFFSET_OSD_MCFB	0x203c
#define FTLCDC200_OFFSET_OSD_PC0	0x2040
#define FTLCDC200_OFFSET_OSD_PC1	0x2044
#define FTLCDC200_OFFSET_OSD_PC2	0x2048
#define FTLCDC200_OFFSET_OSD_PC3	0x204c
#define FTLCDC200_OFFSET_OSD_PC4	0x2050
#define FTLCDC200_OFFSET_OSD_PC5	0x2054
#define FTLCDC200_OFFSET_OSD_PC6	0x2058
#define FTLCDC200_OFFSET_OSD_PC7	0x205c
#define FTLCDC200_OFFSET_OSD_PC8	0x2060
#define FTLCDC200_OFFSET_OSD_PC9	0x2064
#define FTLCDC200_OFFSET_OSD_PC10	0x2068
#define FTLCDC200_OFFSET_OSD_PC11	0x206c
#define FTLCDC200_OFFSET_OSD_PC12	0x2070
#define FTLCDC200_OFFSET_OSD_PC13	0x2074
#define FTLCDC200_OFFSET_OSD_PC14	0x2078
#define FTLCDC200_OFFSET_OSD_PC15	0x207c
#define FTLCDC200_OFFSET_OSD_FONTRAM	0x10000	/* 0x10000 - 0x18ffc */
#define FTLCDC200_OFFSET_OSD_DISPLAYRAM	0x20000	/* 0x20000 - 0x207fc */
#else	/* CONFIG_FTLCDC200_COMPLEX_OSD */
/*
 * Simple OSD Configuration
 */
#define FTLCDC200_OFFSET_OSD_SDC	0x2000
#define FTLCDC200_OFFSET_OSD_POS	0x2004
#define FTLCDC200_OFFSET_OSD_FG		0x2008
#define FTLCDC200_OFFSET_OSD_BG		0x200c
#define FTLCDC200_OFFSET_OSD_FONT	0x8000	/* 0x8000 - 0xbffc */
#define FTLCDC200_OFFSET_OSD_ATTR	0xc000	/* 0xc000 - 0xc7fc */
#endif	/* CONFIG_FTLCDC200_COMPLEX_OSD */

/*
 * LCD Function Enable
 */
#define FTLCDC200_CTRL_ENABLE		(1 << 0)
#define FTLCDC200_CTRL_LCD		(1 << 1)
#define FTLCDC200_CTRL_YUV420		(1 << 2)
#define FTLCDC200_CTRL_YUV		(1 << 3)
#define FTLCDC200_CTRL_OSD		(1 << 4)
#define FTLCDC200_CTRL_SCALAR		(1 << 5)
#define FTLCDC200_CTRL_DITHER		(1 << 6)
#define FTLCDC200_CTRL_POP		(1 << 7)
#define FTLCDC200_CTRL_BLEND		(1 << 8)
#define FTLCDC200_CTRL_PIP_MASK		(0x3 << 9)
#define FTLCDC200_CTRL_PIP_SINGLE	(0x1 << 9)
#define FTLCDC200_CTRL_PIP_DOUBLE	(0x2 << 9)
#define FTLCDC200_CTRL_PATGEN		(1 << 11)

/*
 * LCD Panel Pixel
 */
#define FTLCDC200_PIXEL_BPP1		(0x0 << 0)
#define FTLCDC200_PIXEL_BPP2		(0x1 << 0)
#define FTLCDC200_PIXEL_BPP4		(0x2 << 0)
#define FTLCDC200_PIXEL_BPP8		(0x3 << 0)
#define FTLCDC200_PIXEL_BPP16		(0x4 << 0)
#define FTLCDC200_PIXEL_BPP24		(0x5 << 0)
#define FTLCDC200_PIXEL_PWROFF		(1 << 3)
#define FTLCDC200_PIXEL_BGR		(1 << 4)
#define FTLCDC200_PIXEL_LEB_LEP		(0x0 << 5)	/* little endian byte, little endian pixel */
#define FTLCDC200_PIXEL_BEB_BEP		(0x1 << 5)	/* big    endian byte, big    endian pixel */
#define FTLCDC200_PIXEL_LEB_BEP		(0x2 << 5)	/* little endian byte, big    endian pixel */
/* The following RGB bits indicate the inpu RGB format when BPP16 */
#define FTLCDC200_PIXEL_RGB565		(0x0 << 7)
#define FTLCDC200_PIXEL_RGB555		(0x1 << 7)
#define FTLCDC200_PIXEL_RGB444		(0x2 << 7)
#define FTLCDC200_PIXEL_VSYNC		(0x0 << 9)
#define FTLCDC200_PIXEL_VBACK		(0x1 << 9)
#define FTLCDC200_PIXEL_VACTIVE		(0x2 << 9)
#define FTLCDC200_PIXEL_VFRONT		(0x3 << 9)
#define FTLCDC200_PIXEL_24BIT_PANEL	(1 << 11)
/* The following DITHER bits indicate the dither algorithm used when dither enabled */
#define FTLCDC200_PIXEL_DITHER565	(0x0 << 12)
#define FTLCDC200_PIXEL_DITHER555	(0x1 << 12)
#define FTLCDC200_PIXEL_DITHER444	(0x2 << 12)
#define FTLCDC200_PIXEL_HRST		(1 << 14)	/* HCLK domain reset */
#define FTLCDC200_PIXEL_LRST		(1 << 15)	/* LC_CLK domain reset */

/*
 * LCD Interrupt Enable/Status/Clear
 */
#define FTLCDC200_INT_UNDERRUN		(1 << 0)
#define FTLCDC200_INT_NEXT_BASE		(1 << 1)
#define FTLCDC200_INT_VSTATUS		(1 << 2)
#define FTLCDC200_INT_BUS_ERROR		(1 << 3)

/*
 * Frame buffer
 */
#define FTLCDC200_SCALE_0_MASK		(0x3 << 8)
#define FTLCDC200_SCALE_0_QUARTER	(0x1 << 8)	/* 1/2 x 1/2 */
#define FTLCDC200_SCALE_0_HALF		(0x2 << 8)	/* 1/2 x 1   */
#define FTLCDC200_SCALE_1_MASK		(0x3 << 10)
#define FTLCDC200_SCALE_1_QUARTER	(0x1 << 10)	/* 1/2 x 1/2 */
#define FTLCDC200_SCALE_1_HALF		(0x2 << 10)	/* 1/2 x 1   */
#define FTLCDC200_SCALE_2_MASK		(0x3 << 12)
#define FTLCDC200_SCALE_2_QUARTER	(0x1 << 12)	/* 1/2 x 1/2 */
#define FTLCDC200_SCALE_2_HALF		(0x2 << 12)	/* 1/2 x 1   */
#define FTLCDC200_SCALE_3_MASK		(0x3 << 14)
#define FTLCDC200_SCALE_3_QUARTER	(0x1 << 14)	/* 1/2 x 1/2 */
#define FTLCDC200_SCALE_3_HALF		(0x2 << 14)	/* 1/2 x 1   */

/*
 * LCD Panel Frame Base Address 0/1/2/3
 */
#define FTLCDC200_FRAME_BASE(x)		((x) & ~0x3)

/*
 * LCD Pattern Generator
 */
#define FTLCDC200_PATGEN_0_VCBAR	(0x0 <<	0)
#define FTLCDC200_PATGEN_0_HCBAR	(0x1 << 0)
#define FTLCDC200_PATGEN_0_SC		(0x2 << 0)
#define FTLCDC200_PATGEN_1_VCBAR	(0x0 <<	2)
#define FTLCDC200_PATGEN_1_HCBAR	(0x1 << 2)
#define FTLCDC200_PATGEN_1_SC		(0x2 << 2)
#define FTLCDC200_PATGEN_2_VCBAR	(0x0 <<	4)
#define FTLCDC200_PATGEN_2_HCBAR	(0x1 << 4)
#define FTLCDC200_PATGEN_2_SC		(0x2 << 4)
#define FTLCDC200_PATGEN_3_VCBAR	(0x0 <<	6)
#define FTLCDC200_PATGEN_3_HCBAR	(0x1 << 6)
#define FTLCDC200_PATGEN_3_SC		(0x2 << 6)

/*
 * LCD FIFO Threshold Control
 */
#define FTLCDC200_FIFO_THRESHOLD_0(x)	(((x) & 0xff) << 0)
#define FTLCDC200_FIFO_THRESHOLD_1(x)	(((x) & 0xff) << 8)
#define FTLCDC200_FIFO_THRESHOLD_2(x)	(((x) & 0xff) << 16)
#define FTLCDC200_FIFO_THRESHOLD_3(x)	(((x) & 0xff) << 24)

/*
 * LCD Horizontal Timing Control
 */
#define FTLCDC200_HTIMING_PL(x)		(((x) & 0xff) << 0)
#define FTLCDC200_HTIMING_HW(x)		(((x) & 0xff) << 8)	/* HW determines X-Position */
#define FTLCDC200_HTIMING_HFP(x)	(((x) & 0xff) << 16)
#define FTLCDC200_HTIMING_HBP(x)	(((x) & 0xff) << 24)

/*
 * LCD Vertial Timing Control 0
 */
#define FTLCDC200_VTIMING0_LF(x)	(((x) & 0xfff) << 0)
#define FTLCDC200_VTIMING0_VW(x)	(((x) & 0x3f) << 16)	/* VW determines Y-Position */
#define FTLCDC200_VTIMING0_VFP(x)	(((x) & 0xff) << 24)

/*
 * LCD Vertial Timing Control 1
 */
#define FTLCDC200_VTIMING1_VBP(x)	(((x) & 0xff) << 0)

/*
 * LCD Polarity Control
 */
#define FTLCDC200_POLARITY_IVS		(1 << 0)
#define FTLCDC200_POLARITY_IHS		(1 << 1)
#define FTLCDC200_POLARITY_ICK		(1 << 2)
#define FTLCDC200_POLARITY_IDE		(1 << 3)
#define FTLCDC200_POLARITY_IPWR		(1 << 4)
#define FTLCDC200_POLARITY_DIVNO(x)	(((x) & 0x7f) << 8)	/* bus clock rate / (x + 1) determines the frame rate */

/*
 * LCD Serial Panel Pixel
 */
#define FTLCDC200_SERIAL_SERIAL		(1 << 0)
#define FTLCDC200_SERIAL_DELTA_TYPE	(1 << 1)
#define FTLCDC200_SERIAL_RGB		(0x0 << 2)
#define FTLCDC200_SERIAL_BRG		(0x1 << 2)
#define FTLCDC200_SERIAL_GBR		(0x2 << 2)
#define FTLCDC200_SERIAL_LSR		(1 << 4)
#define FTLCDC200_SERIAL_AUO052		(1 << 5)

/*
 * CCIR656
 */
#define FTLCDC200_CCIR656_NTSC		(1 << 0)	/* 0: PAL, 1: NTSC */
#define FTLCDC200_CCIR656_P720		(1 << 1)	/* 0: 640, 1: 720 pixels per line */
#define FTLCDC200_CCIR656_PHASE		(1 << 2)

/*
 * PIP (Picture in Picture)
 */
#define FTLCDC200_PIP_BLEND_1(x)	(((x) & 0x1f) << 0)
#define FTLCDC200_PIP_BLEND_2(x)	(((x) & 0x1f) << 8)

/*
 * PIP Sub Picture 1/2 Position
 */
#define FTLCDC200_PIP_POS_V(y)		(((y) & 0x7ff) << 0)
#define FTLCDC200_PIP_POS_H(x)		(((x) & 0x7ff) << 16)

/*
 * PIP Sub Picture 1/2 Dimension
 */
#define FTLCDC200_PIP_DIM_V(y)		(((y) & 0x7ff) << 0)
#define FTLCDC200_PIP_DIM_H(x)		(((x) & 0x7ff) << 16)

/*
 * Color Management 0
 */
#define FTLCDC200_CM0_BRIGHT(x)		(((x) & 0x7f) << 0)
#define FTLCDC200_CM0_BRIGHT_NEGATIVE	(1 << 7)
#define FTLCDC200_CM0_SATURATE(x)	(((x) & 0x3f) << 8)

/*
 * Color Management 1
 */
#define FTLCDC200_CM1_HUE_SIN(x)	(((x) & 0x3f) << 0)
#define FTLCDC200_CM1_HUE_SIN_NEGATIVE	(1 << 6)
#define FTLCDC200_CM1_HUE_COS(x)	(((x) & 0x3f) << 8)
#define FTLCDC200_CM1_HUE_COS_NEGATIVE	(1 << 14)

/*
 * Color Management 2
 */
#define FTLCDC200_CM2_SHARP_TH0(x)	(((x) & 0xff) << 0)
#define FTLCDC200_CM2_SHARP_TH1(x)	(((x) & 0xff) << 8)
#define FTLCDC200_CM2_K0(x)		(((x) & 0xf) << 16)
#define FTLCDC200_CM2_K1(x)		(((x) & 0xf) << 20)

/*
 * Color Management 3
 */
#define FTLCDC200_CM3_CONTRAST_OFFSET(x)	(((x) & 0xfff) << 0)
#define FTLCDC200_CM3_CONTRAST_NEGATIVE		(1 << 12)
#define FTLCDC200_CM3_CONTRAST_SLOPE(x)		(((x) & 0x1f) << 16)

#ifdef	CONFIG_FTLCDC200_COMPLEX_OSD

/*
 * Complex OSD configuration
 */

/*
 * OSD Window On/Off
 */
#define FTLCDC200_OSD_ENABLE_W1		(1 << 0)
#define FTLCDC200_OSD_ENABLE_W2		(1 << 1)
#define FTLCDC200_OSD_ENABLE_W3		(1 << 2)
#define FTLCDC200_OSD_ENABLE_W4		(1 << 3)

/*
 * OSD Font Control 0
 */
#define FTLCDC200_OSD_FONT0_VZOOM_EN		(1 << 0)
#define FTLCDC200_OSD_FONT0_VZOOM_IN		(1 << 1)
#define FTLCDC200_OSD_FONT0_VZOOM_IN_X1		(0x0 << 2)
#define FTLCDC200_OSD_FONT0_VZOOM_IN_X2		(0x1 << 2)
#define FTLCDC200_OSD_FONT0_VZOOM_IN_X3		(0x2 << 2)
#define FTLCDC200_OSD_FONT0_VZOOM_IN_X4		(0x3 << 2)
#define FTLCDC200_OSD_FONT0_VZOOM_OUT_HALF	(1 << 4)
#define FTLCDC200_OSD_FONT0_HZOOM_EN		(1 << 5)
#define FTLCDC200_OSD_FONT0_HZOOM_IN		(1 << 6)
#define FTLCDC200_OSD_FONT0_HZOOM_IN_X1		(0x0 << 7)
#define FTLCDC200_OSD_FONT0_HZOOM_IN_X2		(0x1 << 7)
#define FTLCDC200_OSD_FONT0_HZOOM_IN_X3		(0x2 << 7)
#define FTLCDC200_OSD_FONT0_HZOOM_IN_X4		(0x3 << 7)
#define FTLCDC200_OSD_FONT0_HZOOM_OUT_HALF	(1 << 9)
#define FTLCDC200_OSD_FONT0_TRANSPARENCY0	(0x0 << 10)
#define FTLCDC200_OSD_FONT0_TRANSPARENCY25	(0x1 << 10)
#define FTLCDC200_OSD_FONT0_TRANSPARENCY50	(0x2 << 10)
#define FTLCDC200_OSD_FONT0_TRANSPARENCY75	(0x3 << 10)
#define FTLCDC200_OSD_FONT0_TRANSPARENCY100	(0x4 << 10)
#define FTLCDC200_OSD_FONT0_TYPE_NORMAL		(0x0 << 13)
#define FTLCDC200_OSD_FONT0_TYPE_BORDER		(0x1 << 13)
#define FTLCDC200_OSD_FONT0_TYPE_SHADOW		(0x2 << 13)
#define FTLCDC200_OSD_FONT0_BORDER_COLOR(x)	(((x) & 0xf) << 16)
#define FTLCDC200_OSD_FONT0_SHADOW_COLOR(x)	(((x) & 0xf) << 20)

/*
 * OSD Font Control 1
 */
#define FTLCDC200_OSD_FONT1_ROWSPACE_EN		(1 << 0)
#define FTLCDC200_OSD_FONT1_ROWSPACE(x)		(((x) & 0xf) << 1)
#define FTLCDC200_OSD_FONT1_COLSPACE_EN		(1 << 8)
#define FTLCDC200_OSD_FONT1_COLSPACE(x)		(((x) & 0xf) << 9)

/*
 * OSD Window 1/2/3/4 Control 1
 */
#define FTLCDC200_OSD_WC1_XDIM(x)		(((x) & 0x7f) << 0)
#define FTLCDC200_OSD_WC1_YDIM(y)		(((y) & 0x7f) << 8)
#define FTLCDC200_OSD_WC1_HIGHLIGHT		(1 << 16)
#define FTLCDC200_OSD_WC1_SHADOW		(1 << 17)
#define FTLCDC200_OSD_WC1_BORDER_WIDTH(x)	(((x) & 0x3) << 18)
#define FTLCDC200_OSD_WC1_BORDER_COLOR(x)	(((x) & 0xf) << 20)
#define FTLCDC200_OSD_WC1_SHADOW_WIDTH(x)	(((x) & 0x3) << 24)
#define FTLCDC200_OSD_WC1_SHADOW_COLOR(x)	(((x) & 0xf) << 26)

/*
 * OSD Window 1/2/3/4 Control 2
 */
#define FTLCDC200_OSD_WC2_FONT_BASE(x)	((x) & 0x1ff)

/*
 * OSD Multi-Color Font Base Address
 */
#define FTLCDC200_OSD_MCFB(x)		((x) & 0x3fff)

/*
 * OSD Palette Color 0/1/2/3/4/5/6/7/8/9/10/11/12/13/14/15
 */
#define FTLCDC200_OSD_PC_R(x)		(((x) & 0xff) << 0)
#define FTLCDC200_OSD_PC_G(x)		(((x) & 0xff) << 8)
#define FTLCDC200_OSD_PC_B(x)		(((x) & 0xff) << 16)

#else	/* CONFIG_FTLCDC200_COMPLEX_OSD */

/*
 * Simple OSD Configuration
 */

/*
 * OSD Scaling and Dimension Control
 */
#define FTLCDC200_OSD_SDC_VSCALE_X1	(0x0 << 0)
#define FTLCDC200_OSD_SDC_VSCALE_X2	(0x1 << 0)
#define FTLCDC200_OSD_SDC_VSCALE_X3	(0x2 << 0)
#define FTLCDC200_OSD_SDC_VSCALE_X4	(0x3 << 0)
#define FTLCDC200_OSD_SDC_HSCALE_X1	(0x0 << 2)
#define FTLCDC200_OSD_SDC_HSCALE_X2	(0x1 << 2)
#define FTLCDC200_OSD_SDC_HSCALE_X3	(0x2 << 2)
#define FTLCDC200_OSD_SDC_HSCALE_X4	(0x3 << 2)
#define FTLCDC200_OSD_SDC_VDIM(x)	(((x) & 0x1f) << 4)
#define FTLCDC200_OSD_SDC_HDIM(x)	(((x) & 0x3f) << 12)

/*
 * OSD Position Control
 */
#define FTLCDC200_OSD_POS_V(x)		(((x) & 0x7ff) << 0)
#define FTLCDC200_OSD_POS_H(x)		(((x) & 0xfff) << 12)

/*
 * OSD Foreground Color Control
 */
#define FTLCDC200_OSD_FG_PAL0(x)	(((x) & 0xff) << 0)
#define FTLCDC200_OSD_FG_PAL1(x)	(((x) & 0xff) << 8)
#define FTLCDC200_OSD_FG_PAL2(x)	(((x) & 0xff) << 16)
#define FTLCDC200_OSD_FG_PAL3(x)	(((x) & 0xff) << 24)

/*
 * OSD Background Color Control
 */
#define FTLCDC200_OSD_BG_TRANSPARENCY25		(0x0 << 0)
#define FTLCDC200_OSD_BG_TRANSPARENCY50		(0x1 << 0)
#define FTLCDC200_OSD_BG_TRANSPARENCY75		(0x2 << 0)
#define FTLCDC200_OSD_BG_TRANSPARENCY100	(0x3 << 0)
#define FTLCDC200_OSD_BG_PAL1(x)		(((x) & 0xff) << 8)
#define FTLCDC200_OSD_BG_PAL2(x)		(((x) & 0xff) << 16)
#define FTLCDC200_OSD_BG_PAL3(x)		(((x) & 0xff) << 24)

#endif	/* CONFIG_FTLCDC200_COMPLEX_OSD */

#endif	/* __FTLCDC200_H */
