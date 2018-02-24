CC = clang++
fileSystem: CLI.o StoreManager.o AllocManager.o FSBase.o FSAPI.o
	$(CC) CLI.o StoreManager.o AllocManager.o FSBase.o FSAPI.o -o fileSystem -O2
CLI.o: CLI.cc FSAPI.h
	$(CC) CLI.cc -c -O2
FSAPI.o: FSAPI.h FSAPI.cc FSBase.h
	$(CC) FSAPI.cc -c -O2
FSBase.o: FSBase.h FSBase.cc AllocManager.h
	$(CC) FSBase.cc -c -O2
AllocManager.o: AllocManager.h AllocManager.cc StoreManager.h
	$(CC) AllocManager.cc -c -O2
StoreManager.o: StoreManager.h StoreManager.cc Color.h
	$(CC) StoreManager.cc -c -O2
clean:
	rm *.o
	rm fileSystem