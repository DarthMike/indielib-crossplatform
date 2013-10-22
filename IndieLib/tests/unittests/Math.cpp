/*********************************** The zlib License ************************************
 *
 * Copyright (c) 2013 Indielib-crossplatform Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 *****************************************************************************************/
#include "dependencies/unittest++/src/UnitTest++.h"
#include "CIndieLib.h"


/*
    // ----- Matrix utilities -----
    inline void Matrix4DMultiply(IND_Matrix& m1, IND_Matrix& m2, IND_Matrix& result);
    inline void Matrix4DSetIdentity(IND_Matrix& m);
    inline void Matrix4DSetTranslation(IND_Matrix& m, float transx, float transy, float transz);
    inline void Matrix4DSetRotationAroundAxis(IND_Matrix& m, float angledegrees, IND_Vector3& axis);
    inline void Matrix4DSetScale(IND_Matrix& m, float scalex, float scaley, float scalez);
    inline void Matrix4DLookAtMatrix(IND_Vector3& r, IND_Vector3& u, IND_Vector3& l, IND_Vector3& pos, IND_Matrix& result);

    // ----- Matrix4/Vector3 utilities -----
    inline void TransformVector3DbyMatrix4D(IND_Vector3& vector, IND_Matrix& mat);
*/

struct INDMathTests
{
    INDMathTests() { math = new IND_Math(); }
    ~INDMathTests() { DISPOSE(math); }
    
    IND_Math* math;
};

TEST_FIXTURE(INDMathTests,MatrixMultiplyIdentity) {
	IND_Matrix m1;
	IND_Matrix m2;
	IND_Matrix result;

	//Construct first matrix
	math->matrix4DSetIdentity(m1);
	//Construct second matrix
	m2._11 = 11.0f;
	m2._12 = 12.0f;
	m2._13 = 12.0f;
	m2._14 = 14.0f;
	m2._21 = 21.0f;
	m2._22 = 22.0f;
	m2._23 = 22.0f;
	m2._24 = 24.0f;
	m2._31 = 31.0f;
	m2._32 = 32.0f;
	m2._33 = 32.0f;
	m2._34 = 34.0f;
	m2._41 = 41.0f;
	m2._42 = 42.0f;
	m2._43 = 42.0f;
	m2._44 = 44.0f;

	//Perform matrix multiplication
	math->matrix4DMultiply(m1, m2, result);

	//Checkings
	CHECK_CLOSE(m2._11, result._11, 0.01f);
	CHECK_CLOSE(m2._12, result._12, 0.01f);
	CHECK_CLOSE(m2._13, result._13, 0.01f);
	CHECK_CLOSE(m2._14, result._14, 0.01f);

	CHECK_CLOSE(m2._21, result._21, 0.01f);
	CHECK_CLOSE(m2._22, result._22, 0.01f);
	CHECK_CLOSE(m2._23, result._23, 0.01f);
	CHECK_CLOSE(m2._24, result._24, 0.01f);

	CHECK_CLOSE(m2._31, result._31, 0.01f);
	CHECK_CLOSE(m2._32, result._32, 0.01f);
	CHECK_CLOSE(m2._33, result._33, 0.01f);
	CHECK_CLOSE(m2._34, result._34, 0.01f);

	CHECK_CLOSE(m2._41, result._41, 0.01f);
	CHECK_CLOSE(m2._42, result._42, 0.01f);
	CHECK_CLOSE(m2._43, result._43, 0.01f);
	CHECK_CLOSE(m2._44, result._44, 0.01f);
}

