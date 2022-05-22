# You should not edit this file(this will be replaced)

TEST_DIR=testdata
PROGRAM=output/cycle_calculator

if [ $# -eq 0 ] ;then
	echo "you need to specify testdata"
	echo "ex: $0 no_forwarding"
	echo "available folders are :"
	find testdata -mindepth 1 -type d | cut -d '/' -f 2
else
	source $TEST_DIR/$1/argument
	for inst in $TEST_DIR/$1/*.s; do
		result=$($PROGRAM -fw=$fw -bd=$bd < $inst)
		diff $result $(echo $inst |cut  -f 1 -d '.').out
	done
fi
