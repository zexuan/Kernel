/*
 * EFLAGS: Current Status Register of Processor
 *
 * (C) 2018.08.10 BiscuitOS <buddy.zhang@aliyun.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <demo/debug.h>

#define CF_BIT              0x00
#define PF_BIT              0x02

/*
 * LAHF -- Load Status Flags into AH Register
 *
 *   LAHE
 *
 *   This instruction executes as described above in compatibility mode and
 *   legacy mode.
 *
 *   IF instrcution LAHF
 *       THEN
 *           AH <--- EFLAGS(SF:ZF:0:AF:0:PF:1:CF)
 *   FI
 *
 *   The state of the flags in the EFLAGS register is not affected.
 */
static __unused int eflags_LAHF(void)
{
    unsigned char AH;

    /*
     * Layout of AH register
     *
     * +-----+----------------------------------------+
     * | Bit | Descriptor                             |
     * +-----+----------------------------------------+
     * | 0   | CF flag                                |
     * +-----+----------------------------------------+
     * | 1   | Reserved as 1                          |
     * +-----+----------------------------------------+
     * | 2   | PF flag                                |
     * +-----+----------------------------------------+
     * | 3   | Reserved as 0                          |
     * +-----+----------------------------------------+
     * | 4   | AF flag                                |
     * +-----+----------------------------------------+
     * | 5   | Reserved as 0                          |
     * +-----+----------------------------------------+
     * | 6   | ZF flag                                |
     * +-----+----------------------------------------+
     * | 7   | SF flag                                |
     * +-----+----------------------------------------+
     */
    __asm__ ("lahf\n\r"
             "mov %%ah, %0" : "=m" (AH) :);
    printk("EFLAGS: LAHF to AH: %#x\n", AH);

    return 0;
}

/*
 * SAHF -- Store AH into Flags
 *
 *   SAHF
 *
 *   Loads the SF, ZF, AF, PF, and CF flags of the EFLAGS register with value
 *   from the corresponding bits in the AH register (bit 7, 6, 4, 2, and 0,
 *   respectively). Bit 1, 3, and 5 of register AH are ignored; the
 *   corresponding reserved bits (1, 3, and 5) in the EFLAGS register remain.
 *
 *   IF instrcution SAHF
 *       THEN
 *           EFLAGS(SF:ZF:0:AF:0:PF:1:CF) <---- AH
 *   FI
 *
 *   The SF, Zf, AF, PF, and CF flags are loaded with values from the AH
 *   register. Bits 1, 3, and 5 of the EFLAGS register are unaffected, with
 *   values remaining 1, 0, and 0, respectively.
 */
static __unused int eflags_SAHF(void)
{
    unsigned char AH;

    __asm__ ("lahf\n\r"
             "mov %%ah, %0" : "=m" (AH) :);

    /*
     * Layout of AH register
     *
     * +-----+----------------------------------------+
     * | Bit | Descriptor                             |
     * +-----+----------------------------------------+
     * | 0   | CF flag                                |
     * +-----+----------------------------------------+
     * | 1   | Reserved as 1                          |
     * +-----+----------------------------------------+
     * | 2   | PF flag                                |
     * +-----+----------------------------------------+
     * | 3   | Reserved as 0                          |
     * +-----+----------------------------------------+
     * | 4   | AF flag                                |
     * +-----+----------------------------------------+
     * | 5   | Reserved as 0                          |
     * +-----+----------------------------------------+
     * | 6   | ZF flag                                |
     * +-----+----------------------------------------+
     * | 7   | SF flag                                |
     * +-----+----------------------------------------+
     */
    __asm__ ("sahf" :: "a" (AH));
    return 0;
}

/*
 * PUSHF -- Push EFLAGS Register onto the Stack
 *
 *   PUSHF
 *
 *   Decrements the stack pointer by 4 (if the current operand-size attribute
 *   is 32) and pushes the entire contents of the EFLAGS register onto the 
 *   stack, or decrements the stack pointer by 2 (if the operand-size 
 *   attribute is 16) and pushes the lower 16 bits of the EFLAGS register (
 *   that is, the EFLAGS register) onto the stack. These instructions reverse 
 *   the operation of the POPF instructions.
 *
 *   When copying the entire EFLAGS register to the stack, the VM and RF flags
 *   (bit 16 and bit 17) are not copied; instead, the values for these flags
 *   are cleared in the EFLAGS image stored on the stack.
 *
 *   The PUSHF (push flags) mnemonics reference the same opcode. The PUSHF
 *   instrcution is intended for use when the operand-size attribute is 16
 *   and the PUSHFD instruction for when the operand-size attribute is 32.
 *   Some assemblers may force the operand size to 16 when PUSHF is used
 *   and to 32 when PUSHFD is used. Others may treat these mnemonics as
 *   synonyms (PUSHF/PUSHFD) and used the current setting of the operand-size
 *   attribute to determine the size of values to be pushed from the stack,
 *   regardless of the mnemonic used.
 */
