#ifndef _COLOURSHADERCLASS_H_
#define _COLOURSHADERCLASS_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

//used to envoke shaders
class ColourShaderClass
{

public: 
	ColourShaderClass();
	ColourShaderClass(const ColourShaderClass& other);
	~ColourShaderClass();

	bool Init(ID3D11Device* device, HWND hwnd);
	void Exit();
	bool Render(ID3D11DeviceContext* device_context, int index, 
		XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix);


private:
	//cbuffer type definition must match vertex shader
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	bool initShader(ID3D11Device* device, HWND hwnd, const wchar_t* vs_name, const wchar_t* ps_name);
	void shutdownShader();
	void shaderErrorOutput(ID3D10Blob* error_message, HWND hwnd, const wchar_t* shader_name);

	bool setShaderParameters(ID3D11DeviceContext* device_context, 
		XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix);
	void renderShader(ID3D11DeviceContext* device_context, int index);

	ID3D11VertexShader* m_vShader;
	ID3D11PixelShader* m_PShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrix_buffer;
};

#endif // !_COLOURSHADERCLASS_H_
