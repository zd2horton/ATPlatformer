#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_
#pragma comment(lib, "d3d11.lib") //links D3D functionality
#pragma comment(lib, "dxgi.lib") //links hardware interface tools and dxdiag
#pragma comment(lib, "d3dcompiler.lib") //links shader compiliing functionality 

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass& other);
	~D3DClass();

	bool Init(int s_width, int s_height, bool vsync_on,
		HWND hwnd, bool fullscreen, float s_depth, float s_near);
	void Exit();

	void beginScene(float red, float green, float blue, float alpha);
	void endScene();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	void getProjectionMatrix(XMMATRIX& projection_matrix); 
	void getWorldMatrix(XMMATRIX& world_matrix);
	void getOrthoMatrix(XMMATRIX& ortho_matrix);
	void getVideoCard(char* card_name, int& memory);

private:
	bool m_vsync_enabled;
	int m_video_card_memory;
	char m_video_card_description[128];

	IDXGISwapChain* m_swap_chain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_device_context;
	ID3D11RenderTargetView* m_render_target_view;
	ID3D11Texture2D* m_depth_stencil_buffer;

	ID3D11DepthStencilState* m_depth_stencil_state;
	ID3D11DepthStencilView* m_depth_stencil_view;
	ID3D11RasterizerState* m_raster_state;

	XMMATRIX m_projection_matrix;
	XMMATRIX m_world_matrix;
	XMMATRIX m_ortho_matrix;

};

#endif // !_D3DCLASS_H_




