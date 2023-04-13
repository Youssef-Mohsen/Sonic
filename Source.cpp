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
using namespace sf;
using namespace std;


struct player
{
    Texture sonicTex;
    Sprite player;
    RectangleShape PlayerColl, LeftColl, RightColl;
    Vector2f Velocity;
    int runrightanimation = 0, Idleanimation = 0, runleftanimation = 0, takingdamageanimation = 0, jumpinganimation = 0;
    int scoreValue = 0, lives = 5, hitCounter = -1, deathDealy = 0, FinalScore = 0;
    bool start = false, Running = false;
    bool on_ground = true, hitRight = false, hitLeft = false, RunningSound = false, HitAbove = false;
};


int main()
{
    // variables
    bool isdead = false;
    
    bool isground = false;
    Clock gameclock;
    
   
    
    float delay = 0.1f;
    float deltatime = 0;
    float timer = 0;
   
    int animleft = 0;
    int animright = 0;
    int jumpanim = 0;
    int idleanim = 0;
    int deadanim = 0;

    RenderWindow window(VideoMode(1940, 1080), "Sonic.exe", Style::Default);
    window.setFramerateLimit(60);
//structs
    //sonic
    player sonic;
    sonic.player.setTextureRect(IntRect(48.87, 0, 48.87, 43));

//out of structs
   
    
    //Textures
    
    Texture spiketex;
    Texture walltex;
    Texture Groundtex;
    Texture backgroundTexture;
    Texture groundTexture;
    Texture ringTexture;
    Texture rockTexture;

    
    
    //Loading from files
    sonic.sonicTex.loadFromFile("sonic22.png");
    walltex.loadFromFile("Wall.png");
    Groundtex.loadFromFile("Wall.png");
    spiketex.loadFromFile("Spike.png");
    backgroundTexture.loadFromFile("background.png");
    groundTexture.loadFromFile("ground2.png");
    ringTexture.loadFromFile("ring.png");
    rockTexture.loadFromFile("rock.png");
    
    //sprites
    
    Sprite wall;
    Sprite Ground;
    Sprite spike;
    // set texture
    sonic.player.setTexture(sonic.sonicTex);
    
    
    wall.setTexture(walltex);
    Ground.setTexture(Groundtex);
    spike.setTexture(spiketex);
    //set positions
   
    
    sonic.player.setPosition(350, 700);
    spike.setPosition(1300, 910);
    wall.setPosition(1000, 630);
    Ground.setPosition(200, 800);
    
    
    //scaling
    sonic.player.setScale(Vector2f(2.f, 2.f));
    sonic.PlayerColl.setSize(Vector2f(45.f, 70.f));
    sonic.LeftColl.setSize(Vector2f(15.f, 70.f));
    sonic.RightColl.setSize(Vector2f(15.f, 70.f));
    
    wall.setScale(1.6, 1);
    Ground.setScale(11.9, 2);
    spike.setScale(0.5, 0.3);
    //Background
    Sprite background[15];
    background[0].setTexture(backgroundTexture);
    background[0].setPosition(Vector2f(-1920, 0));
    for (int i = 1, j = 0; i <= 15; i++, j++)
    {
        background[i].setTexture(backgroundTexture);
        background[i].setPosition(Vector2f((j * 1920), 0));
    }

    //Rocks
    Sprite rock1;
    Sprite rock3;
    rock1.setTexture(rockTexture);
    rock3.setTexture(rockTexture);
    rock1.setPosition(-690, 543);
    rock3.setPosition(-905, 747);
    rock3.setScale(0.5, 0.5);

    //Border
    RectangleShape border1(Vector2f(10, 1080));
    border1.setFillColor(Color::Transparent);

    //Ground
    Sprite ground[1000];
    //Ground Behind Sonic
    for (int i = 0, j = -1; i < 8; i++, j--)
    {
        ground[i].setScale(Vector2f(2, 2));
        ground[i].setTexture(groundTexture);
        ground[i].setPosition(Vector2f((j * 256), 952));
    }
    //Ground Infront Of Sonic
    for (int i = 8, j = 0; i < 100; i++, j++)
    {
        ground[i].setScale(Vector2f(2, 2));
        ground[i].setTexture(groundTexture);
        ground[i].setPosition(Vector2f((j * 256), 952));
    }

    //Rings
    Sprite ring[100];
    int ringanimator = 0;
    for (int i = 0; i < 11; i++)
    {
        ring[i].setTexture(ringTexture);
        ring[i].setPosition(Vector2f(960 + (i * 50), 890));
    }
    for (int i = 11, j = 0; i < 22; i++, j++)
    {
        ring[i].setTexture(ringTexture);
        ring[i].setPosition(Vector2f(960 + (j * 50), 890 - 62));
    }

    //Fonts
    int score = 0;
    Font font;
    font.loadFromFile("font2.ttf");
    Text collector;
    collector.setFont(font);
    collector.setString("Score: "+ to_string(score));
    collector.setPosition(-600, 100);
    collector.setCharacterSize(72);
    collector.setFillColor( Color:: White);

    View cam(Vector2f(0.f, 0.f), Vector2f(1920.f, 1080.f));
    //
    while (window.isOpen())
    {
        cam.setCenter(sonic.player.getPosition());
        cam.setCenter(Vector2f(sonic.player.getPosition().x, 600));
        window.setView(cam);
        gameclock.restart();
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            if (event.type == Event::KeyPressed)
            {
                if ((event.key.code == Keyboard::Space) && isground  && !isdead)
                {
                    sonic.Velocity.y = -30;
                    isground = false;
                   
                }


            }
            if (event.type == Event::KeyReleased)
            {
                if ((event.key.code == Keyboard::A))
                {
                    animleft = 0;

                    sonic.Velocity.x = 0;
                }
                if ((event.key.code == Keyboard::D))
                {
                    animright = 0;
                    sonic.Velocity.x = 0;
                }

            }
        }
        sonic.player.move(sonic.Velocity.x, sonic.Velocity.y);

        // collision system and gravity system
        if (!isdead)
        {
            for (int i = 0; i < 65; i++)
            {
                if (sonic.player.getGlobalBounds().intersects(ground[i].getGlobalBounds()))
                {
                    if ((sonic.player.getPosition().x >= ground[i].getPosition().x+20000 ))
                    {
                        

                    }
                    else if ((sonic.PlayerColl.getPosition().x + 40 <= ground[i].getPosition().x ))
                    {
                       
                    }
                    else
                    {
                        sonic.player.setPosition(sonic.player.getPosition().x, ground[i].getPosition().y-75);
                        sonic.Velocity.y = -0.01;
                        isground = true;
                    }

                }
                else
                {
                    sonic.Velocity.y += 0.02;
                }
            }
               
            
             if (sonic.player.getGlobalBounds().intersects(wall.getGlobalBounds()))
            {

                if ((sonic.player.getPosition().x >= wall.getPosition().x + 240))
                {
                    sonic.player.setPosition(sonic.player.getPosition().x + 15, sonic.player.getPosition().y);

                    collector.setPosition(collector.getPosition().x + 15, collector.getPosition().y);
                }
                else if ((sonic.PlayerColl.getPosition().x + 15 <= wall.getPosition().x))
                {
                    sonic.player.setPosition(sonic.player.getPosition().x - 15, sonic.player.getPosition().y);
                    
                    collector.setPosition(collector.getPosition().x - 15, collector.getPosition().y);
                }
                else if ((sonic.PlayerColl.getPosition().y > wall.getPosition().y + 28))
                {

                    sonic.player.setPosition(sonic.player.getPosition().x, sonic.player.getPosition().y + (40));
                    isground = false;
                    sonic.Velocity.y += 5;
                    collector.move(0, 0);
                }
                else
                {
                    sonic.player.setPosition(sonic.player.getPosition().x, wall.getPosition().y - 75);
                    sonic.Velocity.y = -0.01;
                    isground = true;
                }

            }

            


            //jump animation
            if (!isground  && !isdead)
            {
                if (timer < 0)
                {
                    jumpanim++;
                    jumpanim = jumpanim % 16;

                    sonic.player.setTextureRect(IntRect((jumpanim * 48.87), 2 * 59.4, 48.87, 46));
                    timer = delay;
                }
                else
                    timer -= deltatime;
            }




            //player collision
            if (Keyboard::isKeyPressed(Keyboard::A))
            {

                if (isground  && !isdead)
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
                                sonic.Velocity.x -= 1.5;

                                sonic.player.setTextureRect(IntRect((animleft * 48.87), 1 * 59.4, 48.87, 46));
                                timer = delay;
                            }
                            else
                                timer -= deltatime;
                        }
                        else
                        {
                            sonic.player.setTextureRect(IntRect((animleft * 48.87), 1 * 59.4, 48.87, 46));
                            timer = delay;
                        }
                    }
                    else
                        timer -= deltatime;
                }
               
                
                    sonic.player.setScale(-2, 2);
                    sonic.Velocity.x = -5;
                    sonic.player.setOrigin(0, 0);
                    collector.move(-5, 0);
            }


            if (Keyboard::isKeyPressed(Keyboard::D))
            {

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
                                sonic.Velocity.x = 10;

                                sonic.player.setTextureRect(IntRect((animright * 48.87), 1 * 59.4, 48.87, 46));
                                timer = delay;
                            }
                            else
                                timer -= deltatime;
                        }
                        else
                        {
                            sonic.player.setTextureRect(IntRect((animright * 48.87), 1 * 59.4, 48.87, 46));
                            timer = delay;
                        }
                    }
                    else
                        timer -= deltatime;
                }

                
                
                    sonic.player.setScale(2, 2);
                    sonic.Velocity.x = 5;
                    sonic.player.setOrigin(sonic.player.getLocalBounds().width, 0);
                    collector.move(5, 0);

               
            }
            if (isground  && !(Keyboard::isKeyPressed(Keyboard::D)) && !(Keyboard::isKeyPressed(Keyboard::A)) && !(Keyboard::isKeyPressed(Keyboard::Space)))
            {
                if (timer < 0)
                {
                    idleanim++;
                    idleanim = idleanim % 8;

                    sonic.player.setTextureRect(IntRect((idleanim * 48.87), 0, 48.87, 41));
                    timer = delay;
                }
                else
                    timer -= deltatime;
            }

            if (sonic.PlayerColl.getGlobalBounds().intersects(spike.getGlobalBounds()))
            {

                if ((sonic.PlayerColl.getPosition().x >= spike.getPosition().x + 65))
                {

                    sonic.player.setPosition(sonic.player.getPosition().x, sonic.player.getPosition().y - 200);
                    isdead = true;


                }
                else if ((sonic.PlayerColl.getPosition().x + 38 <= spike.getPosition().x))
                {

                    sonic.player.setPosition(sonic.player.getPosition().x, sonic.player.getPosition().y - 200);
                    isdead = true;


                }
                else 
                {

                    sonic.Velocity.y = -0.01;
                    sonic.player.setPosition(sonic.player.getPosition().x, sonic.player.getPosition().y - 200);
                    isdead = true;
                }

            }
        }
        if (isdead )
        {
            for (int deadanim = 6; deadanim <= 7; deadanim++)
            {
                if (timer < 0)
                {
                    sonic.Velocity.y += 1;
                    sonic.Velocity.x = 0;

                    sonic.player.setTextureRect(IntRect((deadanim * 45.5), 4 * 55, 47, 50));
                    timer = 0.7;
                }
                else
                    timer -= deltatime;
            }
        }
        if (sonic.player.getGlobalBounds().intersects(border1.getGlobalBounds()))
        {
            sonic.Velocity.x = 5;
        }

        //Rings Disappearing When Collision
        for (int i = 0; i < 22; i++)
        {
            if (sonic.player.getGlobalBounds().intersects(ring[i].getGlobalBounds()))
            {
                ring[i].setScale(Vector2f(0, 0));
                score++;
            }
        }

        //Rings Animation
        ringanimator++;
        ringanimator %= 10;
        for (int i = 0; i < 22; i++)
        {
            ring[i].setTextureRect(IntRect((ringanimator * 64), 0, 64, 62));
        }
        if (sonic.PlayerColl.getPosition().y > window.getSize().y)
        {
            sonic.player.setPosition(300, 720);
            collector.setPosition(-600, 100);
            isdead = false;
            isground = true;
        }


        collector.setString("Score: "+ to_string(score));

        // position of sonic collision
        sonic.PlayerColl.setPosition(sonic.player.getPosition().x - 70, sonic.player.getPosition().y + 10);
        sonic.LeftColl.setPosition(sonic.player.getPosition().x - 70, sonic.player.getPosition().y + 10);
        sonic.RightColl.setPosition(sonic.player.getPosition().x - 40, sonic.player.getPosition().y + 10);

        //Update
        window.clear();
        //Draw

        window.draw(collector);
        for (int i = 0; i <= 14; i++)
            window.draw(background[i]);
        for (int i = 0; i < 65; i++)
            window.draw(ground[i]);
        window.draw(border1);
        for (int i = 0; i < 22; i++)
            window.draw(ring[i]);
        window.draw(rock1);
        window.draw(rock3);
        window.draw(sonic.player);
        window.draw(collector);
        window.draw(wall);
        window.draw(spike);
       
        window.draw(collector);
        
        
       
       



        //Display
        window.display();
        deltatime = gameclock.getElapsedTime().asSeconds();

    }
}