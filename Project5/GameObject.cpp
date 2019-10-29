#include "GameObject.h"



GameObject::GameObject()
{
}

GameObject::GameObject(const GameObject & other)
{
}


GameObject::~GameObject()
{
}

XMFLOAT3 GameObject::getPos()
{
	return object_pos;
}

void GameObject::setPos(XMFLOAT3 new_pos)
{
	object_pos = new_pos;
}

XMMATRIX GameObject::getRotMatrix()
{
	return rot_matrix;
}

void GameObject::setRotMatrix(XMMATRIX new_rot)
{
	rot_matrix = new_rot;
}

float GameObject::getRoll()
{
	
	return object_rot.x;
}

float GameObject::getPitch()
{
	return object_rot.y;
}

float GameObject::getYaw()
{
	return object_rot.z;
}

void GameObject::setRoll(float new_roll)
{
	object_rot.x = new_roll;
}

void GameObject::setPitch(float new_pitch)
{
	object_rot.y = new_pitch;
}

void GameObject::setYaw(float new_yaw)
{
	object_rot.z = new_yaw;
}

XMFLOAT3 GameObject::getScaling()
{
	return object_scale;
}

void GameObject::setScaling(XMFLOAT3 new_scale)
{
	object_scale = new_scale;
}

bool GameObject::Frame()
{
	XMMATRIX translation_matrix;
	translation_matrix = XMMatrixTranslation(object_pos.x, object_pos.y, object_pos.z);
	rot_matrix = XMMatrixRotationRollPitchYaw(object_rot.x, object_rot.y, object_rot.z);
	scale_matrix = XMMatrixScaling(object_scale.x, object_scale.y, object_scale.z);
	world_matrix = translation_matrix * rot_matrix * scale_matrix;
	return true;
}

XMMATRIX GameObject::getWorldMatrix()
{
	return world_matrix;
}
