#include "sprite.h"

Dirt::Dirt(float xLocation, float yLocation)
{
    m_coordinates.m_xLocation = xLocation;
    m_coordinates.m_yLocation = yLocation;
}

Dirt::~Dirt()
{

}

void Dirt::Draw(GLuint &textureID, float cellWidth, float cellHeight)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_xLocation, m_coordinates.m_yLocation, 0);

    // White
    glColor3f(1.0f,1.0f,1.0f);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
        glTexCoord2f(1.0f/8*1, 1.0f/12*7);
        glVertex2f(0, 0);
        glTexCoord2f(1.0f/8*2, 1.0f/12*7);
        glVertex2f(cellWidth, 0);
        glTexCoord2f(1.0f/8*2, 1.0f/12*8);
        glVertex2f(cellWidth, cellHeight);
        glTexCoord2f(1.0f/8*1, 1.0f/12*8);
        glVertex2f(0, cellHeight);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, NULL);
}

Wall::Wall(float xLocation, float yLocation)
{
    m_coordinates.m_xLocation = xLocation;
    m_coordinates.m_yLocation = yLocation;
}

Wall::~Wall()
{

}

void Wall::Draw(GLuint &textureID, float cellWidth, float cellHeight)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_xLocation, m_coordinates.m_yLocation, 0);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
        glTexCoord2f(1.0f/8*3, 1.0f/12*6);
        glVertex2f(0, 0);
        glTexCoord2f(1.0f/8*4, 1.0f/12*6);
        glVertex2f(cellWidth, 0);
        glTexCoord2f(1.0f/8*4, 1.0f/12*7);
        glVertex2f(cellWidth, cellHeight);
        glTexCoord2f(1.0f/8*3, 1.0f/12*7);
        glVertex2f(0, cellHeight);
    glEnd();
}

Boulder::Boulder(float xLocation, float yLocation)
{
    m_coordinates.m_xLocation = xLocation;
    m_coordinates.m_yLocation = yLocation;

    m_velocity = 0;
}

Boulder::~Boulder()
{

}

void Boulder::Draw(GLuint &textureID, float cellWidth, float cellHeight)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_xLocation, m_coordinates.m_yLocation, 0);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
        glTexCoord2f(1.0f/8*0, 1.0f/12*7);
        glVertex2f(0, 0);
        glTexCoord2f(1.0f/8*1, 1.0f/12*7);
        glVertex2f(cellWidth, 0);
        glTexCoord2f(1.0f/8*1, 1.0f/12*8);
        glVertex2f(cellWidth, cellHeight);
        glTexCoord2f(1.0f/8*0, 1.0f/12*8);
        glVertex2f(0, cellHeight);
    glEnd();
}

void Boulder::Move(std::string whichWay, float cellWidth, float cellHeight, std::vector<Nothing> &nothing)
{
    bool alreadyExists = false;

    // Make sure 'nothing' exists in the spot the boulder has just left
    for(int i = 0; i < nothing.size(); i++)
    {
        if((int)nothing[i].m_coordinates.m_xLocation == (int)m_coordinates.m_xLocation &&
           (int)nothing[i].m_coordinates.m_yLocation == (int)m_coordinates.m_yLocation)
        {
            alreadyExists = true;
        }
    }

    if(alreadyExists != true)
    {
        nothing.push_back(Nothing(m_coordinates.m_xLocation, m_coordinates.m_yLocation));
    }

    if(whichWay == "down")
        m_coordinates.m_yLocation += cellHeight;
    if(whichWay == "up")
        m_coordinates.m_yLocation -= cellHeight;
    if(whichWay == "left")
        m_coordinates.m_xLocation -= cellWidth;
    if(whichWay == "right")
        m_coordinates.m_xLocation += cellWidth;

    // If 'nothing' exists in the same position as the boulder after moving, delete nothing because the boulder is there
    for(int i = 0; i < nothing.size(); i++)
    {
        if((int)nothing[i].m_coordinates.m_xLocation == (int)m_coordinates.m_xLocation &&
           (int)nothing[i].m_coordinates.m_yLocation == (int)m_coordinates.m_yLocation)
        {
            nothing.erase(nothing.begin() + i);
        }
    }
}

