EXE = http_server console.cgi
OBJ_DIR = obj

SOURCES = $(wildcard src/*.cpp)

SOURCES_HTTP_SERVER = $(SOURCES) $(wildcard src/http_server/*.cpp)
SOURCES_CONSOLE_CGI = $(SOURCES) $(wildcard src/console_cgi/*.cpp)

OBJS_HTTP_SERVER = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES_HTTP_SERVER)))))
OBJS_CONSOLE_CGI = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES_CONSOLE_CGI)))))

CXXFLAGS = -std=c++17 -I./include -Wall -O2

LIBS = -lpthread

all: create_object_directory $(EXE)
	@echo Compile Success

create_object_directory:
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: src/http_server/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: src/console_cgi/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

http_server: $(OBJS_HTTP_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

console.cgi: $(OBJS_CONSOLE_CGI)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf $(EXE) $(OBJ_DIR)
