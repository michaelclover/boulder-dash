#include "utility.h"

void InitializeGL(int width, int height)
{
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0f, ORTHO_WIDTH, ORTHO_HEIGHT, 0.0f, 1.0f, -1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void InitializeDevIL()
{
    ilInit();
    iluInit();
    ilClearColour(255, 255, 255, 000);
}

void UtilityRenderText(std::string s, GLuint &texID, float cw, float ch)
{
  std::vector<float*> coordinates;
  for(auto& i : s)
  {
    coordinates.push_back(GetCoordinates(i));
  }

  for(int k = 0; k < coordinates.size(); k++)
  {
      // White
      glColor3f(1.0f,1.0f,1.0f);

      glBindTexture(GL_TEXTURE_2D, texID);

      glBegin(GL_QUADS);
          glTexCoord2f(coordinates[k][0], coordinates[k][1]);
          glVertex2f(0, 0);
          glTexCoord2f(coordinates[k][0] + 1.0f/10*1, coordinates[k][1]);
          glVertex2f(cw, 0);
          glTexCoord2f(coordinates[k][0] + 1.0f/10*1, coordinates[k][1] + 1.0f/4*1);
          glVertex2f(cw, ch);
          glTexCoord2f(coordinates[k][0], coordinates[k][1] + 1.0f/4*1);
          glVertex2f(0, ch);
      glEnd();

      glBindTexture(GL_TEXTURE_2D, NULL);

      glTranslatef(cw, 0, 0);
  }

  for(int i = 0; i < coordinates.size(); i++)
      delete coordinates[i];
}

void UtilityRenderText(int num, GLuint &texID, float cw, float ch)
{
    std::stringstream convertToString;
    convertToString << num;

    std::string numberAsString = convertToString.str();

    std::vector<float*> coordinates;

    for(int i = 0; i < numberAsString.length(); i++)
    {
        coordinates.push_back(GetCoordinates(numberAsString[i]));
    }

    for(int k = 0; k < numberAsString.length(); k++)
    {
        // White
        glColor3f(1.0f,1.0f,1.0f);

        glBindTexture(GL_TEXTURE_2D, texID);

        glBegin(GL_QUADS);
            glTexCoord2f(coordinates[k][0], coordinates[k][1]);
            glVertex2f(0, 0);
            glTexCoord2f(coordinates[k][0] + 1.0f/10*1, coordinates[k][1]);
            glVertex2f(cw, 0);
            glTexCoord2f(coordinates[k][0] + 1.0f/10*1, coordinates[k][1] + 1.0f/4*1);
            glVertex2f(cw, ch);
            glTexCoord2f(coordinates[k][0], coordinates[k][1] + 1.0f/4*1);
            glVertex2f(0, ch);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, NULL);

        glTranslatef(cw, 0, 0);
    }

    for(int i = 0; i < coordinates.size(); i++)
        delete coordinates[i];
}

float* GetCoordinates(char requestedChar)
{
    float* coordinates = new float[2];

    switch(requestedChar)
    {
    case '0':   coordinates[0] = 1.0f/10*0; coordinates[1] = 1.0f/4*0; break;
    case '1':   coordinates[0] = 1.0f/10*1; coordinates[1] = 1.0f/4*0; break;
    case '2':   coordinates[0] = 1.0f/10*2; coordinates[1] = 1.0f/4*0; break;
    case '3':   coordinates[0] = 1.0f/10*3; coordinates[1] = 1.0f/4*0; break;
    case '4':   coordinates[0] = 1.0f/10*4; coordinates[1] = 1.0f/4*0; break;
    case '5':   coordinates[0] = 1.0f/10*5; coordinates[1] = 1.0f/4*0; break;
    case '6':   coordinates[0] = 1.0f/10*6; coordinates[1] = 1.0f/4*0; break;
    case '7':   coordinates[0] = 1.0f/10*7; coordinates[1] = 1.0f/4*0; break;
    case '8':   coordinates[0] = 1.0f/10*8; coordinates[1] = 1.0f/4*0; break;
    case '9':   coordinates[0] = 1.0f/10*9; coordinates[1] = 1.0f/4*0; break;
    case 'a':   coordinates[0] = 1.0f/10*0; coordinates[1] = 1.0f/4*1; break;
    case 'b':   coordinates[0] = 1.0f/10*1; coordinates[1] = 1.0f/4*1; break;
    case 'c':   coordinates[0] = 1.0f/10*2; coordinates[1] = 1.0f/4*1; break;
    case 'd':   coordinates[0] = 1.0f/10*3; coordinates[1] = 1.0f/4*1; break;
    case 'e':   coordinates[0] = 1.0f/10*4; coordinates[1] = 1.0f/4*1; break;
    case 'f':   coordinates[0] = 1.0f/10*5; coordinates[1] = 1.0f/4*1; break;
    case 'g':   coordinates[0] = 1.0f/10*6; coordinates[1] = 1.0f/4*1; break;
    case 'h':   coordinates[0] = 1.0f/10*7; coordinates[1] = 1.0f/4*1; break;
    case 'i':   coordinates[0] = 1.0f/10*8; coordinates[1] = 1.0f/4*1; break;
    case 'j':   coordinates[0] = 1.0f/10*9; coordinates[1] = 1.0f/4*1; break;
    case 'k':   coordinates[0] = 1.0f/10*0; coordinates[1] = 1.0f/4*2; break;
    case 'l':   coordinates[0] = 1.0f/10*1; coordinates[1] = 1.0f/4*2; break;
    case 'm':   coordinates[0] = 1.0f/10*2; coordinates[1] = 1.0f/4*2; break;
    case 'n':   coordinates[0] = 1.0f/10*3; coordinates[1] = 1.0f/4*2; break;
    case 'o':   coordinates[0] = 1.0f/10*4; coordinates[1] = 1.0f/4*2; break;
    case 'p':   coordinates[0] = 1.0f/10*5; coordinates[1] = 1.0f/4*2; break;
    case 'q':   coordinates[0] = 1.0f/10*6; coordinates[1] = 1.0f/4*2; break;
    case 'r':   coordinates[0] = 1.0f/10*7; coordinates[1] = 1.0f/4*2; break;
    case 's':   coordinates[0] = 1.0f/10*8; coordinates[1] = 1.0f/4*2; break;
    case 't':   coordinates[0] = 1.0f/10*9; coordinates[1] = 1.0f/4*2; break;
    case 'u':   coordinates[0] = 1.0f/10*0; coordinates[1] = 1.0f/4*3; break;
    case 'v':   coordinates[0] = 1.0f/10*1; coordinates[1] = 1.0f/4*3; break;
    case 'w':   coordinates[0] = 1.0f/10*2; coordinates[1] = 1.0f/4*3; break;
    case 'x':   coordinates[0] = 1.0f/10*3; coordinates[1] = 1.0f/4*3; break;
    case 'y':   coordinates[0] = 1.0f/10*4; coordinates[1] = 1.0f/4*3; break;
    case 'z':   coordinates[0] = 1.0f/10*5; coordinates[1] = 1.0f/4*3; break;
    case ' ':   coordinates[0] = 1.0f/10*6; coordinates[1] = 1.0f/4*3; break;
    default: coordinates[0] = 1.0f/10*0; coordinates[1] = 1.0f/4*0; break;
    }

    return coordinates;
}
