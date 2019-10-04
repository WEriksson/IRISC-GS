cmake_minimum_required(VERSION 3.13)
project(irisc_Gs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# compiler flags
set(CFLAGS "-O2 -std=gnu17 -Wall -Wpedantic")
# useful defines: GYRO_DEBUG, GPS_DEBUG, ENCODER_DEBUG, ST_TEST, SEQ_TEST, SEQ_DEBUG
set(COMPILE_DEFINES "-DST_DEBUG -DSEQ_TEST -DSEQ_DEBUG -DST_TEST -DGYRO_DEBUG -DGPS_DEBUG -DENCODER_DEBUG")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS} ${COMPILE_DEFINES}")

# linker flags
set(LDFLAGS "-lrt -lm -pthread -lcfitsio -lzstd")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LDFLAGS}")

# list of sources
set(SCR_DIR ${CMAKE_SOURCE_DIR}/src)
set(SOURCES
	${SCR_DIR}/init/init.c
	${SCR_DIR}/global_utils/global_utils.c
	${SCR_DIR}/e_link/e_link.c
	${SCR_DIR}/telemetry/telemetry.c
	${SCR_DIR}/telecommand/telecommand.c
)

# list of libraries
string(COMPARE EQUAL ${CMAKE_SYSTEM_PROCESSOR} "armv7l" strcmp)
if(strcmp)
	set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib_arm)
else()
        set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
endif(strcmp)
unset(strcmp CACHE)

#remove filenames from source files to add include directories
set(DIRS ${SOURCES})
string(REGEX REPLACE "\\/[^\\/]*\\.c" "" DIRS "${DIRS}")
include_directories(${DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include/)

add_executable(irisc-gs ${SOURCES})
target_link_libraries(irisc-gs ${LIBS})
