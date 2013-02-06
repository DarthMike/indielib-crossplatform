#bin/bash
ARCHIVENAME="Indielib_OSX"
#clean all targets first
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
cd ../tutorials/mac
./release.sh

#package distributable files in zip
echo"*************PACKAGE FILES************************"
cd ../../../
rm $ARCHIVENAME.zip

#-X Option not to add hidden files .* in OSX
zip -r -X $ARCHIVENAME.zip Indielib/bin/mac
zip -r -X $ARCHIVENAME.zip Indielib/common/include
zip -r -X $ARCHIVENAME.zip Indielib/resources
zip -r -X $ARCHIVENAME.zip Indielib/tutorials --exclude "Indielib/tutorials/win/*" "Indielib/tutorials/linux/*"
zip -r -X $ARCHIVENAME.zip Indielib/docs --exclude "Indielib/docs/Doxygen/Doxyfile" "Indielib/docs/wiki/*"
#zip $ARCHIVENAME.zip Indielib/LICENSE
