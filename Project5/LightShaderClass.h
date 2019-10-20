#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

class LightShaderClass
{
public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass& other);
	~LightShaderClass();

	bool Init(ID3D11Device* device, HWND hwnd);
	void Exit();
	bool Render(ID3D11DeviceContext* device_context, int index,
		XMMATRIX world_matrix, XMMATRIX view_matrix,
		XMMATRIX projection_matrix, ID3D11ShaderResourceView* texture, 
		XMFLOAT3 light_direction, XMFLOAT4 diffuse_colour);


private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		XMFLOAT4 diffuse_colour;
		XMFLOAT3 light_direction;
		float padding; //making structure a multiple of 16 for createbuffer 
	};

	bool initShader(ID3D11Device* device, HWND hwnd, WCHAR* vs_name, WCHAR* ps_name);
	void shutdownShader();
	void shaderErrorOutput(ID3D10Blob* error_message, HWND hwnd, WCHAR* shader_name);

	bool setShaderParameters(ID3D11DeviceContext* device_context,
		XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix,
		ID3D11ShaderResourceView* texture,
		XMFLOAT3 light_direction, XMFLOAT4 diffuse_colour);
	void renderShader(ID3D11DeviceContext* device_context, int index);

	ID3D11VertexShader* m_vShader;
	ID3D11PixelShader* m_pShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrix_buffer;
	ID3D11SamplerState* m_sample_state;
	ID3D11Buffer* m_light_buffer;
};

#endif // !_LIGHTSHADERCLASS_H_



