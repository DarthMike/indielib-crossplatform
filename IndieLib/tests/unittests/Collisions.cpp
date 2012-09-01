
#include "dependencies/unittest++/src/UnitTest++.h"
#include "CIndieLib_vc2008.h"

TEST(pointToLineHorizontalDistanceFromMiddleIsSegment) {
	CIndieLib *iLib = CIndieLib::instance();
	IND_Vector2 a (10.0f, 10.0f);
	IND_Vector2 b (20.0f, 10.0f);
	IND_Vector2 c (15.0f, 20.0f);
	
	CHECK_CLOSE(10.0,iLib->_math->pointToLineDistance(a,b,c,true),0.1);
}

TEST(pointToLineHorizontalDistanceFromAIsSegment) {
	CIndieLib *iLib = CIndieLib::instance();
	IND_Vector2 a (10.0f, 10.0f);
	IND_Vector2 b (20.0f, 10.0f);
	IND_Vector2 c (10.0f, 20.0f);
	
	CHECK_CLOSE(10.0,iLib->_math->pointToLineDistance(a,b,c,true),0.1);
}

TEST(pointToLineHorizontaleDistanceFromBIsSegment) {
	CIndieLib *iLib = CIndieLib::instance();
	IND_Vector2 a (10.0f, 10.0f);
	IND_Vector2 b (20.0f, 10.0f);
	IND_Vector2 c (20.0f, 20.0f);
	
	CHECK_CLOSE(10.0,iLib->_math->pointToLineDistance(a,b,c,true),0.1);
}

TEST(pointToLineHorizontalDistanceFromMiddleIsSegmentInverted) {
	CIndieLib *iLib = CIndieLib::instance();
	IND_Vector2 a (20.0f, 10.0f);
	IND_Vector2 b (10.0f, 10.0f);
	IND_Vector2 c (15.0f, 20.0f);
	
	CHECK_CLOSE(10.0,iLib->_math->pointToLineDistance(a,b,c,true),0.1);
}

TEST(pointToLineHorizontalDistanceFromAIsSegmentInverted) {
	CIndieLib *iLib = CIndieLib::instance();
	IND_Vector2 a (20.0f, 10.0f);
	IND_Vector2 b (10.0f, 10.0f);
	IND_Vector2 c (15.0f, 20.0f);
	
	CHECK_CLOSE(10.0,iLib->_math->pointToLineDistance(a,b,c,true),0.1);
}

TEST(pointToLineHorizontalDistanceFromBIsSegmentInverted) {
	CIndieLib *iLib = CIndieLib::instance();
	IND_Vector2 a (20.0f, 10.0f);
	IND_Vector2 b (10.0f, 10.0f);
	IND_Vector2 c (15.0f, 20.0f);
	
	CHECK_CLOSE(10.0,iLib->_math->pointToLineDistance(a,b,c,true),0.1);
}

TEST(pointToLineDistanceFromMiddleNotSegment) {
	CIndieLib *iLib = CIndieLib::instance();
	IND_Vector2 a (10.0f, 10.0f);
	IND_Vector2 b (20.0f, 10.0f);
	IND_Vector2 c (20.0f, 20.0f);
	
	CHECK_CLOSE(10.0,iLib->_math->pointToLineDistance(a,b,c,false),0.1);
}

TEST(pointToLineDistanceFromMiddleFarRightNotSegment) {
	CIndieLib *iLib = CIndieLib::instance();
	IND_Vector2 a (10.0f, 10.0f);
	IND_Vector2 b (20.0f, 10.0f);
	IND_Vector2 c (150.0f, 20.0f);
	
	CHECK_CLOSE(10.0,iLib->_math->pointToLineDistance(a,b,c,false),0.1);
}

TEST(pointToLineDistanceFromMiddleFarLeftNotSegment) {
	CIndieLib *iLib = CIndieLib::instance();
	IND_Vector2 a (10.0f, 10.0f);
	IND_Vector2 b (20.0f, 10.0f);
	IND_Vector2 c (-150.0f, 20.0f);
	
	CHECK_CLOSE(10.0,iLib->_math->pointToLineDistance(a,b,c,false),0.1);
}

TEST(segmentIntersectioncrossedyes) {
	CIndieLib *iLib = CIndieLib::instance();
	IND_Vector2 a (10.0f, 10.0f);
	IND_Vector2 b (20.0f, 20.0f);
	IND_Vector2 c (10.0f, 20.0f);
	IND_Vector2 d (20.0f, 10.0f);

	CHECK_EQUAL(true,iLib->_math->isSegmentIntersection(a,b,c,d));

}

TEST(segmentIntersectionperpendicularyes) {
	CIndieLib *iLib = CIndieLib::instance();
	IND_Vector2 a (10.0f, 10.0f);
	IND_Vector2 b (20.0f, 10.0f);
	IND_Vector2 c (15.0f, 15.0f);
	IND_Vector2 d (15.0f, 5.0f);

	CHECK_EQUAL(true,iLib->_math->isSegmentIntersection(a,b,c,d));

}

TEST(segmentIntersectionnot) {
	CIndieLib *iLib = CIndieLib::instance();
	IND_Vector2 a (10.0f, 10.0f);
	IND_Vector2 b (20.0f, 20.0f);
	IND_Vector2 c (-10.0f, -10.0f);
	IND_Vector2 d (-20.0f, -20.0f);

	CHECK_EQUAL(false,iLib->_math->isSegmentIntersection(a,b,c,d));

}

