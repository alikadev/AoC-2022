CC		:=	gcc
D_SRC	:=	src
D_BUILD	:=	build
D_REL	:=	release
BINARY	:=	$(notdir $(shell pwd))

CFLAGS	:=	-Ofast -std=c99 -Wno-unused-command-line-argument
LDFLAGS	:= 	

SRC		:=	$(shell find $(D_SRC) -name '*.c')
OBJS 	:=	$(subst $(D_SRC), $(D_BUILD), $(SRC:%c=%o))

# ===== OS SPECIFIC ===== #
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CFLAGS+=-I/opt/homebrew/include -DOSX
	LDFLAGS+=-L/opt/homebrew/lib
endif
ifeq ($(UNAME_S),Linux)
	CFLAGS+=-DLINUX
	LDFLAGS+=
endif

# ===== ANY ===== #

build: $(BINARY)

all: clean $(BINARY)

release: release_code clean
release_code: $(BINARY)
ifndef ID
	@echo You need to define the release ID! make release ID=1
	@$(error ID is not set... use "make release ID=1")
endif
	@mkdir -p $(D_REL)
	@mkdir -p $(D_REL)/$(BINARY)_$(ID)
	@cp $(BINARY) $(D_REL)/$(BINARY)_$(ID)/
	@cp -r $(D_SRC) $(D_REL)/$(BINARY)_$(ID)/
	@echo $(BINARY) is copied to $(D_REL)/$(BINARY)_$(ID)

help:
	@echo 'Command list:'
	@echo '- help    | print this'
	@echo '- clean   | clean build'
	@echo '- build   | build (default)'
	@echo '- all     | clean and build all'
	@echo '- release | build all and put result in release directory'

clean:
	@echo "Code is clean"
	@mkdir -p $(D_BUILD)
	@rm -rf $(D_BUILD)/*
	@rm -f $(BINARY)

$(BINARY): $(OBJS)
	@echo
	@echo Linking the $(OBJS) to $@
	@$(CC) $(LDFLAGS) $(OBJS) -o $@

$(D_BUILD)/%.o: $(D_SRC)/%.c
	@mkdir -p ${dir $@}
	@echo Building $< to $@
	@$(CC) $(CFLAGS) -c $^ -o $@