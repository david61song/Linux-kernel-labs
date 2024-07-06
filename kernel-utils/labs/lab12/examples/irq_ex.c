#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/interrupt.h>

// TODO: To raise IRQ, you must modify kernel codes
// Add the EXPORT_SYMBOL_GPL(vector_irq) after #include definition in arch/x86/kernel/irq.c
// Add the EXPORT_SYMBOL_GPL(irq_to_desc) after irq_to_desc() in kernel/irq/irqdesc.c
// After then, we need to rebuild the linux source: make -j$(nproc)
// Include the asm/hw_irq.h


static irqreturn_t my_handler(int irq_no, void *d) {
	return 0;
}


static int __init init_module(void) {
	//...
	struct irq_desc *desc;
	int err;

	// err = request_irq(MY_IRQ, my_handler, IRQF_SHARED,
	// 		"com1", my_data);

	if (err < 0) {
		/* handle error */
		return err;
	}

	/* Raise IRQ */
	desc = irq_to_desc(IRQ_NO);
	if (!desc) {
		return -EINVAL;
	}
	__this_cpu_write(vector_irq[59], desc);
	asm("int $0x3B");  // Corresponding to irq 11
	//...
}

static void __exit exit_module(void) {
	//free_irq();
}

module_init(init_module);
module_exit(exit_module);
MODULE_LICENSE("GPL");
