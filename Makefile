demo: demo.cc
	g++ demo.cc -std=c++14 src/binary.cc src/proxyloader.cc src/metadata.cc src/section.cc src/symbol.cc src/data.cc

loader:
	g++ demo.cc -D USE_PBA_LOADER src/binary.cc src/pbaloader.cc src/metadata.cc src/section.cc src/symbol.cc src/data.cc -lbfd -std=c++14

clean:
	rm a.out