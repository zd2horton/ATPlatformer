#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_
#include <Windows.h>
#include "Constants.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ColourShaderClass.h"
#include "ModelClass.h"
#include "TextureShaderClass.h"
#include "LightShaderClass.h"
#include "LightClass.h"

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass& other);
	~GraphicsClass();

	bool Init(int s_width, int s_height, HWND hwnd);
	void Exit();
	bool Frame();

private:
	bool Render(float rotation);
	D3DClass* m_Direct3D;
	CameraClass* m_camera;
	ColourShaderClass* m_colour_shader;
	ModelClass* m_model;
	TextureShaderClass* m_texture_shader;
	LightShaderClass* m_light_shader;
	LightClass* m_light;
};

#endif // !_GRAPHICSCLASS_H_