#include <linux/kernel.h>  /* We're doing kernel work */
#include <linux/module.h>  /* Specifically, a module */
#include <linux/proc_fs.h> /* Necessary because we use the proc fs */
#include <linux/uaccess.h> /* for copy_from_user */
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROCFS_MAX_SIZE 1024
#define PROCFS_NAME "my_entry"

/* This structure hold information about the /proc file */
static struct proc_dir_entry *our_proc_file;

/* The buffer used to store character for this module */
static char procfs_buffer[PROCFS_MAX_SIZE];

/* The size of the buffer */
static unsigned long procfs_buffer_size = 0;

static pid_t pid;            // pid recieved
static u64 utime;            // requested value
static unsigned long nvcsw;  // requested value
static unsigned long nivcsw; // requested value

//                 |
// custom function v
static void my_itos(char *str, unsigned long long a)
{
    char temp[50];
    int i1 = 0;
    int i2 = 0;
    int i3 = 0;
    int j = 0;
    for (; i1 < 50; i1++)
    {
        temp[i1] = '\0';
    }
    for (;; i2++)
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

    for (; i3 < 50; i3++)
    {
        if (temp[49 - i3] != '\0')
        {
            str[j] = temp[49 - i3];
            j++;
        }
    }
    for (; j < 50; j++)
    {
        str[j] = '\0';
    }
    if (str[0] == '\0')
    {
        str[0] = '0';
        str[1] = '\0';
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
    // format:<a>;<b>;<c>
    int i = 0;
    int j = 0;
    int k = 0;
    for (; i < my_sizeof(a) - 1; i++)
    {
        rt[i] = a[i];
    }
    rt[i] = ';';
    i++;
    for (; (j + i) < my_sizeof(a) + my_sizeof(b) - 1; j++)
    {
        rt[i + j] = b[j];
    }
    rt[i + j] = ';';
    j++;
    for (; (k + j + i) < my_sizeof(a) + my_sizeof(b) + my_sizeof(b) - 1; k++)
    {
        rt[i + j + k] = c[k];
    }
    rt[i + j + k] = '\0';
}

// custom function ^
//                 |
/* This function is called then the /proc file is read */
static ssize_t procfile_read(struct file *filePointer, char __user *buffer,
                             size_t buffer_length, loff_t *offset)
{
    char m[100];
    int len = sizeof(m);
    ssize_t ret = len;
    char utime_str[50];
    char nvcsw_str[50];
    char nivcsw_str[50];
    // get thread info
    struct task_struct *ts = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (ts == NULL)
    {
        pr_info("unable to find task_struct\n");
        return 0;
    }
    utime = ts->utime;
    nvcsw = ts->nvcsw;
    nivcsw = ts->nivcsw;
    pr_info("=====read call=========\n");
    pr_info("pid: %d\n", ts->pid);
    pr_info("utime: %llu\n", utime);
    pr_info("nvcsw: %lu\n", nvcsw);
    pr_info("nivcsw: %lu\n", nivcsw);
    pr_info("=======================\n");
    // put info into one char* and send back
    my_itos(utime_str, utime);
    pr_info("[myitos utime]:%s\n", utime_str);
    my_itos(nvcsw_str, nvcsw);
    pr_info("[myitos nvcsw]:%s\n", nvcsw_str);
    my_itos(nivcsw_str, nivcsw);
    pr_info("[myitos nivcsw]:%s\n", nivcsw_str);
    // my_itos(utime_str, utime);
    // my_itos(nvcsw_str, nvcsw);
    // my_itos(nivcsw_str, nivcsw);
    my_strcat(m, utime_str, nvcsw_str, nivcsw_str); // format:<utime>;<nvcsw>;<nivcsw>
    pr_info("[mess]:%s\n", m);

    if (*offset >= len || copy_to_user(buffer, m, len))
    {
        pr_info("copy_to_user failed\n");
        ret = 0;
    }
    else
    {
        pr_info("procfile read %s\n", filePointer->f_path.dentry->d_name.name);
        *offset += len;
    }

    return ret;
}

/* This function is called with the /proc file is written. */
static ssize_t procfile_write(struct file *file, const char __user *buff,
                              size_t len, loff_t *off)
{
    int ret;
    unsigned long long res;

    procfs_buffer_size = len;
    if (procfs_buffer_size > PROCFS_MAX_SIZE)
        procfs_buffer_size = PROCFS_MAX_SIZE;

    if (copy_from_user(procfs_buffer, buff, procfs_buffer_size))
        return -EFAULT;

    procfs_buffer[procfs_buffer_size & (PROCFS_MAX_SIZE - 1)] = '\0';
    *off += procfs_buffer_size;
    pr_info("procfile write %s\n", procfs_buffer);
    // char* to int
    ret = kstrtoull_from_user(buff, len, 10, &res);
    if (ret)
    {
        /* Negative error code. */
        pr_info("fetch TID failed = %d\n", ret);
    }
    else
    {
        pid = res; // implicit conversion between unsigned long long and int
        pr_info("TID Success = %u\n", pid);
        *off = len;
    }
    return procfs_buffer_size;
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_file_fops = {
    .proc_read = procfile_read,
    .proc_write = procfile_write,
};
#else
static const struct file_operations proc_file_fops = {
    .read = procfile_read,
    .write = procfile_write,
};
#endif

static int __init procfs2_init(void)
{
    our_proc_file = proc_create(PROCFS_NAME, 0777, NULL, &proc_file_fops);
    if (NULL == our_proc_file)
    {
        proc_remove(our_proc_file);
        pr_alert("Error:Could not initialize /proc/%s\n", PROCFS_NAME);
        return -ENOMEM;
    }

    pr_info("|||||||||||||||||||||||||||||\n");
    pr_info("/proc/%s created\n", PROCFS_NAME);
    return 0;
}

static void __exit procfs2_exit(void)
{
    proc_remove(our_proc_file);
    pr_info("/proc/%s removed\n", PROCFS_NAME);
}

module_init(procfs2_init);
module_exit(procfs2_exit);

MODULE_LICENSE("GPL");