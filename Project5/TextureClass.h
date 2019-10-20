#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <d3d11.h>
#include <stdio.h>

//loading and accessing of a texture resource
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass& other);
	~TextureClass();

	bool Init(ID3D11Device* device, ID3D11DeviceContext* device_context, char* file_name);
	void Exit();
	ID3D11ShaderResourceView* GetTexture();

private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	bool LoadTarga(char* file_name, int& height, int& width);

	unsigned char* m_targa_data;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_texture_view; //used by shader to access texture data
};


#endif // !_TEXTURECLASS_H_




