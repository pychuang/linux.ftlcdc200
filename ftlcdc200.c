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

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/fb.h>
#include <linux/init.h>

#include "ftlcdc200.h"

/*
 * Actually, I don't know if LC_CLK is AHB clock on A320.  It is not written
 * in A320 data sheet.
 */
#define LC_CLK	AHB_CLK_IN

/*
 * Select a panel configuration
 */
#undef CONFIG_AUO_A036QN01_CPLD
#define CONFIG_TPO_TD070WGEC2

/* 
 * This structure defines the hardware state of the graphics card. Normally
 * you place this in a header file in linux/include/video. This file usually
 * also includes register information. That allows other driver subsystems
 * and userland applications the ability to use the same header file to 
 * avoid duplicate work and easy porting of software. 
 */
struct ftlcdc200 {
	struct resource *res;
	void *base;
	int irq;

	/*
	 * This pseudo_palette is used _only_ by fbcon, thus
	 * it only contains 16 entries to match the number of colors supported
	 * by the console. The pseudo_palette is used only if the visual is
	 * in directcolor or truecolor mode.  With other visuals, the
	 * pseudo_palette is not used. (This might change in the future.)
	 */
	u32 pseudo_palette[16];
};

/**
 * ftlcdc200_default_fix - Default struct fb_fix_screeninfo
 * It is only used in ftlcdc200_probe, so mark it as __devinitdata
 */
static struct fb_fix_screeninfo ftlcdc200_default_fix __devinitdata = {
	.id		= "ftlcdc200",
	.type		= FB_TYPE_PACKED_PIXELS,
	.ypanstep	= 1,
	.accel		= FB_ACCEL_NONE,
};

/**
 * ftlcdc200_default_var - Default struct fb_var_screeninfo
 * It is only used in ftlcdc200_probe, so mark it as __devinitdata
 */

#ifdef CONFIG_AUO_A036QN01_CPLD
static struct fb_var_screeninfo ftlcdc200_default_var __devinitdata = {
	.xres		= 320,
	.yres		= 240,
	.xres_virtual	= 320,
	.yres_virtual	= 240,
	.bits_per_pixel	= 16,
	.pixclock	= 171521,
	.left_margin	= 44,
	.right_margin	= 6,
	.upper_margin	= 11,
	.lower_margin	= 8,
	.hsync_len	= 21,
	.vsync_len	= 3,
	.vmode		= FB_VMODE_NONINTERLACED,
	.sync		= 0,
};
#define CONFIG_FTLCDC200_PIXEL_BGR
#endif

#ifdef CONFIG_TPO_TD070WGEC2
static struct fb_var_screeninfo ftlcdc200_default_var __devinitdata = {
	.xres		= 800,
	.yres		= 480,
	.xres_virtual	= 800,
	.yres_virtual	= 480,
	.bits_per_pixel	= 16,
	.pixclock	= 41521,
	.left_margin	= 1,
	.right_margin	= 44,
	.upper_margin	= 1,
	.lower_margin	= 1,
	.hsync_len	= 1,
	.vsync_len	= 1,
	.vmode		= FB_VMODE_NONINTERLACED,
	.sync		= 0,
};
#define CONFIG_FTLCDC200_SERIAL
#endif

/******************************************************************************
 * internal functions
 *****************************************************************************/
static int ftlcdc200_grow_framebuffer(struct fb_info *info,
	struct fb_var_screeninfo *var)
{
	struct device *dev = info->device;
	struct ftlcdc200 *ftlcdc200 = info->par;
	unsigned int reg;
	unsigned long smem_len = (var->xres_virtual * var->yres_virtual
				 * DIV_ROUND_UP(var->bits_per_pixel, 8));
	unsigned long smem_start;
	void *screen_base;

	if (smem_len <= info->fix.smem_len) {
		/* current framebuffer is big enough */
		return 0;
	}

	/*
	 * Allocate bigger framebuffer
	 */
	screen_base = dma_alloc_writecombine(NULL, smem_len,
				(dma_addr_t *)&smem_start,
				GFP_KERNEL | GFP_DMA);

	if (!screen_base) {
		dev_err(dev, "Failed to allocate frame buffer\n");
		return -ENOMEM;
	}

	memset(screen_base, 0, smem_len);
	dev_dbg(dev, "  frame buffer: vitual = %p, physical = %08lx\n",
		screen_base, smem_start);

