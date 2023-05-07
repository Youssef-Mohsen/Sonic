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
// Global //
////////////
bool mainmusicison = true;
bool mainmusicispermitted = true;
bool ispaused = false;
bool restart = false;
bool gameover = false;
bool level = false;
int volume = 5;
int pagenumber = 0;
int score = 0;
int selected = 0;
int mainmenuselected = 0;
int b = 0;
int p = 0;
int levelselected = 0;
int playerprogress = 1;
Texture healthbar[12];
string pname;

////////////
// Structs//
////////////
struct player
{
    Texture sonicTex;
    Sprite player;
    RectangleShape PlayerColl;
    Vector2f Velocity;
    int animright = 0, idleanim = 0, animleft = 0, jumpanim = 0, deadanim = 0,attackanimInd = 0;;
    float deathdelay = 0.3, timer = 0, acc = 0;
    int score = 0, lives = 5, highscore = 0;
    bool start = false, Running = false;
    bool  hitRight = false, hitLeft = false, slidingLeft = false, slidingRight = false,isground = false, isdead = false, check[3] = { false };
    
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
struct boss
{
    Sprite sprite;
    Texture bosstex;
    Vector2f Velocity;
    RectangleShape coll;
    int bossanimInd = 0;
    int health = 10000;
    float bossdelay = 0.3, bosstimer = 0;
    bool isdead = false, isground = false;
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
void pausemenufunc(RenderWindow& window, Sprite& sonic, int& x,bool&check);
void mainmusic(Music& mainmenumusic);
void playername(RenderWindow& window);
void leaderboard(RenderWindow& window);
void options(RenderWindow& window, Music& mainmenumusic);
void mainmenu(RenderWindow& window);
void levelselection(RenderWindow& window);
void Game(RenderWindow&window,float&delay,float&deltatime,Clock& gameclock);
void init_health_bar();
int update_health_bar(int bosshealth);
void boss_level(RenderWindow& window, Clock& gameclock, float& deltatime, float& delay);
int main()
{
    ////////////
    //Variables//
    ////////////
    Clock gameclock;
    float delay = 0.1f;
    float deltatime = 0;
    
    ////////////
    // Window //
    ////////////
    RenderWindow window(VideoMode(1930, 1080), "Sonic.exe", Style::Default);
    window.setFramerateLimit(60);
   
    Music mainmenumusic;
    mainmenumusic.openFromFile("Sounds/mainmenumusic.ogg.opus");
    mainmenumusic.setVolume(volume);

    while (window.isOpen())
    {
        mainmusic(mainmenumusic);

        if (pagenumber == 0)
        {
            mainmenu(window);
        }
        else if (pagenumber == 1)
        {
            playername(window);
        }
        else if (pagenumber == 2)
        {
            leaderboard(window);
        }
        else if (pagenumber == 3)
        {
            options(window, mainmenumusic);
        }
        else if (pagenumber == 4)
        {
            levelselection(window);
        }
        else if (pagenumber == 5)
        {
            Game(window, delay, deltatime, gameclock);
        }
        else if (pagenumber == 6)
        {
            boss_level(window, gameclock, deltatime, delay);
        }
        else if (pagenumber == -1)
        {
            window.close();
        }
    }
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
void pausemenufunc(RenderWindow& window,Sprite&sonic,int&x,bool& check)
{
    selected = 0;
    int pressed=1000;

    Texture levelmenupic;
    levelmenupic.loadFromFile("Images/realinfobackground.png");
    Sprite levelbackpic;
    levelbackpic.setTexture(levelmenupic);
    levelbackpic.setPosition(sonic.getPosition().x - 960, 0);

    Font pausefont;
    pausefont.loadFromFile("Fonts/font.ttf");
    Text pausemneutext[3];

    pausemneutext[0].setFont(pausefont);
    pausemneutext[0].setFillColor(Color(255, 204, 0));
    pausemneutext[0].setString("continue");
    pausemneutext[0].setCharacterSize(60);
    

    pausemneutext[1].setFont(pausefont);
    pausemneutext[1].setFillColor(Color(255, 250, 250));
    pausemneutext[1].setString("Restart");
    pausemneutext[1].setCharacterSize(60);
   

    pausemneutext[2].setFont(pausefont);
    pausemneutext[2].setFillColor(Color(255, 250, 250));
    pausemneutext[2].setString("Quit");
    pausemneutext[2].setCharacterSize(60);
    

    if(level)
    {
        for (int i = 0, j = 0; i < 3; i++, j++)
        {
            pausemneutext[i].setPosition(b, 400 + (200 * j));
        }
    }
    else
    {
        pausemneutext[0].setPosition(850, 400);
        pausemneutext[1].setPosition(850, 600);
        pausemneutext[2].setPosition(850, 800);
        levelbackpic.setPosition(40, 0);
    }
    while (ispaused == true)
    {
        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                    break;
                }

                if (event.type == Event::KeyReleased)
                {
                    if (event.key.code == Keyboard::Up)
                    {
                        pausemneutext[selected].setFillColor(Color(255, 250, 250));
                        selected--;
                        pausemneutext[selected].setFillColor(Color(255, 204, 0));
                        if (selected == -1)
                        {
                            selected = 2;
                            pausemneutext[selected].setFillColor(Color(255, 204, 0));

                        }
                    }
                    if (event.key.code == Keyboard::Down)
                    {
                        pausemneutext[selected].setFillColor(Color(255, 250, 250));
                        selected++;
                        pausemneutext[selected].setFillColor(Color(255, 204, 0));
                        if (selected == 3)
                        {
                            selected = 0;
                            pausemneutext[selected].setFillColor(Color(255, 204, 0));
                        }
                    }
                    if (event.key.code == Keyboard::Enter && !check)
                    {
                        pressed = selected;

                        if (pressed == 0)
                        {
                            sonic.setPosition(x, 800);
                            ispaused = false;
                            return;
                        }
                        else if (pressed == 1)
                        {
                            ispaused = false;
                            gameover = false;
                            restart = true;
                            return;
                        }
                        else if (pressed == 2)
                        {
                            ispaused = false;
                            window.close();
                            return;
                        }
                    }
                    if (event.key.code == Keyboard::Enter && check)
                    {
                        pressed = selected;

                        if (pressed == 0)
                        {
                            pagenumber=0;
                            ispaused = false;
                            return;
                        }
                        else if (pressed == 1)
                        {
                            ispaused = false;
                            gameover = false;
                            restart = true;
                            return;
                        }
                        else if (pressed == 2)
                        {
                            ispaused = false;
                            window.close();
                            pagenumber = 0;
                            return;
                        }
                    }
                }
            }
            window.clear();
            window.draw(levelbackpic);
            for (int i = 0; i < 3; i++)
            {
                window.draw(pausemneutext[i]);
            }
            window.display();
        }
    }
}
void mainmusic(Music& mainmenumusic)
{
    if (mainmusicispermitted == true)
    {
        mainmenumusic.play();
    }
    else
    {
        mainmenumusic.stop();
    }
    return;
}
void playername(RenderWindow& window)
{
    if (!pname.empty())
        pname.clear();
    Texture playerinfoback;
    playerinfoback.loadFromFile("Images/realinfobackground.png");
    Sprite playerinfobacks;
    playerinfobacks.setTexture(playerinfoback);
    Font font;
    font.loadFromFile("Fonts/Font.ttf");
    Text t1;
    Text t2;
    t1.setFont(font);
    t2.setFont(font);
    t1.setString("please, Enter First name: ");
    t1.setCharacterSize(80);
    t2.setCharacterSize(100);
    
    t1.setFillColor(Color(255, 250, 250));
    t2.setFillColor(Color(255, 250, 250));
    if(level)
    {
        playerinfobacks.setPosition(p, 0);
        t1.setPosition(p + 600, 400);
        t2.setPosition(p + 750, 600);
    }
    else
    {
        playerinfobacks.setPosition(0, 0);
        t1.setPosition(600, 400);
        t2.setPosition(750, 600);
    }
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed())
                window.close();

            if (event.type == Event::TextEntered)
            {
                pname += static_cast<char>(event.text.unicode);
                if (Keyboard::isKeyPressed(Keyboard::Backspace) && pname.size() > 0)
                    pname.resize(pname.size() - 1);
            }
            if (event.type == Event::KeyReleased)
            {
                if (event.key.code == Keyboard::Escape)
                {
                    pagenumber = 0;
                    return;
                }
                if (event.key.code == Keyboard::Return && pname.size() > 1)
                {
                    pagenumber = 4;
                    mainmusicispermitted = false;
                    return;
                }
            }
        }

        t2.setString(pname);
        window.clear();
        window.draw(playerinfobacks);
        window.draw(t1);
        window.draw(t2);
        window.display();
    }
}
void leaderboard(RenderWindow& window)
{

    Texture leaderboardbackground;
    leaderboardbackground.loadFromFile("Images/leaderboardinfo.png");
    Sprite leaderboardbackgrounds;
    leaderboardbackgrounds.setTexture(leaderboardbackground);


    Font scorefont;
    scorefont.loadFromFile("Fonts/Font.ttf");

    Text leaderboardmaintext[2];

    leaderboardmaintext[0].setFont(scorefont);;
    leaderboardmaintext[0].setFillColor(Color(255, 250, 250));
    leaderboardmaintext[0].setCharacterSize(60);
    
    leaderboardmaintext[0].setString("Name :");

    leaderboardmaintext[1].setFont(scorefont);;
    leaderboardmaintext[1].setFillColor(Color(255, 250, 250));
    leaderboardmaintext[1].setCharacterSize(60);
    leaderboardmaintext[1].setString("Score :");

    if(level)
    {
        leaderboardbackgrounds.setPosition(p, 0);
        leaderboardmaintext[0].setPosition(p + 200, 150);
        leaderboardmaintext[1].setPosition(p + 1000, 150);
    }
    else
    {
        leaderboardbackgrounds.setPosition(0, 0);
        leaderboardmaintext[0].setPosition(200, 150);
        leaderboardmaintext[1].setPosition(1000, 150);
    }
    Text scoretexts[51];
    for (int i = 0; i <= 50; i++)
    {
        scoretexts[i].setFont(scorefont);
        scoretexts[i].setFillColor(Color(255, 250, 250));
        scoretexts[i].setCharacterSize(60);
    }

    ifstream infile;
    infile.open("leaderboardhistory.txt", ios::in);

    vector<string> lines;
    string line;
    while (getline(infile, line, '*'))
    {
        lines.push_back(line);
    }
    for (int i = 0; i < lines.size(); i++)
    {
        scoretexts[i].setString(lines[i]);
        scoretexts[i].setPosition(p+400, (250) + (200 * i));
    }

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed())
                window.close();

            if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
            {
                pagenumber = 0;
                return;
            }

            if (Keyboard::isKeyPressed(Keyboard::Key::Up))
            {
                if (scoretexts[0].getPosition().y <= 5)
                {
                    for (int i = 0; i < lines.size(); i++)
                    {
                        scoretexts[i].move(0, 5);
                    }
                }
            }

            if (Keyboard::isKeyPressed(Keyboard::Key::Down))
            {
                if (scoretexts[(lines.size()) - 1].getPosition().y >= window.getPosition().y + 1000)
                {
                    for (int i = 0; i < lines.size() + 10; i++)
                    {
                        scoretexts[i].move(0, -5);
                    }
                }
            }
        }
        window.clear();
        window.draw(leaderboardbackgrounds);
        for (int i = 0; i < 2; i++)
        {
            window.draw(leaderboardmaintext[i]);
        }
        for (int i = 0; i < lines.size(); i++)
        {
            window.draw(scoretexts[i]);
        }
        window.display();
    }
}
void options(RenderWindow& window, Music& mainmenumusic)
{

    Texture optionsmenu;
    optionsmenu.loadFromFile("Images/realinfobackground.png");
    Sprite optionsmenus;
    optionsmenus.setTexture(optionsmenu);

    Font font;
    font.loadFromFile("Fonts/Font.ttf");

    Text optionstext[2];

    optionstext[0].setFont(font);
    optionstext[0].setFillColor(Color(255, 204, 0));
    optionstext[0].setString("MENU SOUND :");
    optionstext[0].setCharacterSize(80);
    ;


    optionstext[1].setFont(font);
    optionstext[1].setFillColor(Color(255, 250, 250));
    optionstext[1].setCharacterSize(100);
    optionstext[1].setPosition(p+1100, 390);
    if(level)
    {
        optionsmenus.setPosition(p, 0);
        optionstext[0].setPosition(p + 600, 400);
        optionstext[1].setPosition(p + 1100, 390);
    }
    else
    {
        optionsmenus.setPosition(0, 0);
        optionstext[0].setPosition(600, 400);
        optionstext[1].setPosition(1100, 390);
    }
    if (pagenumber == 3)
    {
        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {

                if (event.type == Event::Closed)
                {
                    window.close();
                    break;
                }

                if (event.type == Event::KeyPressed)
                {
                    if (event.key.code == Keyboard::Key::Right)
                    {
                        if (mainmenumusic.getVolume() <= 100)
                            mainmenumusic.setVolume(mainmenumusic.getVolume() + 2);
                        if (mainmenumusic.getVolume() + 2 > 100)
                            mainmenumusic.setVolume(100);
                        volume = mainmenumusic.getVolume();
                    }

                    if (event.key.code == Keyboard::Key::Left)
                    {
                        if (mainmenumusic.getVolume() >= 0)
                            mainmenumusic.setVolume(mainmenumusic.getVolume() - 2);
                        if (mainmenumusic.getVolume() - 2 < 0)
                            mainmenumusic.setVolume(0);
                        volume = mainmenumusic.getVolume();
                    }
                }

                if (event.type == Event::KeyReleased)
                {
                    if (event.key.code == Keyboard::Escape)
                    {
                        pagenumber = 0;
                        return;
                    }
                }


            }
            volume = mainmenumusic.getVolume();
            string volumestring = to_string(volume);
            optionstext[1].setString(volumestring);
            window.clear();
            window.draw(optionsmenus);
            for (int i = 0; i < 2; i++)
            {
                window.draw(optionstext[i]);
            }
            window.display();
        }
    }
}
void mainmenu(RenderWindow& window)
{
    mainmenuselected = 0;
    int pressed =1000;

    Texture mainmenupic;
    mainmenupic.loadFromFile("Images/mainmenubackground.png");
    Sprite backpic;
    backpic.setTexture(mainmenupic);

    Font mainmenufont;
    mainmenufont.loadFromFile("Fonts/font.ttf");
    Text mainmenu[4];

    mainmenu[0].setFont(mainmenufont);
    mainmenu[0].setFillColor(Color(255, 204, 0));
    mainmenu[0].setString("Start");
    mainmenu[0].setCharacterSize(80);
    
    

    mainmenu[1].setFont(mainmenufont);
    mainmenu[1].setFillColor(Color(255, 250, 250));
    mainmenu[1].setString("leaderboard");
    mainmenu[1].setCharacterSize(80);
    
   

    mainmenu[2].setFont(mainmenufont);
    mainmenu[2].setFillColor(Color(255, 250, 250));
    mainmenu[2].setString("Options");
    mainmenu[2].setCharacterSize(80);
    

    mainmenu[3].setFont(mainmenufont);
    mainmenu[3].setFillColor(Color(255, 250, 250));
    mainmenu[3].setString("Exit");
    mainmenu[3].setCharacterSize(80);
    //220
    mainmenu[3].setPosition(Vector2f(p+220, 2000 / (4) + 350));


    Music mainmenumusic;
    mainmenumusic.openFromFile("Sounds/mainmenumusic.ogg.opus");
    if(level)
    {
        backpic.setPosition(p, 0);
        mainmenu[0].setPosition(Vector2f(p + 200, 2000 / (4) - 100));
        mainmenu[1].setPosition(Vector2f(p + 100, 2000 / (4) + 50));
        mainmenu[2].setPosition(Vector2f(p + 180, 2000 / (4) + (200)));
        mainmenu[3].setPosition(Vector2f(p + 220, 2000 / (4) + 350));
    }
    else
    {
        backpic.setPosition(0, 0);
        mainmenu[0].setPosition(Vector2f(200, 2000 / (4) - 100));
        mainmenu[1].setPosition(Vector2f(100, 2000 / (4) + 50));
        mainmenu[2].setPosition(Vector2f(180, 2000 / (4) + (200)));
        mainmenu[3].setPosition(Vector2f(220, 2000 / (4) + 350));
    }
    while (true)
    {

        if (pagenumber == 0)
        {
            while (window.isOpen())
            {
                Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == Event::Closed)
                    {
                        pagenumber = -1;
                        window.close();
                        break;
                    }

                    if (event.type == Event::KeyReleased)
                    {
                        if (event.key.code == Keyboard::Up)
                        {
                            mainmenu[mainmenuselected].setFillColor(Color(255, 250, 250));
                            mainmenuselected--;
                            mainmenu[mainmenuselected].setFillColor(Color(255, 204, 0));
                            if (mainmenuselected == -1)
                            {
                                mainmenuselected = 3;
                                mainmenu[mainmenuselected].setFillColor(Color(255, 204, 0));

                            }
                        }
                        if (event.key.code == Keyboard::Down)
                        {
                            mainmenu[mainmenuselected].setFillColor(Color(255, 250, 250));
                            mainmenuselected++;
                            mainmenu[mainmenuselected].setFillColor(Color(255, 204, 0));
                            if (mainmenuselected == 4)
                            {
                                mainmenuselected = 0;
                                mainmenu[mainmenuselected].setFillColor(Color(255, 204, 0));
                            }
                        }
                        if (event.key.code == Keyboard::Enter)
                        {
                            pressed = mainmenuselected;
                            if (pressed == 0)
                            {
                                pagenumber = 1;
                                return;
                            }
                            else if (pressed == 1)
                            {
                                pagenumber = 2;
                                return;
                            }
                            else if (pressed == 2)
                            {
                                pagenumber = 3;
                                return;
                            }
                            else if (pressed == 3)
                            {
                                pagenumber = -1;
                                return;
                            }

                        }
                       
                    }

                }
                window.clear();
                if (pagenumber != 0)
                {
                    return;
                    break;
                }
                window.draw(backpic);
                for (int i = 0; i < 4; i++)
                {
                    window.draw(mainmenu[i]);
                }
                window.display();
            }
        }
    }
}
void levelselection(RenderWindow& window)
{

    levelselected = 0;
    int pressed = 1000;

    Texture levelselectionbackground;
    levelselectionbackground.loadFromFile("Images/realinfobackground.png");

    Sprite levelselectionbackgrounds;
    levelselectionbackgrounds.setTexture(levelselectionbackground);

    Font levelselectionfont;
    levelselectionfont.loadFromFile("Fonts/font.ttf");
    Text levelselection[2];

    levelselection[0].setFont(levelselectionfont);
    levelselection[0].setFillColor(Color(255, 204, 0));
    levelselection[0].setString("Level 1 : Pacing Forward");
    levelselection[0].setCharacterSize(70);
    

    levelselection[1].setFont(levelselectionfont);
    levelselection[1].setFillColor(Color(119, 136, 153));
    levelselection[1].setString("Level 2 : Finally, We Met");
    levelselection[1].setCharacterSize(70);
    

    
    if (level)
    {
        levelselectionbackgrounds.setPosition(p, 0);
        levelselection[0].setPosition(Vector2f(p + 550, 400));
        levelselection[1].setPosition(Vector2f(p + 550,600));
    }
    else
    {
        levelselectionbackgrounds.setPosition(0, 0);
        levelselection[0].setPosition(Vector2f(550, 400));
        levelselection[1].setPosition(Vector2f(550, 600));
    }
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                break;
            }

            if (playerprogress == 1)
            {
                if (event.type == Event::KeyReleased)
                {
                    if (event.key.code == Keyboard::Enter)
                    {
                        pagenumber = 5;
                        return;
                    }

                    if (event.key.code == Keyboard::Escape)
                    {
                        pagenumber = 1;
                        return;
                    }
                }
            }

            if (playerprogress == 2)
            {

                levelselection[1].setFillColor(Color(255, 250, 250));

                if (event.type == Event::KeyReleased)
                {
                    if (event.key.code == Keyboard::Up)
                    {
                        levelselection[levelselected].setFillColor(Color(255, 250, 250));
                        levelselected--;
                        levelselection[levelselected].setFillColor(Color(255, 204, 0));
                        if (levelselected == -1)
                        {
                            levelselected = 1;
                            levelselection[levelselected].setFillColor(Color(255, 204, 0));
                        }
                    }
                    if (event.key.code == Keyboard::Down)
                    {
                        levelselection[levelselected].setFillColor(Color(255, 250, 250));
                        levelselected++;
                        levelselection[levelselected].setFillColor(Color(255, 204, 0));
                        if (levelselected == 2)
                        {
                            levelselected = 0;
                            levelselection[levelselected].setFillColor(Color(255, 204, 0));
                        }
                    }
                    if (event.key.code == Keyboard::Enter)
                    {
                        pressed = levelselected;

                        if (pressed == 0)
                        {
                            pagenumber = 5;
                            return;
                        }
                        else if (pressed == 1)
                        {
                            pagenumber = 6;
                            return;
                        }
                    }
                    if (event.key.code == Keyboard::Escape)
                    {
                        pagenumber = 0;
                        return;
                    }
                }
            }

        }
        window.clear();
        window.draw(levelselectionbackgrounds);
        for (int i = 0; i < 2; i++)
        {
            window.draw(levelselection[i]);
        }
        window.display();
    }
}
void Game(RenderWindow& window, float& delay, float& deltatime,Clock&gameclock)
{
    int x =0;
    level = true;
    ////////////
    //Structs//
    ////////////

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

    //////////////////
    //Out of structs//
    //////////////////



    //////////////
    //Velocities//
    //////////////
    Vector2f Velocity;
    Vector2f Velocity2;

    ////////////
    //Textures//
    ////////////
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
    Texture overtex;
    Texture leveltex;
    ///////////////////////
    //Loading From Files//
    //////////////////////
    crabtex.loadFromFile("Images/Enemies1.png");
    fishtex.loadFromFile("Images/Enemies2.png");
    sonic.sonicTex.loadFromFile("Images/Sonic.png");
    platformtex.loadFromFile("Images/Wall.png");
    spiketex.loadFromFile("Images/Spike.png");
    backgroundTexture.loadFromFile("Images/Background.png");
    groundTexture.loadFromFile("Images/Ground.png");
    ringTexture.loadFromFile("Images/Ring.png");
    rockTexture.loadFromFile("Images/Rock.png");
    waterTexture.loadFromFile("Images/Water.png");
    treetex.loadFromFile("Images/Tree.png");
    liveTex.loadFromFile("Images/Lives.png");
    checktex.loadFromFile("Images/Checkpoints.png");
    overtex.loadFromFile("Images/GameOver.png");
    leveltex.loadFromFile("Images/LevelComplete.png");
    ////////////
    //Sprites//
    ////////////
    Sprite lives[5];
    Sprite rock1;
    Sprite rock3;
    Sprite background;
    Sprite gameover1;
    Sprite levelcomplete;
    ////////////////////
    //Setting Textures//
    ////////////////////
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
    //game over 1
    gameover1.setTexture(overtex);
    //rock2
    rock1.setTexture(rockTexture);
    //rock3
    rock3.setTexture(rockTexture);
    //level complete
    levelcomplete.setTexture(leveltex);
    //ground and water
    for (int i = 0; i < 70; i++)
    {
        ground[i].coll.setTexture(groundTexture);
        water[i].coll.setTexture(waterTexture);
    }

    ////////////////
    //Texture Rect//
    ///////////////
    //checkpoint
    for (int i = 0; i < 3; i++)
        checkpoint[i].coll.setTextureRect(IntRect(0, 0, 120, 77));
    //background
    background.setTextureRect(IntRect(0, 0, 1920 * 33.33333, 1080));

    ////////////////////
    //Rectangle shapes//
    ////////////////////
    RectangleShape windowy;
    RectangleShape check[4];
    RectangleShape border1(Vector2f(10, 1080));

    ////////////
    //Positions//
    ////////////

    // Building
    if(level)
    {
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
        sonic.player.setPosition(20000, 720);
        sonic.PlayerColl.setPosition(250, 720);
        // crabs Position
        crab[0].enemie.setPosition(Vector2f(950, 800));
        crab[1].enemie.setPosition(Vector2f(4500, 800));
        crab[2].enemie.setPosition(Vector2f(9500, 800));
        crab[3].enemie.setPosition(Vector2f(10500, 800));
        enemiepos(crab, 14900, 800, 4, 8);
        // Rest Of Crabs Positions
        for (int i = 8; i < 100; i++)
            crab[i].enemie.setPosition(-1000, 0);
        // Checkpoints Position and Checks
        checkpoint[0].coll.setPosition(5000, 870);
        check[0].setPosition(5020, 200);
        checkpoint[1].coll.setPosition(11020, 870);
        check[1].setPosition(11000, 200);
        checkpoint[2].coll.setPosition(20520, 870);
        check[2].setPosition(20550, 200);
        // Fish Position
        enemiepos(fish, 2600, 360, 0, 3);
        enemiepos(fish, 6380, 360, 3, 7);
        enemiepos(fish, 11550, 490, 7, 13);
        // Rest of Fishs Position
        for (int i = 13; i < 100; i++)
            fish[i].enemie.setPosition(-1000, 0);
        // Window y Position
        windowy.setPosition(0, 1020);
        //Background Position
        background.setPosition(Vector2f(-1000, 0));
        // rock2 Position
        rock1.setPosition(-690, 543);
        // rock3 Position
        rock3.setPosition(-905, 747);
        //Lives Position
        for (int i = 0, j = 63; i < 5; i++)
        {
            lives[i].setPosition(-710 + (j * i), 1000);
        }
    }

    ////////////
    // Scaling//
    ////////////
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
        check[i].setSize(Vector2f(50, 1000));
    //game over1
    gameover1.setScale(1.2, 1);
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
   
    font.loadFromFile("Fonts/Font.ttf");
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
    SoundBuffer gameoverbuffer;
    // sounds
    Sound coinsound;
    Sound jumpsound;
    Sound deathsound;
    Sound enemiedeath;
    Sound checks;
    Sound gameover5;
    //set buffer
    coinsound.setBuffer(coinsoundbuffer);
    jumpsound.setBuffer(jumpsoundbuffer);
    deathsound.setBuffer(deathsoundbuffer);
    enemiedeath.setBuffer(enemiesoundbuffer);
    checks.setBuffer(checksoundbuffer);
    gameover5.setBuffer(gameoverbuffer);
    // load sounds
    coinsoundbuffer.loadFromFile("Sounds/Coin.WAV");
    jumpsoundbuffer.loadFromFile("Sounds/Jump.WAV");
    deathsoundbuffer.loadFromFile("Sounds/Death.WAV");
    enemiesoundbuffer.loadFromFile("Sounds/EnemieDeath.WAV");
    checksoundbuffer.loadFromFile("Sounds/Checks.WAV");
    gameoverbuffer.loadFromFile("Sounds/GameOver.flac");
    ///////////
    // View  //
    ///////////
    View cam(Vector2f(0.f, 0.f), Vector2f(1920.f, 1080.f));

    while (window.isOpen())
    {
        ///////////////
       // Boss Level //
       ///////////////
        if (!sonic.check[2]||!ispaused)
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
            if (sonic.lives == 0)
            {
                ofstream offile;
                offile.open("leaderboardhistory.txt", ios::app);
                offile << pname << "                                                               " << sonic.score << "*" << endl;
                sonic.score = 0;
                sonic.lives = -1;
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
                   
                }
                if ((event.key.code == Keyboard::D))
                {
                    sonic.animright = 0;
                    sonic.Velocity.x = 0;
                    Velocity.x = 0;
                    Velocity2.x = 0;
                    sonic.acc = 0;
                    
                }
                x = sonic.player.getPosition().x;
                if ((event.key.code == Keyboard::Escape) &&!sonic.check[2])
                {
                    x = sonic.player.getPosition().x;
                    ispaused = true;
                    pausemenufunc(window,sonic.player,x,sonic.check[2]);
                    if (restart == true)
                    {
                        Game(window, delay, deltatime, gameclock);
                    }
                    if (pagenumber == 0)
                        return;
                }
                if (event.type == Event::KeyReleased && sonic.check[2])
                {
                    ispaused = true;
                    pausemenufunc(window, sonic.player, x, sonic.check[2]);
                    if (restart == true)
                    {
                        Game(window, delay, deltatime, gameclock);
                    }
                    if (pagenumber == 0)
                        return;
                    if (pagenumber == 4)
                        return;
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
        if (!sonic.isdead && sonic.lives > 0 &&!sonic.check[2])
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
                        sonic.player.setPosition(sonic.player.getPosition().x - sonic.Velocity.x, sonic.player.getPosition().y);
                        playerprogress++;

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
            if (sonic.PlayerColl.getPosition().y > window.getSize().y && !sonic.check[i] && sonic.lives > 0)
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
            if (sonic.PlayerColl.getPosition().y > window.getSize().y && sonic.check[z] && sonic.lives > 0)
            {
                if(z<=1)
                {
                    checks.play();
                }
                if (z > 2)
                {
                    checks.pause();
                }
                sonic.isdead = false;
                sonic.isground = true;
                //if(z<=1)
                //{
                    sonic.player.setPosition(check[z].getPosition().x, 730);
               // }
                    collector.setPosition(check[z].getPosition().x - 930, 50);
                    for (int i = 0, j = 63; i < 5; i++)
                    {
                        lives[i].setPosition(check[z].getPosition().x - 960 + (j * i), 1000);
                    }
                sonic.Velocity.x = 0;
                sonic.acc = 0;
                sonic.animleft = 0;
                sonic.animright = 0;
               
            }
        }

        if (sonic.PlayerColl.getPosition().y > window.getSize().y && sonic.lives <= 0 && !sonic.isground &&!sonic.check[2])
        {
            sonic.player.setPosition(250, 0);
            gameover1.setPosition(-900, 0);
            gameover = true;
            deathsound.pause();
            gameover5.play();
            sonic.isground = true;
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

        b = sonic.player.getPosition().x-120;
        levelcomplete.setPosition(sonic.player.getPosition().x - 972, 0);
        p = sonic.player.getPosition().x - 960;
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
        if (!sonic.check[2] && gameover)
        {
            window.draw(gameover1);
        }
        if (sonic.check[2])
        {
            window.draw(levelcomplete);
        }

        ///////////
        //DISPLAY//
        ///////////
        window.display();
        //DELTATIME
        deltatime = gameclock.getElapsedTime().asSeconds();
    }
}
void init_health_bar()
{
    healthbar[10].loadFromFile("Images/health_bar/health_bar100.png");
    healthbar[9].loadFromFile("Images/health_bar/health_bar90.png");
    healthbar[8].loadFromFile("Images/health_bar/health_bar80.png");
    healthbar[7].loadFromFile("Images/health_bar/health_bar70.png");
    healthbar[6].loadFromFile("Images/health_bar/health_bar60.png");
    healthbar[5].loadFromFile("Images/health_bar/health_bar50.png");
    healthbar[4].loadFromFile("Images/health_bar/health_bar40.png");
    healthbar[3].loadFromFile("Images/health_bar/health_bar30.png");
    healthbar[2].loadFromFile("Images/health_bar/health_bar20.png");
    healthbar[1].loadFromFile("Images/health_bar/health_bar10.png");
    healthbar[0].loadFromFile("Images/health_bar/health_bar00.png");
}
int update_health_bar(int bosshealth)
{
    if (bosshealth == 10000)
        return 10;
    else if (bosshealth == 9000)
        return 9;
    else if (bosshealth == 8000)
        return 8;
    else if (bosshealth == 7000)
        return 7;
    else if (bosshealth == 6000)
        return 6;
    else if (bosshealth == 5000)
        return 5;
    else if (bosshealth == 4000)
        return 4;
    else if (bosshealth == 3000)
        return 3;
    else if (bosshealth == 2000)
        return 2;
    else if (bosshealth == 1000)
        return 1;
    else if (bosshealth == 0)
        return 0;

    return -1;
}
void boss_level(RenderWindow& window,Clock&gameclock,float&deltatime,float&delay)
{
    //window.setFramerateLimit(120);
    // Variables
    float attackdelay = 0.1;
    int arr_index = 10;
    int x = 0;
    level = false;
    // Structs
    // 
    // Sonic
    player sonic;
    sonic.player.setTextureRect(IntRect(48.87, 0, 48.87, 43));
    boss eggman;
    eggman.sprite.setTextureRect(IntRect(0, 0, 73.3333, 73.3333));

    // Velocities
    Vector2f Velocity;
    Vector2f Velocity2;

    //Textures
    Texture bossbackgroundTexture;
    Texture bossgroundTexture;
    Texture liveTex;
    Texture overtex;

    //Sprites
    Sprite lives[5];
    Sprite gameover1;

    //health bar init
    Texture healthbartex;
    init_health_bar();

    RectangleShape eggman_healthBar(Vector2f(700.f, 70.f));
    eggman_healthBar.setScale(1.f, 1.f);
    eggman_healthBar.setOrigin(eggman_healthBar.getPosition().x / 2, eggman_healthBar.getPosition().y / 2);
    healthbartex = healthbar[arr_index];
    eggman_healthBar.setTexture(&healthbartex);
    eggman_healthBar.setPosition(40, 100);

    // Loading From Files
    sonic.sonicTex.loadFromFile("Images/sonic.png");
    bossbackgroundTexture.loadFromFile("Images/boss_level_background.jpg");
    bossgroundTexture.loadFromFile("Images/metal_ground.png");
    eggman.bosstex.loadFromFile("Images/eggman.png");
    liveTex.loadFromFile("Images/Lives.png");
    overtex.loadFromFile("Images/gameover.png");
    // Setting Textures
    sonic.player.setTexture(sonic.sonicTex);
    eggman.sprite.setTexture(eggman.bosstex);
    for (int i = 0; i < 5; i++)
    {
        lives[i].setTexture(liveTex);
    }
    gameover1.setTexture(overtex);


    // Position
    sonic.player.setPosition(500, 900);
    sonic.PlayerColl.setPosition(500, 720);
    eggman.sprite.setPosition(1500, 600);
    for (int i = 0, j = 63; i < 5; i++)
    {
        lives[i].setPosition(40 + (j * i), 1000);
    }
    gameover1.setPosition(Vector2f(40, 0));

    // Scaling and sizing
    sonic.player.setScale(Vector2f(2.f, 2.f));
    sonic.PlayerColl.setSize(Vector2f(45.f, 70.f));

    eggman.sprite.setScale(4, 4);
    eggman.coll.setSize(Vector2f(180, 180));
    for (int i = 0; i < 5; i++)
        lives[i].setScale(0.1, 0.1);


    // Background
    Sprite bossbackground;
    bossbackground.setScale(1.25, 1.05);
    bossbackground.setTexture(bossbackgroundTexture);
    bossbackground.setTextureRect(IntRect(0, 0, 1920, 960));
    bossbackground.setPosition(Vector2f(0, 0));
    bossbackgroundTexture.setRepeated(false);


    // Ground
    Sprite bossground;
    bossground.setTexture(bossgroundTexture);
    bossground.setTextureRect(IntRect(0, 0, 128 * 50, 128));
    bossground.setPosition(0, 952);
    bossgroundTexture.setRepeated(true);


    //borders
    RectangleShape border1(Vector2f(10, 1000));
    RectangleShape border2(Vector2f(10, 1000));

    border1.setPosition(0, 0);
    border2.setPosition(1890, 0);



    //========//
    // Sounds //
    //========//

    // sounds buffer
    SoundBuffer jumpsoundbuffer;
    SoundBuffer deathsoundbuffer;
    SoundBuffer oversoundBuffer;
    
    // sounds
    Sound jumpsound;
    Sound deathsound;
    Sound gameover5;
    //set buffer
    jumpsound.setBuffer(jumpsoundbuffer);
    deathsound.setBuffer(deathsoundbuffer);
    gameover5.setBuffer(oversoundBuffer);
    // load sounds
    jumpsoundbuffer.loadFromFile("Sounds/jump.WAV");
    deathsoundbuffer.loadFromFile("Sounds/death.WAV");
    oversoundBuffer.loadFromFile("Sounds/GameOver.flac");


    //Music bossmusic;
   // bossmusic.openFromFile("Sounds/boss.WAV");
    // view
    View cam(Vector2f(0.f, 0.f), Vector2f(1920.f, 1080.f));

    //game loop
    while (window.isOpen())
    {
        //veiw
        if(!level)
        {
            cam.setCenter(Vector2f(1000, 600));
            window.setView(cam);
        }


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
                    sonic.acc = 0;
                }
                if ((event.key.code == Keyboard::D))
                {
                    sonic.animright = 0;
                    sonic.Velocity.x = 0;
                    Velocity.x = 0;
                    Velocity2.x = 0;
                    sonic.acc = 0;
                }
                x = sonic.player.getPosition().x;
                if ((event.key.code == Keyboard::Escape) && !level)
                {
                    x = sonic.player.getPosition().x;
                    ispaused = true;
                    pausemenufunc(window, sonic.player, x, sonic.check[2]);
                    if (restart == true)
                    {
                        boss_level(window, gameclock, deltatime, delay);
                    }
                    if (pagenumber == 0)
                        return;
                }
            }
        }

        sonic.hitLeft = false;
        sonic.hitRight = false;


        //============//
        // velocities //
        //============//


        sonic.player.move(sonic.Velocity.x, sonic.Velocity.y);
        eggman.sprite.move(eggman.Velocity.x, eggman.Velocity.y);
        //collector.move(Velocity.x, Velocity.y);


        //=======//
        // sonic //
        //=======//
        

        //sonic physics& animation
        //===============================//

        if (!sonic.isdead)
        {
            if(!eggman.isdead)
            {
                for (int i = 0; i < 65; i++)
                {
                    //if sonic on the ground:
                    if (sonic.player.getGlobalBounds().intersects(bossground.getGlobalBounds()))
                    {
                        sonic.player.setPosition(sonic.player.getPosition().x, bossground.getPosition().y - 75);
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
                
                //jump animation 
                if (!sonic.isground && !sonic.isdead)
                {
                    animate_sprite(sonic.player, 16, 2 * 59.4, 48.87, 46, sonic.jumpanim, delay, deltatime, sonic.timer, 0);
                }

                //move left & right
                if (!sonic.slidingRight && !sonic.slidingLeft && !sonic.PlayerColl.getGlobalBounds().intersects(eggman.coll.getGlobalBounds()))
                {
                    if (Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::S))
                    {
                        move_left(sonic.player, sonic.Velocity, Velocity, Velocity2, sonic.acc, sonic.animleft, deltatime, sonic.timer, sonic.isground, sonic.isdead, sonic.check[2]);
                    }
                    else if (Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::S))
                    {
                        move_right(sonic.player, sonic.Velocity, Velocity, Velocity2, sonic.acc, sonic.animright, deltatime, sonic.timer, sonic.isground, sonic.isdead, sonic.check[2]);
                    }
                }
            }

            // idle animation
            if (sonic.isground && !(Keyboard::isKeyPressed(Keyboard::D)) && !(Keyboard::isKeyPressed(Keyboard::S)) && !(Keyboard::isKeyPressed(Keyboard::A)) && !(Keyboard::isKeyPressed(Keyboard::Space)))
            {
                animate_sprite(sonic.player, 8, 0, 48.87, 41, sonic.idleanim, delay, deltatime, sonic.timer, 0);
            }

            //attack
            if (Keyboard::isKeyPressed(Keyboard::S))
            {

                animate_sprite(sonic.player, 2, 2 * 59.4, 48.8, 46, sonic.attackanimInd, attackdelay, deltatime, sonic.timer, 0);

                if (Keyboard::isKeyPressed(Keyboard::A))
                {
                    sonic.Velocity.x = -20;
                }
                else if (Keyboard::isKeyPressed(Keyboard::D))
                {

                    sonic.Velocity.x = 20;
                }
            }


            //boss fight

            //gravity
            if (eggman.sprite.getGlobalBounds().intersects(bossground.getGlobalBounds()))
            {
                eggman.sprite.setPosition(eggman.sprite.getPosition().x, bossground.getPosition().y - 235);
                eggman.Velocity.y = 0.01;
                eggman.isground = true;
            }
            else
            {
                eggman.Velocity.y += 0.6;
            }

            if (!eggman.isdead)
            {
                //collision with sonic
                if (sonic.PlayerColl.getGlobalBounds().intersects(eggman.coll.getGlobalBounds()))
                {
                    if (sonic.PlayerColl.getPosition().y + 70 <= eggman.coll.getPosition().y + 75)
                    {
                        sonic.lives--;
                        sonic.isdead = true;
                        sonic.isground = false;
                        sonic.Velocity.y = -15;
                        deathsound.play();
                    }


                    else if (sonic.PlayerColl.getPosition().x + 45 >= eggman.coll.getPosition().x && sonic.PlayerColl.getPosition().x <= eggman.coll.getPosition().x + 90)
                    {
                        if (eggman.isground && Keyboard::isKeyPressed(Keyboard::S))
                        {
                            if (sonic.PlayerColl.getPosition().x + 45 > eggman.coll.getPosition().x - 200)
                            {
                                eggman.Velocity.y = -20;
                                eggman.isground = false;
                            }
                        }


                        sonic.hitLeft = true;
                        sonic.slidingLeft = true;
                        sonic.Velocity.x = -50;


                        if (Keyboard::isKeyPressed(Keyboard::S))
                        {
                            sonic.Velocity.x = -25;
                            eggman.health -= 50;
                        }
                        arr_index = update_health_bar(eggman.health);
                        if (arr_index != -1)
                        {
                            healthbartex = healthbar[arr_index];
                            eggman_healthBar.setTexture(&healthbartex);
                        }
                    }

                    else if (sonic.PlayerColl.getPosition().x >= eggman.coll.getPosition().x + 90)
                    {
                        if (eggman.isground && Keyboard::isKeyPressed(Keyboard::S))
                        {
                            if (sonic.PlayerColl.getPosition().x < eggman.coll.getPosition().x + 350)
                            {
                                eggman.Velocity.y = -20;
                                eggman.isground = false;
                            }
                        }


                        sonic.hitRight = true;
                        sonic.slidingRight = true;
                        sonic.Velocity.x = 50;


                        if (Keyboard::isKeyPressed(Keyboard::S))
                        {
                            sonic.Velocity.x = 25;
                            eggman.health -= 50;
                        }
                        arr_index = update_health_bar(eggman.health);
                        if (arr_index != -1)
                        {
                            healthbartex = healthbar[arr_index];
                            eggman_healthBar.setTexture(&healthbartex);
                        }
                    }
                }

                if (!sonic.PlayerColl.getGlobalBounds().intersects(eggman.coll.getGlobalBounds()))
                {
                    if (sonic.Velocity.x != 0)
                    {
                        if (sonic.Velocity.x < 0)
                        {
                            sonic.Velocity.x += 2;
                        }
                        if (sonic.Velocity.x > 0)
                        {
                            sonic.Velocity.x -= 2;
                        }
                    }
                    else
                    {
                        sonic.slidingLeft = false;
                        sonic.slidingRight = false;
                    }
                }


                //dodgign sonic
                if (Keyboard::isKeyPressed(Keyboard::S) && sonic.PlayerColl.getPosition().x + 45 <= eggman.coll.getPosition().x + 90 && eggman.health <= 1000)
                {
                    eggman.sprite.setOrigin(eggman.sprite.getLocalBounds().width, 0);
                    eggman.sprite.setScale(-4, 4);
                    eggman.Velocity.x = 10;
                    animate_sprite(eggman.sprite, 2, 0, 73.333, 63, eggman.bossanimInd, eggman.bossdelay, deltatime, eggman.bosstimer, 1);
                }
                else if (Keyboard::isKeyPressed(Keyboard::S) && sonic.PlayerColl.getPosition().x >= eggman.coll.getPosition().x + 90 && eggman.health <= 1000)
                {
                    eggman.sprite.setOrigin(0, 0);
                    eggman.sprite.setScale(4, 4);
                    eggman.Velocity.x = -10;
                    animate_sprite(eggman.sprite, 2, 0, 73.333, 63, eggman.bossanimInd, eggman.bossdelay, deltatime, eggman.bosstimer, 1);
                }


                //attacking sonic
                if (eggman.health > 1000 || !Keyboard::isKeyPressed(Keyboard::S))
                {
                    if (sonic.PlayerColl.getPosition().x + 45 <= eggman.coll.getPosition().x + 90)
                    {
                        eggman.sprite.setOrigin(0, 0);
                        eggman.sprite.setScale(4, 4);
                        eggman.Velocity.x = -6;
                        animate_sprite(eggman.sprite, 2, 0, 73.333, 63, eggman.bossanimInd, eggman.bossdelay, deltatime, eggman.bosstimer, 1);
                    }
                    else if (sonic.PlayerColl.getPosition().x >= eggman.coll.getPosition().x + 90)
                    {
                        eggman.sprite.setOrigin(eggman.sprite.getLocalBounds().width, 0);
                        eggman.sprite.setScale(-4, 4);
                        eggman.Velocity.x = 6;
                        animate_sprite(eggman.sprite, 2, 0, 73.333, 63, eggman.bossanimInd, eggman.bossdelay, deltatime, eggman.bosstimer, 1);
                    }
                }


                if (eggman.health <= 0)
                {
                    eggman.isdead = true;
                }
            }
        }

        //bossmusic.play();


        // sonic death
        if (sonic.isdead && !sonic.isground)
        {
            animate_sprite(sonic.player, 2, 4 * 55, 45.5, 50, sonic.deadanim, sonic.deathdelay, deltatime, sonic.timer, 6);
            sonic.Velocity.y += 0.5;
            sonic.Velocity.x = 0;
            Velocity.x = 0;
            Velocity2.x = 0;
        }

        // eggman death
        if (eggman.isdead && !sonic.isdead)
        {
            //bossmusic.stop();
            if (eggman.bosstimer < 0)
            {
                eggman.bossanimInd++;
                if (eggman.bossanimInd > 4)
                    eggman.bossanimInd = 4;
                eggman.sprite.setTextureRect(IntRect((eggman.bossanimInd * 70), 2 * 70, 70, 63));

                eggman.bosstimer = 1.25;
            }
            else
            {
                eggman.bosstimer -= deltatime;
            }
            eggman.Velocity.x = 0;
        }



        //===================//
        //       map         //
        //===================//


        //display movement
        //==============================//

        //respawn sonic & collector
        if (sonic.PlayerColl.getPosition().y > window.getSize().y &&sonic.lives>0)
        {
            sonic.isdead = false;
            sonic.isground = true;
            sonic.player.setPosition(500, 900);
            sonic.Velocity.x = 0;
            sonic.acc = 0;
            sonic.animleft = 0;
            sonic.animright = 0;
        }
        if (sonic.PlayerColl.getPosition().y > window.getSize().y && sonic.lives <= 0 && !sonic.isground)
        {
            gameover = true;
           // bossmusic.stop();
            deathsound.pause();
            gameover5.play();
            sonic.isground = true;
        }


        //borders of the level
        //==================================//
        if (sonic.player.getGlobalBounds().intersects(border1.getGlobalBounds()))
        {
            sonic.player.setPosition(sonic.player.getPosition().x + 20, sonic.player.getPosition().y);
        }
        if (sonic.player.getGlobalBounds().intersects(border2.getGlobalBounds()))
        {
            sonic.player.setPosition(sonic.player.getPosition().x - 20, sonic.player.getPosition().y);
        }
        if (eggman.sprite.getGlobalBounds().intersects(border1.getGlobalBounds()))
        {
            eggman.sprite.setPosition(eggman.sprite.getPosition().x + 20, eggman.sprite.getPosition().y);
        }
        if (eggman.sprite.getGlobalBounds().intersects(border2.getGlobalBounds()))
        {
            eggman.sprite.setPosition(eggman.sprite.getPosition().x - 20, eggman.sprite.getPosition().y);
        }


        //position of collision rectangles
        //====================================//
        sonic.PlayerColl.setPosition(sonic.player.getPosition().x - 70, sonic.player.getPosition().y + 10);
        eggman.coll.setPosition(eggman.sprite.getPosition().x + 70, eggman.sprite.getPosition().y + 70);



        //=======================================================================================================================================================//


        //Update
        window.clear();


        //Draw
        window.draw(bossbackground);
        window.draw(bossground);
        window.draw(eggman.sprite);
        for (int i = 0; i < sonic.lives; i++)
            window.draw(lives[i]);
        window.draw(sonic.player);
        window.draw(eggman_healthBar);
        if (gameover)
        {
            window.draw(gameover1);
        }

        //Display
        window.display();
        deltatime = gameclock.getElapsedTime().asSeconds();


        //=======================================================================================================================================================//
    }
}
