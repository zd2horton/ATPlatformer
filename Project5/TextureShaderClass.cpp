#include "TextureShaderClass.h"

TextureShaderClass::TextureShaderClass()
{
	m_vShader = 0;
	m_PShader = 0;
	m_layout = 0;
	m_matrix_buffer = 0;
	m_sample_state = 0;
}

TextureShaderClass::TextureShaderClass(const TextureShaderClass& other)
{

}

TextureShaderClass::~TextureShaderClass()
{

}

bool TextureShaderClass::Init(ID3D11Device* device, HWND hwnd)
{
	bool output;
	output = initShader(device, hwnd, (WCHAR*)L"texture.vs", (WCHAR*)L"texture.ps");
	if (!output)
	{
		return false;
	}

	return true;
}

void TextureShaderClass::Exit()
{
	shutdownShader();
	return;
}

bool TextureShaderClass::Render(ID3D11DeviceContext* device_context, int index, 
	XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix, ID3D11ShaderResourceView* texture)
{
	bool output;
	output = setShaderParameters(device_context, world_matrix, view_matrix, projection_matrix, texture);
	
	if (!output)
	{
		return false;
	}

	renderShader(device_context, index);
	return true;
}

bool TextureShaderClass::initShader(ID3D11Device* device, HWND hwnd, WCHAR* vs_name, WCHAR* ps_name)
{
	HRESULT result;
	ID3D10Blob* error_message;
	ID3D10Blob* vShader_buffer;
	ID3D10Blob* pShader_buffer;
	D3D11_INPUT_ELEMENT_DESC polygon_layout[2];
	unsigned int num_elements;
	D3D11_BUFFER_DESC matrix_buffer_desc;
	D3D11_SAMPLER_DESC sampler_desc;

	error_message = 0;
	vShader_buffer = 0;
	pShader_buffer = 0;

	result = D3DCompileFromFile(vs_name, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 
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

	result = D3DCompileFromFile(ps_name, NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
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

	polygon_layout[0].SemanticName = "POSITION";
	polygon_layout[0].SemanticIndex = 0;
	polygon_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygon_layout[0].InputSlot = 0;
	polygon_layout[0].AlignedByteOffset = 0;
	polygon_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[0].InstanceDataStepRate = 0;

	polygon_layout[1].SemanticName = "TEXCOORD";
	polygon_layout[1].SemanticIndex = 0;
	polygon_layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygon_layout[1].InputSlot = 0;
	polygon_layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[1].InstanceDataStepRate = 0;

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

	matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	matrix_buffer_desc.ByteWidth = sizeof(MatrixBufferType);
	matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrix_buffer_desc.MiscFlags = 0;
	matrix_buffer_desc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrix_buffer_desc, NULL, &m_matrix_buffer);
	if (FAILED(result))
	{
		return false;
	}

	//sampler state description, passed to PS
	//"filter" determines how which pixels used for texture look is decided 

	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.BorderColor[0] = 0;
	sampler_desc.BorderColor[1] = 0;
	sampler_desc.BorderColor[2] = 0;
	sampler_desc.BorderColor[3] = 0;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	sampler_desc.MinLOD = 0;
	sampler_desc.MipLODBias = 0.0f;

	result = device->CreateSamplerState(&sampler_desc, &m_sample_state);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureShaderClass::shutdownShader()
{
	if (m_sample_state)
	{
		m_sample_state->Release();
		m_sample_state = 0;
	}

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

void TextureShaderClass::shaderErrorOutput(ID3D10Blob* error_message, HWND hwnd, const wchar_t* shader_name)
{
	char* compile_error;
	unsigned long long buffer_size;
	ofstream fout;

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
	return;
}

bool TextureShaderClass::setShaderParameters(ID3D11DeviceContext* device_context, XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix, ID3D11ShaderResourceView* texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	MatrixBufferType* data_ptr;
	unsigned int buffer_number;

	world_matrix = XMMatrixTranspose(world_matrix);
	view_matrix = XMMatrixTranspose(view_matrix);
	projection_matrix = XMMatrixTranspose(projection_matrix);

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

	buffer_number = 0;
	device_context->VSSetConstantBuffers(buffer_number, 1, &m_matrix_buffer);
	
	//setting of texture in pixel shader
	device_context->PSSetShaderResources(0, 1, &texture);
	return true;
}

void TextureShaderClass::renderShader(ID3D11DeviceContext* device_context, int index)
{
	device_context->IASetInputLayout(m_layout);
	device_context->VSSetShader(m_vShader, NULL, 0);
	device_context->PSSetShader(m_PShader, NULL, 0);

	//set sample state in PS
	device_context->PSSetSamplers(0, 1, &m_sample_state);
	device_context->DrawIndexed(index, 0, 0);
	return;
}
