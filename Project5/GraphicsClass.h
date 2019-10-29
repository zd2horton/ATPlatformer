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

	ModelClass* initModel(ModelClass* init_model, char* texture_name, char* model_filename,
		XMFLOAT3 model_pos, XMFLOAT3 model_scale, HWND hwnd);
	void exitModel(ModelClass* model_exit);

	LightClass* initLight(LightClass* init_light, XMFLOAT4 new_diffuse, XMFLOAT3 new_direction);
	void exitLight(LightClass* model_light);
	

private:
	bool initRenderer();
	bool Render(ModelClass* render_model, LightClass* render_light);

	D3DClass* m_Direct3D;
	CameraClass* m_camera;
	ColourShaderClass* m_colour_shader;
	LightShaderClass* m_light_shader;
	TextureShaderClass* m_texture_shader;

	ModelClass* m_cube1;
	LightClass* m_light1;

	ModelClass* m_cube2;
	LightClass* m_light2;

	ModelClass* m_cube3;
	LightClass* m_light3;

	int cube_number;
	
	ModelClass* cubes;
};

#endif // !_GRAPHICSCLASS_H_