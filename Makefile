EXE_1 = http_server console.cgi
EXE_2 = cgi_server.exe
OBJ_DIR = obj

SOURCES = $(wildcard src/*/*.cpp)

OBJS = $(addsuffix .o, $(basename $(SOURCES:src/%=%)))

OBJS_HTTP_SERVER = $(addprefix $(OBJ_DIR)/linux/, $(notdir $(filter http_server/%.o, $(OBJS))))
OBJS_CONSOLE_CGI = $(addprefix $(OBJ_DIR)/linux/, $(notdir $(filter console_cgi/%.o, $(OBJS))))
OBJS_CGI_SERVER = $(addprefix $(OBJ_DIR)/windows/, $(filter-out console_cgi.o http_server.o, $(notdir $(OBJS))))

CXXFLAGS = -std=c++17 -I./include -Wall -O2

LIBS = -lpthread -lboost_system -lboost_filesystem

all: part1 part2
	@echo Compile Success

part1: create_object_directory $(EXE_1)
	@echo Part 1 Compile Finish

part2: create_object_directory $(EXE_2)
	@echod Part 2 Compile Finish

create_object_directory:
	mkdir -p $(OBJ_DIR)/linux $(OBJ_DIR)/windows

$(OBJ_DIR)/linux/%.o: src/http_server/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/linux/%.o: src/console_cgi/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/windows/%.o: src/http_server/%.cpp
	$(CXX) $(CXXFLAGS) -DWINDOWS -c -o $@ $<

$(OBJ_DIR)/windows/%.o: src/console_cgi/%.cpp
	$(CXX) $(CXXFLAGS) -DWINDOWS -c -o $@ $<

$(OBJ_DIR)/windows/%.o: src/panel_cgi/%.cpp
	$(CXX) $(CXXFLAGS) -DWINDOWS -c -o $@ $<

$(OBJ_DIR)/windows/%.o: src/cgi_server/%.cpp
	$(CXX) $(CXXFLAGS) -DWINDOWS -c -o $@ $<

http_server: $(OBJS_HTTP_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

console.cgi: $(OBJS_CONSOLE_CGI)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

cgi_server.exe: $(OBJS_CGI_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf $(EXE_1) $(EXE_2) $(OBJ_DIR)
