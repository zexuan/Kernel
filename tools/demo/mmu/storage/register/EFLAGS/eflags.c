/*
 * EFLAGS: Current Status Register of Processor
 *
 * (C) 2018.08.10 BuddyZhang1 <buddy.zhang@aliyun.com>
 * (C) 2018.09.03 BiscuitOS   <buddy.biscuitos@gmail.com>
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
    unsigned char  __unused CF;
    unsigned short __unused AX;
    unsigned short __unused BX;
    unsigned short __unused CX;
    unsigned short __unused DX;
    unsigned int   __unused EAX;
    unsigned int   __unused EBX;
    unsigned int   __unused ECX;
    unsigned int   __unused EDX;

#ifdef CONFIG_DEBUG_CF_AAA
    /* 
     * AAA -- ASCII Adjust After Addition 
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
     *
     * More information about BCD, see: tools/demo/Data/Base/BCD/README.md
     *
     * JC  -- Jump if Carry bit is set.
     * JNC -- Jump if Carry bit is clear.
     */ 
    __asm__ ("mov $0x9, %%ax\n\r"
             "add $0x1, %%al\n\r"
             "aaa\n\r"
             "jc CF_SET0\n\r"
             "jnc CF_CLEAR0\n\r"
      "CF_SET0:\n\r"
             "mov $1, %%ebx\n\r"
             "jmp out0\n\r"
    "CF_CLEAR0:\n\r"
             "mov $0, %%ebx\n\r"
             "out0:\n\r"
             "mov %%ebx, %0\n\r"
             "mov %%ax, %1" : "=m" (CF), "=m" (AX):);

    if (CF)
        printk("CF has carry on AAA instruction. Unpacked BCD: %#x\n", AX);

#endif

#ifdef CONFIG_DEBUG_CF_AAS
    /*
     * AAS -- ASCII adjust AL after subtraction
     *   Adjust the result of the subtraction of two unpacked BCD value to 
     *   create a unpacked BCD result. The AL register is the implied source
     *   and destination operand for this instruction. The AAS instruction
     *   is only useful when it follows a SUB instruction that subtracts (
     *   binary subtraction) one unpacked BCD value from another and stores
     *   a byte result in the AL register. The AAS instruction then adjusts
     *   the contents of the AL register to contain the correct 1-digit
     *   unpacked BCD result.
     *
     *   If the subtraction produced a decimal carry, the AH register 
     *   decrements by 1, and the CF and AF flags are set. If no decimal
     *   carry occurred, the CF and AF flags are cleared, and the AH register
     *   is unchanged. In either case, the AL register is left with its top
     *   four bits set to 0.
     *
     *   IF ((AL AND 0FH) > 9) or (AF = 1)
     *       THEN
     *           AX <---- AX - 6
     *           AH <---- AH - 1
     *           AF <---- 1
     *           CF <---- 1
     *           AL <---- AL AND 0FH
     *       ELSE
     *           CF <---- 0
     *           AF <---- 0
     *           AL <---- AL AND 0FH
     *   FI
     *
     * More information about BCD, see: tools/demo/Data/Base/BCD/README.md
     *
     * JC  -- Jump if Carry bit is set.
     * JNC -- Jump if Carry bit is clear.
     */
    __asm__ ("mov $8, %%ax\n\r"
             "sub $9, %%al\n\r"
             "aas\n\r"
             "jc CF_SET1\n\r"
             "jc CF_CLEAR1\n\r"
      "CF_SET1:\n\r"
             "mov $1, %%ebx\n\r"
             "jmp out1\n\r"
    "CF_CLEAR1:\n\r"
             "mov $0, %%ebx\n\r"
         "out1:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1" : "=m" (CF), "=m" (AX));

    /*
     * Subtraction is done by adding the ten's complement of the subtrachend.
     * To illustrate signed unpacked BCD subtraction, as "8 - 9":
     * 
     * In signed unpacked BCD, 8 is '0000 1000'. The ten's complement of 9
     * can be obtained by taking the nine's complement of 9, and then adding
     * one. So "9 - 9 = 0", and "0 + 1 = 1" By preceding 1 in BCD by 
     * the negative sign code, the number "-9" can be represented. So, "-9"
     * in signed BCD is "0000 1001 0000 0001".
     *
     * Now that both numbers are represented in signed BCD, they can be added
     * together:
     * 
     *                                      0000 1000
     *                                              8
     *                          + 0000 1001 0000 0001
     *                                    9         1
     *                          ---------------------
     *                          = 0000 1001 0000 1001
     *                                    9         9
     *
     * To represent the sign of a number in BCD, the number "0000" is unsed
     * to represent a positive number, and "1001" is used to represent a 
     * negative number. So adding 6 to the invalid entries result in the 
     * following:
     *
     *                            0000 1001 0000 1001
     *                                    9         9
     *                          + 0000 0000 0000 0000
     *                                    0         0
     *                          ---------------------
     *                          = 0000 1001 0000 1001
     *                                    9         9
     *
     * Thus the result of the subtraction if "0000 1001 0000 1001" 
     * (-9). To check the answer, note that the first digit is 9, which means
     * negative. This seems to be correct, since "8 - 9" should result in a 
     * negative number. To check the rest of the digits, represent them in
     * decimal. "0000 1001" is 9. Then ten's compleent of 9 is "10 - 9"
     * = "9 - 9 + 1" = "0 + 1" = 1. so the calculated answer is "-1".
     */
    if (CF)
        printk("CF has carry on AAS instruction. Unpacked BCD: %#x\n", AX);

#endif

#ifdef CONFIG_DEBUG_CF_ADC
    /*
     * ADC -- Add with Carry
     *
     * Adds the destination operand (first operand), the source operand (
     * second operand), and the carry (CF) flag and stores the result in the
     * destination operand. The destination operand can be a register or a
     * memory location; the source operand can be an immediate, a register,
     * or a memory location. (However, two memory operand cannot be used in
     * one instruction.) The state of the CF flag represents a carry from
     * a previous addition. When an immediate value is used as an operand,
     * it is sign-extended to the length of the destination operand format.
     *
     * The ADC instruction does not distinguish between signed or unsigned
     * operands. Instead, the processor evaluates the result for both data
     * types and sets the OF and CF flags to indicate a carry in the signed
     * unsigned result, respectively. The SF flags indicates the sign of the
     * signed result.
     *
     * DEST <---- DEST + SRC + CF
     *
     */
    __asm__ ("mov $0xFFFF, %%ax\n\r"
             "add $0x1, %%ax\n\r"
             "mov $0x0, %%bx\n\r"
             "adc $0x1, %%bx\n\r"
             "mov %%bx, %0" : "=m" (BX) :);
    printk("ADC: BX = BX(0) + 1 + CF = %#x\n", BX);
#endif

#ifdef CONFIG_DEBUG_CF_ADCX
    /*
     * ADCX -- Unsigned integer addition of two operands with Carry flags
     *
     * Performs an unsigned addition of the destination operand (first 
     * operand), the source operand (second operand) and the carry-flag (CF)
     * and stores the result in the destination operand. The destination 
     * operand is a general purpose register, whereas the source operand
     * can be a general-purpose register or memory location. The state of
     * CF can represent a carry from a previous addition. The instruction
     * sets the CF flag with the carry generated by the unsigned addition
     * of the operands.
     *
     * The ADCX instruction is executed in the context of multi-precision 
     * addition, where we add a series of operands with a carry-chain. At
     * the beginning of a chain of additions, we need to make sure the CF
     * is in a desired initial state. Often, this inital state need to be
     * 0, which can be achieved with an instruction to zero the CF.
     *
     * CF:DEST[31:0] <---- DEST[31:0] + SRC[31:0] + CF
     */
    __asm__ ("movl $0xFFFFFFFF, %%eax\n\r"
             "adcx $0x1, %%eax\n\r"
             "jc CF_SET2\n\r"
             "jnc CF_CLEAR2\n\r"
      "CF_SET2:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out2\n\r"
    "CF_CLEAR2:\n\r"
             "mov $0, %%bx\n\r"
         "out2:\n\r"
             "mov %%bx, %0\n\r"
             "movl %%eax, %1"
             : "=m" (CF), "=m" (EAX) :);
    if (CF)
        printk("ADCX: 0xFFFFFFFF + 0x1 + CF = %#x\n", EAX);
#endif

#ifdef CONFIG_DEBUG_CF_ADD
    /*
     * ADD -- Addition with two operands.
     *
     * Adds the destination operand (first operand) and the source operand (
     * second operand) and then stores the result in the destination operand.
     * The destination operand can be a register or a memory location; the 
     * source operand can be an immediate, a register, or a memory location.
     * (However, two memory operands cannot be used in one instruction.) When
     * an immediate value is used as an operand, it it sign-extended to the
     * length of the destination operand format.
     * 
     * The ADD instruction performs integer addition. It evaluates the result
     * for both signed and unsigned integer operands and sets the CF and
     * OF flags to indicate a carry (overflow) in the signed or unsigned 
     * result, respectively. The SF flag indicates the sign of the signed 
     * result.
     *
     * DEST <---- DEST + SRC
     *
     * The OF, SF, ZF, AF, CF, and PF flags are set according to the result.
     */
    __asm__ ("mov $0xFFFF, %%ax\n\r"
             "add $1, %%ax\n\r"
             "jc CF_SET3\n\r"
             "jnc CF_CLEAR3\n\r"
      "CF_SET3:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out3\n\r"
    "CF_CLEAR3:\n\r"
             "mov $0, %%bx\n\r"
         "out3:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX) :);
    if (CF) 
        printk("ADD: Carry on 0xFFFF + 0x1 = %#x\n", AX);
#endif

#ifdef CONFIG_DEBUG_CF_DAA
    /*
     * DAA -- Decimal Adjust AL after Addition
     *
     * Adjust the sum of two packed BCD values to create a packed BCD result.
     * The AL register is the implied source and destination operand. The 
     * DAA instruction is only useful when it follow an ADD instruction that
     * adds (binary addition) two 2-digit, packed BCD values and stores a
     * byte result in the AL register. The DAA instruction then adjusts the
     * contents of the AL register to contain the correct 2-digit, packed
     * BCD result. If a decimal carry is detected, the CF and AF flags are
     * set accordingly.
     *
     * IF instruction DAA
     *     old_AL <---- AL
     *     old_CF <---- CF
     *     CF     <---- 0
     *     IF (((AL AND 0FH) > 9) or AF = 1)
     *         THEN
     *             AL <---- AL + 6 
     *             CF <---- old_CF or (Carry from AL <---- AL + 6)
     *             AF <---- 1
     *         ELSE
     *             AF <---- 0
     *     FI
     *     IF ((old_AL > 99H) or (old_CF = 1))
     *         THEN
     *             AL <---- AL + 60H
     *             CF <---- 1
     *         ELSE
     *             CF <---- 0
     *     FI
     * FI
     *
     * The CF and AF flags are set if the adjustment of the value results
     * in a decimal carry in either digit of the result. The SF, ZF, and PF
     * flags are set according to the result. The OF flag is undefined.
     */
    CX = 0x79;
    DX = 0x35;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "add %%bl, %%al\n\r"
             "daa\n\r"
             "jc CF_SET4\n\r"
             "jnc CF_CLEAR4\n\r"
      "CF_SET4:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out4\n\r"
    "CF_CLEAR4:\n\r"
             "mov $0, %%bx\n\r"
         "out4:\n\r"
             "mov %%ax, %0\n\r"
             "mov %%bx, %1"
             : "=m" (AX), "=m" (CF) : "m" (CX), "m" (DX));
    if (CF)
        printk("DAA: CF set on adjust after %#x + %#x = %#x\n", CX, DX, AX);
    else
        printk("DAA: CF cleared on adjust after %#x + %#x = %#x\n",
                                CX, DX, AX);
#endif

#ifdef CONFIG_DEBUG_CF_DAS
    /*
     * DAS -- Decimal Adjust AL after Subtraction
     *
     * Adjusts the result of the subtraction of two packed BCD values to 
     * create a packed BCD result. The AL register is the implied source
     * and destination operand. The DAS instruction is only useful when
     * it follows a SUB instruction that subtracts (binary subtraction)
     * one 2-digit, packed BCD value from another and stores a byte result
     * in the AL register. The DAS instruction then adjusts the contents of
     * the AL register to contain the correct 2-digit, packed BCD result.
     * IF a decimal borrow is detected, the CF and AF flags are set 
     * accordingly.
     *
     * IF instruction DAS
     *     old_AL <---- AL
     *     old_CF <---- CF
     *     CF     <---- 0
     *     IF (((AL AND 0FH) > 9) or AF = 1)
     *         THEN
     *             AL <---- AL - 6
     *             CF <---- old_CF or (Borrow from AL <---- AL - 6)
     *             AF <---- 1
     *         ELSE
     *             AF <---- 0
     *     FI
     *     IF ((old_AL > 99H) or (old_CF = 1))
     *         THEN
     *             AL <---- AL - 60H
     *             CF <---- 1
     *     FI
     * FI
     */
    CX = 0x35;
    DX = 0x47;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "sub %%bl, %%al\n\r"
             "das\n\r"
             "jc CF_SET5\n\r"
             "jnc CF_CLEAR5\n\r"
      "CF_SET5:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out5\n\r"
    "CF_CLEAR5:\n\r"
             "mov $0, %%bx\n\r"
         "out5:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX) : "m" (CX), "m" (DX));
    if (CF)
        printk("DAS: CF set on adjust after %#x - %#x = %#x\n",
                                  CX, DX, AX);
    else
        printk("DAS: CF cleared on adjust after %#x - %#x = %#x\n",
                                  CX, DX, AX);
#endif

#ifdef CONFIG_DEBUG_CF_BT
    /*
     * BT -- Bit Test
     *
     * Select the bit in a bit string (specified with the first operand, 
     * called the bit base) at the bit-position designated by the bit offset
     * (specified by the second operand) and stores the value of the bit
     * in the CF flag. The bit base operand can be a register or memory 
     * location; the bit offset operand can be a register or an immediate
     * value:
     *
     * * If the bit base operand specifies a register, the instruction takes
     *   the modulo 16, 32, or 64 of the bit offset operand (modulo size 
     *   depends on the mode and register size).
     *
     * * If the bit base operand specifies a memory location, the operand
     *   represents the address of the byte in memory that contains the bit
     *   base (bit 0 of the specified byte) of the bit string. The range of
     *   the bit position that can be referenced by the offset operand depends
     *   on the operand size.
     *
     * CF <---- Bit(BitBase, BitOffset)
     * 
     * The CF flag contains the value of the selected bit. The ZF flag is 
     * unaffected. The OF, SF, AF, and PF flags are undefined.
     */
    DX = 0x3;
    __asm__ ("mov %1, %%ax\n\r"
             "bt $0, %%ax\n\r"
             "jc CF_SET6\n\r"
             "jnc CF_CLEAR6\n\r"
      "CF_SET6:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out6\n\r"
    "CF_CLEAR6:\n\r"
             "mov $0, %%bx\n\r"
         "out6:\n\r"
             "mov %%bx, %0"
             : "=m" (CF) : "m" (DX));
    if (CF)
        printk("BT: %#x test bit 0 is set\n", DX);
    else
        printk("BT: %#x test bit 0 is cleared\n", DX);
#endif

