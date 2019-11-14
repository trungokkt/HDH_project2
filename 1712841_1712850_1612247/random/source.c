#include <linux/device.h>  // for struct device, class
#include <linux/fs.h>      // for struct file_operations
#include <linux/init.h>    // for macro __init, __exit
#include <linux/kernel.h>  // macro, function for kernel
#include <linux/module.h>  // for loading module

//#include <linux/mutex.h>   // for mutex sync
#include <linux/types.h>  // dev_t (major & minor number)
#include <linux/random.h>  // for function get_random_bytes()
#include <linux/uaccess.h> // for function copy_to_user()


MODULE_LICENSE("GPL"); //giay phep su dung
MODULE_AUTHOR("1712841 - 17128850 - 1612247"); //tac gia
MODULE_DESCRIPTION("Project2 - Linux Kernel Module"); //mo ta chuc nang

static DEFINE_MUTEX(random_mutex);

#define DEVICE_NAME "RANDOM"
#define CLASS_NAME "Random"

static int NumberMin = 0; 
static int NumberMax = 1000000; //so dc tao ngay nhien tu NumberMin  -> NumberMax 
static int majorNumber;
static int Number = 0;

static struct class *random_class = NULL;
static struct device *random_device = NULL;

static int random_open(struct inode*, struct file *);
static int random_release(struct inode*, struct file*);
static ssize_t random_read(struct file*, char*, size_t, loff_t*);

static struct file_operations fops={
    .open = random_open,
    .release = random_release,
    .read = random_read,    
};

static int __init random_init(void){
    printk(KERN_INFO "Welcome to random module init\n");

    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0){
        printk(KERN_ALERT "FAILED! Can't register a major number.\n");
        return majorNumber;
    }
    
    printk(KERN_INFO "SUCCESSED!!Can register a major number %d\n", majorNumber);
    
    random_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(random_class)){
        unregister_chrdev(majorNumber,DEVICE_NAME);
        printk(KERN_ALERT "FAILED! Can't register a class\n");
        return PTR_ERR(random_class);
    }

    random_device = device_create(random_class,NULL,MKDEV(majorNumber,0),NULL,DEVICE_NAME);
    if (IS_ERR(random_device)){
        class_destroy(random_class);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "FAILED! Can't creare a device\n");
        return PTR_ERR(random_device);
    }

    printk(KERN_INFO "RANDOM created\n");

    //mutex_init(&random_mutex);
    return 0;
}

static void __exit random_exit(void){
    //mutex_destroy(&random_mutex);

    device_destroy(random_class, MKDEV(majorNumber, 0));// huy thiet bi
    class_unregister(random_class);// huy dang ky device class
    class_destroy(random_class);// xoa class
    unregister_chrdev(majorNumber, DEVICE_NAME); // huy dang ky device major number
    printk(KERN_INFO "exit\n");
}

static int random_open(struct inode *inodep, struct file *filep){
    if (!mutex_trylock(&random_mutex)){
        printk(KERN_ALERT "Random device is opened!\n");
        return -EBUSY;
    }
    printk(KERN_INFO "RANDOM is opening\n");
    return 0;
}

static ssize_t random_read(struct file *filep, char *buffer, size_t len, loff_t *offest){
    get_random_bytes(&Number, sizeof(Number));
    
    Number = NumberMin + Number%(NumberMax-NumberMin);
    
    printk(KERN_INFO "The random number is: %d\n",Number);

    int stt=0;
    stt=copy_to_user(buffer, &Number, sizeof(Number));
    if (stt==0){
        printk(KERN_INFO "Sent to user the random number\n");
        return 0;       
    }
    else{
        printk(KERN_INFO "Failed to sent to user the random number\n");
        return -EFAULT;
    }
}

static int random_release(struct inode *inodep, struct file *filep){
    //mutex_unlock(&random_mutex);
    printk(KERN_INFO "Released successfully\n");
    return 0;
}

module_init(random_init);
module_exit(random_exit); 