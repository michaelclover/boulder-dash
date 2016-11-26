#include "sprite.h"

Dirt::~Dirt()
{
}

void Dirt::Draw(float cw, float ch)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_x, m_coordinates.m_y, 0);
    
    // White
    glColor3f(1.0f,1.0f,1.0f);

    glBindTexture(GL_TEXTURE_2D, *m_textureID);

    glBegin(GL_QUADS);
        glTexCoord2f(1.0f/8*1, 1.0f/12*7);
        glVertex2f(0, 0);
        glTexCoord2f(1.0f/8*2, 1.0f/12*7);
        glVertex2f(cw, 0);
        glTexCoord2f(1.0f/8*2, 1.0f/12*8);
        glVertex2f(cw, ch);
        glTexCoord2f(1.0f/8*1, 1.0f/12*8);
        glVertex2f(0, ch);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, NULL);
}

Wall::~Wall()
{

}

void Wall::Draw(float cw, float ch)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_x, m_coordinates.m_y, 0);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, *m_textureID);

    glBegin(GL_QUADS);
        glTexCoord2f(1.0f/8*3, 1.0f/12*6);
        glVertex2f(0, 0);
        glTexCoord2f(1.0f/8*4, 1.0f/12*6);
        glVertex2f(cw, 0);
        glTexCoord2f(1.0f/8*4, 1.0f/12*7);
        glVertex2f(cw, ch);
        glTexCoord2f(1.0f/8*3, 1.0f/12*7);
        glVertex2f(0, ch);
    glEnd();
}

Boulder::~Boulder()
{
}

void Boulder::Draw(float cw, float ch)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_x, m_coordinates.m_y, 0);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, *m_textureID);

    glBegin(GL_QUADS);
        glTexCoord2f(1.0f/8*0, 1.0f/12*7);
        glVertex2f(0, 0);
        glTexCoord2f(1.0f/8*1, 1.0f/12*7);
        glVertex2f(cw, 0);
        glTexCoord2f(1.0f/8*1, 1.0f/12*8);
        glVertex2f(cw, ch);
        glTexCoord2f(1.0f/8*0, 1.0f/12*8);
        glVertex2f(0, ch);
    glEnd();
}

Coin::~Coin()
{
}

void Coin::Draw(float cw, float ch)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_x, m_coordinates.m_y, 0);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, *m_textureID);

    glBegin(GL_QUADS);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*10);
        glVertex2f(0, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*10);
        glVertex2f(cw, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*11);
        glVertex2f(cw, ch);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*11);
        glVertex2f(0, ch);
    glEnd();

    m_animationSequence++;

    if(m_animationSequence == 8)
        m_animationSequence = 0;
}

Exit::~Exit()
{
}

void Exit::Draw(float cw, float ch)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_x, m_coordinates.m_y, 0);

    // White
    glColor3f(1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, *m_textureID);

    if(m_animationSequence == 3)
        m_animationSequence = 1;

    glBegin(GL_QUADS);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*6);
        glVertex2f(0, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*6);
        glVertex2f(cw, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*7);
        glVertex2f(cw, ch);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*7);
        glVertex2f(0, ch);
    glEnd();

    if(m_triggerAnimation == true)
        m_animationSequence++;

    glBindTexture(GL_TEXTURE_2D, NULL);
}

Impassable::~Impassable()
{
}

void Impassable::Draw(float cw, float ch)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_x, m_coordinates.m_y, 0);

    // White
    glColor3f(1.0f,1.0f,1.0f);

    glBindTexture(GL_TEXTURE_2D, *m_textureID);

    glBegin(GL_QUADS);
        glTexCoord2f(1.0f/8*1, 1.0f/12*6);
        glVertex2f(0, 0);
        glTexCoord2f(1.0f/8*2, 1.0f/12*6);
        glVertex2f(cw, 0);
        glTexCoord2f(1.0f/8*2, 1.0f/12*7);
        glVertex2f(cw, ch);
        glTexCoord2f(1.0f/8*1, 1.0f/12*7);
        glVertex2f(0, ch);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, NULL);
}

Player::~Player()
{
}

void Player::Draw(float cw, float ch)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_x, m_coordinates.m_y, 0);

    glColor3f(1.0f,1.0f,1.0f);

    glBindTexture(GL_TEXTURE_2D, *m_textureID);

    if(m_direction == 0)
    {
        glBegin(GL_QUADS);
            glTexCoord2f(1.0f/8*0, 1.0f/12*0);
            glVertex2f(0, 0);
            glTexCoord2f(1.0f/8*1, 1.0f/12*0);
            glVertex2f(cw, 0);
            glTexCoord2f(1.0f/8*1, 1.0f/12*1);
            glVertex2f(cw, ch);
            glTexCoord2f(1.0f/8*0, 1.0f/12*1);
            glVertex2f(0, ch);
        glEnd();
    }
    else
    {
        glBegin(GL_QUADS);
            glTexCoord2f((1.0f/8*m_animationSequence), (1.0f/12*m_direction));
            glVertex2f(0, 0);
            glTexCoord2f((1.0f/8*(m_animationSequence+1)), (1.0f/12*m_direction));
            glVertex2f(cw, 0);
            glTexCoord2f((1.0f/8*(m_animationSequence+1)), (1.0f/12*(m_direction+1)));
            glVertex2f(cw, ch);
            glTexCoord2f((1.0f/8*m_animationSequence), (1.0f/12*(m_direction+1)));
            glVertex2f(0, ch);
        glEnd();

        m_animationSequence++;
    }

    if(m_animationSequence == 8)
        m_animationSequence = 0;

    glBindTexture(GL_TEXTURE_2D, NULL);
}

Amoeba::~Amoeba()
{
}

void Amoeba::Draw(float cw, float ch)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_x, m_coordinates.m_y, 0);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, *m_textureID);

    glBegin(GL_QUADS);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*8);
        glVertex2f(0, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*8);
        glVertex2f(cw, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*9);
        glVertex2f(cw, ch);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*9);
        glVertex2f(0, ch);
    glEnd();

    m_animationSequence++;

    if(m_animationSequence == 8)
        m_animationSequence = 0;
}

MagicWall::~MagicWall()
{
}

void MagicWall::Draw(float cw, float ch)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_x, m_coordinates.m_y, 0);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, *m_textureID);

    glBegin(GL_QUADS);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*6);
        glVertex2f(0, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*6);
        glVertex2f(cw, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*7);
        glVertex2f(cw, ch);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*7);
        glVertex2f(0, ch);
    glEnd();

    m_animationSequence++;

    if(m_animationSequence == 8)
        m_animationSequence = 4;
}

Butterfly::~Butterfly()
{
}

void Butterfly::Draw(float cw, float ch)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_x, m_coordinates.m_y, 0);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, *m_textureID);

    glBegin(GL_QUADS);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*11);
        glVertex2f(0, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*11);
        glVertex2f(cw, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*12);
        glVertex2f(cw, ch);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*12);
        glVertex2f(0, ch);
    glEnd();

    m_animationSequence++;

    if(m_animationSequence == 6)
        m_animationSequence = 2;
}

Firefly::~Firefly()
{
}

void Firefly::Draw(float cw, float ch)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_x, m_coordinates.m_y, 0);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, *m_textureID);

    glBegin(GL_QUADS);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*9);
        glVertex2f(0, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*9);
        glVertex2f(cw, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*10);
        glVertex2f(cw, ch);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*10);
        glVertex2f(0, ch);
    glEnd();

    m_animationSequence++;

    if(m_animationSequence == 8)
        m_animationSequence = 0;
}