#ifdef CONFIG_DEBUG_CF_BTC
    /*
     * BTC -- Bit Test and Complement
     *
     * Selects the bit in a bit string (specified with the first operand, 
     * called the bit base) at the bit-position designated by the bit offset
     * operand (second operand), stores the value of the bit in the CF flag,
     * and complements the selected bit in the bit string. The bit base 
     * operand can be a register or a memory location; the bit offset operand
     * can be a register or an immediate value.
     *
     * CF <---- Bit(BitBase, BitOffset)
     * Bit(BitBase, BitOffset) <---- BOT Bit(BitBase, BitOffset)
     *
     * The CF flag contains the value of the selected bit before it is 
     * complemented. The ZF flag is unaffected. The OF, SF, AF, and PF flags
     * are undefined.
     */
    CX = 0x3;
    __asm__ ("mov %2, %%ax\n\r"
             "btc $0, %%ax\n\r"
             "jc CF_SET7\n\r"
             "jnc CF_CLEAR7\n\r"
      "CF_SET7:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out7\n\r"
    "CF_CLEAR7:\n\r"
             "mov $0, %%bx\n\r"
         "out7:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX) : "m" (CX));
    if (CF) 
        printk("BTC: %#x Bit 0 has set and invert bit 0 as %#x\n", CX, AX);
    else
        printk("BTC: %#x Bit 0 has cleared and invert bit 0 as %#x\n", 
                                            CX, AX);
#endif

#ifdef CONFIG_DEBUG_CF_BTR
    /*
     * BTR -- Bit Test and Reset
     *
     * Selector the bit in a bit string (specified with the first operand,
     * called the bit base) at the bit-position designated by the bit offset
     * operand (second operand), stores the value of the bit in the CF flag,
     * and clears the selected bit in the bit string to 0.
     *
     * CF <---- Bit(BitBase, BitOffset)
     * Bit(BitBase, BitOffset) <---- 0
     *
     * The CF flag contains the value of the selected bit before it is 
     * cleared. The ZF flag is unaffected. The OF, SF, AF, and PF flags
     * are undefined.
     */
    DX = 0x203;
    __asm__ ("mov %2, %%ax\n\r"
             "btr $0, %%ax\n\r"
             "jc CF_SET8\n\r"
             "jnc CF_CLEAR8\n\r"
      "CF_SET8:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out8\n\r"
    "CF_CLEAR8:\n\r"
             "mov $0, %%bx\n\r"
         "out8:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX) : "m" (DX));
    if (CF)
        printk("BTR: %#x Bit 0 has set and clear bit 0 as %#x\n", DX, AX);
    else
        printk("BTR: %#x Bit 0 has clear and clear bit 0 as %#x\n", DX, AX);
#endif

#ifdef CONFIG_DEBUG_CF_BTS
    /*
     * BTS -- Bit Test and Set
     *
     * Selects the bit in a bit string (specified with the first operand,
     * called the bit base) at the bit-position designated by the bit offset
     * operand (second operand), stores the value of the bit in the CF flag,
     * and sets the selected bit in the bit string to 1.
     *
     * CF <---- Bit(BitBase, BitOffset)
     * Bit(BitBase, BitOffset) <---- 1
     *
     * The CF flag contains the value of the selected bit before it is set.
     * The ZF flag is unaffected. The OF, SF, AF, and PF flags are undefined.
     */
    DX = 0x203;
    __asm__ ("mov %2, %%ax\n\r"
             "bts $0, %%ax\n\r"
             "jc CF_SET9\n\r"
             "jnc CF_CLEAR9\n\r"
      "CF_SET9:\n\r"             
             "jmp out9\n\r"
             "mov $1, %%bx\n\r"
             "jmp out9\n\r"
    "CF_CLEAR9:\n\r"
             "mov $0, %%bx\n\r"
         "out9:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX) : "m" (DX));
    if (CF)
        printk("BTS: %#x Bit 0 has set and set bit 0 as %#x\n", DX, AX);
    else
        printk("BTS: %#x Bit 0 has clear and set bit 0 as %#x\n", DX, AX);
#endif

#ifdef CONFIG_DEBUG_CF_CLC
    /*
     * CLC -- Clear Carry Flag
     *
     * Clears the CF flag in the EFLAGS register. Operation is the same in
     * all modes.
     *
     * CF <---- 0
     *
     * The CF flag is set to 0. The OF, ZF, SF, AF, and PF flags are
     * unaffected.
     */
    __asm__ ("clc\n\r"
             "jc CF_SET10\n\r"
             "jnc CF_CLEAR10\n\r"
      "CF_SET10:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out10\n\r"
    "CF_CLEAR10:\n\r"
             "mov $0, %%bx\n\r"
         "out10:\n\r"
             "mov %%bx, %0"
             : "=m" (CF) :);
    if (CF)
        panic("CLC: CF doesn't clear on EFLAGS register");
    else
        printk("CLC: CF has cleared on EFLAGS register\n");
#endif

#ifdef CONFIG_DEBUG_CF_STC
    /*
     * STC -- Set Carry Flag
     *
     * Sets the CF flag in the EFLAGS register. Operation is the same in 
     * all modes.
     *
     * CF <---- 1
     *
     * The CF flag is set. The OF, ZF, SF, AF, and PF flags are unaffected.
     */
    __asm__ ("stc\n\r"
             "jc CF_SET11\n\r"
             "jnc CF_CLEAR11\n\r"
      "CF_SET11:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out11\n\r"
    "CF_CLEAR11:\n\r"
             "mov $0, %%bx\n\r"
         "out11:\n\r"
             "mov %%bx, %0"
             : "=m" (CF) :);
    if (CF)
        printk("STC: CF has set on EFLAGS register\n");
    else
        panic("STC: CF has cleared on EFLAGS register");
#endif

#ifdef CONFIG_DEBUG_CF_MUL
    /*
     * MUL -- Unsigned Multiply
     *
     * MUL r/m8        AX       <---- AL * r/m8
     * MUL r/m16       DX:AX    <---- AX * r/m16
     * MUL r/m32       EDX:EAX  <---- EAX * r/m32
     *
     * Performs an unsigned multiplication of the frist operand (destination
     * operand) and the second operand (source operand) and stores the 
     * result in the destination operand. The destination operand is an 
     * implied operand located in register AL, AX or EAX (depending on the
     * size of the operand); the source operand is located in a general-
     * purpose register or a memory location.
     *
     * The result is stored in register AX, register pair DX:AX, or register
     * EDX:EAX (depending on the operand size), with the high-order bits of
     * the product contained in register AH, DX, or EDX, respectively. If the
     * high-order bits of the product are 0, the CF and OF flags are cleared;
     * otherwise, the flags are set.
     *
     * IF (Byte operation)
     *     THEN
     *         AX <---- AL * SRC
     *     ELSE (* Word or doubleword operation *)
     *         IF OperandSize = 16
     *             THEN
     *                 DX:AX <---- AX * SRC
     *             ELSE IF OperandSize = 32
     *                 THEN 
     *                     EDX:EAX <---- EAX * SRC
     *             FI
     *     FI
     * FI
     * 
     * The OF and CF flags are set to 0 if the upper half of the result is
     * 0; otherwise, they are set to 1. The SF, ZF, AF, and PF flags are 
     * undefined.
     */

    /* MUL R/M8 */
    CX = 0x40;
    DX = 0x4;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "mul %%bl\n\r"
             "jc CF_SET12\n\r"
             "jnc CF_CLEAR12\n\r"
      "CF_SET12:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out12\n\r"
    "CF_CLEAR12:\n\r"
             "mov $0, %%bx\n\r"
         "out12:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX) : "m" (CX), "m" (DX));
    if (CF)
        printk("MUL 8: CF has set on %#x * %#x = %#x\n", CX, DX, AX);
    else
        printk("MUL 8: CF has clear on %#x * %#x = %#x\n", CX, DX, AX);

    /* MUL R/M16 */
    CX = 0x1000;
    BX = 0x10;
    __asm__ ("mov %3, %%ax\n\r"
             "mov %4, %%bx\n\r"
             "mul %%bx\n\r"
             "jc CF_SET13\n\r"
             "jnc CF_CLEAR13\n\r"
      "CF_SET13:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out13\n\r"
    "CF_CLEAR13:\n\r"
             "mov $0, %%bx\n\r"
         "out13:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1\n\r"
             "mov %%dx, %2\n\r"
             : "=m" (CF), "=m" (AX), "=m" (DX) 
             : "m" (CX), "m" (BX));
    if (CF)
        printk("MUL 16: CF has set on %#x * %#x = %#x\n", 
                        CX, DX, (DX << 16) | AX);
    else
        printk("MUL 16: CF has cleared on %#x * %#x = %#x\n", CX, DX, 
                                 (DX << 16) | AX);
    
    /* MUL R/M32 */
    ECX = 0x100000;
    EBX = 0x1000; 
    __asm__ ("movl %3, %%eax\n\r"
             "movl %4, %%ebx\n\r"
             "mul %%ebx\n\r"
             "jc CF_SET14\n\r"
             "jnc CF_CLEAR14\n\r"
    "CF_SET14:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out14\n\r"
  "CF_CLEAR14:\n\r"
             "mov $0, %%bx\n\r"
       "out14:\n\r"
             "mov %%bx, %0\n\r"
             "movl %%edx, %1\n\r"
             "movl %%eax, %2"
             : "=m" (CF), "=m" (EDX), "=m" (EAX)
             : "m" (ECX), "m" (EBX));
    if (CF)
        printk("MUL 32: CF has set on %#x * %#x = %#x%08x\n", 
                                ECX, EBX, EDX, EAX);
    else
        printk("MUL 32: CF has clear on %#x * %#x = %#x%08x\n",
                                ECX, EBX, EDX, EAX);
#endif

#ifdef CONFIG_DEBUG_CF_SUB
    /*
     * SUB -- Subtract
     *
     * SUB AL, imm8
     * SUB AX, imm16
     * SUB EAX, imm32
     *
     * Subracts the second operand (source operand) from the first operand (
     * destination operand) and stores the result in the destination operand.
     * The destination operand can be a register or a memory location; the 
     * source operand can be an immediate, register, or memory location. (
     * However, two memory operands cannot be used in one instruction.) When
     * an immediate value is used as an operand, it is sign-extended to the
     * length of the destination operand format.
     *
     * The SUB instruction performs integer subtraction. It evaluates the 
     * result for both signed and unsigned integer operands and sets the
     * OF and CF flags to indicate an overflow in the signed or unsigned
     * result, respectively. The SF flag indicates the sign of the signed
     * result.
     *
     * DEST <---- (DEST - SRC)
     *
     * The OF, SF, ZF, AF, PF, and CF flags are set according to the result.
     */
    /* SUB AL, imm8 */
    CX = 0x1;
    DX = 0x2;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "sub %%bl, %%al\n\r"
             "jc CF_SET15\n\r"
             "jnc CF_CLEAR15\n\r"
    "CF_SET15:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out15\n\r"
  "CF_CLEAR15:\n\r"
             "mov $0, %%bx\n\r"
       "out15:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (CF)
        printk("SUB  8: CF has set on %#x - %#x = %#x\n", CX, DX, 
                                            ~(AX & 0xFF) + 1);
    else
        printk("SUB  8: CF has cleared on %#x - %#x = %#x\n", CX, DX, 
                                            AX & 0xFF);

    /* SUB AX, imm16 */
    CX = 0x1000;
    DX = 0x2000;
    __asm__ ("mov %2, %%ax\n\r"
             "mov %3, %%bx\n\r"
             "sub %%bx, %%ax\n\r"
             "jc CF_SET16\n\r"
             "jnc CF_CLEAR16\n\r"
    "CF_SET16:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out16\n\r"
  "CF_CLEAR16:\n\r"
             "mov $0, %%bx\n\r"
       "out16:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (CF)
        printk("SUB 16: CF has set on %#x - %#x = %#x\n", CX, DX, ~(AX) + 1);
    else
        printk("SUB 16: CF has cleared on %#x - %#x = %#x\n", CX, DX, AX);

    /* SUB EAX, imm32 */
    ECX = 0x100000;
    EDX = 0x200000;
    __asm__ ("movl %2, %%eax\n\r"
             "movl %3, %%ebx\n\r"
             "subl %%ebx, %%eax\n\r"
             "jc CF_SET17\n\r"
             "jnc CF_CLEAR17\n\r"
    "CF_SET17:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out17\n\r"
  "CF_CLEAR17:\n\r"
             "mov $0, %%bx\n\r"
       "out17:\n\r"
             "mov %%bx, %0\n\r"
             "movl %%eax, %1"
             : "=m" (CF), "=m" (EAX)
             : "m" (ECX), "m" (EDX));
    if (CF)
        printk("SUB 32: CF has set on %#x - %#x = %#x\n", ECX, EDX,
                                          ~(EAX) + 1);
    else
        printk("SUB 32: CF has cleared on %#x - %#x = %#x\n", ECX, EDX, EAX);
#endif 

#ifdef CONFIG_DEBUG_CF_SBB
    /*
     * SBB -- Integer Subtraction with Borrow
     *
     * SBB AL, imm8
     * SBB AX, imm16
     * SBB EAX, imm32
     *
     * Adds the source operand (second operand) and the carry (CF) flag, and
     * subtracts the result from the destination operand (first operand). 
     * The result of the subtraction is stored in the destination operand. 
     * The destination operand can be register or a memory location; the
     * source operand can be an immediate, a register, or a memory location.
     * (However, two memory operands cannot be used in one instruction.) The
     * state of the CF flag represents a borrow from a previous subtraction.
     *
     * When an immediate value is used as an operand, it is sign-extended to 
     * the length of the detination operand format.
     *
     * The SBB instruction does not distinguish between signed or unsigned
     * operands. Instead, the processor evaluates the result for both data
     * types and sets the OF and CF flags to indicate a borrow in the signed
     * or unsigned result, respectively. The SF flag flag indicates the
     * sign of the sign of the signed result.
     *
     * The SBB instruction is usually executed as part of a multibyte or 
     * multiword subtraction in which a SUB instruction is followed by a
     * SBB instruction.
     *
     * DEST <---- (DEST  - (SRC + CF))
     *
     * The OF, SF, ZF, AF, PF, and CF flags are set according to the result.
     */

    /* SBB AL, imm8 */
    CX = 0x5;
    DX = 0x2;
    __asm__ ("stc\n\r"
             "mov %1, %%al\n\r"
             "mov %2, %%bl\n\r"
             "sbb %%bl, %%al\n\r"
             "mov %%ax, %0"
             : "=m" (AX)
             : "m" (CX), "m" (DX));
    printk("SBB  8: Subtraction with borrow from %#x - %#x = %#x\n", CX, DX,
                                          AX & 0xFF);

   /* SBB AX, imm16 */
   CX = 0x2000;
   DX = 0x1000;
   __asm__ ("stc\n\r"
            "mov %1, %%ax\n\r"
            "mov %2, %%bx\n\r"
            "sbb %%bx, %%ax\n\r"
            "mov %%ax, %0"
            : "=m" (AX) : "m" (CX), "m" (DX));
    printk("SBB 16: Subtraction with borrow from %#x - %#x = %#x\n",
                                 CX, DX, AX);

    /* SBB EAX, imm32 */
    ECX = 0x200000;
    EDX = 0x100000;
    __asm__ ("stc\n\r"
             "movl %1, %%eax\n\r"
             "movl %2, %%ebx\n\r"
             "sbbl %%ebx, %%eax\n\r"
             "movl %%eax, %0" 
             : "=m" (EAX) : "m" (ECX), "m" (EDX));
    printk("SBB 32: Subtraction with borrow from %#x - %#x = %#x\n",
                             ECX, EDX, EAX);
#endif

#ifdef CONFIG_DEBUG_CF_SHL
    /*
     * SHL -- Shift logical Left
     *
     * SHL r/m8,  cl
     * SHL r/m16, cl
     * SHL r/m32, cl
     *
     * Shifts the bits in the first operand (destination operand) to the left
     * by the number of bits specified in the second operand (count operand).
     * Bits shifted beyond the destination operand boundary are first shifted
     * into the CF flag, then discared. At the end of the shift operation, the
     * CF flag contains the last bit shifted out of the destination operand.
     * 
     * The shift logical left (SHL) instruction perform the same operation;
     * they shift the bits in the destination operand to the left (toward
     * more significant bit locations). For each shift count, the most 
     * significant bit of the destination operand is shifted into the CF flag,
     * and the least significant bit is cleared.
     */

    /* SHL r/m8, CL */
    CX = 0x8E;
    DX = 0x1;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%cl\n\r"
             "shl %%cl, %%al\n\r"
             "jc CF_SET18\n\r"
             "jnc CF_CLEAR18\n\r"
    "CF_SET18:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out18\n\r"
  "CF_CLEAR18:\n\r"
             "mov $0, %%bx\n\r"
       "out18:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (CF)
        printk("SHL  8: CF has set on %#x << %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SHL  8: CF has clear on %#x << %#x = %#x\n", CX, DX, AX & 0xFF);

    /* SHL r/m16, CL */
    CX = 0x8001;
    DX = 0x1;
    __asm__ ("mov %2, %%ax\n\r"
             "mov %3, %%cl\n\r"
             "shl %%cl, %%ax\n\r"
             "jc CF_SET19\n\r"
             "jnc CF_CLEAR19\n\r"
    "CF_SET19:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out19\n\r"
  "CF_CLEAR19:\n\r"
             "mov $0, %%bx\n\r"
       "out19:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (CF)
        printk("SHL 16: CF has set on %#x << %#x = %#x\n", CX, DX, AX);
    else
        printk("SHL 16: CF has cleared on %#x << %#x = %#x\n", CX, DX, AX);

    /* SHL r/m32, CL */
    ECX = 0x80000010;
    EDX = 0x1;
    __asm__ ("movl %2, %%eax\n\r"
             "mov %3, %%cl\n\r"
             "shl %%cl, %%eax\n\r"
             "jc CF_SET20\n\r"
             "jnc CF_CLEAR20\n\r"
    "CF_SET20:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out20\n\r"
  "CF_CLEAR20:\n\r"
             "mov $0, %%bx\n\r"
       "out20:\n\r"
             "mov %%bx, %0\n\r"
             "movl %%eax, %1"
             : "=m" (CF), "=m" (EAX)
             : "m" (ECX), "m" (EDX));
    if (CF)
        printk("SHL 32: CF has set on %#x << %#x = %#x\n", ECX, EDX, EAX);
    else
        printk("SHL 32: CF has cleared on %#x << %#x = %#x\n", ECX, EDX, EAX);
#endif

#ifdef CONFIG_DEBUG_CF_SHR
    /*
     * SHR --- Shift logical right
     *
     * SHR r/m8, CL
     * SHR r/m16, CL
     * SHR r/m32, CL
     *
     * Shift the bit the first operand (destination operand) to the right by
     * the number of bits specified in the second operand (count operand).
     * Bits shifted beyond the destination operand boundary are first shift
     * into the CF flag, hten discared. At the end of the shift operation, 
     * the CF flag contains the last bit shifted out of the destination 
     * operand.
     *
     * The shift logical right (SHR) instrution shift the bits of the 
     * destination operand to the right (toward less significant bit location).
     * For each shift count, the least significant bit of the destination
     * operand is shifted into the CF flag, and the most significant bit is
     * either set or cleared depending on the instruction type. 
     */

    /* SHR r/m8, CL */
    CX = 0x03;
    DX = 0x01;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%cl\n\r"
             "shr %%cl, %%al\n\r"
             "jc CF_SET21\n\r"
             "jnc CF_CLEAR21\n\r"
    "CF_SET21:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out21\n\r"
  "CF_CLEAR21:\n\r"
             "mov $0, %%bx\n\r"
       "out21:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (CF)
        printk("SHR  8: CF has set on %#x >> %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SHR  8: CF has clear on %#x >> %#x = %#x\n",
                                         CX, DX, AX & 0xFF);
    /* SHR r/m16, CL */
    CX = 0x101;
    DX = 0x1;
    __asm__ ("mov %2, %%ax\n\r"
             "mov %3, %%cl\n\r"
             "shr %%cl, %%ax\n\r"
             "jc CF_SET22\n\r"
             "jnc CF_CLEAR22\n\r"
    "CF_SET22:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out22\n\r"
  "CF_CLEAR22:\n\r"
             "mov $0, %%bx\n\r"
       "out22:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (CF)
        printk("SHR 16: CF has set on %#x >> %#x = %#x\n", CX, DX, AX);
    else
        printk("SHR 16: CF has cleared on %#x >> %#x = %#x\n", CX, DX, AX);
    
    /* SHR r/m32, CL */
    ECX = 0x1000001;
    EDX = 0x1;
    __asm__ ("mov %2, %%eax\n\r"
             "mov %3, %%cl\n\r"
             "shrl %%cl, %%eax\n\r"
             "jc CF_SET23\n\r"
             "jnc CF_CLEAR23\n\r"
    "CF_SET23:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out23\n\r"
  "CF_CLEAR23:\n\r"
             "mov $0, %%bx\n\r"
       "out23:\n\r"
             "mov %%bx, %0\n\r"
             "movl %%eax, %1"
             : "=m" (CF), "=m" (EAX)
             : "m" (ECX), "m" (EDX));
    if (CF)
        printk("SHR 32: CF has set on %#x >> %#x = %#x\n", ECX, EDX, EAX);
    else
        printk("SHR 32: CF has cleared on %#x >> %#x = %#x\n", ECX, EDX, EAX);
#endif

#ifdef CONFIG_DEBUG_CF_SAL
    /*
     * SAL -- Shift arithmetic left
     * 
     * SAL r/m8, CL
     * SAL r/m16, CL
     * SAL r/m32, CL
     *
     * Shifts the bits in the first operand (destination operand) to the left
     * by the number of bits specified in the second operand (count operand).
     * Bits shifted beyond the destination operand boundary are first shifted
     * into the CF flag, then discarded. At the end of the shift operation, 
     * the CF flag contains the last bit shifted out of the destination 
     * operand.
     *
     * The shift arithmetic left (SAL) and shift logical left (SHL) 
     * instructions perform the same operation; they shift the bits in the 
     * destination operand to the left (toward more significant bit locations).
     * For each shift count, the most significant bit of the destination 
     * operand is shifted into the CF flag, and the least significant bit is
     * cleared.
     */
    /* SAL r/m8, CL */
    CX = 0x80;
    DX = 0x1;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%cl\n\r"
             "sal %%cl, %%al\n\r"
             "jc CF_SET24\n\r"
             "jnc CF_CLEAR24\n\r"
    "CF_SET24:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out24\n\r"
  "CF_CLEAR24:\n\r"
             "mov $0, %%bx\n\r"
       "out24:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (CF)
        printk("SAL  8: CF has set on %#x << %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SAL  8: CF has cleared on %#x << %#x = %#x\n", CX, DX, 
                                  AX & 0xFF);

    /* SAL r/m16, CL */
    CX = 0x8000;
    DX = 0x1;
    __asm__ ("mov %2, %%ax\n\r"
             "mov %3, %%cl\n\r"
             "sal %%cl, %%ax\n\r"
             "jc CF_SET25\n\r"
             "jnc CF_CLEAR25\n\r"
    "CF_SET25:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out25\n\r"
  "CF_CLEAR25:\n\r"
             "mov $0, %%bx\n\r"
       "out25:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (CF)
        printk("SAL 16: CF has set on %#x << %#x = %#x\n", CX, DX, AX);
    else
        printk("SAL 16: CF has cleared on %#x << %#x = %#x\n", CX, DX, AX);

    /* SAL r/m32, CL */
    ECX = 0x80000000;
    EDX = 0x01;
    __asm__ ("movl %2, %%eax\n\r"
             "movl %3, %%ecx\n\r"
             "sal %%cl, %%eax\n\r"
             "jc CF_SET26\n\r"
             "jnc CF_CLEAR26\n\r"
    "CF_SET26:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out26\n\r"
  "CF_CLEAR26:\n\r"
             "mov $0, %%bx\n\r"
       "out26:\n\r"
             "mov %%bx, %0\n\r"
             "movl %%eax, %1"
             : "=m" (CF), "=m" (EAX)
             : "m" (ECX), "m" (EDX));
    if (CF)
        printk("SAL 32: CF has set on %#x << %#x = %#x\n", ECX, EDX, EAX);
    else
        printk("SAL 32: CF has cleared on %#x << %#x = %#x\n", ECX, EDX, EAX);
#endif

#ifdef CONFIG_DEBUG_CF_SAR
    /*
     * SAR -- Shift arithmetic right
     *
     * SAR r/m8, CL
     * SAR r/m16, CL
     * SAR r/m32, CL
     *
     * Shifts the bits in the first operand (destination operand) to the right
     * by the number of bits specified in the second operand (count operand).
     * Bits shifted beyond the destination operand boundary are first into
     * the CF flag, then discarded. At the end of the shift operation, the 
     * CF flag contains the last bit shifted out of the destination operand.
     *
     * The shift arithmetic right (SAR) instruction shift the bits of the 
     * bits of the destination operand to the right (toward less significant
     * bit locations). For each shift count, the least significant bit of the
     * destination operand is shifted into the CF flag, and the most 
     * significant bit is either set or clear depending on the instruction
     * type. The SAR instruction sets or clears the most significant bit to
     * correspond to the sign (most significant bit) of the original value
     * in the destination operand. In effect, the SAR instruction fills the
     * empty bit position's shifted value with the sign of the unshifted value.
     */

    /* SAR r/m8, CL */
    CX = 0x83;
    DX = 0x1;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%cl\n\r"
             "sar %%cl, %%al\n\r"
             "jc CF_SET27\n\r"
             "jnc CF_CLEAR27\n\r"
    "CF_SET27:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out27\n\r"
  "CF_CLEAR27:\n\r"
             "mov $0, %%bx\n\r"
       "out27:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (CF)
        printk("SAR  8: CF has set on %#X >> %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SAR  8: CF has cleared on %#x >> %#x = %#x\n", CX, DX, 
                              AX & 0xFF);

    /* SAR r/m16, CL */
    CX = 0x8003;
    DX = 0x1;
    __asm__ ("mov %2, %%ax\n\r"
             "mov %3, %%cl\n\r"
             "sar %%cl, %%ax\n\r"
             "jc CF_SET28\n\r"
             "jc CF_CLEAR28\n\r"
    "CF_SET28:\n\r"
             "mov %1, %%bx\n\r"
             "jmp out28\n\r"
  "CF_CLEAR28:\n\r"
             "mov $0, %%bx\n\r"
       "out28:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (CF)
        printk("SAR 16: CF has set on %#x >> %#x = %#x\n", CX, DX, AX);
    else
        printk("SAR 16: CF has cleared on %#x >> %#x = %#x\n", CX, DX, AX);

    /* SAR r/m32, CL */
    ECX = 0x80000003;
    EDX = 0x01;
    __asm__ ("movl %2, %%eax\n\r"
             "movl %3, %%ecx\n\r"
             "sar %%cl, %%eax\n\r"
             "jc CF_SET29\n\r"
             "jnc CF_CLEAR29\n\r"
    "CF_SET29:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out29\n\r"
  "CF_CLEAR29:\n\r"
             "mov $0, %%bx\n\r"
       "out29:\n\r"
             "mov %%bx, %0\n\r"
             "movl %%eax, %1"
             : "=m" (CF), "=m" (EAX)
             : "m" (ECX), "m" (EDX));
    if (CF)
        printk("SAR 32: CF has set on %#x >> %#x = %#x\n", ECX, EDX, EAX);
    else
        printk("SAR 32: CF has cleared on %#x >> %#x = %#x\n", ECX, EDX, EAX);
#endif

#ifdef CONFIG_DEBUG_CF_RCL
    /*
     * RCL -- Rotate through carry left
     *
     * RCL r/m8, CL
     * RCL r/m16, CL
     * RCL r/m32, CL
     *
     * Shifts (rotate) the bits of the first operand (destination operand) the
     * number of bit positions specified in the second operand (count operand)
     * and stores the result in the destination operand. The destination 
     * operand can be a register or a memory location; the count operand is
     * an unsinged integer that can be an immediate or a value in the CL
     * register. The count is marked to 5 bits.
     *
     * The rotate left through carry left (RCL) instruction shift all the bits
     * toward more-significant bit positions, except for the most-significant
     * bit, which is rotated to the least-significant bit location. The RCL
     * instruction include the CF flag in the rotation. The RCL instruction 
     * shifts the CF flag into the least-significant bit and shifts the most-
     * significant bit into the CF flag.
     */
    /* RCL r/m8, CL */
    CX = 0x80;
    DX = 0x4;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%cl\n\r"
             "stc\n\r"
             "rcl %%cl, %%al\n\r"
             "jc CF_SET30\n\r"
             "jnc CF_CLEAR30\n\r"
    "CF_SET30:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out30\n\r"
  "CF_CLEAR30:\n\r"
             "mov $0, %%bx\n\r"
       "out30:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (CF)
        printk("RCL  8: CF set on %#x << %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("RCL  8: CF clear on %#x << %#x = %#x\n", CX, DX, 
                                                            AX & 0xFF);

    /* RCL r/m16, CL */
    CX = 0x8000;
    DX = 0x04;
    __asm__ ("mov %2, %%ax\n\r"
             "mov %3, %%cl\n\r"
             "stc\n\r"
             "rcl %%cl, %%ax\n\r"
             "jc CF_SET31\n\r"
             "jnc CF_CLEAR31\n\r"
    "CF_SET31:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out31\n\r"
  "CF_CLEAR31:\n\r"
             "mov %%bx, %0\n\r"
       "out31:\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (CF)
        printk("RCL 16: CF set on %#x << %#x = %#x\n", CX, DX, AX);
    else
        printk("RCL 16: CF clear on %#x << %#x = %#x\n", CX, DX, AX);

    /* RCL r/m32, CL */
    ECX = 0x80000000;
    EDX = 0x04;
    __asm__ ("movl %2, %%eax\n\r"
             "movl %3, %%ecx\n\r"
             "stc\n\r"
             "rcl %%cl, %%eax\n\r"
             "jc CF_SET32\n\r"
             "jnc CF_CLEAR32\n\r"
    "CF_SET32:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out32\n\r"
  "CF_CLEAR32:\n\r"
             "mov $0, %%bx\n\r"
       "out32:\n\r"
             "mov %%bx, %0\n\r"
             "movl %%eax, %1"
             : "=m" (CF), "=m" (EAX)
             : "m" (ECX), "m" (EDX));
    if (CF)
        printk("RCL 32: CF set on %#x << %#x = %#x\n", ECX, EDX, EAX);
    else
        printk("RCL 32: CF clear on %#x << %#x = %#x\n", ECX, EDX, EAX);
#endif

#ifdef CONFIG_DEBUG_CF_RCR
    /*
     * RCR -- Rotate through carry right
     *
     * RCR r/m8, CL
     * RCR r/m16, CL
     * RCR r/m32, CL
     *
     * Shifts (rotates) the bits of the first operand (destination operand) 
     * the number of bit positions specified in the second operand (count
     * operand) and stores the result in the destination operand. The 
     * destination operand can be a register or a memory location; the count
     * operand is an unsigned integer that can be an immediate or a value in
     * the CL register.
     *
     * The rotate right (ROR) and rotate throght carry right (CRC) instruction
     * shift all the bits toward less significant bit positions, except for
     * the least-significant bit, which is rotated to the most-significant bit
     * location. The RCR instructions include the CF flag in the rotation.
     * The RCR instruction shifts the CF flag into the most-significant bit
     * and shifts the least-significant bit into the CF flag.
     */

    /* RCR r/m8, CL */
    CX = 0x7;
    DX = 0x04;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%cl\n\r"
             "stc\n\r"
             "rcr %%cl, %%al\n\r"
             "jc CF_SET33\n\r"
             "jnc CF_CLEAR33\n\r"
    "CF_SET33:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out33\n\r"
  "CF_CLEAR33:\n\r"
             "mov $0, %%bx\n\r"
       "out33:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (CF)
        printk("RCR  8: CF set on %#x >> %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("RCR  8: CF clear on %#x >> %#x = %#x\n", CX, DX, AX & 0xFF);

    /* RCR r/m16, CL */
    CX = 0x8007;
    DX = 0x04;
    __asm__ ("mov %2, %%ax\n\r"
             "mov %3, %%cl\n\r"
             "stc\n\r"
             "rcr %%cl, %%ax\n\r"
             "jc CF_SET34\n\r"
             "jnc CF_CLEAR34\n\r"
    "CF_SET34:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out34\n\r"
  "CF_CLEAR34:\n\r"
             "mov $0, %%bx\n\r"
       "out34:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (CF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (CF)
        printk("RCR 16: CF set on %#x >> %#x = %#x\n", CX, DX, AX);
    else
        printk("RCR 16: CF clear on %#x >> %#x = %#x\n", CX, DX, AX);

    /* RCR r/m32, CL */
    ECX = 0x80000007;
    EDX = 0x04;
    __asm__ ("movl %2, %%eax\n\r"
             "movl %3, %%ecx\n\r"
             "stc\n\r"
             "rcr %%cl, %%eax\n\r"
             "jc CF_SET35\n\r"
             "jnc CF_CLEAR35\n\r"
    "CF_SET35:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out35\n\r"
  "CF_CLEAR35:\n\r"
             "mov %%bx, %0\n\r"
       "out35:\n\r"
             "movl %%eax, %1"
             : "=m" (CF), "=m" (EAX)
             : "m" (ECX), "m" (EDX));
    if (CF)
        printk("RCR 17: CF set on %#x >> %#x = %#x\n", ECX, EDX, EAX);
    else
        printk("RCR 17: CF clear on %#x >> %#x = %#x\n", ECX, EDX, EAX);
#endif

#ifdef CONFIG_DEBUG_CF_ROR
    /*
     * ROR -- Rotate right
     *
     * ROR r/m8,  CL
     * ROR r/m16, CL
     * ROR r/m32, CL
     *
     * Shifts (rotate) the bits of the first operand (destination operand) the
     * number of bit positions specified in the second operand (count operand)
     * and stores the result in the destination operand. The destination
     * operand can be a register or a memory location; the count operand is
     * an unsigned integer that can be an immediate or a value in the CL
     * register. 
     *
     * The rotate right (ROR) instruction shift all the bit toward less 
     * significant bit positions, except for the least-significant bit, which
     * is rotated to the most-significant bit location. For the ROR 
     * instruction, the original value of the CF flag is not a part of the
     * result, but the CF flag recevices a copy of the bit taht was shifted
     * from one end to the other.
     */

    /* ROR r/m8, CL */
    CX = 0x81;
    DX = 0x01;
    __asm__ ("mov %1, %%al\n\r"
             "mov %2, %%cl\n\r"
             "stc\n\r"
             "ror %%cl, %%al\n\r"
             "mov %%ax, %0"
             : "=m" (AX) : "m" (CX), "m" (DX));
    printk("ROR  8: %#x >> %#x = %#x\n", CX, DX, AX & 0xFF);

    /* ROR r/m16, CL */
    CX = 0x8001;
    DX = 0x1;
    __asm__ ("mov %1, %%ax\n\r"
             "mov %2, %%cl\n\r"
             "stc\n\r"
             "ror %%cl, %%ax\n\r"
             "mov %%ax, %0"
             : "=m" (AX) : "m" (CX), "m" (DX));
    printk("ROR 16: %#x >> %#x = %#x\n", CX, DX, AX);

    /* ROR r/m32, CL */
    ECX = 0x80000001;
    EDX = 0x1;
    __asm__ ("movl %1, %%eax\n\r"
             "movl %2, %%ecx\n\r"
             "stc\n\r"
             "ror %%cl, %%eax\n\r"
             "movl %%eax, %0"
             : "=m" (EAX) : "m" (ECX), "m" (EDX));
    printk("ROR 32: %#x >> %#x = %#x\n", ECX, EDX, EAX);
#endif

#ifdef CONFIG_DEBUG_CF_ROL
    /*
     * ROL -- Rotate left
     *
     * ROL r/m8, CL
     * ROL r/m16, CL
     * ROL r/m32, CL
     *
     * Shifts (rotates) the bits of the first operand (destination operand)
     * the number of bit positions specified in the second operand (count
     * operand) and stores the result in the destination operand. The 
     * destination operand can be a register or a memory location; the count
     * operand is an unsigned integer that can be an immediate or a value in
     * the CL register.
     *
     * The rotate left instruction shift all the bits toward more-significant
     * bit positions, except for the most-significant bit, which is rotate
     * to the least-significant bit location. For the ROL instruction, the
     * original value of the CF flag is not a part of the result, but the
     * CF flag receives a copy the bit that was shifted from one end to the
     * other.
     */


    /* ROL r/m8, CL */
    CX = 0x81;
    DX = 0x01;
    __asm__ ("mov %1, %%al\n\r"
             "mov %2, %%cl\n\r"
             "stc\n\r"
             "rol %%cl, %%al\n\r"
             "mov %%ax, %0"
             : "=m" (AX) : "m" (CX), "m" (DX));
    printk("ROL  8: %#x << %#x = %#x\n", CX, DX, AX & 0xFF);

    /* ROL r/m16, CL */
    CX = 0x8001;
    DX = 0x01;
    __asm__ ("mov %1, %%ax\n\r"
             "mov %2, %%cx\n\r"
             "stc\n\r"
             "rol %%cl, %%ax\n\r"
             "mov %%ax, %0"
             : "=m" (AX) : "m" (CX), "m" (DX));
    printk("ROL 16: %#x << %#x = %#x\n", CX, DX, AX);

    /* ROL r/m32, CL */
    ECX = 0x80000001;
    EDX = 0x01;
    __asm__ ("movl %1, %%eax\n\r"
             "movl %2, %%ecx\n\r"
             "stc\n\r"
             "rol %%cl, %%eax\n\r"
             "movl %%eax, %0"
             : "=m" (EAX) : "m" (ECX), "m" (EDX));
    printk("ROL 32: %#x << %#x = %#x\n", ECX, EDX, EAX);
#endif

    return 0;
}

/*
 * PF (bit 2) -- Parity flag
 *
 *   Parity flag -- Set if the least-significant byte of the result
 *   contains an even number of 1 bits.
 */
static __unused int eflags_PF(void)
{
    unsigned char  __unused AL;
    unsigned short __unused AX;
    unsigned short __unused BX;
    unsigned short __unused CX;
    unsigned short __unused DX;
    unsigned short __unused PF;
    unsigned int   __unused EAX;
    unsigned int   __unused EBX;
    unsigned int   __unused ECX;
    unsigned int   __unused EDX;

#ifdef CONFIG_DEBUG_PF_AAD
    /*
     * AAD -- ASCII Adjust AX Before Division
     *
     * Adjusts two unpacked BCD digits (the least-significant digit in the AL
     * register and the most-significant digit in the AH register) so that a
     * division operation performed on the result will yield a correct
     * unoacked BCD value. The AAD instruction is only useful when it precedes
     * a DIV instruction that divides (binary division) the adjusted value
     * in the AX register by an unpacked BCD value.
     *
     * The AAD instruction sets the value in the AL register to (AL + (10 * AH))
     * and then clears the AH register to 00H. The value in the AX register
     * is then equal to the binary equivalent of the original unpacked two-
     * digit (base 10) number in register AH and AL.
     *
     * The generalized version of this instruction allows adjustment of two
     * unpacked digits of any number base, by setting the imm8 byte to the 
     * selected number base (for example, 08H for octal, 0AH for decimal, or
     * 0CH for base 12 numbers). The AAD mnemonic is interpreted by all 
     * assemblyers to means adjust ASCII (base 10) values. To adjust values in
     * another number base, the instruction must be hand coded in machine
     * code (D5 imm8)
     *
     * IF AAD instruction
     *     THEN
     *         tempAL <---- AL;
     *         tmepAH <---- AH;
     *         AL <---- (tmpAL + (tmpAH * imm8)) AND 0xFFH;
     *         (* imm8 is set to 0AH for the AAD mnemonic *)
     *         AH <---- 0
     * FI
     */
    CX = 0x0503;
    DX = 0xa;
    __asm__ ("mov %3, %%ax\n\r"
             "mov %4, %%bl\n\r"
             "aad\n\r"
             "jp PF_SET0\n\r"
             "jnp PF_CLEAR0\n\r"
     "PF_SET0:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out0\n\r"
   "PF_CLEAR0:\n\r"
             "mov $0, %%dx\n\r"
        "out0:\n\r"
             "mov %%ax, %0\n\r" 
             "div %%bl\n\r"
             "mov %%ax, %1\n\r"
             "mov %%dx, %2"
             : "=m" (AX), "=m" (BX), "=m" (PF)
             : "m" (CX), "m" (DX));
    printk("AX: %#x AAD format %#x\n", CX, AX);
    if (PF)
        printk("AAD: PF set: %#x / %#x = %#x\n", CX, DX, BX);
    else
        printk("AAD: PF clear: %#x / %#x = %#x\n", CX, DX, BX);
#endif

#ifdef CONFIG_DEBUG_PF_AAM
    /*
     * AAM -- ASCII adjust AX after multiply
     *
     * Adjust the result of the multiplication of two unpacked BCD values to
     * create a pair of unpacked (base 10) BCD values. The AX register is 
     * the implied source and destination operand for this instruction. The
     * The AAM instruction is only useful when it follows an MUL instruction
     * that multiplies (binary multiplication) two unpacked BCD values and 
     * stores a word result in the AX register. The AAM instruction then 
     * adjusts contents of the AX register to contain the correct 2-digit 
     * unpacked (base 10) BCD result.
     *
     * The general version of this instruction allows adjustment of the 
     * contents of the AX to create two unpacked digits of any number base.
     * Here, the imm8 byte is set to the selected number base (for example,
     * 0x08H for octal, 0AH for decimal, or 0CH for base 12 numbers). The 
     * AAM mnemonic is interpreted by all assemblers to mean adjust to ASCII
     * (base 10) values. To adjust to values in another number base, the 
     * instruction must be hand coded inb machine code (D4 imm8)
     *
     * IF instruction AAM
     *     THEN
     *         temp <---- AL;
     *         AH <---- tempAL / imm8; 
     *         (* imm8 is set to 0AH for the AAM mnemonic *)
     *         AL <---- temp MODE imm8
     * FI
     */
    CX = 0x9;
    DX = 0x7;
    __asm__ ("mov %3, %%al\n\r"
             "mov %4, %%bl\n\r"
             "mul %%bl\n\r"
             "mov %%ax, %0\n\r"
             "aam\n\r"
             "jp PF_SET1\n\r"
             "jnp PF_CLEAR1\n\r"
     "PF_SET1:\n\r"
             "mov $1, %%bx\n\r"
             "jmp out1\n\r"
   "PF_CLEAR1:\n\r"
             "mov $0, %%bx\n\r"
        "out1:\n\r"
             "mov %%bx, %1\n\r"
             "mov %%ax, %2"
             : "=m" (BX), "=m" (PF), "=m" (AX)
             : "m" (CX), "m" (DX));
    printk("AX: %#x AAM format %#x\n", BX, AX);
    if (PF) 
        printk("AAM - PF set: %#x * %#x = %#x\n", CX, DX, AX);
    else
        printk("AAM - PF clear: %#x * %#x = %#x\n", CX, DX, AX);

#endif

#ifdef CONFIG_DEBUG_PF_ADC
    /*
     * ADC -- Add with Carry
     *
     * ADC AL,  imm8
     * ADC AX,  imm16
     * ADC EAX, imm32
     *
     * Adds the destination operand (first operand), the source operand (
     * second operand), and the carry (CF) flag and stores the result in the
     * destination operand. The destination operand can be a register or a
     * memory location; the source operand can be an immediate, a register,
     * or a memory location. (However, two memory operand cannot be used in
     * one instruction.) The state of the CF flag represents a carry from
     * a previous addition. When an immediate value is used as an operand,
     * it is sign-extended to the length of the destination operand format.
     *
     * The ADC instruction does not distinguish between signed or unsigned
     * operands. Instead, the processor evaluates the result for both data
     * types and sets the OF and CF flags to indicate a carry in the signed
     * unsigned result, respectively. The SF flags indicates the sign of the
     * signed result.
     *
     * DEST <---- DEST + SRC + CF
     *
     */

    /* ADC AL, imm8 */
    CX = 0x1;
    DX = 0x1;
    __asm__ ("stc\n\r"
             "mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "adc %%bl, %%al\n\r"
             "jp PF_S2\n\r"
             "jnp PF_C2\n\r"
       "PF_S2:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out2\n\r"
       "PF_C2:\n\r"
             "mov $0, %%dx\n\r"
        "out2:\n\r"
             "mov %%ax, %0\n\r"
             "mov %%dx, %1"
             : "=m" (AX), "=m" (PF)
             : "m" (CX), "m" (DX));
    if (PF)
        printk("ADC  8: even number on LSB-AL: %#x + %#x = %#x(AX: %#x)\n", 
                      CX, DX, AX & 0xFF, AX);
    else
        printk("ADC  8: odd number on LSB-AL: %#x + %#x = %#x(AX: %#x)\n", 
                      CX, DX, AX & 0xFF, AX);

    /* ADC AX, imm16 */
    CX = 0x101;
    DX = 0x100;
    __asm__ ("stc\n\r"
             "mov %2, %%ax\n\r"
             "mov %3, %%bx\n\r"
             "adc %%bx, %%ax\n\r"
             "jp PF_S3\n\r"
             "jnp PF_C3\n\r"
       "PF_S3:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out3\n\r"
       "PF_C3:\n\r"
             "mov $0, %%dx\n\r"
        "out3:\n\r"
             "mov %%ax, %0\n\r"
             "mov %%dx, %1"
             : "=m" (AX), "=m" (PF)
             : "m" (CX), "m" (DX));
    if (PF)
        printk("ADC 16: even number LSB-AL: %#x + %#x = %#x(AL: %#x)\n",
                               CX, DX, AX, AX & 0xFF);
    else
        printk("ADC 16: odd number LSB-AL: %#x + %#x = %#x(AL: %#x)\n",
                               CX, DX, AX, AX & 0xFF);

    /* ADC EAX, imm32 */
    ECX = 0x10000000;
    EDX = 0x00001002;
    __asm__ ("stc\n\r"
             "movl %2, %%eax\n\r"
             "movl %3, %%ebx\n\r"
             "adcl %%ebx, %%eax\n\r"
             "jp PF_S4\n\r"
             "jnp PF_C4\n\r"
       "PF_S4:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out4\n\r"
       "PF_C4:\n\r"
             "mov $0, %%dx\n\r"
        "out4:\n\r"
             "movl %%eax, %0\n\r"
             "mov %%dx, %1"
             : "=m" (EAX), "=m" (PF)
             : "m" (ECX), "m" (EDX));
    if (PF)
        printk("ADC 32: even nunmber LSB-AL: %#x + %#x = %#x (AL: %#x)\n",
                      ECX, EDX, EAX, EAX & 0xFF);
    else
        printk("ADC 32: odd number LSB-AL: %#x + %#x = %#x (AL: %#x)\n",
                      ECX, EDX, EAX, EAX & 0xFF);
    
#endif

#ifdef CONFIG_DEBUG_PF_ADD
    /*
     * ADD -- Addition with two operands.
     *
     * Adds the destination operand (first operand) and the source operand (
     * second operand) and then stores the result in the destination operand.
     * The destination operand can be a register or a memory location; the 
     * source operand can be an immediate, a register, or a memory location.
     * (However, two memory operands cannot be used in one instruction.) When
     * an immediate value is used as an operand, it it sign-extended to the
     * length of the destination operand format.
     * 
     * The ADD instruction performs integer addition. It evaluates the result
     * for both signed and unsigned integer operands and sets the CF and
     * OF flags to indicate a carry (overflow) in the signed or unsigned 
     * result, respectively. The SF flag indicates the sign of the signed 
     * result.
     *
     * DEST <---- DEST + SRC
     *
     * The OF, SF, ZF, AF, CF, and PF flags are set according to the result.
     */
    CX = 0x01;
    DX = 0x02;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "add %%bl, %%al\n\r"
             "jp PF_S5\n\r"
             "jnp PF_C5\n\r"
       "PF_S5:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out5\n\r"
       "PF_C5:\n\r"
             "mov $0, %%dx\n\r"
        "out5:\n\r"
             "mov %%ax, %0\n\r"
             "mov %%dx, %1"
             : "=m" (AX), "=m" (PF)
             : "m" (CX), "m" (DX));
    if (PF)
        printk("ADD: even number LSB-AL: %#x + %#x = %#x (AL: %#x)\n",
                   CX, DX, AX, AX & 0xFF);
    else
        printk("ADD: odd number LSB-AL: %#x + %#x = %#x (AL: %#x)\n",
                   CX, DX, AX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_PF_AND
    /*
     * AND -- Logical AND
     *
     * Performs a bitwise AND operation on the destination (first) and source
     * (second) operands and stores the result in the destination operand
     * location. The source operand can be an immediate, a register, or a 
     * memory location; the destination operand can be a register or a memory
     * location. (However, two memory operands cannot be used in one 
     * instruction.) Each bit of the result is set to 1 if both corresponding
     * bits of the first and second operands are 1; otherwise, it is set to 0.
     *
     * DEST <---- DEST AND SRC
     */
    CX = 0x3;
    DX = 0x1;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "and %%bl, %%al\n\r"
             "jp PF_S6\n\r"
             "jnp PF_C6\n\r"
       "PF_S6:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out6\n\r"
       "PF_C6:\n\r"
             "mov $0, %%dx\n\r"
        "out6:\n\r"
             "mov %%ax, %0\n\r"
             "mov %%dx, %1"
             : "=m" (AX), "=m" (PF)
             : "m" (CX), "m" (DX));
    if (PF)
        printk("AND: even number LSB-AL: %#x AND %#x = %#x(AL: %#x)\n",
                       CX, DX, AX & 0xFF, AX & 0xFF);
    else
        printk("AND: odd number LSB-AL: %#x AND %#x = %#x(AL: %#x)\n",
                       CX, DX, AX & 0xFF, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_PF_CMP
    /*
     * CMP -- Compare two operands
     *
     * Compares the first source operand with the second source operand and
     * the status flags in the EFLAGS register according to the result. The
     * comparison is performed by subtracting the second operand from the
     * first operand and then setting the status flags in the same manner
     * as the SUB instruction. When an immediate value is used as an operand,
     * it is sign-extended to the length of the first operand.
     *
     * temp <---- SRC1 - SignExtend(SRC2);
     * ModifyStatusFlags; 
     * (* Modify status flags in the same manner as the SUB instruction *)
     */
    CX = 0x04;
    DX = 0x01;
    __asm__ ("mov %1, %%al\n\r"
             "mov %2, %%bl\n\r"
             "cmp %%bl, %%al\n\r"
             "jp PF_S7\n\r"
             "jnp PF_C7\n\r"
       "PF_S7:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out7\n\r"
       "PF_C7:\n\r"
             "mov $0, %%dx\n\r"
        "out7:\n\r"
             "mov %%dx, %0"
             : "=m" (PF)
             : "m" (CX), "m" (DX));
    if (PF)
        printk("CMP: even number LSB-AL: %#x CMP %#x\n", CX, DX);
    else
        printk("CMP: odd number LSB-AL: %#x CMP %#x\n", CX, DX);
#endif

#ifdef CONFIG_DEBUG_PF_CMPSB
    /*
     * CMPSB -- Compare string operands in byte
     *
     * Compares the byte with the first source operand with the byte specified
     * with the second source operand and sets the status flags in EFLAGS 
     * register according to the result.
     *
     * Both source operands are located in memory. The address of the first
     * source operand is read from DS:SI (depending on the address-size 
     * attribute of the instruction is 16). The address of the second source
     * operand is read from ES:DI (again depending on the address-size 
     * attribute of the instruction is 16). The DS segment may be overriden
     * with a segment override prefix, but the ES segment cannot be overridden.
     *
     * At the assembly-code level, two forms of this instruction are allowed:
     * the "explicit-operands" from and the "no-operands" from. The explicit-
     * operands form (specified with the CMPS mnemonic) allows the two source
     * operands to the specified explicitly. Here, the source operands should
     * be symbols that indicate the size and location of the source values.
     * This explicit-operand form is provided to allow documentation. However,
     * not that the documentation provided by this form can be misleading.
     * That is, the source operand symbols must specify the correct type
     * (size) of the operands (bytes), but they do not have to specify the
     * correct location. Locations of the source operands are always specified
     * by the DS:SI and ES:DI register, which must be loaded correctly before
     * the compare string instruction is executed.
     *
     * The size of the source operands is selected with the mnemonic: CMPSB
     * (byte comparison). After the comparison, the SI and DI registers
     * increment or decrement automatically according to the setting of the
     * DF flag in the EFLAGS register. (If the DF flags is 0, the SI and DI
     * register increment; if the DF flag is 1, the register decrement.) The
     * register increment or decrement by 1 for byte operations.
     *
     * The CMPSB instruction can be preceded by the REP prefix for block
     * comparisons. More often, however, these instructions will be used in 
     * a LOOP construct that takes some action based on the setting of the 
     * status flags before the next comparison is made.
     */
    ECX = (unsigned int)(unsigned long)&"Hello EWorld"[0];
    EDX = (unsigned int)(unsigned long)&"Hello BiscuitOS"[0];
    CX  = 10;
    __asm__ ("mov %2, %%esi\n\r"
             "mov %3, %%edi\n\r"
             "mov %4, %%cx\n\r"
             "cld\n\r"
             "repz cmpsb\n\r"
             "jp PF_S8\n\r"
             "jnp PF_C8\n\r"
       "PF_S8:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out8\n\r"
       "PF_C8:\n\r"
             "mov $0, %%dx\n\r"
        "out8:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%cx, %1"
             : "=m" (PF), "=m" (AX)
             : "m" (ECX), "m" (EDX), "m" (CX));
    if (PF) {
        printk("CMPSB: even number -> \"%s\" DIFF \"%s\" in %d's Byte\n",
            (char *)(unsigned long)ECX,
            (char *)(unsigned long)EDX,
            (CX - AX - 1));
        ((char *)(unsigned long)ECX)[(CX - AX) * 1] = '\0';
        ((char *)(unsigned long)EDX)[(CX - AX) * 1] = '\0';
        printk("CMPSB: %d's Byte: \"%s\" -- \"%s\" (first DIFF)\n", 
            CX - AX - 1,
            &((char *)(unsigned long)ECX)[(CX - AX - 1) * 1],
            &((char *)(unsigned long)EDX)[(CX - AX - 1) * 1]);
    } else {
        printk("CMPSB: odd number -> \"%s\" DIFF \"%s\" in %d's Byte\n",
            (char *)(unsigned long)ECX,
            (char *)(unsigned long)EDX,
            (CX - AX - 1));
        ((char *)(unsigned long)ECX)[(CX - AX) * 1] = '\0';
        ((char *)(unsigned long)EDX)[(CX - AX) * 1] = '\0';
        printk("CMPSB: %d's Byte: \"%s\" -- \"%s\" (first DIFF)\n", 
            CX - AX - 1,
            &((char *)(unsigned long)ECX)[(CX - AX - 1) * 1],
            &((char *)(unsigned long)EDX)[(CX - AX - 1) * 1]);
    }

#endif

#ifdef CONFIG_DEBUG_PF_CMPSW
    /*
     * CMPSW -- Compare string operands in word
     *
     * Compares the byte with the first source operand with the byte specified
     * with the second source operand and sets the status flags in EFLAGS 
     * register according to the result.
     *
     * Both source operands are located in memory. The address of the first
     * source operand is read from DS:SI (depending on the address-size 
     * attribute of the instruction is 16). The address of the second source
     * operand is read from ES:DI (again depending on the address-size 
     * attribute of the instruction is 16). The DS segment may be overriden
     * with a segment override prefix, but the ES segment cannot be overridden.
     *
     * At the assembly-code level, two forms of this instruction are allowed:
     * the "explicit-operands" from and the "no-operands" from. The explicit-
     * operands form (specified with the CMPS mnemonic) allows the two source
     * operands to the specified explicitly. Here, the source operands should
     * be symbols that indicate the size and location of the source values.
     * This explicit-operand form is provided to allow documentation. However,
     * not that the documentation provided by this form can be misleading.
     * That is, the source operand symbols must specify the correct type
     * (size) of the operands (bytes), but they do not have to specify the
     * correct location. Locations of the source operands are always specified
     * by the DS:SI and ES:DI register, which must be loaded correctly before
     * the compare string instruction is executed.
     *
     * The size of the source operands is selected with the mnemonic: CMPSW
     * (word comparison). After the comparison, the SI and DI registers
     * increment or decrement automatically according to the setting of the
     * DF flag in the EFLAGS register. (If the DF flags is 0, the SI and DI
     * register increment; if the DF flag is 1, the register decrement.) The
     * register increment or decrement by 1 for byte operations.
     *
     * The CMPSW instruction can be preceded by the REP prefix for block
     * comparisons. More often, however, these instructions will be used in 
     * a LOOP construct that takes some action based on the setting of the 
     * status flags before the next comparison is made.
     */

    ECX = (unsigned int)(unsigned long)&"Hello EWorld"[0];
    EDX = (unsigned int)(unsigned long)&"Hello BiscuitOS"[0];
    CX  = 10;
    __asm__ ("mov %2, %%esi\n\r"
             "mov %3, %%edi\n\r"
             "mov %4, %%cx\n\r"
             "cld\n\r"
             "repz cmpsw\n\r"
             "jp PF_S9\n\r"
             "jnp PF_C9\n\r"
       "PF_S9:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out9\n\r"
       "PF_C9:\n\r"
             "mov $0, %%dx\n\r"
        "out9:\n\r"
             "mov %%cx, %0\n\r"
             "mov %%dx, %1"
             : "=m" (AX), "=m" (PF)
             : "m" (ECX), "m" (EDX), "m" (CX));
    if (PF) {
        printk("CMPSW: even number -> \"%s\" DIFF \"%s\" in %d's Word\n",
            (char *)(unsigned long)ECX,
            (char *)(unsigned long)EDX,
            (CX - AX - 1));
        ((char *)(unsigned long)ECX)[(CX - AX) * 2] = '\0';
        ((char *)(unsigned long)EDX)[(CX - AX) * 2] = '\0';
        printk("CMPSW: %d's Word: \"%s\" -- \"%s\" (first DIFF)\n", 
            CX - AX - 1,
            &((char *)(unsigned long)ECX)[(CX - AX - 1) * 2],
            &((char *)(unsigned long)EDX)[(CX - AX - 1) * 2]);
    } else {
        printk("CMPSW: odd number -> \"%s\" DIFF \"%s\" in %d's Word\n",
            (char *)(unsigned long)ECX,
            (char *)(unsigned long)EDX,
            (CX - AX - 1));
        ((char *)(unsigned long)ECX)[(CX - AX) * 2] = '\0';
        ((char *)(unsigned long)EDX)[(CX - AX) * 2] = '\0';
        printk("CMPSW: %d's Word: \"%s\" -- \"%s\" (fisrt DIFF)\n", 
            CX - AX - 1,
            &((char *)(unsigned long)ECX)[(CX - AX - 1) * 2],
            &((char *)(unsigned long)EDX)[(CX - AX - 1) * 2]);
    }
 
#endif

#ifdef CONFIG_DEBUG_PF_CMPSD
    /*
     * CMPSD -- Compare string operands in Double-word
     *
     * Compares the byte with the irst source operand with the byte specified
     * with the second source operand and sets the status flags in EFLAGS 
     * register according to the result.
     *
     * Both source operands are located in memory. The address of the first
     * source operand is read from EDS:ESI (depending on the address-size 
     * attribute of the instruction is 32). The address of the second source
     * operand is read from ES:EDI (again depending on the address-size 
     * attribute of the instruction is 32). The DS segment may be overriden
     * with a segment override prefix, but the ES segment cannot be overridden.
     *
     * At the assembly-code level, two forms of this instruction are allowed:
     * the "explicit-operands" from and the "no-operands" from. The explicit-
     * operands form (specified with the CMPS mnemonic) allows the two source
     * operands to the specified explicitly. Here, the source operands should
     * be symbols that indicate the size and location of the source values.
     * This explicit-operand form is provided to allow documentation. However,
     * not that the documentation provided by this form can be misleading.
     * That is, the source operand symbols must specify the correct type
     * (size) of the operands (bytes), but they do not have to specify the
     * correct location. Locations of the source operands are always specified
     * by the DS:ESI and ES:EDI register, which must be loaded correctly before
     * the compare string instruction is executed.
     *
     * The size of the source operands is selected with the mnemonic: CMPSD
     * (double-word comparison). After the comparison, the ESI and EDI registers
     * increment or decrement automatically according to the setting of the
     * DF flag in the EFLAGS register. (If the DF flags is 0, the SI and DI
     * register increment; if the DF flag is 1, the register decrement.) The
     * register increment or decrement by 1 for byte operations.
     *
     * The CMPSD instruction can be preceded by the REP prefix for block
     * comparisons. More often, however, these instructions will be used in 
     * a LOOP construct that takes some action based on the setting of the 
     * status flags before the next comparison is made.
     */

    ECX = (unsigned int)(unsigned long)&"HellEWorld"[0];
    EDX = (unsigned int)(unsigned long)&"HellBiscuitOS"[0];
    CX  = 10;
    __asm__ ("movl %2, %%esi\n\r"
             "movl %3, %%edi\n\r"
             "mov  %4, %%cx\n\r"
             "cld\n\r"
             "repz cmpsd\n\r"
             "jp PF_SA\n\r"
             "jnp PF_CA\n\r"
       "PF_SA:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outA\n\r"
       "PF_CA:\n\r"
             "mov $0, %%dx\n\r"
        "outA:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%cx, %1"
             : "=m" (PF), "=m" (AX)
             : "m" (ECX), "m" (EDX), "m" (CX));
    if (PF) {
        printk("CMPSD: even number -> \"%s\" DIFF \"%s\" in %d Double-word\n",
            (char *)(unsigned long)ECX,
            (char *)(unsigned long)EDX,
            (CX - AX - 1));
        ((char *)(unsigned long)ECX)[(CX - AX) * 4] = '\0';
        ((char *)(unsigned long)EDX)[(CX - AX) * 4] = '\0';
        printk("CMPSD: %d's Double-word: \"%s\" -- \"%s\" (first DIFF)\n", 
            CX - AX - 1,
            &((char *)(unsigned long)ECX)[(CX - AX - 1) * 4],
            &((char *)(unsigned long)EDX)[(CX - AX - 1) * 4]);
    } else {
        printk("CMPSD: odd number -> \"%s\" DIFF \"%s\" in %d Double-word\n",
            (char *)(unsigned long)ECX,
            (char *)(unsigned long)EDX,
            (CX - AX - 1));
	((char *)(unsigned long)ECX)[(CX - AX) * 4] = '\0';
        ((char *)(unsigned long)EDX)[(CX - AX) * 4] = '\0';
        printk("CMPSD: %d's Double-word: \"%s\" -- \"%s\" (first DIFF)\n", 
            CX - AX - 1,
            &((char *)(unsigned long)ECX)[(CX - AX - 1) * 4],
            &((char *)(unsigned long)EDX)[(CX - AX - 1) * 4]);
    }

#endif

#ifdef CONFIG_DEBUG_PF_DAA
    /*
     * DAA -- Decimal Adjust AL after Addition
     *
     * Adjust the sum of two packed BCD values to create a packed BCD result.
     * The AL register is the implied source and destination operand. The 
     * DAA instruction is only useful when it follow an ADD instruction that
     * adds (binary addition) two 2-digit, packed BCD values and stores a
     * byte result in the AL register. The DAA instruction then adjusts the
     * contents of the AL register to contain the correct 2-digit, packed
     * BCD result. If a decimal carry is detected, the CF and AF flags are
     * set accordingly.
     *
     * IF instruction DAA
     *     old_AL <---- AL
     *     old_CF <---- CF
     *     CF     <---- 0
     *     IF (((AL AND 0FH) > 9) or AF = 1)
     *         THEN
     *             AL <---- AL + 6 
     *             CF <---- old_CF or (Carry from AL <---- AL + 6)
     *             AF <---- 1
     *         ELSE
     *             AF <---- 0
     *     FI
     *     IF ((old_AL > 99H) or (old_CF = 1))
     *         THEN
     *             AL <---- AL + 60H
     *             CF <---- 1
     *         ELSE
     *             CF <---- 0
     *     FI
     * FI
     *
     * The CF and AF flags are set if the adjustment of the value results
     * in a decimal carry in either digit of the result. The SF, ZF, and PF
     * flags are set according to the result. The OF flag is undefined.
     */
    CX = 0x79;
    DX = 0x35;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "add %%bl, %%al\n\r"
             "daa\n\r"
             "jp PF_SB\n\r"
             "jnp PF_CB\n\r"
       "PF_SB:\n\r"
             "mov $1, %%bx\n\r"
             "jmp outB\n\r"
       "PF_CB:\n\r"
             "mov $0, %%bx\n\r"
         "outB:\n\r"
             "mov %%ax, %0\n\r"
             "mov %%bx, %1"
             : "=m" (AX), "=m" (PF) : "m" (CX), "m" (DX));
    if (PF)
        printk("DAA: even number -> %#x + %#x = %#x\n", CX, DX, AX);
    else
        printk("DAA: odd number -> %#x + %#x = %#x\n", CX, DX, AX);
#endif

#ifdef CONFIG_DEBUG_PF_DAS
    /*
     * DAS -- Decimal Adjust AL after Subtraction
     *
     * Adjusts the result of the subtraction of two packed BCD values to 
     * create a packed BCD result. The AL register is the implied source
     * and destination operand. The DAS instruction is only useful when
     * it follows a SUB instruction that subtracts (binary subtraction)
     * one 2-digit, packed BCD value from another and stores a byte result
     * in the AL register. The DAS instruction then adjusts the contents of
     * the AL register to contain the correct 2-digit, packed BCD result.
     * IF a decimal borrow is detected, the CF and AF flags are set 
     * accordingly.
     *
     * IF instruction DAS
     *     old_AL <---- AL
     *     old_CF <---- CF
     *     CF     <---- 0
     *     IF (((AL AND 0FH) > 9) or AF = 1)
     *         THEN
     *             AL <---- AL - 6
     *             CF <---- old_CF or (Borrow from AL <---- AL - 6)
     *             AF <---- 1
     *         ELSE
     *             AF <---- 0
     *     FI
     *     IF ((old_AL > 99H) or (old_CF = 1))
     *         THEN
     *             AL <---- AL - 60H
     *             CF <---- 1
     *     FI
     * FI
     */
    CX = 0x35;
    DX = 0x47;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "sub %%bl, %%al\n\r"
             "das\n\r"
             "jp CF_SC\n\r"
             "jnp CF_CC\n\r"
       "CF_SC:\n\r"
             "mov $1, %%bx\n\r"
             "jmp outC\n\r"
       "CF_CC:\n\r"
             "mov $0, %%bx\n\r"
         "outC:\n\r"
             "mov %%bx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (PF), "=m" (AX) : "m" (CX), "m" (DX));
    if (PF)
        printk("DAS: even number -> %#x - %#x = %#x\n", CX, DX, AX);
    else
        printk("DAS: odd number -> %#x - %#x = %#x\n", CX, DX, AX);
#endif

#ifdef CONFIG_DEBUG_PF_DEC
    /*
     * DEC -- Decreament by 1
     *
     * Subtracts 1 from the destination operand, while preserving the state
     * of the CF flag. The destination operand can be a register or a memory
     * location. This instrution allows a loop counter to be updated without
     * disturbing the CF flag. (To perform a decrement operation that updates
     * the CF flag, use a SUB instruction with an immediate operand of 1.)
     *
     * DEST <---- DEST - 1
     */
    CX = 0x4;
    __asm__ ("mov %2, %%al\n\r"
             "dec %%al\n\r"
             "jp PF_SD\n\r"
             "jnp PF_CD\n\r"
       "PF_SD:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outD\n\r"
       "PF_CD:\n\r"
             "mov $0, %%dx\n\r"
        "outD:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (PF), "=m" (AX)
             : "m" (CX));
    if (PF)
        printk("DEC: even number -> %#x - 1 = %#x\n", CX, AX & 0xFF);
    else
        printk("DEC: odd number -> %#x - 1 = %#x\n", CX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_PF_INC
    /*
     * INC -- Increment by 1
     *
     * Adds 1 to the destination operand, while preserving the state of CF
     * flag. The destination operand can be a register or a memory location.
     * This instruction allows a loop counter to the updated without 
     * disturbing the CF flag. (Use a ADD instruction with an immediate 
     * operand of 1 to perform an increment operation that does updates the
     * CF flag.)
     *
     * DEST <---- DEST + 1;
     */
    CX = 0x2;
    __asm__ ("mov %2, %%al\n\r"
             "inc %%al\n\r"
             "jp PF_SE\n\r"
             "jnp PF_CE\n\r"
       "PF_SE:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outE\n\r"
       "PF_CE:\n\r"
             "mov $0, %%dx\n\r"
        "outE:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (PF), "=m" (AX)
             : "m" (CX));
    if (PF)
        printk("INC: even number -> %#x + 1 = %#x\n", CX, AX & 0xFF);
    else
        printk("INC: odd number -> %#x + 1 = %#x\n", CX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_PF_NEG
    /*
     * NEG -- two's complement negation
     *
     * Replaces the value of operand (the destination operand) with its 
     * two's complement. (This operation is equivalent to subtraction the 
     * operand from 0.) The destination operand is located in a general-
     * purpose register or a memory location.
     *
     * IF DEST = 0
     *     THEN CF <---- 0;
     *     ELSE CF <---- 0;
     * FI
     * DEST <---- [~(DEST)]
     */
    CX = 4;
    __asm__ ("mov %2, %%al\n\r"
             "neg %%al\n\r"
             "jp PF_SF\n\r"
             "jnp PF_CF\n\r"
       "PF_SF:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outF\n\r"
       "PF_CF:\n\r"
             "mov $0, %%dx\n\r"
        "outF:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (PF), "=m" (AX)
             : "m" (CX));
    if (PF)
        printk("NEG: even number -> ~(%#x) + 1 = %#x\n", CX, AX & 0xFF);
    else
        printk("NEG: odd number -> ~(%#x) + 1 = %#x\n", CX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_PF_OR
    /*
     * OR -- Logical inclusive OR
     *
     * Performs a bitwise inclusive OR operation between the destination 
     * (first) and source (second) operands and stores the result int the
     * destination operand location. The source operand can be an immediate,
     * a register, or a memory location; the destination operand can be a
     * register or a memory location. (However, two memory operands cannot
     * be used in one instruction.) Each bit of the result of the OR 
     * instruction is set to 0 if both corresponding bits of the first and
     * second operands are 0; otherwise, each bit is set to 1.
     *
     * DEST <---- DEST OR SRC;
     */
    CX = 0x3;
    DX = 0x1;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "or %%bl, %%al\n\r"
             "jp PF_SG\n\r"
             "jnp PF_CG\n\r"
       "PF_SG:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outG\n\r"
       "PF_CG:\n\r"
             "mov $0, %%dx\n\r"
        "outG:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (PF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (PF)
        printk("OR: even number -> %#x OR %#x = %#x\n\r", CX, DX, AX & 0xF);
    else
        printk("OR: odd number -> %#x OR %#x = %#x\n\r", CX, DX, AX & 0xF);
#endif

#ifdef CONFIG_DEBUG_PF_SAL
    /*
     * SAL -- Shift arithmetic left
     * 
     * SAL r/m8, CL
     * SAL r/m16, CL
     * SAL r/m32, CL
     *
     * Shifts the bits in the first operand (destination operand) to the left
     * by the number of bits specified in the second operand (count operand).
     * Bits shifted beyond the destination operand boundary are first shifted
     * into the CF flag, then discarded. At the end of the shift operation, 
     * the CF flag contains the last bit shifted out of the destination 
     * operand.
     *
     * The shift arithmetic left (SAL) and shift logical left (SHL) 
     * instructions perform the same operation; they shift the bits in the 
     * destination operand to the left (toward more significant bit locations).
     * For each shift count, the most significant bit of the destination 
     * operand is shifted into the CF flag, and the least significant bit is
     * cleared.
     */
    CX = 0x8F;
    DX = 2;
    __asm__("mov %2, %%al\n\r"
            "mov %3, %%cl\n\r"
            "sal %%cl, %%al\n\r"
            "jp PF_SH\n\r"
            "jnp PF_CH\n\r"
      "PF_SH:\n\r"
            "mov $1, %%dx\n\r"
            "jmp outH\n\r"
      "PF_CH:\n\r"
            "mov $0, %%dx\n\r"
       "outH:\n\r"
            "mov %%dx, %0\n\r"
            "mov %%ax, %1"
            : "=m" (PF), "=m" (AX)
            : "m" (CX), "m" (DX));
    if (PF)
        printk("SAL: even number -> %#x << %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SAL: odd number -> %#x << %#x = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_PF_SAR
    /*
     * SAR -- Shift arithmetic right
     *
     * SAR r/m8, CL
     * SAR r/m16, CL
     * SAR r/m32, CL
     *
     * Shifts the bits in the first operand (destination operand) to the right
     * by the number of bits specified in the second operand (count operand).
     * Bits shifted beyond the destination operand boundary are first into
     * the CF flag, then discarded. At the end of the shift operation, the 
     * CF flag contains the last bit shifted out of the destination operand.
     *
     * The shift arithmetic right (SAR) instruction shift the bits of the 
     * bits of the destination operand to the right (toward less significant
     * bit locations). For each shift count, the least significant bit of the
     * destination operand is shifted into the CF flag, and the most 
     * significant bit is either set or clear depending on the instruction
     * type. The SAR instruction sets or clears the most significant bit to
     * correspond to the sign (most significant bit) of the original value
     * in the destination operand. In effect, the SAR instruction fills the
     * empty bit position's shifted value with the sign of the unshifted value.
     */
    CX = 0x27;
    DX = 0x2;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%cl\n\r"
             "sar %%cl, %%al\n\r"
             "jp PF_SI\n\r"
             "jnp PF_CI\n\r"
       "PF_SI:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outI\n\r"
       "PF_CI:\n\r"
             "mov $0, %%dx\n\r"
        "outI:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (PF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (PF)
        printk("SAR: even number -> %#x >> %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SAR: odd number -> %#x >> %#x = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_PF_SHR
    /*
     * SHR --- Shift logical right
     *
     * SHR r/m8, CL
     * SHR r/m16, CL
     * SHR r/m32, CL
     *
     * Shift the bit the first operand (destination operand) to the right by
     * the number of bits specified in the second operand (count operand).
     * Bits shifted beyond the destination operand boundary are first shift
     * into the CF flag, hten discared. At the end of the shift operation, 
     * the CF flag contains the last bit shifted out of the destination 
     * operand.
     *
     * The shift logical right (SHR) instrution shift the bits of the 
     * destination operand to the right (toward less significant bit location).
     * For each shift count, the least significant bit of the destination
     * operand is shifted into the CF flag, and the most significant bit is
     * either set or cleared depending on the instruction type. 
     */
    CX = 0x27;
    DX = 0x2;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%cl\n\r"
             "shr %%cl, %%al\n\r"
             "jp PF_SJ\n\r"
             "jnp PF_CJ\n\r"
       "PF_SJ:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outJ\n\r"
       "PF_CJ:\n\r"
             "mov $0, %%dx\n\r"
        "outJ:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (PF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (PF)
        printk("SHR: even number -> %#x >> %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SHR: odd number -> %#x >> %#x = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_PF_SHL
    /*
     * SHL -- Shift logical Left
     *
     * SHL r/m8,  cl
     * SHL r/m16, cl
     * SHL r/m32, cl
     *
     * Shifts the bits in the first operand (destination operand) to the left
     * by the number of bits specified in the second operand (count operand).
     * Bits shifted beyond the destination operand boundary are first shifted
     * into the CF flag, then discared. At the end of the shift operation, the
     * CF flag contains the last bit shifted out of the destination operand.
     * 
     * The shift logical left (SHL) instruction perform the same operation;
     * they shift the bits in the destination operand to the left (toward
     * more significant bit locations). For each shift count, the most 
     * significant bit of the destination operand is shifted into the CF flag,
     * and the least significant bit is cleared.
     */
    CX = 0x8F;
    DX = 0x2;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%cl\n\r"
             "shl %%cl, %%al\n\r"
             "jp PF_SK\n\r"
             "jnp PF_CK\n\r"
       "PF_SK:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outK\n\r"
       "PF_CK:\n\r"
             "mov $0, %%dx\n\r"
        "outK:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (PF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (PF)
        printk("SHL: even number -> %#x << %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SHL: odd number -> %#x << %#x = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_PF_SBB
    /*
     * SBB -- Integer Subtraction with Borrow
     *
     * SBB AL, imm8
     * SBB AX, imm16
     * SBB EAX, imm32
     *
     * Adds the source operand (second operand) and the carry (CF) flag, and
     * subtracts the result from the destination operand (first operand). 
     * The result of the subtraction is stored in the destination operand. 
     * The destination operand can be register or a memory location; the
     * source operand can be an immediate, a register, or a memory location.
     * (However, two memory operands cannot be used in one instruction.) The
     * state of the CF flag represents a borrow from a previous subtraction.
     *
     * When an immediate value is used as an operand, it is sign-extended to 
     * the length of the detination operand format.
     *
     * The SBB instruction does not distinguish between signed or unsigned
     * operands. Instead, the processor evaluates the result for both data
     * types and sets the OF and CF flags to indicate a borrow in the signed
     * or unsigned result, respectively. The SF flag flag indicates the
     * sign of the sign of the signed result.
     *
     * The SBB instruction is usually executed as part of a multibyte or 
     * multiword subtraction in which a SUB instruction is followed by a
     * SBB instruction.
     *
     * DEST <---- (DEST  - (SRC + CF))
     *
     * The OF, SF, ZF, AF, PF, and CF flags are set according to the result.
     */
    CX = 0x10;
    DX = 0x3;
    __asm__ ("stc\n\r"
             "mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "sbb %%bl, %%al\n\r"
             "jp PF_SL\n\r"
             "jnp PF_CL\n\r"
       "PF_SL:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outL\n\r"
       "PF_CL:\n\r"
             "mov $0, %%dx\n\r"
        "outL:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (PF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (PF)
        printk("SBB: even number -> %#x - %#x - CF= %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SBB: odd number -> %#x - %#x -CF = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_PF_SCASB
    /*
     * SCASB -- Scan String in Byte
     *
     * The no-operands form of the instruction uses a short form of SCAS. 
     * Again, ES:(E)DI is assumed to be the memory operand and AL, AX, or EAX
     * is assumed to be the register operand. The size of operands is 
     * selected by the mnemonic: SCASB (byte comparision).
     * 
     * After the comparison, the (EDI) register is increamented or decremented
     * automatically according to the setting of the DF flag in the EFLAGS
     * register. If the DF flag is 0, the (E)DI register is increamented; if
     * the DF flag is 1, the (E)DI register is decremented. The register is
     * increamented or decremented by 1 for byte operations.
     *
     * SCASB can be preceded by the REP prefix for block comparisions of ECX
     * bytes. Often, however, these instructions will be used in a LOOP 
     * construct that takes some action based on the setting of status flags.
     */
    ECX = (unsigned int)(unsigned long)&"Hello World"[0];
    AL  = (unsigned char)(unsigned long)'d';
    CX  = 15;
    __asm__ ("movl %2, %%edi\n\r"
             "mov  %3, %%al\n\r"
             "mov  %4, %%cx\n\r"
             "cld\n\r"
             "repne scasb\n\r"
             "jp PF_SM\n\r"
             "jnp PF_CM\n\r"
       "PF_SM:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outM\n\r"
       "PF_CM:\n\r"
             "mov $0, %%dx\n\r"
        "outM:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%cx, %1"
             : "=m" (PF), "=m" (AX)
             : "m" (ECX), "m" (AL), "m" (CX)); 
    if (PF)
        printk("SCASB: even number ->");
    else
        printk("SCASB: odd number ->");

    printk("\"%s\" Scan \"%c\" in %d\n", 
           (char *)(unsigned long)ECX, AL, CX - AX - 1);
#endif

#ifdef CONFIG_DEBUG_PF_SCASW
    /*
     * SCASW -- Scan String in Word
     *
     * The no-operands form of the instruction uses a short form of SCAS. 
     * Again, ES:(E)DI is assumed to be the memory operand and AL, AX, or EAX
     * is assumed to be the register operand. The size of operands is 
     * selected by the mnemonic: SCASW (word comparision).
     * 
     * After the comparison, the (EDI) register is increamented or decremented
     * automatically according to the setting of the DF flag in the EFLAGS
     * register. If the DF flag is 0, the (E)DI register is increamented; if
     * the DF flag is 1, the (E)DI register is decremented. The register is
     * increamented or decremented by 1 for byte operations.
     *
     * SCASW can be preceded by the REP prefix for block comparisions of ECX
     * word. Often, however, these instructions will be used in a LOOP 
     * construct that takes some action based on the setting of status flags.
     */
    ECX = (unsigned int)(unsigned long)&"Hello World"[0];
    AX  = (('o' << 8) | 'W');
    CX  = 15;
    __asm__ ("movl %2, %%edi\n\r"
             "mov %3, %%ax\n\r"
             "mov %4, %%cx\n\r"
             "cld\n\r"
             "repne scasw\n\r"
             "jp PF_SN\n\r"
             "jnp PF_CN\n\r"
       "PF_SN:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outN\n\r"
       "PF_CN:\n\r"
             "mov $0, %%dx\n\r"
        "outN:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%cx, %1"
             : "=m" (PF), "=m" (DX)
             : "m" (ECX), "m" (AX), "m" (CX));
    if (PF)
        printk("SCASW: even number ->");
    else
        printk("SCASW: odd number ->");

    
    printk("\"%s\" Scan \"%c%c\" in word %d\n", 
            (char *)(unsigned long)ECX,
            AX & 0xFF, AX >> 8,
            CX - DX - 1);
#endif

#ifdef CONFIG_DEBUG_PF_SUB
    /*
     * SUB -- Subtract
     *
     * SUB AL, imm8
     * SUB AX, imm16
     * SUB EAX, imm32
     *
     * Subracts the second operand (source operand) from the first operand (
     * destination operand) and stores the result in the destination operand.
     * The destination operand can be a register or a memory location; the 
     * source operand can be an immediate, register, or memory location. (
     * However, two memory operands cannot be used in one instruction.) When
     * an immediate value is used as an operand, it is sign-extended to the
     * length of the destination operand format.
     *
     * The SUB instruction performs integer subtraction. It evaluates the 
     * result for both signed and unsigned integer operands and sets the
     * OF and CF flags to indicate an overflow in the signed or unsigned
     * result, respectively. The SF flag indicates the sign of the signed
     * result.
     *
     * DEST <---- (DEST - SRC)
     *
     * The OF, SF, ZF, AF, PF, and CF flags are set according to the result.
     */
    CX = 0x9;
    DX = 0x3;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "sub %%bl, %%al\n\r"
             "jp PF_SO\n\r"
             "jnp PF_CO\n\r"
       "PF_SO:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outO\n\r"
       "PF_CO:\n\r"
             "mov $0, %%dx\n\r"
        "outO:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (PF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (PF)
        printk("SUB: even number -> %#x - %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SUB: odd number -> %#x - %#x = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_PF_TEST
    /*
     * TEST -- Logical Compare
     *
     * Computes the bit-wise logical AND of first operand (source 1 operand)
     * and second operand (source 2 operand) and sets the SF, ZF, and PF
     * status flags according to the result. The result is then discarded.
     *
     */
    CX = 0x7;
    DX = 0x3;
    __asm__ ("mov %1, %%al\n\r"
             "mov %2, %%bl\n\r"
             "test %%bl, %%al\n\r"
             "jp PF_SP\n\r"
             "jnp PF_CP\n\r"
       "PF_SP:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outP\n\r"
       "PF_CP:\n\r"
             "mov $0, %%dx\n\r"
        "outP:\n\r"
             "mov %%dx, %0"
             : "=m" (PF)
             : "m" (CX), "m" (DX));
    if (PF)
        printk("TEST: even number -> %#x TEST %#x = %#x\n", CX, DX, CX & DX);
    else
        printk("TEST: odd number -> %#x TEST %#x = %#x\n", CX, DX, CX & DX);
#endif

#if defined CONFIG_DEBUG_PF_JP || defined CONFIG_DEBUG_PF_JNP
    /*
     * JP  -- Jump short if parity (PF = 1)
     * JNP -- Jump short if not parity (PF = 0)
     *
     * Checks the state of one or more of the status flags in the EFLAGS
     * register (PF) and, if the flags are in the specified state (condition),
     * performs a jump tp the target instruction specified by the destination
     * operand. A condition code (cc) is associated with each instruction
     * to indicate the condition being tested for. If the condition is not 
     * statisfied, the jump is not performed and execution continues with
     * instruction the Jcc instruction.
     *
     * The target instruction is specified with a relative offset (a signed 
     * offset relative to the current value of the instruction pointer in
     * the EIP register). A relative offset (rel8, rel16, or rel32) is 
     * gernerally specified as a label in assembly code, but at the machine 
     * level, it is encoded as a signed, 8-bit or 32-bit immediate value,
     * which is added to the instruction pointer. Instruction coding is most
     * efficient for offsets of -128 to +127. If the operand-size attribute
     * is 16, then upper two bytes of the EIP register are cleared, 
     * resulting in a maximum instruction pointer size of 16 bits.
     */
    CX = 0x7;
    DX = 0x3;
    __asm__ ("mov %1, %%al\n\r"
             "mov %2, %%bl\n\r"
             "test %%bl, %%al\n\r"
             "jp PF_SQ\n\r"
             "jnp PF_CQ\n\r"
       "PF_SQ:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outQ\n\r"
       "PF_CQ:\n\r"
             "mov $0, %%dx\n\r"
        "outQ:\n\r"
             "mov %%dx, %0"
             : "=m" (PF)
             : "m" (CX), "m" (DX));
    if (PF)
        printk("JP/JNP: even number -> %#x TEST %#x = %#x\n", CX, DX, CX & DX);
    else
        printk("JP/JNP: odd number -> %#x TEST %#x = %#x\n", CX, DX, CX & DX);

#endif

#if defined CONFIG_DEBUG_PF_JPE || defined CONFIG_DEBUG_PF_JPO
    /*
     * JPE -- Jump short if parity even (PF = 1)
     * JPO -- Jump short if parity odd (PF = 0)
     *
     * Checks the state of one or more of the status flags in the EFLAGS
     * register (PF) and, if the flags are in the specified state (condition),
     * performs a jump tp the target instruction specified by the destination
     * operand. A condition code (cc) is associated with each instruction
     * to indicate the condition being tested for. If the condition is not 
     * statisfied, the jump is not performed and execution continues with
     * instruction the Jcc instruction.
     *
     * The target instruction is specified with a relative offset (a signed 
     * offset relative to the current value of the instruction pointer in
     * the EIP register). A relative offset (rel8, rel16, or rel32) is 
     * gernerally specified as a label in assembly code, but at the machine 
     * level, it is encoded as a signed, 8-bit or 32-bit immediate value,
     * which is added to the instruction pointer. Instruction coding is most
     * efficient for offsets of -128 to +127. If the operand-size attribute
     * is 16, then upper two bytes of the EIP register are cleared, 
     * resulting in a maximum instruction pointer size of 16 bits.
     */
    CX = 0x7;
    DX = 0x3;
    __asm__ ("mov %1, %%al\n\r"
             "mov %2, %%bl\n\r"
             "test %%bl, %%al\n\r"
             "jpe PF_SR\n\r"
             "jpo PF_CR\n\r"
       "PF_SR:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outR\n\r"
       "PF_CR:\n\r"
             "mov $0, %%dx\n\r"
        "outR:\n\r"
             "mov %%dx, %0"
             : "=m" (PF)
             : "m" (CX), "m" (DX));
    if (PF)
        printk("JPE/JPO: even number -> %#x TEST %#x = %#x\n", CX, DX, CX & DX);
    else
        printk("JPE/JPO: odd number -> %#x TEST %#x = %#x\n", CX, DX, CX & DX);

#endif

    return 0;
}

/*
 * ZF ---- Zero flag
 *
 * Set if the result is zero; deard otherwise.
 */
static __unused int eflags_ZF(void)
{
    unsigned char  __unused AL;
    unsigned char  __unused ZF;
    unsigned short __unused AX;
    unsigned short __unused BX;
    unsigned short __unused CX;
    unsigned short __unused DX;
    unsigned short __unused DS;
    unsigned short __unused ES;
    unsigned short __unused GS;
    unsigned short __unused FS;
    unsigned int   __unused EAX;
    unsigned int   __unused EBX;
    unsigned int   __unused ECX;
    unsigned int   __unused EDX;

#ifdef CONFIG_DEBUG_ZF_AAD
    /*
     * AAD -- ASCII Adjust AX Before Division
     *
     * Adjusts two unpacked BCD digits (the least-significant digit in the AL
     * register and the most-significant digit in the AH register) so that a
     * division operation performed on the result will yield a correct
     * unoacked BCD value. The AAD instruction is only useful when it precedes
     * a DIV instruction that divides (binary division) the adjusted value
     * in the AX register by an unpacked BCD value.
     *
     * The AAD instruction sets the value in the AL register to (AL + (10 * AH))
     * and then clears the AH register to 00H. The value in the AX register
     * is then equal to the binary equivalent of the original unpacked two-
     * digit (base 10) number in register AH and AL.
     *
     * The generalized version of this instruction allows adjustment of two
     * unpacked digits of any number base, by setting the imm8 byte to the 
     * selected number base (for example, 08H for octal, 0AH for decimal, or
     * 0CH for base 12 numbers). The AAD mnemonic is interpreted by all 
     * assemblyers to means adjust ASCII (base 10) values. To adjust values in
     * another number base, the instruction must be hand coded in machine
     * code (D5 imm8)
     *
     * IF AAD instruction
     *     THEN
     *         tempAL <---- AL;
     *         tmepAH <---- AH;
     *         AL <---- (tmpAL + (tmpAH * imm8)) AND 0xFFH;
     *         (* imm8 is set to 0AH for the AAD mnemonic *)
     *         AH <---- 0
     * FI
     */
    CX = 0x0;
    DX = 0x2;
    __asm__ ("mov %2, %%ax\n\r"
             "mov %3, %%bl\n\r"
             "aad\n\r"
             "jz ZF_0S\n\r"
             "jz ZF_0C\n\r"
       "ZF_0S:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out0\n\r"
       "ZF_0C:\n\r"
             "mov $0, %%dx\n\r"
        "out0:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("AAD: Zero -> %#x / %#x = %#x\n", CX, DX, AX);
    else
        printk("AAD: No-Zero -> %#x / %#x = %#x\n", CX, DX, AX);
#endif

#ifdef CONFIG_DEBUG_ZF_AAM
    /*
     * AAM -- ASCII adjust AX after multiply
     *
     * Adjust the result of the multiplication of two unpacked BCD values to
     * create a pair of unpacked (base 10) BCD values. The AX register is 
     * the implied source and destination operand for this instruction. The
     * The AAM instruction is only useful when it follows an MUL instruction
     * that multiplies (binary multiplication) two unpacked BCD values and 
     * stores a word result in the AX register. The AAM instruction then 
     * adjusts contents of the AX register to contain the correct 2-digit 
     * unpacked (base 10) BCD result.
     *
     * The general version of this instruction allows adjustment of the 
     * contents of the AX to create two unpacked digits of any number base.
     * Here, the imm8 byte is set to the selected number base (for example,
     * 0x08H for octal, 0AH for decimal, or 0CH for base 12 numbers). The 
     * AAM mnemonic is interpreted by all assemblers to mean adjust to ASCII
     * (base 10) values. To adjust to values in another number base, the 
     * instruction must be hand coded inb machine code (D4 imm8)
     *
     * IF instruction AAM
     *     THEN
     *         temp <---- AL;
     *         AH <---- tempAL / imm8; 
     *         (* imm8 is set to 0AH for the AAM mnemonic *)
     *         AL <---- temp MODE imm8
     * FI
     */
    CX = 0x7;
    DX = 0x0;
    __asm__ ("mov %3, %%ax\n\r"
             "mov %4, %%bl\n\r"
             "mul %%bl\n\r"
             "mov %%ax, %0\n\r"
             "aam\n\r"
             "jz ZF_S1\n\r"
             "jnz ZF_C1\n\r"
       "ZF_S1:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out1\n\r"
       "ZF_C1:\n\r"
             "mov $0, %%dx\n\r"
        "out1:\n\r"
             "mov %%dx, %1\n\r"
             "mov %%ax, %2\n\r"
             : "=m" (AX), "=m" (ZF), "=m" (BX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("AAM: Zero -> %#x * %#x = %#x (%#x)\n", CX, DX, BX, AX);
    else
        printk("AAM: No-Zero -> %#x * %#x = %#x (%#x)\n", CX, DX, BX, AX);
#endif

#ifdef CONFIG_DEBUG_ZF_ADC
    /*
     * ADC -- Add with Carry
     *
     * Adds the destination operand (first operand), the source operand (
     * second operand), and the carry (CF) flag and stores the result in the
     * destination operand. The destination operand can be a register or a
     * memory location; the source operand can be an immediate, a register,
     * or a memory location. (However, two memory operand cannot be used in
     * one instruction.) The state of the CF flag represents a carry from
     * a previous addition. When an immediate value is used as an operand,
     * it is sign-extended to the length of the destination operand format.
     *
     * The ADC instruction does not distinguish between signed or unsigned
     * operands. Instead, the processor evaluates the result for both data
     * types and sets the OF and CF flags to indicate a carry in the signed
     * unsigned result, respectively. The SF flags indicates the sign of the
     * signed result.
     *
     * DEST <---- DEST + SRC + CF
     *
     */
    CX = -2;
    DX = 1;
    __asm__ ("stc\n\r"
             "mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "adc %%bl, %%al\n\r"
             "jz ZF_S2\n\r"
             "jz ZF_C2\n\r"
       "ZF_S2:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out2\n\r"
       "ZF_C2:\n\r"
             "mov $0, %%dx\n\r"
        "out2:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("ADC: Zero -> %#x + %#x + CF = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("ADC: No-Zero -> %#x + %#x + CF = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_AND
    /*
     * AND -- Logical AND
     *
     * Performs a bitwise AND operation on the destination (first) and source
     * (second) operands and stores the result in the destination operand
     * location. The source operand can be an immediate, a register, or a 
     * memory location; the destination operand can be a register or a memory
     * location. (However, two memory operands cannot be used in one 
     * instruction.) Each bit of the result is set to 1 if both corresponding
     * bits of the first and second operands are 1; otherwise, it is set to 0.
     *
     * DEST <---- DEST AND SRC
     */
    CX = 0x0A;
    DX = 0x05;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "and %%bl, %%al\n\r"
             "jz ZF_S3\n\r"
             "jnz ZF_C3\n\r"
      "ZF_S3:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out3\n\r"
      "ZF_C3:\n\r"
             "mov $0, %%dx\n\r"
       "out3:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("AND: Zero -> %#x & %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("AND: No-Zero -> %#x & %#x = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_CMP
    /*
     * CMP -- Compare two operands
     *
     * Compares the first source operand with the second source operand and
     * the status flags in the EFLAGS register according to the result. The
     * comparison is performed by subtracting the second operand from the
     * first operand and then setting the status flags in the same manner
     * as the SUB instruction. When an immediate value is used as an operand,
     * it is sign-extended to the length of the first operand.
     *
     * temp <---- SRC1 - SignExtend(SRC2);
     * ModifyStatusFlags; 
     * (* Modify status flags in the same manner as the SUB instruction *)
     */
    CX = 0x105;
    DX = 0x05;
    __asm__ ("mov %1, %%al\n\r"
             "mov %2, %%bl\n\r"
             "cmp %%bl, %%al\n\r"
             "jz ZF_S4\n\r"
             "jnz ZF_C4\n\r"
      "ZF_S4:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out4\n\r"
      "ZF_C4:\n\r"
             "mov $0, %%dx\n\r"
       "out4:\n\r"
             "mov %%dx, %0"
             : "=m" (ZF)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("CMP: Zero -> %#x - %#x = %#x\n", CX, DX, (CX - DX));
    else
        printk("CMP: No-Zero -> %#x - %#x = %#x\n", CX, DX, (CX - DX));
#endif

#ifdef CONFIG_DEBUG_ZF_CMPSB
    /*
     * CMPSB -- Compare string operands in byte
     *
     * Compares the byte with the first source operand with the byte specified
     * with the second source operand and sets the status flags in EFLAGS 
     * register according to the result.
     *
     * Both source operands are located in memory. The address of the first
     * source operand is read from DS:SI (depending on the address-size 
     * attribute of the instruction is 16). The address of the second source
     * operand is read from ES:DI (again depending on the address-size 
     * attribute of the instruction is 16). The DS segment may be overriden
     * with a segment override prefix, but the ES segment cannot be overridden.
     *
     * At the assembly-code level, two forms of this instruction are allowed:
     * the "explicit-operands" from and the "no-operands" from. The explicit-
     * operands form (specified with the CMPS mnemonic) allows the two source
     * operands to the specified explicitly. Here, the source operands should
     * be symbols that indicate the size and location of the source values.
     * This explicit-operand form is provided to allow documentation. However,
     * not that the documentation provided by this form can be misleading.
     * That is, the source operand symbols must specify the correct type
     * (size) of the operands (bytes), but they do not have to specify the
     * correct location. Locations of the source operands are always specified
     * by the DS:SI and ES:DI register, which must be loaded correctly before
     * the compare string instruction is executed.
     *
     * The size of the source operands is selected with the mnemonic: CMPSB
     * (byte comparison). After the comparison, the SI and DI registers
     * increment or decrement automatically according to the setting of the
     * DF flag in the EFLAGS register. (If the DF flags is 0, the SI and DI
     * register increment; if the DF flag is 1, the register decrement.) The
     * register increment or decrement by 1 for byte operations.
     *
     * The CMPSB instruction can be preceded by the REP prefix for block
     * comparisons. More often, however, these instructions will be used in 
     * a LOOP construct that takes some action based on the setting of the 
     * status flags before the next comparison is made.
     */
    ECX = (unsigned int)(unsigned long)&"Hello BiscuitOS"[0];
    EDX = (unsigned int)(unsigned long)&"Hello World"[0];
    CX  = 15;
    __asm__ ("movl %2, %%edi\n\r"
             "movl %3, %%esi\n\r"
             "mov %4, %%cx\n\r"
             "cld\n\r"
             "repz cmpsb\n\r"
             "jz ZF_S5\n\r"
             "jnz ZF_C5\n\r"
       "ZF_S5:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out5\n\r"
       "ZF_C5:\n\r"
             "mov $0, %%dx\n\r"
        "out5:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%cx, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (ECX), "m" (EDX), "m" (CX));
    if (ZF)
        printk("CMPSB: Zero-> ");
    else
        printk("CMPSB: Non-Zero -> ");
    printk("\"%s\" DIFF \"%s\" on %d byte\n", (char *)(unsigned long)ECX, 
                         (char *)(unsigned long)EDX, CX - AX - 1);
    printk("'%c' is different '%c'\n", 
                ((char *)(unsigned long)ECX)[CX - AX - 1],
                ((char *)(unsigned long)EDX)[CX - AX - 1]);
#endif

#ifdef CONFIG_DEBUG_ZF_CMPSW
    /*
     * CMPSW -- Compare string operands in word
     *
     * Compares the byte with the first source operand with the byte specified
     * with the second source operand and sets the status flags in EFLAGS 
     * register according to the result.
     *
     * Both source operands are located in memory. The address of the first
     * source operand is read from DS:SI (depending on the address-size 
     * attribute of the instruction is 16). The address of the second source
     * operand is read from ES:DI (again depending on the address-size 
     * attribute of the instruction is 16). The DS segment may be overriden
     * with a segment override prefix, but the ES segment cannot be overridden.
     *
     * At the assembly-code level, two forms of this instruction are allowed:
     * the "explicit-operands" from and the "no-operands" from. The explicit-
     * operands form (specified with the CMPS mnemonic) allows the two source
     * operands to the specified explicitly. Here, the source operands should
     * be symbols that indicate the size and location of the source values.
     * This explicit-operand form is provided to allow documentation. However,
     * not that the documentation provided by this form can be misleading.
     * That is, the source operand symbols must specify the correct type
     * (size) of the operands (bytes), but they do not have to specify the
     * correct location. Locations of the source operands are always specified
     * by the DS:SI and ES:DI register, which must be loaded correctly before
     * the compare string instruction is executed.
     *
     * The size of the source operands is selected with the mnemonic: CMPSW
     * (word comparison). After the comparison, the SI and DI registers
     * increment or decrement automatically according to the setting of the
     * DF flag in the EFLAGS register. (If the DF flags is 0, the SI and DI
     * register increment; if the DF flag is 1, the register decrement.) The
     * register increment or decrement by 1 for byte operations.
     *
     * The CMPSW instruction can be preceded by the REP prefix for block
     * comparisons. More often, however, these instructions will be used in 
     * a LOOP construct that takes some action based on the setting of the 
     * status flags before the next comparison is made.
     */
    ECX = (unsigned int)(unsigned long)&"Hello BiscuitOS"[0];
    EDX = (unsigned int)(unsigned long)&"Hello Word"[0];
    CX  = 15;
    __asm__ ("movl %2, %%esi\n\r"
             "movl %3, %%edi\n\r"
             "mov %4, %%cx\n\r"
             "cld\n\r"
             "repz cmpsw\n\r"
             "jz ZF_S6\n\r"
             "jnz ZF_C6\n\r"
       "ZF_S6:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out6\n\r"
       "ZF_C6:\n\r"
             "mov $0, %%dx\n\r"
        "out6:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%cx, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (ECX), "m" (EDX), "m" (CX));
    if (ZF)
        printk("CMPSW: Zero -> ");
    else
        printk("CMPSW: Non-Zero -> ");
    printk("\"%s\" DIFF \"%s\" on %d word\n", (char *)(unsigned long)ECX,
               (char *)(unsigned long)EDX, CX - AX - 1);
    ((char *)(unsigned long)ECX)[(CX - AX) * 2] = '\0';
    ((char *)(unsigned long)EDX)[(CX - AX) * 2] = '\0';
    printk("\"%s\" is different \"%s\" in first word compare\n", 
                      &((char *)(unsigned long)ECX)[(CX - AX - 1) * 2],
                      &((char *)(unsigned long)EDX)[(CX - AX - 1) * 2]);
#endif

#ifdef CONFIG_DEBUG_ZF_CMPSD
    /*
     * CMPSD -- Compare string operands in Double-word
     *
     * Compares the byte with the irst source operand with the byte specified
     * with the second source operand and sets the status flags in EFLAGS 
     * register according to the result.
     *
     * Both source operands are located in memory. The address of the first
     * source operand is read from EDS:ESI (depending on the address-size 
     * attribute of the instruction is 32). The address of the second source
     * operand is read from ES:EDI (again depending on the address-size 
     * attribute of the instruction is 32). The DS segment may be overriden
     * with a segment override prefix, but the ES segment cannot be overridden.
     *
     * At the assembly-code level, two forms of this instruction are allowed:
     * the "explicit-operands" from and the "no-operands" from. The explicit-
     * operands form (specified with the CMPS mnemonic) allows the two source
     * operands to the specified explicitly. Here, the source operands should
     * be symbols that indicate the size and location of the source values.
     * This explicit-operand form is provided to allow documentation. However,
     * not that the documentation provided by this form can be misleading.
     * That is, the source operand symbols must specify the correct type
     * (size) of the operands (bytes), but they do not have to specify the
     * correct location. Locations of the source operands are always specified
     * by the DS:ESI and ES:EDI register, which must be loaded correctly before
     * the compare string instruction is executed.
     *
     * The size of the source operands is selected with the mnemonic: CMPSD
     * (double-word comparison). After the comparison, the ESI and EDI registers
     * increment or decrement automatically according to the setting of the
     * DF flag in the EFLAGS register. (If the DF flags is 0, the SI and DI
     * register increment; if the DF flag is 1, the register decrement.) The
     * register increment or decrement by 1 for byte operations.
     *
     * The CMPSD instruction can be preceded by the REP prefix for block
     * comparisons. More often, however, these instructions will be used in 
     * a LOOP construct that takes some action based on the setting of the 
     * status flags before the next comparison is made.
     */
    ECX = (unsigned int)(unsigned long)&"Hello BiscuitOS"[0];
    EDX = (unsigned int)(unsigned long)&"Hello World"[0];
    CX  = 15;
    __asm__ ("movl %2, %%esi\n\r"
             "movl %3, %%edi\n\r"
             "mov %4, %%cx\n\r"
             "cld\n\r"
             "repz cmpsd\n\r"
             "jz ZF_S7\n\r"
             "jz ZF_C7\n\r"
       "ZF_S7:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out7\n\r"
       "ZF_C7:\n\r"
             "mov $0, %%dx\n\r"
        "out7:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%cx, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (ECX), "m" (EDX), "m" (CX));
    if (ZF)
        printk("CMPSD: Zero -> ");
    else
        printk("CMPSD: Non-Zero -> ");
    printk("\"%s\" DIFF \"%s\" on %d Double-word\n", 
          (char *)(unsigned long)ECX, (char *)(unsigned long)EDX, 
          CX - AX - 1); 
    ((char *)(unsigned long)ECX)[(CX - AX) * 4] = '\0';
    ((char *)(unsigned long)EDX)[(CX - AX) * 4] = '\0';

    printk("\"%s\" is different \"%s\" in first Double-word compare\n",
           &((char *)(unsigned long)ECX)[(CX - AX - 1) * 4], 
           &((char *)(unsigned long)EDX)[(CX - AX - 1) * 4]);
#endif

#ifdef CONFIG_DEBUG_ZF_DAA
    /*
     * DAA -- Decimal Adjust AL after Addition
     *
     * Adjust the sum of two packed BCD values to create a packed BCD result.
     * The AL register is the implied source and destination operand. The 
     * DAA instruction is only useful when it follow an ADD instruction that
     * adds (binary addition) two 2-digit, packed BCD values and stores a
     * byte result in the AL register. The DAA instruction then adjusts the
     * contents of the AL register to contain the correct 2-digit, packed
     * BCD result. If a decimal carry is detected, the CF and AF flags are
     * set accordingly.
     *
     * IF instruction DAA
     *     old_AL <---- AL
     *     old_CF <---- CF
     *     CF     <---- 0
     *     IF (((AL AND 0FH) > 9) or AF = 1)
     *         THEN
     *             AL <---- AL + 6 
     *             CF <---- old_CF or (Carry from AL <---- AL + 6)
     *             AF <---- 1
     *         ELSE
     *             AF <---- 0
     *     FI
     *     IF ((old_AL > 99H) or (old_CF = 1))
     *         THEN
     *             AL <---- AL + 60H
     *             CF <---- 1
     *         ELSE
     *             CF <---- 0
     *     FI
     * FI
     *
     * The CF and AF flags are set if the adjustment of the value results
     * in a decimal carry in either digit of the result. The SF, ZF, and PF
     * flags are set according to the result. The OF flag is undefined.
     */
    CX = 0x99;
    DX = 0x1;
    __asm__ ("mov %3, %%al\n\r"
             "mov %4, %%bl\n\r"
             "add %%bl, %%al\n\r"
             "mov %%ax, %0\n\r"
             "daa\n\r"
             "jz ZF_S8\n\r"
             "jnz ZF_C8\n\r"
       "ZF_S8:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out8\n\r"
       "ZF_C8:\n\r"
             "mov $0, %%dx\n\r"
        "out8:\n\r"
             "mov %%dx, %1\n\r"
             "mov %%ax, %2"
             : "=m" (AX), "=m" (ZF), "=m" (BX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("DAA: Zero -> %#x + %#x = %#x(%#x)\n", CX, DX, 
                        BX & 0xFF, AX & 0xFF);
    else
        printk("DAA: Non-Zero -> %#x + %#x = %#x(%#x)\n", CX, DX, 
                        BX & 0xFF, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_DAS
    /*
     * DAS -- Decimal Adjust AL after Subtraction
     *
     * Adjusts the result of the subtraction of two packed BCD values to 
     * create a packed BCD result. The AL register is the implied source
     * and destination operand. The DAS instruction is only useful when
     * it follows a SUB instruction that subtracts (binary subtraction)
     * one 2-digit, packed BCD value from another and stores a byte result
     * in the AL register. The DAS instruction then adjusts the contents of
     * the AL register to contain the correct 2-digit, packed BCD result.
     * IF a decimal borrow is detected, the CF and AF flags are set 
     * accordingly.
     *
     * IF instruction DAS
     *     old_AL <---- AL
     *     old_CF <---- CF
     *     CF     <---- 0
     *     IF (((AL AND 0FH) > 9) or AF = 1)
     *         THEN
     *             AL <---- AL - 6
     *             CF <---- old_CF or (Borrow from AL <---- AL - 6)
     *             AF <---- 1
     *         ELSE
     *             AF <---- 0
     *     FI
     *     IF ((old_AL > 99H) or (old_CF = 1))
     *         THEN
     *             AL <---- AL - 60H
     *             CF <---- 1
     *     FI
     * FI
     */
    CX = 0x100;
    DX = 0x0;
    __asm__ ("mov %3, %%ax\n\r"
             "mov %4, %%bx\n\r"
             "sub %%bx, %%ax\n\r"
             "mov %%ax, %0\n\r"
             "das\n\r"
             "jz ZF_S9\n\r"
             "jnz ZF_C9\n\r"
       "ZF_S9:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out9\n\r"
       "ZF_C9:\n\r"
             "mov $0, %%dx\n\r"
        "out9:\n\r"
             "mov %%dx, %1\n\r"
             "mov %%ax, %2"
             : "=m" (AX), "=m" (ZF), "=m" (BX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("DAS: Zero -> %#x - %#x = %#x(%#x)\n", CX, DX, BX, AX & 0xFF);
    else
        printk("DAS: Non-Zero -> %#x - %#x = %#x(%#x)\n", CX, DX, BX, 
                                                                  AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_DEC
    /*
     * DEC -- Decreament by 1
     *
     * Subtracts 1 from the destination operand, while preserving the state
     * of the CF flag. The destination operand can be a register or a memory
     * location. This instrution allows a loop counter to be updated without
     * disturbing the CF flag. (To perform a decrement operation that updates
     * the CF flag, use a SUB instruction with an immediate operand of 1.)
     *
     * DEST <---- DEST - 1
     */
    CX = 0x1;
    __asm__ ("mov %2, %%al\n\r"
             "dec %%al\n\r"
             "jz ZF_SA\n\r"
             "jnz ZF_CA\n\r"
       "ZF_SA:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outA\n\r"
       "ZF_CA:\n\r"
             "mov $0, %%dx\n\r"
        "outA:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX));
    if (ZF)
        printk("DEC: Zero -> %#x - 1 = %#x\n", CX, AX & 0xFF);
    else
        printk("DEC: No-Zero -> %#x - 1 = %#x\n", CX, AX & 0xFF);

#endif

#ifdef CONFIG_DEBUG_ZF_INC
    /*
     * INC -- Increment by 1
     *
     * Adds 1 to the destination operand, while preserving the state of CF
     * flag. The destination operand can be a register or a memory location.
     * This instruction allows a loop counter to the updated without 
     * disturbing the CF flag. (Use a ADD instruction with an immediate 
     * operand of 1 to perform an increment operation that does updates the
     * CF flag.)
     *
     * DEST <---- DEST + 1;
     */
    CX = -1;
    __asm__ ("mov %2, %%al\n\r"
             "inc %%al\n\r"
             "jz ZF_SB\n\r"
             "jnz ZF_CB\n\r"
       "ZF_SB:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outB\n\r"
       "ZF_CB:\n\r"
             "mov $0, %%dx\n\r"
        "outB:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX));
    if (ZF)
        printk("INC: Zero -> %#x + 1 = %#x\n", CX, AX & 0xFF);
    else
        printk("INC: Non-Zero -> %#x + 1 = %#x\n", CX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_NEG
    /*
     * NEG -- two's complement negation
     *
     * Replaces the value of operand (the destination operand) with its 
     * two's complement. (This operation is equivalent to subtraction the 
     * operand from 0.) The destination operand is located in a general-
     * purpose register or a memory location.
     *
     * IF DEST = 0
     *     THEN CF <---- 0;
     *     ELSE CF <---- 0;
     * FI
     * DEST <---- [~(DEST)]
     */
    CX = 0x0;
    __asm__ ("mov %2, %%al\n\r"
             "neg %%al\n\r"
             "jz ZF_SC\n\r"
             "jnz ZF_CC\n\r"
       "ZF_SC:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outC\n\r"
       "ZF_CC:\n\r"
             "mov $0, %%dx\n\r"
        "outC:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX));
    if (ZF)
        printk("NEG: Zero -> ~(%#x) + 1 = %#x\n", CX, AX & 0xFF);
    else
        printk("NEG: Non-Zero -> ~(%#x) + 1 = %#x\n", CX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_OR
    /*
     * OR -- Logical inclusive OR
     *
     * Performs a bitwise inclusive OR operation between the destination 
     * (first) and source (second) operands and stores the result int the
     * destination operand location. The source operand can be an immediate,
     * a register, or a memory location; the destination operand can be a
     * register or a memory location. (However, two memory operands cannot
     * be used in one instruction.) Each bit of the result of the OR 
     * instruction is set to 0 if both corresponding bits of the first and
     * second operands are 0; otherwise, each bit is set to 1.
     *
     * DEST <---- DEST OR SRC;
     */
    CX = 0x0;
    DX = 0x0;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "or %%bl, %%al\n\r"
             "jz ZF_SD\n\r"
             "jnz ZF_CD\n\r"
       "ZF_SD:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outD\n\r"
       "ZF_CD:\n\r"
             "mov $0, %%dx\n\r"
        "outD:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("OR: Zero -> %#x OR %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("OR: Non-Zero -> %#x OR %#x = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_SAL
    /*
     * SAL -- Shift arithmetic left
     * 
     * SAL r/m8, CL
     * SAL r/m16, CL
     * SAL r/m32, CL
     *
     * Shifts the bits in the first operand (destination operand) to the left
     * by the number of bits specified in the second operand (count operand).
     * Bits shifted beyond the destination operand boundary are first shifted
     * into the CF flag, then discarded. At the end of the shift operation, 
     * the CF flag contains the last bit shifted out of the destination 
     * operand.
     *
     * The shift arithmetic left (SAL) and shift logical left (SHL) 
     * instructions perform the same operation; they shift the bits in the 
     * destination operand to the left (toward more significant bit locations).
     * For each shift count, the most significant bit of the destination 
     * operand is shifted into the CF flag, and the least significant bit is
     * cleared.
     */
    CX = 0x40;
    DX = 0x2;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%cl\n\r"
             "sal %%cl, %%al\n\r"
             "jz ZF_SE\n\r"
             "jnz ZF_CE\n\r"
       "ZF_SE:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outE\n\r"
       "ZF_CE:\n\r"
             "mov $0, %%dx\n\r"
        "outE:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("SAL: Zero -> %#x << %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SAL: Non-Zero -> %#x << %#x = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_SAR
    /*
     * SAR -- Shift arithmetic right
     *
     * SAR r/m8, CL
     * SAR r/m16, CL
     * SAR r/m32, CL
     *
     * Shifts the bits in the first operand (destination operand) to the right
     * by the number of bits specified in the second operand (count operand).
     * Bits shifted beyond the destination operand boundary are first into
     * the CF flag, then discarded. At the end of the shift operation, the 
     * CF flag contains the last bit shifted out of the destination operand.
     *
     * The shift arithmetic right (SAR) instruction shift the bits of the 
     * bits of the destination operand to the right (toward less significant
     * bit locations). For each shift count, the least significant bit of the
     * destination operand is shifted into the CF flag, and the most 
     * significant bit is either set or clear depending on the instruction
     * type. The SAR instruction sets or clears the most significant bit to
     * correspond to the sign (most significant bit) of the original value
     * in the destination operand. In effect, the SAR instruction fills the
     * empty bit position's shifted value with the sign of the unshifted value.
     */
    CX = 0x2;
    DX = 0x2;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%cl\n\r"
             "sar %%cl, %%al\n\r"
             "jz ZF_SF\n\r"
             "jnz ZF_CF\n\r"
       "ZF_SF:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outF\n\r"
       "ZF_CF:\n\r"
             "mov $0, %%dx\n\r"
        "outF:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (ZF) 
        printk("SAR: Zero -> %#x >> %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SAR: Non-Zero -> %#x >> %#x = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_SHL
    /*
     * SHL -- Shift logical Left
     *
     * SHL r/m8,  cl
     * SHL r/m16, cl
     * SHL r/m32, cl
     *
     * Shifts the bits in the first operand (destination operand) to the left
     * by the number of bits specified in the second operand (count operand).
     * Bits shifted beyond the destination operand boundary are first shifted
     * into the CF flag, then discared. At the end of the shift operation, the
     * CF flag contains the last bit shifted out of the destination operand.
     * 
     * The shift logical left (SHL) instruction perform the same operation;
     * they shift the bits in the destination operand to the left (toward
     * more significant bit locations). For each shift count, the most 
     * significant bit of the destination operand is shifted into the CF flag,
     * and the least significant bit is cleared.
     */
    CX = 0x40;
    DX = 0x02;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%cl\n\r"
             "shl %%cl, %%al\n\r"
             "jz ZF_SG\n\r"
             "jnz ZF_CG\n\r"
       "ZF_SG:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outG\n\r"
       "ZF_CG:\n\r"
             "mov $0, %%dx\n\r"
        "outG:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("SHL: Zero -> %#x << %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SHL: Non-Zero -> %#x << %#x = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_SHR
    /*
     * SHR --- Shift logical right
     *
     * SHR r/m8, CL
     * SHR r/m16, CL
     * SHR r/m32, CL
     *
     * Shift the bit the first operand (destination operand) to the right by
     * the number of bits specified in the second operand (count operand).
     * Bits shifted beyond the destination operand boundary are first shift
     * into the CF flag, hten discared. At the end of the shift operation, 
     * the CF flag contains the last bit shifted out of the destination 
     * operand.
     *
     * The shift logical right (SHR) instrution shift the bits of the 
     * destination operand to the right (toward less significant bit location).
     * For each shift count, the least significant bit of the destination
     * operand is shifted into the CF flag, and the most significant bit is
     * either set or cleared depending on the instruction type. 
     */
    CX = 0x4;
    DX = 0x2;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%cl\n\r"
             "shr %%cl, %%al\n\r"
             "jz ZF_SH\n\r"
             "jnz ZF_CH\n\r"
       "ZF_SH:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outH\n\r"
       "ZF_CH:\n\r"
             "mov $0, %%dx\n\r"
        "outH:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("SHR: Zero -> %#x >> %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SHR: Non-Zero -> %#x >> %#x = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_SBB
    /*
     * SBB -- Integer Subtraction with Borrow
     *
     * SBB AL, imm8
     * SBB AX, imm16
     * SBB EAX, imm32
     *
     * Adds the source operand (second operand) and the carry (CF) flag, and
     * subtracts the result from the destination operand (first operand). 
     * The result of the subtraction is stored in the destination operand. 
     * The destination operand can be register or a memory location; the
     * source operand can be an immediate, a register, or a memory location.
     * (However, two memory operands cannot be used in one instruction.) The
     * state of the CF flag represents a borrow from a previous subtraction.
     *
     * When an immediate value is used as an operand, it is sign-extended to 
     * the length of the detination operand format.
     *
     * The SBB instruction does not distinguish between signed or unsigned
     * operands. Instead, the processor evaluates the result for both data
     * types and sets the OF and CF flags to indicate a borrow in the signed
     * or unsigned result, respectively. The SF flag flag indicates the
     * sign of the sign of the signed result.
     *
     * The SBB instruction is usually executed as part of a multibyte or 
     * multiword subtraction in which a SUB instruction is followed by a
     * SBB instruction.
     *
     * DEST <---- (DEST  - (SRC + CF))
     *
     * The OF, SF, ZF, AF, PF, and CF flags are set according to the result.
     */
    CX = 0x1;
    DX = 0x0;
    __asm__ ("stc\n\r"
             "mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "sbb %%bl, %%al\n\r"
             "jz ZF_SI\n\r"
             "jnz ZF_CI\n\r"
       "ZF_SI:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outI\n\r"
       "ZF_CI:\n\r"
             "mov $0, %%dx\n\r"
        "outI:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("SBB: Zero -> %#x - %#x - 1 = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SBB: Non-Zero -> %#x - %#x - 1 = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_SCASB
    /*
     * SCASB -- Scan String in Byte
     *
     * The no-operands form of the instruction uses a short form of SCAS. 
     * Again, ES:(E)DI is assumed to be the memory operand and AL, AX, or EAX
     * is assumed to be the register operand. The size of operands is 
     * selected by the mnemonic: SCASB (byte comparision).
     * 
     * After the comparison, the (EDI) register is increamented or decremented
     * automatically according to the setting of the DF flag in the EFLAGS
     * register. If the DF flag is 0, the (E)DI register is increamented; if
     * the DF flag is 1, the (E)DI register is decremented. The register is
     * increamented or decremented by 1 for byte operations.
     *
     * SCASB can be preceded by the REP prefix for block comparisions of ECX
     * bytes. Often, however, these instructions will be used in a LOOP 
     * construct that takes some action based on the setting of status flags.
     */
    ECX = (unsigned int)(unsigned long)&"Hello World"[0];
    AL =  (unsigned char)(unsigned long)'d';
    CX = 15;
    __asm__ ("movl %2, %%edi\n\r"
             "mov %3, %%al\n\r"
             "mov %4, %%cx\n\r"
             "cld\n\r"
             "repne scasb\n\r"
             "jz ZF_SJ\n\r"
             "jnz ZF_CJ\n\r"
       "ZF_SJ:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outJ\n\r"
       "ZF_CJ:\n\r"
             "mov $0, %%dx\n\r"
        "outJ:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%cx, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (ECX), "m" (AL), "m" (CX));
    if (ZF)
        printk("SCASB: Zero -> ");
    else
        printk("SCASB: Non-Zero -> ");
    printk("\"%s\" Scan \"%#x\" in %d byte\n",
           (char *)(unsigned long)ECX, AL, CX - AX - 1);
#endif

#ifdef CONFIG_DEBUG_ZF_SCASW
    /*
     * SCASW -- Scan String in Word
     *
     * The no-operands form of the instruction uses a short form of SCAS. 
     * Again, ES:(E)DI is assumed to be the memory operand and AL, AX, or EAX
     * is assumed to be the register operand. The size of operands is 
     * selected by the mnemonic: SCASW (word comparision).
     * 
     * After the comparison, the (EDI) register is increamented or decremented
     * automatically according to the setting of the DF flag in the EFLAGS
     * register. If the DF flag is 0, the (E)DI register is increamented; if
     * the DF flag is 1, the (E)DI register is decremented. The register is
     * increamented or decremented by 1 for byte operations.
     *
     * SCASW can be preceded by the REP prefix for block comparisions of ECX
     * word. Often, however, these instructions will be used in a LOOP 
     * construct that takes some action based on the setting of status flags.
     */
    ECX = (unsigned int)(unsigned long)&"Hello World"[0];
    AX  = (unsigned short)(unsigned long)(('o' << 8) | 'W');
    CX  = 15;
    __asm__ ("movl %2, %%edi\n\r"
             "mov %3, %%ax\n\r"
             "mov %4, %%cx\n\r"
             "cld\n\r"
             "repne scasw\n\r"
             "jz ZF_SK\n\r"
             "jnz ZF_CK\n\r"
       "ZF_SK:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outK\n\r"
       "ZF_CK:\n\r"
             "mov $0, %%dx\n\r"
        "outK:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%cx, %1"
             : "=m" (ZF), "=m" (BX)
             : "m" (ECX), "m" (AX), "m" (CX));
    if (ZF)
        printk("SCASW: Zero -> ");
    else
        printk("SCASW: Non-Zero -> ");
    printk("\"%s\" scan \"%c%c\" in %d word\n", (char *)(unsigned long)ECX,
                 (unsigned char)(unsigned long)((AX >> 0) & 0xFF), 
                 (unsigned char)(unsigned long)((AX >> 8) & 0xFF), 
                 CX - BX - 1);
#endif

#ifdef CONFIG_DEBUG_ZF_SUB
    /*
     * SUB -- Subtract
     *
     * SUB AL, imm8
     * SUB AX, imm16
     * SUB EAX, imm32
     *
     * Subracts the second operand (source operand) from the first operand (
     * destination operand) and stores the result in the destination operand.
     * The destination operand can be a register or a memory location; the 
     * source operand can be an immediate, register, or memory location. (
     * However, two memory operands cannot be used in one instruction.) When
     * an immediate value is used as an operand, it is sign-extended to the
     * length of the destination operand format.
     *
     * The SUB instruction performs integer subtraction. It evaluates the 
     * result for both signed and unsigned integer operands and sets the
     * OF and CF flags to indicate an overflow in the signed or unsigned
     * result, respectively. The SF flag indicates the sign of the signed
     * result.
     *
     * DEST <---- (DEST - SRC)
     *
     * The OF, SF, ZF, AF, PF, and CF flags are set according to the result.
     */
    CX = 0x1;
    DX = 0x1;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "sub %%bl, %%al\n\r"
             "jz ZF_SL\n\r"
             "jnz ZF_CL\n\r"
       "ZF_SL:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outL\n\r"
       "ZF_CL:\n\r"
             "mov $0, %%dx\n\r"
        "outL:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("SUB: Zero -> %#x - %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("SUB: Non-Zero -> %#x - %#x = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_TEST
    /*
     * TEST -- Logical Compare
     *
     * Computes the bit-wise logical AND of first operand (source 1 operand)
     * and second operand (source 2 operand) and sets the SF, ZF, and PF
     * status flags according to the result. The result is then discarded.
     *
     */
    CX = 0xAA;
    DX = 0x55;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "test %%bl, %%al\n\r"
             "jz ZF_SM\n\r"
             "jnz ZF_CM\n\r"
       "ZF_SM:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outM\n\r"
       "ZF_CM:\n\r"
             "mov $0, %%dx\n\r"
        "outM:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("TEST: Zero -> %#x & %#x = %#x\n", CX, DX, (CX & DX) & 0xFF);
    else
        printk("TEST: Non-Zero -> %#x & %#x = %#x\n", CX, DX, 
                    (CX & DX) & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_ARPL
    /*
     * ARPL -- Adjust RPL field of segment selector
     *
     * Compares the PRL fields of two segment selectors. The first operand (
     * the destination operand) contains one segment selector and the second
     * operand (source operand) contains the other. (The RPL field is located
     * in bits 0 and 1 of each operand.) If the RPL field of the destination
     * operand is less then the PRL field of the source operand, the ZF flag
     * is set and the RPL field of the destination operand is increased to 
     * match that of the source operand. Otherwise, the ZF flag is cleared and
     * no change  is made to the destination operand. (The destination operand
     * can be a word register or memory location; then source operand must be
     * a word register.)
     *
     * The ARPL instruction is provided for use by operating-system procedures
     * (however, it can also be used by applications). It is generally used
     * to adjust the PRL of a segment selector that has been passed to the 
     * operating system by an application program to match the privilege level
     * of the application program. Here the segment selector passed to the 
     * operating system is placed in the destination operand and segment 
     * selector for the application program's code segment is placed in the 
     * source operand. (The RPL field in the source operand represents the 
     * privilege level of the application program.) Execution of the ARPL
     * instruction then ensures that the PRL of the segment selector received
     * by the operating system is no lower (does not have a higher privilege)
     * than the privilege level of the application program (the segment 
     * selector for the application program's code segment can be read from
     * the stack following a procedure call).
     *
     * IF 64-BIT MODE
     *   THEN
     *     See MOVSXD
     *   ELSE
     *     IF DEST [RPL] < SRC [RPL]
     *       THEN
     *         ZF <---- 1;
     *         DEST [RPL] <---- SRC[RPL];
     *       ELSE
     *         ZF <---- 0;
     *     FI
     * FI
     *
     * Flags Affected
     * 
     * The ZF flag is set to 1 if the RPL field of the destination operand
     * is less than of the source operand; otherwise, it is set to 0.
     */
    CX = 0x01;
    __asm__ ("mov %3, %%cx\n\r"
             "mov %%es, %%ax\n\r"
             "mov %%ax, %0\n\r"
             "arpl %%cx, %%ax\n\r"
             "jz ZF_SN\n\r"
             "jnz ZF_CN\n\r"
       "ZF_SN:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outN\n\r"
       "ZF_CN:\n\r"
             "mov $0, %%dx\n\r"
        "outN:\n\r"
             "mov %%dx, %1\n\r"
             "mov %%ax, %2"
             : "=m" (AX), "=m" (ZF), "=m" (BX)
             : "m" (CX));
    if (ZF) {
        printk("ARPL: Zero -> ES segment ARPL %#x less %#x\n", AX & 0x3, CX);
        printk("Change ES Segment ARPL %#x to %#x\n", AX & 0x3, BX & 0x3);
    } else {
        printk("ARPL: Non-Zero -> ES segment ARPL %#x same as %#x\n", 
                                         AX & 0x3, CX);
    }
#endif

#ifdef CONFIG_DEBUG_ZF_BSF
    /*
     * BSF -- Bit scan forward
     *
     * Searches the source operand (second operand) for the least significat 
     * set bit (1 bit). If a least significant 1 bit is found, its bit index
     * is stored in the destination operand (first operand). The source 
     * operand can be a register or a memory location; the destination operand
     * is a register. The bit index is an unsinged offset from bit 0 of the
     * source operand. If the content of the source operand is 0, the content
     * of the destination operand is undefined.
     *
     * IF SRC = 0
     *   THEN
     *     ZF     <---- 1;
     *     DEST is undefined;
     *   ELSE
     *     ZF     <---- 0;
     *     temp   <---- 0;
     *     WHILE Bit(SRC, temp) = 0;
     *     DO
     *       temp <---- temp + 1;
     *     OD;
     *     DEST   <---- temp;
     * FI
     *
     * The ZF flag is set to 1 if all the source operand is 0; otherwise, the
     * ZF flag is cleared. The CF, OF, SF, AF, and PF, flags are undefined.
     */
    CX = 0x700;
    __asm__ ("mov %2, %%ax\n\r"
             "bsf %%ax, %%bx\n\r"
             "jz ZF_SO\n\r"
             "jnz ZF_CO\n\r"
       "ZF_SO:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outO\n\r"
       "ZF_CO:\n\r"
             "mov $0, %%dx\n\r"
        "outO:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%bx, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX));
    if (ZF)
        printk("BSF: Zero -> %#x can't find 1\n", CX);
    else
        printk("BSF: Non-Zero -> %#x find first least bit 1 on bit %#x\n", 
                                                   CX, AX);
#endif

#ifdef CONFIG_DEBUG_ZF_BSR
    /*
     * BSR -- Bit Scan Reverse
     *
     * Searches the source operand (second operand) for the most significant 
     * set bit (1 bit). If a most significant 1 bit is found, its bit index
     * is stored in the destination operand (first operand). The source operand
     * can be a register or a memory location; the destination operand is a
     * register. The bit index is an unsigned offset from bit 0 of the source
     * operand. If the content source operand is 0, the content of the 
     * destination operand is undefined.
     *
     * IF SRC = 0
     *   THEN
     *     ZF     <---- 1;
     *     DEST is undefined
     *   ELSE
     *     zF     <---- 0;
     *     temp   <---- OperandSize - 1;
     *     WHILE BIT(SRC, temp) = 0
     *     DO
     *       temp <---- temp - 1;
     *     OD;
     *     DEST   <---- temp;
     * FI
     *
     * The ZF flag is set to 1 if all the source operand is 0; otherwise. the
     * ZF flag is cleared.
     */
    CX = 0x300;
    __asm__ ("mov %2, %%ax\n\r"
             "bsr %%ax, %%bx\n\r"
             "jz ZF_SP\n\r"
             "jnz ZF_CP\n\r"
       "ZF_SP:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outP\n\r"
       "ZF_CP:\n\r"
             "mov $0, %%dx\n\r"
        "outP:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%bx, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX));
    if (ZF)
        printk("BSR: Zero -> %#x can't find 1\n", CX);
    else
        printk("BSR: Non-Zero -> %#x find first most bit 1 on bit %#x\n",
                                                  CX, AX);
#endif

#ifdef CONFIG_DEBUG_ZF_CMPXCHG
    /*
     * CMPXCHG -- Compare and Exchange
     *
     * Compares the value in the AL, AX, or EAX register with the first 
     * operand (destination operand). If the two values are equal, the 
     * second operand (source operand) is loaded into the destination operand.
     * Otherwise, the destination operand is loaded into the AL, AX, or EAX
     * register.
     */

    /* Case 1 */
    DS = 0x200;
    ES = 0x200;
    GS = 0x9F;
    __asm__ ("mov %4, %%ax\n\r"
             "mov %5, %%bx\n\r"
             "mov %6, %%cx\n\r"
             "cmpxchg %%cx, %%bx\n\r"
             "jz ZF_SQ\n\r"
             "jnz ZF_CQ\n\r"
       "ZF_SQ:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outQ\n\r"
       "ZF_CQ:\n\r"
             "mov $0, %%dx\n\r"
        "outQ:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1\n\r"
             "mov %%bx, %2\n\r"
             "mov %%cx, %3"
             : "=m" (ZF), "=m" (AX), "=m" (BX), "=m" (CX) 
             : "m" (DS), "m" (ES), "m" (GS));
    printk("AX: %#x => \"CMPXCHG CX:%#x, BX:%#x\"\n", DS, GS, ES);
    if (ZF)
        printk("CMPXCHG: Zero -> AX:%#x == BX:%#x, load CX:%#x into BX:%#x\n", 
                                 DS, ES, GS, BX);
    else
        printk("CMPXCHG: Non-Zero -> AX:%#x != BX:%#x, "
                                 "load BX:%#x into AX:%#x\n", 
                                 DS, ES, ES, AX);

    /* Case 2 */
    DS = 0x200;
    ES = 0x201;
    GS = 0x9F;
    __asm__ ("mov %4, %%ax\n\r"
             "mov %5, %%bx\n\r"
             "mov %6, %%cx\n\r"
             "cmpxchg %%cx, %%bx\n\r"
             "jz ZF_SR\n\r"
             "jnz ZF_CR\n\r"
       "ZF_SR:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outR\n\r"
       "ZF_CR:\n\r"
             "mov $0, %%dx\n\r"
        "outR:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1\n\r"
             "mov %%bx, %2\n\r"
             "mov %%cx, %3"
             : "=m" (ZF), "=m" (AX), "=m" (BX), "=m" (CX)
             : "m" (DS), "m" (ES), "m" (GS));
    printk("AX: %#x => \"CMPXCHG CX:%#x, BX:%#x\"\n", DS, GS, ES);
    if (ZF)
        printk("CMPXCHG: Zero -> AX:%#x == BX:%#x, load CX:%#x into BX:%#x\n",
                                DS, ES, GS, BX);
    else
        printk("CMPXCHG: Non-Zero -> AX:%#x != BX:%#x, "
                                "load BX:%#x into AX:%#x\n",
                                DS, ES, ES, AX);
#endif

#ifdef CONFIG_DEBUG_ZF_CMPXCHG8B
    /*
     * CMPXCHG8B -- Compare and Exchange Byte
     *
     * Compare the 64-bit value in EDX:EAX with the operand (destination 
     * operand). If the values are equal, the 64-bit value in ECX:EBX is 
     * stored in the destination operand. Otherwise, the value in the
     * destination operand is loaded into EDX:EAX. The destination operand
     * is an 8-byte memory location. For the EDX:EAX and ECX:EBX register
     * pairs, EDX and ECX contain the high-order 32 bits and EAX and EBX
     * contain the low-order 32 bits of a 64-bit value.
     */

    /* Case 1: Same */

    /* Establish 64-bit number: 0x2030405060708090 */
    unsigned int dest[2]  = { 0x60708090, 0x20304050};
    unsigned int destx[2] = { 0x60708090, 0x20304050};
    /* Establish EDX:EAX number: 0x2030405060708090 */
    unsigned int edx = 0x20304050;
    unsigned int eax = 0x60708090;
    /* Establish ECX:EBX number: 0x1122334455667788 */
    unsigned int ecx = 0x11223344;
    unsigned int ebx = 0x55667788;

    printk("Case 1: Same\n");
    printk("EDX:EAX:%#x:%#x AND ECX:EBX:%#x:%#x\n", edx, eax, ecx, ebx);
    printk("CMPXCHG8B m64:%#x%x\n", dest[1], dest[0]);

     __asm__ ("movl %5, %%eax\n\r"
              "movl %6, %%edx\n\r"
              "movl %7, %%ecx\n\r"
              "movl %8, %%ebx\n\r"
              "cmpxchg8b %9\n\r"
              "jz ZF_SS\n\r"
              "jnz ZF_CS\n\r"
        "ZF_SS:\n\r"
              "mov $1, %%di\n\r"
              "jmp outS\n\r"
        "ZF_CS:\n\r"
              "mov $0, %%di\n\r"
         "outS:\n\r"
              "mov %%di, %0\n\r"
              "movl %%eax, %1\n\r"
              "movl %%ebx, %2\n\r"
              "movl %%ecx, %3\n\r"
              "movl %%edx, %4"
              : "=m" (ZF), "=m" (EAX), "=m" (EBX), "=m" (ECX), "=m" (EDX)
              : "m" (eax), "m" (edx), "m" (ecx), "m" (ebx), "m" (dest));
    if (ZF) {
        printk("CMPXCHG8B: Zero -> [EDX:EAX]:%#x:%#x == m64:%#x%x\n",
                   EDX, EAX, destx[1], destx[0]);
        printk("Load [ECX:EBX]:%#x:%#x into m64:%#x%x\n", 
                   ECX, EBX, dest[1], dest[0]);
    }

    /* Case 2: Diff */    

    /* Establish 64-bit number: 0x2030405060708099 */
    dest[0]  = 0x20304050;
    dest[1]  = 0x60708099;
    destx[2] = 0x20304050;
    destx[2] = 0x60708099;

    printk("Case 2: Diff\n");
    printk("EDX:EAX:%#x:%#x AND ECX:EBX:%#x:%#x\n", edx, eax, ecx, ebx);
    printk("CMPXCHG8B m64:%#x%x\n", dest[1], dest[0]);

     __asm__ ("movl %5, %%eax\n\r"
              "movl %6, %%edx\n\r"
              "movl %7, %%ecx\n\r"
              "movl %8, %%ebx\n\r"
              "cmpxchg8b %9\n\r"
              "jz ZF_ST\n\r"
              "jnz ZF_CT\n\r"
        "ZF_ST:\n\r"
              "mov $1, %%di\n\r"
              "jmp outT\n\r"
        "ZF_CT:\n\r"
              "mov $0, %%di\n\r"
         "outT:\n\r"
              "mov %%di, %0\n\r"
              "movl %%eax, %1\n\r"
              "movl %%ebx, %2\n\r"
              "movl %%ecx, %3\n\r"
              "movl %%edx, %4"
              : "=m" (ZF), "=m" (EAX), "=m" (EBX), "=m" (ECX), "=m" (EDX)
              : "m" (eax), "m" (edx), "m" (ecx), "m" (ebx), "m" (dest));
    if (!ZF) {
        printk("CMPXCHG8B: Non-Zero -> [EDX:EAX]:%#x:%#x != m64:%#x%x\n",
                   edx, eax, destx[1], destx[0]);
        printk("Load m64:%#x%x into [EDX:EAX]:%#x:%#x\n", 
                   dest[1], dest[0], EDX, EAX);
    }
#endif

#if  defined CONFIG_DEBUG_ZF_JZ | defined CONFIG_DEBUG_ZF_JNZ
    /*
     * JZ  -- Jump near if zero
     * JNZ -- Jump near if not zero
     *
     * Checks the state of one or more of the status flags in the EFLAGS
     * register (ZF) and, if the flags are in the specified state (condition),
     * performs a jump tp the target instruction specified by the destination
     * operand. A condition code (cc) is associated with each instruction
     * to indicate the condition being tested for. If the condition is not 
     * statisfied, the jump is not performed and execution continues with
     * instruction the Jcc instruction.
     *
     * The target instruction is specified with a relative offset (a signed 
     * offset relative to the current value of the instruction pointer in
     * the EIP register). A relative offset (rel8, rel16, or rel32) is 
     * gernerally specified as a label in assembly code, but at the machine 
     * level, it is encoded as a signed, 8-bit or 32-bit immediate value,
     * which is added to the instruction pointer. Instruction coding is most
     * efficient for offsets of -128 to +127. If the operand-size attribute
     * is 16, then upper two bytes of the EIP register are cleared, 
     * resulting in a maximum instruction pointer size of 16 bits.
     */
#ifdef CONFIG_DEBUG_ZF_JZ
    CX = 0x1;
#elif defined CONFIG_DEBUG_ZF_JNZ
    CX = 0x2;
#endif
    __asm__ ("mov %2, %%al\n\r"
             "dec %%al\n\r"
             "jz ZF_SU\n\r"
             "jnz ZF_CU\n\r"
       "ZF_SU:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outU\n\r"
       "ZF_CU:\n\r"
             "mov $0, %%dx\n\r"
        "outU:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX));
    if (ZF)
        printk("JZ: Zero -> %#x - 1 = %#x\n", CX, AX & 0xFF);
    else
        printk("JNZ: No-Zero -> %#x - 1 = %#x\n", CX, AX & 0xFF);

#endif

#if  defined CONFIG_DEBUG_ZF_JE | defined CONFIG_DEBUG_ZF_JNE
    /*
     * JE  -- Jump short if equal
     * JNE -- Jump short if not equal
     *
     * Checks the state of one or more of the status flags in the EFLAGS
     * register (ZF) and, if the flags are in the specified state (condition),
     * performs a jump tp the target instruction specified by the destination
     * operand. A condition code (cc) is associated with each instruction
     * to indicate the condition being tested for. If the condition is not 
     * statisfied, the jump is not performed and execution continues with
     * instruction the Jcc instruction.
     *
     * The target instruction is specified with a relative offset (a signed 
     * offset relative to the current value of the instruction pointer in
     * the EIP register). A relative offset (rel8, rel16, or rel32) is 
     * gernerally specified as a label in assembly code, but at the machine 
     * level, it is encoded as a signed, 8-bit or 32-bit immediate value,
     * which is added to the instruction pointer. Instruction coding is most
     * efficient for offsets of -128 to +127. If the operand-size attribute
     * is 16, then upper two bytes of the EIP register are cleared, 
     * resulting in a maximum instruction pointer size of 16 bits.
     */
    CX = 0x10;
#ifdef CONFIG_DEBUG_ZF_JE
    DX = 0x10;
#elif defined CONFIG_DEBUG_ZF_JNE
    DX = 0x05;
#endif
    __asm__ ("mov %1, %%al\n\r"
             "mov %2, %%bl\n\r"
             "cmp %%bl, %%al\n\r"
             "jz ZF_SV\n\r"
             "jnz ZF_CV\n\r"
      "ZF_SV:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outV\n\r"
      "ZF_CV:\n\r"
             "mov $0, %%dx\n\r"
       "outV:\n\r"
             "mov %%dx, %0"
             : "=m" (ZF)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("JE: Zero -> %#x - %#x = %#x\n", CX, DX, (CX - DX));
    else
        printk("JNE: No-Zero -> %#x - %#x = %#x\n", CX, DX, (CX - DX));

#endif

#if  defined CONFIG_DEBUG_ZF_JA | defined CONFIG_DEBUG_ZF_JNA
    /*
     * JA  -- Jump short if above
     * JNA -- Jump short if not above
     *
     * Checks the state of one or more of the status flags in the EFLAGS
     * register (ZF) and, if the flags are in the specified state (condition),
     * performs a jump tp the target instruction specified by the destination
     * operand. A condition code (cc) is associated with each instruction
     * to indicate the condition being tested for. If the condition is not 
     * statisfied, the jump is not performed and execution continues with
     * instruction the Jcc instruction.
     *
     * The target instruction is specified with a relative offset (a signed 
     * offset relative to the current value of the instruction pointer in
     * the EIP register). A relative offset (rel8, rel16, or rel32) is 
     * gernerally specified as a label in assembly code, but at the machine 
     * level, it is encoded as a signed, 8-bit or 32-bit immediate value,
     * which is added to the instruction pointer. Instruction coding is most
     * efficient for offsets of -128 to +127. If the operand-size attribute
     * is 16, then upper two bytes of the EIP register are cleared, 
     * resulting in a maximum instruction pointer size of 16 bits.
     */
#ifdef CONFIG_DEBUG_ZF_JA
    CX = 0x12;
#elif defined CONFIG_DEBUG_ZF_JNA
    CX = 0x10;
#endif
    DX = 0x10;
    __asm__ ("mov %1, %%al\n\r"
             "mov %2, %%bl\n\r"
             "cmp %%bl, %%al\n\r"
             "jna ZF_SW\n\r"
             "ja ZF_CW\n\r"
      "ZF_SW:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outW\n\r"
      "ZF_CW:\n\r"
             "mov $0, %%dx\n\r"
       "outW:\n\r"
             "mov %%dx, %0"
             : "=m" (ZF)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("JNA: Zero -> %#x <= %#x\n", CX, DX);
    else
        printk("JA: No-Zero -> %#x > %#x\n", CX, DX);

#endif

#if defined CONFIG_DEBUG_ZF_JNG | defined CONFIG_DEBUG_ZF_JG
    /*
     * JG   -- Jump short if greate
     * JNG  -- Jump short if not greate
     *
     * Checks the state of one or more of the status flags in the EFLAGS
     * register (ZF) and, if the flags are in the specified state (condition),
     * performs a jump tp the target instruction specified by the destination
     * operand. A condition code (cc) is associated with each instruction
     * to indicate the condition being tested for. If the condition is not 
     * statisfied, the jump is not performed and execution continues with
     * instruction the Jcc instruction.
     *
     * The target instruction is specified with a relative offset (a signed 
     * offset relative to the current value of the instruction pointer in
     * the EIP register). A relative offset (rel8, rel16, or rel32) is 
     * gernerally specified as a label in assembly code, but at the machine 
     * level, it is encoded as a signed, 8-bit or 32-bit immediate value,
     * which is added to the instruction pointer. Instruction coding is most
     * efficient for offsets of -128 to +127. If the operand-size attribute
     * is 16, then upper two bytes of the EIP register are cleared, 
     * resulting in a maximum instruction pointer size of 16 bits.
     */
#ifdef CONFIG_DEBUG_ZF_JNG
    CX = 0x10;
#elif defined CONFIG_DEBUG_ZF_JG
    CX = 0x11;
#endif
    DX = 0x10;
    __asm__ ("mov %1, %%al\n\r"
             "mov %2, %%bl\n\r"
             "cmp %%bl, %%al\n\r"
             "jng ZF_SX\n\r"
             "jg ZF_CX\n\r"
      "ZF_SX:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outX\n\r"
      "ZF_CX:\n\r"
             "mov $0, %%dx\n\r"
       "outX:\n\r"
             "mov %%dx, %0"
             : "=m" (ZF)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("JNG: Zero -> %#x <= %#x\n", CX, DX);
    else
        printk("JG: Non-Zero -> %#x > %#x\n", CX, DX);

#endif

#if defined CONFIG_DEBUG_ZF_JLE | defined CONFIG_DEBUG_ZF_JBE
    /*
     * JLE  -- Jump short if less or equal
     * JBE  -- Jump short if below or equal
     *
     * Checks the state of one or more of the status flags in the EFLAGS
     * register (ZF) and, if the flags are in the specified state (condition),
     * performs a jump tp the target instruction specified by the destination
     * operand. A condition code (cc) is associated with each instruction
     * to indicate the condition being tested for. If the condition is not 
     * statisfied, the jump is not performed and execution continues with
     * instruction the Jcc instruction.
     *
     * The target instruction is specified with a relative offset (a signed 
     * offset relative to the current value of the instruction pointer in
     * the EIP register). A relative offset (rel8, rel16, or rel32) is 
     * gernerally specified as a label in assembly code, but at the machine 
     * level, it is encoded as a signed, 8-bit or 32-bit immediate value,
     * which is added to the instruction pointer. Instruction coding is most
     * efficient for offsets of -128 to +127. If the operand-size attribute
     * is 16, then upper two bytes of the EIP register are cleared, 
     * resulting in a maximum instruction pointer size of 16 bits.
     */
    CX = 0x10;
    DX = 0x10;
    __asm__ ("mov %1, %%al\n\r"
             "mov %2, %%bl\n\r"
             "cmp %%bl, %%al\n\r"
#ifdef CONFIG_DEBUG_ZF_JBE
             "jbe ZF_SY\n\r"
#elif defined CONFIG_DEBUG_ZF_JLE
             "jle ZF_SY\n\r"
#endif
             "jnz ZF_CY\n\r"
      "ZF_SY:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outY\n\r"
      "ZF_CY:\n\r"
             "mov $0, %%dx\n\r"
       "outY:\n\r"
             "mov %%dx, %0"
             : "=m" (ZF)
             : "m" (CX), "m" (DX));
    if (ZF)
#ifdef CONFIG_DEBUG_ZF_JBE
        printk("JBE: Zero -> %#x <= %#x\n", CX, DX);
#elif defined CONFIG_DEBUG_ZF_JLE
        printk("JLE: Zero -> %#x <= %#x\n", CX, DX);
#endif

#endif

#if defined CONFIG_DEBUG_ZF_REPZ | defined CONFIG_DEBUG_ZF_REPNZ
    /*
     * REPZ   -- Repeat while zero
     * REPNZ  -- Repeat while no zero
     *
     * Repeats a string instruction the number of times specified in the
     * count register or until the indicated condition of the ZF flag is
     * no longer met. The REP prefix can added to the INS, OUTS, MOVS, LODS,
     * and STOS instructions, and the REPZ, and REPNZ prefixes can be added
     * to the CMPS and SCAS instructions.
     */
    ECX = (unsigned int)(unsigned long)&"Hello BiscuitOS"[0];
    EDX = (unsigned int)(unsigned long)&"Hello World"[0];
    CX  = 15;
    __asm__ ("movl %2, %%edi\n\r"
             "movl %3, %%esi\n\r"
             "mov %4, %%cx\n\r"
             "cld\n\r"
#ifdef CONFIG_DEBUG_ZF_REPZ
             "repz cmpsb\n\r"
#elif defined CONFIG_DEBUG_ZF_REPNZ
             "repnz cmpsb\n\r"
#endif
             "jz ZF_SZ\n\r"
             "jnz ZF_CZ\n\r"
       "ZF_SZ:\n\r"
             "mov $1, %%dx\n\r"
             "jmp outZ\n\r"
       "ZF_CZ:\n\r"
             "mov $0, %%dx\n\r"
        "outZ:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%cx, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (ECX), "m" (EDX), "m" (CX));
    if (!ZF) {
        printk("REPZ: Zero-> ");
        printk("\"%s\" DIFF \"%s\" on %d byte\n", (char *)(unsigned long)ECX,
                         (char *)(unsigned long)EDX, CX - AX - 1);
        printk("'%c' is different '%c'\n",
                ((char *)(unsigned long)ECX)[CX - AX - 1],
                ((char *)(unsigned long)EDX)[CX - AX - 1]);
    } else {
        printk("REPNZ: Non-Zero -> ");
        printk("\"%s\" SAME \"%s\" on %d byte\n", (char *)(unsigned long)ECX,
                         (char *)(unsigned long)EDX, CX - AX - 1);
        printk("'%c' is SAME '%c'\n",
                ((char *)(unsigned long)ECX)[CX - AX - 1],
                ((char *)(unsigned long)EDX)[CX - AX - 1]);
    }
#endif

#ifdef CONFIG_DEBUG_ZF_XOR
    /*
     * XOR -- Logical Exclusive OR
     *
     * Perform a bitwise exclusive OR (XOR) operation on the destination 
     * (first) and source (second) operands and stores the result in the
     * destination operand location. The source operand can be an immediate,
     * a register, or a memory location; the destination operand can be a 
     * register or a memory location. (However, two memory operands cannot be
     * used in one instruction.) Each bit of the result is 1 if the 
     * corresponding bits of the operands are different; each bit is 0 if 
     * the corresponding bits are the same.
     *
     * DEST <---- DEST XOR SRC;
     *
     * The OF and CF flags are cleared; the SF, ZF, and PF flags are set
     * according to the result. The state of the AF flag is undefined.
     */
    CX = 0x8;
    DX = 0x8;
    __asm__ ("mov %2, %%ax\n\r"
             "mov %3, %%bx\n\r"
             "xor %%bl, %%al\n\r"
             "jz ZF_S01\n\r"
             "jnz ZF_C01\n\r"
      "ZF_S01:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out01\n\r"
      "ZF_C01:\n\r"
             "mov $0, %%dx\n\r"
       "out01:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("XOR: Zero -> %#x XOR %#x = %#x\n", CX, DX, AX & 0xFF);
    else
        printk("XOR: Non-Zero -> %#x XOR %#x = %#x\n", CX, DX, AX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_XADD
    /*
     * XADD -- Echange and Add
     *
     * Exchanges the first operand (destination operand) with the second 
     * operand (source operand), then loads the sum of the two values into
     * the destination operand. The destination operand can be a register or
     * a memory location; the source operand is a register.
     *
     * TEMP    <---- SRC + DEST;
     * SRC     <---- DEST;
     * DEST    <---- TEMP;
     */
    CX = 0x1;
    DX = -1;
    __asm__ ("mov %3, %%ax\n\r"
             "mov %4, %%bx\n\r"
             "xadd %%bl, %%al\n\r"
             "jz ZF_S02\n\r"
             "jnz ZF_C02\n\r"
      "ZF_S02:\n\r"
             "mov $1, %%dx\n\r"
             "jmp out02\n\r"
      "ZF_C02:\n\r"
             "mov $0, %%dx\n\r"
       "out02:\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1\n\r"
             "mov %%bx, %2"
             : "=m" (ZF), "=m" (AX), "=m" (BX)
             : "m" (CX), "m" (DX));
    if (ZF)
        printk("XADD: Zero -> AX:%#x <+> BX:%#x = AX:%#x(BX:%#x)\n", 
                        CX, DX, AX & 0xFF, BX & 0xFF);
    else
        printk("XADD: Non-Zero -> AX:%#x <+> BX:%#x = AX:%#x(BX:%#x)\n",
                        CX, DX, AX & 0xFF, BX & 0xFF);
#endif

#ifdef CONFIG_DEBUG_ZF_SETZ
    /*
     * SETZ -- Set Byte if zero
     *
     * Sets the destination operand to 0 or 1 depending on the settings of
     * the status flags (ZF) in the EFLAGS register. The destination operand
     * points to a byte register or a byte in memory. The condition code
     * suffix (cc) indicates the condition being tested for.
     */
    CX = 0x1;
    __asm__ ("mov %2, %%al\n\r"
             "mov $0, %%bx\n\r"
             "dec %%al\n\r"
             "setz %%bl\n\r"
             "mov %%bl, %0\n\r"
             "mov %%ax, %1"
             : "=m" (ZF), "=m" (AX)
             : "m" (CX));
    if (ZF)
        printk("SETZ: Zero -> %#x - 1 = %#x\n", CX, AX & 0xFF);
    else
        printk("SETZ: No-Zero -> %#x - 1 = %#x\n", CX, AX & 0xFF);

#endif

    return 0;
}

/*
 * SF (bit 7) -- Sign flag
 *
 *   Parity flag -- Set if the least-significant byte of the result
 *   contains an even number of 1 bits.
 */
static __unused int eflags_SF(void)
{
    unsigned char  __unused AL;
    unsigned short __unused AX;
    unsigned short __unused BX;
    unsigned short __unused CX;
    unsigned short __unused DX;
    unsigned short __unused SF;
    unsigned int   __unused EAX;
    unsigned int   __unused EBX;
    unsigned int   __unused ECX;
    unsigned int   __unused EDX;

#ifdef CONFIG_DEBUG_SF_AAD
    /*
     * AAD -- ASCII Adjust AX Before Division
     *
     * Adjusts two unpacked BCD digits (the least-significant digit in the AL
     * register and the most-significant digit in the AH register) so that a
     * division operation performed on the result will yield a correct
     * unoacked BCD value. The AAD instruction is only useful when it precedes
     * a DIV instruction that divides (binary division) the adjusted value
     * in the AX register by an unpacked BCD value.
     *
     * The AAD instruction sets the value in the AL register to (AL + (10 * AH))
     * and then clears the AH register to 00H. The value in the AX register
     * is then equal to the binary equivalent of the original unpacked two-
     * digit (base 10) number in register AH and AL.
     *
     * The generalized version of this instruction allows adjustment of two
     * unpacked digits of any number base, by setting the imm8 byte to the 
     * selected number base (for example, 08H for octal, 0AH for decimal, or
     * 0CH for base 12 numbers). The AAD mnemonic is interpreted by all 
     * assemblyers to means adjust ASCII (base 10) values. To adjust values in
     * another number base, the instruction must be hand coded in machine
     * code (D5 imm8)
     *
     * IF AAD instruction
     *     THEN
     *         tempAL <---- AL;
     *         tmepAH <---- AH;
     *         AL <---- (tmpAL + (tmpAH * imm8)) AND 0xFFH;
     *         (* imm8 is set to 0AH for the AAD mnemonic *)
     *         AH <---- 0
     * FI
     */
    CX = 0x0909;
    DX = 0x1;
    __asm__ ("mov %2, %%ax\n\r"
             "mov %3, %%bx\n\r"
             "mov $0, %%dx\n\r"
             "aad\n\r"
             "sets %%dl\n\r"
             "div %%bl\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (SF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (SF & 0xFF)
        printk("AAD(EFLAGS:SF = 1) -> %#x / %#x = %#x\n", CX, DX, AX);
    else
        printk("AAD(EFLAGS:SF = 0) -> %#x / %#x = %#x\n", CX, DX, AX);

#endif

#ifdef CONFIG_DEBUG_SF_AAM
    /*
     * AAM -- ASCII adjust AX after multiply
     *
     * Adjust the result of the multiplication of two unpacked BCD values to
     * create a pair of unpacked (base 10) BCD values. The AX register is 
     * the implied source and destination operand for this instruction. The
     * The AAM instruction is only useful when it follows an MUL instruction
     * that multiplies (binary multiplication) two unpacked BCD values and 
     * stores a word result in the AX register. The AAM instruction then 
     * adjusts contents of the AX register to contain the correct 2-digit 
     * unpacked (base 10) BCD result.
     *
     * The general version of this instruction allows adjustment of the 
     * contents of the AX to create two unpacked digits of any number base.
     * Here, the imm8 byte is set to the selected number base (for example,
     * 0x08H for octal, 0AH for decimal, or 0CH for base 12 numbers). The 
     * AAM mnemonic is interpreted by all assemblers to mean adjust to ASCII
     * (base 10) values. To adjust to values in another number base, the 
     * instruction must be hand coded inb machine code (D4 imm8)
     *
     * IF instruction AAM
     *     THEN
     *         temp <---- AL;
     *         AH <---- tempAL / imm8; 
     *         (* imm8 is set to 0AH for the AAM mnemonic *)
     *         AL <---- temp MODE imm8
     * FI
     */
    CX = 0x9;
    DX = 0x9;
    __asm__ ("mov %3, %%al\n\r"
             "mov %4, %%bl\n\r"
             "mov $0, %%dx\n\r"
             "mul %%bl\n\r"
             "mov %%ax, %0\n\r"
             "aam\n\r"
             "sets %%dl\n\r"
             "mov %%ax, %1\n\r"
             "mov %%dx, %2"
             : "=m" (AX), "=m" (BX), "=m" (SF)
             : "m" (CX), "m" (DX));
    if (SF)
        printk("AAM(EFLAGS: SF = 1) --> %#x * %#x = %#x(Unpacked-BCD: %#x)\n",
                        CX, DX, AX, BX);
    else
        printk("AAM(EFLAGS: SF = 0) --> %#x * %#x = %#x(Unpacked-BCD: %#x)\n",
                        CX, DX, AX, BX);
#endif

#ifdef CONFIG_DEBUG_SF_ADC
    /*
     * ADC -- Add with Carry
     *
     * Adds the destination operand (first operand), the source operand (
     * second operand), and the carry (CF) flag and stores the result in the
     * destination operand. The destination operand can be a register or a
     * memory location; the source operand can be an immediate, a register,
     * or a memory location. (However, two memory operand cannot be used in
     * one instruction.) The state of the CF flag represents a carry from
     * a previous addition. When an immediate value is used as an operand,
     * it is sign-extended to the length of the destination operand format.
     *
     * The ADC instruction does not distinguish between signed or unsigned
     * operands. Instead, the processor evaluates the result for both data
     * types and sets the OF and CF flags to indicate a carry in the signed
     * unsigned result, respectively. The SF flags indicates the sign of the
     * signed result.
     *
     * DEST <---- DEST + SRC + CF
     *
     */
     CX = -6;
     DX = 1;
     __asm__ ("stc\n\r"
              "mov %2, %%al\n\r"
              "mov %3, %%bl\n\r"
              "mov $0, %%dx\n\r"
              "adc %%bl, %%al\n\r"
              "sets %%dl\n\r"
              "mov %%al, %0\n\r"
              "mov %%dx, %1"
              : "=m" (AX), "=m" (SF)
              : "m" (CX), "m" (DX));
    if (SF)
        printk("ADC(EFLAGS: SF = 1) --> %#x + %#x + 1 = %#x\n", CX, DX, AX);
    else
        printk("ADC(EFLAGS: SF = 0) --> %#x + %#x + 1 = %#x\n", CX, DX, AX); 
#endif

#ifdef CONFIG_DEBUG_SF_ADD
    /*
     * ADD -- Addition with two operands.
     *
     * Adds the destination operand (first operand) and the source operand (
     * second operand) and then stores the result in the destination operand.
     * The destination operand can be a register or a memory location; the 
     * source operand can be an immediate, a register, or a memory location.
     * (However, two memory operands cannot be used in one instruction.) When
     * an immediate value is used as an operand, it it sign-extended to the
     * length of the destination operand format.
     * 
     * The ADD instruction performs integer addition. It evaluates the result
     * for both signed and unsigned integer operands and sets the CF and
     * OF flags to indicate a carry (overflow) in the signed or unsigned 
     * result, respectively. The SF flag indicates the sign of the signed 
     * result.
     *
     * DEST <---- DEST + SRC
     *
     * The OF, SF, ZF, AF, CF, and PF flags are set according to the result.
     */
    CX = -6;
    DX = 1;
    __asm__ ("mov %2, %%al\n\r"
             "mov %3, %%bl\n\r"
             "mov $0, %%dx\n\r"
             "add %%bl, %%al\n\r"
             "sets %%dl\n\r"
             "mov %%dx, %0\n\r"
             "mov %%al, %1"
             : "=m" (SF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (SF)
        printk("ADD(EFLAGS: SF = 1) --> %#x + %#x = %#x\n", CX, DX, AX);
    else
        printk("ADD(EFLAGS: SF = 0) --> %#x + %#x = %#x\n", CX, DX, AX);
#endif

#ifdef CONFIG_DEBUG_SF_AND
    /*
     * AND -- Logical AND
     *
     * Performs a bitwise AND operation on the destination (first) and source
     * (second) operands and stores the result in the destination operand
     * location. The source operand can be an immediate, a register, or a 
     * memory location; the destination operand can be a register or a memory
     * location. (However, two memory operands cannot be used in one 
     * instruction.) Each bit of the result is set to 1 if both corresponding
     * bits of the first and second operands are 1; otherwise, it is set to 0.
     *
     * DEST <---- DEST AND SRC
     */
    CX = 0x81;
    DX = 0x80;
    __asm__ ("mov %2, %%ax\n\r"
             "mov %3, %%bx\n\r"
             "mov $0, %%dl\n\r"
             "and %%bl, %%al\n\r"
             "sets %%dl\n\r"
             "mov %%dl, %0\n\r"
             "mov %%ax, %1"
             : "=m" (SF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (SF)
        printk("AND(EFLAGS: SF = 1) --> %#x AND %#x = %#x\n", CX, DX, AX);
    else
        printk("AND(EFLAGS: SF = 0) --> %#x AND %#x = %#x\n", CX, DX, AX);
#endif

#ifdef CONFIG_DEBUG_SF_CMP
    /*
     * CMP -- Compare two operands
     *
     * Compares the first source operand with the second source operand and
     * the status flags in the EFLAGS register according to the result. The
     * comparison is performed by subtracting the second operand from the
     * first operand and then setting the status flags in the same manner
     * as the SUB instruction. When an immediate value is used as an operand,
     * it is sign-extended to the length of the first operand.
     *
     * temp <---- SRC1 - SignExtend(SRC2);
     * ModifyStatusFlags; 
     * (* Modify status flags in the same manner as the SUB instruction *)
     */
    CX = 0x81;
    DX = 0x1;
    __asm__ ("mov %2, %%ax\n\r"
             "mov %3, %%bx\n\r"
             "mov $0, %%dx\n\r"
             "cmp %%bl, %%al\n\r"
             "sets %%dl\n\r"
             "mov %%dx, %0\n\r"
             "mov %%ax, %1"
             : "=m" (SF), "=m" (AX)
             : "m" (CX), "m" (DX));
    if (SF)
        printk("CMP(EFLAGS: SF = 1) --> %#x - %#x = %#x\n", CX, DX, AX);
    else
        printk("CMP(EFLAGS: SF = 0) --> %#x - %#x = %#x\n", CX, DX, AX);
#endif

#ifdef CONFIG_DEBUG_SF_CMPSB
    /*
     * CMPSB -- Compare string operands in byte
     *
     * Compares the byte with the first source operand with the byte specified
     * with the second source operand and sets the status flags in EFLAGS 
     * register according to the result.
     *
     * Both source operands are located in memory. The address of the first
     * source operand is read from DS:SI (depending on the address-size 
     * attribute of the instruction is 16). The address of the second source
     * operand is read from ES:DI (again depending on the address-size 
     * attribute of the instruction is 16). The DS segment may be overriden
     * with a segment override prefix, but the ES segment cannot be overridden.
     *
     * At the assembly-code level, two forms of this instruction are allowed:
     * the "explicit-operands" from and the "no-operands" from. The explicit-
     * operands form (specified with the CMPS mnemonic) allows the two source
     * operands to the specified explicitly. Here, the source operands should
     * be symbols that indicate the size and location of the source values.
     * This explicit-operand form is provided to allow documentation. However,
     * not that the documentation provided by this form can be misleading.
     * That is, the source operand symbols must specify the correct type
     * (size) of the operands (bytes), but they do not have to specify the
     * correct location. Locations of the source operands are always specified
     * by the DS:SI and ES:DI register, which must be loaded correctly before
     * the compare string instruction is executed.
     *
     * The size of the source operands is selected with the mnemonic: CMPSB
     * (byte comparison). After the comparison, the SI and DI registers
     * increment or decrement automatically according to the setting of the
     * DF flag in the EFLAGS register. (If the DF flags is 0, the SI and DI
     * register increment; if the DF flag is 1, the register decrement.) The
     * register increment or decrement by 1 for byte operations.
     *
     * The CMPSB instruction can be preceded by the REP prefix for block
     * comparisons. More often, however, these instructions will be used in 
     * a LOOP construct that takes some action based on the setting of the 
     * status flags before the next comparison is made.
     */
    ECX = (unsigned int)(unsigned long)&"Hello World"[0];
    EDX = (unsigned int)(unsigned long)&"Hello a BiscuitOS"[0];
    CX = 8;
    __asm__ ("movl %2, %%esi\n\r"
             "movl %3, %%edi\n\r"
             "mov %4, %%cx\n\r"
             "mov $0, %%dx\n\r"
             "cld\n\r"
             "repz cmpsb\n\r"
             "sets %%dl\n\r"
             "mov %%dl, %0\n\r"
             "mov %%cx, %1"
             : "=m" (SF), "=m" (BX)
             : "m" (ECX), "m" (EDX), "m" (CX));
    if (SF)
        printk("CMPSB(EFLAGS: SF = 1) -->\n");
    else
        printk("CMPSB(EFLAGS: SF = 0) -->\n");
    printk("\"%s\" Diff \"%s\" in %#x bytes\n", 
                     (char *)(unsigned long)ECX,
                     (char *)(unsigned long)EDX, CX - BX - 1);
#endif

#ifdef CONFIG_DEBUG_SF_CMPSW
    /*
     * CMPSW -- Compare string operands in word
     *
     * Compares the byte with the first source operand with the byte specified
     * with the second source operand and sets the status flags in EFLAGS 
     * register according to the result.
     *
     * Both source operands are located in memory. The address of the first
     * source operand is read from DS:SI (depending on the address-size 
     * attribute of the instruction is 16). The address of the second source
     * operand is read from ES:DI (again depending on the address-size 
     * attribute of the instruction is 16). The DS segment may be overriden
     * with a segment override prefix, but the ES segment cannot be overridden.
     *
     * At the assembly-code level, two forms of this instruction are allowed:
     * the "explicit-operands" from and the "no-operands" from. The explicit-
     * operands form (specified with the CMPS mnemonic) allows the two source
     * operands to the specified explicitly. Here, the source operands should
     * be symbols that indicate the size and location of the source values.
     * This explicit-operand form is provided to allow documentation. However,
     * not that the documentation provided by this form can be misleading.
     * That is, the source operand symbols must specify the correct type
     * (size) of the operands (bytes), but they do not have to specify the
     * correct location. Locations of the source operands are always specified
     * by the DS:SI and ES:DI register, which must be loaded correctly before
     * the compare string instruction is executed.
     *
     * The size of the source operands is selected with the mnemonic: CMPSW
     * (word comparison). After the comparison, the SI and DI registers
     * increment or decrement automatically according to the setting of the
     * DF flag in the EFLAGS register. (If the DF flags is 0, the SI and DI
     * register increment; if the DF flag is 1, the register decrement.) The
     * register increment or decrement by 1 for byte operations.
     *
     * The CMPSW instruction can be preceded by the REP prefix for block
     * comparisons. More often, however, these instructions will be used in 
     * a LOOP construct that takes some action based on the setting of the 
     * status flags before the next comparison is made.
     */
    ECX = (unsigned int)(unsigned long)&"Hello  World"[0];
    EDX = (unsigned int)(unsigned long)&"Hello  a BiscuitOS"[0];
    CX = 15;
    __asm__ ("movl %2, %%esi\n\r"
             "movl %3, %%edi\n\r"
             "mov %4, %%cx\n\r"
             "mov $0, %%dx\n\r"
             "cld\n\r"
             "repz cmpsw\n\r"
             "sets %%dl\n\r"
             "mov %%dx, %0\n\r"
             "mov %%cx, %1"
             : "=m" (SF), "=m" (BX)
             : "m" (ECX), "m" (EDX), "m" (CX));
    if (SF)
        printk("CMPSW(EFLAGS: SF = 1) -->\n");
    else
        printk("CMPSW(EFLAGS: SF = 0) -->\n");
    printk("\"%s\" DIFF \"%s\" in %d word\n", (char *)(unsigned long)ECX, 
                     (char *)(unsigned long)EDX, CX - BX - 1);
#endif

#ifdef CONFIG_DEBUG_SF_CMPSD
    /*
     * CMPSD -- Compare string operands in Double-word
     *
     * Compares the byte with the irst source operand with the byte specified
     * with the second source operand and sets the status flags in EFLAGS 
     * register according to the result.
     *
     * Both source operands are located in memory. The address of the first
     * source operand is read from EDS:ESI (depending on the address-size 
     * attribute of the instruction is 32). The address of the second source
     * operand is read from ES:EDI (again depending on the address-size 
     * attribute of the instruction is 32). The DS segment may be overriden
     * with a segment override prefix, but the ES segment cannot be overridden.
     *
     * At the assembly-code level, two forms of this instruction are allowed:
     * the "explicit-operands" from and the "no-operands" from. The explicit-
     * operands form (specified with the CMPS mnemonic) allows the two source
     * operands to the specified explicitly. Here, the source operands should
     * be symbols that indicate the size and location of the source values.
     * This explicit-operand form is provided to allow documentation. However,
     * not that the documentation provided by this form can be misleading.
     * That is, the source operand symbols must specify the correct type
     * (size) of the operands (bytes), but they do not have to specify the
     * correct location. Locations of the source operands are always specified
     * by the DS:ESI and ES:EDI register, which must be loaded correctly before
     * the compare string instruction is executed.
     *
     * The size of the source operands is selected with the mnemonic: CMPSD
     * (double-word comparison). After the comparison, the ESI and EDI registers
     * increment or decrement automatically according to the setting of the
     * DF flag in the EFLAGS register. (If the DF flags is 0, the SI and DI
     * register increment; if the DF flag is 1, the register decrement.) The
     * register increment or decrement by 1 for byte operations.
     *
     * The CMPSD instruction can be preceded by the REP prefix for block
     * comparisons. More often, however, these instructions will be used in 
     * a LOOP construct that takes some action based on the setting of the 
     * status flags before the next comparison is made.
     */
    ECX = (unsigned int)(unsigned long)&"HellW World"[0];
    EDX = (unsigned int)(unsigned long)&"Hella World"[0];
    CX = 15;
    __asm__ ("movl %2, %%esi\n\r"
             "movl %3, %%edi\n\r"
             "mov %4, %%cx\n\r"
             "mov $0, %%dx\n\r"
             "cld\n\r"
             "repz cmpsd\n\r"
             "sets %%dl\n\r"
             "mov %%dx, %0\n\r"
             "mov %%cx, %1"
             : "=m" (SF), "=m" (BX)
             : "m" (ECX), "m" (EDX), "m" (CX));
    if (SF)
        printk("CMPSD(EFLAGS: SF = 1) -->\n");
    else
        printk("CMPSD(EFLAGS: SF = 0) -->\n");
    printk("\"%s\" DIFF \"%s\" in %d Double-word\n", 
         (char *)(unsigned long)ECX, (char *)(unsigned long)EDX, CX - BX - 1);
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

#ifdef CONFIG_DEBUG_EFLAGS_PF
    eflags_PF();
#endif

#ifdef CONFIG_DEBUG_EFLAGS_ZF
    eflags_ZF();
#endif

#ifdef CONFIG_DEBUG_EFLAGS_SF
    eflags_SF();
#endif

    return 0;
}
late_debugcall(debug_eflags);
