all:	control.c write.c
		gcc -c control.c
		gcc -c write.c
		gcc -o control control.o
		gcc -o write write.o
