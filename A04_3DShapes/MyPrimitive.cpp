#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}

void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 top(0.0f, a_fHeight, 0.0f);
	vector3 bottom(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int j = i + 1;
		vector3 point1(cosf((i*2*pi)/ a_nSubdivisions) * a_fRadius,0.0f, sinf((i * 2 * pi) / a_nSubdivisions) * a_fRadius);
		vector3 point2(cosf((j * 2 * pi) / a_nSubdivisions) * a_fRadius, 0.0f, sinf((j * 2 * pi) / a_nSubdivisions) * a_fRadius);
		AddQuad(point1, top, point2, point1);
		AddQuad(point1, bottom, point2, point1);
	}

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	float halfheight = a_fHeight / 2;
	//3--2
	//|  |
	//0--1
	vector3 top(0.0f, halfheight, 0.0f);
	vector3 bottom(0.0f, -halfheight, 0.0f);
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int j = i + 1;
		
		vector3 point1(cosf((i * 2 * pi) / a_nSubdivisions) * a_fRadius, halfheight, sinf((i * 2 * pi) / a_nSubdivisions) * a_fRadius);
		vector3 point2(cosf((j * 2 * pi) / a_nSubdivisions) * a_fRadius, halfheight, sinf((j * 2 * pi) / a_nSubdivisions) * a_fRadius);
		vector3 point3(cosf((i * 2 * pi) / a_nSubdivisions) * a_fRadius, -halfheight, sinf((i * 2 * pi) / a_nSubdivisions) * a_fRadius);
		vector3 point4(cosf((j * 2 * pi) / a_nSubdivisions) * a_fRadius, -halfheight, sinf((j * 2 * pi) / a_nSubdivisions) * a_fRadius);
		//top
		AddQuad(point1, top, point2, point1);
		//side
		AddQuad(point1, point2, point3, point4);
		//bottom
		AddQuad(point3, bottom, point4, point3);

	}
	

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	float halfheight = a_fHeight / 2;
	//3--2
	//|  |
	//0--1
	vector3 top(0.0f, halfheight, 0.0f);
	vector3 bottom(0.0f, -halfheight, 0.0f);
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int j = i + 1;
		//outer top
		vector3 point1(cosf((i * 2 * pi) / a_nSubdivisions) * a_fOuterRadius, halfheight, sinf((i * 2 * pi) / a_nSubdivisions) * a_fOuterRadius);
		vector3 point2(cosf((j * 2 * pi) / a_nSubdivisions) * a_fOuterRadius, halfheight, sinf((j * 2 * pi) / a_nSubdivisions) * a_fOuterRadius);
		//outer bottom
		vector3 point3(cosf((i * 2 * pi) / a_nSubdivisions) * a_fOuterRadius, -halfheight, sinf((i * 2 * pi) / a_nSubdivisions) * a_fOuterRadius);
		vector3 point4(cosf((j * 2 * pi) / a_nSubdivisions) * a_fOuterRadius, -halfheight, sinf((j * 2 * pi) / a_nSubdivisions) * a_fOuterRadius);
		//inner top
		vector3 point1i(cosf((i * 2 * pi) / a_nSubdivisions) * a_fInnerRadius, halfheight, sinf((i * 2 * pi) / a_nSubdivisions) * a_fInnerRadius);
		vector3 point2i(cosf((j * 2 * pi) / a_nSubdivisions) * a_fInnerRadius, halfheight, sinf((j * 2 * pi) / a_nSubdivisions) * a_fInnerRadius);
		//inner bottom
		vector3 point3i(cosf((i * 2 * pi) / a_nSubdivisions) * a_fInnerRadius, -halfheight, sinf((i * 2 * pi) / a_nSubdivisions) * a_fInnerRadius);
		vector3 point4i(cosf((j * 2 * pi) / a_nSubdivisions) * a_fInnerRadius, -halfheight, sinf((j * 2 * pi) / a_nSubdivisions) * a_fInnerRadius);
		
		
		//top 
		AddQuad(point2i, point2,  point1i, point1);
		//bottom
		AddQuad( point3i, point3, point4i, point4);
		//innerwalls
		AddQuad(point4i, point2i, point3i, point1i);
		//outerwalls
		AddQuad(point1, point2, point3, point4);

	}


	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	AddQuad(point0, point1, point3, point2);

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 12)
		a_nSubdivisions = 12;

	Release();
	Init();
	float halfradius = a_fRadius / 2;
	//3--2
	//|  |
	//0--1
	vector3 top(0.0f, halfradius, 0.0f);
	vector3 bottom(0.0f, halfradius, 0.0f);
	//radius were working with
	float crad;
	float height;
	//rings
	for (int r = 0; r < a_nSubdivisions; r++)
	{
		
		int	s = r + 1;
		for (int i = 0; i < a_nSubdivisions ; i++)
		{
			height = cos((r * 1.0 / a_nSubdivisions) * pi) * a_fRadius;
			//top ring
			int j = i + 1;
			crad = sin((r*1.0 / a_nSubdivisions) * pi) * a_fRadius;
			
			vector3 point1(cosf((i * 2 * pi) / a_nSubdivisions) * crad, height, sinf((i * 2 * pi) / a_nSubdivisions) * crad);
			vector3 point2(cosf((j * 2 * pi) / a_nSubdivisions) * crad, height, sinf((j * 2 * pi) / a_nSubdivisions) * crad);

			//bottom ring
			height = cos((s * 1.0 / a_nSubdivisions) * pi) * a_fRadius;
			crad = sin((s*1.0 / a_nSubdivisions) * pi) * a_fRadius;
			vector3 point3(cosf((i * 2 * pi) / a_nSubdivisions) * crad, height, sinf((i * 2 * pi) / a_nSubdivisions) * crad);
			vector3 point4(cosf((j * 2 * pi) / a_nSubdivisions) * crad, height, sinf((j * 2 * pi) / a_nSubdivisions) * crad);

			AddQuad(point1, point2, point3, point4);

		}

	}

	//Your code ends here
	CompileObject(a_v3Color);
}