	reg = FTLCDC200_FRAME_BASE(smem_start);
	iowrite32(reg, ftlcdc200->base + FTLCDC200_OFFSET_FRAME_BASE0);

	/*
	 * Free current framebuffer (if any)
	 */
	if (info->screen_base) {
		dma_free_writecombine(NULL, info->fix.smem_len,
			info->screen_base, (dma_addr_t )info->fix.smem_start);
	}

	info->screen_base = screen_base;
	info->fix.smem_start = smem_start;
	info->fix.smem_len = smem_len;

	return 0;
}

static void ftlcdc200_set_gamma(void *gtbase, unsigned int i, unsigned int val)
{
	unsigned int reg;
	unsigned int offset = i & ~0x3;
	unsigned int shift = (i % 4) * 8;
	unsigned int mask = 0xff << shift;

	val = val > 0xff ? 0xff : val;
	val <<= shift;

	reg = ioread32(gtbase + offset);
	reg &= ~mask;
	reg |= val;
	iowrite32(reg, gtbase + offset);
}

static void ftlcdc200_set_gamma_red(void *base, unsigned int i, unsigned int val)
{
	ftlcdc200_set_gamma(base + FTLCDC200_OFFSET_GAMMA_R, i, val);
}

static void ftlcdc200_set_gamma_green(void *base, unsigned int i, unsigned int val)
{
	ftlcdc200_set_gamma(base + FTLCDC200_OFFSET_GAMMA_G, i, val);
}

static void ftlcdc200_set_gamma_blue(void *base, unsigned int i, unsigned int val)
{
	ftlcdc200_set_gamma(base + FTLCDC200_OFFSET_GAMMA_B, i, val);
}

/**
 * ftlcdc200_set_linear_gamma - Setup linear gamma tables
 *
 * Note: gamma tables must be programmed while LCD disabled
 */
static void ftlcdc200_set_linear_gamma(struct ftlcdc200 *ftlcdc200)
{
	int i;

	for (i = 0; i < 256; i++) {
		ftlcdc200_set_gamma_red(ftlcdc200->base, i, i);
		ftlcdc200_set_gamma_green(ftlcdc200->base, i, i);
		ftlcdc200_set_gamma_blue(ftlcdc200->base, i, i);
	}
}

/******************************************************************************
 * interrupt handler
 *****************************************************************************/
static irqreturn_t ftlcdc200_interrupt(int irq, void *dev_id)
{
	struct fb_info *info = dev_id;
	struct ftlcdc200 *ftlcdc200 = info->par;
	unsigned int status;

	status = ioread32(ftlcdc200->base + FTLCDC200_OFFSET_INT_STATUS);

	if (status & FTLCDC200_INT_UNDERRUN) {
		if (printk_ratelimit())
			dev_notice(info->device, "underrun\n");
	}

	if (status & FTLCDC200_INT_NEXT_BASE) {
		if (printk_ratelimit())
			dev_dbg(info->device, "frame base updated\n");
	}

	if (status & FTLCDC200_INT_VSTATUS) {
		if (printk_ratelimit())
			dev_dbg(info->device, "vertical duration reached \n");

	}

	if (status & FTLCDC200_INT_BUS_ERROR) {
		if (printk_ratelimit())
			dev_err(info->device, "bus error!\n");

	}

	iowrite32(status, ftlcdc200->base + FTLCDC200_OFFSET_INT_CLEAR);

	return IRQ_HANDLED;
}

/******************************************************************************
 * struct platform_driver functions
 *****************************************************************************/
/**
 * ftlcdc200_check_var - Validates a var passed in.
 * @var: frame buffer variable screen structure
 * @info: frame buffer structure that represents a single frame buffer 
 *
 * Checks to see if the hardware supports the state requested by
 * var passed in. This function does not alter the hardware state!!! 
 * This means the data stored in struct fb_info and struct ftlcdc200 do 
 * not change. This includes the var inside of struct fb_info. 
 * Do NOT change these. This function can be called on its own if we
 * intent to only test a mode and not actually set it. The stuff in 
 * modedb.c is a example of this. If the var passed in is slightly 
 * off by what the hardware can support then we alter the var PASSED in
 * to what we can do.
 *
 * For values that are off, this function must round them _up_ to the
 * next value that is supported by the hardware.  If the value is
 * greater than the highest value supported by the hardware, then this
 * function must return -EINVAL.
 *
 * Exception to the above rule:  Some drivers have a fixed mode, ie,
 * the hardware is already set at boot up, and cannot be changed.  In
 * this case, it is more acceptable that this function just return
 * a copy of the currently working var (info->var). Better is to not
 * implement this function, as the upper layer will do the copying
 * of the current var for you.
 *
 * Note:  This is the only function where the contents of var can be
 * freely adjusted after the driver has been registered. If you find
 * that you have code outside of this function that alters the content
 * of var, then you are doing something wrong.  Note also that the
 * contents of info->var must be left untouched at all times after
 * driver registration.
 *
 * Returns negative errno on error, or zero on success.
 */
