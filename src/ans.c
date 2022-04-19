#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/**
 * author:509
 * this program calculate mips instruction finish cycle.
 * only pass lw,sw,beq and R-type instruction
 * -fw :enable forwarding feature
 * -bd=(ID/EX/MEM) :branch decision is made in (ID/EX/MEM) stage(default:EX)
 */

#define MAX(a,b) 				\
	({ __typeof__ (a) _a = (a); \
	 __typeof__ (b) _b = (b);	\
	 _a > _b ? _a : _b; })

bool fw = 0;
enum branch_decision{ID, EX, MEM};
enum branch_decision bd = EX;

char in[30];

struct{
	// char op[4];
	char w_reg[4];
	uint8_t delay;
} instruction;
struct instruction inst[4],cur_inst;
short cycle;

// check how many cycle 
int check_read(char r_reg[4])
{
	uint8_t max = 0;
	for(int i = 0; i < 4 ; ++i){
		if(!strcmp(r_reg, inst[i].w_reg) && (max < delay - i))
			max = delay - i;
	}
	return max;
}

int calc_cycle()
{
	char command[4];
	char r_reg[2][4];
	int tmp;

	sscanf(in, "%s", command);
	// lw
	if(!strcmp(command, "lw")){
		cur_inst.delay = 1;
		sscanf(in + 3, "%s,%d(%s)", cur_inst.w_reg, tmp, r_reg[0]);
		return check_read(r_reg[0]);
	}
	// sw
	else if(!strcmp(command, "sw")){
		cur_inst.w_reg = "$0";
		cur_inst.delay = 0;
		sscanf(in + 3, "%s,%d(%s)", r_reg[0], tmp, r_reg[1]);
		return MAX(check_read(r_reg[0]), check_read(r_reg[1]));
	}
	// beq
    else if(!strcmp(command, "beq")){
    	cur_inst.w_reg = ""
    }
	// R-type
	else {
	}
}

int main(int argc, char* argv[])
{
	// init parameter
	char *tmp;
	for(int i = 0;i < argc;++i){
		if(!strcmp(argv[i], "-fw")) // have forwarding
			fw = 1;
		else if(!strncmp(argv[i], "-bd=", 4)){ // where start branch
			tmp = argv[i]+3;
			if(!strcmp(tmp, "ID"))
				bd = ID;
			else if(!strcmp(tmp, "EX"))
				bd = EX;
			else if(!strcmp(tmp, "MEM"))
				bd = MEM;
		}
	}

	// init inst to nop
	for(int i = 1;i < 4;++i)
		inst[i] = {"$0","$0","$0",i};

	// start input & calculate
	char command[4];
	while(fgets(in,sizeof(in),stdin) != EOF){
		// calc cycle
		cycle = calc_cycle();
		printf("%d\n", cycle);

		// move instruction list to old
		// TODO: rewrite into linked list to increase performance;
		inst[0] = cur_inst;
		for(int i=1;i<4;++i)
			inst[i] = inst[i-1];
	}

	return 0;
}
