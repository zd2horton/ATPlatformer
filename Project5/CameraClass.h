#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <directxmath.h>
using namespace DirectX;

class CameraClass
{
public:
	CameraClass();
	~CameraClass();
	CameraClass(const CameraClass& other);

	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);

	XMFLOAT3 getPosition();
	XMFLOAT3 getRotation();

	void render();
	void getViewMatrix(XMMATRIX& view_matrix);

private:
	float m_position_x;
	float m_position_y;
	float m_position_z;
	float m_rotation_x; 
	float m_rotation_y;
	float m_rotation_z;
	XMMATRIX m_view_matrix;
};
#endif // !_CAMERACLASS_H_