TEST_FIXTURE(INDMathTests,MatrixMultiply) {
	IND_Matrix m1;
	IND_Matrix m2;
	IND_Matrix result;

	//Construct first matrix
	m1._11 = 1.0f;
	m1._12 = 2.0f;
	m1._13 = 3.0f;
	m1._14 = 4.0f;
	m1._21 = 5.0f;
	m1._22 = 6.0f;
	m1._23 = 7.0f;
	m1._24 = 8.0f;
	m1._31 = 9.0f;
	m1._32 = 10.0f;
	m1._33 = 11.0f;
	m1._34 = 12.0f;
	m1._41 = 13.0f;
	m1._42 = 14.0f;
	m1._43 = 15.0f;
	m1._44 = 16.0f;
	//Construct second matrix
	m2._11 = 1.0f;
	m2._12 = 2.0f;
	m2._13 = 3.0f;
	m2._14 = 4.0f;
	m2._21 = 5.0f;
	m2._22 = 6.0f;
	m2._23 = 7.0f;
	m2._24 = 8.0f;
	m2._31 = 9.0f;
	m2._32 = 10.0f;
	m2._33 = 11.0f;
	m2._34 = 12.0f;
	m2._41 = 13.0f;
	m2._42 = 14.0f;
	m2._43 = 15.0f;
	m2._44 = 16.0f;

	//Perform matrix multiplication
	math->matrix4DMultiply(m1, m2, result);

	//Checkings
	CHECK_CLOSE(90.0f, result._11, 0.01f);
	CHECK_CLOSE(100.0f, result._12, 0.01f);
	CHECK_CLOSE(110.0f, result._13, 0.01f);
	CHECK_CLOSE(120.0f, result._14, 0.01f);

	CHECK_CLOSE(202.0f, result._21, 0.01f);
	CHECK_CLOSE(228.0f, result._22, 0.01f);
	CHECK_CLOSE(254.0f, result._23, 0.01f);
	CHECK_CLOSE(280.0f, result._24, 0.01f);

	CHECK_CLOSE(314.0f, result._31, 0.01f);
	CHECK_CLOSE(356.0f, result._32, 0.01f);
	CHECK_CLOSE(398.0f, result._33, 0.01f);
	CHECK_CLOSE(440.0f, result._34, 0.01f);

	CHECK_CLOSE(426.0f, result._41, 0.01f);
	CHECK_CLOSE(484.0f, result._42, 0.01f);
	CHECK_CLOSE(542.0f, result._43, 0.01f);
	CHECK_CLOSE(600.0f, result._44, 0.01f);
}

TEST_FIXTURE(INDMathTests,MatrixMultiplyInPlace) {
	IND_Matrix m1;
	IND_Matrix m2;
    
	//Construct first matrix
	m1._11 = 1.0f;
	m1._12 = 2.0f;
	m1._13 = 3.0f;
	m1._14 = 4.0f;
	m1._21 = 5.0f;
	m1._22 = 6.0f;
	m1._23 = 7.0f;
	m1._24 = 8.0f;
	m1._31 = 9.0f;
	m1._32 = 10.0f;
	m1._33 = 11.0f;
	m1._34 = 12.0f;
	m1._41 = 13.0f;
	m1._42 = 14.0f;
	m1._43 = 15.0f;
	m1._44 = 16.0f;
	//Construct second matrix
	m2._11 = 1.0f;
	m2._12 = 2.0f;
	m2._13 = 3.0f;
	m2._14 = 4.0f;
	m2._21 = 5.0f;
	m2._22 = 6.0f;
	m2._23 = 7.0f;
	m2._24 = 8.0f;
	m2._31 = 9.0f;
	m2._32 = 10.0f;
	m2._33 = 11.0f;
	m2._34 = 12.0f;
	m2._41 = 13.0f;
	m2._42 = 14.0f;
	m2._43 = 15.0f;
	m2._44 = 16.0f;
    
	//Perform matrix multiplication
	math->matrix4DMultiplyInPlace(m1, m2);
    
	//Checkings
	CHECK_CLOSE(90.0f, m1._11, 0.01f);
	CHECK_CLOSE(100.0f, m1._12, 0.01f);
	CHECK_CLOSE(110.0f, m1._13, 0.01f);
	CHECK_CLOSE(120.0f, m1._14, 0.01f);
    
	CHECK_CLOSE(202.0f, m1._21, 0.01f);
	CHECK_CLOSE(228.0f, m1._22, 0.01f);
	CHECK_CLOSE(254.0f, m1._23, 0.01f);
	CHECK_CLOSE(280.0f, m1._24, 0.01f);
    
	CHECK_CLOSE(314.0f, m1._31, 0.01f);
	CHECK_CLOSE(356.0f, m1._32, 0.01f);
	CHECK_CLOSE(398.0f, m1._33, 0.01f);
	CHECK_CLOSE(440.0f, m1._34, 0.01f);
    
	CHECK_CLOSE(426.0f, m1._41, 0.01f);
	CHECK_CLOSE(484.0f, m1._42, 0.01f);
	CHECK_CLOSE(542.0f, m1._43, 0.01f);
	CHECK_CLOSE(600.0f, m1._44, 0.01f);
}

