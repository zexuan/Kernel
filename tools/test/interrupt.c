#include <linux/kernel.h>

#ifdef CONFIG_TESTCODE

/*
 * Test interrupt 0 - divide zero
 * The processor generates a type 0 interrupt whenever executing a divide
 * instruction—either 'div' (divide) or 'idiv' (integer divide)—results in 
 * a quotient that is larger than the destination specified. The default 
 * interrupt handler on Linux displays a Floating point exception message
 * and terminates the program.
 */
void test_interrupt0_divide_error(void)
{
#ifndef CONFIG_SOFT_INTERRUPT
#ifdef CONFIG_DIVIDE_ERROR0
	int a;
	int b = 0;
#else
	unsigned short _rax = 0xFFF;
	unsigned short _res;
#endif
	
#ifdef CONFIG_DIVIDE_ERROR0
	/* case 0 */
	/* divide error -> interrupt0 */
	a = 3 / b;	
	b = a;
#else
	/* case 1 */
	/* 
	 * EAX or AX can't store a rightful result-value 
	 * eg, _rax / 0x01 = 0xFFF, and "AL" can't store result '0xFFF'. 
	 */

	__asm__("mov %1, %%ax\n\t"
			"movb $0x1, %%bl\n\t"
			"div %%bl\n\t"
			"movb %%al, %0"
			: "=m" (_res) : "m" (_rax));
	printk("The result %#x\n", _res);
#endif /* CONFIG_DIVIDE_ERROR0 */
#else
	__asm__("int $0");
#endif /* CONFIG_SOFT_INTERRUPT */
}

/*
 * Test interrupt 1 - debug
 * Single-Step Interrupt Single-stepping is a useful debugging tool to
 * observe the behavior of a program instruction by instruction. To start
 * single-stepping, the trap flag (TF) bit in the flags register should 
 * be set (i.e., TF = 1). When TF is set, the CPU automatically generates
 * a type 1 interrupt after executing each instruction. Some exceptions
 * do exist, but we do not worry about them here.
 * The interrupt handler for the type 1 interrupt can be used to display 
 * relevant information about the state of the program. For example, 
 * the contents of all registers could be displayed.
 * To end single stepping, the TF should be cleared. The instruction set, 
 * however, does not have instructions to directly manipulate the TF bit. 
 * Instead, we have to resort to an indirect means. You have to push flags
 * register using pushf and manipulate the TF bit and use popf to store this
 * value back in the flags register. Here is an example code fragment that
 * sets the trap flag.
 */
void test_interrupt1_debug(void)
{
#ifndef CONFIG_SOFT_INTERRUPT
	/* case 0 */
	/*
	 * Set TF on EFLAGS will invoke interrupt 1 (debug).
	 */
	__asm__("pushl %%eax\n\t"
			"pushf\n\t"
			"movl %%esp, %%eax\n\t"
			"orl $0x0100, (%%eax)\n\t" // set TF bit.
			"popf\n\t"
			"popl %%eax"
			::);
#else
	__asm__("int $1");
#endif
}

/*
 * Test Interrupt 3 - int3 
 * Breakpoint Interrupt If you have used a debugger, which you should
 * have by now, you already know the usefulness of inserting breakpoints
 * while debugging a program. The type 3 interrupt is dedicated to the 
 * breakpoint processing. This type of interrupt can be generated by 
 * using the special single-byte form of 'int3' (opcode CCH). Using the
 * 'int3' instruction automatically causes the assembler to encode the 
 * instruction into the single-byte version. Note that the standard encoding
 * for the 'int' instruction is two bytes long.
 * Inserting a breakpoint in a program involves replacing the program 
 * code byte by CCH while saving the program byte for later restoration
 * to remove the breakpoint. The standard 2-byte version of 'int3' can 
 * cause problems in certain situations, as there are instructions that 
 * require only a single byte to encode.
 */
void test_interrupt3_int3(void)
{
#ifndef CONFIG_SOFT_INTERRUPT
	/* general interrupt entry */
	__asm__("pushl %%eax\n\t"
			"pushf\n\t"
			"movl %%esp, %%eax\n\t"
			"orl $0x0100, (%%eax)\n\t"
			"popf\n\t"
			"popl %%eax"
			::);	
#else
	__asm__("int $3");
#endif
}

/*
 * Test Interrupt 4 - overflow error
 * The type 4 interrupt is dedicated to handle overflow conditions. 
 * There are two ways by which a type 4 interrupt can be generated: 
 * either by 'int4' or by 'into' . Like the breakpoint interrupt, 
 * 'into' requires only one byte to encode, as it does not require 
 * the specification of the interrupt type number as part of the 
 * instruction. Unlike 'int4', which unconditionally generates a 
 * type 4 interrupt, 'into' generates a type 4 interrupt only if the
 * overflow flag is set. We do not normally use 'into' , as the 
 * overflow condition is usually detected and processed by using 
 * the conditional jump instructions 'jo' and 'jno'.
 */
void test_interrupt4_overflow(void)
{
#ifndef CONFIG_SOFT_INTERRUPT
	/* case 0 */
	/* 
	 * 'OF' set and call 'into'
	 */
	__asm__("pushl %%ebx\n\t"
			"movb $0x7f, %%bl\n\t"
			"addb $10, %%bl\n\t"
			"into\n\t"
			"popl %%ebx"
			::);
#else
	/* case 1 */
	__asm__("int $4");
#endif
}

#endif
