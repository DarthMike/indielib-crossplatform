DEPSDIR="../common/dependencies"
RUNDIR=`pwd`
FREEIMAGEDIR="FreeImage"
SDLDIR="SDL-2.0"
GLEWDIR="glew-1.9.0"
FREETYPEDIR="freetype-2.5.0.1"

set -e

echo "DEPENDENCIES RELATIVE DIR: $DEPSDIR"

#build dependencies
cd $DEPSDIR

#builds static + shared. Will delete shared to link statically
echo -e "\n\n************* BUILDING FREEIMAGE ***********************\n\n"
cd "$FREEIMAGEDIR"
make -f Makefile.osx
echo -e "\n\n+++++++++++++ FREEIMAGE BUILT! +++++++++++++++++++++++++\n\n"

#build static SDL lib only
echo -e "\n\n************* BUILDING SDL ***********************\n\n"
cd ..
cd "$SDLDIR"
./configure --disable-shared --enable-static --prefix=`pwd`/osx
make clean
make
make install
echo -e "\n\n+++++++++++++ SDL BUILT! +++++++++++++++++++++++++\n\n"

#builds static + shared. Will delete shared to link statically
echo -e "\n\n************* BUILDING GLEW ***********************\n\n"
cd ..
cd "$GLEWDIR"
make clean
make
echo -e "\n\n+++++++++++++ GLEW BUILT! +++++++++++++++++++++++++\n\n"

#build static freetype
echo -e "\n\n************* BUILDING FREETYPE ***********************\n\n"
cd ..
cd "$FREETYPEDIR"
export MACOSX_DEPLOYMENT_TARGET=10.6 
./configure --disable-shared --enable-static --prefix=`pwd`/osx --with-old-mac-fonts CFLAGS="-Os -arch i386 -arch x86_64"
make clean
make
make install
echo -e "\n\n+++++++++++++ FREETYPE BUILT! +++++++++++++++++++++++++\n\n"

cd $RUNDIR

set +e