TEST_FIXTURE(INDMathTests,TranslateVector) {
	//Translation matrix
	IND_Matrix mtrans;
	math->matrix4DSetTranslation(mtrans, 10.0f, 10.0f, 10.0f);
	//Vector to translate
	IND_Vector3 vec(1.0f, 1.0f, 1.0f);
	//Translation
	math->transformVector3DbyMatrix4D(vec, mtrans);

	CHECK_CLOSE(11.0f, vec._x, 0.01f);
	CHECK_CLOSE(11.0f, vec._y, 0.01f);
	CHECK_CLOSE(11.0f, vec._z, 0.01f);
}

TEST_FIXTURE(INDMathTests,TranslateVectorX) {
	//Translation matrix
	IND_Matrix mtrans;
	math->matrix4DSetTranslation(mtrans, 10.0f, 0.0f, 0.0f);
	//Vector to translate
	IND_Vector3 vec(1.0f, 1.0f, 1.0f);
	//Translation
	math->transformVector3DbyMatrix4D(vec, mtrans);

	CHECK_CLOSE(11.0f, vec._x, 0.01f);
	CHECK_CLOSE(1.0f, vec._y, 0.01f);
	CHECK_CLOSE(1.0f, vec._z, 0.01f);
}

TEST_FIXTURE(INDMathTests,TranslateVectorY) {
	//Translation matrix
	IND_Matrix mtrans;
	math->matrix4DSetTranslation(mtrans, 0.0f, 10.0f, 0.0f);
	//Vector to translate
	IND_Vector3 vec(1.0f, 1.0f, 1.0f);
	//Translation
	math->transformVector3DbyMatrix4D(vec, mtrans);

	CHECK_CLOSE(1.0f, vec._x, 0.01f);
	CHECK_CLOSE(11.0f, vec._y, 0.01f);
	CHECK_CLOSE(1.0f, vec._z, 0.01f);
}

TEST_FIXTURE(INDMathTests,TranslateVectorZ) {
	//Translation matrix
	IND_Matrix mtrans;
	math->matrix4DSetTranslation(mtrans, 0.0f, 0.0f, 10.0f);
	//Vector to translate
	IND_Vector3 vec(1.0f, 1.0f, 1.0f);
	//Translation
	math->transformVector3DbyMatrix4D(vec, mtrans);

	CHECK_CLOSE(1.0f, vec._x, 0.01f);
	CHECK_CLOSE(1.0f, vec._y, 0.01f);
	CHECK_CLOSE(11.0f, vec._z, 0.01f);
}

