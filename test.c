#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <libelf.h>
#include <fcntl.h>
#include <gelf.h>
#include <bfd.h>

// andi
#define INST_ANDI 0x7013
#define INST_ANDI_MASK 0x707f

// addi
#define INST_ADDI 0x13
#define INST_ADDI_MASK 0x707f

// slti
#define INST_SLTI 0x2013
#define INST_SLTI_MASK 0x707f

// sltiu
#define INST_SLTIU 0x3013
#define INST_SLTIU_MASK 0x707f

// ori
#define INST_ORI 0x6013
#define INST_ORI_MASK 0x707f

// xori
#define INST_XORI 0x4013
#define INST_XORI_MASK 0x707f

// slli
#define INST_SLLI 0x1013
#define INST_SLLI_MASK 0xfc00707f

// srli
#define INST_SRLI 0x5013
#define INST_SRLI_MASK 0xfc00707f

// srai
#define INST_SRAI 0x40005013
#define INST_SRAI_MASK 0xfc00707f

// lui
#define INST_LUI 0x37
#define INST_LUI_MASK 0x7f

// auipc
#define INST_AUIPC 0x17
#define INST_AUIPC_MASK 0x7f

// add
#define INST_ADD 0x33
#define INST_ADD_MASK 0xfe00707f

// sub
#define INST_SUB 0x40000033
#define INST_SUB_MASK 0xfe00707f

// slt
#define INST_SLT 0x2033
#define INST_SLT_MASK 0xfe00707f

// sltu
#define INST_SLTU 0x3033
#define INST_SLTU_MASK 0xfe00707f

// xor
#define INST_XOR 0x4033
#define INST_XOR_MASK 0xfe00707f

// or
#define INST_OR 0x6033
#define INST_OR_MASK 0xfe00707f

// and
#define INST_AND 0x7033
#define INST_AND_MASK 0xfe00707f

// sll
#define INST_SLL 0x1033
#define INST_SLL_MASK 0xfe00707f

// srl
#define INST_SRL 0x5033
#define INST_SRL_MASK 0xfe00707f

// sra
#define INST_SRA 0x40005033
#define INST_SRA_MASK 0xfe00707f

// jal
#define INST_JAL 0x6f
#define INST_JAL_MASK 0x7f

// jalr
#define INST_JALR 0x67
#define INST_JALR_MASK 0x707f

// beq
#define INST_BEQ 0x63
#define INST_BEQ_MASK 0x707f

// bne
#define INST_BNE 0x1063
#define INST_BNE_MASK 0x707f

// blt
#define INST_BLT 0x4063
#define INST_BLT_MASK 0x707f

// bge
#define INST_BGE 0x5063
#define INST_BGE_MASK 0x707f

// bltu
#define INST_BLTU 0x6063
#define INST_BLTU_MASK 0x707f

// bgeu
#define INST_BGEU 0x7063
#define INST_BGEU_MASK 0x707f

// lb
#define INST_LB 0x3
#define INST_LB_MASK 0x707f

// lh
#define INST_LH 0x1003
#define INST_LH_MASK 0x707f

// lw
#define INST_LW 0x2003
#define INST_LW_MASK 0x707f

// lbu
#define INST_LBU 0x4003
#define INST_LBU_MASK 0x707f

// lhu
#define INST_LHU 0x5003
#define INST_LHU_MASK 0x707f

// lwu
#define INST_LWU 0x6003
#define INST_LWU_MASK 0x707f

// sb
#define INST_SB 0x23
#define INST_SB_MASK 0x707f

// sh
#define INST_SH 0x1023
#define INST_SH_MASK 0x707f

// sw
#define INST_SW 0x2023
#define INST_SW_MASK 0x707f

// ecall
#define INST_ECALL 0x73
#define INST_ECALL_MASK 0xffffffff

// ebreak
#define INST_EBREAK 0x100073
#define INST_EBREAK_MASK 0xffffffff

// eret
#define INST_ERET 0x200073
#define INST_ERET_MASK 0xcfffffff

// csrrw
#define INST_CSRRW 0x1073
#define INST_CSRRW_MASK 0x707f

