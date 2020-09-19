#srcs := $(shell find $(src_dir) -type f -name '*.cpp' -printf '%f\n' | sort)
#hdrs := $(shell find $(src_dir) -type f -name '*.h' -printf '%f\n' | sort)

srcs := debug.cpp do_args.cpp do_buildconf.cpp do_env.cpp do_yaml.cpp  main.cpp \
    Options_store.cpp
hdrs := debug.h do_args.h do_buildconf.h do_env.h do_yaml.h \
    Options_store.h


bin_dir ?= bin
target ?= $(bin_dir)/cs3
dep_dir ?= dep
src_dir ?= .
#inc_dirs := $(shell find $(src_dir) -type d -name inc)
inc_dirs ?= 

objs := $(srcs:%.cpp=$(bin_dir)/%.o)
deps := $(srcs:%.cpp=$(dep_dir)/%.d)

inc_flags := $(addprefix -I,$(inc_dirs))

CPPFLAGS =  $(inc_flags)
CFLAGS = -std=c18 -O2
CXXFLAGS = -std=c++20 -O2 -DDEBUG

LDFLAGS = -lglfw -lvulkan -lm -lstdc++ -lyaml-cpp

.PHONY: clang-format clean

all : $(target)

$(target) : $(objs)
	$(CXX) $(objs) -o $@ $(LDFLAGS)

$(subdirs):
	$(MAKE) -C $@

format :
	for f in $(srcs) $(hdrs) ; do \
	    clang-format-12 -i $$f ; \
	done

clean : 
	$(RM) $(objs)
	$(RM) $(deps)
	$(RM) $(target)

test: $(target)
	./$(target)

deps : $(deps)
	@echo "Dependencies are created in directory dep"

objs : $(objs)

# assembly
$(bin_dir)/%.o: %.s
	$(mkdir_p) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(bin_dir)/%.o: %.c 
	$(mkdir_p) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(bin_dir)/%.o: %.cpp 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# dependencies
$(dep_dir)/%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM -MT '$(patsubst %.cpp,$(bin_dir)/%.o,$<)' $< -MF $@

-include $(deps)

