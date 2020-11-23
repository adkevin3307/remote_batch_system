EXE = http_server console.cgi
OBJ_DIR = obj

SOURCES = $(wildcard src/*.cpp)

OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

OBJS_HTTP_SERVER = $(OBJS)
OBJS_CONSOLE_CGI = $(OBJS)

CXXFLAGS = -std=c++17 -I./include -Wall -O2

LIBS =

all: create_object_directory $(EXE)
	@echo Compile Success

create_object_directory:
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

http_server: $(OBJS_HTTP_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

console.cgi: $(OBJS_CONSOLE_CGI)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf $(EXE) $(OBJ_DIR)
