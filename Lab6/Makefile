CC = clang
fileSystem: CLI.o StoreManager.o AllocManager.o FSBase.o FSAPI.o
	$(CC) CLI.o StoreManager.o AllocManager.o FSBase.o FSAPI.o -o fileSystem -O2
CLI.o: CLI.c FSAPI.h
	$(CC) CLI.c -c -O2
FSAPI.o: FSAPI.h FSAPI.c FSBase.h
	$(CC) FSAPI.c -c -O2
FSBase.o: FSBase.h FSBase.c AllocManager.h
	$(CC) FSBase.c -c -O2
AllocManager.o: AllocManager.h AllocManager.c StoreManager.h
	$(CC) AllocManager.c -c -O2
StoreManager.o: StoreManager.h StoreManager.c Color.h
	$(CC) StoreManager.c -c -O2
clean:
	rm *.o
	rm fileSystem