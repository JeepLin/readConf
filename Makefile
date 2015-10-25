obj:= main.o printer.o stream.o zmalloc.o sds.o importer.o cpp_file.o command_interface.o
tar:= main

$(tar):$(obj)
	g++ $^ -o $@ -g

clean:
	rm *.o main -rf

