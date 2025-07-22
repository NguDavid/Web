##
## EPITECH PROJECT, 2025
## Makefile
## File description:
##
##

SRC         =   src/parser/KeyValueParser.cpp  \
                src/parser/IniParser.cpp  \
                src/parser/HttpParser.cpp  \
                src/network/Server.cpp  \
                src/network/Socket.cpp  \
                src/network/ServerSocket.cpp  \
                src/network/ClientSocket.cpp  \
                src/network/EpollMultiplexer.cpp  \
                src/ModuleManager.cpp  \
                src/HtmlModule.cpp  \
                src/main.cpp

TEST_SRC	=   src/parser/KeyValueParser.cpp  \
                src/parser/IniParser.cpp  \
                src/parser/HttpParser.cpp

OBJ         =   $(SRC:.cpp=.o)

CXX         =   g++

CXXFLAGS    =   -Wall -Wextra -std=c++20

CPPFLAGS	=	-I./ -Iinclude/ -Iinclude/network/ -Iinclude/parser/

NAME        =   a.out

TEST_NAME   =   unit_test

LDFLAGS     =   -Llib/ -Iinclude/

LDLIBS      =

TESTFLAGS   =   --coverage -lcriterion

MODULES_DIR     =   libs

MODULE_NAMES    =   HtmlModule

MODULE_SRC      =   $(addsuffix .cpp, $(addprefix src/, $(MODULE_NAMES)))

MODULE_SO       =   $(addsuffix .so, $(addprefix $(MODULES_DIR)/lib, $(MODULE_NAMES)))

all: $(NAME) modules

modules: $(MODULE_SO)

$(MODULES_DIR)/lib%.so: src/%.cpp
	mkdir -p $(MODULES_DIR)
	$(CXX) -Wall -Wextra -std=c++20 -fPIC -shared $(CPPFLAGS) $< -o $@

$(NAME): $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(LDFLAGS) $(LDLIBS) $(CPPFLAGS)

test_run:
	$(CXX) tests/test.cpp $(TEST_SRC) $(CXXFLAGS) $(LDFLAGS) $(TESTFLAGS) -o $(TEST_NAME)
	./$(TEST_NAME)

coverage:
	gcovr --exclude  tests/ --branches

clean:
	rm -f $(OBJ)
	rm -f *.gcda
	rm -f *.gcno
	rm -f $(MODULE_SO)

fclean: clean
	rm -f $(NAME)
	rm -f $(TEST_NAME)

re:	fclean all

.PHONY: all clean fclean re test_run modules
