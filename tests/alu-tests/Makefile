NAME = alutest
SRCS = build/alu_test.c
include $(AM_HOME)/Makefile

GENERATOR = build/gen_alu_test

$(GENERATOR): gen_alu_test.c
	gcc -O2 $^ -o $@

$(SRCS): $(GENERATOR)
	$^ > $@
