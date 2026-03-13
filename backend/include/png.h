#include <cstring>
#include <libpng/png.h>
#include <string>
typedef unsigned char Byte;

struct image {
	int width;
	int height;
	int channel;
	png_bytep* data;

	image(int width = 0, int height = 0) {
		this->width = width, this->height = height;
		this->data = new png_bytep[height];
        channel = 4;
		for (int i = 0; i < height; i++) this->data[i] = reinterpret_cast<png_bytep>(new Byte[width * 4]);
	}

	image(const image &a) {
		width = a.width;
		height = a.height;
		channel = a.channel;
		this->data = new png_bytep[height];
		for (int i = 0; i < height; i++) this->data[i] = reinterpret_cast<png_bytep>(new Byte[width * 4]);
		for (int i = 0; i < height; i++) memcpy(this->data[i], a.data[i], width * 4);
	}

	image& operator = (const image &a) {
		for (int i = 0; i < height; i++) delete[] this->data[i];
		delete[] this->data;
		width = a.width;
		height = a.height;
		channel = a.channel;
		this->data = new png_bytep[height];
		for (int i = 0; i < height; i++) this->data[i] = reinterpret_cast<png_bytep>(new Byte[width * 4]);
		for (int i = 0; i < height; i++) memcpy(this->data[i], a.data[i], width * 4);
		return *this;
	}
	
	~image() {
		for (int i = 0; i < height; i++) delete[] this->data[i];
		delete[] this->data;
	}
};

image readImage(std::string path) {
	FILE *fp = fopen(path.c_str(), "rb");
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
	png_init_io(png_ptr, fp);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

	image img;
	img.width = png_get_image_width(png_ptr, info_ptr);
	img.height = png_get_image_height(png_ptr, info_ptr);
	img.data = png_get_rows(png_ptr, info_ptr);
	img.channel = png_get_channels(png_ptr, info_ptr);
    fclose(fp);
	return img;
}
void pngReadFromString(png_structp png_ptr, png_bytep data, png_size_t length) {
    std::string* input = (std::string*)png_get_io_ptr(png_ptr);
    if (input->size() < length) png_error(png_ptr, "Input string too short");
    std::memcpy(data, input->c_str(), length);
    input->erase(0, length);
}
image readImageFromString(std::string content) {
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
    png_set_read_fn(png_ptr, &content, pngReadFromString);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

	image img;
	img.width = png_get_image_width(png_ptr, info_ptr);
	img.height = png_get_image_height(png_ptr, info_ptr);
	img.data = png_get_rows(png_ptr, info_ptr);
	img.channel = png_get_channels(png_ptr, info_ptr);
	return img;
}

void writeImage(std::string path, image img) {
	FILE *fp = fopen(path.c_str(), "wb");
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, img.width, img.height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);
	png_write_image(png_ptr, img.data);
	png_write_end(png_ptr, NULL);
	fclose(fp);
}
void pngWriteToString(png_structp png_ptr, png_bytep data, png_size_t length) {
    std::string* output = (std::string*)png_get_io_ptr(png_ptr);
    output->append((char*)data, length);
}
void pngFlushString(png_structp png_ptr) {}
std::string writeImageToString(image img) {
    std::string res;
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
    png_set_write_fn(png_ptr, &res, pngWriteToString, pngFlushString);
	png_set_IHDR(png_ptr, info_ptr, img.width, img.height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);
	png_write_image(png_ptr, img.data);
	png_write_end(png_ptr, NULL);
    return res;
}