#include "texture.h"

TextureSheet::TextureSheet()
{

}

TextureSheet::~TextureSheet()
{

}

bool TextureSheet::LoadTextureFromFile(std::string file)
{
    bool isTextureLoaded = false;

    // Generate the image ID and bind it temporarily
    ILuint imageID = 0;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    // Load image
    ILboolean success = ilLoadImage(file.c_str());

    // If the image has loaded successfully
    if(success == IL_TRUE)
    {
        // Convert the image into RGBA format
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

        // If there were no errors during the conversion
        if(success == IL_TRUE)
        {
            // Get the width and height of the loaded image
            m_imageWidth = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
            m_imageHeight = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

            // Create the texture from loaded pixels and convert it for use with OpenGL
            isTextureLoaded = LoadTextureFromPixels((GLuint*)ilGetData());
        }

        // Delete file from temporary memory
        ilDeleteImages(1, &imageID);
    }

    // Report error if there is one
    if(!isTextureLoaded)
    {
        std::cout << "Unable to load image from file, cannot find: " << file << std::endl;
    }

    return isTextureLoaded;
}

bool TextureSheet::LoadTextureFromPixels(GLuint* pixels)
{
    // Generate OpenGL texture identifier
    glGenTextures(1, &m_texID);

    // Bind the texture ID
    glBindTexture(GL_TEXTURE_2D, m_texID);

    // Generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_imageWidth, m_imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    // Set the way in which the texture is to be displayed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Unbind the texture from OpenGL's memory
    glBindTexture(GL_TEXTURE_2D, NULL);

    // If there is an OpenGL error, report it
    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::cout << "Error generating texture from pixels" << std::endl;
        return false;
    }

    return true;
}
