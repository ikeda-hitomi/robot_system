#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/uaccess.h>
#include<linux/io.h>
#include<linux/timer.h>
#include<asm/delay.h>
#include<linux/timer.h>
#include<linux/delay.h>

MODULE_AUTHOR("Hitomi Ikeda and Ryuichi Ueda");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;

static ssize_t led_write(struct file* filp, const char* buf, size_t count,loff_t* pos){

        char c;
        int i;
        if(copy_from_user(&c, buf, sizeof(char)))
        return -EFAULT;

        if(c == '1'){
                gpio_base[10] = 1 << 25;
                msleep(400);
                for(i = 0; i < 15; i++){
                        if(i == 5){
                                gpio_base[7] = 1 << 25;
                                msleep(300);
                                gpio_base[10] = 1 << 25;
                                msleep(600);
                        }else if(i == 2 || i == 8 || i == 12){
                                gpio_base[7] = 1 << 25;
                                msleep(100);
                                gpio_base[10] = 1 << 25;
                                msleep(600);
                        }else{
                                gpio_base[7] = 1 << 25;
                                msleep(100);
                                gpio_base[10] = 1 << 25;
                                msleep(100);
                        }
                        msleep(500);
                   }

                        }else if(c == '0'){

                                gpio_base[10] = 1 << 25;

                        }
        return 1;
}

static ssize_t sushi_read(struct file* filp, char* buf, size_t count,loff_t* pos){

        int size = 0;
        char sushi[] = {'s', 'u', 's', 'h', 'i'};
        if(copy_to_user(buf+size, (const char *)sushi, sizeof(sushi))){
        printk(KERN_ERR "sushi : copy_to_user failed\n");
        return -EFAULT;
        }

        size += sizeof(sushi);
        return size;
}

static struct file_operations led_fops = {

        .owner = THIS_MODULE,
        .write = led_write,
        .read = sushi_read
};

static int __init init_mod(void)
{
        int retval;
        retval = alloc_chrdev_region(&dev, 0 , 1, "myled");
        if(retval < 0){
                printk(KERN_ERR "alloc_chrdev_region fail.\n");
                return retval;
        }

        printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__, MAJOR(dev));

        cdev_init(&cdv, &led_fops);
        retval = cdev_add(&cdv, dev, 1);
        if(retval < 0){
                printk(KERN_ERR "cdev_add failed. major:%d, major:%d", MAJOR(dev), MINOR(dev));
                return retval;
        }

        cls = class_create(THIS_MODULE, "myled");
        if(IS_ERR(cls)){
                printk(KERN_ERR "class_create failed.");
                return PTR_ERR(cls);
        }

        device_create(cls, NULL, dev, NULL, "myled%d", MINOR(dev));

        gpio_base = ioremap_nocache(0x3f200000, 0xA0);

        const u32 led = 25;
        const u32 index = led/10;
        const u32 shift = (led%10)*3;
        const u32 mask = ~(0x7 << shift);
        gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);
        return 0;
}

static void __exit cleanup_mod(void) 
{
        cdev_del(&cdv);
        device_destroy(cls, dev);
        class_destroy(cls);
        unregister_chrdev_region(dev, 1);
        printk(KERN_INFO "%s is unloaded.major:%d\n",__FILE__, MAJOR(dev));
}