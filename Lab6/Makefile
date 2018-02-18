CC = clang++
fileSystem: CLI.o StoreManager.o AllocManager.o FSBase.o FSAPI.o
	$(CC) CLI.o StoreManager.o AllocManager.o FSBase.o FSAPI.o -o fileSystem
CLI.o: CLI.cc StoreManager.h AllocManager.h FSBase.h FSAPI.h
	$(CC) CLI.cc -c
StoreManager.o: StoreManager.h StoreManager.cc
	$(CC) StoreManager.cc -c
AllocManager.o: AllocManager.h AllocManager.cc
	$(CC) AllocManager.cc -c
FSBase.o: FSBase.h FSBase.cc
	$(CC) FSBase.cc -c
FSAPI.o: FSAPI.h FSAPI.cc
	$(CC) FSAPI.cc -c
clean:
	rm *.o
	rm fileSystem