static int ftlcdc200_check_var(struct fb_var_screeninfo *var,
		struct fb_info *info)
{
	struct device *dev = info->device;
	unsigned long clk_value_khz;
	int ret;

	clk_value_khz = LC_CLK / 1000;

	dev_dbg(dev, "%s:\n", __func__);

	if (var->pixclock == 0) {
		dev_err(dev, "pixclock not specified\n");
		return -EINVAL;
	}

	dev_dbg(dev, "  resolution: %ux%u (%ux%u virtual)\n",
		var->xres, var->yres,
		var->xres_virtual, var->yres_virtual);
	dev_dbg(dev, "  pixclk:       %lu KHz\n", PICOS2KHZ(var->pixclock));
	dev_dbg(dev, "  bpp:          %u\n", var->bits_per_pixel);
	dev_dbg(dev, "  clk:          %lu KHz\n", clk_value_khz);
	dev_dbg(dev, "  left  margin: %u\n", var->left_margin);
	dev_dbg(dev, "  right margin: %u\n", var->right_margin);
	dev_dbg(dev, "  upper margin: %u\n", var->upper_margin);
	dev_dbg(dev, "  lower margin: %u\n", var->lower_margin);
	dev_dbg(dev, "  hsync:        %u\n", var->hsync_len);
	dev_dbg(dev, "  vsync:        %u\n", var->vsync_len);

	if (PICOS2KHZ(var->pixclock) * DIV_ROUND_UP(var->bits_per_pixel, 8)
			> clk_value_khz) {
		dev_err(dev, "%lu KHz pixel clock is too fast\n",
			PICOS2KHZ(var->pixclock));
		return -EINVAL;
	}

	if (var->xres != info->var.xres)
		return -EINVAL;

	if (var->yres != info->var.yres)
		return -EINVAL;

	if (var->xres_virtual != info->var.xres_virtual)
		return -EINVAL;

	if (var->xres_virtual != info->var.xres)
		return -EINVAL;

	if (var->yres_virtual < info->var.yres)
		return -EINVAL;

	ret = ftlcdc200_grow_framebuffer(info, var);
	if (ret)
		return ret;

	switch (var->bits_per_pixel) {
	case 1: case 2: case 4: case 8:
		var->red.offset = var->green.offset = var->blue.offset = 0;
		var->red.length = var->green.length = var->blue.length
				= var->bits_per_pixel;
		break;

	case 16:	/* RGB:565 mode */
		var->red.offset		= 11;
		var->green.offset	= 5;
		var->blue.offset	= 0;

		var->red.length		= 5;
		var->green.length	= 6;
		var->blue.length	= 5;
		var->transp.length	= 0;
		break;

	case 32:	/* RGB:888 mode */
		var->red.offset		= 16;
		var->green.offset	= 8;
		var->blue.offset	= 0;
		var->transp.offset	= 24;

		var->red.length = var->green.length = var->blue.length = 8;
		var->transp.length	= 8;
		break;

	default:
		dev_err(dev, "color depth %d not supported\n",
			var->bits_per_pixel);
		return -EINVAL;
	}

	return 0;
}

/**
 * ftlcdc200_set_par - Alters the hardware state.
 * @info: frame buffer structure that represents a single frame buffer
 *
 * Using the fb_var_screeninfo in fb_info we set the resolution of the
 * this particular framebuffer. This function alters the par AND the
 * fb_fix_screeninfo stored in fb_info. It does not alter var in 
 * fb_info since we are using that data. This means we depend on the
 * data in var inside fb_info to be supported by the hardware. 
 *
 * This function is also used to recover/restore the hardware to a
 * known working state.
 *
 * fb_check_var is always called before fb_set_par to ensure that
 * the contents of var is always valid.
 *
 * Again if you can't change the resolution you don't need this function.
 *
 * However, even if your hardware does not support mode changing,
 * a set_par might be needed to at least initialize the hardware to
 * a known working state, especially if it came back from another
 * process that also modifies the same hardware, such as X.
 *
 * Returns negative errno on error, or zero on success.
 */
