#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include<cstdlib>
#include<time.h>
using namespace sf;
using namespace std;


struct player
{
    Texture sonicTex;
    Sprite player;
    RectangleShape PlayerColl , LeftColl ,RightColl;
    Vector2f Velocity;
    int runrightanimation = 0, Idleanimation = 0, runleftanimation = 0, takingdamageanimation = 0, jumpinganimation = 0;
    int scoreValue = 0, lives = 5, hitCounter = -1, deathDealy = 0, FinalScore = 0;
    bool start = false, Running = false;
    bool on_ground = true, hitRight = false, hitLeft = false, RunningSound = false, HitAbove = false;
};


int main()
{

    bool isground = false;
    Clock gameclock;
    float delay = 0.1f;
    float deltatime = 0;
    float timer = 0;

    int animleft = 0;
    int animright = 0;
    int jumpanim = 0;
    int idleanim = 0;

    RenderWindow window(VideoMode(1920, 1080), "Sonic.exe", Style::Default);
    window.setFramerateLimit(60);

    RectangleShape ground(Vector2f(1500, 80));
    ground.setPosition(200, 800);
    RectangleShape block(Vector2f(200, 30));
    block.setPosition(1000, 520);
    //spikes
   
   //


    //sonic
    player sonic;
    //set texture;
    sonic.player.setTexture(sonic.sonicTex);
    //load images
    sonic.sonicTex.loadFromFile("sonic22.png");
    // scaling
    sonic.player.setScale(Vector2f(2.f, 2.f));
    sonic.player.setTextureRect(IntRect(48.87, 0, 48.87, 43));
    sonic.player.setPosition(250, 700);
    // collision scaling
    sonic.PlayerColl.setSize(Vector2f(45.f, 70.f));
    sonic.LeftColl.setSize(Vector2f(15.f, 70.f));
    sonic.RightColl.setSize(Vector2f(15.f, 70.f));
    
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


            if (sonic.player.getGlobalBounds().intersects(ground.getGlobalBounds()))
            {
                if ((sonic.player.getPosition().x >= ground.getPosition().x + 1550))
                {
                    sonic.player.setPosition(sonic.player.getPosition().x + 15, sonic.player.getPosition().y);
                    isground = false;

                }
                else if ((sonic.PlayerColl.getPosition().x +40 <= ground.getPosition().x))
                {
                    sonic.player.setPosition(sonic.player.getPosition().x - 15, sonic.player.getPosition().y);
                    isground = false;
                }
                else
                {
                    sonic.player.setPosition(sonic.player.getPosition().x , ground.getPosition().y -80);
                    sonic.Velocity.y = -0.01;
                    isground = true;
                }
                sonic.Velocity.y = -0.01;
                isground = true;
            }
            else if (sonic.player.getGlobalBounds().intersects(block.getGlobalBounds()))
            {
                if ((sonic.player.getPosition().x >= block.getPosition().x + 260))
                {
                    sonic.player.setPosition(sonic.player.getPosition().x + 15, sonic.player.getPosition().y-5);
                   

                }
                else if ((sonic.PlayerColl.getPosition().x + 40 <= block.getPosition().x))
                {
                    sonic.player.setPosition(sonic.player.getPosition().x - 15, sonic.player.getPosition().y-5);
                    
                }
                else if ((sonic.PlayerColl.getPosition().y > block.getPosition().y + 30))
                {
                    
                    sonic.player.setPosition(sonic.player.getPosition().x , sonic.player.getPosition().y +(15));
                    
                }
                else
                {
                    sonic.player.setPosition(sonic.player.getPosition().x, block.getPosition().y - 80);
                    sonic.Velocity.y = -0.01;
                    isground = true;
                }
                sonic.Velocity.y = -0.01;
                isground = true;
            }
            else
            {
                isground = false;
                sonic.Velocity.y +=1.5;
                
            }
            if (!isground)
            {
                if (timer < 0)
                {
                    jumpanim++;
                    jumpanim = jumpanim % 16;

                    sonic.player.setTextureRect(IntRect((jumpanim * 48.87), 2* 59.4, 48.87, 46));
                    timer = delay;
                }
                else
                  timer -= deltatime;
            }
            
            


            //player collision
            if (Keyboard::isKeyPressed(Keyboard::A))
            {

                if(isground)
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




            sonic.PlayerColl.setPosition(sonic.player.getPosition().x-70 , sonic.player.getPosition().y + 30);
            sonic.LeftColl.setPosition(sonic.player.getPosition().x -70, sonic.player.getPosition().y + 30);
            sonic.RightColl.setPosition(sonic.player.getPosition().x -40, sonic.player.getPosition().y + 30);
            
            //
            window.clear();
            //



            window.draw(sonic.player);
            
            

            
            window.draw(ground);
            window.draw(block);
           
           

            //
            window.display();
            deltatime = gameclock.getElapsedTime().asSeconds();

        }
    }






