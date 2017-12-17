## Tools

# echo with turned on backslash symbol recognition
ECHO = echo -e
# mkdir with flag to create perent directories
MKDIR = mkdir -p
# cp with recursive flag
CP_FOLDER = cp -r
# rm for folders
RM_FOLDER = rm -rf 

## Compilers

# C compiler
CC = gcc
# C++ compiler
CXX = g++

## Directories

# Test source directory
TESTS_DIRECTORY:=./Tests
# Directory for store builded test files
TEST_BUILD = $(TESTS_DIRECTORY)/Build
# Directory ti store build artifacts (in form of object files)
OBJ_DIR = $(TEST_BUILD)/obj
# Directory where library will be installed
INSTALL_DIR = /usr/local/include
# Source directories to be copy recursively when install target is called 
DIRECTORIES_TO_COPY = 	ArgumentsLib \
						ClassUtilsLib \
						DebugLib \
						PatternsLib

# Directories to include while building tests
INCLUDE_DIRECTORIES = 	-I. \
						$(DIRECTORIES_TO_COPY:%=-I./%) \
						-I./Test

## Flags

# Define type of system
ifeq ($(OS), Windows_NT)
	SYSTEM_FLAGS = -D WIN32 
else
	SYSTEM_FLAGS = -D LINUX
endif
# Warning flags
WARNING_FLAGS:= -pedantic -Wall -Wextra
# Debug flags
DEBUG_FLAGS:= -g3 -ggdb3 -D DEBUG
# Profiler flags
PROFILER_FLAGS:= -p -pg
# Optimisation flags
OPTIMISATION_FLAGS:= -O0
# C++ standard to use
CXX_STANDARD:=c++17

# C++ flags
CXX_FLAGS+= -std=$(CXX_STANDARD) \
			$(SYSTEM_FLAGS) \
			$(WARNING_FLAGS) \
			$(DEBUG_FLAGS) \
			$(PROFILER_FLAGS) \
			$(OPTIMISATION_FLAGS) \
			-fexceptions \
			$(INCLUDE_DIRECTORIES)

## Files

