#bin/bash

TUTORIALS="Tutorial01 Tutorial02 Tutorial03 Tutorial04 Tutorial05 Tutorial06 Tutorial07 Tutorial08 Tutorial09 Tutorial10 Tutorial11 Tutorial12 Tutorial13 Tutorial14 Tutorial15 Tutorial16 TutorialAdvanced01 TutorialAdvanced02 TutorialAdvanced03 TutorialAdvanced04 TutorialAdvanced05 Benchmark01 Benchmark02"


#clean all targets first
echo "************CLEAN ALL TARGETS*******************"
ALLSCHEMES=$(xcodebuild -list)

for SCHEME in $ALLSCHEMES
do
xcodebuild -project IndielibTutorials.xcodeproj -scheme $SCHEME clean
done

#build tutorials
echo "************START BUILDING TUTORIALS*******************"
for TUTORIAL in $TUTORIALS
do 
xcodebuild -project IndielibTutorials.xcodeproj -scheme $TUTORIAL -configuration release
done

