bin_dir ?= bin
src_dir ?= .
target ?= $(bin_dir)/cs4

#srcs := $(shell find $(src_dir) -type f -name '*.cpp' -printf '%f\n' | sort)
#hdrs := $(shell find $(src_dir) -type f -name '*.hpp' -printf '%f\n' | sort)

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
    app.hpp \
    app_base.hpp \
    cs_config.hpp \
    debug.hpp \
    do_args.hpp \
    do_meson.hpp \
    do_env.hpp \
    do_yaml.hpp \
    global.hpp \
    img_stuff.hpp \
    model_base.hpp \
    model_cube.hpp \
    options_store.hpp \
    shader_stuff.hpp \
    window.hpp \
    window_factory.hpp \
    window_glfw.hpp \


dep_dir ?= dep
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
	@for f in $(shell find .  \( -name '*.cpp' -or -name '*.hpp' -or -name '*.c'  -or -name '*.h' \) -and -cnewer fmt-stamp) ; do \
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

