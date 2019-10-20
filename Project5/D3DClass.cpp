#include "d3dclass.h"

D3DClass::D3DClass()
{
	//set to null
	m_swap_chain = 0;
	m_device = 0;
	m_device_context = 0;
	m_render_target_view = 0;
	m_depth_stencil_buffer = 0;
	m_depth_stencil_state = 0;
	m_depth_stencil_view = 0;
	m_raster_state = 0;
}

D3DClass::D3DClass(const D3DClass& other)
{
}

D3DClass::~D3DClass()
{
}

bool D3DClass::Init(int s_width, int s_height, bool vsync_on, 
	 HWND hwnd, bool fullscreen, float s_depth, float s_near)
{
	//set up D3D11, HWND is a window handle, vSync for user refresh rate or fast as possible
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapter_output;
	
	unsigned int num_modes;
	unsigned int numerator;
	unsigned int denominator;
	//unsigned long long string_length;
	//int error;
	float fov;
	float s_aspect;
	
	DXGI_MODE_DESC* display_mode_list;
	DXGI_ADAPTER_DESC adapter_desc;
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	
	ID3D11Texture2D* back_buffer_ptr;
	D3D_FEATURE_LEVEL feature_level;
	D3D11_TEXTURE2D_DESC depth_buffer_desc;
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
	D3D11_RASTERIZER_DESC raster_desc;
	D3D11_VIEWPORT viewport;

	m_vsync_enabled = vsync_on;

	//getting refresh rate from video card, deno and numer

	//create DirectX graphics interface factory, adapter for video card, enumerate
	//monitor (primary adapter output), get number of modes fitting display format
	//for monitor, create list holding all possible display modes for card and monitor
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result))
	{
		return false;
	}

	result = factory->EnumAdapters(0, &adapter);
	if(FAILED(result))
	{
		return false;
	}

	result = adapter->EnumOutputs(0, &adapter_output);
	if(FAILED(result))
	{
		return false;
	}

	result = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 
	DXGI_ENUM_MODES_INTERLACED, &num_modes, NULL);
	if(FAILED(result))
	{
		return false;
	}

	display_mode_list = new DXGI_MODE_DESC[num_modes];
	if(!display_mode_list)
	{
		return false;
	}

	//display mode list structures filled
	result = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 
	DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list);
	if(FAILED(result))
	{
		return false;
	}

	//find display mode matching screen dimensions, store deno and numer of refresh
	//of matching dimensions
	for(int i = 0; i < num_modes; i++)
	{
		if(display_mode_list[i].Width == (unsigned int)s_width)
		{
			if(display_mode_list[i].Height == (unsigned int)s_height)
			{
				numerator = display_mode_list[i].RefreshRate.Numerator;
				denominator = display_mode_list[i].RefreshRate.Denominator;
			}
		}
	}

	//retrieve data of video card
	result = adapter->GetDesc(&adapter_desc);
	if(FAILED(result))
	{
		return false;
	}
	
	m_video_card_memory = (int)(adapter_desc.DedicatedVideoMemory / pow(1024, 2)); //mb

	/*error = wcstombs_s(&string_length, m_video_card_description, 128, adapter_desc.Description, 128);
	if(error != 0)
	{
		return false;
	}*/

	delete[] display_mode_list;
	display_mode_list = 0;
	adapter_output->Release();
	adapter_output = 0;
	adapter->Release();
	adapter = 0;
	factory->Release();
	factory = 0;

	//refresh rate now used for DirectX initialisation
	//swap chain uses front and back buffer, back drawn to and front swapped to for user
	
    ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));

	//set to one back buffer and its width and height, regular 32-bit surface
    swap_chain_desc.BufferCount = 1;
    swap_chain_desc.BufferDesc.Width = s_width;
    swap_chain_desc.BufferDesc.Height = s_height;
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//vsync only draws screen 60 times a second
	if(m_vsync_enabled)
	{
	    swap_chain_desc.BufferDesc.RefreshRate.Numerator = numerator;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	
	else
	{
	    swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	}
	
	//usage of back buffer, handle for window to render to, multisampling off
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.OutputWindow = hwnd;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;

	if(fullscreen)
	{
		swap_chain_desc.Windowed = false;
	}
	
	else
	{
		swap_chain_desc.Windowed = true;
	}

	//scan line ordering and scaling to be unspecified, discord back buffer contents
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = 0;
	
	//set up feature level
	feature_level = D3D_FEATURE_LEVEL_11_0;

	//create swap chain, D3D device and its context
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, 
	&feature_level, 1, D3D11_SDK_VERSION, &swap_chain_desc, &m_swap_chain, 
	&m_device, NULL, &m_device_context);
	
	if(FAILED(result))
	{
		return false;
	}

	//workaround for primary -> <D3D11
	//pointer sent to back buffer
	result = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_ptr);
	if(FAILED(result))
	{
		return false;
	}

	//create render target view with pointer
	result = m_device->CreateRenderTargetView(back_buffer_ptr, NULL, &m_render_target_view);
	if(FAILED(result))
	{
		return false;
	}

	//release pointer to back buffer
	back_buffer_ptr->Release();
	back_buffer_ptr = 0;

	//set up depth buffer description
	ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));
	depth_buffer_desc.Width = s_width;
	depth_buffer_desc.Height = s_height;
	depth_buffer_desc.MipLevels = 1;
	depth_buffer_desc.ArraySize = 1;
	depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_buffer_desc.SampleDesc.Count = 1;
	depth_buffer_desc.SampleDesc.Quality = 0;
	depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_buffer_desc.CPUAccessFlags = 0;
	depth_buffer_desc.MiscFlags = 0;

	result = m_device->CreateTexture2D(&depth_buffer_desc, NULL, &m_depth_stencil_buffer);
	if(FAILED(result))
	{
		return false;
	}

	//create depth/stencil buffer texture before initialising description of stencil state
	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilReadMask = 0xFF;
	depth_stencil_desc.StencilWriteMask = 0xFF;

	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//create deoth stencil rate and have it take effect
	result = m_device->CreateDepthStencilState(&depth_stencil_desc, &m_depth_stencil_state);
	if(FAILED(result))
	{
		return false;
	}
	m_device_context->OMSetDepthStencilState(m_depth_stencil_state, 1);

	//create desc. of depth stencil buffer view, set up description
	ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));
	depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;
	result = m_device->CreateDepthStencilView(m_depth_stencil_buffer, 
	&depth_stencil_view_desc, &m_depth_stencil_view);
	if(FAILED(result))
	{
		return false;
	}

	//bind render target view and depth stencil buffer to output render pipeline
	m_device_context->OMSetRenderTargets(1, &m_render_target_view,
	m_depth_stencil_view);

	//raster description for polygons being drawn
	raster_desc.AntialiasedLineEnable = false;
	raster_desc.CullMode = D3D11_CULL_BACK;
	raster_desc.DepthBias = 0;
	raster_desc.DepthBiasClamp = 0.0f;
	raster_desc.DepthClipEnable = true;
	raster_desc.FillMode = D3D11_FILL_SOLID;
	raster_desc.FrontCounterClockwise = false;
	raster_desc.MultisampleEnable = false;
	raster_desc.ScissorEnable = false;
	raster_desc.SlopeScaledDepthBias = 0.0f;
	
	result = m_device->CreateRasterizerState(&raster_desc, &m_raster_state);
	if(FAILED(result))
	{
		return false;
	}

	//set rasteriser state
	m_device_context->RSSetState(m_raster_state);
	
	//set up viewport
    viewport.Width = (float)s_width;
    viewport.Height = (float)s_height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    m_device_context->RSSetViewports(1, &viewport);

	//create projection matrix (used to translate 3D scene into 2D viewport)
	fov = 3.141592654f / 4.0f;
	s_aspect = (float)s_width / (float)s_height;
	m_projection_matrix = XMMatrixPerspectiveFovLH(fov, s_aspect, s_near, s_depth);

	//create world matrix (used to convert object vertices into 3D vertices)
	//init world matrix to identity matrix
	m_world_matrix = XMMatrixIdentity();

	//create orthographic projection matrix for 2D rendering
	m_ortho_matrix = XMMatrixOrthographicLH((float)s_width, (float)s_height, s_near, s_depth);
    return true;
}