# Source files of tests
TESTS_SOURCES:= $(notdir $(wildcard $(TESTS_DIRECTORY)/*.cpp))
# Object file names of tests
TESTS_OBJECTS:= $(TESTS_SOURCES:%.cpp=$(OBJ_DIR)/%.o)
# Names of test applications to be build
TESTS_APPS:= $(TESTS_SOURCES:%.cpp=$(TEST_BUILD)/%.app)
# Names of dummy targets that runs test applications
TESTS_APP_RUN:= $(TESTS_SOURCES:%.cpp=$(TEST_BUILD)/%.run)
# Make dependency file names 
TESTS_DEPENDENCIES:= $(TESTS_OBJECTS:%.o=%.d)

## Other

REPOSITORY_LINK:= https://github.com/echo-Mike/CodeSnippets

PROJECT_NAME:=CodeSnippets

## Targets

# Main target
all: install tests

# Install library headers
install:
	@$(ECHO) "Copying files to: "$(INSTALL_DIR)/$(PROJECT_NAME)
	@$(MKDIR) $(INSTALL_DIR)/$(PROJECT_NAME)
	@$(CP_FOLDER) $(DIRECTORIES_TO_COPY) -t $(INSTALL_DIR)/$(PROJECT_NAME)/

# Clean tests build directory
clean:
	@$(ECHO) "Removing test files from: "$(TEST_BUILD)
	@$(RM_FOLDER) $(TEST_BUILD)

# Uninstall library headers
uninstall:
	@$(ECHO) "Removing files from: "$(INSTALL_DIR)/$(PROJECT_NAME)
	@$(RM_FOLDER) $(INSTALL_DIR)/$(PROJECT_NAME)

# Target for building and runing tests
tests: $(OBJ_DIR) $(TESTS_APPS) run_tests

# Include generated rules
-include $(TESTS_DEPENDENCIES)

# Generic rule to produce object and dependency files for test apps
$(OBJ_DIR)/%.o: $(TESTS_DIRECTORY)/%.cpp
	@$(ECHO) "Compiler error output is redirected to: "$(basename $@)ObjectBuildLog.txt
	$(CXX) $(CXX_FLAGS) -MD -o $@ -c $< 2> $(basename $@)ObjectBuildLog.txt

# Generic rule to produce executable files for test apps
$(TEST_BUILD)/%.app: $(OBJ_DIR)/%.o
	$(CXX) $(CXX_FLAGS) $^ -o $(basename $@)

ifeq ($(OS), Windows_NT)
# Generic rule to run created test executables on Windows platform
$(TEST_BUILD)/%.run: $(TEST_BUILD)/%.app
	@$(ECHO) "Starting test: " $(notdir $(basename $@))
	@$(basename $@).exe

else
# Generic rule to run created test executables on Linux platform
$(TEST_BUILD)/%.run: $(TEST_BUILD)/%.app
	@$(ECHO) "Starting test: " $(notdir $(basename $@))
	@eval $(basename $@)

endif

# Creating build directories
$(OBJ_DIR):
	@$(MKDIR) $@

# Dummy target for runing all tests
run_tests: $(TESTS_APP_RUN)

# Dummy target : prints out main Make variables of this script
make_test:
	@$(ECHO) "Using echo to show Make variables:"
	@$(ECHO) "\INSTALL_DIR = "$(INSTALL_DIR)
	@$(ECHO) "\tTESTS_SOURCES = "$(TESTS_SOURCES)
	@$(ECHO) "\tTESTS_OBJECTS = "$(TESTS_OBJECTS)
	@$(ECHO) "\tTESTS_APPS = "$(TESTS_APPS)
	@$(ECHO) "\tTESTS_DEPENDENCIES = "$(TESTS_DEPENDENCIES)
	@$(ECHO) "\tINCLUDE_DIRECTORIES = "$(INCLUDE_DIRECTORIES)
	@$(ECHO) "\tCXX_FLAGS = "$(CXX_FLAGS)

# Dummy target : prints out other target purposes
help:
	@$(ECHO) "Install or/and run tests of CodeSnippets library"
	@$(ECHO)
	@$(ECHO) "TARGETS:"
	@$(ECHO) "\thelp         Prints out this text"
	@$(ECHO) "\tinstall      Install library headers"
	@$(ECHO) "\tclean        Clean tests build directory"
	@$(ECHO) "\tuninstall    Uninstall library headers"
	@$(ECHO) "\ttests        Target for building and runing tests"
	@$(ECHO) "\trun_tests    Dummy target for runing all tests"
	@$(ECHO) "\tmake_test    Dummy target : prints out main Make variables of this script"
	@$(ECHO) "\tall          Runs install and then tests"
	@$(ECHO)
	@$(ECHO) "Supported variables:"
	@$(ECHO) "\tCXX          C++ compiler"
	@$(ECHO) "\tCC           C compiler - not used"
	@$(ECHO) "\tINSTALL_DIR  Directory where library will be installed in subdirectory INSTALL_DIR/$(PROJECT_NAME)"
	@$(ECHO) "\t             Default is: "$(INSTALL_DIR)
	@$(ECHO) "\tCXX_FLAGS    Aditional c++ flags"
	@$(ECHO) "\tCXX_STANDARD C++ standard to use. Default is c++17"
	@$(ECHO) "\tECHO         echo tool. Default is echo -e"
	@$(ECHO) "\tMKDIR        mkdir tool. Default is mkdir -p"
	@$(ECHO) "\tCP_FOLDER    Tool to recursively copy folders. Default is cp -r"
	@$(ECHO) "\tRM_FOLDER    Tool to recursively delete folders. Default is rm -rf"
	@$(ECHO)
	@$(ECHO) "Author:"
	@$(ECHO) "\tMikhail Demchenko : dev.echo.mike@gmail.com 2017"
	@$(ECHO) "\tThis file is part of $(REPOSITORY_LINK) repository"
	@$(ECHO) "\tPlease check LICENSE file for legals"

.PHONY: all install clean make_test $(OBJ_DIR) run_tests uninstall help

.PRECIOUS: $(OBJ_DIR)/%.o

.SECONDARY: $(OBJ_DIR)/%.o