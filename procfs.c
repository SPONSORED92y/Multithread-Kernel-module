#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/sched.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
#include <linux/minmax.h>
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROCFS_MAX_SIZE 2048UL
#define PROCFS_ENTRY_FILENAME "my_entry"

static struct proc_dir_entry *our_proc_file;
static char procfs_buffer[PROCFS_MAX_SIZE];
static unsigned long procfs_buffer_size = 0;
static pid_t pid = 215;
static u64 utime = 99;
static unsigned long nvcsw = 99;
static unsigned long nivcsw = 99;

static void my_itos(char *str, unsigned long long int a)
{
    char temp[50];
    int i1 = 0;
    for (; i1 < 50; i1++)
    {
        temp[i1] = '\0';
    }
    int i2 = 0;
    for (int i2 = 0;; i2++)
    {
        if (a == 0)
        {
            break;
        }
        switch (a % 10)
        {
        case 0:
            temp[i2] = '0';
            break;
        case 1:
            temp[i2] = '1';
            break;
        case 2:
            temp[i2] = '2';
            break;
        case 3:
            temp[i2] = '3';
            break;
        case 4:
            temp[i2] = '4';
            break;
        case 5:
            temp[i2] = '5';
            break;
        case 6:
            temp[i2] = '6';
            break;
        case 7:
            temp[i2] = '7';
            break;
        case 8:
            temp[i2] = '8';
            break;
        case 9:
            temp[i2] = '9';
            break;
        }
        a /= 10;
    }
    int i3 = 0;
    int j = 0;
    for (; i3 < 50; i3++)
    {
        if (temp[49 - i3] != '\0')
        {
            str[j] = temp[49 - i3];
            j++;
        }
    }
    return;
}

static unsigned int my_sizeof(char *str)
{
    unsigned int len = 0;
    for (; str[len] != '\0'; len++)
        ;
    return len + 1;
}

static void my_strcat(char *rt, char *a, char *b, char *c)
{
    int i = 0;
    int j = 0;
    int k = 0;
    for (; i < my_sizeof(a) - 1; i++)
    {
        rt[i] = a[i];
    }
    rt[i] = ';';
    i++;
    for (; (j + i) < my_sizeof(a) + my_sizeof(b) - 2; j++)
    {
        rt[i + j] = b[j];
    }
    rt[i + j] = ';';
    j++;
    for (; (k + j + i) < my_sizeof(a) + my_sizeof(b) + my_sizeof(b) - 2; k++)
    {
        rt[i + j + k] = c[k];
    }
}

static ssize_t procfs_read(struct file *filp, char __user *buffer,
                           size_t length, loff_t *offset)
{
    // get_thread_info
    struct task_struct *ts = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (ts == NULL)
    {
        pr_info("unable to find task_struct\n");
    }
    utime = ts->utime;
    nvcsw = ts->nvcsw;
    nivcsw = ts->nivcsw;
    pr_info("-------------------------");
    pr_info("pid: %d\n", pid);
    pr_info("utime: %llu\n", utime);
    pr_info("nvcsw: %lu\n", nvcsw);
    pr_info("nivcsw: %lu\n", nivcsw);
    pr_info("-------------------------");
    put_task_struct(ts);
    char utime_str[50];
    char nvcsw_str[50];
    char nivcsw_str[50];
    my_itos(utime_str, utime);
    my_itos(nvcsw_str, nvcsw);
    my_itos(nivcsw_str, nivcsw);
    char *message;
    my_strcat(procfs_buffer, utime_str, nvcsw_str, nivcsw_str);
    //
    if (*offset || procfs_buffer_size == 0)
    {
        pr_debug("procfs_read: END\n");
        *offset = 0;
        return 0;
    }
    procfs_buffer_size = min(procfs_buffer_size, length);
    if (copy_to_user(buffer, procfs_buffer, procfs_buffer_size))
        return -EFAULT;
    *offset += procfs_buffer_size;

    pr_debug("procfs_read: read %lu bytes\n", procfs_buffer_size);
    return procfs_buffer_size;
}
static ssize_t procfs_write(struct file *file, const char __user *buffer,
                            size_t len, loff_t *off)
{
    procfs_buffer_size = min(PROCFS_MAX_SIZE, len);
    if (copy_from_user(procfs_buffer, buffer, procfs_buffer_size))
        return -EFAULT;
    *off += procfs_buffer_size;

    pr_debug("procfs_write: write %lu bytes\n", procfs_buffer_size);

    int ret;
    unsigned long long res;
    ret = kstrtoull_from_user(buffer, len, 10, &res);
    if (ret)
    {
        /* Negative error code. */
        pr_info("fetch TID failed = %d\n", ret);
    }
    else
    {
        pr_info("TID Success = %llu\n", res);
        pid = ret;
        *off = len;
    }
    return procfs_buffer_size;
}
static int procfs_open(struct inode *inode, struct file *file)
{
    try_module_get(THIS_MODULE);
    return 0;
}
static int procfs_close(struct inode *inode, struct file *file)
{
    module_put(THIS_MODULE);
    return 0;
}

#ifdef HAVE_PROC_OPS
static struct proc_ops file_ops_4_our_proc_file = {
    .proc_read = procfs_read,
    .proc_write = procfs_write,
    .proc_open = procfs_open,
    .proc_release = procfs_close,
};
#else
static const struct file_operations file_ops_4_our_proc_file = {
    .read = procfs_read,
    .write = procfs_write,
    .open = procfs_open,
    .release = procfs_close,
};
#endif

static int __init procfs3_init(void)
{
    our_proc_file = proc_create(PROCFS_ENTRY_FILENAME, 0777, NULL,
                                &file_ops_4_our_proc_file);
    if (our_proc_file == NULL)
    {
        remove_proc_entry(PROCFS_ENTRY_FILENAME, NULL);
        pr_debug("Error: Could not initialize /proc/%s\n",
                 PROCFS_ENTRY_FILENAME);
        return -ENOMEM;
    }
    proc_set_size(our_proc_file, 80);
    proc_set_user(our_proc_file, GLOBAL_ROOT_UID, GLOBAL_ROOT_GID);

    pr_debug("/proc/%s created\n", PROCFS_ENTRY_FILENAME);
    return 0;
}

static void __exit procfs3_exit(void)
{
    remove_proc_entry(PROCFS_ENTRY_FILENAME, NULL);
    pr_debug("/proc/%s removed\n", PROCFS_ENTRY_FILENAME);
}

module_init(procfs3_init);
module_exit(procfs3_exit);

MODULE_LICENSE("GPL");