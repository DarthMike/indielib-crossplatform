# Copyright (C) 2007-2012 LuaDist.
# Created by Peter Kapec
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.

# This file is taken from : https://github.com/LuaDist/freetype/blob/master/CMakeLists.txt


include ( CheckIncludeFile )

# Specify library include directories
        include_directories("${PROJECT_SOURCE_DIR}/builds/ansi")
        include_directories("${PROJECT_SOURCE_DIR}/include")
        include_directories("${PROJECT_SOURCE_DIR}/include/freetype")
        include_directories("${PROJECT_SOURCE_DIR}/include/freetype/config")

        # For the auto-generated ftmodule.h file
        include_directories("${PROJECT_BINARY_DIR}/include")
        include_directories("${PROJECT_SOURCE_DIR}/objs")

        INCLUDE_DIRECTORIES(include)
        include_directories("src/truetype")
        include_directories("src/sfnt")
        include_directories("src/autofit")
        include_directories("src/smooth")
        include_directories("src/raster")
        include_directories("src/psaux")
        include_directories("src/psnames")

# First, compiler definitions for building the library
        ADD_DEFINITIONS(-DFT2_BUILD_LIBRARY)
        add_definitions("-DFT_CONFIG_MODULES_H=<ftmodule.h>")
	add_definitions("-fPIC")

set(BASE_SRCS
    src/base/ftsystem.c
    src/base/ftdebug.c
    src/base/ftinit.c
    src/base/ftbbox.c
    src/base/ftbitmap.c
    src/base/ftcid.c
    src/base/ftadvanc.c
    src/base/ftcalc.c
    src/base/ftdbgmem.c
    src/base/ftgloadr.c
    src/base/ftobjs.c
    src/base/ftoutln.c
    src/base/ftrfork.c
    src/base/ftsnames.c
    src/base/ftstream.c
    src/base/fttrigon.c
    src/base/ftutil.c
    src/base/ftfstype.c
    src/base/ftgasp.c
    src/base/ftglyph.c
    src/base/ftgxval.c
    src/base/ftlcdfil.c
    src/base/ftmm.c
    src/base/ftotval.c
    src/base/ftpatent.c
    src/base/ftpfr.c
    src/base/ftstroke.c
    src/base/ftsynth.c
    src/base/fttype1.c
    src/base/ftwinfnt.c
    src/base/ftxf86.c
    src/truetype/truetype.c
    src/type1/type1.c
    src/cff/cff.c
    src/cid/type1cid.c
    src/pfr/pfr.c
    src/type42/type42.c
    src/winfonts/winfnt.c
    src/pcf/pcf.c
    src/bdf/bdf.c
    src/sfnt/sfnt.c
    src/autofit/autofit.c
    src/pshinter/pshinter.c
    src/raster/raster.c
    src/smooth/smooth.c
    src/cache/ftcache.c
    src/gzip/ftgzip.c
    src/lzw/ftlzw.c
    src/bzip2/ftbzip2.c
    src/psaux/psaux.c
    src/psnames/psmodule.c)

        ADD_LIBRARY(freetype STATIC ${BASE_SRCS} )

# Tests and other stuff
#        CHECK_INCLUDE_FILE(sys/wait.h HAVE_WAIT_H)
#
#        IF(HAVE_WAIT_H)
#                ADD_EXECUTABLE(ftrandom src/tools/ftrandom/ftrandom.c)
#                TARGET_LINK_LIBRARIES(ftrandom freetype m)                # todo: m
#                INSTALL(TARGETS ftrandom DESTINATION ${INSTALL_EXAMPLE} COMPONENT Example)
#        ENDIF()
#
#        ADD_EXECUTABLE(test_afm src/tools/test_afm.c)
#        TARGET_LINK_LIBRARIES(test_afm freetype m)
#
#        ADD_EXECUTABLE(test_bbox src/tools/test_bbox.c)
#        TARGET_LINK_LIBRARIES(test_bbox freetype m)
#
#        ADD_EXECUTABLE(test_trig src/tools/test_trig.c)
#        TARGET_LINK_LIBRARIES(test_trig freetype m)
#
#        INSTALL(TARGETS test_afm test_bbox test_trig RUNTIME DESTINATION ${INSTALL_EXAMPLE} COMPONENT Example)
#
# Install Freetype Library
#       install_library(freetype)
#
# Install Freetype Headers
#        install_header(include/)
#
# Install Freetype Documentation
#        install_doc(docs/reference)
#        install_doc(docs/VERSION.DLL docs/formats.txt docs/FTL.TXT docs/GPLv2.TXT docs/LICENSE.TXT docs/PROBLEMS docs/raster.txt docs/TODO
#                ChangeLog ChangeLog.20 ChangeLog.21 ChangeLog.22 ChangeLog.23 README README.git)
#
#
#
#
