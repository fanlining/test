#include <linux/clk.h>
#include <linux/gpio.h>
#include <linux/module.h>

#include <sound/soc.h>


/* 参考sound\soc\samsung\s3c24xx_uda134x.c
 */



/*
 * 1. 分配注册一个名为soc-audio的平台设备
 * 2. 这个平台设备有一个私有数据 snd_soc_card
 *    snd_soc_card里有一项snd_soc_dai_link
 *    snd_soc_dai_link被用来决定ASOC各部分的驱动
 */

/*
 必须
 1   必须  snd_soc_dai_link s3c2440_uda1341_dai_link    platform_set_drvdata(&asoc_dev, &myalsa_card);
 2   snd_soc_dai_link s3c2440_uda1341_dai_link   依赖  static struct snd_soc_card myalsa_card
 3   platform_set_drvdata(&asoc_dev, &myalsa_card);  依赖 static struct platform_device asoc_dev = {
*/
static struct snd_soc_ops s3c2440_uda1341_ops = {
	//.hw_params = s3c24xx_uda134x_hw_params,
};

static struct snd_soc_dai_link s3c2440_uda1341_dai_link = {
	.name = "100ask_UDA1341",         //  似乎没用
	.stream_name = "100ask_UDA1341",  //  似乎没用
	.codec_name = "wm8976-codec",     //  对应驱动
	.codec_dai_name = "wm8976-iis",   //  对应驱动
	.cpu_dai_name = "s3c2440-iis",    //  对应驱动  
	.ops = &s3c2440_uda1341_ops,                             //应该使用 声卡会使用的函数
	.platform_name	= "s3c2440-dma",  //  对应驱动  dma
};


static struct snd_soc_card myalsa_card = {
	.name = "S3C2440_UDA1341",
	.owner = THIS_MODULE,
	.dai_link = &s3c2440_uda1341_dai_link,  
	.num_links = 1,
};

static void asoc_release(struct device * dev)
{
}

static struct platform_device asoc_dev = {  
    .name         = "soc-audio",   //  1  导致soc-core.c中的soc_probe函数被调用  2 进而分配／设置／注册 snd_card
    .id       = -1,
    .dev = { 
    	.release = asoc_release, 
	},
};

static int s3c2440_uda1341_init(void)
{
	platform_set_drvdata(&asoc_dev, &myalsa_card);
    platform_device_register(&asoc_dev);  // 内核 4.14 也有么 ！
    return 0;
}

static void s3c2440_uda1341_exit(void)
{
    platform_device_unregister(&asoc_dev);
}

module_init(s3c2440_uda1341_init);
module_exit(s3c2440_uda1341_exit);

MODULE_LICENSE("GPL");
