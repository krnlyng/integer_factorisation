OUT			:= ltbnjf_factorisation
SRC			:= main.cpp ../common/common.cpp
OBJ         := $(patsubst %.c, %.o, $(filter %.c, $(SRC)))
OBJ         += $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRC)))
DEP         := $(OBJ:.o=.d)

USE_GMP		?= 1

CFLAGS      ?= -Wall -Werror -pedantic -std=c99 -DUSE_GMP=$(USE_GMP)
CXXFLAGS    ?= -Wall -Werror -pedantic -std=c++11 -DUSE_GMP=$(USE_GMP)
LDFLAGS     :=

ifeq ($(USE_GMP), 1)
LDLIBS      := -lgmp -lgmpxx
else
LDLIBS		:=
endif

DEBUG       ?= 0
VERBOSE     ?= 0

ifeq ($(DEBUG),1)
	CFLAGS += -O0 -g3 -ggdb -pg -DDEBUG=1
	CXXFLAGS += -O0 -g3 -ggdb -pg -DDEBUG=1
	LDFLAGS += -pg
endif

ifeq ($(VERBOSE),1)
	MSG := @true
	CMD :=
else
	MSG := @echo
	CMD := @
endif

.PHONY: release clean

release: CFLAGS += -O3 -flto
release: CXXFLAGS += -O3 -flto
release: LDFLAGS += -flto
release: $(OUT)

clean:
	$(MSG) -e "\tCLEAN\t"
	$(CMD)$(RM) $(OBJ) $(DEP) $(OUT)

$(OUT): $(OBJ)
	$(MSG) -e "\tLINK\t$@"
	$(CMD)$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c %.d
	$(MSG) -e "\tCC\t$@"
	$(CMD)$(CC) $(CFLAGS) -c $< -o $@

%.d: %.c
	$(MSG) -e "\tDEP\t$@"
	$(CMD)$(CC) $(CFLAGS) -MF $@ -MM $<

%.o: %.cpp %.d
	$(MSG) -e "\tCXX\t$@"
	$(CMD)$(CXX) $(CXXFLAGS) -c $< -o $@

%.d: %.cpp
	$(MSG) -e "\tDEP\t$@"
	$(CMD)$(CXX) $(CXXFLAGS) -MF $@ -MM $<

ifneq ($(MAKECMDGOALS),clean)
-include $(DEP)
endif

