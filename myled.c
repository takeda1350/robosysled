#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>

MODULE_AUTHOR("Mitsuki Fujiwara");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("3.0");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;  //アドレスをマッピングするための配列をグローバルで定義

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char c;
	int i;
	if(copy_from_user(&c,buf,sizeof(char)))
		return -EFAULT;
	if(c == '1'){
		gpio_based[7] = 1 << 25; //on
		msleep(2000);
		gpio_based[10] = 1 << 25; //off
		msleep(400);
		gpio_based[7] = 1 << 25; //on
		msleep(100);
		gpio_based[10] = 1 << 25; //off
		msleep(100);
		gpio_based[7] = 1 << 25; //on
		msleep(100);
		gpio_based[10] = 1 << 25; //off
		msleep(1000);
		gpio_based[7] = 1 << 25; //on
		msleep(2000);
		gpio_based[10] = 1 << 25; //off
		msleep(400);
		gpio_based[7] = 1 << 25; //on
		msleep(100);
		gpio_based[10] = 1 << 25; //off
		msleep(100);
		gpio_based[7] = 1 << 25; //on
		msleep(100);
		gpio_based[10] = 1 << 25; //off
	}
	return 1;
}


static struct file_operations led_fops = {
        .owner = THIS_MODULE,
        .write = led_write
};

static int __init init_mod(void)  //カーネルモジュールの初期化
{
	int retval;

	gpio_base = ioremap_nocache(0x3f200000, 0xA0);

	const u32 led = 25;
	const u32 index = led/10;//GPFSEL2
	const u32 shift = (led%10)*3;//15bit
	const u32 mask = ~(0x7 << shift);//11111111111111000111111111111111
	gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);//001: output flag
	//11111111111111001111111111111111

	retval =  alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval < 0){
		printk(KERN_ERR "alloc_chrdev_region failed.\n");
		return retval;
	}
	printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));

        cdev_init(&cdv, &led_fops);
        retval = cdev_add(&cdv, dev, 1);
        if(retval < 0){
                printk(KERN_ERR "cdev_add failed. major:%d, minor:%d",MAJOR(dev),MINOR(dev));
                return retval;
        }
	cls = class_create(THIS_MODULE,"myled");   //ここから追加
        if(IS_ERR(cls)){
                printk(KERN_ERR "class_create failed.");
                return PTR_ERR(cls);
        }
	device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));
	return 0;
}

static void __exit cleanup_mod(void)  //後始末
{
	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__,MAJOR(dev));
}

module_init(init_mod);  //マクロで関数を登録
module_exit(cleanup_mod);  //同上