// csrrs
#define INST_CSRRS 0x2073
#define INST_CSRRS_MASK 0x707f

// csrrc
#define INST_CSRRC 0x3073
#define INST_CSRRC_MASK 0x707f

// csrrwi
#define INST_CSRRWI 0x5073
#define INST_CSRRWI_MASK 0x707f

// csrrsi
#define INST_CSRRSI 0x6073
#define INST_CSRRSI_MASK 0x707f

// csrrci
#define INST_CSRRCI 0x7073
#define INST_CSRRCI_MASK 0x707f

// mul
#define INST_MUL 0x2000033
#define INST_MUL_MASK 0xfe00707f

// mulh
#define INST_MULH 0x2001033
#define INST_MULH_MASK 0xfe00707f

// mulhsu
#define INST_MULHSU 0x2002033
#define INST_MULHSU_MASK 0xfe00707f

// mulhu
#define INST_MULHU 0x2003033
#define INST_MULHU_MASK 0xfe00707f

// div
#define INST_DIV 0x2004033
#define INST_DIV_MASK 0xfe00707f

// divu
#define INST_DIVU 0x2005033
#define INST_DIVU_MASK 0xfe00707f

// rem
#define INST_REM 0x2006033
#define INST_REM_MASK 0xfe00707f

// remu
#define INST_REMU 0x2007033
#define INST_REMU_MASK 0xfe00707f

// wfi
#define INST_WFI 0x10500073
#define INST_WFI_MASK 0xffff8fff

// fence
#define INST_FENCE 0xf
#define INST_FENCE_MASK 0x707f

// sfence
#define INST_SFENCE 0x12000073
#define INST_SFENCE_MASK 0xfe007fff

// fence.i
#define INST_IFENCE 0x100f
#define INST_IFENCE_MASK 0x707f


#define SHIFT_MASK(x, s, n) (((x) >> (s)) & ((1<<(n)) - 1))
#define SHIFT(x, s) ((x) >> (s))


//-----------------------------------------------------------------
// elf_load, copy from riscv-isa-sim
//-----------------------------------------------------------------
int elf_load(const char *filename, uint32_t *start_addr)
{
    int fd;
    Elf * e;
    Elf_Kind ek;
    Elf_Scn *scn;
    Elf_Data *data;
    Elf32_Shdr *shdr;
    size_t shstrndx;

    if (elf_version ( EV_CURRENT ) == EV_NONE)
        return 0;

    if ((fd = open ( filename , O_RDONLY , 0)) < 0)
        return 0;

    if ((e = elf_begin ( fd , ELF_C_READ, NULL )) == NULL)
        return 0;
    
    ek = elf_kind ( e );
    if (ek != ELF_K_ELF)
        return 0;

    // Get section name header index
    if (elf_getshdrstrndx(e, &shstrndx)!=0)
        return 0;

    int section_idx = 0;
    while ((scn = elf_getscn(e, section_idx)) != NULL)
    {
        shdr = elf32_getshdr(scn);

        // Section which need allocating
        if ((shdr->sh_flags & SHF_ALLOC) && (shdr->sh_size > 0))
        {
            data = elf_getdata(scn, NULL);

            printf("Memory: 0x%x - 0x%x (Size=%dKB) [%s]\n", shdr->sh_addr, shdr->sh_addr + shdr->sh_size - 1, shdr->sh_size / 1024, elf_strptr(e, shstrndx, shdr->sh_name));


            memcpy(shdr->sh_addr + start_addr, data->d_buf, shdr->sh_size);
        }

        section_idx++;
    }    

    elf_end ( e );
    close ( fd );
    
    return 1;
}

