#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>

#define ZOMCNT 5

using namespace sf;

RenderWindow app(VideoMode(1900,504),"Game");

bool gv = false;

Vector2f charPos = {100,350};
Vector2f zomPos[5];
float zomSpd[5];
bool zomhide[5];
bool flip[6];
Vector2i obst[1];
int cnt = 0;
Vector2f bulletPos = {0,0};
bool shot = false;
float shootDir = 0;

float dy = 0;

int main() {
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
    Font font;
    font.loadFromFile("arial.ttf");
    Text gamev, win;
    gamev.setFont(font);
    gamev.setString("Game Over");
    gamev.setCharacterSize(52);
    gamev.setPosition(1900/2 - gamev.getGlobalBounds().width/2, 504/2 - gamev.getGlobalBounds().height/2);
    gamev.setFillColor(Color::Red);
    win.setFont(font);
    win.setString("You Win");
    win.setCharacterSize(52);
    win.setPosition(1900/2 - win.getGlobalBounds().width/2, 504/2 - win.getGlobalBounds().height/2);


    Texture characterT, backgroundT, zombieT, axeT;
    if (!characterT.loadFromFile("char.png")){
        return -1;
    }
    if (!backgroundT.loadFromFile("background2.jpg")){
        return -1;
    }
    if (!zombieT.loadFromFile("zombie.png")){
        return -1;
    }
    if (!axeT.loadFromFile("axe.png")){
        return -1;
    }

    Sprite chara(characterT), background(backgroundT), zombie[ZOMCNT], axe(axeT);
    for (int i = 0;i<ZOMCNT;i++){
        zombie[i].setTexture(zombieT);
        zombie[i].scale({0.3,0.3});
        zomPos[i].x = rand() % (1900-900) + 900;
        zomPos[i].y = 350;
        zomSpd[i] = rand() % 3 + 1;
        zombie[i].setOrigin(51,53);
    }
    chara.scale({0.3,0.3});
    axe.scale({0.15,0.15});
    axe.setOrigin({51,51});
    background.scale({0.5,0.5});


    while (app.isOpen()) {
        Event event;
        while(app.pollEvent(event)) {
            if (event.type == Event::Closed){
                app.close();
            }
        }

        if (!gv) {
                if (Keyboard::isKeyPressed(Keyboard::Right)) {
                charPos.x+=5;
                flip[5] = 1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            charPos.x-=5;
            flip[5] = 0;

        }

        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            sound.play();
            shot = true;
            bulletPos = charPos;
            if (flip[5] == 1)
                shootDir = 1;
            else
                shootDir = -1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) if (dy==0) dy = -13;

        if (dy != 0) dy += 0.4;
        charPos.y+=dy;
        if (charPos.y > 349) dy = 0;

        chara.setPosition(charPos);
        app.clear();
        app.draw(background);
        for (int i = 0;i<ZOMCNT;i++){
            int d = charPos.x - zomPos[i].x;
            if (d < 0) {
                zomPos[i].x -= zomSpd[i];
                if (!flip[i]){
                    zombie[i].scale(-1,1);
                    flip[i] = !flip[i];
                }
            }
            else {
                zomPos[i].x += zomSpd[i];
                if (flip[i]){
                    zombie[i].scale(-1,1);
                    flip[i] = !flip[i];
                }
            }
            zombie[i].setPosition(zomPos[i]);
            if (!zomhide[i]) app.draw(zombie[i]);
            if (chara.getGlobalBounds().intersects(zombie[i].getGlobalBounds()) && zomhide[i] == 0) {
                gv = true;
            }
        }
        if (shot) {
            axe.rotate(1);
            bulletPos.x += shootDir * 10;
            axe.setPosition(bulletPos);
            app.draw(axe);
            for (int i = 0;i<ZOMCNT;i++)
                if (axe.getGlobalBounds().intersects(zombie[i].getGlobalBounds()) && zomhide[i] == 0) {
                    zomhide[i] = 1;
                    shot = false;
                    cnt--;
                if (zomhide[i] == 1) cnt++;
            }
        }
        app.draw(chara);
        }
        else if (cnt == 5){
            app.clear();
            app.draw(win);
        }
        else {
            app.clear();
            app.draw(gamev);
        }
        app.display();
        cnt = 0;
    }
    return 0;
}
