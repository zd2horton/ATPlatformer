#include "CameraClass.h"

CameraClass::CameraClass()
{
	m_position_x = 0.0f;
	m_position_y = 0.0f;
	m_position_z = 0.0f;

	m_rotation_x = 0.0f;
	m_rotation_y = 0.0f;
	m_rotation_z = 0.0f;
}


CameraClass::~CameraClass()
{

}

CameraClass::CameraClass(const CameraClass& other)
{

}

void CameraClass::setPosition(float x, float y, float z)
{
	m_position_x = x;
	m_position_y = y;
	m_position_z = z;
	return;
}

void CameraClass::setRotation(float x, float y, float z)
{
	m_rotation_x = x;
	m_rotation_y = y;
	m_rotation_z = z;
	return;
}

XMFLOAT3 CameraClass::getPosition()
{
	return XMFLOAT3(m_position_x, m_position_y, m_position_z);
}

XMFLOAT3 CameraClass::getRotation()
{
	return XMFLOAT3(m_rotation_x, m_rotation_y, m_rotation_z);
}

void CameraClass::render()
{
	//uses camera position and rotation to build and update view matrix
	XMFLOAT3 up;
	XMFLOAT3 position;
	XMFLOAT3 look_at;

	XMVECTOR up_vector;
	XMVECTOR position_vector;
	XMVECTOR look_at_vector;

	float yaw;
	float pitch;
	float roll;

	XMMATRIX rotation_matrix;


	//set up up vectors
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	//load into XMVECTOR structure
	up_vector = XMLoadFloat3(&up);

	//set up camera position
	position.x = m_position_x;
	position.y = m_position_y;
	position.z = m_position_z;
	position_vector = XMLoadFloat3(&position);

	//setup where camera is looking originally
	look_at.x = 0.0f;
	look_at.y = 0.0f;
	look_at.z = 1.0f;
	look_at_vector = XMLoadFloat3(&look_at);

	//set rotations in radians
	pitch = m_rotation_x * 0.0174532925f;
	yaw = m_rotation_y * 0.0174532925f;
	roll = m_rotation_z * 0.0174532925f;
	//create rotation matrix
	rotation_matrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//transform look_at and up vectors by rotation matrix to set view correctly
	look_at_vector = XMVector3TransformCoord(look_at_vector, rotation_matrix);
	up_vector = XMVector3TransformCoord(up_vector, rotation_matrix);

	//translate rotated camera to viewer
	look_at_vector = XMVectorAdd(position_vector, look_at_vector);

	//create view matrix
	m_view_matrix = XMMatrixLookAtLH(position_vector, look_at_vector, up_vector);
	return;
}

void CameraClass::getViewMatrix(XMMATRIX& view_matrix)
{
	view_matrix = m_view_matrix;
}
