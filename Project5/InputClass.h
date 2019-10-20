#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass& other);
	~InputClass();

	void Init();

	void KeyDown(unsigned int key_input);
	void KeyUp(unsigned int key_input);

	bool IsKeyDown(unsigned int key_input);

private:
	bool m_keys[256];
};

#endif // !_INPUTCLASS_H_