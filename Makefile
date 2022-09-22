
genetics: init.o manager.o utils.o process.bin timer.bin
	gcc -g -fsanitize=undefined -Wall -o genetics init.o manager.o utils.o

process.bin: process/process.c process/process.h utils.o
	gcc -c -g -Wall -fsanitize=undefined -D_GNU_SOURCE process/process.c process/process.h
	gcc process.o utils.o -Wall -fsanitize=undefined -o process.bin
	chmod +x process.bin

timer.bin: timer/timer.h timer/timer.c utils.o
	gcc -c -g -Wall -fsanitize=undefined timer/timer.c timer/timer.h
	gcc timer.o utils.o -Wall -fsanitize=undefined -o timer.bin
	chmod +x timer.bin

manager.o: manager.c manager.h
	gcc -c -g -Wall  -fsanitize=undefined manager.c manager.h

init.o: init/init.c init/init.h
	gcc -c -g -Wall -fsanitize=undefined init/init.c init/init.h

utils.o: utils/utils.c utils/utils.h
	gcc -c -g -Wall -fsanitize=undefined utils/utils.c utils/utils.h

exec: genetics
	./genetics; make clean

quick:
	make clean; make; ./genetics

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf genetics
	rm -rf timer.bin
	rm -rf process.bin
	rm -rf manager.h.gch
	rm -rf init/init.h.gch
	rm -rf utils/utils.h.gch
	rm -rf timer/timer.h.gch
	rm -rf process/process.h.gch
	killall process.bin || killall timer.bin || ./ripcs.sh