static int ftlcdc200_set_par(struct fb_info *info)
{
	struct ftlcdc200 *ftlcdc200 = info->par;
	unsigned long clk_value_khz;
	unsigned int divno;
	unsigned int reg;

	dev_dbg(info->device, "%s:\n", __func__);
	dev_dbg(info->device, "  resolution:     %ux%u (%ux%u virtual)\n",
		info->var.xres, info->var.yres,
		info->var.xres_virtual, info->var.yres_virtual);

	/*
	 * Fill uninitialized fields of struct fb_fix_screeninfo
	 */
	if (info->var.bits_per_pixel == 1)
		info->fix.visual = FB_VISUAL_MONO01;
	else if (info->var.bits_per_pixel <= 8)
		info->fix.visual = FB_VISUAL_PSEUDOCOLOR;
	else
		info->fix.visual = FB_VISUAL_TRUECOLOR;

	info->fix.line_length = info->var.xres_virtual *
				DIV_ROUND_UP(info->var.bits_per_pixel, 8);

	/*
	 * polarity control
	 */
	clk_value_khz = LC_CLK / 1000;

	divno = DIV_ROUND_UP(clk_value_khz, PICOS2KHZ(info->var.pixclock));
	if (divno == 0) {
		dev_err(info->device,
			"pixel clock(%lu kHz) > bus clock(%lu kHz)\n",
			PICOS2KHZ(info->var.pixclock), clk_value_khz);
		return -EINVAL;
	}

	clk_value_khz = DIV_ROUND_UP(clk_value_khz, divno);
	info->var.pixclock = KHZ2PICOS(clk_value_khz);
	dev_dbg(info->device, "  updated pixclk: %lu KHz (divno = %d)\n",
		clk_value_khz, divno - 1);

	dev_dbg(info->device, "  frame rate:     %lu Hz\n",
		clk_value_khz * 1000
		/ (info->var.xres + info->var.left_margin
			+ info->var.right_margin + info->var.hsync_len)
		/ (info->var.yres + info->var.upper_margin
			+ info->var.lower_margin + info->var.vsync_len));

	reg = FTLCDC200_POLARITY_ICK
	    | FTLCDC200_POLARITY_DIVNO(divno - 1);

	if ((info->var.sync & FB_SYNC_HOR_HIGH_ACT) == 0)
		reg |= FTLCDC200_POLARITY_IHS;

	if ((info->var.sync & FB_SYNC_VERT_HIGH_ACT) == 0)
		reg |= FTLCDC200_POLARITY_IVS;

	dev_dbg(info->device, "  [POLARITY]   = %08x\n", reg);
	iowrite32(reg, ftlcdc200->base + FTLCDC200_OFFSET_POLARITY);

	/*
	 * horizontal timing control
	 */
	reg = FTLCDC200_HTIMING_PL(info->var.xres / 16 - 1);
	reg |= FTLCDC200_HTIMING_HW(info->var.hsync_len - 1);
	reg |= FTLCDC200_HTIMING_HFP(info->var.right_margin - 1);
	reg |= FTLCDC200_HTIMING_HBP(info->var.left_margin - 1);

	dev_dbg(info->device, "  [HTIMING]    = %08x\n", reg);
	iowrite32(reg, ftlcdc200->base + FTLCDC200_OFFSET_HTIMING);

	/*
	 * vertical timing control
	 */
	reg = FTLCDC200_VTIMING0_LF(info->var.yres - 1);
	reg |= FTLCDC200_VTIMING0_VW(info->var.vsync_len - 1);
	reg |= FTLCDC200_VTIMING0_VFP(info->var.lower_margin);

	dev_dbg(info->device, "  [VTIMING0]   = %08x\n", reg);
	iowrite32(reg, ftlcdc200->base + FTLCDC200_OFFSET_VTIMING0);

	reg = FTLCDC200_VTIMING1_VBP(info->var.upper_margin);

	dev_dbg(info->device, "  [VTIMING1]   = %08x\n", reg);
	iowrite32(reg, ftlcdc200->base + FTLCDC200_OFFSET_VTIMING1);

	/*
	 * Panel Pixel
	 */
	reg = FTLCDC200_PIXEL_LEB_LEP;
#ifdef CONFIG_FTLCDC200_PIXEL_BGR
	reg |= FTLCDC200_PIXEL_BGR;
#endif

	switch (info->var.bits_per_pixel) {
		case 1:
			reg |= FTLCDC200_PIXEL_BPP1;
			break;

		case 2:
			reg |= FTLCDC200_PIXEL_BPP2;
			break;

		case 4:
			reg |= FTLCDC200_PIXEL_BPP4;
			break;

		case 8:
			reg |= FTLCDC200_PIXEL_BPP8;
			break;

		case 16:
			reg |= FTLCDC200_PIXEL_BPP16;
			break;

		case 32:
			reg |= FTLCDC200_PIXEL_BPP24;
			break;

		default:
			BUG();
			break;
	}

	dev_dbg(info->device, "  [PIXEL]      = %08x\n", reg);
	iowrite32(reg, ftlcdc200->base + FTLCDC200_OFFSET_PIXEL);

	/*
	 * Serial Panel Pixel
	 */
	reg = 0;
#ifdef CONFIG_FTLCDC200_SERIAL
	reg |= FTLCDC200_SERIAL_SERIAL;
#endif

	dev_dbg(info->device, "  [SERIAL]     = %08x\n", reg);
	iowrite32(reg, ftlcdc200->base + FTLCDC200_OFFSET_SERIAL);

	/*
	 * Function Enable
	 */
	reg = FTLCDC200_CTRL_ENABLE
	    | FTLCDC200_CTRL_LCD;

	dev_dbg(info->device, "  [CTRL]       = %08x\n", reg);
	iowrite32(reg, ftlcdc200->base + FTLCDC200_OFFSET_CTRL);

	return 0;
}

