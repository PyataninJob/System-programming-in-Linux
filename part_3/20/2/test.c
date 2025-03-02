#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/rwlock.h>
#include <linux/uaccess.h>

// Прототипы функций
ssize_t test_read(struct file *fd, char __user *buff, size_t size, loff_t *off);
ssize_t test_write(struct file *fd, const char __user *buff, size_t size, loff_t *off);

static int major = 0;
static rwlock_t lock;
static char test_string[15] = "Hello!\0";

ssize_t test_read(struct file *fd, char __user *buff, size_t size, loff_t *off)
{
    ssize_t rc = 0;
    size_t available = sizeof(test_string) - *off; // Доступные для чтения байты
    size_t count = min(size, available);          // Сколько можно прочитать

    pr_info("test_read called with size=%zu, off=%lld\n", size, *off);

    if (*off >= sizeof(test_string)) {
        pr_info("Offset %lld is beyond buffer size (%zu). Returning EOF.\n", *off, sizeof(test_string));
        return 0; // Конец файла (EOF)
    }

    read_lock(&lock);
    rc = simple_read_from_buffer(buff, count, off, test_string, sizeof(test_string));
    read_unlock(&lock);

    if (rc < 0) {
        pr_err("Error in simple_read_from_buffer: %zd\n", rc);
        return rc;
    }

    pr_info("Read %zd bytes from buffer. New offset: %lld\n", rc, *off);
    return rc;
}

ssize_t test_write(struct file *fd, const char __user *buff, size_t size, loff_t *off)
{
    ssize_t rc = 0;

    pr_info("test_write called with size=%zu, off=%lld\n", size, *off);

    if (size > sizeof(test_string) - 1) {
        pr_err("Write size exceeds buffer limit (%zu). Returning -EINVAL.\n", sizeof(test_string) - 1);
        return -EINVAL;
    }

    write_lock(&lock);
    rc = simple_write_to_buffer(test_string, sizeof(test_string), off, buff, size);
    write_unlock(&lock);

    if (rc < 0) {
        pr_err("Error in simple_write_to_buffer: %zd\n", rc);
        return rc;
    }

    // Обеспечиваем завершающий нулевой символ
    test_string[sizeof(test_string) - 1] = '\0';

    pr_info("Wrote %zd bytes to buffer. New offset: %lld. Buffer content: '%s'\n", rc, *off, test_string);
    return rc;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = test_read,
    .write = test_write
};

int init_module(void)
{
    pr_info("Test module is loading...\n");

    rwlock_init(&lock);

    major = register_chrdev(major, "test", &fops);
    if (major < 0) {
        pr_err("Failed to register character device: %d\n", major);
        return major;
    }

    pr_info("Test module loaded successfully. Major number: %d\n", major);
    return 0;
}

void cleanup_module(void)
{
    unregister_chrdev(major, "test");
    pr_info("Test module unloaded.\n");
}

MODULE_LICENSE("GPL");