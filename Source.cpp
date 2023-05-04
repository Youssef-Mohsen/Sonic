#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
using namespace std;
using namespace sf;



//structs

struct player
{
    Texture sonicTex;
    Sprite player;
    RectangleShape PlayerColl, LeftColl, RightColl;
    Vector2f Velocity;
    int animright = 0, idleanim = 0, animleft = 0, jumpanim = 0, deadanim = 0;
    float deathdelay = 0.3;
    int score = 0, lives = 5, FinalScore = 0;
    bool start = false, Running = false;
    bool isground = false, isdead = false, hitRight = false, hitLeft = false, RunningSound = false, HitAbove = false;
};

struct enemy
{
    Sprite sprite;
    Vector2f Velocity;
    RectangleShape coll;
    int animright = 0, animleft = 0;
    bool isdead = false, isground = false;
};



//functions

void move_right(Sprite& sonic, Vector2f& sonicVelocity, Vector2f& Velocity, Vector2f& Velocity2, float& acc, int& animright, float& deltatime, float& timer, bool isground, bool isdead);
void move_left(Sprite& sonic, Vector2f& sonicVelocity, Vector2f& Velocity, Vector2f& Velocity2, float& acc, int& animleft, float& deltatime, float& timer, bool isground, bool isdead);
void animate_sprite(Sprite& sprite, int num, float y, float width, float height, int& animationInd, float& delay, float& deltatime, float& timer, int shift);
void animate_struct_stack(struct enemy sprites[], int size, int num, int y, int width, int height, int& animationInd, int& delay);
void animate_stack(Sprite sprites[], int size, int num, int y, int width, int height, int& animationInd, int& delay);
void construct(Sprite item[], int start, int amount, float x, int differenceX, int differenceY, float y);
void enemierespawn(struct enemy sprite[], RenderWindow& window, int x, int z, int start, int amount);
void setGround(Sprite item[], int num, int width, int height, int x, int y);
void enemiepos(struct enemy sprite[], int x, int z, int start, int amount);
void move_crab(Sprite crab, int num, bool isdead, Vector2f& velocity);


