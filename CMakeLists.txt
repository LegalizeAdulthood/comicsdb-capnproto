cmake_minimum_required(VERSION 3.22)
project(comicsdb-capnproto)

find_package(RapidJSON CONFIG REQUIRED)
find_package(CapnProto CONFIG REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(comicsdb)
