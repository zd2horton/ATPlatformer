#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

#include "ModelClass.h"
#include "InputClass.h"

class Player : public ModelClass
{
public:
	Player();
	Player(const Player& other);
	~Player();
	bool Frame();

protected:
	InputClass* m_Input;
};
#endif // !_PLAYER_H_
