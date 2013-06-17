DEPSDIR="../common/dependencies"
RUNDIR=`pwd`

echo "DEPENDENCIES RELATIVE DIR: $DEPSDIR"

#build dependencies
cd $DEPSDIR

#builds static + shared. Will delete shared to link statically
cd ./FreeImage
make clean
make -f Makefile.iphone

#build static SDL lib only
cd ../SDL-2.0
MIN_OS_VERSION=5.0
./build-scripts/iosbuild.sh clean
./build-scripts/iosbuild.sh configure-armv7 configure-i386
./build-scripts/iosbuild.sh make-armv7 make-i386

cd $RUNDIR