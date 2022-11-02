#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <SFML/Audio.hpp>

#define ZOMCNT 5

using namespace sf;

RenderWindow app(VideoMode(1900,504),"Game");

bool gv = false;
bool ready = false;

Vector2f charPos = {100,350};
Vector2f zomPos[ZOMCNT];
float zomSpd[ZOMCNT];
bool zomhide[ZOMCNT];
bool flip[6];
int cnt = 0;
Vector2f bulletPos = {0,0};
bool shot = false;
float shootDir = 0;

float dy = 0;

int main()
{
    app.setFramerateLimit(60);
    Music bg;
    if (!bg.openFromFile("background.wav"))
    {
        return -1;
    }
    bg.setLoop(true);
    bg.play();
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("shot.wav"))
        return -1;
    Sound sound;
    sound.setBuffer(buffer);
    sound.setVolume(100);
    bg.play();

    Font font;
    font.loadFromFile("arial.ttf");
    Text gamev, win, score, readytxt;
    gamev.setFont(font);
    gamev.setString("Game Over");
    gamev.setCharacterSize(52);
    gamev.setPosition(1900/2 - gamev.getGlobalBounds().width/2, 504/2 - gamev.getGlobalBounds().height/2);
    gamev.setFillColor(Color::Red);
    win.setFont(font);
    win.setString("You Win");
    win.setCharacterSize(52);
    win.setPosition(1900/2 - win.getGlobalBounds().width/2, 504/2 - win.getGlobalBounds().height/2);
    score.setFont(font);
    score.setString("0");
    score.setCharacterSize(52);
    score.setPosition(1900/2 - win.getGlobalBounds().width/2, 50);
    readytxt.setFont(font);
    readytxt.setString("Press Enter to Start!");
    readytxt.setCharacterSize(52);
    readytxt.setPosition(1900/2 - readytxt.getGlobalBounds().width/2, 504/2 - readytxt.getGlobalBounds().height/2);


    Texture characterT, backgroundT, zombieT, axeT;
    if (!characterT.loadFromFile("char.png"))
    {
        return -1;
    }
    if (!backgroundT.loadFromFile("background2.jpg"))
    {
        return -1;
    }
    if (!zombieT.loadFromFile("zombie.png"))
    {
        return -1;
    }
    if (!axeT.loadFromFile("axe.png"))
    {
        return -1;
    }

    Sprite chara(characterT), background(backgroundT), zombie[ZOMCNT], axe(axeT);
    for (int i = 0; i<ZOMCNT; i++)
    {
        zombie[i].setTexture(zombieT);
        zombie[i].scale({0.3,0.3});
        zomPos[i].x = rand() % (1900-900) + 900;
        zomPos[i].y = 350;
        zomSpd[i] = rand() % 3 + 1;
        zombie[i].setOrigin(51,53);
        zomhide[i] = 0;
    }
    chara.scale({0.3,0.3});
    axe.scale({0.15,0.15});
    axe.setOrigin({51,51});
    background.scale({0.5,0.5});


    while (app.isOpen())
    {
        Event event;
        while(app.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                app.close();
            }
        }
        if(!ready)
        {
            app.clear();
            if (Keyboard::isKeyPressed(Keyboard::Enter))
                ready = true;
            app.draw(readytxt);
        }
        else if (cnt == 5)
        {
            if (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                for (int i = 0; i<ZOMCNT; i++)
                {
                    zomPos[i].x = rand() % (1900-900) + 900;
                    zomPos[i].y = 350;
                    zomSpd[i] = rand() % 3 + 1;
                    zomhide[i] = 0;
                }
                charPos = {100,350};
                gv = false;
                cnt = 0;
            }
            app.clear();
            app.draw(win);
        }
        else if (!gv)
        {
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                charPos.x+=10;
                flip[5] = 1;
            }
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                charPos.x-=10;
                flip[5] = 0;

            }

            if (Keyboard::isKeyPressed(Keyboard::Space))
            {
                if (!shot)
                {
                    sound.play();
                    shot = true;
                    bulletPos = charPos;
                    bulletPos.y +=25;
                    if (flip[5] == 1)
                        shootDir = 1;
                    else
                        shootDir = -1;
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Up)) if (dy==0) dy = -15;

            if (dy != 0) dy += 0.4;
            charPos.y+=dy;
            if (charPos.y > 349) dy = 0;

            chara.setPosition(charPos);
            app.clear();
            app.draw(background);
            for (int i = 0; i<ZOMCNT; i++)
            {
                int d = charPos.x - zomPos[i].x;
                if (d < 0)
                {
                    zomPos[i].x -= zomSpd[i];
                    if (!flip[i])
                    {
                        zombie[i].scale(-1,1);
                        flip[i] = !flip[i];
                    }
                }
                else
                {
                    zomPos[i].x += zomSpd[i];
                    if (flip[i])
                    {
                        zombie[i].scale(-1,1);
                        flip[i] = !flip[i];
                    }
                }
                zombie[i].setPosition(zomPos[i]);
                if (!zomhide[i]) app.draw(zombie[i]);
                if (chara.getGlobalBounds().intersects(zombie[i].getGlobalBounds()) && zomhide[i] == 0)
                {
                    gv = true;
                }
            }
            if (shot)
            {
                axe.rotate(1);
                bulletPos.x += shootDir * 15;
                axe.setPosition(bulletPos);
                app.draw(axe);
                for (int i = 0; i<ZOMCNT; i++)
                    if (axe.getGlobalBounds().intersects(zombie[i].getGlobalBounds()) && zomhide[i] == 0)
                    {
                        zomhide[i] = 1;
                        shot = false;
                        cnt++;
                    }
                if (bulletPos.x > 1900 || bulletPos.x < 0) shot = false;

            }
            score.setString(std::to_string(cnt));
            app.draw(chara);
            app.draw(score);
        }
        else
        {
            if (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                for (int i = 0; i<ZOMCNT; i++)
                {
                    zomPos[i].x = rand() % (1900-900) + 900;
                    zomPos[i].y = 350;
                    zomSpd[i] = rand() % 3 + 1;
                    zomhide[i] = 0;
                }
                charPos = {100,350};
                gv = false;
                cnt = 0;
            }
            app.clear();
            app.draw(gamev);
        }
        app.display();
    }
    return 0;
}
