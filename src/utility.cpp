#include "utility.h"

void InitializeGL(int width, int height)
{
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    glEnable(GL_TEXTURE_2D);

    glViewport(50.0f, 50.0f, (float)width-100.0f, (float)height-100.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0f, (float)width, (float)height, 0.0f, 1.0f, -1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void InitializeDevIL()
{
    ilInit();
    iluInit();
    ilClearColour(255, 255, 255, 000);
}

void UtilityRenderText(int numbersToDraw, GLuint &textureID, float cellWidth, float cellHeight)
{
    std::stringstream convertToString;
    convertToString << numbersToDraw;

    std::string numberAsString = convertToString.str();

    std::vector<float*> textCoordinates;

    for(int i = 0; i < numberAsString.length(); i++)
    {
        textCoordinates.push_back(GetTextCoordinates(numberAsString[i]));
    }

    for(int k = 0; k < numberAsString.length(); k++)
    {
        // White
        glColor3f(1.0f,1.0f,1.0f);

        glBindTexture(GL_TEXTURE_2D, textureID);

        glBegin(GL_QUADS);
            glTexCoord2f(textCoordinates[k][0], textCoordinates[k][1]);
            glVertex2f(0, 0);
            glTexCoord2f(textCoordinates[k][0] + 1.0f/10*1, textCoordinates[k][1]);
            glVertex2f(cellWidth, 0);
            glTexCoord2f(textCoordinates[k][0] + 1.0f/10*1, textCoordinates[k][1] + 1.0f/4*1);
            glVertex2f(cellWidth, cellHeight);
            glTexCoord2f(textCoordinates[k][0], textCoordinates[k][1] + 1.0f/4*1);
            glVertex2f(0, cellHeight);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, NULL);

        glTranslatef(cellWidth, 0, 0);
    }

    for(int i = 0; i < textCoordinates.size(); i++)
        delete textCoordinates[i];
}

float* GetTextCoordinates(char requestedChar)
{
    float* textCoordinates = new float[2];

    switch(requestedChar)
    {
    case '0':   textCoordinates[0] = 1.0f/10*0; textCoordinates[1] = 1.0f/4*0; break;
    case '1':   textCoordinates[0] = 1.0f/10*1; textCoordinates[1] = 1.0f/4*0; break;
    case '2':   textCoordinates[0] = 1.0f/10*2; textCoordinates[1] = 1.0f/4*0; break;
    case '3':   textCoordinates[0] = 1.0f/10*3; textCoordinates[1] = 1.0f/4*0; break;
    case '4':   textCoordinates[0] = 1.0f/10*4; textCoordinates[1] = 1.0f/4*0; break;
    case '5':   textCoordinates[0] = 1.0f/10*5; textCoordinates[1] = 1.0f/4*0; break;
    case '6':   textCoordinates[0] = 1.0f/10*6; textCoordinates[1] = 1.0f/4*0; break;
    case '7':   textCoordinates[0] = 1.0f/10*7; textCoordinates[1] = 1.0f/4*0; break;
    case '8':   textCoordinates[0] = 1.0f/10*8; textCoordinates[1] = 1.0f/4*0; break;
    case '9':   textCoordinates[0] = 1.0f/10*9; textCoordinates[1] = 1.0f/4*0; break;
    default: textCoordinates[0] = 1.0f/10*0; textCoordinates[1] = 1.0f/4*0; break;
    }

    return textCoordinates;
}