TEST_FIXTURE(INDMathTests,RotateVector) {
	//Rotation matrix
	IND_Matrix mrot;
	//Vector to rotate
	IND_Vector3 vec;

    //------------Rotation around Z axis-------------------------------
	//90 degress around z axis (x axis vector)
	vec = IND_Vector3(1.0f, 0.0f, 0.0f);
    math->matrix4DSetRotationAroundAxis(mrot, 90.f, IND_Vector3(0.0f, 0.0f, 1.0f));
	
    //Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(0.0f, vec._x, 0.01f);
	CHECK_CLOSE(1.0f, vec._y, 0.01f);
	CHECK_CLOSE(0.0f, vec._z, 0.01f);

    //45 degress around z axis (x axis vector)
	vec = IND_Vector3(1.0f, 0.0f, 0.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 45.0f, IND_Vector3(0.0f, 0.0f, 1.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(0.7f, vec._x, 0.01f);
	CHECK_CLOSE(0.7f, vec._y, 0.01f);
	CHECK_CLOSE(0.0f, vec._z, 0.01f);

	//90 degress around z axis (y axis vector)
	vec = IND_Vector3(0.0f, 1.0f, 0.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 90.f, IND_Vector3(0.0f, 0.0f, 1.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(-1.0f, vec._x, 0.01f);
	CHECK_CLOSE(0.0f, vec._y, 0.01f);
	CHECK_CLOSE(0.0f, vec._z, 0.01f);

    //45 degress around z axis (y axis vector)
	vec = IND_Vector3(0.0f, 1.0f, 0.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 45.0f, IND_Vector3(0.0f, 0.0f, 1.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(-0.7f, vec._x, 0.01f);
	CHECK_CLOSE(0.7f, vec._y, 0.01f);
	CHECK_CLOSE(0.0f, vec._z, 0.01f);

	//90 degress around z axis (z axis vector)
	vec = IND_Vector3(0.0f, 0.0f, 1.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 90.f, IND_Vector3(0.0f, 0.0f, 1.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(0.0f, vec._x, 0.01f);
	CHECK_CLOSE(0.0f, vec._y, 0.01f);
	CHECK_CLOSE(1.0f, vec._z, 0.01f);

    //45 degress around z axis (z axis vector)
	vec = IND_Vector3(0.0f, 0.0f, 1.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 45.0f, IND_Vector3(0.0f, 0.0f, 1.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(0.0f, vec._x, 0.01f);
	CHECK_CLOSE(0.0f, vec._y, 0.01f);
	CHECK_CLOSE(1.0f, vec._z, 0.01f);

    //------------Rotation around Y axis-------------------------------
	//90 degress around y axis (x axis vector)
	vec = IND_Vector3(1.0f, 0.0f, 0.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 90.f, IND_Vector3(0.0f, 1.0f, 0.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(0.0f, vec._x, 0.01f);
	CHECK_CLOSE(0.0f, vec._y, 0.01f);
	CHECK_CLOSE(-1.0f, vec._z, 0.01f);

    //45 degress around y axis (x axis vector)
	vec = IND_Vector3(1.0f, 0.0f, 0.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 45.0f, IND_Vector3(0.0f, 1.0f, 0.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(0.7f, vec._x, 0.01f);
	CHECK_CLOSE(0.0f, vec._y, 0.01f);
	CHECK_CLOSE(-0.7f, vec._z, 0.01f);

	//90 degress around y axis (y axis vector)
	vec = IND_Vector3(0.0f, 1.0f, 0.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 90.f, IND_Vector3(0.0f, 1.0f, 0.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(0.0f, vec._x, 0.01f);
	CHECK_CLOSE(1.0f, vec._y, 0.01f);
	CHECK_CLOSE(0.0f, vec._z, 0.01f);

    //45 degress around y axis (y axis vector)
	vec = IND_Vector3(0.0f, 1.0f, 0.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 45.0f, IND_Vector3(0.0f, 1.0f, 0.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(0.0f, vec._x, 0.01f);
	CHECK_CLOSE(1.0f, vec._y, 0.01f);
	CHECK_CLOSE(0.0f, vec._z, 0.01f);


    //90 degress around y axis (z axis vector)
	vec = IND_Vector3(0.0f, 0.0f, 1.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 90.f, IND_Vector3(0.0f, 1.0f, 0.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(1.0f, vec._x, 0.01f);
	CHECK_CLOSE(0.0f, vec._y, 0.01f);
	CHECK_CLOSE(0.0f, vec._z, 0.01f);

    //45 degress around y axis (z axis vector)
	vec = IND_Vector3(0.0f, 0.0f, 1.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 45.0f, IND_Vector3(0.0f, 1.0f, 0.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(0.7f, vec._x, 0.01f);
	CHECK_CLOSE(0.0f, vec._y, 0.01f);
	CHECK_CLOSE(0.7f, vec._z, 0.01f);

    //------------Rotation around X axis-------------------------------
    //90 degress around x axis (x axis vector)
	vec = IND_Vector3(1.0f, 0.0f, 0.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 90.f, IND_Vector3(1.0f, 0.0f, 0.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(1.0f, vec._x, 0.01f);
	CHECK_CLOSE(0.0f, vec._y, 0.01f);
	CHECK_CLOSE(0.0f, vec._z, 0.01f);

    //45 degress around x axis (x axis vector)
	vec = IND_Vector3(1.0f, 0.0f, 0.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 45.0f, IND_Vector3(1.0f, 0.0f, 0.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(1.0f, vec._x, 0.01f);
	CHECK_CLOSE(0.0f, vec._y, 0.01f);
	CHECK_CLOSE(0.0f, vec._z, 0.01f);

	//90 degress around x axis (y axis vector)
	vec = IND_Vector3(0.0f, 1.0f, 0.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 90.f, IND_Vector3(1.0f, 0.0f, 0.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(0.0f, vec._x, 0.01f);
	CHECK_CLOSE(0.0f, vec._y, 0.01f);
	CHECK_CLOSE(1.0f, vec._z, 0.01f);

    //45 degress around x axis (y axis vector)
	vec = IND_Vector3(0.0f, 1.0f, 0.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 45.0f, IND_Vector3(1.0f, 0.0f, 0.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(0.0f, vec._x, 0.01f);
	CHECK_CLOSE(0.7f, vec._y, 0.01f);
	CHECK_CLOSE(0.7f, vec._z, 0.01f);
	
    //90 degress around x axis (z axis vector)
	vec = IND_Vector3(0.0f, 0.0f, 1.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 90.f, IND_Vector3(1.0f, 0.0f, 0.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(0.0f, vec._x, 0.01f);
	CHECK_CLOSE(-1.0f, vec._y, 0.01f);
	CHECK_CLOSE(0.0f, vec._z, 0.01f);

    //45 degress around x axis (z axis vector)
	vec = IND_Vector3(0.0f, 0.0f, 1.0f);
	math->matrix4DSetRotationAroundAxis(mrot, 45.0f, IND_Vector3(1.0f, 0.0f, 0.0f));

	//Rotation
	math->transformVector3DbyMatrix4D(vec, mrot);

	CHECK_CLOSE(0.0f, vec._x, 0.01f);
	CHECK_CLOSE(-0.7f, vec._y, 0.01f);
	CHECK_CLOSE(0.7f, vec._z, 0.01f);
}

TEST_FIXTURE(INDMathTests,ScaleVector) {
	//Scale matrix
	IND_Matrix mscale;
	//Vector to scale
	IND_Vector3 vec(1.0f, 1.0f, 1.0f);

	math->matrix4DSetScale(mscale, 10.f, 11.0f, 12.0f);
	math->transformVector3DbyMatrix4D(vec, mscale);

	CHECK_CLOSE(10.0f, vec._x, 0.01f);
	CHECK_CLOSE(11.0f, vec._y, 0.01f);
	CHECK_CLOSE(12.0f, vec._z, 0.01f);
}

TEST_FIXTURE(INDMathTests,LookAtMatrixLH) {
	//Result
	IND_Matrix result;
	IND_Vector3 up(0.0f, 1.0f, 0.0f);
	IND_Vector3 right(1.0f, 0.0f, 0.0f);
	IND_Vector3 look(0.0f, 0.0f, -1.0f);
	IND_Vector3 pos(1.0f, 2.0f, 3.0f);

	math->matrix4DLookAtMatrixLH(right, up, look, pos, result);

	CHECK_CLOSE(right._x, result._11, 0.01f);
	CHECK_CLOSE(right._y, result._12, 0.01f);
	CHECK_CLOSE(right._z, result._13, 0.01f);
	CHECK_CLOSE(0.0f, result._41, 0.01f);

	CHECK_CLOSE(up._x, result._21, 0.01f);
	CHECK_CLOSE(up._y, result._22, 0.01f);
	CHECK_CLOSE(up._z, result._23, 0.01f);
	CHECK_CLOSE(0.0f, result._42, 0.01f);

	CHECK_CLOSE(look._x, result._31, 0.01f);
	CHECK_CLOSE(look._y, result._32, 0.01f);
	CHECK_CLOSE(look._z, result._33, 0.01f);
	CHECK_CLOSE(0.0f, result._43, 0.01f);

	CHECK_CLOSE(-pos.dotProduct(right), result._14, 0.01f);
	CHECK_CLOSE(-pos.dotProduct(up), result._24, 0.01f);
	CHECK_CLOSE(-pos.dotProduct(look), result._34, 0.01f);
	CHECK_CLOSE(1.0f, result._44, 0.01f);
}

TEST_FIXTURE(INDMathTests,LookAtMatrixEyeLookUpLH) {
	//Result
	IND_Matrix result;
	IND_Vector3 eye(1.0f, 2.0f, 3.0f);
	IND_Vector3 look(1.0f, 2.0f, 2.0f);  //Just on z coord lower
	IND_Vector3 up(0.0f, -1.0f, 0.0f);

    math->matrix4DLookAtMatrixEyeLookUpLH(eye,look,up,result);

	IND_Vector3 x(1.0f, 0.0f, 0.0f);
    IND_Vector3 y(0.0f, -1.0f, 0.0f);
	IND_Vector3 z(0.0f, 0.0f, -1.0f);

	CHECK_CLOSE(x._x, result._11, 0.01f);
	CHECK_CLOSE(x._y, result._12, 0.01f);
	CHECK_CLOSE(x._z, result._13, 0.01f);
	CHECK_CLOSE(0.0f, result._41, 0.01f);

	CHECK_CLOSE(y._x, result._21, 0.01f);
	CHECK_CLOSE(y._y, result._22, 0.01f);
	CHECK_CLOSE(y._z, result._23, 0.01f);
	CHECK_CLOSE(0.0f, result._42, 0.01f);

	CHECK_CLOSE(z._x, result._31, 0.01f);
	CHECK_CLOSE(z._y, result._32, 0.01f);
	CHECK_CLOSE(z._z, result._33, 0.01f);
	CHECK_CLOSE(0.0f, result._43, 0.01f);

	CHECK_CLOSE(-x.dotProduct(eye), result._14, 0.01f);
	CHECK_CLOSE(-y.dotProduct(eye), result._24, 0.01f);
	CHECK_CLOSE(-z.dotProduct(eye), result._34, 0.01f);
	CHECK_CLOSE(1.0f, result._44, 0.01f);
}

TEST_FIXTURE(INDMathTests,idendityMatrix) {
    IND_Matrix matrix = IND_Matrix::identity();
    
    CHECK_CLOSE(1.f, matrix._11, 0.01f);
    CHECK_CLOSE(0.f, matrix._12, 0.01f);
    CHECK_CLOSE(0.f, matrix._13, 0.01f);
    CHECK_CLOSE(0.f, matrix._14, 0.01f);
    CHECK_CLOSE(0.f, matrix._21, 0.01f);
    CHECK_CLOSE(1.f, matrix._22, 0.01f);
    CHECK_CLOSE(0.f, matrix._23, 0.01f);
    CHECK_CLOSE(0.f, matrix._24, 0.01f);
    CHECK_CLOSE(0.f, matrix._31, 0.01f);
    CHECK_CLOSE(0.f, matrix._32, 0.01f);
    CHECK_CLOSE(1.f, matrix._33, 0.01f);
    CHECK_CLOSE(0.f, matrix._34, 0.01f);
    CHECK_CLOSE(0.f, matrix._41, 0.01f);
    CHECK_CLOSE(0.f, matrix._42, 0.01f);
    CHECK_CLOSE(0.f, matrix._43, 0.01f);
    CHECK_CLOSE(1.f, matrix._44, 0.01f);
}
