DEPSDIR="../common/dependencies"
RUNDIR=`pwd`

echo "DEPENDENCIES RELATIVE DIR: $DEPSDIR"

#build dependencies
cd $DEPSDIR

#builds static + shared. Will delete shared to link statically
cd ./FreeImage
make clean
make -f Makefile.iphone
echo "Creating FreeImage fat Binary.."
lipo -create Dist/libFreeimage-iphonesimulator.a Dist/libFreeimage-iphone.a -output Dist/libFreeimage.a
echo "Created FreeImage fat Binary"

#build static SDL lib only
cd ../SDL-2.0
export MIN_OS_VERSION=6.0
./build-scripts/iosbuild.sh clean
./build-scripts/iosbuild.sh configure-armv7 
./build-scripts/iosbuild.sh make-armv7 
./build-scripts/iosbuild.sh configure-i386
./build-scripts/iosbuild.sh make-i386
./build-scripts/iosbuild.sh merge

# copy binaries in to a more accessible location
#mkdir ios
#mkdir ios/armv7
#mkdir ios/i386
#cp ./build/armv7/build/*.a ./ios/armv7
#cp ./build/armv7/build/*.la ./ios/armv7
#cp ./build/i386/build/*.a ./ios/i386
#cp ./build/i386/build/*.la ./ios/i386

cd $RUNDIR