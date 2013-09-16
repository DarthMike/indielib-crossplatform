DEPSDIR="../common/dependencies"
RUNDIR=`pwd`

echo "DEPENDENCIES RELATIVE DIR: $DEPSDIR"

#build dependencies
cd $DEPSDIR

#builds static + shared. Will delete shared to link statically
cd ./FreeImage
make clean
make
rm Dist/*.dylib

#build static SDL lib only
cd ../SDL-2.0
./configure --disable-shared --enable-static --prefix=`pwd`/osx
make clean
make
make install

#builds static + shared. Will delete shared to link statically
cd ../glew-1.9.0
make clean
make
rm lib/*.dylib

#build static freetype
cd ../freetype-2.5.0.1
./configure --disable-shared --enable-static --prefix=`pwd`/osx
make clean
make
make install

cd $RUNDIR