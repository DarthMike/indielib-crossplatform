CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(freeimage)

INCLUDE (CheckIncludeFiles)
CHECK_INCLUDE_FILES (search.h CMAKE_HAVE_SEARCH_H)
if (CMAKE_HAVE_SEARCH_H)
  Message("Search file found (search.h)")
else (CMAKE_HAVE_SEARCH_H)
  Message("Search file NOT found (search.h)")
endif (CMAKE_HAVE_SEARCH_H)
CONFIGURE_FILE(Source/LibTIFF4/tif_config.h ${CMAKE_CURRENT_SOURCE_DIR}/Source/LibTIFF4/tif_config.h)


INCLUDE (CheckIncludeFiles)
CHECK_INCLUDE_FILES (machine/cpu-features.h CMAKE_HAVE_MCPUFEATURES)
CHECK_INCLUDE_FILES (asm/cpu-features.h     CMAKE_HAVE_ACPUFEATURES)

if (CMAKE_HAVE_MCPUFEATURES OR CMAKE_HAVE_ACPUFEATURES)
  Message("Adding Android cpu features ${ANDROID_NDK}/sources/android/cpufeatures")
  include_directories("${ANDROID_NDK}/sources/android/cpufeatures")
endif (CMAKE_HAVE_MCPUFEATURES OR CMAKE_HAVE_ACPUFEATURES)


SET(FREEIMAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(LIBPNG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source/LibPNG/)
#SET(ZLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source/zlib/)

ADD_DEFINITIONS("-D_CRT_SECURE_NO_DEPRECATE")
ADD_DEFINITIONS("-D_CRT_NONSTDC_NO_WARNINGS")
ADD_DEFINITIONS("-DFREEIMAGE_EXPORTS")
ADD_DEFINITIONS("-DHAVE_STDLIB_H")
ADD_DEFINITIONS("-DOPJ_STATIC")
ADD_DEFINITIONS("-DLIBRAW_BUILDLIB")
ADD_DEFINITIONS("-DLIBRAW_NODLL")
ADD_DEFINITIONS("-DMNG_BUILD_SO")
ADD_DEFINITIONS("-DFIP_EXPORTS")
ADD_DEFINITIONS("-DUSE_JPIP")

SET(CMAKE_C_FLAGS "-O3 -fPIC -fexceptions -fvisibility=hidden -DDISABLE_PERF_MEASUREMENT -D__ANSI__ -w -fPIC ${CMAKE_C_FLAGS} ")
SET(CMAKE_CXX_FLAGS "-w -fPIC -O3 -fPIC -fexceptions -fvisibility=hidden -Wno-ctor-dtor-privacy -D__ANSI__ ${CMAKE_CXX_FLAGS}")

#INCLUDE_DIRECTORIES(${LIBPNG_DIR})
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibPNG)
#INCLUDE_DIRECTORIES(${ZLIB_DIR})
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/ZLib)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibRawLite)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibRawLite/libraw)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibRawLite/src)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibRawLite/internal)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibOpenJPEG)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/OpenEXR/Imath)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/OpenEXR/IlmImf)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/OpenEXR/Iex)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/OpenEXR/IexMath)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/OpenEXR/Half)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/OpenEXR/IlmThread)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/OpenEXR/)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Wrapper/FreeImagePlus/)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibWebP/)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibWebP/src/)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibJXR/)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibJXR/jxrgluelib)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibJXR/image/sys)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibJXR/image/x86)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibJXR/image/encode)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibJXR/image/decode)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibJXR/common/include)
INCLUDE_DIRECTORIES(${FREEIMAGE_DIR}/Source/LibJPEG)


