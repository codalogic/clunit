CPPTESTFILES = \
	Test/main-test.cpp \
	Test/clunit-test.cpp \
	Test/example-test.cpp

OBJDIR = obj

CCFLAGS = -I .. -I ../.. -I /rw/sw/clunit \
	-O3 -Werror -Wall -Wuninitialized -Wunused-variable -Wno-deprecated
	# -Wno-deprecated included to allow #include <strstream> (see code for why)

OBJTESTFILES = $(addprefix $(OBJDIR)/,$(CPPTESTFILES:.cpp=.o))

.PHONY: all

all: $(OBJDIR)/clunit-test

$(OBJDIR)/clunit-test: $(OBJTESTFILES)
	g++ -o $(OBJDIR)/clunit-test $(OBJTESTFILES) 
	$(OBJDIR)/clunit-test

$(OBJDIR)/%.o : %.cpp clunit.h
	g++ -c $(CCFLAGS) $< -o $@
