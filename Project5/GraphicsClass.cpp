#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_Direct3D = 0;
	m_camera = 0;
	m_colour_shader = 0;
	m_cube1 = 0;
	m_light1 = 0;
	m_cube2 = 0;
	m_light2 = 0;
	m_cube3 = 0;
	m_light3 = 0;
	m_texture_shader = 0;
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

	//init models and lighting
	m_cube1 = initModel(m_cube1, (char*)"stonetex.tga", (char*)"cube.txt", XMFLOAT3(-3.0f, 0.0f, 0.0f),
		XMFLOAT3(0.5f, 0.5f, 0.5f), hwnd);

	m_light1 = initLight(m_light1, XMFLOAT4(0.52734375f, 0.8046875f, 0.91796875f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f));


	m_cube2 = initModel(m_cube2, (char*)"stonetex.tga", (char*)"cube.txt", XMFLOAT3(-8.0f, 3.0f, 0.0f),
		XMFLOAT3(0.25f, 0.25f, 0.25f), hwnd);

	m_light2 = initLight(m_light2, XMFLOAT4(1.0f, 0.592156862745098f, 0.4784313725490196f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f));

	m_cube3 = initModel(m_cube3, (char*)"stonetex.tga", (char*)"cube.txt", XMFLOAT3(5.0f, -8.0f, 0.0f),
		XMFLOAT3(0.75f, 0.125f, 0.5f), hwnd);

	m_light3 = initLight(m_light3, XMFLOAT4(0.7f, 0.7f, 0.7f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f));


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
	return true;
}

ModelClass* GraphicsClass::initModel(ModelClass* init_model, char* texture_name, char* model_filename, 
					XMFLOAT3 model_pos, XMFLOAT3 model_scale, HWND hwnd)
{
	bool result;

	init_model = new ModelClass;
	if (!init_model)
	{
		return false;
	}

	result = init_model->Init(m_Direct3D->getDevice(), m_Direct3D->getDeviceContext(), texture_name, model_filename);
	if (!result)
	{
		MessageBox(hwnd, L"Cannot initialize model object.", L"Error!", MB_OK);
		return false;
	}

	init_model->setPos(model_pos);
	init_model->setScaling(model_scale);
	return init_model;
}

LightClass* GraphicsClass::initLight(LightClass* init_light, XMFLOAT4 new_diffuse, XMFLOAT3 new_direction)
{
	init_light = new LightClass;
	if (!init_light)
	{
		return false;
	}
	init_light->setDiffuseColour(new_diffuse);
	init_light->setDirection(new_direction);
	return init_light;
}

void GraphicsClass::Exit()
{
	if (m_light_shader)
	{
		m_light_shader->Exit();
		delete m_light_shader;
		m_light_shader = 0;
	}

	exitModel(m_cube1);
	exitModel(m_cube2);
	exitModel(m_cube3);
	exitLight(m_light1);
	exitLight(m_light2);
	exitLight(m_light3);

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

	if (m_Direct3D)
	{
		m_Direct3D->Exit();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
	return;
}

void GraphicsClass::exitModel(ModelClass* model_exit)
{
	if (model_exit)
	{
		model_exit->Exit();
		delete model_exit;
		model_exit = 0;
	}
}

void GraphicsClass::exitLight(LightClass* light_exit)
{
	if (light_exit)
	{
		delete light_exit;
		light_exit = 0;
	}
}

bool GraphicsClass::Frame()
{
	bool result;
	//static float rotation = 0.0f;

	//update rotation variable
	//rotation += (float)XM_PI * 0.01f;

	//if (rotation > 360.0f)
	//{
		//rotation -= 360.0f;
	//}

	m_cube1->setPitch(m_cube1->getPitch() + 0.02f);
	m_cube1->setRoll(m_cube1->getRoll() + 0.02f);

	m_cube2->setRoll(m_cube2->getRoll() + 0.1f);

	m_cube3->setPitch(m_cube3->getPitch() + 0.08f);
	m_cube3->setRoll(m_cube3->getRoll() + 0.1f);
	m_cube3->setYaw(m_cube3->getYaw() + 0.05f);

	m_cube1->Frame();
	m_cube2->Frame();
	m_cube3->Frame();

	result = initRenderer();
	if (!result)
	{
		return false;
	}

	result = Render(m_cube1, m_light1);
	if (!result)
	{
		return false;
	}

	result = Render(m_cube2, m_light2);
	if (!result)
	{
		return false;
	}

	result = Render(m_cube3, m_light3);
	if (!result)
	{
		return false;
	}

	m_Direct3D->endScene();

	return true;
}

bool GraphicsClass::initRenderer()
{
	m_Direct3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//generate view matrix from position
	m_camera->render();
	return true;
}

bool GraphicsClass::Render(ModelClass* render_model, LightClass* render_light)
{
	////XMMATRIX world_matrix;
	XMMATRIX view_matrix;
	XMMATRIX projection_matrix;

	bool result;

	//get matrixes from camera and D3D objects
	m_camera->getViewMatrix(view_matrix);
	////m_Direct3D->getWorldMatrix(world_matrix);
	m_Direct3D->getProjectionMatrix(projection_matrix);

	//rotate world matrix by rotation value to rotate triangle
	////world_matrix = XMMatrixRotationY(rotation);

	//put model vertex and index buffers on pipeline for drawing
	render_model->Render(m_Direct3D->getDeviceContext());

	//render model with light shader
	result = m_light_shader->Render(m_Direct3D->getDeviceContext(), render_model->getIndexCount(),
		render_model->getWorldMatrix(), view_matrix, projection_matrix, render_model->GetTexture(),
		render_light->GetDirection(), render_light->GetDiffuseColor());

	if (!result)
	{
		return false;
	}



	//render model with colour shader
	//result = m_colour_shader->Render(m_Direct3D->getDeviceContext(), 
	//m_cube1->getIndexCount(), world_matrix, view_matrix, projection_matrix);
	//if (!result)
	//{
		//return false;
	//}

	//render model using texture shader
	//result = m_texture_shader->Render(m_Direct3D->getDeviceContext(), m_cube1->getIndexCount(),
			//world_matrix, view_matrix, projection_matrix, m_cube1->GetTexture());
	return true;
}