/**
 * ftlcdc200_setcolreg - Optional function. Sets a color register.
 * @regno: Which register in the CLUT we are programming 
 * @red: The red value which can be up to 16 bits wide 
 * @green: The green value which can be up to 16 bits wide 
 * @blue:  The blue value which can be up to 16 bits wide.
 * @transp: If supported, the alpha value which can be up to 16 bits wide.
 * @info: frame buffer info structure
 * 
 * Set a single color register. The values supplied have a 16 bit
 * magnitude which needs to be scaled in this function for the hardware. 
 * Things to take into consideration are how many color registers, if
 * any, are supported with the current color visual. With truecolor mode
 * no color palettes are supported. Here a pseudo palette is created
 * which we store the value in pseudo_palette in struct fb_info. For
 * pseudocolor mode we have a limited color palette. To deal with this
 * we can program what color is displayed for a particular pixel value.
 * DirectColor is similar in that we can program each color field. If
 * we have a static colormap we don't need to implement this function. 
 * 
 * Returns negative errno on error, or zero on success.
 */
static int ftlcdc200_setcolreg(unsigned regno, unsigned red, unsigned green,
	unsigned blue, unsigned transp, struct fb_info *info)
{
	struct ftlcdc200 *ftlcdc200 = info->par;
	u32 val;

	dev_dbg(info->device, "%s(%d, %d, %d, %d, %d)\n", __func__,
		regno, red, green, blue, transp);

	if (regno >= 256)  /* no. of hw registers */
		return -EINVAL;

	/*
	 * If grayscale is true, then we convert the RGB value
	 * to grayscale no mater what visual we are using.
	 */
	if (info->var.grayscale) {
		/* grayscale = 0.30*R + 0.59*G + 0.11*B */
		red = green = blue = (red * 77 + green * 151 + blue * 28) >> 8;
	}

#define CNVT_TOHW(val,width) ((((val) << (width)) + 0x7FFF - (val)) >> 16)
	red = CNVT_TOHW(red, info->var.red.length);
	green = CNVT_TOHW(green, info->var.green.length);
	blue = CNVT_TOHW(blue, info->var.blue.length);
	transp = CNVT_TOHW(transp, info->var.transp.length);
#undef CNVT_TOHW

	switch (info->fix.visual) {
	case FB_VISUAL_TRUECOLOR:
		if (regno >= 16)
			return -EINVAL;
		/*
		 * The contents of the pseudo_palette is in raw pixel format.
		 * Ie, each entry can be written directly to the framebuffer
		 * without any conversion.
		 */
	
		val = (red << info->var.red.offset);
		val |= (green << info->var.green.offset);
		val |= (blue << info->var.blue.offset);

		ftlcdc200->pseudo_palette[regno] = val;

		break;

	case FB_VISUAL_STATIC_PSEUDOCOLOR:
	case FB_VISUAL_PSEUDOCOLOR:
		/* TODO set palette registers */
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

/**
 * ftlcdc200_pan_display - NOT a required function. Pans the display.
 * @var: frame buffer variable screen structure
 * @info: frame buffer structure that represents a single frame buffer
 *
 * Pan (or wrap, depending on the `vmode' field) the display using the
 * `xoffset' and `yoffset' fields of the `var' structure.
 * If the values don't fit, return -EINVAL.
 *
 * Returns negative errno on error, or zero on success.
 */
static int ftlcdc200_pan_display(struct fb_var_screeninfo *var,
			       struct fb_info *info)
{
	struct ftlcdc200 *ftlcdc200 = info->par;
	unsigned long dma_addr;
	unsigned int value;

	dev_dbg(info->device, "%s\n", __func__);

	dma_addr = info->fix.smem_start + var->yoffset * info->fix.line_length;
	value = FTLCDC200_FRAME_BASE(dma_addr);

	iowrite32(value, ftlcdc200->base + FTLCDC200_OFFSET_FRAME_BASE0);
	dev_dbg(info->device, "  [FRAME BASE] = %08x\n", value);
	return 0;
}

static struct fb_ops ftlcdc200_fb_ops = {
	.owner		= THIS_MODULE,
	.fb_check_var	= ftlcdc200_check_var,
	.fb_set_par	= ftlcdc200_set_par,
	.fb_setcolreg	= ftlcdc200_setcolreg,
	.fb_pan_display	= ftlcdc200_pan_display,

	/* These are generic software based fb functions */
	.fb_fillrect	= cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
};

/******************************************************************************
 * struct platform_driver functions
 *****************************************************************************/
static int __init ftlcdc200_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct ftlcdc200 *ftlcdc200;
	struct resource *res;
	struct fb_info *info;
	unsigned int reg;
	int irq;
	int ret;

	dev_dbg(dev, "%s\n", __func__);
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		return -ENXIO;
	}

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		ret = irq;
		goto err_get_irq;
	}

	/*
	 * Allocate info and par
	 */
	info = framebuffer_alloc(sizeof(struct ftlcdc200), dev);
	if (!info) {
		dev_err(dev, "Failed to allocate fb_info\n");
		ret = -ENOMEM;
		goto err_alloc_info;
	}

	platform_set_drvdata(pdev, info);

	ftlcdc200 = info->par;

	/*
	 * Map io memory
	 */
	ftlcdc200->res = request_mem_region(res->start, res->end - res->start,
			dev_name(dev));
	if (!ftlcdc200->res) {
		dev_err(dev, "Could not reserve memory region\n");
		ret = -ENOMEM;
		goto err_req_mem_region;
	}

	ftlcdc200->base = ioremap(res->start, res->end - res->start);
	if (!ftlcdc200->base) {
		dev_err(dev, "Failed to ioremap registers\n");
		ret = -EIO;
		goto err_ioremap;
	}

	/*
	 * Make sure LCD controller is disabled
	 */
	iowrite32(0, ftlcdc200->base + FTLCDC200_OFFSET_CTRL);

	/*
	 * Register interrupt handler
	 */
	ret = request_irq(irq, ftlcdc200_interrupt, IRQF_SHARED, pdev->name, info);
	if (ret < 0) {
		dev_err(dev, "Failed to request irq %d\n", irq);
		goto err_req_irq;
	}

	ftlcdc200->irq = irq;

	/*
	 * Enable interrupts
	 */
	reg = FTLCDC200_INT_UNDERRUN
	    | FTLCDC200_INT_BUS_ERROR;

	iowrite32(reg, ftlcdc200->base + FTLCDC200_OFFSET_INT_ENABLE);

	/*
	 * Initialize gamma table
	 */
	ftlcdc200_set_linear_gamma(ftlcdc200);
