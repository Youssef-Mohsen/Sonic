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
    bool isground = false;
    Clock gameclock;
    float delay = 0.1f;
    float deltatime = 0;
    float timer = 0;

    int animleft = 0;
    int animright = 0;
    int jumpanim = 0;
    int idleanim = 0;

    RenderWindow window(VideoMode(1940, 1080), "Sonic.exe", Style::Default);
    window.setFramerateLimit(60);
//structs
    //sonic
    player sonic;
    sonic.player.setTextureRect(IntRect(48.87, 0, 48.87, 43));

//out of structs
    
    
    //Textures
    Texture backgroundTexture;
    Texture spiketex;
    Texture walltex;
    Texture Groundtex;
    
    //Loading from files
    backgroundTexture.loadFromFile("background.png");
    
    sonic.sonicTex.loadFromFile("sonic22.png");
    walltex.loadFromFile("Wall.png");
    Groundtex.loadFromFile("Wall.png");
   
   
    
    //sprites
    Sprite background;
   
    Sprite wall;
    Sprite Ground;
    // set texture
    sonic.player.setTexture(sonic.sonicTex);
    background.setTexture(backgroundTexture);
    
    wall.setTexture(walltex);
    Ground.setTexture(Groundtex);
    //set positions
   
    
    sonic.player.setPosition(250, 700);
    
    wall.setPosition(1000, 520);
    Ground.setPosition(200, 800);
    
    //scaling
    sonic.player.setScale(Vector2f(2.f, 2.f));
    sonic.PlayerColl.setSize(Vector2f(45.f, 70.f));
    sonic.LeftColl.setSize(Vector2f(15.f, 70.f));
    sonic.RightColl.setSize(Vector2f(15.f, 70.f));
    
    wall.setScale(1.6, 1);
    Ground.setScale(11.9, 2);
    

    //
    while (window.isOpen())
    {
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
                if ((event.key.code == Keyboard::Space) && isground)
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
                }
                if ((event.key.code == Keyboard::D))
                {
                    animright = 0;
                }

            }
        }
        sonic.player.move(0, sonic.Velocity.y);


        if (sonic.player.getGlobalBounds().intersects(Ground.getGlobalBounds()))
        {
            if ((sonic.player.getPosition().x >= Ground.getPosition().x + 1550))
            {
                sonic.player.setPosition(sonic.player.getPosition().x + 15, sonic.player.getPosition().y);
                isground = false;

            }
            else if ((sonic.PlayerColl.getPosition().x + 40 <= Ground.getPosition().x))
            {
                sonic.player.setPosition(sonic.player.getPosition().x - 15, sonic.player.getPosition().y);
                isground = false;
            }
            else
            {
                sonic.player.setPosition(sonic.player.getPosition().x, Ground.getPosition().y - 73);
                sonic.Velocity.y = -0.01;
                isground = true;
            }

        }
        else if (sonic.player.getGlobalBounds().intersects(wall.getGlobalBounds()))
        {
            if ((sonic.player.getPosition().x >= wall.getPosition().x + 260))
            {
                sonic.player.setPosition(sonic.player.getPosition().x + 15, sonic.player.getPosition().y );


            }
            else if ((sonic.PlayerColl.getPosition().x + 40 <= wall.getPosition().x))
            {
                sonic.player.setPosition(sonic.player.getPosition().x - 15, sonic.player.getPosition().y );

            }
            else if ((sonic.PlayerColl.getPosition().y > wall.getPosition().y + 28))
            {

                sonic.player.setPosition(sonic.player.getPosition().x, sonic.player.getPosition().y + (40));
                isground = false;
                sonic.Velocity.y += 5;
            }
            else
            {
                sonic.player.setPosition(sonic.player.getPosition().x, wall.getPosition().y - 75);
                sonic.Velocity.y = -0.01;
                isground = true;
            }

        }
        else
        {
            isground = false;
            sonic.Velocity.y += 1.5;

        }
        if (!isground)
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

            if (isground)
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
            sonic.player.move(-5, 0);
            sonic.player.setOrigin(0, 0);
        }


        if (Keyboard::isKeyPressed(Keyboard::D))
        {



            if (isground)
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
            sonic.player.move(5, 0);

            sonic.player.setOrigin(sonic.player.getLocalBounds().width, 0);
        }
        if (isground && !(Keyboard::isKeyPressed(Keyboard::D)) && !(Keyboard::isKeyPressed(Keyboard::A)) && !(Keyboard::isKeyPressed(Keyboard::Space)))
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




        sonic.PlayerColl.setPosition(sonic.player.getPosition().x - 70, sonic.player.getPosition().y + 30);
        sonic.LeftColl.setPosition(sonic.player.getPosition().x - 70, sonic.player.getPosition().y + 30);
        sonic.RightColl.setPosition(sonic.player.getPosition().x - 40, sonic.player.getPosition().y + 30);

        //
        window.clear();
        //


        window.draw(background);
        
        window.draw(sonic.player);
        
        window.draw(Ground);
       
        
        window.draw(wall);
        
       
       



        //
        window.display();
        deltatime = gameclock.getElapsedTime().asSeconds();

    }
}