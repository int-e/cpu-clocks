/*
 * localtime.c -- query scheduler clocks of CPUs
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static int localtime_show(struct seq_file *m, void *v)
{
	int cpu;
	for_each_online_cpu(cpu) {
		seq_printf(m, "%d %llu\n", cpu, cpu_clock(cpu));
	}
	return 0;
}

static int localtime_open(struct inode *inode, struct file *file)
{
	return single_open(file, localtime_show, NULL);
}

static const struct file_operations localtime_fops = {
	.owner = THIS_MODULE,
	.open = localtime_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

int __init init_localtime(void)
{
	proc_create("localtime", 0, NULL, &localtime_fops);
	return 0;
}

void __exit cleanup_localtime(void)
{
	remove_proc_entry("localtime", NULL);
}

module_init(init_localtime);
module_exit(cleanup_localtime);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bertram Felgenhauer <int-e@gmx.de>");
MODULE_DESCRIPTION("Query processor local times.");
MODULE_SUPPORTED_DEVICE("localtime");
