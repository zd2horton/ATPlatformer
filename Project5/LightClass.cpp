#include "LightClass.h"

LightClass::LightClass()
{

}

LightClass::LightClass(const LightClass& other)
{

}


LightClass::~LightClass()
{

}

void LightClass::setDiffuseColour(float red, float green, float blue, float alpha)
{
	m_diffuse_colour = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::setDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return m_diffuse_colour;
}

XMFLOAT3 LightClass::GetDirection()
{
	return m_direction;
}
