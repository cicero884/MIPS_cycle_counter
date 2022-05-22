#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * author:509
 * this program calculate mips instruction finish cycle.
 * only pass lw,sw,beq and R-type instruction
 * -fw=(EX/MEM/ALL) :enable forwarding wire from stage ? to ID
 * -bd=(ID/EX/MEM) :branch decision is made in (ID/EX/MEM) stage(default:EX)
 */

#define MAX(a, b)               \
    ({                          \
        __typeof__(a) _a = (a); \
        __typeof__(b) _b = (b); \
        _a > _b ? _a : _b;      \
    })


enum stage { ID = 0x01, EX = 0x02, MEM = 0x04 } __attribute__((__packed__));
;
enum stage bd = EX;
/**
 * If beq related command occured,
 * and if predict faild(judge by comment behined),
 * will need to wait specific cycle by bd=?
 */
_Bool predict = 1;
unsigned char fw = 0;


struct instruction {
    enum stage out_stage;
    char w_reg[4];
};

/**
 * Check and count how many cycle to wait if the previous write instruction
 * write on current instruction require read return cycle to wait(min:1)
 */
unsigned check_read(const char reg[], struct instruction inst[])
{
    if (!strcmp(reg, "$0"))
        return 1;

    unsigned max = 1;
    for (unsigned char i = 0; i < 3; ++i) {
        // read match previous write
        if (!strcmp(reg, inst[i].w_reg)) {
            // no forward
            if (inst[i].out_stage && !((unsigned char) inst[i].out_stage & fw))
                max = MAX(max, 1u + __builtin_ctz(inst[i].out_stage) - i);
        }
    }
    return max;
}

/**
 * detect which instruction and calculate how many cycle required
 * return cycle to wait
 */
unsigned calc_next_cycle(const char in[], struct instruction inst[])
{
    char command[4];
    char r_reg[2][4];
    char tmp[10];
    struct instruction cur_inst;
    unsigned require_cycle;

    sscanf(in, "%s", command);
    // lw
    if (!strcmp(command, "lw")) {
        cur_inst.out_stage = MEM;
        sscanf(in + 3, "%s,%s(%s)", cur_inst.w_reg, tmp, r_reg[0]);
        require_cycle = check_read(r_reg[0], inst);
    }
    // sw
    else if (!strcmp(command, "sw")) {
        cur_inst = (struct instruction){0, "$0"};
        sscanf(in + 3, "%s,%s(%s)", r_reg[0], tmp, r_reg[1]);
        require_cycle =
            MAX(check_read(r_reg[0], inst), check_read(r_reg[1], inst));
    }
    // beq
    else if (!strcmp(command, "beq")) {
        char predict_str[10];
        cur_inst = (struct instruction){0, "$0"};
        sscanf(in + 4, "%s,%s,%s;%s", r_reg[0], r_reg[1], tmp, predict_str);
        predict = (strcmp("success", predict_str));
        require_cycle =
            MAX(check_read(r_reg[0], inst), check_read(r_reg[1], inst));
    }
    // R-type
    else {
        char op[5];
        sscanf(in, "%s %s,%s,%s", op, cur_inst.w_reg, r_reg[0], r_reg[1]);
        require_cycle =
            MAX(check_read(r_reg[0], inst), check_read(r_reg[1], inst));
    }

    // move instruction list to old
    inst[2] = cur_inst;
    for (int i = 0; i < 2; ++i)
        inst[i] = inst[i + 1];

    return (predict) ? require_cycle
                     : MAX(require_cycle, 1u + __builtin_ctz(bd));
}

int main(int argc, char *argv[])
{
    // init parameter
    char *tmp;
    for (int i = 0; i < argc; ++i) {
        if (!strcmp(argv[i], "-fw=")) {
            tmp = argv[i] + 3;
            if (!strcmp(tmp, "EX"))
                fw = (unsigned char) EX;
            else if (!strcmp(tmp, "MEM"))
                fw = (unsigned char) MEM;
            else if (!strcmp(tmp, "ALL"))
                fw = (unsigned char) EX & (unsigned char) MEM;
        } else if (!strncmp(argv[i], "-bd=", 4)) {
            tmp = argv[i] + 3;
            if (!strcmp(tmp, "ID"))
                bd = ID;
            else if (!strcmp(tmp, "EX"))
                bd = EX;
            else if (!strcmp(tmp, "MEM"))
                bd = MEM;
        }
    }
    struct instruction inst[3];
    // init inst to nop
    for (int i = 1; i < 3; ++i)
        inst[i] = (struct instruction){0, "$0"};

    // start input & calculate
    char command[4], in[30];
    unsigned cycle = 0;
    while (fgets(in, sizeof(in), stdin) != NULL) {
        // calc cycle
        cycle += calc_next_cycle(in, inst);
        printf("%u\n", cycle);
    }
    return 0;
}
