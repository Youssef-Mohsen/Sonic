#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <cmath>
#include <fstream>
using namespace std;
using namespace sf;


////////////
// Structs//
////////////
struct player
{
    Texture sonicTex;
    Sprite player;
    RectangleShape PlayerColl;
    Vector2f Velocity;
    int animright = 0, idleanim = 0, animleft = 0, jumpanim = 0, deadanim = 0;
    float deathdelay = 0.3, timer = 0, acc = 0;
    int score = 0, lives = 5, highscore = 0;
    bool start = false, Running = false;
    bool isground = false, isdead = false, RunningSound = false, check[3] = { false };
};
struct enemies1
{
    Sprite enemie;
    Vector2f Velocity;
    RectangleShape coll;
    float timer = 0;
    int animright = 0, animleft = 0, animup = 0;
    bool isdead = false, isground = false;
};
struct Coll
{
    Sprite coll;
    int anim = 0;
    float timer = 0;
};
////////////
// Function//
////////////
void animate_sprite(Sprite& sprite, int num, float y, float width, float height, int& animationInd, float& delay, float& deltatime, float& timer, int shift);
void move_left(Sprite& sonic, Vector2f& sonicVelocity, Vector2f& Velocity, Vector2f& Velocity2, float& acc, int& animleft, float& deltatime, float& timer, bool isground, bool isdea, bool soniccheck);
void move_right(Sprite& sonic, Vector2f& sonicVelocity, Vector2f& Velocity, Vector2f& Velocity2, float& acc, int& animright, float& deltatime, float& timer, bool isground, bool isdead, bool soniccheck);
void animate_stack(struct Coll sprites[], int size, int y, int num, int width, int height, float& delay, float& deltatime);
void setGround(struct Coll item[], int num, int width, int height, int x, int y);
void construct(struct Coll item[], int start, int amount, float x, int differenceX, int differenceY, float y);
void enemierespawn(struct enemies1 sprite[], RenderWindow& window, int x, int z, int start, int amount);
void enemiepos(struct enemies1 fish[], int x, int z, int start, int amount);
void move_crab0(struct enemies1 sprite[], int start, int amount);
void move_crab(struct enemies1 sprite[], int num, int start, int amount);
void crabcoll(Sprite& sonic, bool& isdead, bool& isground, RectangleShape& playercoll, Vector2f& Velocity, int& score, int& lives, struct enemies1 crab[], struct Coll ground[], Sound& enemiedeath, Sound& deathsound, int start1, int end1, int start2, int end2);
void crabanim(struct enemies1 crab[], float& deltatime);
void structanim(struct enemies1 sprite[], float& delay, float& deltatime, int start, int amount, int num, float width, float height, float width2, float height2);
void savehighscore(int highscore);
int loadhighscore();
void transition(RenderWindow& window);
void transition_reverse(RenderWindow& window);
void Game(float& delay, float& deltatime, RenderWindow& window, Clock& gameclock);
int main()
{
    // Variables
    Clock gameclock;
    float delay = 0.1f;
    float deltatime = 0;

    RenderWindow window(VideoMode(1940, 1080), "Sonic.exe", Style::Default);
    window.setFramerateLimit(60);

    Game(delay, deltatime, window, gameclock);

}
////////////////
// Definations//
////////////////
void move_left(Sprite& sonic, Vector2f& sonicVelocity, Vector2f& Velocity, Vector2f& Velocity2, float& acc, int& animleft, float& deltatime, float& timer, bool isground, bool isdead, bool soniccheck)
{
    sonicVelocity.x = -8 + acc;
    if (!soniccheck)
    {
        Velocity.x = -8 + acc;
        Velocity2.x = -8 + acc;
    }
    else
    {
        Velocity.x = 0;
        Velocity2.x = 0;
    }
    if (isground && !isdead)
    {
        if (timer < 0)
        {
            animleft++;
            animleft = animleft % 23;
            if (animleft == 22)
            {


                if (timer < 0)
                {
                    animleft = 17;
                    animleft = animleft % 23;
                    animleft++;
                    acc -= 1;

                    sonic.setTextureRect(IntRect((animleft * 48.87), 1 * 59.4, 48.87, 46));
                    timer = 0.1;
                }
                else
                    timer -= deltatime;
            }
            else
            {
                sonic.setTextureRect(IntRect((animleft * 48.87), 1 * 59.4, 48.87, 46));
                timer = 0.07;
            }
        }
        else
            timer -= deltatime;
    }

    sonic.setScale(-2, 2);
    sonic.setOrigin(0, 0);
}
void move_right(Sprite& sonic, Vector2f& sonicVelocity, Vector2f& Velocity, Vector2f& Velocity2, float& acc, int& animright, float& deltatime, float& timer, bool isground, bool isdead, bool soniccheck)
{
    sonicVelocity.x = 8 + acc;
    if (!soniccheck)
    {
        Velocity.x = 8 + acc;
        Velocity2.x = 8 + acc;
    }
    else
    {
        Velocity.x = 0;
        Velocity2.x = 0;
    }
    if (isground && !isdead)
    {
        if (timer < 0)
        {
            animright++;
            animright = animright % 23;
            if (animright == 22)
            {

                if (timer < 0)
                {
                    animright = 17;
                    animright = animright % 23;
                    animright++;
                    acc += 1;

                    sonic.setTextureRect(IntRect((animright * 48.87), 1 * 59.4, 48.87, 46));
                    timer = 0.07;
                }
                else
                    timer -= deltatime;
            }
            else
            {
                sonic.setTextureRect(IntRect((animright * 48.87), 1 * 59.4, 48.87, 46));
                timer = 0.1;
            }
        }
        else
            timer -= deltatime;
    }



    sonic.setScale(2, 2);

    sonic.setOrigin(sonic.getLocalBounds().width, 0);
}
void animate_sprite(Sprite& sprite, int num, float y, float width, float height, int& animationInd, float& delay, float& deltatime, float& timer, int shift)
{
    if (timer < 0)
    {
        int animationIndShift;

        animationInd++;
        animationInd %= num;
        animationIndShift = animationInd + shift;

        sprite.setTextureRect(IntRect((animationIndShift * width), y, width, height));
        timer = delay;
    }
    else
    {
        timer -= deltatime;
    }
}
void animate_stack(struct Coll sprites[], int size, int y, int num, int width, int height, float& delay, float& deltatime)
{
    for (int i = 0; i < size; i++)
    {
        if (sprites[i].timer < 0)
        {
            sprites[i].anim++;
            sprites[i].anim %= num;

            sprites[i].coll.setTextureRect(IntRect((sprites[i].anim * width), y, width, height));
            sprites[i].timer = delay;
        }
        else
        {
            sprites[i].timer -= deltatime;
        }
    }
}
void construct(struct Coll item[], int start, int amount, float x, int differenceX, int differenceY, float y)
{
    for (int i = start, j = 0; i < amount; i++, j++)
        item[i].coll.setPosition(Vector2f(x + (j * differenceX), y - (j * differenceY)));
}
void setGround(struct Coll item[], int num, int width, int height, int x, int y)
{
    item[num].coll.setTextureRect(IntRect(0, 0, width, height));
    item[num].coll.setPosition(x, y);
}
void enemierespawn(struct enemies1 sprite[], RenderWindow& window, int x, int z, int start, int amount)
{
    for (int i = start, j = 0; i < amount; i++, j++)
    {
        if (sprite[i].enemie.getPosition().y < window.getSize().y - 1024)
        {
            sprite[i].enemie.setPosition(x + (z * j), 940);
        }
    }
}
void enemiepos(struct enemies1 fish[], int x, int z, int start, int amount)
{
    for (int i = start, j = 0; i < amount; i++, j++)
    {
        fish[i].enemie.setPosition(x + (z * j), 940);
    }
}
void move_crab0(struct enemies1 sprite[], int start, int amount)
{
    for (int i = start; i < amount; i++)
    {
        int num = 0;
        if (i == 0)
            num = 950;
        if (i == 1)
            num = 4500;
        if (i == 2)
            num = 9500;
        if (i == 3)
            num = 10500;
        if (sprite[i].enemie.getPosition().x <= num && !sprite[i].isdead)
        {
            sprite[i].Velocity.x = 3;

        }
        else if (sprite[i].enemie.getPosition().x >= (num + 480) && !sprite[i].isdead)
        {
            sprite[i].Velocity.x = -3;

        }
    }
}
void move_crab(struct enemies1 sprite[], int num, int start, int amount)
{
    for (int i = start, j = 0; i < amount; i++, j++)
    {


        if (sprite[i].enemie.getPosition().x <= num + (j * 800) && !sprite[i].isdead)
        {
            sprite[i].Velocity.x = 3;

        }
        else if (sprite[i].enemie.getPosition().x >= (num + 480) + (j * 800) && !sprite[i].isdead)
        {
            sprite[i].Velocity.x = -3;

        }
    }
}
void crabcoll(Sprite& sonic, bool& isdead, bool& isground, RectangleShape& playercoll, Vector2f& Velocity, int& score, int& lives, struct enemies1 crab[], struct Coll ground[], Sound& enemiedeath, Sound& deathsound, int start1, int end1, int start2, int end2)
{
    for (int i = start1; i < end1; i++)
    {
        if (!crab[i].isdead && !isdead)
        {
            //crab intersection with ground
            for (int j = start2; j < end2; j++)
            {
                if (crab[i].enemie.getGlobalBounds().intersects(ground[j].coll.getGlobalBounds()))
                {
                    crab[i].enemie.setPosition(crab[i].enemie.getPosition().x, ground[j].coll.getPosition().y - 65);
                    crab[i].Velocity.y = -0.01;
                    crab[i].isground = true;
                }
                else if (!crab[i].isground)
                    crab[i].Velocity.y = 5;
            }

            //crab collision with sonic

            if (playercoll.getGlobalBounds().intersects(crab[i].enemie.getGlobalBounds()))
            {
                //crab death from upper
                if (playercoll.getPosition().y + 70 <= crab[i].coll.getPosition().y && !isground)
                {
                    enemiedeath.play();
                    crab[i].isdead = true;
                    Velocity.y = -12;
                    Velocity.y += 3;
                    score += 50;
                }
                //sonic death from collision of crab
                else if (playercoll.getPosition().x + 45 >= crab[i].coll.getPosition().x)
                {
                    lives--;
                    isdead = true;
                    isground = false;
                    Velocity.y = -15;
                    deathsound.play();
                }
                else if (playercoll.getPosition().x <= crab[i].coll.getPosition().x + 38)
                {
                    lives--;
                    isdead = true;
                    isground = false;
                    Velocity.y = -15;
                    deathsound.play();
                }
            }

            // crab movement
            move_crab0(crab, 0, 4);
            move_crab(crab, 14900, 4, 8);
        }
    }
}
void structanim(struct enemies1 sprite[], float& delay, float& deltatime, int start, int amount, int num, float width, float height, float width2, float height2)
{
    for (int i = start; i < amount; i++)
    {
        if (sprite[i].timer < 0)
        {
            sprite[i].animup++;
            sprite[i].animup = sprite[i].animup % num;

            sprite[i].enemie.setTextureRect(IntRect((sprite[i].animup * width), height, width2, height2));
            sprite[i].timer = delay;
        }
        else
            sprite[i].timer -= deltatime;
    }
}
void crabanim(struct enemies1 crab[], float& deltatime)
{
    for (int i = 0; i < 35; i++)
    {
        if (crab[i].Velocity.x == 3)
        {

            crab[i].enemie.setOrigin(0, 0);
            crab[i].enemie.setScale(2, 2);
            if (crab[i].timer < 0)
            {
                crab[i].animright++;
                crab[i].animright = crab[i].animright % 4;

                crab[i].enemie.setTextureRect(IntRect((crab[i].animright * 48.5), 0 * 36, 46.5, 36));
                crab[i].timer = 0.45;
            }
            else
                crab[i].timer -= deltatime;
        }
        else if (crab[i].Velocity.x == -3)
        {
            crab[i].enemie.setOrigin(crab[i].enemie.getLocalBounds().width, 0);
            crab[i].enemie.setScale(-2, 2);
            if (crab[i].timer < 0)
            {
                crab[i].animleft++;
                crab[i].animleft = crab[i].animleft % 4;

                crab[i].enemie.setTextureRect(IntRect((crab[i].animleft * 48.5), 0 * 36, 46.5, 36));
                crab[i].timer = 0.45;
            }
            else
                crab[i].timer -= deltatime;
        }
    }
}
void savehighscore(int highscore)
{
    ofstream file("highscore.txt");
    if (file.is_open())
    {
        file << highscore;
        file.close();
    }
}
int loadhighscore()
{
    int highscore = 0;
    ifstream file("highscore.txt");
    if (file.is_open())
    {
        file >> highscore;
        file.close();
    }
    return highscore;
}
void transition(RenderWindow& window)
{
    Clock t1;
    for (int i = 4; i <= 7; i++)
    {
        window.clear(Color{ Uint8(84 - 12 * i), Uint8(84 - 12 * i), Uint8(84 - 12 * i) });
        window.display();
        while (true)
        {
            if (t1.getElapsedTime().asMilliseconds() > 300)
            {
                t1.restart();
                break;
            }
        }
    }
}
void transition_reverse(RenderWindow& window)
{
    Clock t1;
    for (int i = 0; i < 4; i++)
    {
        window.clear(Color{ Uint8(12 * i), Uint8(12 * i), Uint8(12 * i) });
        window.display();
        while (true)
        {
            if (t1.getElapsedTime().asMilliseconds() > 300)
            {
                t1.restart();
                break;
            }
        }
    }
}
void Game(float& delay, float& deltatime, RenderWindow& window, Clock& gameclock)
{
    // Structs
    // Sonic
    player sonic;
    sonic.player.setTextureRect(IntRect(48.87, 0, 48.87, 43));
    //enemies
    enemies1 crab[100];
    enemies1 fish[100];
    //Others
    Coll ring[100];
    Coll spike[50];
    Coll tree[100];
    Coll checkpoint[4];
    Coll rock[2];
    Coll ground[70];
    Coll water[70];
    Coll platform[20];

    //crab
    for (int i = 0; i < 20; i++)
        crab[i].enemie.setTextureRect(IntRect(48.5, 0 * 36, 46.5, 36));
    //fish
    for (int i = 0; i < 20; i++)
        fish[i].enemie.setTextureRect(IntRect(48.5, 0 * 36, 46.5, 36));
    // Out Of Structs
    // Velocities
    Vector2f Velocity;
    Vector2f Velocity2;

    //Textures
    Texture spiketex;
    Texture platformtex;
    Texture backgroundTexture;
    Texture groundTexture;
    Texture ringTexture;
    Texture rockTexture;
    Texture waterTexture;
    Texture liveTex;
    Texture treetex;
    Texture crabtex;
    Texture checktex;
    Texture fishtex;
    // Loading From Files
    crabtex.loadFromFile("Enemies1.png");
    fishtex.loadFromFile("Enemies2.png");
    sonic.sonicTex.loadFromFile("sonic22.png");
    platformtex.loadFromFile("Wall.png");
    spiketex.loadFromFile("Spike.png");
    backgroundTexture.loadFromFile("background.png");
    groundTexture.loadFromFile("ground2.png");
    ringTexture.loadFromFile("ring.png");
    rockTexture.loadFromFile("rock.png");
    waterTexture.loadFromFile("water.png");
    treetex.loadFromFile("tree.png");
    liveTex.loadFromFile("lives.png");
    checktex.loadFromFile("checkpoints2.png");

    // Sprites
    Sprite lives[5];
    Sprite rock1;
    Sprite rock3;
    Sprite background;

    // Setting Textures
    //sonic
    sonic.player.setTexture(sonic.sonicTex);
    //crab
    for (int i = 0; i < 35; i++)
        crab[i].enemie.setTexture(crabtex);
    //fish
    for (int i = 0; i < 20; i++)
        fish[i].enemie.setTexture(fishtex);
    //lives
    for (int i = 0; i < 5; i++)
    {
        lives[i].setTexture(liveTex);
    }
    //platform
    for (int i = 0; i < 20; i++)
        platform[i].coll.setTexture(platformtex);
    //spike
    for (int i = 0; i < 50; i++)
        spike[i].coll.setTexture(spiketex);
    //rings
    for (int i = 0; i < 100; i++)
        ring[i].coll.setTexture(ringTexture);
    //trees
    for (int i = 0; i < 100; i++)
        tree[i].coll.setTexture(treetex);
    //rock
    for (int i = 0; i < 2; i++)
        rock[i].coll.setTexture(rockTexture);
    //checkpoints
    for (int i = 0; i < 3; i++)
        checkpoint[i].coll.setTexture(checktex);
    //background
    background.setTexture(backgroundTexture);
    //rock2
    rock1.setTexture(rockTexture);
    //rock3
    rock3.setTexture(rockTexture);
    //ground and water
    for (int i = 0; i < 70; i++)
    {
        ground[i].coll.setTexture(groundTexture);
        water[i].coll.setTexture(waterTexture);
    }

    // set texture rect
    //checkpoint
    for (int i = 0; i < 3; i++)
        checkpoint[i].coll.setTextureRect(IntRect(0, 0, 120, 77));
    //background
    background.setTextureRect(IntRect(0, 0, 1920 * 33.33333, 1080));

    // Rectangle shapes
    RectangleShape windowy;
    RectangleShape check[4];
    RectangleShape border1(Vector2f(10, 1080));

    // Building
    construct(ring, 0, 11, 960, 50, 0, 890);
    construct(ring, 11, 22, 960, 50, 0, 828);
    construct(spike, 0, 3, 2000, 200, 0, 856);
    construct(platform, 0, 2, 2700, 400, 0, 650);
    construct(ring, 22, 24, 2767, 400, 0, 590);
    construct(spike, 3, 6, 3527, 200, 0, 856);
    construct(ring, 24, 35, 4474, 50, 0, 890);
    construct(ring, 35, 46, 4474, 50, 0, 828);
    construct(platform, 2, 5, 6500, 350, 150, 828);
    construct(spike, 6, 9, 6630, 350, 150, 735);
    construct(ring, 46, 49, 6550, 350, 150, 770);
    construct(ring, 49, 60, 7500, 150, 0, 890);
    construct(spike, 9, 15, 7570, 300, 0, 856);
    construct(tree, 0, 2, 100, 1500, 0, 238);
    construct(tree, 2, 4, 3300, 1700, 0, 238);
    construct(tree, 4, 7, 6000, 1613, 0, 238);
    construct(platform, 5, 10, 11700, 500, 0, 700);
    construct(ring, 60, 64, 11880, 50, 60, 470);
    construct(ring, 64, 67, 12080, 50, -60, 350);
    construct(ring, 67, 71, 12380, 50, 60, 470);
    construct(ring, 71, 74, 12580, 50, -60, 350);
    construct(ring, 74, 78, 12880, 50, 60, 470);
    construct(ring, 78, 81, 13080, 50, -60, 350);
    construct(ring, 81, 85, 13380, 50, 60, 470);
    construct(ring, 85, 88, 13580, 50, -60, 350);
    construct(tree, 7, 9, 11200, 3000, 0, 238);
    construct(spike, 15, 17, 14663, 64, 0, 856);
    construct(spike, 17, 19, 15463, 64, 0, 856);
    construct(spike, 19, 21, 16263, 64, 0, 856);
    construct(spike, 21, 23, 17063, 64, 0, 856);
    construct(spike, 23, 25, 17863, 64, 0, 856);
    construct(platform, 10, 14, 15030, 800, 0, 700);
    construct(ring, 88, 92, 15096, 800, 0, 640);
    construct(tree, 9, 11, 15600, 2000, 0, 238);
    construct(rock, 0, 1, -690, 0, 0, 543);
    construct(rock, 1, 2, -905, 0, 0, 747);
    construct(platform, 14, 16, 10049, 400, 0, 700);
    construct(ring, 92, 95, 10057, 60, 0, 640);
    construct(ring, 95, 98, 10456, 60, 0, 640);

    // Sonic Position
    sonic.player.setPosition(250, 720);
    sonic.PlayerColl.setPosition(250, 720);
    // crabs Position
    crab[0].enemie.setPosition(Vector2f(950, 800));
    crab[1].enemie.setPosition(Vector2f(4500, 800));
    crab[2].enemie.setPosition(Vector2f(9500, 800));
    crab[3].enemie.setPosition(Vector2f(10500, 800));
    crab[4].enemie.setPosition(Vector2f(14900, 800));
    crab[5].enemie.setPosition(Vector2f(15700, 800));
    crab[6].enemie.setPosition(Vector2f(16500, 800));
    crab[7].enemie.setPosition(Vector2f(17300, 800));
    // rest of crabs Position
    for (int i = 8; i < 100; i++)
        crab[i].enemie.setPosition(-1000, 0);
    // checkpoints Position and checks
    checkpoint[0].coll.setPosition(5000, 870);
    check[0].setPosition(5020, 870);
    checkpoint[1].coll.setPosition(11020, 870);
    check[1].setPosition(11000, 870);
    checkpoint[2].coll.setPosition(20520, 870);
    check[2].setPosition(20550, 870);
    // fish Position
    enemiepos(fish, 2600, 360, 0, 3);
    enemiepos(fish, 6380, 360, 3, 7);
    enemiepos(fish, 11550, 490, 7, 13);
    // rest of fishs Position
    for (int i = 13; i < 100; i++)
        fish[i].enemie.setPosition(-1000, 0);
    // window y Position
    windowy.setPosition(0, 1020);
    //background Position
    background.setPosition(Vector2f(-1000, 0));
    // rock2 Position
    rock1.setPosition(-690, 543);
    // rock3 Position
    rock3.setPosition(-905, 747);
    //lives Position
    for (int i = 0, j = 63; i < 5; i++)
    {
        lives[i].setPosition(-680 + (j * i), 1000);
    }

    // Scaling and sizing
    //sonic
    sonic.player.setScale(Vector2f(2.f, 2.f));
    // sonic rectangle
    sonic.PlayerColl.setSize(Vector2f(45.f, 90.f));
    //crab
    for (int i = 0; i < 35; i++)
        crab[i].enemie.setScale(2.f, 2.f);
    //crab rectangle
    for (int i = 0; i < 20; i++)
    {
        crab[i].coll.setSize(Vector2f(80.f, 60.f));
    }
    //fish
    for (int i = 0; i < 20; i++)
        fish[i].enemie.setScale(2.f, 2.f);
    // lives
    for (int i = 0; i < 5; i++)
        lives[i].setScale(0.1, 0.1);
    //window y
    windowy.setSize(Vector2f(40000, 20));
    //platform
    for (int i = 0; i < 20; i++)
        platform[i].coll.setScale(1.6, 1);
    //spikes
    for (int i = 0; i < 50; i++)
        spike[i].coll.setScale(0.5, 0.6);
    //tree
    for (int i = 0; i < 100; i++)
        tree[i].coll.setScale(2, 2);
    //rock2
    rock[1].coll.setScale(0.5, 0.5);
    for (int i = 0; i < 3; i++)
    {
        checkpoint[i].coll.setScale(1, 1);
    }
    // checksrectangles
    for (int i = 0; i < 3; i++)
        check[i].setSize(Vector2f(100, 80));
    //rock3
    rock3.setScale(0.5, 0.5);

    //set fill color
    border1.setFillColor(Color::Transparent);

    // Ground/Water Building

    setGround(ground, 0, 3600, 128, -1000, 952);
    setGround(ground, 1, 3000, 128, 3400, 952);
    setGround(water, 0, 800, 128, 2600, 980);
    setGround(ground, 2, 4000, 128, 7500, 952);
    setGround(water, 1, 1500, 128, 6100, 980);
    setGround(water, 2, 2600, 128, 11500, 980);
    setGround(ground, 3, 9000, 128, 14100, 952);

    //set repeated
    groundTexture.setRepeated(true);
    waterTexture.setRepeated(true);
    backgroundTexture.setRepeated(true);

    ///////////
    // Fonts //
    ///////////
    Font font;
    font.loadFromFile("font.ttf");
    Text collector;
    collector.setFont(font);
    collector.setString("Score: " + to_string(sonic.score));
    collector.setPosition(-680, 50);
    collector.setCharacterSize(72);
    collector.setFillColor(Color::White);

    ///////////
    // Sounds//
    ///////////

    // sounds buffer
    SoundBuffer coinsoundbuffer;
    SoundBuffer jumpsoundbuffer;
    SoundBuffer deathsoundbuffer;
    SoundBuffer enemiesoundbuffer;
    SoundBuffer checksoundbuffer;
    // sounds
    Sound coinsound;
    Sound jumpsound;
    Sound deathsound;
    Sound enemiedeath;
    Sound checks;
    //set buffer
    coinsound.setBuffer(coinsoundbuffer);
    jumpsound.setBuffer(jumpsoundbuffer);
    deathsound.setBuffer(deathsoundbuffer);
    enemiedeath.setBuffer(enemiesoundbuffer);
    checks.setBuffer(checksoundbuffer);
    // load sounds
    coinsoundbuffer.loadFromFile("coin.WAV");
    jumpsoundbuffer.loadFromFile("jump.WAV");
    deathsoundbuffer.loadFromFile("death2.WAV");
    enemiesoundbuffer.loadFromFile("enemiedeath.WAV");
    checksoundbuffer.loadFromFile("checks.WAV");

    ///////////
    // View  //
    ///////////
    View cam(Vector2f(0.f, 0.f), Vector2f(1920.f, 1080.f));

    while (window.isOpen())
    {
        ///////////////
       // Boss Level //
       ///////////////
        if (!sonic.check[2])
        {
            cam.setCenter(sonic.player.getPosition());
            cam.setCenter(Vector2f(sonic.player.getPosition().x, 600));
            window.setView(cam);
        }
        gameclock.restart();
        Event event;
        while (window.pollEvent(event))
        {
            //close sonic
            if (event.type == Event::Closed)
            {
                window.close();
            }
            //jumping
            if (event.type == Event::KeyPressed)
            {
                if ((event.key.code == Keyboard::Space) && sonic.isground && !sonic.isdead)
                {
                    sonic.Velocity.y = -30;
                    sonic.isground = false;
                    jumpsound.play();

                }

            }
            // When Released A and D
            if (event.type == Event::KeyReleased)
            {
                if ((event.key.code == Keyboard::A))
                {
                    sonic.animleft = 0;
                    Velocity2.x = 0;
                    sonic.Velocity.x = 0;
                    Velocity.x = 0;
                    sonic.acc = 0;
                    sonic.RunningSound = false;
                }
                if ((event.key.code == Keyboard::D))
                {
                    sonic.animright = 0;
                    sonic.Velocity.x = 0;
                    Velocity.x = 0;
                    Velocity2.x = 0;
                    sonic.acc = 0;
                    sonic.RunningSound = false;
                }

            }
        }
        ///////////////
       // Velocities //
       ///////////////

        //sonic
        sonic.player.move(sonic.Velocity.x, sonic.Velocity.y);
        //crab
        for (size_t i = 0; i < 35; i++)
        {
            crab[i].enemie.move(crab[i].Velocity.x, crab[i].Velocity.y);
        }
        //fish
        for (int i = 0; i < 20; i++)
            fish[i].enemie.move(fish[i].Velocity.x, fish[i].Velocity.y);
        //lives
        for (size_t i = 0; i < 5; i++)
        {
            lives[i].move(Velocity2.x, Velocity2.y);
        }
        // score and highscore
        collector.move(Velocity.x, Velocity.y);

        ///////////////      ///////////////
       //  Collision // And  //  Gravity  //
       ///////////////       ///////////////

       //sonic with ground
        if (!sonic.isdead)
        {
            for (int i = 0; i < 65; i++)
            {
                //if sonic on the ground:
                if (sonic.player.getGlobalBounds().intersects(ground[i].coll.getGlobalBounds()))
                {
                    sonic.player.setPosition(sonic.player.getPosition().x, ground[i].coll.getPosition().y - 75);
                    sonic.Velocity.y = -0.01;
                    sonic.isground = true;
                }
                //if sonic is above the ground:
                else
                {
                    //gravity:
                    sonic.Velocity.y += 0.02;
                }

            }
            //sonic intersection with platforms 
            for (int i = 0; i < 20; i++)
            {
                if (sonic.player.getGlobalBounds().intersects(platform[i].coll.getGlobalBounds()))
                {
                    //right side
                    if ((sonic.player.getPosition().x >= platform[i].coll.getPosition().x + 240))
                    {
                        sonic.player.setPosition(sonic.player.getPosition().x + 15, sonic.player.getPosition().y);
                        for (int i = 0; i < 5; i++)
                        {
                            lives[i].setPosition(lives[i].getPosition().x + 15, lives[i].getPosition().y);
                        }
                        collector.setPosition(collector.getPosition().x + 15, collector.getPosition().y);
                        sonic.isground = false;
                    }
                    //left side
                    else if ((sonic.PlayerColl.getPosition().x + 15 <= platform[i].coll.getPosition().x))
                    {
                        sonic.player.setPosition(sonic.player.getPosition().x - 15, sonic.player.getPosition().y);
                        for (int i = 0; i < 5; i++)
                        {
                            lives[i].setPosition(lives[i].getPosition().x - 15, lives[i].getPosition().y);
                        }
                        collector.setPosition(collector.getPosition().x - 15, collector.getPosition().y);
                        sonic.isground = false;
                    }
                    //lower side
                    else if ((sonic.PlayerColl.getPosition().y > platform[i].coll.getPosition().y + 28))
                    {

                        sonic.player.setPosition(sonic.player.getPosition().x, sonic.player.getPosition().y + (40));
                        sonic.isground = false;
                        sonic.Velocity.y += 5;
                    }
                    // upper side
                    else
                    {
                        sonic.player.setPosition(sonic.player.getPosition().x, platform[i].coll.getPosition().y - 75);
                        sonic.Velocity.y = -0.01;
                        sonic.isground = true;
                    }

                }
            }
            // sonic with window y collision
            if (sonic.PlayerColl.getGlobalBounds().intersects(windowy.getGlobalBounds()))
            {
                sonic.lives--;
                sonic.isdead = true;
                sonic.isground = false;
                sonic.Velocity.y = -15;
                deathsound.play();
            }
            //sonic intersection with checkpoints
            for (int i = 0; i < 3; i++)
                if (sonic.PlayerColl.getGlobalBounds().intersects(check[i].getGlobalBounds()))
                {
                    sonic.check[i] = true;
                    // end of level

                    ///////////////
                    // Boss Level //
                    ///////////////
                    if (i == 2)
                    {
                        transition(window);
                        transition_reverse(window);
                        sonic.isdead = false;
                        sonic.isground = true;
                        sonic.player.setPosition(250, 730);
                        collector.setPosition(-680, 50);
                        for (int i = 0, j = 63; i < 5; i++)
                        {
                            lives[i].setPosition(-680 + (j * i), 1000);
                        }
                        cam.setCenter(sonic.player.getPosition());
                        cam.setCenter(Vector2f(sonic.player.getPosition().x, 600));
                        window.setView(cam);
                        sonic.Velocity.x = 0;
                        sonic.acc = 0;
                        sonic.animleft = 0;
                        sonic.animright = 0;

                    }
                }
            //when not intersect
                else if (!(sonic.PlayerColl.getGlobalBounds().intersects(check[i].getGlobalBounds())))
                {
                    checkpoint[i].coll.setTextureRect(IntRect((1 * 0), 0, 120, 77));
                    if (i == 2)
                        checkpoint[i].coll.setTextureRect(IntRect((5.9 * 98.57), 0, 120, 77));
                }
            //sonic collision with fish
            for (int i = 0; i < 20; i++)
            {
                if (sonic.PlayerColl.getGlobalBounds().intersects(fish[i].enemie.getGlobalBounds()))
                {
                    sonic.lives--;
                    sonic.isdead = true;
                    sonic.isground = false;
                    sonic.Velocity.y = -15;
                    deathsound.play();
                }
            }
            //crab collision with sonic
            crabcoll(sonic.player, sonic.isdead, sonic.isground, sonic.PlayerColl, sonic.Velocity, sonic.score, sonic.lives, crab, ground, enemiedeath, deathsound, 0, 20, 0, 65);
            //////////////
            // Animation//
            //////////////


            ///////////
            // Jump //
            ///////// 
            if (!sonic.isground && !sonic.isdead)
            {
                animate_sprite(sonic.player, 16, 2 * 59.4, 48.87, 46, sonic.jumpanim, delay, deltatime, sonic.timer, 0);
            }
            ///////////////
            // Move Left //
            ////////////////
            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                move_left(sonic.player, sonic.Velocity, Velocity, Velocity2, sonic.acc, sonic.animleft, deltatime, sonic.timer, sonic.isground, sonic.isdead, sonic.check[2]);
            }
            ///////////////
            // Move Right//
            ////////////////
            else if (Keyboard::isKeyPressed(Keyboard::D))
            {
                move_right(sonic.player, sonic.Velocity, Velocity, Velocity2, sonic.acc, sonic.animright, deltatime, sonic.timer, sonic.isground, sonic.isdead, sonic.check[2]);
            }

            /////////
            // IDLE //
            //////////
            if (sonic.isground && !(Keyboard::isKeyPressed(Keyboard::D)) && !(Keyboard::isKeyPressed(Keyboard::A)) && !(Keyboard::isKeyPressed(Keyboard::Space)))
            {
                animate_sprite(sonic.player, 8, 0, 48.87, 41, sonic.idleanim, delay, deltatime, sonic.timer, 0);
            }

            //water collision with sonic
            for (int i = 0; i < 2; i++)
            {
                if (sonic.PlayerColl.getGlobalBounds().intersects(water[i].coll.getGlobalBounds()))
                {
                    sonic.lives--;
                    sonic.isdead = true;
                    sonic.isground = false;
                    sonic.Velocity.y = -15;
                    deathsound.play();
                }
            }
            // spike collision
            for (int i = 0; i < 40; i++)
            {
                if (sonic.PlayerColl.getGlobalBounds().intersects(spike[i].coll.getGlobalBounds()) && sonic.isground)
                {
                    //right side
                    if ((sonic.PlayerColl.getPosition().x >= spike[i].coll.getPosition().x + 65))
                    {
                        sonic.lives--;
                        sonic.isdead = true;
                        sonic.isground = false;
                        sonic.Velocity.y = -15;
                        deathsound.play();

                    }
                    //left side
                    else if ((sonic.PlayerColl.getPosition().x + 38 <= spike[i].coll.getPosition().x))
                    {
                        sonic.lives--;
                        sonic.isdead = true;
                        sonic.isground = false;
                        sonic.Velocity.y = -15;
                        deathsound.play();
                    }
                    //upper side
                    else
                    {
                        sonic.lives--;
                        sonic.isdead = true;
                        sonic.isground = false;
                        sonic.Velocity.y = -15;
                        deathsound.play();
                    }
                }
            }

        }
        // crab animation
        crabanim(crab, deltatime);
        ///////////////
       // Sonic death//
       ///////////////
        if (sonic.isdead && !sonic.isground)
        {
            animate_sprite(sonic.player, 2, 4 * 55, 45.5, 50, sonic.deadanim, sonic.deathdelay, deltatime, sonic.timer, 6);
            sonic.Velocity.y += 0.5;
            sonic.Velocity.x = 0;
            Velocity.x = 0;
            Velocity2.x = 0;

        }
        // crab death
        for (int i = 0; i < 20; i++)
        {
            if (crab[i].isdead)
            {
                crab[i].coll.setScale(0, 0);
                crab[i].enemie.setScale(0, 0);
            }
        }
        // can't go left in start of the level
        if (sonic.player.getGlobalBounds().intersects(border1.getGlobalBounds()))
        {
            sonic.player.setPosition(sonic.player.getPosition().x - sonic.Velocity.x, sonic.player.getPosition().y);
            collector.setPosition(collector.getPosition().x - Velocity.x, collector.getPosition().y);
            for (int i = 0; i < 5; i++)
            {
                lives[i].setPosition(lives[i].getPosition().x - Velocity2.x, lives[i].getPosition().y);
            }
        }
        //Velocity fish
        for (int i = 0; i < 20; i++)
            fish[i].Velocity.y = -10;

        // animation fish
        structanim(fish, delay, deltatime, 0, 20, 2, 41.5, 0, 40, 36);

        // fish repawn
        enemierespawn(fish, window, 2600, 360, 0, 3);
        enemierespawn(fish, window, 6380, 360, 3, 7);
        enemierespawn(fish, window, 11550, 490, 7, 13);

        //Rings Disappearing When Collision
        for (int i = 0; i < 100; i++)
        {
            if (sonic.PlayerColl.getGlobalBounds().intersects(ring[i].coll.getGlobalBounds()) && !sonic.isdead)
            {
                ring[i].coll.setScale(Vector2f(0, 0));
                sonic.score += 100;
                coinsound.play();
            }
        }

        //Rings Animation
        animate_stack(ring, 100, 0, 10, 64, 62, delay, deltatime);

        //////////////////
       // Sonic Respawn //
       ///////////////////

        //when not intersect any of checkpoints
        for (int i = 0; i < 3; i++)
        {
            if (sonic.PlayerColl.getPosition().y > window.getSize().y && !sonic.check[i])
            {
                sonic.isdead = false;
                sonic.isground = true;
                sonic.player.setPosition(250, 730);
                collector.setPosition(-680, 50);
                for (int i = 0, j = 63; i < 5; i++)
                {
                    lives[i].setPosition(-680 + (j * i), 1000);
                }
                sonic.Velocity.x = 0;
                sonic.acc = 0;
                sonic.animleft = 0;
                sonic.animright = 0;
            }
        }
        //when intersect with checkpoints
        for (int z = 0; z < 3; z++)
        {
            if (sonic.PlayerColl.getPosition().y > window.getSize().y && sonic.check[z])
            {
                checks.play();
                sonic.isdead = false;
                sonic.isground = true;
                if (z <= 1)
                {
                    sonic.player.setPosition(check[z].getPosition().x, 730);
                    collector.setPosition(check[z].getPosition().x - 930, 50);
                    for (int i = 0, j = 63; i < 5; i++)
                    {
                        lives[i].setPosition(check[z].getPosition().x - 930 + (j * i), 1000);
                    }
                }
                else
                {
                    sonic.player.setPosition(check[z].getPosition().x, 730);
                    collector.setPosition(check[z].getPosition().x - 910, 50);
                    for (int i = 0, j = 63; i < 5; i++)
                    {
                        lives[i].setPosition(check[z].getPosition().x - 910 + (j * i), 1000);
                    }
                }
                sonic.Velocity.x = 0;
                sonic.acc = 0;
                sonic.animleft = 0;
                sonic.animright = 0;
            }
        }
        //////////      ///////////
       // Score // And //HighScore//
       //////////      /////////////
        sonic.highscore = loadhighscore();
        if (sonic.score > sonic.highscore)
        {
            sonic.highscore = sonic.score;
            savehighscore(sonic.highscore);
        }
        collector.setString("Score: " + to_string(sonic.score) + "\nHigh Score: " + to_string(sonic.highscore));

        //position of sonic rectangle
        sonic.PlayerColl.setPosition(sonic.player.getPosition().x - 70, sonic.player.getPosition().y + 10);
        // position of crab rectangle
        for (int i = 0; i < 20; i++)
            crab[i].coll.setPosition(crab[i].enemie.getPosition().x + 8, crab[i].enemie.getPosition().y + 10);

        //Update
        window.clear();

        ////////
       //DRAW//
       /////////

        //BACKGROUND
        window.draw(background);
        //TREE
        for (int i = 0; i < 11; i++)
            window.draw(tree[i].coll);
        //WATER
        for (int i = 0; i < 3; i++)
            window.draw(water[i].coll);
        //CHECKPOINTS
        for (int i = 0; i < 3; i++)
            window.draw(checkpoint[i].coll);
        //RINGS
        for (int i = 0; i < 98; i++)
            window.draw(ring[i].coll);
        //CRABS
        for (int i = 0; i < 8; i++)
            window.draw(crab[i].enemie);
        //GROUND
        for (int i = 0; i < 4; i++)
            window.draw(ground[i].coll);
        //BORDER
        window.draw(border1);
        for (int i = 0; i < 2; i++)
            window.draw(rock[i].coll);
        //SCORE AND HIGHSCORE
        window.draw(collector);
        //PLATFORMS
        for (int i = 0; i < 16; i++)
            window.draw(platform[i].coll);
        //SPIKES
        for (int i = 0; i < 25; i++)
            window.draw(spike[i].coll);
        //LIVES
        for (int i = 0; i < sonic.lives; i++)
            window.draw(lives[i]);
        //FISH
        for (int i = 0; i < 13; i++)
            window.draw(fish[i].enemie);
        //SONIC
        window.draw(sonic.player);

       ///////////
       //DISPLAY//
       ///////////
        window.display();
        //DELTATIME
        deltatime = gameclock.getElapsedTime().asSeconds();
    }
}

