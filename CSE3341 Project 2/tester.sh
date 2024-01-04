https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#!/bin/bash

runner=$1

echo "Attempting to compile java code..."
javac *.java
runner="java Main"

score=0
error=0

for value in {1..26}
do
	echo ""
	echo "Running ${value}.code"
	timeout 5 ${runner} Correct/${value}.code > Correct/${value}.student
	#Check for correct print
	tr -d '[:space:]' < Correct/${value}.student > temp1
	tr -d '[:space:]' < Correct/${value}.code > temp2
	echo "Comparing input and output"
	if cmp -s "temp1" "temp2"; then
		echo "Print looks good"
		score=$(($score + 1))
	else
		echo "Student output and expected output are different"
	fi
done

rm temp1
rm temp2

echo ""
echo "Running error cases:"
echo ""
echo "Running 01.error:"
timeout 5 ${runner} Error/01.code
echo ""
echo ""
read -n 1 -p "Error is extra + in expression. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo ""
echo "Running 02.error:"
timeout 5 ${runner} Error/02.code
echo ""
echo ""
read -n 1 -p "Error is undeclared variable 'y' being used. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo ""
echo "Running 03.error:"
timeout 5 ${runner} Error/03.code
echo ""
echo ""
read -n 1 -p "Error is variable 'x' declared twice in the same scope. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo ""
echo "Running 04.error:"
timeout 5 ${runner} Error/04.code
echo ""
echo ""
read -n 1 -p "Error is while loop ends with 'else' missing. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo ""
echo "Running 05.error:"
timeout 5 ${runner} Error/05.code
echo ""
echo ""
read -n 1 -p "Error is integer used in 'new record' declaration. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo ""
echo "Running 06.error:"
timeout 5 ${runner} Error/06.code
echo ""
echo ""
read -n 1 -p "Error is 'do' in if statement. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo ""
echo "Running 07.error:"
timeout 5 ${runner} Error/07.code
echo ""
echo ""
read -n 1 -p "Error is equals used in assignment statement. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo ""
echo "Running 08.error:"
timeout 5 ${runner} Error/08.code
echo ""
echo ""
read -n 1 -p "Error is index used with an int variable. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo ""
echo "Running 09.error:"
timeout 5 ${runner} Error/09.code
echo ""
echo ""
read -n 1 -p "Error is extra tokens after end. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo ""
echo "Running 10.error:"
timeout 5 ${runner} Error/10.code
echo ""
echo ""
read -n 1 -p "Error is missing left parenthesis in expression. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi


echo ""
echo ""
echo "Correct cases score out of 26:"
echo $score
echo "Error cases score out of 10:"
echo $error

echo "Done!"