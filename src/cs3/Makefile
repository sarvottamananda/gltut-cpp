#srcs := $(shell find $(src_dir) -type f -name '*.cpp' -printf '%f\n' | sort)
#hdrs := $(shell find $(src_dir) -type f -name '*.h' -printf '%f\n' | sort)

srcs := \
    app.cpp \
    app_base.cpp \
    debug.cpp \
    do_args.cpp \
    do_meson.cpp \
    do_env.cpp \
    do_yaml.cpp  \
    global.cpp \
    img_stuff.cpp \
    main.cpp \
    model_base.cpp \
    model_cube.cpp \
    options_store.cpp \
    shader_stuff.cpp \
    stb_image_impl.cpp \
    window.cpp \
    window_factory.cpp \
    window_glfw.cpp \


hdrs := \
    app.h \
    app_base.h \
    cs_config.h \
    debug.h \
    do_args.h \
    do_meson.h \
    do_env.h \
    do_yaml.h \
    global.h \
    img_stuff.h \
    model_base.h \
    model_cube.h \
    options_store.h \
    shader_stuff.h \
    window.h \
    window_factory.h \
    window_glfw.h \


bin_dir ?= bin
target ?= $(bin_dir)/cs3
dep_dir ?= dep
src_dir ?= .
#inc_dirs := $(shell find $(src_dir) -type d -name inc)
#inc_dirs ?= ../../subprojects/glad/

#objs := $(srcs:%.cpp=$(bin_dir)/%.o) bin/glad.o 
objs := $(srcs:%.cpp=$(bin_dir)/%.o) 
deps := $(srcs:%.cpp=$(dep_dir)/%.d)

inc_flags := $(addprefix -I,$(inc_dirs)) -I.

CPPFLAGS =  $(inc_flags)
CFLAGS = -std=c11 -g
CXXFLAGS = -std=c++20 -DDEBUG -g

#LDFLAGS = -lglfw -lm -lstdc++ -lyaml-cpp -ldl
LDFLAGS = -lm -lstdc++ -lyaml-cpp -lGL -lGLEW -lglfw

.PHONY: clean format tags

all : $(target)

$(target) : $(objs)
	$(CXX) $(CXXFLAGS) $(objs) -o $@ $(LDFLAGS)


$(bin_dir)/glad.o : ../../subprojects/glad/glad.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean : 
	$(RM) $(objs)
	$(RM) $(deps)
	$(RM) $(target)

deps : $(deps)
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

# dependencies
$(dep_dir)/%.d: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MT '$(patsubst %.cpp,$(bin_dir)/%.o,$<)' $< -MF $@

-include $(deps)

