#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_vertex_buffer = 0;
	m_index_buffer = 0;
	m_texture = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{

}


ModelClass::~ModelClass()
{

}

bool ModelClass::Init(ID3D11Device* device, ID3D11DeviceContext* device_context, char* file_name)
{
	bool result;
	//init buffers
	result = initBuffers(device);
	if (!result)
	{
		return false;
	}

	//load texture
	result = loadTexture(device, device_context, file_name);
	if (!result)
	{
		return false;
	}
	return true;
}

void ModelClass::Exit()
{
	releaseTexture();
	exitBuffers();
}

void ModelClass::Render(ID3D11DeviceContext* device_context)
{
	//put buffers in pipeline for drawing
	renderBuffers(device_context);
	return;
}

int ModelClass::getIndexCount()
{
	//returns number of indexes in the model
	return m_index_count;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_texture->GetTexture();
}

bool ModelClass::initBuffers(ID3D11Device* device)
{
	//handles creating buffers, set points in buffers manually for triangle
	VertexType* vertices;
	unsigned long* indices;

	D3D11_BUFFER_DESC vertex_buffer_desc;
	D3D11_BUFFER_DESC index_buffer_desc;
	D3D11_SUBRESOURCE_DATA vertex_data;
	D3D11_SUBRESOURCE_DATA index_data;
	HRESULT result;

	//arrays for holding vertex and index data for use of populating final buffers
	m_vertex_count = 3;
	m_index_count = 3;

	vertices = new VertexType[m_vertex_count];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_index_count];
	if (!indices)
	{
		return false;
	}

	//fill arrays with triangle points and index to each (clockwise)
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  
	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  
	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f); 

	//vertices[0].colour = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//vertices[1].colour = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//vertices[2].colour = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[1].texture = XMFLOAT2(0.5f, 0.0f);
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	indices[0] = 0;  
	indices[1] = 1;
	indices[2] = 2; 

	//create buffers using this data
	//buffer descriptions
	vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_buffer_desc.ByteWidth = sizeof(VertexType) * m_vertex_count;
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.CPUAccessFlags = 0;
	vertex_buffer_desc.MiscFlags = 0;
	vertex_buffer_desc.StructureByteStride = 0;

	//fill out subresource pointer for previous array
	vertex_data.pSysMem = vertices;
	vertex_data.SysMemPitch = 0;
	vertex_data.SysMemSlicePitch = 0;

	//create buffer
	result = device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
	if (FAILED(result))
	{
		return false;
	}

	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_desc.ByteWidth = sizeof(unsigned long) * m_index_count;
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.CPUAccessFlags = 0;
	index_buffer_desc.MiscFlags = 0;
	index_buffer_desc.StructureByteStride = 0;

	index_data.pSysMem = indices;
	index_data.SysMemPitch = 0;
	index_data.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
	return true;
}

void ModelClass::exitBuffers()
{
	if (m_index_buffer)
	{
		m_index_buffer->Release();
		m_index_buffer = 0;
	}

	if (m_vertex_buffer)
	{
		m_vertex_buffer->Release();
		m_vertex_buffer = 0;
	}
	return;
}

void ModelClass::renderBuffers(ID3D11DeviceContext* device_context)
{
	//set buffers as active on input assembler in GPU, render buffer using shader
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	//set buffers to active in input assembler for rendering
	device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
	device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

	//set type of primitive that should be rendered
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return;
}

bool ModelClass::loadTexture(ID3D11Device* device, ID3D11DeviceContext* device_context, char* file_name)
{
	bool output;

	m_texture = new TextureClass;
	if (!m_texture)
	{
		return false;
	}

	//initialise texture object
	output = m_texture->Init(device, device_context, file_name);
	if (!output)
	{
		return false;
	}
	return true;
}

void ModelClass::releaseTexture()
{
	if (m_texture)
	{
		m_texture->Exit();
		delete m_texture;
		m_texture = 0;
	}
	return;
}
