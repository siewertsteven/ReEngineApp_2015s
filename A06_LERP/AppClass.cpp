#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	//starting points
	curPoint = 1;
	start = vector3(-4.0f, -2.0f, 5.0f);
	end = vector3(1.0f, -2.0f, 5.0f);
	
	//populating lerpPoints
	lerpPoints = new vector3[11];
	
	lerpPoints[0] = vector3(-4.0f, -2.0f, 5.0f);
	lerpPoints[1] = vector3(1.0f, -2.0f, 5.0f);
	lerpPoints[2] = vector3(-3.0f, -1.0f, 3.0f);
	lerpPoints[3] = vector3(2.0f, -1.0f, 3.0f);
	lerpPoints[4] = vector3(-2.0f, -0.0f, 0.0f);
	lerpPoints[5] = vector3(3.0f, 0.0f, 0.0f);;
	lerpPoints[6] = vector3(-1.0f, 1.0f, -3.0f);
	lerpPoints[7] = vector3(4.0f, 1.0f, -3.0f);
	lerpPoints[8] = vector3(0.0f, 2.0f, -5.0f);
	lerpPoints[9] = vector3(5.0f, 2.0f, -5.0f);
	lerpPoints[10] = vector3(1.0f, 3.0f, -5.0f);

	//generating spheres an their transformations
	m_pSphere = new PrimitiveClass[11];
	m_pMatrix = new matrix4[11];
	for (int i = 0; i < 11; i++)
	{
		m_pSphere[i].GenerateSphere(.1f, 5, RERED);

		m_pMatrix[i] = glm::translate(lerpPoints[i]);
	}



	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;
}

void AppClass::Update(void)
{


#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	//get the run time and map that to a percentage
	float fPercentage = MapValue(fRunTime, 0.0, static_cast<double>(fDuration), 0.0, 1.0);

	//interpolate between the start and end point using the percentage
	vector3 v3Interpolation = glm::lerp(start, end, static_cast<float>(fPercentage));
	//apply that vector to a translations
	matrix4 m4_transform = glm::translate(v3Interpolation);

	//change the start and end points when fpercentage is equal to 1 and reset the timer
	if (fPercentage >= 1 && curPoint < 10)
	{
		curPoint++;
		start = end;
		end = lerpPoints[curPoint];
		fRunTime = 0;
	}


	m_pMeshMngr->SetModelMatrix(m4_transform, "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	//rendering spheres
	for (int i = 0; i < 11; i++)
	{
		m_pSphere[i].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_pMatrix[i]);
	}
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	delete[] m_pSphere;
	delete[] m_pMatrix;
	delete[] lerpPoints;
	super::Release(); //release the memory of the inherited fields
}