#if 0
	/* bypass gamma - this register is not in the data sheet */
	iowrite32(0xf, ftlcdc200->base + 0x54);
#endif

	/*
	 * Set up flags to indicate what sort of acceleration your
	 * driver can provide (pan/wrap/copyarea/etc.) and whether it
	 * is a module -- see FBINFO_* in include/linux/fb.h
	 */
	info->flags = FBINFO_DEFAULT;

	info->fbops = &ftlcdc200_fb_ops;
	info->pseudo_palette = ftlcdc200->pseudo_palette;

	/*
	 * Allocate colormap
	 */
	ret = fb_alloc_cmap(&info->cmap, 256, 0);
	if (ret < 0) {
		dev_err(dev, "Failed to allocate colormap\n");
		goto err_alloc_cmap;
	}

	/*
	 * Copy default parameters
	 */
	info->fix = ftlcdc200_default_fix;
	info->var = ftlcdc200_default_var;

	ret = ftlcdc200_check_var(&info->var, info);
	if (ret < 0) {
		dev_err(dev, "ftlcdc200_check_var() failed\n");
		goto err_check_var;
	}

	/*
	 * Does a call to fb_set_par() before register_framebuffer needed?  This
	 * will depend on you and the hardware.  If you are sure that your driver
	 * is the only device in the system, a call to fb_set_par() is safe.
	 *
	 * Hardware in x86 systems has a VGA core.  Calling set_par() at this
	 * point will corrupt the VGA console, so it might be safer to skip a
	 * call to set_par here and just allow fbcon to do it for you.
	 */
	ftlcdc200_set_par(info);

	/*
	 * Tell the world that we're ready to go
	 */
	if (register_framebuffer(info) < 0) {
		dev_err(dev, "Failed to register frame buffer\n");
		ret = -EINVAL;
		goto err_register_info;
	}

	dev_info(dev, "fb%d: %s frame buffer device\n", info->node,
		info->fix.id);

	return 0;

