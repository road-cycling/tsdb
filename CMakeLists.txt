cmake_minimum_required(VERSION 3.22)
project(timeseries_db)

set(CMAKE_CXX_STANDARD 20)

include_directories(/opt/homebrew/Cellar/protobuf/3.19.4/lib)
include_directories(/opt/homebrew/Cellar/protobuf/3.19.4/include)

#find_package(Protobuf REQUIRED)
find_package(roaring REQUIRED)

add_executable(timeseries_db main.cpp index/indexer.h index/indexer.cpp index/indexer_router.cpp index/indexer_router.h stream/bitstream.cpp stream/bitstream.h stream/blockbuilder.cpp stream/blockbuilder.h timeseries/timeseries.cpp timeseries/timeseries.h service/query_write.pb.cpp service/query_write.pb.h hash/xxhash64.h index/indexed_schema.cpp index/indexed_schema.h)
target_link_libraries(timeseries_db PUBLIC roaring::roaring)