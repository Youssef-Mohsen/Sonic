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

    RectangleShape ground(Vector2f(1024, 80));
    ground.setPosition(0, 600);
    //spikes
    Sprite spike;
    Texture spiketex;
    spike.setTexture(spiketex);
    spiketex.loadFromFile("Spike.png");
    spike.setScale(Vector2f(1.f, 1.f));
    spike.setPosition(200, 450);
   //


    //sonic
    player sonic;
    //set texture;
    sonic.player.setTexture(sonic.sonicTex);
    //load images
    sonic.sonicTex.loadFromFile("sonic.png");
    // scaling
    sonic.player.setScale(Vector2f(2.f, 2.f));
    sonic.player.setTextureRect(IntRect(48.87, 0, 48.87, 59.4));
    sonic.player.setPosition(80, 500);
    // collision scaling
    sonic.PlayerColl.setSize(Vector2f(55.f, 80.f));
    sonic.LeftColl.setSize(Vector2f(15.f, 80.f));
    sonic.RightColl.setSize(Vector2f(15.f, 80.f));
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
        }
            sonic.player.move(0, sonic.Velocity.y);


            if (sonic.player.getGlobalBounds().intersects(ground.getGlobalBounds()))
            {
                sonic.Velocity.y = 0;
                isground = true;
            }
            else
            {
                isground = false;
                sonic.Velocity.y +=1.5;
                
            }
            if (sonic.Velocity.y != 0)
            {
                if (timer < 0)
                {
                    jumpanim++;
                    jumpanim = jumpanim % 16;

                    sonic.player.setTextureRect(IntRect((jumpanim * 48.87), 2* 59.4, 48.87, 59.4));
                    timer = delay;
                }
                else
                  timer -= deltatime;
            }
            


            //player collision
            if (Keyboard::isKeyPressed(Keyboard::A))
            {

                if(sonic.Velocity.y==0)
                {
                    if (timer < 0)
                    {
                        animright++;
                        animright = animright % 23;

                        sonic.player.setTextureRect(IntRect((animright * 48.87), 1*59.4, 48.87, 59.4));
                        timer = delay;
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

                if (sonic.Velocity.y == 0)
                {
                    if (timer < 0)
                    {
                        animleft++;
                        animleft = animleft % 23;

                        sonic.player.setTextureRect(IntRect((animleft * 48.87), 59.4, 48.87, 59.4));
                        timer = delay;
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

                    sonic.player.setTextureRect(IntRect((idleanim * 48.87), 0, 48.87, 59.4));
                    timer = delay;
                }
                else
                    timer -= deltatime;
            }




            sonic.PlayerColl.setPosition(sonic.player.getPosition().x + 20, sonic.player.getPosition().y + 40);
            sonic.LeftColl.setPosition(sonic.player.getPosition().x + 5, sonic.player.getPosition().y + 40);
            sonic.RightColl.setPosition(sonic.player.getPosition().x + 58, sonic.player.getPosition().y + 40);
            //
            window.clear();
            //



            window.draw(sonic.player);
            window.draw(spike);
            window.draw(ground);
           

            //
            window.display();
            deltatime = gameclock.getElapsedTime().asSeconds();

        }
    }






