CC = clang++
fileSystem: CLI.o StoreManager.o AllocManager.o FSBase.o FSAPI.o
	$(CC) CLI.o StoreManager.o AllocManager.o FSBase.o FSAPI.o -o fileSystem -g
CLI.o: CLI.cc FSAPI.h
	$(CC) CLI.cc -c -g
FSAPI.o: FSAPI.h FSAPI.cc FSBase.h
	$(CC) FSAPI.cc -c -g
FSBase.o: FSBase.h FSBase.cc AllocManager.h
	$(CC) FSBase.cc -c -g
AllocManager.o: AllocManager.h AllocManager.cc StoreManager.h
	$(CC) AllocManager.cc -c -g
StoreManager.o: StoreManager.h StoreManager.cc Color.h
	$(CC) StoreManager.cc -c -g
clean:
	rm *.o
	rm fileSystem