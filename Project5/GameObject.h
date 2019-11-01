#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

using namespace DirectX;
using namespace std;

class GameObject
{
public:
	GameObject();
	GameObject(const GameObject& other);
	~GameObject();

	XMFLOAT3 getPos();
	void setPos(XMFLOAT3 new_pos);

	XMMATRIX getRotMatrix();
	void setRotMatrix(XMMATRIX new_rot);

	float getRoll();
	float getPitch();
	float getYaw();

	void setRoll(float new_roll);
	void setPitch(float new_pitch);
	void setYaw(float new_yaw);

	XMFLOAT3 getScaling();
	void setScaling(XMFLOAT3 new_scale);
	bool Frame();

	XMMATRIX getWorldMatrix();

	string getTextureName();
	void setTextureName(string new_tname);

	string getModelName();
	void setModelName(string new_mname);

protected:

	string texture_name;
	string model_name;

	XMFLOAT3 object_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 object_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 object_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMMATRIX world_matrix = XMMatrixIdentity();
	XMMATRIX rot_matrix = XMMatrixIdentity();
	XMMATRIX scale_matrix = XMMatrixIdentity();
};
#endif // !_GAMEOBJECT_H_