int main()
{
    // Variables
    Clock gameclock;
    float delay = 0.1f;
    float deltatime = 0;
    float timer = 0, timer2 = 0, timer3 = 0;
    float acc = 0;
    int ringanimator = 0;
    int ringdelay = 4;
    int fishanimInd = 0;
    int fishdelay = 6;
    int crabanimInd = 0;
    int crabdelay = 6;
    RenderWindow window(VideoMode(1940, 1080), "Sonic.exe", Style::Default);
    window.setFramerateLimit(60);

    // Structs
    // Sonic
    player sonic;
    sonic.player.setTextureRect(IntRect(48.87, 0, 48.87, 43));
    //enemies
    enemy crab[100];
    enemy fish[100];

    for (int i = 0; i < 20; i++)
        crab[i].sprite.setTextureRect(IntRect(48.5, 0 * 36, 46.5, 36));
    for (int i = 0; i < 20; i++)
        fish[i].sprite.setTextureRect(IntRect(48.5, 0 * 36, 46.5, 36));

    // Velocities
    Vector2f Velocity;
    Vector2f Velocity2;

    //Textures
    Texture spiketex;
    Texture platformtex;
    Texture Groundtex;
    Texture backgroundTexture;
    Texture groundTexture;
    Texture ringTexture;
    Texture rockTexture;
    Texture waterTexture;
    Texture liveTex;
    Texture treetex;
    Texture crabtex;
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
    // Sprites

    Sprite lives[5];
    Sprite Ground;
    Sprite platform[20];
    Sprite spike[50];
    Sprite ring[100];
    Sprite tree[100];
    Sprite rock[2];

    // Setting Textures
    sonic.player.setTexture(sonic.sonicTex);
    for (int i = 0; i < 35; i++)
        crab[i].sprite.setTexture(crabtex);
    for (int i = 0; i < 20; i++)
        fish[i].sprite.setTexture(fishtex);
    for (int i = 0; i < 5; i++)
    {
        lives[i].setTexture(liveTex);
    }
    for (int i = 0; i < 20; i++)
        platform[i].setTexture(platformtex);
    for (int i = 0; i < 50; i++)
        spike[i].setTexture(spiketex);
    for (int i = 0; i < 100; i++)
        ring[i].setTexture(ringTexture);
    for (int i = 0; i < 100; i++)
        tree[i].setTexture(treetex);
    for (int i = 0; i < 2; i++)
        rock[i].setTexture(rockTexture);

    // window y
    RectangleShape windowy;


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
    construct(spike, 15, 19, 14600, 64, 0, 856);
    construct(spike, 19, 23, 15400, 64, 0, 856);
    construct(spike, 23, 27, 16200, 64, 0, 856);
    construct(spike, 27, 31, 17000, 64, 0, 856);
    construct(spike, 31, 35, 17800, 64, 0, 856);
    construct(platform, 10, 14, 15030, 800, 0, 700);
    construct(ring, 88, 92, 15096, 800, 0, 640);
    construct(tree, 9, 11, 15600, 2000, 0, 238);
    construct(rock, 0, 1, -690, 0, 0, 543);
    construct(rock, 1, 2, -905, 0, 0, 747);

    // Sonic Position
    sonic.player.setPosition(250, 720);
    sonic.PlayerColl.setPosition(250, 720);
    // crabs pos
    crab[0].sprite.setPosition(Vector2f(1000, 800));
    crab[1].sprite.setPosition(Vector2f(4500, 800));
    crab[2].sprite.setPosition(Vector2f(10500, 800));
    // rest of crabs pos
    for (int i = 3; i < 100; i++)
        crab[i].sprite.setPosition(20000, 940);

    // fish pos
    enemiepos(fish, 2600, 360, 0, 3);
    enemiepos(fish, 6380, 360, 3, 7);
    enemiepos(fish, 11550, 490, 7, 13);
    // rest of fishs pos
    for (int i = 13; i < 100; i++)
        fish[i].sprite.setPosition(-1000, 0);
    // window y pos
    windowy.setPosition(0, 1000);
    // Ground Position
    Ground.setPosition(200, 800);

    for (int i = 0, j = 63; i < 5; i++)
    {
        lives[i].setPosition(-680 + (j * i), 1000);
    }

    // Scaling and sizing
    sonic.player.setScale(Vector2f(2.f, 2.f));
    for (int i = 0; i < 35; i++)
        crab[i].sprite.setScale(2.f, 2.f);
    for (int i = 0; i < 20; i++)
        fish[i].sprite.setScale(2.f, 2.f);
    for (int i = 0; i < 5; i++)
        lives[i].setScale(0.1, 0.1);
    windowy.setSize(Vector2f(20000, 20));
    sonic.PlayerColl.setSize(Vector2f(45.f, 70.f));
    sonic.LeftColl.setSize(Vector2f(15.f, 70.f));
    sonic.RightColl.setSize(Vector2f(15.f, 70.f));
    for (int i = 0; i < 20; i++)
    {
        crab[i].coll.setSize(Vector2f(80.f, 60.f));
    }
    for (int i = 0; i < 20; i++)
        platform[i].setScale(1.6, 1);
    for (int i = 0; i < 50; i++)
        spike[i].setScale(0.5, 0.6);
    for (int i = 0; i < 100; i++)
        tree[i].setScale(2, 2);
    rock[1].setScale(0.5, 0.5);

    // Background
    Sprite background;
    background.setTexture(backgroundTexture);
    background.setTextureRect(IntRect(0, 0, 1920 * 33.33333, 1080));
    background.setPosition(Vector2f(-1000, 0));
    backgroundTexture.setRepeated(true);

    // Rocks
    Sprite rock1;
    Sprite rock3;
    rock1.setTexture(rockTexture);
    rock3.setTexture(rockTexture);
    rock1.setPosition(-690, 543);
    rock3.setPosition(-905, 747);
    rock3.setScale(0.5, 0.5);

    // Border
    RectangleShape border1(Vector2f(10, 1080));
    border1.setFillColor(Color::Transparent);

    // Ground/Water
    Sprite ground[70];
    Sprite water[70];
    for (int i = 0; i < 70; i++)
    {
        ground[i].setTexture(groundTexture);
        water[i].setTexture(waterTexture);
    }
    setGround(ground, 0, 3600, 128, -1000, 952);
    setGround(ground, 1, 3000, 128, 3400, 952);
    setGround(water, 0, 800, 128, 2600, 980);
    setGround(ground, 2, 4000, 128, 7500, 952);
    setGround(water, 1, 1500, 128, 6100, 980);
    setGround(water, 2, 2600, 128, 11500, 980);
    setGround(ground, 3, 9000, 128, 14100, 952);
    groundTexture.setRepeated(true);
    waterTexture.setRepeated(true);


    //=======//
    // Fonts //
    //=======//

    int score = 0;
    Font font;
    font.loadFromFile("font.ttf");
    Text collector;
    collector.setFont(font);
    collector.setString("Score: " + to_string(score));
    collector.setPosition(-680, 100);
    collector.setCharacterSize(72);
    collector.setFillColor(Color::White);


    //========//
    // Sounds //
    //========//
    
    // sounds buffer
    SoundBuffer coinsoundbuffer;
    SoundBuffer jumpsoundbuffer;
    SoundBuffer deathsoundbuffer;
    SoundBuffer enemiesoundbuffer;
    // sounds
    Sound coinsound;
    Sound jumpsound;
    Sound deathsound;
    Sound enemiedeath;
    //set buffer
    coinsound.setBuffer(coinsoundbuffer);
    jumpsound.setBuffer(jumpsoundbuffer);
    deathsound.setBuffer(deathsoundbuffer);
    enemiedeath.setBuffer(enemiesoundbuffer);
    // load sounds
    coinsoundbuffer.loadFromFile("coin.WAV");
    jumpsoundbuffer.loadFromFile("jump.WAV");
    deathsoundbuffer.loadFromFile("death2.WAV");
    enemiesoundbuffer.loadFromFile("enemiedeath.WAV");

    // view
    View cam(Vector2f(0.f, 0.f), Vector2f(1920.f, 1080.f));

    //game loop
    while (window.isOpen())
    {
        //veiw
        cam.setCenter(sonic.player.getPosition());
        cam.setCenter(Vector2f(sonic.player.getPosition().x, 600));
        window.setView(cam);


        gameclock.restart();
        Event event;
        //events loop
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            if (event.type == Event::KeyPressed)
            {
                if ((event.key.code == Keyboard::Space) && sonic.isground && !sonic.isdead)
                {
                    sonic.Velocity.y = -30;
                    sonic.isground = false;
                    jumpsound.play();

                }

            }
            if (event.type == Event::KeyReleased)
            {
                if ((event.key.code == Keyboard::A))
                {
                    sonic.animleft = 0;
                    Velocity2.x = 0;
                    sonic.Velocity.x = 0;
                    Velocity.x = 0;
                    acc = 0;
                    sonic.RunningSound = false;
                }
                if ((event.key.code == Keyboard::D))
                {
                    sonic.animright = 0;
                    sonic.Velocity.x = 0;
                    Velocity.x = 0;
                    Velocity2.x = 0;
                    acc = 0;
                    sonic.RunningSound = false;
                }

            }
        }




        //============//
        // velocities //
        //============//


        sonic.player.move(sonic.Velocity.x, sonic.Velocity.y);
        collector.move(Velocity.x, Velocity.y);
        for (size_t i = 0; i < 35; i++)
        {
            crab[i].sprite.move(crab[i].Velocity.x, crab[i].Velocity.y);
        }
        for (int i = 0; i < 20; i++)
        {
            fish[i].sprite.move(fish[i].Velocity.x, fish[i].Velocity.y);
        }
        for (size_t i = 0; i < 5; i++)
        {
            lives[i].move(Velocity2.x, Velocity2.y);
        }
        



        //=======//
        // sonic //
        //=======//
        

        //sonic physics& animation
        //===============================//

        if (!sonic.isdead)
        {
            for (int i = 0; i < 65; i++)
            {
                //if sonic on the ground:
                if (sonic.player.getGlobalBounds().intersects(ground[i].getGlobalBounds()))
                {
                    sonic.player.setPosition(sonic.player.getPosition().x, ground[i].getPosition().y - 75);
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

            //sonic colision with platforms 
            for (int i = 0; i < 20; i++)
            {
                if (sonic.player.getGlobalBounds().intersects(platform[i].getGlobalBounds()))
                {

                    if ((sonic.player.getPosition().x >= platform[i].getPosition().x + 240))
                    {
                        sonic.player.setPosition(sonic.player.getPosition().x + 15, sonic.player.getPosition().y);
                        for (int i = 0; i < 5; i++)
                        {
                            lives[i].setPosition(lives[i].getPosition().x + 15, lives[i].getPosition().y);
                        }
                        collector.setPosition(collector.getPosition().x + 15, collector.getPosition().y);
                        sonic.isground = false;
                    }
                    else if ((sonic.PlayerColl.getPosition().x + 15 <= platform[i].getPosition().x))
                    {
                        sonic.player.setPosition(sonic.player.getPosition().x - 15, sonic.player.getPosition().y);
                        for (int i = 0; i < 5; i++)
                        {
                            lives[i].setPosition(lives[i].getPosition().x - 15, lives[i].getPosition().y);
                        }
                        collector.setPosition(collector.getPosition().x - 15, collector.getPosition().y);
                        sonic.isground = false;
                    }
                    else if ((sonic.PlayerColl.getPosition().y > platform[i].getPosition().y + 28))
                    {

                        sonic.player.setPosition(sonic.player.getPosition().x, sonic.player.getPosition().y + (40));
                        sonic.isground = false;
                        sonic.Velocity.y += 5;
                    }
                    else
                    {
                        sonic.player.setPosition(sonic.player.getPosition().x, platform[i].getPosition().y - 75);
                        sonic.Velocity.y = -0.01;
                        sonic.isground = true;
                    }

                }
            }

            //jump animation 
            if (!sonic.isground && !sonic.isdead)
            {
                animate_sprite(sonic.player, 16, 2 * 59.4, 48.87, 46, sonic.jumpanim, delay, deltatime, timer, 0);
            }

            //move left & right
            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                move_left(sonic.player, sonic.Velocity, Velocity, Velocity2, acc, sonic.animleft, deltatime, timer, sonic.isground, sonic.isdead);
            }
            else if (Keyboard::isKeyPressed(Keyboard::D))
            {
                move_right(sonic.player, sonic.Velocity, Velocity, Velocity2, acc, sonic.animright, deltatime, timer, sonic.isground, sonic.isdead);
            }

            // idle animation
            if (sonic.isground && !(Keyboard::isKeyPressed(Keyboard::D)) && !(Keyboard::isKeyPressed(Keyboard::A)) && !(Keyboard::isKeyPressed(Keyboard::Space)))
            {
                animate_sprite(sonic.player, 8, 0, 48.87, 41, sonic.idleanim, delay, deltatime, timer, 0);
            }

            // window y collision
            if (sonic.PlayerColl.getGlobalBounds().intersects(windowy.getGlobalBounds()))
            {
                sonic.lives--;
                sonic.isdead = true;
                sonic.isground = false;
                sonic.Velocity.y = -15;
                deathsound.play();
            }




            //=========//
            // enemies //
            //=========//
            

            //crab
            //==============//

            //crab movement
            for (int i = 0; i < 20; i++)
            {
                if (!crab[i].isdead && !sonic.isdead)
                {
                    //crab intersection with ground
                    for (int j = 0; j < 65; j++)
                    {
                        if (crab[i].sprite.getGlobalBounds().intersects(ground[j].getGlobalBounds()))
                        {
                            crab[i].sprite.setPosition(crab[i].sprite.getPosition().x, ground[j].getPosition().y - 65);
                            crab[i].Velocity.y = -0.01;
                            crab[i].isground = true;
                        }
                        else if (!crab[i].isground)
                            crab[i].Velocity.y = 5;
                    }


                    //crab collision with sonic
                    if (sonic.PlayerColl.getGlobalBounds().intersects(crab[i].sprite.getGlobalBounds()))
                    {
                        //when sonic hit the crab
                        if (sonic.PlayerColl.getPosition().y + 70 <= crab[i].coll.getPosition().y)
                        {
                            enemiedeath.play();
                            crab[i].isdead = true;
                            crab[i].sprite.setScale(0, 0);
                            crab[i].coll.setScale(0, 0);
                            sonic.Velocity.y = -12;
                            sonic.Velocity.y += 3;
                            score += 50;
                        }


                        //when crab hit sonic
                        else if (sonic.PlayerColl.getPosition().x + 45 >= crab[i].coll.getPosition().x)
                        {
                            sonic.lives--;
                            sonic.isdead = true;
                            sonic.isground = false;
                            sonic.Velocity.y = -15;
                            deathsound.play();
                        }
                        else if (sonic.PlayerColl.getPosition().x <= crab[i].coll.getPosition().x + 38)
                        {
                            sonic.lives--;
                            sonic.isdead = true;
                            sonic.isground = false;
                            sonic.Velocity.y = -15;
                            deathsound.play();
                        }
                    }

                    // crab movement
                    move_crab(crab[0].sprite, 1000, crab[0].isdead, crab[0].Velocity);
                    move_crab(crab[1].sprite, 4500, crab[1].isdead, crab[1].Velocity);
                    move_crab(crab[2].sprite, 10500, crab[2].isdead, crab[2].Velocity);

                }
            }


            //fish
            //===========================//

            for (int i = 0; i < 20; i++)
            {
                fish[i].Velocity.y = -10;
            }
            for (int i = 0; i < 20; i++)
            {
                if (sonic.PlayerColl.getGlobalBounds().intersects(fish[i].sprite.getGlobalBounds()))
                {
                    sonic.lives--;
                    sonic.isdead = true;
                    sonic.isground = false;
                    sonic.Velocity.y = -15;
                    deathsound.play();
                }
            }
            enemierespawn(fish, window, 2600, 360, 0, 3);
            enemierespawn(fish, window, 6380, 360, 3, 7);
            enemierespawn(fish, window, 11550, 490, 7, 13);




            //===========//
            // obstacles //
            //===========//


            //water
            //===============//

            for (int i = 0; i < 2; i++)
            {
                if (sonic.PlayerColl.getGlobalBounds().intersects(water[i].getGlobalBounds()))
                {
                    sonic.lives--;
                    sonic.isdead = true;
                    sonic.isground = false;
                    sonic.Velocity.y = -15;
                    deathsound.play();
                }
            }

            //spike
            //====================//

            for (int i = 0; i < 40; i++)
            {
                if (sonic.PlayerColl.getGlobalBounds().intersects(spike[i].getGlobalBounds()) && sonic.isground)
                {

                    if ((sonic.PlayerColl.getPosition().x >= spike[i].getPosition().x + 65))
                    {
                        sonic.lives--;
                        sonic.isdead = true;
                        sonic.isground = false;
                        sonic.Velocity.y = -15;
                        deathsound.play();

                    }
                    else if ((sonic.PlayerColl.getPosition().x + 38 <= spike[i].getPosition().x))
                    {
                        sonic.lives--;
                        sonic.isdead = true;
                        sonic.isground = false;
                        sonic.Velocity.y = -15;
                        deathsound.play();
                    }
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




        //===================//
        //     animation     //
        //===================//
        

        //crab animation
        animate_struct_stack(crab, 100, 4, 0, 47.5, 36, crabanimInd, crabdelay);

        //fish animation & movement
        animate_struct_stack(fish, 20, 2, 0, 41.5, 36, fishanimInd, fishdelay);


        // sonic death
        if (sonic.isdead && !sonic.isground)
        {
            animate_sprite(sonic.player, 2, 4 * 55, 45.5, 50, sonic.deadanim, sonic.deathdelay, deltatime, timer, 6);
            sonic.Velocity.y += 0.5;
            sonic.Velocity.x = 0;
            Velocity.x = 0;
            Velocity2.x = 0;
        }


        //reward system
        //=====================//
        
        //Rings Collision & Animation
        for (int i = 0; i < 100; i++)
        {
            if (sonic.PlayerColl.getGlobalBounds().intersects(ring[i].getGlobalBounds()) && !sonic.isdead)
            {
                ring[i].setScale(Vector2f(0, 0));
                score += 100;
                coinsound.play();
            }
        }
        animate_stack(ring, 100, 10, 0, 64, 62, ringanimator, ringdelay);




        //===================//
        //       map         //
        //===================//
        

        //display movement
        //==============================//
        
        //respawn sonic & collector
        if (sonic.PlayerColl.getPosition().y > window.getSize().y)
        {
            sonic.isdead = false;
            sonic.isground = true;
            sonic.player.setPosition(250, 730);
            collector.setPosition(-680, 100);
            for (int i = 0, j = 63; i < 5; i++)
            {
                lives[i].setPosition(-680 + (j * i), 1000);
            }
            sonic.Velocity.x = 0;
            acc = 0;
            sonic.animleft = 0;
            sonic.animright = 0;
        }
        collector.setString("Score: " + to_string(score));

        //border of the level
        //==================================//
        if (sonic.player.getGlobalBounds().intersects(border1.getGlobalBounds()))
        {
            sonic.player.setPosition(sonic.player.getPosition().x - sonic.Velocity.x, sonic.player.getPosition().y);
            collector.setPosition(collector.getPosition().x - Velocity.x, collector.getPosition().y);
            for (int i = 0; i < 5; i++)
            {
                lives[i].setPosition(lives[i].getPosition().x - Velocity2.x, lives[i].getPosition().y);
            }
        }


        //position of collision rectangles
        //====================================//
        sonic.PlayerColl.setPosition(sonic.player.getPosition().x - 70, sonic.player.getPosition().y + 10);
        sonic.LeftColl.setPosition(sonic.player.getPosition().x - 70, sonic.player.getPosition().y + 10);
        sonic.RightColl.setPosition(sonic.player.getPosition().x - 40, sonic.player.getPosition().y + 10);
        for (int i = 0; i < 20; i++)
            crab[i].coll.setPosition(crab[i].sprite.getPosition().x + 8, crab[i].sprite.getPosition().y + 10);




        //=======================================================================================================================================================//


        //Update
        window.clear();


        //Draw
        window.draw(collector);
        window.draw(background);
        for (int i = 0; i < 11; i++)
            window.draw(tree[i]);
        for (int i = 0; i < 3; i++)
            window.draw(water[i]);
        for (int i = 0; i < 92; i++)
            window.draw(ring[i]);
        for (int i = 0; i < 3; i++)
            window.draw(crab[i].sprite);
        for (int i = 0; i < 4; i++)
            window.draw(ground[i]);
        window.draw(border1);
        for (int i = 0; i < 2; i++)
            window.draw(rock[i]);
        window.draw(collector);
        for (int i = 0; i < 14; i++)
            window.draw(platform[i]);
        for (int i = 0; i < 35; i++)
            window.draw(spike[i]);
        window.draw(collector);
        for (int i = 0; i < sonic.lives; i++)
            window.draw(lives[i]);
        for (int i = 0; i < 13; i++)
            window.draw(fish[i].sprite);
        window.draw(sonic.player);
        

        //Display
        window.display();
        deltatime = gameclock.getElapsedTime().asSeconds();


        //=======================================================================================================================================================//
    }
}


//functions
void move_right(Sprite& sonic, Vector2f& sonicVelocity, Vector2f& Velocity, Vector2f& Velocity2, float& acc, int& animright, float& deltatime, float& timer, bool isground, bool isdead)
{
    sonicVelocity.x = 8 + acc;
    Velocity.x = 8 + acc;
    Velocity2.x = 8 + acc;
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
void move_left(Sprite& sonic, Vector2f& sonicVelocity, Vector2f& Velocity, Vector2f& Velocity2, float& acc, int& animleft, float& deltatime, float& timer, bool isground, bool isdead)
{
    sonicVelocity.x = -8 + acc;
    Velocity.x = -8 + acc;
    Velocity2.x = -8 + acc;
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
void animate_struct_stack(struct enemy sprites[], int size, int num, int y, int width, int height, int& animationInd, int& delay)
{
    animationInd++;
    animationInd %= num * delay;
    for (int i = 0; i < size; i++)
    {
        sprites[i].sprite.setTextureRect(IntRect((animationInd / delay * width), y, width, height));
    }
}
void animate_stack(Sprite sprites[], int size, int num, int y, int width, int height, int& animationInd, int& delay)
{
    animationInd++;
    animationInd %= num * delay;
    for (int i = 0; i < size; i++)
    {
        sprites[i].setTextureRect(IntRect((animationInd / delay * width), y, width, height));
    }
}
void construct(Sprite item[], int start, int amount, float x, int differenceX, int differenceY, float y)
{
    for (int i = start, j = 0; i < amount; i++, j++)
        item[i].setPosition(Vector2f(x + (j * differenceX), y - (j * differenceY)));
}
void enemierespawn(struct enemy sprite[], RenderWindow& window, int x, int z, int start, int amount)
{
    for (int i = start, j = 0; i < amount; i++, j++)
    {
        if (sprite[i].sprite.getPosition().y < window.getSize().y - 1024)
        {
            sprite[i].sprite.setPosition(x + (z * j), 940);
        }
    }
}
void setGround(Sprite item[], int num, int width, int height, int x, int y)
{
    item[num].setTextureRect(IntRect(0, 0, width, height));
    item[num].setPosition(x, y);
}
void enemiepos(struct enemy fish[], int x, int z, int start, int amount)
{
    for (int i = start, j = 0; i < amount; i++, j++)
    {
        fish[i].sprite.setPosition(x + (z * j), 940);
    }
}
void move_crab(Sprite crab, int num, bool isdead, Vector2f& velocity)
{
    if (crab.getPosition().x <= num && !isdead)
    {
        velocity.x = 3;

    }
    else if (crab.getPosition().x >= (num + 250) && !isdead)
    {
        velocity.x = -3;

    }
}