int main(int argc, char *argv[]){
    unsigned _cycles = 0;
    int max_cycles = -1;
    char *filename = NULL;
    int help = 0;
    int trace = 0;
    uint32_t trace_mask = 1;
    uint32_t stop_pc = 0xFFFFFFFF;
    uint32_t trace_pc = 0xFFFFFFFF;
    uint32_t mem_base = 0x00000000;
    uint32_t mem_size = (32 * 1024 * 1024);
    char *   dump_file      = NULL;
    char *   dump_sym_start = NULL;
    char *   dump_sym_end   = NULL;
    int c;

    while ((c = getopt (argc, argv, "t:v:f:c:r:d:b:s:e:p:j:k:")) != -1)
    {
        switch(c)
        {
            case 't':
                trace = strtoul(optarg, NULL, 0);
                break;
            case 'v':
                trace_mask = strtoul(optarg, NULL, 0);
                break;
            case 'r':
                stop_pc = strtoul(optarg, NULL, 0);
                break;
            case 'f':
                filename = optarg;
                break;
            case 'c':
                max_cycles = (int)strtoul(optarg, NULL, 0);
                break;
            case 'b':
                mem_base = strtoul(optarg, NULL, 0);
                break;
            case 's':
                mem_size = strtoul(optarg, NULL, 0);
                break;
            case 'e':
                trace_pc = strtoul(optarg, NULL, 0);
                break;
            case 'p':
                dump_file = optarg;
                break;
            case 'j':
                dump_sym_start = optarg;
                break;
            case 'k':
                dump_sym_end = optarg;
                break;
            case '?':
            default:
                help = 1;   
                break;
        }
    }

    if (help || filename == NULL)
    {
        fprintf (stderr,"Usage:\n");
        fprintf (stderr,"-f filename.elf = Executable to load (ELF)\n");
        fprintf (stderr,"-t [0/1]        = Enable program trace\n");
        fprintf (stderr,"-v 0xX          = Trace Mask\n");
        fprintf (stderr,"-c nnnn         = Max instructions to execute\n");
        fprintf (stderr,"-r 0xnnnn       = Stop at PC address\n");
        fprintf (stderr,"-e 0xnnnn       = Trace from PC address\n");
        fprintf (stderr,"-b 0xnnnn       = Memory base address (for binary loads)\n");
        fprintf (stderr,"-s nnnn         = Memory size (for binary loads)\n");
        fprintf (stderr,"-p dumpfile.bin = Post simulation memory dump file\n");
        fprintf (stderr,"-j sym_name     = Symbol for memory dump start\n");
        fprintf (stderr,"-k sym_name     = Symbol for memory dump end\n");
        exit(-1);
    }

    void * mem_start_addr = 0;
    void * mem_end_addr = 0;

    printf("MEM: Create memory 0x%08x-%08x\n", mem_base, mem_base + mem_size-1);
    mem_start_addr = malloc(mem_size-1);

    printf("MEM: mem_start_addr %p\n", mem_start_addr);
    mem_end_addr = mem_start_addr + mem_size-5;

    elf_load(filename, mem_start_addr);

    void * pc_p = 0;
    int32_t pc = 0;
    uint32_t instr = 0;

    //decode
    uint32_t  rd = 0;
    uint32_t  rs1= 0;
    uint32_t  rs2= 0;

    uint32_t u_i11_imm = 0;
    int32_t  i11_imm  = 0;
    int32_t  s11_imm  = 0;
    int32_t  b12_imm  = 0;
    int32_t  u31_imm  = 0;
    int32_t  j20_imm  = 0;

    //reg
    uint32_t regs[32];
    for(int i = 0; i < 32; i++)
        regs[i] = 0;

    while (_cycles < max_cycles) {
        pc_p = pc + mem_start_addr;
        if (pc_p > mem_end_addr) break;

        instr = * (uint32_t *)pc_p;
        rd    = SHIFT_MASK(instr, 7, 5);
        rs1   = SHIFT_MASK(instr, 15, 5);
        rs2   = SHIFT_MASK(instr, 20, 5);

        u_i11_imm = SHIFT_MASK(instr, 20, 12);
        i11_imm   = SHIFT((int32_t)instr, 20);
        s11_imm   = SHIFT((int32_t)instr, 20) & 0xffffffe0 | SHIFT_MASK(instr, 7, 5);
        b12_imm   = SHIFT((int32_t)instr, 19) & 0xfffff000 | (instr >> 7 << 31 >> 20) | (instr << 20 >> 28 << 1);
        u31_imm   = (int32_t)instr & 0xfffff000;
        j20_imm   = SHIFT((int32_t)instr, 11) & 0xfff00000 | (instr & 0x000ff000) | ((instr & 0x00100000) >> 9) | (instr << 1 >> 22 << 1);

        if ((instr & INST_ADDI_MASK) == INST_ADDI) {
            printf("PC:%x CODE:0x%x, ADDI x%u, x%u,(%d)\n", pc, instr, rd, rs1, i11_imm);
            regs[rd] = (uint32_t)((int32_t)regs[rs1] + i11_imm);
        }
        else if ((instr & INST_ADD_MASK) == INST_ADD) {
            printf("PC:%x CODE:0x%x, ADD x%u, x%u, x%u\n", pc, instr, rd, rs1, rs2);
            regs[rd] = (uint32_t)((int32_t)regs[rs1] + (int32_t)regs[rs2]);
        }
        else if ((instr & INST_SUB_MASK) == INST_SUB) {
            printf("PC:%x CODE:0x%x, SUB x%u, x%u, x%u\n", pc, instr, rd, rs1, rs2);
            regs[rd] = (uint32_t)((int32_t)regs[rs1] - (int32_t)regs[rs2]);
        }
        else if ((instr & INST_LUI_MASK) == INST_LUI) {
            printf("PC:%x CODE:0x%x, LUI x%u, (%d)\n", pc, instr, rd, u31_imm);
            regs[rd] = (uint32_t)u31_imm;
        }
        else if ((instr & INST_AUIPC_MASK) == INST_AUIPC) {
            printf("PC:%x CODE:0x%x, AUIPC x%u, (%d)\n", pc, instr, rd, u31_imm);
            regs[rd] = (uint32_t)(u31_imm + pc);
        }
        else if ((instr & INST_SLL_MASK) == INST_SLL) {
            printf("PC:%x CODE:0x%x, SLL x%u, x%u, x%u\n", pc, instr, rd, rs1, rs2);
            regs[rd] = (uint32_t)((int32_t)regs[rs1] << (int32_t)regs[rs2]);
        }
        else if ((instr & INST_SLLI_MASK) == INST_SLLI) {
            printf("PC:%x CODE:0x%x, SLLI x%u, x%u,(%d)\n", pc, instr, rd, rs1, i11_imm);
            regs[rd] = (uint32_t)((int32_t)regs[rs1] << i11_imm);
        }
        else if ((instr & INST_SRL_MASK) == INST_SRL) {
            printf("PC:%x CODE:0x%x, SRL x%u, x%u, x%u\n", pc, instr, rd, rs1, rs2);
            regs[rd] = (uint32_t)(regs[rs1] >> regs[rs2]);
        }
        else if ((instr & INST_SRLI_MASK) == INST_SRLI) {
            printf("PC:%x CODE:0x%x, SRLI x%u, x%u,(%d)\n", pc, instr, rd, rs1, i11_imm);
            regs[rd] = (uint32_t)(regs[rs1] >> i11_imm);
        }
        else if ((instr & INST_SRA_MASK) == INST_SRA) {
            printf("PC:%x CODE:0x%x, SRA x%u, x%u, x%u\n", pc, instr, rd, rs1, rs2);
            regs[rd] = (uint32_t)((int32_t)regs[rs1] >> regs[rs2]);
        }
        else if ((instr & INST_SRAI_MASK) == INST_SRAI) {
            printf("PC:%x CODE:0x%x, SRAI x%u, x%u,(%d)\n", pc, instr, rd, rs1, i11_imm);
            regs[rd] = (uint32_t)((int32_t)regs[rs1] >> i11_imm);
        }
        else if ((instr & INST_XOR_MASK) == INST_XOR) {
            printf("PC:%x CODE:0x%x, XOR x%u, x%u, x%u\n", pc, instr, rd, rs1, rs2);
            regs[rd] = (uint32_t)(regs[rs1] ^ regs[rs2]);
        }
        else if ((instr & INST_XORI_MASK) == INST_XORI) {
            printf("PC:%x CODE:0x%x, XORI x%u, x%u,(%d)\n", pc, instr, rd, rs1, i11_imm);
            regs[rd] = (uint32_t)(regs[rs1] ^ i11_imm);
        }
        else if ((instr & INST_OR_MASK) == INST_OR) {
            printf("PC:%x CODE:0x%x, OR x%u, x%u, x%u\n", pc, instr, rd, rs1, rs2);
            regs[rd] = (uint32_t)(regs[rs1] | regs[rs2]);
        }
        else if ((instr & INST_ORI_MASK) == INST_ORI) {
            printf("PC:%x CODE:0x%x, ORI x%u, x%u,(%d)\n", pc, instr, rd, rs1, i11_imm);
            regs[rd] = (uint32_t)(regs[rs1] ^ i11_imm);
        }
        else if ((instr & INST_AND_MASK) == INST_AND) {
            printf("PC:%x CODE:0x%x, AND x%u, x%u, x%u\n", pc, instr, rd, rs1, rs2);
            regs[rd] = (uint32_t)(regs[rs1] & regs[rs2]);
        }
        else if ((instr & INST_ANDI_MASK) == INST_ANDI) {
            printf("PC:%x CODE:0x%x, ANDI x%u, x%u,(%d)\n", pc, instr, rd, rs1, i11_imm);
            regs[rd] = (uint32_t)(regs[rs1] & i11_imm);
        }
        else if ((instr & INST_SLT_MASK) == INST_SLT) {
            printf("PC:%x CODE:0x%x, SLT x%u, x%u, x%u\n", pc, instr, rd, rs1, rs2);
            regs[rd] = (int32_t)regs[rs1] < (int32_t)regs[rs2] ? 1 : 0;
        }
        else if ((instr & INST_SLTI_MASK) == INST_SLTI) {
            printf("PC:%x CODE:0x%x, SLTI x%u, x%u,(%d)\n", pc, instr, rd, rs1, i11_imm);
            regs[rd] = (int32_t)regs[rs1] < i11_imm ? 1 : 0;
        }
        else if ((instr & INST_SLTU_MASK) == INST_SLTU) {
            printf("PC:%x CODE:0x%x, SLTU x%u, x%u, x%u\n", pc, instr, rd, rs1, rs2);
            regs[rd] = regs[rs1] < regs[rs2] ? 1 : 0;
        }
        else if ((instr & INST_SLTIU_MASK) == INST_SLTIU) {
            printf("PC:%x CODE:0x%x, SLTIU x%u, x%u,(%d)\n", pc, instr, rd, rs1, i11_imm);
            regs[rd] = regs[rs1] < i11_imm ? 1 : 0;
        }
        else if ((instr & INST_SW_MASK) == INST_SW) {
            printf("PC:%x CODE:0x%x, SW x%u, x%u, x%u,(%d)\n", pc, instr, rd, rs1, rs2, s11_imm);
            memcpy(mem_start_addr + (int32_t)regs[rs1] + s11_imm, &regs[rs2], 4);
        }
        else if ((instr & INST_SB_MASK) == INST_SB) {
            printf("PC:%x CODE:0x%x, SW x%u, x%u, x%u,(%d)\n", pc, instr, rd, rs1, rs2, s11_imm);
            memcpy(mem_start_addr + (int32_t)regs[rs1] + s11_imm, &regs[rs2], 1);
        }
        else if ((instr & INST_SH_MASK) == INST_SH) {
            printf("PC:%x CODE:0x%x, SW x%u, x%u, x%u,(%d)\n", pc, instr, rd, rs1, rs2, s11_imm);
            memcpy(mem_start_addr + (int32_t)regs[rs1] + s11_imm, &regs[rs2], 2);
        }
        else if ((instr & INST_BEQ_MASK) == INST_BEQ) {
            printf("PC:%x CODE:0x%x, BEQ x%u, x%u,(%d)\n", pc, instr, rs1, rs2, b12_imm);
            if((int32_t)regs[rs1] == (int32_t)regs[rs2]) {
                pc = pc + b12_imm;
                goto next;
            }
        }
        else if ((instr & INST_BNE_MASK) == INST_BNE) {
            printf("PC:%x CODE:0x%x, BNE x%u, x%u,(%d)\n", pc, instr, rs1, rs2, b12_imm);
            if((int32_t)regs[rs1] != (int32_t)regs[rs2]) {
                pc = pc + b12_imm;
                goto next;
            }
        }
        else if ((instr & INST_BLT_MASK) == INST_BLT) {
            printf("PC:%x CODE:0x%x, BLT x%u, x%u,(%d)\n", pc, instr, rs1, rs2, b12_imm);
            if((int32_t)regs[rs1] < (int32_t)regs[rs2]) {
                pc = pc + b12_imm;
                goto next;
            }
        }
        else if ((instr & INST_BGE_MASK) == INST_BGE) {
            printf("PC:%x CODE:0x%x, BGE x%u, x%u,(%d)\n", pc, instr, rs1, rs2, b12_imm);
            if((int32_t)regs[rs1] > (int32_t)regs[rs2]) {
                pc = pc + b12_imm;
                goto next;
            }
        }
        else if ((instr & INST_BLTU_MASK) == INST_BLTU) {
            printf("PC:%x CODE:0x%x, BLTU x%u, x%u,(%d)\n", pc, instr, rs1, rs2, b12_imm);
            if(regs[rs1] < regs[rs2]) {
                pc = pc + b12_imm;
                goto next;
            }
        }
        else if ((instr & INST_BGEU_MASK) == INST_BGEU) {
            printf("PC:%x CODE:0x%x, BGEU x%u, x%u,(%d)\n", pc, instr, rs1, rs2, b12_imm);
            if(regs[rs1] > regs[rs2]) {
                pc = pc + b12_imm;
                goto next;
            }
        }
        else if ((instr & INST_JAL_MASK) == INST_JAL) {
            printf("PC:%x CODE:0x%x, JAL x%u, (%d)\n", pc, instr, rd, j20_imm);
            regs[rd] = pc + 4;
            pc = pc + j20_imm;
            goto next;
        }
        else if ((instr & INST_JALR_MASK) == INST_JALR) {
            printf("PC:%x CODE:0x%x, JALR x%u, x%u, (%d)\n", pc, instr, rd, rs1, i11_imm);
            regs[rd] = pc + 4;
            pc = i11_imm + regs[rs1];
            goto next;
        }
        else if ((instr & INST_LB_MASK) == INST_LB) {
            printf("PC:%x CODE:0x%x, LB x%u, x%u,(%d)\n", pc, instr, rd, rs1, i11_imm);
            memcpy(&regs[rd], mem_start_addr + (int32_t)regs[rs1] + i11_imm, 1);
            regs[rd] = (int32_t)(regs[rd] << 24) >> 24;
        }
        else if ((instr & INST_LH_MASK) == INST_LH) {
            printf("PC:%x CODE:0x%x, LH x%u, x%u,(%d)\n", pc, instr, rd, rs1, i11_imm);
            memcpy(&regs[rd], mem_start_addr + (int32_t)regs[rs1] + i11_imm, 2);
            regs[rd] = (int32_t)(regs[rd] << 12) >> 12;
        }
        else if ((instr & INST_LW_MASK) == INST_LW) {
            printf("PC:%x CODE:0x%x, LH x%u, x%u,(%d)\n", pc, instr, rd, rs1, i11_imm);
            memcpy(&regs[rd], mem_start_addr + (int32_t)regs[rs1] + i11_imm, 4);
        }
        else if ((instr & INST_LBU_MASK) == INST_LBU) {
            printf("PC:%x CODE:0x%x, LBU x%u, x%u,(%d)\n", pc, instr, rd, rs1, i11_imm);
            memcpy(&regs[rd], mem_start_addr + (int32_t)regs[rs1] + i11_imm, 1);
            regs[rd] = regs[rd] << 24 >> 24;
        }
        else if ((instr & INST_LHU_MASK) == INST_LHU) {
            printf("PC:%x CODE:0x%x, LHU x%u, x%u,(%d)\n", pc, instr, rd, rs1, i11_imm);
            memcpy(&regs[rd], mem_start_addr + (int32_t)regs[rs1] + i11_imm, 2);
            regs[rd] = regs[rd] << 12 >> 12;
        }

        pc = pc + 4;
next:
        _cycles++;
    }
    for(int i = 0; i < 32; i++)
       printf("REG[%d]: 0x%x\n", i, regs[i]);

}
