#srcs := $(shell find $(src_dir) -type f -name '*.cpp' -printf '%f\n' | sort)
#hdrs := $(shell find $(src_dir) -type f -name '*.h' -printf '%f\n' | sort)

srcs := \
    app_lighting.cpp \
    cs5_lighting.cpp \
    options.cpp \


hdrs := \
    app_lighting.h \
    options.h \
    config/cs_config.h \


lib_srcs := \
	app_base.cpp \
	debug.cpp \
	img_stuff.cpp \
	model_base.cpp \
	model_cube.cpp \
	model_ground.cpp \
	opengl_stuff.cpp \
	shader_stuff.cpp \
	stb_image_impl.cpp \
	window.cpp \
	window_factory.cpp \
	window_glfw.cpp \


bin_dir ?= bin
dep_dir ?= dep
src_dir ?= .
lib_dir ?= ../all/

target := $(bin_dir)/cs5
lib_target = $(bin_dir)/libcs.a

#inc_dirs := $(shell find $(src_dir) -type d -name inc)
#inc_dirs ?= ../../subprojects/glad/
inc_dirs := ../inc ./inc

#objs := $(srcs:%.cpp=$(bin_dir)/%.o) bin/glad.o 
objs := $(srcs:%.cpp=$(bin_dir)/%.o) 
deps := $(srcs:%.cpp=$(dep_dir)/%.d)
lib_objs := $(lib_srcs:%.cpp=$(bin_dir)/%.o) 
lib_deps := $(lib_srcs:%.cpp=$(dep_dir)/%.d)


includes := $(addprefix -I,$(inc_dirs)) 
libs := -lcs -lm -lstdc++ -lyaml-cpp -lGL -lGLEW -lglfw 

CPPFLAGS := $(CPPfLAGS)  $(includes)
CFLAGS := $(CFLAGS) -std=c11 -g
CXXFLAGS := $(CXXFLAGS) -std=c++20 -DDEBUG -g
LDFLAGS := $(LDFLAGS) $(libs) -L$(bin_dir)

.PHONY: clean format tags all

all : $(target)

$(target) : $(objs) $(lib_target)
	$(CXX) $(CXXFLAGS) $(objs) -o $@ $(LDFLAGS)

$(lib_target) : $(lib_objs)
	ar rcs $@ $(lib_objs)


$(bin_dir)/glad.o : ../../subprojects/glad/glad.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean : 
	$(RM) $(objs)
	$(RM) $(deps)
	$(RM) $(target)
	$(RM) $(lib_objs)
	$(RM) $(lib_target)

deps : $(deps) $(lib_deps)
	@echo "Dependencies are created in directory dep"

format : 
	@for f in $(shell find .  \( -name '*.cpp' -or -name '*.h' -or -name '*.c' \) -and -cnewer fmt-stamp) ; do \
		echo "Running clang-format on $$f" ; \
	    	clang-format -i $$f ; \
	done
	@touch fmt-stamp

tags:
	ctags -R --languages=C,C++ --kinds-C++=+p --fields=+iaS --extras=+qF --sort=yes

test: $(target)
	@./$(target)

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

# c++ source for lib
$(bin_dir)/%.o: $(lib_dir)/%.cpp 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


# dependencies
$(dep_dir)/%.d: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MT '$(patsubst %.cpp,$(bin_dir)/%.o,$<)' $< -MF $@

$(dep_dir)/%.d: $(lib_dir)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MT '$(patsubst %.cpp,$(bin_dir)/%.o,$<)' $< -MF $@

-include $(deps)
