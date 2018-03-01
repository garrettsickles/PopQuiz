# Compiler
CC_BIN = g++
CC_STD = -std=c++11
CC_DEF = -DPOPQUIZ_DISABLE_STDOUT -DPOPQUIZ_TEST_ALL
CC_WRN = -Wall
CC_INC = -I..

# File Extensions
EXT_CPP  = cpp

# Directories
DIR_EXAMPLE = .
DIR_BIN     = bin

# Targets
TGT_EXAMPLE := $(wildcard $(DIR_EXAMPLE)/*.$(EXT_CPP))

# Binaries
BIN_EXAMPLE := $(TGT_EXAMPLE:$(DIR_EXAMPLE)/%.$(EXT_CPP)=$(DIR_BIN)/%)

# Run examples
example: $(BIN_EXAMPLE)

clean:
	@rm -rf $(DIR_BIN)

# Build each example and run it
$(BIN_EXAMPLE) : $(DIR_BIN)/% : $(DIR_EXAMPLE)/%.$(EXT_CPP)
	@mkdir -p $(DIR_BIN)
	@$(CC_BIN) $(CC_STD) $(CC_DEF) -DPOPQUIZ_JSON_OUTPUT="$(DIR_BIN)/$<" $(CC_WRN) $(CC_INC) $< -g -o $@
	@./$@
