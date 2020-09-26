#!/bin/sh

build_dir=build

if [ -d "$build_dir" ]; then
  echo "Build directory exists.. deleting it"
  rm -rf $build_dir
fi

echo "Building ..."
meson build
echo "Compiling ..."
meson compile -C $build_dir
echo "Running ..."
meson test -C $build_dir

