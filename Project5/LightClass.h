#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass& other);
	~LightClass();

	void setDiffuseColour(float red, float green, float blue, float alpha);
	void setDirection(float x, float y, float z);

	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();

private:
	XMFLOAT4 m_diffuse_colour;
	XMFLOAT3 m_direction;
};
#endif // !_LIGHTCLASS_H_


