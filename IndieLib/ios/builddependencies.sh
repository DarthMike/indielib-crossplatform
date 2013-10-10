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
#SDL built by project dependency on XCode
#cd ../SDL-2.0
#make clean
#export MIN_OS_VERSION=6.0
#./build-scripts/iosbuild.sh clean
#./build-scripts/iosbuild.sh configure-armv7 
#./build-scripts/iosbuild.sh make-armv7 
#./build-scripts/iosbuild.sh configure-i386
#./build-scripts/iosbuild.sh make-i386
#./build-scripts/iosbuild.sh merge

cd $RUNDIR