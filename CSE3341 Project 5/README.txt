https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
Sitao Tong(tong.408)
core.h
CoreMod.pdf
executor.c -- executed parsed tree(with function and gc)
executor.h
main.c
makefile -- makefile
memory.c --starting memory file
memory.c --(with frame defined)
parser.c
parser.h
printer.c --modified to check function
printer.h
README.txt
scanner.c
scanner.h
semantic.c
semantic.h
tester.sh
tree.h

I'm implementing the executor with the order of:
- modifying the 'new record' function and make it on the heap, getting new obj like global one
- adding member in the frame structure and make it point to the object
- adding the counter globally and add a pointer to the counter in frame structure
- implement the counting gc function
- call it to print gc when counter decrease

printGC()
    - count and print the function



*for all error we exit the whole process.