void Boulder::Destroy(std::vector<Nothing> &nothing)
{
    bool alreadyExists = false;

    // Make sure 'nothing' exists in the spot the boulder has just left
    for(int i = 0; i < nothing.size(); i++)
    {
        if((int)nothing[i].m_coordinates.m_xLocation == (int)m_coordinates.m_xLocation &&
           (int)nothing[i].m_coordinates.m_yLocation == (int)m_coordinates.m_yLocation)
        {
            alreadyExists = true;
        }
    }

    if(alreadyExists != true)
    {
        nothing.push_back(Nothing(m_coordinates.m_xLocation, m_coordinates.m_yLocation));
    }
}

Coin::Coin(float xLocation, float yLocation)
{
    m_coordinates.m_xLocation = xLocation;
    m_coordinates.m_yLocation = yLocation;

    m_animationSequence = 0;
    m_velocity = 0;
}

Coin::~Coin()
{

}

void Coin::Draw(GLuint &textureID, float cellWidth, float cellHeight)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_xLocation, m_coordinates.m_yLocation, 0);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*10);
        glVertex2f(0, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*10);
        glVertex2f(cellWidth, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*11);
        glVertex2f(cellWidth, cellHeight);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*11);
        glVertex2f(0, cellHeight);
    glEnd();

    m_animationSequence++;

    if(m_animationSequence == 8)
        m_animationSequence = 0;
}

void Coin::Move(std::string whichWay, float cellWidth, float cellHeight, std::vector<Nothing> &nothing)
{
    bool alreadyExists = false;

    // Make sure 'nothing' exists in the spot the boulder has just left
    for(int i = 0; i < nothing.size(); i++)
    {
        if((int)nothing[i].m_coordinates.m_xLocation == (int)m_coordinates.m_xLocation &&
           (int)nothing[i].m_coordinates.m_yLocation == (int)m_coordinates.m_yLocation)
        {
            alreadyExists = true;
        }
    }

    if(alreadyExists != true)
    {
        nothing.push_back(Nothing(m_coordinates.m_xLocation, m_coordinates.m_yLocation));
    }

    if(whichWay == "down")
        m_coordinates.m_yLocation += cellHeight;
    if(whichWay == "up")
        m_coordinates.m_yLocation -= cellHeight;
    if(whichWay == "left")
        m_coordinates.m_xLocation -= cellWidth;
    if(whichWay == "right")
        m_coordinates.m_xLocation += cellWidth;

    // If 'nothing' exists in the same position as the boulder after moving, delete nothing because the boulder is there
    for(int i = 0; i < nothing.size(); i++)
    {
        if((int)nothing[i].m_coordinates.m_xLocation == (int)m_coordinates.m_xLocation &&
           (int)nothing[i].m_coordinates.m_yLocation == (int)m_coordinates.m_yLocation)
        {
            nothing.erase(nothing.begin() + i);
        }
    }
}

void Coin::Destroy(std::vector<Nothing> &nothing)
{
    bool alreadyExists = false;

    // Make sure 'nothing' exists in the spot the boulder has just left
    for(int i = 0; i < nothing.size(); i++)
    {
        if((int)nothing[i].m_coordinates.m_xLocation == (int)m_coordinates.m_xLocation &&
           (int)nothing[i].m_coordinates.m_yLocation == (int)m_coordinates.m_yLocation)
        {
            alreadyExists = true;
        }
    }

    if(alreadyExists != true)
    {
        nothing.push_back(Nothing(m_coordinates.m_xLocation, m_coordinates.m_yLocation));
    }
}

Exit::Exit(float xLocation, float yLocation)
{
    m_triggerAnimation = false;
    m_animationSequence = 1;

    m_coordinates.m_xLocation = xLocation;
    m_coordinates.m_yLocation = yLocation;
}

Exit::~Exit()
{

}

void Exit::Draw(GLuint& textureID, float cellWidth, float cellHeight)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_xLocation, m_coordinates.m_yLocation, 0);

    // White
    glColor3f(1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, textureID);

    if(m_animationSequence == 3)
        m_animationSequence = 1;

    glBegin(GL_QUADS);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*6);
        glVertex2f(0, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*6);
        glVertex2f(cellWidth, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*7);
        glVertex2f(cellWidth, cellHeight);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*7);
        glVertex2f(0, cellHeight);
    glEnd();

    if(m_triggerAnimation == true)
        m_animationSequence++;

    glBindTexture(GL_TEXTURE_2D, NULL);
}

