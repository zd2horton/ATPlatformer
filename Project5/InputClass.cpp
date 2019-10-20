#include "InputClass.h"


InputClass::InputClass()
{
	
}

InputClass::InputClass(const InputClass& other)
{
	
}

InputClass::~InputClass()
{
	
}

void InputClass::Init()
{
	//keys initialised as not pressed
	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}
	return;
}

void InputClass::KeyDown(unsigned int key_input)
{
	m_keys[key_input] = true;
	return;
}

void InputClass::KeyUp(unsigned int key_input)
{
	m_keys[key_input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key_input)
{
	return m_keys[key_input];
}