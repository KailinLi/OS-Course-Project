CC = clang++
fileSystem: CLI.o StoreManager.o AllocManager.o FSBase.o FSAPI.o
	$(CC) CLI.o StoreManager.o AllocManager.o FSBase.o FSAPI.o -o fileSystem
CLI.o: CLI.cc FSAPI.h
	$(CC) CLI.cc -c
FSAPI.o: FSAPI.h FSAPI.cc FSBase.h
	$(CC) FSAPI.cc -c
FSBase.o: FSBase.h FSBase.cc AllocManager.h
	$(CC) FSBase.cc -c
AllocManager.o: AllocManager.h AllocManager.cc StoreManager.h
	$(CC) AllocManager.cc -c
StoreManager.o: StoreManager.h StoreManager.cc
	$(CC) StoreManager.cc -c
clean:
	rm *.o
	rm fileSystem