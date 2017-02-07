#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/string.h>

#define BUFF_LEN 256

static char content[BUFF_LEN];

static int rec_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "In %s()\n", __FUNCTION__ );
	return 0;
}

static ssize_t rec_read(struct file *filp, char *buffer,
			size_t nbytes, loff_t *ppos)
{
	char message[BUFF_LEN] = "Read recorded! Last written: ";
	size_t len;
	strcat(message, content);
	len = strlen(message);
	printk(KERN_INFO "In %s()\n", __FUNCTION__ );
	if (nbytes < len) {
	printk(KERN_INFO "In %s()\n", __FUNCTION__ );
		return -EINVAL;
	}
	if (*ppos != 0) {
		return 0;
	}
	if (copy_to_user(buffer, message, len)) {
		return -EINVAL;
	}
	*ppos = len;
	return len;
}

static ssize_t rec_write(struct file *filp, const char __user *buffer,
			 size_t nbytes, loff_t *ppos)
{
	u_char message[BUFF_LEN];
	printk(KERN_INFO "In %s()\n", __FUNCTION__ );
	if (copy_from_user(message, buffer, nbytes)) {
		printk(KERN_ERR "Could not write to recread!\n");
		return -EINVAL;
	}

	strcpy(content, message);
	printk(KERN_INFO "Wrote %s\n", message);
	return (ssize_t)nbytes;
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
