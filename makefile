CC = g++
CFLAGS = -Wall -g -std=c++17
LIBS = -lm -ldl -lreadline -lpthread
LIBA = libabc.a libcadical.a
OBJDIR = obj

# find all the source files in the current directory and its subdirectories
SRCS = $(shell find . -name "*.cpp")
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))

# vpath helps make to look for the .cpp files in all subdirectories
vpath %.cpp $(sort $(dir $(SRCS)))

bmatch: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBA) $(LIBS)

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# to clean up the objects and executable
clean:
	-rm -rf $(OBJDIR)
	-rm -f bmatch

.PHONY: clean