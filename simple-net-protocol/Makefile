CXX       = g++
CXXFLAGS  = -Wall -g -std=c++11 -fpic
OBJS      =  port.o ip.o string.o
EXEC      = firewall.exe
# link the program executable
$(EXEC): main.o libfirewall.so libinput.so 
	$(CXX) $(CXXFLAGS) main.o -o $(EXEC) -lfirewall -linput -L.   
# create the shared library
libfirewall.so: ip.o port.o string.o 
	$(CXX) $(CXXFLAGS) -shared $(OBJS) -o libfirewall.so -L.
#create the object files
main.o: main.cpp libfirewall.so input.h
	$(CXX) $(CXXFLAGS) -c -fpic main.cpp

string.o: string.cpp string.h
	$(CXX) $(CXXFLAGS) -c -fpic string.cpp

port.o: port.h port.cpp string.h generic-field.h
	$(CXX) $(CXXFLAGS) -c -fpic port.cpp

ip.o: ip.h ip.cpp string.h generic-field.h string.h 
	$(CXX) $(CXXFLAGS) -c -fpic ip.cpp

clean:
	rm -rf $(EXEC) *.o libfirewall.so
