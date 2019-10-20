#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>

#include "TextureClass.h"

using namespace DirectX;
class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass& other);
	~ModelClass();

	bool Init(ID3D11Device* device, ID3D11DeviceContext* device_context, char* file_name);
	void Exit();
	void Render(ID3D11DeviceContext* device_context);

	int getIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	//vertex type used with vertex buffer
	//struct VertexType //struct for colour
	//{
		//XMFLOAT3 position;
		//XMFLOAT4 colour;
	//};

	struct VertexType //struct for texture coordinate
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal; //light normal
	};

	ID3D11Buffer* m_vertex_buffer;
	ID3D11Buffer* m_index_buffer;
	int m_vertex_count;
	int m_index_count;
	TextureClass* m_texture;

	bool initBuffers(ID3D11Device* device);
	void exitBuffers();
	void renderBuffers(ID3D11DeviceContext* device_context);
	bool loadTexture(ID3D11Device* device, ID3D11DeviceContext* device_context, char* file_name);
	void releaseTexture();
};
#endif // !_MODELCLASS_H_


