#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

class TextureShaderClass
{

public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass& other);
	~TextureShaderClass();

	bool Init(ID3D11Device* device, HWND hwnd);
	void Exit();
	bool Render(ID3D11DeviceContext* device_context, int index,
		XMMATRIX world_matrix, XMMATRIX view_matrix, 
		XMMATRIX projection_matrix, ID3D11ShaderResourceView* texture);


private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	bool initShader(ID3D11Device* device, HWND hwnd, WCHAR* vs_name, WCHAR* ps_name);
	void shutdownShader();
	void shaderErrorOutput(ID3D10Blob* error_message, HWND hwnd, const wchar_t* shader_name);

	bool setShaderParameters(ID3D11DeviceContext* device_context,
		XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix, 
		ID3D11ShaderResourceView* texture);
	void renderShader(ID3D11DeviceContext* device_context, int index);

	ID3D11VertexShader* m_vShader;
	ID3D11PixelShader* m_PShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrix_buffer;
	ID3D11SamplerState* m_sample_state;
};

#endif // !_TEXTURESHADERCLASS_H_