Impassable::Impassable(float xLocation, float yLocation)
{
    m_coordinates.m_xLocation = xLocation;
    m_coordinates.m_yLocation = yLocation;
}

Impassable::~Impassable()
{

}

void Impassable::Draw(GLuint &textureID, float cellWidth, float cellHeight)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_xLocation, m_coordinates.m_yLocation, 0);

    // White
    glColor3f(1.0f,1.0f,1.0f);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
        glTexCoord2f(1.0f/8*1, 1.0f/12*6);
        glVertex2f(0, 0);
        glTexCoord2f(1.0f/8*2, 1.0f/12*6);
        glVertex2f(cellWidth, 0);
        glTexCoord2f(1.0f/8*2, 1.0f/12*7);
        glVertex2f(cellWidth, cellHeight);
        glTexCoord2f(1.0f/8*1, 1.0f/12*7);
        glVertex2f(0, cellHeight);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, NULL);
}

Nothing::Nothing(float xLocation, float yLocation)
{
    m_coordinates.m_xLocation = xLocation;
    m_coordinates.m_yLocation = yLocation;
}

Nothing::~Nothing()
{

}

void Nothing::Draw(float cellWidth, float cellHeight)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_xLocation, m_coordinates.m_yLocation, 0);

    // glColor3f();

    // glBegin();
    // glEnd();
}

Player::Player(float xLocation, float yLocation)
{
    m_coordinates.m_xLocation = xLocation;
    m_coordinates.m_yLocation = yLocation;

    m_lastCoordinates.m_xLocation = 0;
    m_lastCoordinates.m_yLocation = 0;

    m_animationSequence = 0;
    m_direction = 0;
    m_lastDirection = 4;
}

Player::~Player()
{

}

void Player::Draw(GLuint &textureID, float cellWidth, float cellHeight)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_xLocation, m_coordinates.m_yLocation, 0);

    // White
    glColor3f(1.0f,1.0f,1.0f);

    glBindTexture(GL_TEXTURE_2D, textureID);

    if(m_direction == 0)
    {
        glBegin(GL_QUADS);
            glTexCoord2f(1.0f/8*0, 1.0f/12*0);
            glVertex2f(0, 0);
            glTexCoord2f(1.0f/8*1, 1.0f/12*0);
            glVertex2f(cellWidth, 0);
            glTexCoord2f(1.0f/8*1, 1.0f/12*1);
            glVertex2f(cellWidth, cellHeight);
            glTexCoord2f(1.0f/8*0, 1.0f/12*1);
            glVertex2f(0, cellHeight);
        glEnd();
    }

    if(m_direction != 0)
    {
        glBegin(GL_QUADS);
            glTexCoord2f((1.0f/8*m_animationSequence), (1.0f/12*m_direction));
            glVertex2f(0, 0);
            glTexCoord2f((1.0f/8*(m_animationSequence+1)), (1.0f/12*m_direction));
            glVertex2f(cellWidth, 0);
            glTexCoord2f((1.0f/8*(m_animationSequence+1)), (1.0f/12*(m_direction+1)));
            glVertex2f(cellWidth, cellHeight);
            glTexCoord2f((1.0f/8*m_animationSequence), (1.0f/12*(m_direction+1)));
            glVertex2f(0, cellHeight);
        glEnd();

        m_animationSequence++;
    }

    if(m_animationSequence == 8)
        m_animationSequence = 0;

    glBindTexture(GL_TEXTURE_2D, NULL);
}

void Player::Move(std::string whichWay, float cellWidth, float cellHeight, std::vector<Nothing> &nothing)
{
    bool alreadyExists = false;

    // Make sure 'nothing' exists in the spot the player has just left
    for(int i = 0; i < nothing.size(); i++)
    {
        if((int)nothing[i].m_coordinates.m_xLocation == (int)m_coordinates.m_xLocation &&
           (int)nothing[i].m_coordinates.m_yLocation == (int)m_coordinates.m_yLocation)
        {
            alreadyExists = true;
        }
    }

    if(alreadyExists != true)
    {
        nothing.push_back(Nothing(m_coordinates.m_xLocation, m_coordinates.m_yLocation));
    }

    m_lastCoordinates.m_xLocation = m_coordinates.m_xLocation;
    m_lastCoordinates.m_yLocation = m_coordinates.m_yLocation;

    if(whichWay == "up")
    {
        m_coordinates.m_yLocation -= cellHeight;
    }

    if(whichWay == "down")
    {
        m_coordinates.m_yLocation += cellHeight;
    }

    if(whichWay == "left")
    {
        m_coordinates.m_xLocation -= cellWidth;
    }

    if(whichWay == "right")
    {
        m_coordinates.m_xLocation += cellWidth;
    }

    // If 'nothing' exists in the same position as the player after moving, delete nothing because the player is there
    for(int i = 0; i < nothing.size(); i++)
    {
        if((int)nothing[i].m_coordinates.m_xLocation == (int)m_coordinates.m_xLocation &&
           (int)nothing[i].m_coordinates.m_yLocation == (int)m_coordinates.m_yLocation)
        {
            nothing.erase(nothing.begin() + i);
        }
    }

}

