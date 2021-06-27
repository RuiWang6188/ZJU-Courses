#include"tga.h"

static GLubyte TGA_TYPE_HEADER[8] = { 0, 0, 2, 0, 0, 0, 0, 0 };


bool texture::loadTga(const char* path) {

    FILE* file = fopen(path, "rb");

    if (!file) {
        perror("fopen");
        return false;
    }

    GLubyte type_header[8];
    if (fread(type_header, 1, sizeof(type_header), file) != sizeof(type_header)) {
        perror("fread: TGA header type specification");
        fclose(file);
        return false;
    }

    if (memcmp(TGA_TYPE_HEADER, type_header, sizeof(type_header)) != 0) {
        fprintf(stderr, "load_tga: Unsupported TGA type:");
        for (size_t i = 0; i < sizeof(type_header) / sizeof(type_header[0]); ++i) {
            fprintf(stderr, " %02X", type_header[i]);
        }
        fprintf(stderr, "\n");
        fclose(file);
        return false;
    }

    GLubyte image_header[10];
    if (fread(image_header, 1, sizeof(image_header), file) != sizeof(image_header)) {
        perror("fread: TGA header image specification");
        fclose(file);
        return false;
    }

    this->width = image_header[5] * 256u + image_header[4];
    this->height = image_header[7] * 256u + image_header[6];

    if ((this->width <= 0) || (this->height <= 0)) {
        fprintf(stderr, "load_tga: Invalid image size\n");
        fclose(file);
        return false;
    }

    GLubyte depth = image_header[8];
    switch (depth) {
    case 24:
        this->format = GL_RGB;
        break;
    case 32:
        this->format = GL_RGBA;
        break;
    default:
        fprintf(stderr, "load_tga: Invalid color depth\n");
        fclose(file);
        return false;
    }

    size_t pixel_size = depth / 8u;
    size_t data_size = this->width * this->height * pixel_size;
    this->data = (GLubyte*)malloc(data_size);
    if (!this->data) {
        perror("malloc");
        fclose(file);
        return false;
    }

    if (fread(this->data, 1, data_size, file) != data_size) {
        perror("fread: TGA image data");
        free(this->data);
        fclose(file);
        return false;
    }

    // TGA stores image data as BGR(A).
    for (size_t i = 0; i < data_size; i += pixel_size) {
        // See https://en.wikipedia.org/wiki/XOR_swap_algorithm#Reasons_for_avoidance_in_practice .
        GLubyte temp = this->data[i];
        this->data[i] = this->data[i + 2];
        this->data[i + 2] = temp;
    }

    fclose(file);
    return true;
}


GLuint loadTexture(const char* path) {
    texture texture;
    if (!texture.loadTga(path)) {
        exit(1);
    }
    GLuint name;
    glGenTextures(1, &name);
    glBindTexture(GL_TEXTURE_2D, name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, texture.data);
    free(texture.data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return name;
}