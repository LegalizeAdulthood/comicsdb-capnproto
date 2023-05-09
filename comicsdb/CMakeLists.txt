# Application logic
add_library(comicsdb
  comicsdb.h
  comicsdb.cpp
  comic.h
  comic.cpp
  json.h
  json.cpp
)
target_link_libraries(comicsdb PUBLIC rapidjson)

# CapnProto code for comics
capnp_generate_cpp(sources headers comic.capnp)
add_library(comics-capnp
    comic.capnp
    ${sources}
    ${headers})
target_include_directories(comics-capnp PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(comics-capnp PUBLIC
    comicsdb
    CapnProto::kj CapnProto::capnp CapnProto::capnpc CapnProto::kj-tls CapnProto::capnp-rpc)

# Custom RPC service using CapnProto
add_executable(capnpService
  capnpService.cpp
)
target_link_libraries(capnpService PRIVATE comics-capnp)
set_property(TARGET capnpService PROPERTY CXX_STANDARD "17")

# Custom RPC client using CapnProto
add_executable(capnpClient
  capnpClient.cpp
)
target_link_libraries(capnpClient PRIVATE comics-capnp)
