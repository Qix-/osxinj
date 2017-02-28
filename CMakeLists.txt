cmake_minimum_required(VERSION 2.8)

project(osxinj_tut)

set(CMAKE_C_FLAGS " -fmessage-length=0 -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit=0 -std=gnu99 -Wno-trigraphs -fpascal-strings -O0 -Wno-missing-field-initializers -Wno-missing-prototypes -Wno-missing-braces -Wparentheses -Wswitch -Wno-unused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wno-bool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wpointer-sign -Wno-newline-eof -DDEBUG=1 -g3")

set(CMAKE_CXX_FLAGS " -fmessage-length=0 -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit=0 -std=gnu++11 -stdlib=libc++ -Wno-trigraphs -fpascal-strings -O0 -Wno-missing-field-initializers -Wno-missing-prototypes -Wno-non-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wno-unused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wno-bool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wno-c++11-extensions -DDEBUG=1 -g3")

set_source_files_properties(osxinj/mach_inject.c PROPERTIES LANGUAGE C)

# Oddly, you can't have a folder with the same name as the output file. Hence,
# 'osxinj_app'
add_executable(osxinj osxinj/mach_inject.c osxinj/injector.cpp osxinj/main.cpp)

add_library(bootstrap SHARED bootstrap/main.cpp)
set_target_properties(bootstrap PROPERTIES PREFIX "")

add_executable(testapp testapp/main.cpp)