#FILE(GLOB png_files ${LIBPNG_DIR}/*.c)
FILE(GLOB png_files ${FREEIMAGE_DIR}/Source/LibPNG/*.c)
#FILE(GLOB zlib_files ${ZLIB_DIR}/*.c)
FILE(GLOB zlib_files ${FREEIMAGE_DIR}/Source/ZLib/*.c)
FILE(GLOB deprecation_files ${FREEIMAGE_DIR}/Source/DeprecationManager/*.cpp)
FILE(GLOB_RECURSE freeimage_files ${FREEIMAGE_DIR}/Source/FreeImage/*.cpp )
FILE(GLOB_RECURSE freeimage_plus_files ${FREEIMAGE_DIR}/Wrapper/FreeImagePlus/*.cpp )
FILE(GLOB_RECURSE freeimage_c_files ${FREEIMAGE_DIR}/Source/FreeImage/*.c )
FILE(GLOB freeimagetoolkit_files ${FREEIMAGE_DIR}/Source/FreeImageToolkit/*.cpp )
FILE(GLOB libjpeg_files ${FREEIMAGE_DIR}/Source/LibJPEG/j*.c ${FREEIMAGE_DIR}/Source/LibJPEG/transupp.c)
FILE(GLOB libmng_files ${FREEIMAGE_DIR}/Source/LibMNG/*.c)
FILE(GLOB libopenjpeg_files ${FREEIMAGE_DIR}/Source/LibOpenJPEG/*.c)
FILE(GLOB_RECURSE libraw_files ${FREEIMAGE_DIR}/Source/LibRawLite/*.cpp)
FILE(GLOB libtiff_files ${FREEIMAGE_DIR}/Source/LibTIFF4/*.c)
FILE(GLOB metadata_files ${FREEIMAGE_DIR}/Source/Metadata/*.cpp)
FILE(GLOB_RECURSE openexr_files ${FREEIMAGE_DIR}/Source/OpenEXR/*.cpp)
FILE(GLOB_RECURSE libwebp_files ${FREEIMAGE_DIR}/Source/LibWebP/*.c)
FILE(GLOB_RECURSE libjxr_files ${FREEIMAGE_DIR}/Source/LibJXR/*.c)


LIST(REMOVE_ITEM zlib_files "${ZLIB_DIR}/example.c")
LIST(REMOVE_ITEM zlib_files "${ZLIB_DIR}/minigzip.c")
LIST(REMOVE_ITEM png_files "${LIBPNG_DIR}/pngtest.c")

LIST(REMOVE_ITEM libjpeg_files "${FREEIMAGE_DIR}/Source/LibJPEG/jpegtran.c")
LIST(REMOVE_ITEM libjpeg_files "${FREEIMAGE_DIR}/Source/LibJPEG/jmemdos.c")
LIST(REMOVE_ITEM libjpeg_files "${FREEIMAGE_DIR}/Source/LibJPEG/jmemmac.c")
LIST(REMOVE_ITEM libjpeg_files "${FREEIMAGE_DIR}/Source/LibJPEG/jmemansi.c")
LIST(REMOVE_ITEM libjpeg_files "${FREEIMAGE_DIR}/Source/LibJPEG/jmemnobs.c")
LIST(REMOVE_ITEM libjpeg_files "${FREEIMAGE_DIR}/Source/LibJPEG/example.c")
LIST(REMOVE_ITEM libopenjpeg_files "${FREEIMAGE_DIR}/Source/LibOpenJPEG/t1_generate_luts.c")
LIST(REMOVE_ITEM libtiff_files "${FREEIMAGE_DIR}/Source/LibTIFF4/tif_unix.c")
LIST(REMOVE_ITEM libtiff_files "${FREEIMAGE_DIR}/Source/LibTIFF4/tif_acorn.c")
LIST(REMOVE_ITEM libtiff_files "${FREEIMAGE_DIR}/Source/LibTIFF4/mkg3states.c")
LIST(REMOVE_ITEM libtiff_files "${FREEIMAGE_DIR}/Source/LibTIFF4/tif_apple.c")
LIST(REMOVE_ITEM libtiff_files "${FREEIMAGE_DIR}/Source/LibTIFF4/tif_atari.c")
LIST(REMOVE_ITEM libtiff_files "${FREEIMAGE_DIR}/Source/LibTIFF4/tif_vms.c")
LIST(REMOVE_ITEM libtiff_files "${FREEIMAGE_DIR}/Source/LibTIFF4/tif_win3.c")
LIST(REMOVE_ITEM libtiff_files "${FREEIMAGE_DIR}/Source/LibTIFF4/tif_wince.c")
LIST(REMOVE_ITEM libtiff_files "${FREEIMAGE_DIR}/Source/LibTIFF4/tif_win32.c")
LIST(REMOVE_ITEM libtiff_files "${FREEIMAGE_DIR}/Source/LibTIFF4/tif_msdos.c")
LIST(REMOVE_ITEM libtiff_files "${FREEIMAGE_DIR}/Source/LibTIFF4/mkspans.c")
LIST(REMOVE_ITEM openexr_files "${FREEIMAGE_DIR}/Source/OpenEXR/IlmThread/IlmThreadMutexWin32.cpp")
LIST(REMOVE_ITEM openexr_files "${FREEIMAGE_DIR}/Source/OpenEXR/IlmThread/IlmThreadWin32.cpp")
LIST(REMOVE_ITEM openexr_files "${FREEIMAGE_DIR}/Source/OpenEXR/IlmThread/IlmThreadSemaphoreWin32.cpp")
LIST(REMOVE_ITEM openexr_files "${FREEIMAGE_DIR}/Source/OpenEXR/Half/eLut.cpp")
LIST(REMOVE_ITEM openexr_files "${FREEIMAGE_DIR}/Source/OpenEXR/Half/toFloat.cpp")
LIST(REMOVE_ITEM openexr_files "${FREEIMAGE_DIR}/Source/OpenEXR/IlmImf/b44ExpLogTable.cpp")
LIST(REMOVE_ITEM libraw_files "${FREEIMAGE_DIR}/Source/LibRawLite/internal/aahd_demosaic.cpp")
LIST(REMOVE_ITEM libraw_files "${FREEIMAGE_DIR}/Source/LibRawLite/internal/dht_demosaic.cpp")
LIST(REMOVE_ITEM libraw_files "${FREEIMAGE_DIR}/Source/LibRawLite/internal/wf_filtering.cpp")

LIST(REMOVE_ITEM libraw_files "${FREEIMAGE_DIR}/Source/LibRawLite/src/libraw_fuji_compressed.cpp")
LIST(REMOVE_ITEM libraw_files "${FREEIMAGE_DIR}/Source/LibRawLite/src/libraw_xtrans_compressed.cpp")

#LIST(REMOVE_ITEM libwebp_files "${FREEIMAGE_DIR}/Source/LibWebP/src/demux/demux.c")
#LIST(REMOVE_ITEM freeimage_files "${FREEIMAGE_DIR}/Source/FreeImage/PluginWebP.cpp")
#for android TARGET_ARCH_ABI == armeabi-v7a then include the neon below
#LIST(REMOVE_ITEM libwebp_files "${FREEIMAGE_DIR}/Source/LibWebP/src/dsp/dec_neon.c")
#LIST(REMOVE_ITEM libwebp_files "${FREEIMAGE_DIR}/Source/LibWebP/src/dsp/enc_neon.c")
#LIST(REMOVE_ITEM libwebp_files "${FREEIMAGE_DIR}/Source/LibWebP/src/dsp/upsampling_neon.c")
######################################
#LIST(REMOVE_ITEM libwebp_files "${FREEIMAGE_DIR}/Source/LibWebP/src/dsp/dec_clip_tables.c")
#LIST(REMOVE_ITEM libwebp_files "${FREEIMAGE_DIR}/Source/LibWebP/src/dsp/dec_mips32.c")
#LIST(REMOVE_ITEM libwebp_files "${FREEIMAGE_DIR}/Source/LibWebP/src/dsp/upsampling_mips32.c")


if (ANDROID)
  FILE(GLOB_RECURSE libwebp_files ${FREEIMAGE_DIR}/Source/LibWebP/*.doesnotexist)
endif()

ADD_LIBRARY (freeimage ${png_files} ${zlib_files} ${deprecation_files} 
  ${freeimage_files}  ${freeimage_c_files} ${freeimagetoolkit_files} ${freeimagelib_files} ${libjpeg_files}
  ${libmng_files} ${libopenjpeg_files} ${libraw_files}
  ${libtiff_files} ${metadata_files} ${openexr_files} ${libwebp_files} ${libjxr_files} 
  )

install(TARGETS freeimage 
  RUNTIME DESTINATION bin 
  LIBRARY DESTINATION lib 
  ARCHIVE DESTINATION lib)

install(FILES "Wrapper/FreeImagePlus/FreeImagePlus.h"   DESTINATION include)                                          
install(FILES "Source/FreeImage.h"                      DESTINATION include)                                          

