test_pinfos: ../utils/utils.h ../utils/utils.c test_pinfos.c
	rm -rf ../utils/utils.h.gch
	gcc test_pinfos.c ../utils/utils.h ../utils/utils.c -o test_pinfos

debug: test_pinfos
	valgrind --leak-check=full ./test_pinfos
