#bin/sh
DEPSDIR="../common/dependencies"
TUTORIALSDIR="../tutorials/mac"
DESTDIR="../../../"

echo $1
if [ -z "$1" ] 
then
echo "EXPECTED ARCHIVE VERSION INPUT PARAMETER"
exit -1
fi

ARCHIVENAME="Indielib_OSX_$1"
echo $ARCHIVENAME

RUNDIR=`pwd`

echo "WORKING FROM: $RUNDIR"
echo "DEPENDENCIES RELATIVE DIR: $DEPSDIR"
echo "TUTORIALS RELATIVE DIR: $TUTORIALSDIR"
echo "DESTINATION DIRECTORY: $DESTDIR"

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

#clean all targets first
cd $RUNDIR

echo "************CLEAN ALL TARGETS*******************"
ALLSCHEMES=$(xcodebuild -workspace IndielibOSX.xcworkspace -list)

for SCHEME in $ALLSCHEMES
do
xcodebuild -workspace IndielibOSX.xcworkspace -scheme $SCHEME clean
done

#build indielib
echo"*************BUILD INDIELIB**********************"
xcodebuild -workspace IndielibOSX.xcworkspace -scheme IndieLib -configuration release

#build tutorials
cd $TUTORIALSDIR
./release.sh

#package distributable files in zip
echo"*************PACKAGE FILES************************"
cd $DESTDIR
rm $ARCHIVENAME.zip

#-X Option not to add hidden files .* in OSX
zip -r -X $ARCHIVENAME.zip Indielib/bin/mac
zip -r -X $ARCHIVENAME.zip Indielib/common/include
zip -r -X $ARCHIVENAME.zip Indielib/resources
zip -r -X $ARCHIVENAME.zip Indielib/tutorials --exclude "Indielib/tutorials/win/*" "Indielib/tutorials/linux/*"
zip -r -X $ARCHIVENAME.zip Indielib/docs --exclude "Indielib/docs/Doxygen/Doxyfile" "Indielib/docs/wiki/*"
#zip $ARCHIVENAME.zip Indielib/LICENSE
