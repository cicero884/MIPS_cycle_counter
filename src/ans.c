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
	char r_reg1[4];
	char r_reg2[4];
	int cycle;
} instruction;
struct instruction inst[4],cur_inst;

int check_read()

int calc_cycle()
{
	sscanf(in,"%s",command);
	if(!strcmp(command,"lw")){
		int tmp;
		cur_inst.w_reg = "$0";
		sscanf(in+3, "%s,%d(%s)",cur_inst.r_reg1, tmp, cur_inst.r_reg2);
		return MAX(check_read(cur_inst.r_reg1), check_read(cur_inst.r_reg2));
	}
	else if(!strcmp(command,"sw"));
	else if(!strcmp(command,"beq"));
	else ;

}

int main(int argc, char* argv[])
{
	// init parameter
	char *tmp;
	for(int i = 0;i < argc;++i){
		if(!strcmp(argv[i], "-fw"))
			fw = 1;
		else if(!strncmp(argv[i], "-bd=", 4)){
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
	for(int i = 1;i < 5;++i)
		inst[i] = {"$0","$0","$0",i};

	// start input & calculate
	char command[4];
	short cycle;
	while(fgets(in,sizeof(in),stdin) != EOF){
		// calc cycle
		cycle = calc_cycle();
		inst[4].cycle = cycle;
		printf("%d\n", cycle);

		// move instruction list to old
		// TODO: rewrite into linked list to increase performance;
		for(int i=0;i<2;++i)
			inst[i] = inst[i+1];
		inst[4] = cur_inst;
	}

	return 0;
}
