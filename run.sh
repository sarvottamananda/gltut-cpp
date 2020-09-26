#!/bin/sh

build_dir=build

if [ -d "$build_dir" ]; then
  echo "run build.sh first"
fi

echo "Running ..."
meson test -C build