void D3DClass::Exit()
{
	//set to windowed to escape swap chain exception
	if(m_swap_chain)
	{
		m_swap_chain->SetFullscreenState(false, NULL);
	}

	if(m_raster_state)
	{
		m_raster_state->Release();
		m_raster_state = 0;
	}

	if(m_depth_stencil_view)
	{
		m_depth_stencil_view->Release();
		m_depth_stencil_view = 0;
	}

	if(m_depth_stencil_state)
	{
		m_depth_stencil_state->Release();
		m_depth_stencil_state = 0;
	}

	if(m_depth_stencil_buffer)
	{
		m_depth_stencil_buffer->Release();
		m_depth_stencil_buffer = 0;
	}

	if(m_render_target_view)
	{
		m_render_target_view->Release();
		m_render_target_view = 0;
	}

	if(m_device_context)
	{
		m_device_context->Release();
		m_device_context = 0;
	}

	if(m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if(m_swap_chain)
	{
		m_swap_chain->Release();
		m_swap_chain = 0;
	}
	return;
}

void D3DClass::beginScene(float red, float green, float blue, float alpha)
{
	//called for drawing 3D scene at frame beginning
	float colour[4];
	colour[0] = red;
	colour[1] = green;
	colour[2] = blue;
	colour[3] = alpha;

	//clear back and depth buffers
	m_device_context->ClearRenderTargetView(m_render_target_view, colour);
	m_device_context->ClearDepthStencilView(m_depth_stencil_view, 
	D3D11_CLEAR_DEPTH, 1.0f, 0);
	return;
}

void D3DClass::endScene()
{
	//gets swap chain to display complete 3D scene
	if(m_vsync_enabled)
	{
		m_swap_chain->Present(1, 0);
	}
	
	else
	{
		m_swap_chain->Present(0, 0);
	}

	return;
}

ID3D11Device* D3DClass::getDevice()
{
	return m_device;
}

ID3D11DeviceContext* D3DClass::getDeviceContext()
{
	return m_device_context;
}

void D3DClass::getProjectionMatrix(XMMATRIX& projection_matrix)
{
	projection_matrix = m_projection_matrix;
	return;
}

void D3DClass::getWorldMatrix(XMMATRIX& world_matrix)
{
	world_matrix = m_world_matrix;
	return;
}

void D3DClass::getOrthoMatrix(XMMATRIX& ortho_matrix)
{
	ortho_matrix = m_ortho_matrix;
	return;
}

void D3DClass::getVideoCard(char* card_name, int& memory)
{
	//returns by reference video card information
	strcpy_s(card_name, 128, m_video_card_description);
	memory = m_video_card_memory;
	return;
}