Amoeba::Amoeba(float xLocation, float yLocation)
{
    m_coordinates.m_xLocation = xLocation;
    m_coordinates.m_yLocation = yLocation;

    m_animationSequence = 0;
}

Amoeba::~Amoeba()
{

}

void Amoeba::Draw(GLuint &textureID, float cellWidth, float cellHeight)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_xLocation, m_coordinates.m_yLocation, 0);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*8);
        glVertex2f(0, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*8);
        glVertex2f(cellWidth, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*9);
        glVertex2f(cellWidth, cellHeight);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*9);
        glVertex2f(0, cellHeight);
    glEnd();

    m_animationSequence++;

    if(m_animationSequence == 8)
        m_animationSequence = 0;
}

void Amoeba::Move(float cellWidth, float cellHeight, std::vector<Nothing> &nothing)
{

}

MagicWall::MagicWall(float xLocation, float yLocation)
{
    m_coordinates.m_xLocation = xLocation;
    m_coordinates.m_yLocation = yLocation;

    m_animationSequence = 4;
}

MagicWall::~MagicWall()
{

}

void MagicWall::Draw(GLuint &textureID, float cellWidth, float cellHeight)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_xLocation, m_coordinates.m_yLocation, 0);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*6);
        glVertex2f(0, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*6);
        glVertex2f(cellWidth, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*7);
        glVertex2f(cellWidth, cellHeight);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*7);
        glVertex2f(0, cellHeight);
    glEnd();

    m_animationSequence++;

    if(m_animationSequence == 8)
        m_animationSequence = 4;
}

Butterfly::Butterfly(float xLocation, float yLocation)
{
    m_coordinates.m_xLocation = xLocation;
    m_coordinates.m_yLocation = yLocation;

    m_animationSequence = 2;
}

Butterfly::~Butterfly()
{

}

void Butterfly::Draw(GLuint &textureID, float cellWidth, float cellHeight)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_xLocation, m_coordinates.m_yLocation, 0);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*11);
        glVertex2f(0, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*11);
        glVertex2f(cellWidth, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*12);
        glVertex2f(cellWidth, cellHeight);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*12);
        glVertex2f(0, cellHeight);
    glEnd();

    m_animationSequence++;

    if(m_animationSequence == 6)
        m_animationSequence = 2;
}

void Butterfly::Move(float cellWidth, float cellHeight, std::vector<Nothing> &nothing)
{

}

Firefly::Firefly(float xLocation, float yLocation)
{
    m_coordinates.m_xLocation = xLocation;
    m_coordinates.m_yLocation = yLocation;

    m_animationSequence = 0;
}

Firefly::~Firefly()
{

}

void Firefly::Draw(GLuint &textureID, float cellWidth, float cellHeight)
{
    glLoadIdentity();

    glTranslatef(m_coordinates.m_xLocation, m_coordinates.m_yLocation, 0);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*9);
        glVertex2f(0, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*9);
        glVertex2f(cellWidth, 0);
        glTexCoord2f((1.0f/8*(m_animationSequence+1)), 1.0f/12*10);
        glVertex2f(cellWidth, cellHeight);
        glTexCoord2f((1.0f/8*m_animationSequence), 1.0f/12*10);
        glVertex2f(0, cellHeight);
    glEnd();

    m_animationSequence++;

    if(m_animationSequence == 8)
        m_animationSequence = 0;
}

void Firefly::Move(float cellWidth, float cellHeight, std::vector<Nothing> &nothing)
{

}
