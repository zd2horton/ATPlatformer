#include "ColourShaderClass.h"

ColourShaderClass::ColourShaderClass()
{
	m_vShader = 0;
	m_PShader = 0;
	m_layout = 0;
	m_matrix_buffer = 0;
}

ColourShaderClass::ColourShaderClass(const ColourShaderClass& other)
{

}

ColourShaderClass::~ColourShaderClass()
{

}

bool ColourShaderClass::Init(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//init shaders
	result = initShader(device, hwnd, L"colour.vs", L"colour.ps");
	if (!result)
	{
		return false;
	}
	return true;
}

void ColourShaderClass::Exit()
{
	//shutdown shaders and objects
	shutdownShader();
	return;
}

bool ColourShaderClass::Render(ID3D11DeviceContext* device_context, int index, XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix)
{
	//sets parameters inside shader using SetShaderParameters then calls renderShader
	bool result;
	result = setShaderParameters(device_context, world_matrix, view_matrix, projection_matrix);
	if (!result)
	{
		return false;
	}

	renderShader(device_context, index);
	return true;
}

bool ColourShaderClass::initShader(ID3D11Device* device, HWND hwnd, const wchar_t* vs_name, const wchar_t* ps_name)
{
	//loads shader files for GPU and DirectX
	HRESULT result;
	ID3D10Blob* error_message;
	ID3D10Blob* vShader_buffer;
	ID3D10Blob* pShader_buffer;
	D3D11_INPUT_ELEMENT_DESC polygon_layout[2];
	unsigned int num_elements;
	D3D11_BUFFER_DESC matrix_buffer_desc;

	//init pointers to null
	error_message = 0;
	vShader_buffer = 0;
	pShader_buffer = 0;
	
	//compile shader codes into buffers
	result = D3DCompileFromFile(vs_name, NULL, NULL, "ColourVortexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &vShader_buffer, &error_message);
	if (FAILED(result))
	{
		//write message if failed to compile
		if (error_message)
		{
			shaderErrorOutput(error_message, hwnd, vs_name);
		}

		else
		{
			MessageBox(hwnd, vs_name, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	result = D3DCompileFromFile(ps_name, NULL, NULL, "ColourPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &pShader_buffer, &error_message);

	if (FAILED(result))
	{
		if (error_message)
		{
			shaderErrorOutput(error_message, hwnd, ps_name);
		}

		else
		{
			MessageBox(hwnd, ps_name, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//use buffers to make shader objects, pointers used to interface with shaders
	result = device->CreateVertexShader(vShader_buffer->GetBufferPointer(), vShader_buffer->GetBufferSize(), NULL, &m_vShader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(pShader_buffer->GetBufferPointer(), pShader_buffer->GetBufferSize(), NULL, &m_PShader);
	if (FAILED(result))
	{
		return false;
	}

	//create layout of vertex data to be processed by shader, create position and colour vector
	//AlignedByteOffset indicates how data is spaced in buffer
	polygon_layout[0].SemanticName = "POSITION";
	polygon_layout[0].SemanticIndex = 0;
	polygon_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygon_layout[0].InputSlot = 0;
	polygon_layout[0].AlignedByteOffset = 0;
	polygon_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[0].InstanceDataStepRate = 0;

	polygon_layout[1].SemanticName = "COLOUR";
	polygon_layout[1].SemanticIndex = 0;
	polygon_layout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygon_layout[1].InputSlot = 0;
	polygon_layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[1].InstanceDataStepRate = 0;

	//get size of layout description, create input layout

	num_elements = sizeof(polygon_layout) / sizeof(polygon_layout[0]);
	result = device->CreateInputLayout(polygon_layout, num_elements, 
		vShader_buffer->GetBufferPointer(), vShader_buffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	vShader_buffer->Release();
	vShader_buffer = 0;

	pShader_buffer->Release();
	pShader_buffer = 0;

	//set up constant buffer to interface with shader, set to dynamic (constant)
	matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	matrix_buffer_desc.ByteWidth = sizeof(MatrixBufferType);
	matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrix_buffer_desc.MiscFlags = 0;
	matrix_buffer_desc.StructureByteStride = 0;

	//create constant buffer pointer for accessing vertex shader constant buffer
	result = device->CreateBuffer(&matrix_buffer_desc, NULL, &m_matrix_buffer);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

void ColourShaderClass::shutdownShader()
{
	if (m_matrix_buffer)
	{
		m_matrix_buffer->Release();
		m_matrix_buffer = 0;
	}

	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_PShader)
	{
		m_PShader->Release();
		m_PShader = 0;
	}

	if (m_vShader)
	{
		m_vShader->Release();
		m_vShader = 0;
	}
	return;
}

void ColourShaderClass::shaderErrorOutput(ID3D10Blob* error_message, HWND hwnd, const wchar_t* shader_name)
{
	char* compile_error;
	unsigned long long buffer_size;
	ofstream fout;

	//pointer to error message text buffer, length of message, write error message
	compile_error = (char*)(error_message->GetBufferPointer());
	buffer_size = error_message->GetBufferSize();
	fout.open("shader_errors.txt");

	for (unsigned long long i = 0; i < buffer_size; i++)
	{
		fout << compile_error[i];
	}

	fout.close();
	error_message->Release();
	error_message = 0;
	MessageBox(hwnd, L"Error compiling shader. Check logs for more details.", shader_name, MB_OK);
}

bool ColourShaderClass::setShaderParameters(ID3D11DeviceContext* device_context, XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix)
{
	//eases setting global variables in shaders, matrixes used created in GraphicsClass, sent into vertex shader during Render
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	MatrixBufferType* data_ptr;
	unsigned int buffer_number;

	//transpose matrixes
	world_matrix = XMMatrixTranspose(world_matrix);
	view_matrix = XMMatrixTranspose(view_matrix);
	projection_matrix = XMMatrixTranspose(projection_matrix);

	//lock matrixbuffer, set matrixes and unlock
	result = device_context->Map(m_matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
	{
		return false;
	}
	
	data_ptr = (MatrixBufferType*)mapped_resource.pData;
	data_ptr->world = world_matrix;
	data_ptr->view = view_matrix;
	data_ptr->projection = projection_matrix;
	device_context->Unmap(m_matrix_buffer, 0);

	//set updated matrix buffer in HLSL vertex shader
	buffer_number = 0;
	device_context->VSSetConstantBuffers(buffer_number, 1, &m_matrix_buffer);
	return true;
}

void ColourShaderClass::renderShader(ID3D11DeviceContext* device_context, int index)
{
	//set input layout to active in input assembler, set shaders to render this vertex buffer, render triangle
	device_context->IASetInputLayout(m_layout);
	device_context->VSSetShader(m_vShader, NULL, 0);
	device_context->PSSetShader(m_PShader, NULL, 0);
	device_context->DrawIndexed(index, 0, 0);
	return;

}