static __unused int eflags_PUSHF(void)
{
    unsigned int EFLAGS;

    __asm__ ("pushf\n\r"
             "popl %%eax" : "=a" (EFLAGS) :);

    printk("EFLAGS: %#x\n", EFLAGS);
    return 0;
}

/*
 * POPF -- Pop Stack into EFLAGS Register
 *
 *   POPF
 *
 *   Pops a doubleword from the top of the stack (if the current operand-size
 *   attribute is 32) and stores the value in the EFLAGS register, or pops
 *   a word from the top of the stack (if the operand-size attribute is 16)
 *   and stores it in the lower 16 bits of the EFLAGS register (that is, the
 *   EFLAGS register). These instructions reverse the operation of the PUSHF
 *   instructions.
 */
static __unused int eflags_POPF(void)
{
    unsigned int EFLAGS;

    __asm__ ("pushf\n\r"
             "popl %%eax" : "=a" (EFLAGS) :);

    __asm__ ("pushl %%eax\n\r"
             "popfl" :: "a" (EFLAGS));
    return 0;
}

/*
 * CF (bit 0) -- Carry flag
 *
 *   Carry flag -- Set if an arithmetic operation generates a carry or a
 *   borrow out of the most-significant bit of the result; cleared otherwise.
 *   This flag indicates an overflow condition for unsigned-integer 
 *   arithmetic. It is also used in multiple-precision arithmetic.
 */
static __unused int eflags_CF(void)
{
    unsigned char __unused EFLAGS;
    unsigned short __unused AX;

#ifdef CONFIG_DEBUG_CF_AAA
    /* 
     * Case0: AAA -- ASCII Adjust After Addition 
     *   Adjust the sum of two unpacked BCD value to create an unpacked BCD
     *   result. The AL register is the implied source and destination
     *   operand for this instruction. The AAA instruction is only useful
     *   when it follows an ADD instruction that adds (binary addition) two
     *   unpacked BCD values and stores a byte result in the AL register. 
     *   The AAA instruction then adjust the contents of the AL register to
     *   contain the correct 1-digit unpacked BCD result.
     *
     *   If the addition produces a decimal carry, the AH register increments
     *   by 1, and the CF and AF flags are set. If there was no decimal 
     *   carry, the CF and AF flags are clear and the AH register is 
     *   unchanged. In either case, bits 4 through 7 of the AL register are
     *   set to 0.
     *
     *   IF instruction AAA
     *       IF ((AL AND 0FH) > 9) or (AF = 1)
     *           THEN
     *               AX <---- AX + 106H
     *               AF <---- 1
     *               CF <---- 1
     *           ELSE
     *               AF <---- 0
     *               CF <---- 0
     *       FI
     *       AL <---- AL AND 0FH
     *   FI
     */ 
    __asm__ ("mov $0x9, %%ax\n\r"
             "add $0x8, %%al\n\r"
             "aaa\n\r"
             "pushf\n\r"
             "pop %0\n\r" 
             "mov %%eax, %1": "=m" (EFLAGS), "=m" (AX) :);
    if (EFLAGS & 0x1)
        printk("CF has carry on AAA instruction. Unpacked BCD: %#x\n", AX);

#endif

    return 0;
}

static int debug_eflags(void)
{
#ifdef CONFIG_DEBUG_EFLAGS_LAHF
    eflags_LAHF();
#endif

#ifdef CONFIG_DEBUG_EFLAGS_SAHF
    eflags_SAHF();
#endif

#ifdef CONFIG_DEBUG_EFLAGS_PUSHF
    eflags_PUSHF();
#endif

#ifdef CONFIG_DEBUG_EFLAGS_POPF
    eflags_POPF();
#endif

#ifdef CONFIG_DEBUG_EFLAGS_CF
    eflags_CF();
#endif

    return 0;
}
late_debugcall(debug_eflags);