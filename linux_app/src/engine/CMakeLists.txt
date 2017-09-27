# engine test
file(GLOB engine_srcs *.cc *.cpp)
add_executable(engine_test ${engine_srcs})

install(TARGETS engine_test DESTINATION /home/gsc/nfs)
