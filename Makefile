EXE_1 = http_server console.cgi
EXE_2 = cgi_server.exe
OBJ_DIR = obj

SOURCES = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)

OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(SOURCES:src/%=%))))
OBJS_HTTP_SERVER = $(filter $(OBJ_DIR)/http_server/%.o, $(OBJS))
OBJS_CONSOLE_CGI = $(filter $(OBJ_DIR)/console_cgi/%.o, $(OBJS))
OBJS_CGI_SERVER = $(filter-out $(OBJ_DIR)/console_cgi/console_cgi.o $(OBJ_DIR)/http_server/http_server.o, $(OBJS))

CXXFLAGS = -std=c++17 -I./include -Wall -O2

LIBS = -lpthread -lboost_system -lboost_filesystem

ifeq ($(MAKECMDGOALS), part2)
	CXXFLAGS += -DWINDOWS
endif

all: part1 part2
	@echo Compile Success

part1: create_object_directory $(EXE_1)
	@echo Compile Part 1

part2: create_object_directory $(EXE_2)
	@echo Compile Part 2

create_object_directory:
	mkdir -p $(OBJ_DIR)/http_server $(OBJ_DIR)/console_cgi $(OBJ_DIR)/cgi_server $(OBJ_DIR)/panel_cgi

$(OBJ_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%/%.o: src/%/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

http_server: $(OBJS_HTTP_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

console.cgi: $(OBJS_CONSOLE_CGI)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

cgi_server.exe: $(OBJS_CGI_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf $(EXE_1) $(EXE_2) $(OBJ_DIR)