TEST(isPointInsideTriangleInside) {
	CIndieLib *iLib = CIndieLib::instance();
	
	IND_Vector2 a = IND_Vector2(0.0f,0.0f);
	IND_Vector2 b = IND_Vector2(30.0f,0.0f);
	IND_Vector2 c = IND_Vector2(15.0f,30.0f);
	IND_Vector2 pointInside = IND_Vector2(15.0f,1.0f);

	CHECK_EQUAL(true,iLib->_math->isPointInsideTriangle(pointInside,a,b,c));
}	

TEST(isPointInsideTriangleInsideCloseVertex1) {
	CIndieLib *iLib = CIndieLib::instance();
	
	IND_Vector2 a = IND_Vector2(0.0f,0.0f);
	IND_Vector2 b = IND_Vector2(30.0f,0.0f);
	IND_Vector2 c = IND_Vector2(15.0f,30.0f);
	IND_Vector2 pointInside = IND_Vector2(29.9f,0.1f);

	CHECK_EQUAL(true,iLib->_math->isPointInsideTriangle(pointInside,a,b,c));
}	

TEST(isPointInsideTriangleInsideCloseVertex2) {
	CIndieLib *iLib = CIndieLib::instance();
	
	IND_Vector2 a = IND_Vector2(0.0f,0.0f);
	IND_Vector2 b = IND_Vector2(30.0f,0.0f);
	IND_Vector2 c = IND_Vector2(15.0f,30.0f);
	IND_Vector2 pointInside = IND_Vector2(29.9f,0.1f);

	CHECK_EQUAL(true,iLib->_math->isPointInsideTriangle(pointInside,a,b,c));
}	

TEST(isPointInsideTriangleInsideCloseVertex3) {
	CIndieLib *iLib = CIndieLib::instance();
	
	IND_Vector2 a = IND_Vector2(0.0f,0.0f);
	IND_Vector2 b = IND_Vector2(30.0f,0.0f);
	IND_Vector2 c = IND_Vector2(15.0f,30.0f);
	IND_Vector2 pointInside = IND_Vector2(0.1f,0.1f);

	CHECK_EQUAL(true,iLib->_math->isPointInsideTriangle(pointInside,a,b,c));
}	

TEST(isPointInsideTriangleOutsideRightFar) {
	CIndieLib *iLib = CIndieLib::instance();
	
	IND_Vector2 a = IND_Vector2(0.0f,0.0f);
	IND_Vector2 b = IND_Vector2(30.0f,0.0f);
	IND_Vector2 c = IND_Vector2(15.0f,30.0f);
	IND_Vector2 pointInside = IND_Vector2(150.0f,1.0f);

	CHECK_EQUAL(false,iLib->_math->isPointInsideTriangle(pointInside,a,b,c));
}

TEST(isPointInsideTriangleOutsideRightClose) {
	CIndieLib *iLib = CIndieLib::instance();
	
	IND_Vector2 a = IND_Vector2(0.0f,0.0f);
	IND_Vector2 b = IND_Vector2(30.0f,0.0f);
	IND_Vector2 c = IND_Vector2(15.0f,30.0f);
	IND_Vector2 pointInside = IND_Vector2(30.1f,0.0f);

	CHECK_EQUAL(false,iLib->_math->isPointInsideTriangle(pointInside,a,b,c));
}

TEST(isPointInsideTriangleOutsideTopFar) {
	CIndieLib *iLib = CIndieLib::instance();
	
	IND_Vector2 a = IND_Vector2(0.0f,0.0f);
	IND_Vector2 b = IND_Vector2(30.0f,0.0f);
	IND_Vector2 c = IND_Vector2(15.0f,30.0f);
	IND_Vector2 pointInside = IND_Vector2(15.0f,150.0f);

	CHECK_EQUAL(false,iLib->_math->isPointInsideTriangle(pointInside,a,b,c));
}

TEST(isPointInsideTriangleOutsideTopClose) {
	CIndieLib *iLib = CIndieLib::instance();
	
	IND_Vector2 a = IND_Vector2(0.0f,0.0f);
	IND_Vector2 b = IND_Vector2(30.0f,0.0f);
	IND_Vector2 c = IND_Vector2(15.0f,30.0f);
	IND_Vector2 pointInside = IND_Vector2(15.0f,30.1f);

	CHECK_EQUAL(false,iLib->_math->isPointInsideTriangle(pointInside,a,b,c));
}

TEST(isPointInsideTriangleOutsideLeftFar) {
	CIndieLib *iLib = CIndieLib::instance();
	
	IND_Vector2 a = IND_Vector2(0.0f,0.0f);
	IND_Vector2 b = IND_Vector2(30.0f,0.0f);
	IND_Vector2 c = IND_Vector2(15.0f,30.0f);
	IND_Vector2 pointInside = IND_Vector2(-15.0f,0.0f);

	CHECK_EQUAL(false,iLib->_math->isPointInsideTriangle(pointInside,a,b,c));
}

TEST(isPointInsideTriangleOutsideLeftClose) {
	CIndieLib *iLib = CIndieLib::instance();
	
	IND_Vector2 a = IND_Vector2(0.0f,0.0f);
	IND_Vector2 b = IND_Vector2(30.0f,0.0f);
	IND_Vector2 c = IND_Vector2(15.0f,30.0f);
	IND_Vector2 pointInside = IND_Vector2(-0.1f,0.0f);

	CHECK_EQUAL(false,iLib->_math->isPointInsideTriangle(pointInside,a,b,c));
}