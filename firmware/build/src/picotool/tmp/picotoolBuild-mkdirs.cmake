# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/noobinventor/Documents/Projects/pi-pico-drone/firmware/build/_deps/picotool-src"
  "/home/noobinventor/Documents/Projects/pi-pico-drone/firmware/build/_deps/picotool-build"
  "/home/noobinventor/Documents/Projects/pi-pico-drone/firmware/build/_deps"
  "/home/noobinventor/Documents/Projects/pi-pico-drone/firmware/build/src/picotool/tmp"
  "/home/noobinventor/Documents/Projects/pi-pico-drone/firmware/build/src/picotool/src/picotoolBuild-stamp"
  "/home/noobinventor/Documents/Projects/pi-pico-drone/firmware/build/src/picotool/src"
  "/home/noobinventor/Documents/Projects/pi-pico-drone/firmware/build/src/picotool/src/picotoolBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/noobinventor/Documents/Projects/pi-pico-drone/firmware/build/src/picotool/src/picotoolBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/noobinventor/Documents/Projects/pi-pico-drone/firmware/build/src/picotool/src/picotoolBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
