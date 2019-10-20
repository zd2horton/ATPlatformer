#include "TextureClass.h"

TextureClass::TextureClass()
{
	m_targa_data = 0;
	m_texture = 0;
	m_texture_view = 0;
}

TextureClass::TextureClass(const TextureClass& other)
{

}


TextureClass::~TextureClass()
{

}

bool TextureClass::Init(ID3D11Device* device, ID3D11DeviceContext* device_context, char* file_name)
{
	bool output;
	int height;
	int width;
	unsigned int row_pitch;
	D3D11_TEXTURE2D_DESC texture_desc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	HRESULT h_result;

	//load targa file into data array, passes back height and width
	output = LoadTarga(file_name, width, height);

	if (!output)
	{
		return false;
	}

	//setup of directX texture
	texture_desc.ArraySize = 1;
	texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texture_desc.CPUAccessFlags = 0;
	texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture_desc.Height = height;
	texture_desc.MipLevels = 0;
	texture_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.SampleDesc.Quality = 0;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.Width = width;

	//create empty texture
	h_result = device->CreateTexture2D(&texture_desc, NULL, &m_texture);
	if (FAILED(h_result))
	{
		return false;
	}

	row_pitch = (width * 4) * sizeof(unsigned char);
	
	//copies targa data array into directX texture
	//map and unmap used for data reloaded each frame, updateresource used for data loaded rarely
	//(uses higher speed memory getting cache retention preference)

	//copy targa image data into texture
	device_context->UpdateSubresource(m_texture, 0, NULL, m_targa_data, row_pitch, 0);

	//create shader resource view giving a pointer to set texture in shaders
	//description gives full range of mipmap lvls fr better texture rendering
	srv_desc.Format = texture_desc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MostDetailedMip = 0;
	srv_desc.Texture2D.MipLevels = -1;
	h_result = device->CreateShaderResourceView(m_texture, &srv_desc, &m_texture_view);

	if (FAILED(h_result))
	{
		return false;
	}

	//generate mipmaps
	device_context->GenerateMips(m_texture_view);
	delete[] m_targa_data;
	m_targa_data = 0;
	return true;
}

void TextureClass::Exit()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	if (m_texture_view)
	{
		m_texture_view->Release();
		m_texture_view = 0;
	}

	if (m_targa_data)
	{
		delete[] m_targa_data;
		m_targa_data = 0;
	}
}

ID3D11ShaderResourceView * TextureClass::GetTexture()
{
	return m_texture_view;
}

bool TextureClass::LoadTarga(char* file_name, int& height, int& width)
{
	//targas stored upside down, must be flipped
	int error;
	int bpp;
	int image_size;
	int index;
	int k;
	unsigned int count;
	FILE* file_ptr;
	TargaHeader targa_file_header;
	unsigned char* targa_image;

	error = fopen_s(&file_ptr, file_name, "rb");
	if (error != 0)
	{
		return false;
	}
	//file header read
	count = (unsigned int)fread(&targa_file_header, sizeof(TargaHeader), 1, file_ptr);
	if (count != 1)
	{
		return false;
	}

	height = (int)targa_file_header.height;
	width = (int)targa_file_header.width;
	bpp = (int)targa_file_header.bpp;
	if (bpp != 32)
	{
		return false;
	}
	//calc size of 32 bit image data, allocate memory for data and read in
	image_size = width * height * 4;
	targa_image = new unsigned char[image_size];
	if (!targa_image)
	{
		return false;
	}
	count = (unsigned int)fread(targa_image, 1, image_size, file_ptr);
	if (count != image_size)
	{
		return false;
	}

	error = fclose(file_ptr);
	if (error != 0)
	{
		return false;
	}

	//allocate memory for targa destination data, init index into array and image data
	m_targa_data = new unsigned char[image_size];
	if (!m_targa_data)
	{
		return false;
	}
	index = 0;
	k = (width * height * 4) - (width * 4);
	
	//targa image data copied into destination array in correct order (RGBA)
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			m_targa_data[index + 0] = targa_image[k + 2];
			m_targa_data[index + 1] = targa_image[k + 1];
			m_targa_data[index + 2] = targa_image[k + 0];
			m_targa_data[index + 3] = targa_image[k + 3];
			k += 4;
			index += 4;
		}
		//targa image data index set back to preceding row at beginning of column
		k -= (8 * width);
	}
	delete[] targa_image;
	targa_image = 0;
	return true;
}
