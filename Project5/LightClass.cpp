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

void LightClass::setDiffuseColour(XMFLOAT4 new_diffuse)
{
	m_diffuse_colour = new_diffuse;
	return;
}

void LightClass::setDirection(XMFLOAT3 new_direction)
{
	m_direction = new_direction;
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
