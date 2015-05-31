/*
 * cpu_clocks.c -- query scheduler clocks of CPUs
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static int cpu_clocks_show(struct seq_file *m, void *v)
{
	int cpu;
	for_each_online_cpu(cpu) {
		seq_printf(m, "%d %llu\n", cpu, cpu_clock(cpu));
	}
	return 0;
}

static int cpu_clocks_open(struct inode *inode, struct file *file)
{
	return single_open(file, cpu_clocks_show, NULL);
}

static const struct file_operations cpu_clocks_fops = {
	.owner = THIS_MODULE,
	.open = cpu_clocks_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

int __init init_cpu_clocks(void)
{
	proc_create("cpu_clocks", 0, NULL, &cpu_clocks_fops);
	return 0;
}

void __exit cleanup_cpu_clocks(void)
{
	remove_proc_entry("cpu_clocks", NULL);
}

module_init(init_cpu_clocks);
module_exit(cleanup_cpu_clocks);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bertram Felgenhauer <int-e@gmx.de>");
MODULE_DESCRIPTION("Query processor local times.");