err_register_info:
	/* disable LCD HW */
	iowrite32(0, ftlcdc200->base + FTLCDC200_OFFSET_CTRL);
err_check_var:
	if (info->screen_base) {
		dma_free_writecombine(NULL, info->fix.smem_len, info->screen_base,
				(dma_addr_t )info->fix.smem_start);
	}

	fb_dealloc_cmap(&info->cmap);
err_alloc_cmap:
	iowrite32(0, ftlcdc200->base + FTLCDC200_OFFSET_INT_ENABLE);
	free_irq(irq, info);
err_req_irq:
	iounmap(ftlcdc200->base);
err_ioremap:
err_req_mem_region:
	platform_set_drvdata(pdev, NULL);
	framebuffer_release(info);
err_alloc_info:
err_get_irq:
	release_resource(res);
	return ret;
}

static int __exit ftlcdc200_remove(struct platform_device *pdev)
{
	struct fb_info *info;
	struct ftlcdc200 *ftlcdc200;

	info = platform_get_drvdata(pdev);
	ftlcdc200 = info->par;

	/* disable LCD HW */
	iowrite32(0, ftlcdc200->base + FTLCDC200_OFFSET_INT_ENABLE);
	iowrite32(0, ftlcdc200->base + FTLCDC200_OFFSET_CTRL);

	unregister_framebuffer(info);
	dma_free_writecombine(NULL, info->fix.smem_len, info->screen_base,
				(dma_addr_t )info->fix.smem_start);

	fb_dealloc_cmap(&info->cmap);
	free_irq(ftlcdc200->irq, info);
	iounmap(ftlcdc200->base);
	platform_set_drvdata(pdev, NULL);
	framebuffer_release(info);
	release_resource(ftlcdc200->res);

	return 0;
}

static struct platform_driver ftlcdc200_driver = {
	.probe		= ftlcdc200_probe,
	.remove		= __exit_p(ftlcdc200_remove),

	.driver		= {
		.name	= "ftlcdc200",
		.owner	= THIS_MODULE,
	},
};

/******************************************************************************
 * initialization / finalization
 *****************************************************************************/
static int __init ftlcdc200_init(void)
{
	return platform_driver_register(&ftlcdc200_driver);
}

static void __exit ftlcdc200_exit(void)
{
	platform_driver_unregister(&ftlcdc200_driver);
}

module_init(ftlcdc200_init);
module_exit(ftlcdc200_exit);

MODULE_DESCRIPTION("FTLCDC200 LCD Controller framebuffer driver");
MODULE_AUTHOR("Po-Yu Chuang <ratbert@faraday-tech.com>");
MODULE_LICENSE("GPL");
