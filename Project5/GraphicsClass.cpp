#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_Direct3D = 0;
	m_camera = 0;
	m_colour_shader = 0;
	m_model = 0;
	m_texture_shader = 0;
	m_light = 0;
	m_light_shader = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
	
}

GraphicsClass::~GraphicsClass()
{
	
}

bool GraphicsClass::Init(int s_width, int s_height, HWND hwnd)
{
	//create D3DClass object, call init function
	bool result;
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	result = m_Direct3D->Init(s_width, s_height, VSYNC_ENABLED, hwnd, FULL_SCREEN,
		S_DEPTH, S_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Direct3D could not be initialised.", L"Error!", MB_OK);
		return false;
	}

	m_camera = new CameraClass;
	if (!m_camera)
	{
		return false;
	}
	//set initial position
	m_camera->setPosition(0.0f, 0.0f, -5.0f);

	m_colour_shader = new ColourShaderClass;
	if (!m_colour_shader)
	{
		return false;
	}

	result = m_colour_shader->Init(m_Direct3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Cannot initialize colour shader object.", L"Error!", MB_OK);
		return false;
	}

	m_model = new ModelClass;
	if (!m_model)
	{
		return false;
	}

	result = m_model->Init(m_Direct3D->getDevice(), m_Direct3D->getDeviceContext(), (char*)"stonetex.tga", (char*)"cube.txt");
	if (!result)
	{
		MessageBox(hwnd, L"Cannot initialize model object.", L"Error!", MB_OK);
		return false;
	}

	//create texture shader
	m_texture_shader = new TextureShaderClass;
	if (!m_texture_shader)
	{
		return false;
	}

	//init
	result = m_texture_shader->Init(m_Direct3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize texture shader object.", L"Error", MB_OK);
		return false;
	}


	//create and init light shader object
	m_light_shader = new LightShaderClass;
	if (!m_light_shader)
	{
		return false;
	}
	
	result = m_light_shader->Init(m_Direct3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Cannot initialize light shader object.", L"Error", MB_OK);
		return false;
	}

	//create and init light object, set colour to purple and direction to point down +z axis
	m_light = new LightClass;
	if (!m_light)
	{
		return false;
	}
	m_light->setDiffuseColour(0.52734375f, 0.8046875f, 0.91796875f, 0.0f);
	m_light->setDirection(0.0f, 0.0f, 1.0f);
	return true;
}

void GraphicsClass::Exit()
{
	if (m_light_shader)
	{
		m_light_shader->Exit();
		delete m_light_shader;
		m_light_shader = 0;
	}

	if (m_light)
	{
		delete m_light;
		m_light = 0;
	}

	if (m_texture_shader)
	{
		m_texture_shader->Exit();
		delete m_texture_shader;
		m_texture_shader = 0;
	}

	if (m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}

	if (m_colour_shader)
	{
		m_colour_shader->Exit();
		delete m_colour_shader;
		m_colour_shader = 0;
	}

	if (m_model)
	{
		m_model->Exit();
		delete m_model;
		m_model = 0;
	}

	if (m_Direct3D)
	{
		m_Direct3D->Exit();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
	return;
}

bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;

	//update rotation variable
	rotation += (float)XM_PI * 0.01f;

	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render(float rotation)
{
	XMMATRIX world_matrix;
	XMMATRIX view_matrix;
	XMMATRIX projection_matrix;

	bool result;
	m_Direct3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);
	
	//generate view matrix from position
	m_camera->render();

	//get matrixes from camera and D3D objects
	m_camera->getViewMatrix(view_matrix);
	m_Direct3D->getWorldMatrix(world_matrix);
	m_Direct3D->getProjectionMatrix(projection_matrix);

	//rotate world matrix by rotation value to rotate triangle
	world_matrix = XMMatrixRotationY(rotation);

	//put model vertex and index buffers on pipeline for drawing
	m_model->Render(m_Direct3D->getDeviceContext());


	//render model with light shader
	result = m_light_shader->Render(m_Direct3D->getDeviceContext(), m_model->getIndexCount(),
		world_matrix, view_matrix, projection_matrix, m_model->GetTexture(), m_light->GetDirection(),
		m_light->GetDiffuseColor());

	if (!result)
	{
		return false;
	}


	//render model with colour shader
	//result = m_colour_shader->Render(m_Direct3D->getDeviceContext(), 
	//m_model->getIndexCount(), world_matrix, view_matrix, projection_matrix);
	//if (!result)
	//{
		//return false;
	//}

	//render model using texture shader
	//result = m_texture_shader->Render(m_Direct3D->getDeviceContext(), m_model->getIndexCount(),
			//world_matrix, view_matrix, projection_matrix, m_model->GetTexture());
	m_Direct3D->endScene();
	return true;
}