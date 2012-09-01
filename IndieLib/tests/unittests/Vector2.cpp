
#include "dependencies/unittest++/src/UnitTest++.h"
#include "CIndieLib_vc2008.h"

TEST(LengthSimple) {
	IND_Vector2 simple (1.0f,0.0f);
	CHECK_CLOSE(1.0f,simple.length(),0.01f);
}

TEST(LengthCompose) {
	IND_Vector2 composed (3.0f,10.0f);
	CHECK_CLOSE(10.44f,composed.length(),0.01f);
}

TEST(DotProductNulled) {
	IND_Vector2 a(1.0f,0.0f);
	IND_Vector2 b(0.0f,1.0f);
	CHECK_CLOSE(0.0f,a.dotProduct(b),0.01f);
}

TEST(DotProductSimple) {
	IND_Vector2 a(1.0f,3.0f);
	IND_Vector2 b(2.0f,1.0f);
	CHECK_CLOSE(5.0f,a.dotProduct(b),0.01f);
}

TEST(DotProductCompose) {
	IND_Vector2 a(0.5f,2.5f);
	IND_Vector2 b(2.0f,3.0f);
	CHECK_CLOSE(8.5f,a.dotProduct(b),0.01f);
}

TEST(CrossProductNulled) {
	IND_Vector2 a(1.0f,0.0f);
	IND_Vector2 b(1.0f,0.0f);
	CHECK_CLOSE(0.0f,a.crossProduct(b),0.01f);
}

TEST(CrossProductSimple) {
	IND_Vector2 a(1.0f,3.0f);
	IND_Vector2 b(2.0f,1.0f);
	CHECK_CLOSE(-5.0f,a.crossProduct(b),0.01f);
}

TEST(CrossProductCompose) {
	IND_Vector2 a(0.5f,2.5f);
	IND_Vector2 b(2.0f,3.0f);
	CHECK_CLOSE(-3.5f,a.crossProduct(b),0.01f);
}

TEST(NormalizeZero) {
	IND_Vector2 zero (0.0f,0.0f);
	zero.normalise();
	CHECK_CLOSE(0.0f,zero._x,0.01f);
	CHECK_CLOSE(0.0f,zero._y,0.01f);
}

TEST(NormalizeSimple) {
	IND_Vector2 simple (2.0f,0.0f);
	simple.normalise();
	CHECK_CLOSE(1.0f,simple._x,0.01f);
	CHECK_CLOSE(0.0f,simple._y,0.01f);
}