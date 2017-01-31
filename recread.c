#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <asm/uaccess.h>

static int rec_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t rec_read(struct file *filp, char *buf,
		size_t count, loff_t *pos)
{
	char *message = "Read recorded!\n";
	int len = strlen(message);

	if (count < len) {
		return -EINVAL;
	}
	if (*pos != 0) {
		return 0;
	}

	if (copy_to_user(buf, message, len)) {
		return -EINVAL;
	}
	*pos = len;
	return len;
}

static ssize_t rec_write(struct file *filp, const char *buf,
		size_t count, loff_t *pos)
{
	printk(KERN_ERR "Write operation not supported!.\n");
	return -EINVAL;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open  = rec_open,
	.read  = rec_read,
	.write = rec_write,
};

static struct miscdevice recread_dev = {
	MISC_DYNAMIC_MINOR,
	"recread",
	&fops
};

static int __init rec_init(void)
{
	int ret = misc_register(&recread_dev);
	if (ret) {
		printk(KERN_ERR "Unable to register \"recread\" misc device.\n");
	}
	return ret;
}

static void __exit rec_exit(void)
{
	misc_deregister(&recread_dev);
}


module_init(rec_init);
module_exit(rec_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("gabi");
