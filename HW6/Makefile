CXX         = g++
CXXFLAGS    = -Wall -Wextra -g -L.
LDFLAGS     = -fPIC -shared

APP_SOURCES = main.cpp
LIB_SOURCES = string.cpp field.cpp port.cpp ip.cpp
LIB_OTHER   = input

DEST_EXE    = firewall.exe
DEST_LIB    = firewall

all: ${DEST_EXE}

${DEST_LIB}:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIB_SOURCES) -o lib$(DEST_LIB).so

${DEST_EXE}: ${DEST_LIB}
	$(CXX) $(CXXFLAGS) $(APP_SOURCES) -o $(DEST_EXE) -lfirewall -linput -L.
	
clean:
	rm -f $(DEST_EXE) $(DEST_LIB)