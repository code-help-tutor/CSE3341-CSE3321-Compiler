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

for value in {1..30}
do
	echo ""
	echo "Running ${value}.code"
	timeout 5 ${runner} Correct/${value}.code Correct/${value}.data > Correct/${value}.student
	echo "Running diff with ${value}.expected"
	grep -o '[[:digit:]]\+' Correct/${value}.student > temp1
	grep -o '[[:digit:]]\+' Correct/${value}.expected > temp2
	if cmp -s "temp1" "temp2"; then
		echo "Print looks good"
		score=$(($score + 1))
	else
		echo "Student output and expected output are different"
	fi
done

rm temp1
rm temp2

echo "Running error cases:"
echo ""
echo "Running 01.error:"
echo ""
echo ""
timeout 5 ${runner} Error/01.code Error/01.data
echo ""
echo ""
read -n 1 -p "Error is .data file not having enough values. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo "Running 02.error:"
echo ""
echo ""
timeout 5 ${runner} Error/02.code Error/02.data
echo ""
echo ""
read -n 1 -p "Error is assignment to null record variable. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo "Running 03.error:"
echo ""
echo ""
timeout 5 ${runner} Error/03.code Error/03.data
echo ""
echo ""
read -n 1 -p "Error is accessing outside of array bounds. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo "Running 04.error:"
echo ""
echo ""
timeout 5 ${runner} Error/04.code Error/04.data
echo ""
echo ""
read -n 1 -p "Error is division by 0. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi

echo ""
echo "Correct cases score out of 30:"
echo $score
echo "Error cases score out of 4:"
echo $error
echo ""

echo "Done!"