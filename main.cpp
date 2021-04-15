#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
using namespace std;

const int tx=20, ty=20;

class Player
{
public:
    int x,y;
    int hp;
    sf::Texture t[4];
    sf::Texture ded;
    sf::Sprite sprite;
    double vx,vy;

    void updatePos()
    {
        sprite.setPosition(x*32, y*32-16);
    }

    void updateTexture(int n)
    {
        sprite.setTexture(t[n]);
    }

    void dead()
    {
        sprite.setTexture(ded, true);
        sprite.move(-9,4);
    }

    Player() : x(5), y(5), vx(0), vy(0)
    {
        t[0].loadFromFile("images/player0.png");
        t[1].loadFromFile("images/player1.png");
        t[2].loadFromFile("images/player2.png");
        t[3].loadFromFile("images/player3.png");
        ded.loadFromFile("images/ded.png");
        sprite.setTexture(t[1]);
        updatePos();
    }

    void draw(sf::RenderWindow &app)
    {
        sprite.move(vx,vy);
        app.draw(sprite);
    }
};

class Portal
{
    public:
    int x,y;
    sf::Texture t;
    sf::Sprite sprite;

    void updatePos()
    {
        sprite.setPosition(x*32+16, y*32+16);
    }

    Portal() : x(6), y(6)
    {
        t.loadFromFile("images/portal.png");
        sprite.setTexture(t);
        sprite.setOrigin(16,16);
        updatePos();
    }

    void draw(sf::RenderWindow &app)
    {
        sprite.rotate(-2);
        app.draw(sprite);
    }
};

class Enemy
{
public:
    int x,y;
    int dir;
    sf::Sprite sprite;
    sf::Texture *t;
    double vx, vy;

    void updatePos()
    {
        sprite.setPosition(x*32, y*32-16);
    }

    void updateTexture()
    {
        sprite.setTexture(t[dir]);
    }

    Enemy(int X, int Y, sf::Texture tex[4]) : x(X), y(Y), vx(0), vy(0)
    {
        t = tex;
        dir=0;
        sprite.setTexture(t[0]);
        updatePos();
    }

    void draw(sf::RenderWindow &app)
    {
        sprite.move(vx, vy);
        app.draw(sprite);
    }
};

class Crate
{
public:
    int x,y;
    sf::Sprite sprite;
    double vx, vy;

    void updatePos()
    {
        sprite.setPosition(x*32, y*32);
    }

    Crate(int X, int Y, sf::Texture &t) : x(X), y(Y), vx(0), vy(0)
    {
        sprite.setTexture(t);
        updatePos();
    }

    void draw(sf::RenderWindow &app)
    {
        sprite.move(vx, vy);
        app.draw(sprite);
    }
};

class Button
{
public:
    int x,y;
    bool on;
    int dx, dy;
    sf::Sprite s_button, s_door;
    sf::Texture *t_off, *t_on, *t_d;
    int va, frame;

    void updatePos()
    {
        s_button.setPosition(x*32, y*32);
    }

    void updateState(bool b)
    {
       // if (on && b)
         //   va=1;
        //else if (!on && !b)
          //  va=-1;
        //else va=0;
        on = b;
        if(on)
            s_button.setTexture(*t_on);
        else
            s_button.setTexture(*t_off);
    }

    Button(int X, int Y, int DX, int DY, sf::Texture &b_off, sf::Texture &b_on, sf::Texture d[4]) : x(X), y(Y), on(false), dx(DX), dy(DY), va(0), frame(0)
    {
        t_off = &b_off;
        t_on = &b_on;
        s_button.setTexture(b_off);
        t_d = d;
        s_door.setTexture(d[1]);
        updatePos();
        s_door.setPosition(dx*32+16, dy*32+16);
    }

    void draw(sf::RenderWindow &app)
    {
        frame+=va;
        if(frame<0)
            frame=0;
        if (frame>4)
            frame=4;
        if (frame<4)
            s_door.setTexture(t_d[frame]);
        s_door.setOrigin(t_d[frame].getSize().x*0.5f, t_d[frame].getSize().y*0.5f);
        app.draw(s_button);
        if (frame<4)
            app.draw(s_door);
    }
};

class Tunnel
{
    public:
    int x,y;
    int dx, dy;
    sf::Sprite sprite;

    Tunnel(int X, int Y, int DX, int DY, sf::Texture &t) : x(X), y(Y), dx(DX), dy(DY)
    {
        sprite.setTexture(t);
        sprite.setPosition(x*32, y*32);
    }

    void draw(sf::RenderWindow &app)
    {
        app.draw(sprite);
    }
};

class Mapa
{
public:
    int tiles[tx][ty];
    vector<sf::Texture> t;
    sf::Sprite st[tx][ty];

    void updateTile(int x, int y)
    {
        switch(tiles[x][y])
        {
        case 0:
            st[x][y].setTexture(t[0]);
            break;
        case 1:
            st[x][y].setTexture(t[1]);
            break;
        case 2:
            st[x][y].setTexture(t[2]);
            break;
        default:
            cout << "wrong tile id";
            break;
        }
    }

    Mapa()
    {
        t.resize(3);
        t[0].loadFromFile("images/can.png");
        t[1].loadFromFile("images/cant.png");
        t[2].loadFromFile("images/wall.png");
        //load("maps/mapa1.txt");
        for (int i=0; i<tx; i++)
        for (int j=0; j<ty; j++)
        {
            //tiles[i][j]=rand()%3;
            //updateTile(i,j);
            st[i][j].setPosition(i*32, j*32);
        }
    }


    void draw(sf::RenderWindow &app)
    {
        for (int i=0; i<tx; i++)
        for (int j=0; j<ty; j++)
            app.draw(st[i][j]);
    }
};

class Game
{
public:
    Mapa mapa;
    Player player;
    Portal portal;
    vector<Enemy> enemies;
    vector<Crate> crates;
    vector<Button> buttons;
    vector<Tunnel> tunnels;
    sf::Texture t_en[4], t_crate, t_button_off, t_button_on, t_door[4], t_tunnel;
    sf::Vector2i dirs[4];
    int animation=0;
    bool isded=false;
    bool menu_on=true;
    bool won=false;

    int level;
    vector<string> sources;

    sf::Texture t_menu, t_win, t_ded;
    sf::Sprite menu, win, youded;

    void load(string src)
    {
        fstream f;
        f.open(src.c_str());
        for (int i=0; i<tx; i++)
        for (int j=0; j<ty; j++)
        {
            f >> mapa.tiles[i][j];
            mapa.updateTile(i, j);
        }
        int n;
        int c,d,c2,d2;

        f >> player.x >> player.y;
        player.updatePos();
        player.updateTexture(1);

        f >> portal.x >> portal.y;
        portal.updatePos();

        f >> n;
        for (int i=0; i<n; i++)
        {
            f >> c >> d;
            enemies.push_back(Enemy(c, d, t_en));
        }
        f >> n;
        for (int i=0; i<n; i++)
        {
            f >> c >> d;
            crates.push_back(Crate(c, d, t_crate));
        }
        f >> n;
        for (int i=0; i<n; i++)
        {
            f >> c >> d >> c2 >> d2;
            buttons.push_back(Button(c, d, c2, d2, t_button_off, t_button_on, t_door));
        }
        f >> n;
        for (int i=0; i<n; i++)
        {
            f >> c >> d >> c2 >> d2;
            tunnels.push_back(Tunnel(c, d, c2, d2, t_tunnel));
        }
        f.close();
    }

    void reload()
    {
        if(level>=sources.size())
            return;
        enemies.clear();
        crates.clear();
        buttons.clear();
        tunnels.clear();
        animation=0;
        isded=false;
        load(sources[level]);
    }

    Game()
    {
        t_en[0].loadFromFile("images/enemy0.png");
        t_en[1].loadFromFile("images/enemy1.png");
        t_en[2].loadFromFile("images/enemy2.png");
        t_en[3].loadFromFile("images/enemy3.png");
        t_crate.loadFromFile("images/crate.png");
        t_button_off.loadFromFile("images/button_off.png");
        t_button_on.loadFromFile("images/button_on.png");
        t_door[0].loadFromFile("images/door.png");
        t_door[1].loadFromFile("images/door1.png");
        t_door[2].loadFromFile("images/door2.png");
        t_door[3].loadFromFile("images/door3.png");
        t_tunnel.loadFromFile("images/tunnel.png");
        t_menu.loadFromFile("images/menu.png");
        t_win.loadFromFile("images/youwon.png");
        t_ded.loadFromFile("images/yd.png");
        menu.setTexture(t_menu);
        win.setTexture(t_win);
        youded.setTexture(t_ded);
        menu.setPosition(0,0);
        win.setPosition(0,0);

        dirs[0]=sf::Vector2i(-1,0);
        dirs[1]=sf::Vector2i(0,1);
        dirs[2]=sf::Vector2i(1,0);
        dirs[3]=sf::Vector2i(0,-1);
        //load("maps/f1.txt");
        level=10;
        sources.push_back("maps/tut.txt");
        //sources.push_back("maps/mapa1.txt");
        sources.push_back("maps/tut2.txt");
        sources.push_back("maps/f2.txt");
        sources.push_back("maps/f1.txt");
        sources.push_back("maps/f3.txt");
        sources.push_back("maps/f4.txt");
        reload();
    }

    bool isThereADoor(int x, int y) /// or crate
    {
        for (int i=0; i<buttons.size(); i++)
            if (!buttons[i].on && x == buttons[i].dx && y == buttons[i].dy)
                return true;
        for (int i=0; i<crates.size(); i++)
            if (x == crates[i].x && y == crates[i].y)
                return true;
        return false;
    }

    void animateButtons(int x, int y, bool stepon)
    {
        for (int i=0; i<buttons.size(); i++)
        {
            if (buttons[i].x == x && buttons[i].y == y)
                stepon ? buttons[i].va=1 : buttons[i].va=-1;
        }
    }

    void checkButtons()
    {
        for (int i=0; i<buttons.size(); i++)
        {
            //buttons[i].updateState(false);
            bool uf=true;
            if (player.x==buttons[i].x && player.y==buttons[i].y)
            {
                buttons[i].updateState(true);
                uf=false;
            }
            for (int j=0; j<crates.size(); j++)
            {
                if (buttons[i].x==crates[j].x && buttons[i].y==crates[j].y)
                {
                    buttons[i].updateState(true);
                    uf=false;
                }
            }
            for (int j=0; j<enemies.size(); j++)
            {
                if (buttons[i].x==enemies[j].x && buttons[i].y==enemies[j].y)
                {
                    buttons[i].updateState(true);
                    uf=false;
                }
            }
            if (uf)
                buttons[i].updateState(false);
        }
    }

    void updateEnemies()
    {
        for (int i=0; i<enemies.size(); i++)
        {
            if (mapa.tiles[enemies[i].x+dirs[enemies[i].dir].x][enemies[i].y+dirs[enemies[i].dir].y]==0
                && !isThereADoor(enemies[i].x+dirs[enemies[i].dir].x, enemies[i].y+dirs[enemies[i].dir].y))
            {
                animateButtons(enemies[i].x, enemies[i].y, false);
                enemies[i].x+=dirs[enemies[i].dir].x;
                enemies[i].y+=dirs[enemies[i].dir].y;
                //enemies[i].updatePos();
                enemies[i].vx = dirs[enemies[i].dir].x*8;
                enemies[i].vy = dirs[enemies[i].dir].y*8;
                //animation
                animateButtons(enemies[i].x, enemies[i].y, true);
            }
            else
            {
                if (mapa.tiles[enemies[i].x+dirs[(enemies[i].dir+1)%4].x][enemies[i].y+dirs[(enemies[i].dir+1)%4].y]==0
                    && !isThereADoor(enemies[i].x+dirs[(enemies[i].dir+1)%4].x, enemies[i].y+dirs[(enemies[i].dir+1)%4].y))
                    enemies[i].dir = (enemies[i].dir+1)%4;
                else if (mapa.tiles[enemies[i].x+dirs[(enemies[i].dir+3)%4].x][enemies[i].y+dirs[(enemies[i].dir+3)%4].y]==0
                    && !isThereADoor(enemies[i].x+dirs[(enemies[i].dir+3)%4].x, enemies[i].y+dirs[(enemies[i].dir+3)%4].y))
                    enemies[i].dir = (enemies[i].dir+3)%4;
                else
                    enemies[i].dir = (enemies[i].dir+2)%4;
            }

            enemies[i].updateTexture();

            if (enemies[i].x == player.x && enemies[i].y == player.y)
            {
                dead();
                return;
            }
        }
    }

    void update()
    {
        updateEnemies();
        checkButtons();
    }

    void dead()
    {
        isded=true;
        player.dead();
        //reload();
        youded.setPosition(player.sprite.getPosition().x-8, player.sprite.getPosition().y-20);
    }

    void go(int a, int b)
    {
        if (isded)
            return;
        if (animation>0)
            return;
        animate();
        if (a==-1)
            player.updateTexture(0);
        if (b==1)
            player.updateTexture(1);
        if (a==1)
            player.updateTexture(2);
        if (b==-1)
            player.updateTexture(3);

        if (mapa.tiles[player.x+a][player.y+b]==0)
        {
            for (int i=0; i<crates.size(); i++)
            {
                if (crates[i].x==player.x+a && crates[i].y==player.y+b)
                {
                    if (mapa.tiles[crates[i].x+a][crates[i].y+b]==0)
                    {
                        for (int j=0; j<crates.size(); j++)
                            if (crates[j].x==crates[i].x+a && crates[j].y==crates[i].y+b)
                                return;
                        animateButtons(crates[i].x, crates[i].y, false);
                        crates[i].x+=a;
                        crates[i].y+=b;
                        animateButtons(crates[i].x, crates[i].y, true);
                        //crates[i].updatePos();
                        crates[i].vx=a*8;
                        crates[i].vy=b*8;
                    }
                    else
                        return;
                }
            }
            for (int i=0; i<enemies.size(); i++)
            {
                if (player.x+a==enemies[i].x && player.y+b==enemies[i].y)
                {
                    dead();
                    return;
                }
            }
            for (int i=0; i<buttons.size(); i++)
            {
                if (!buttons[i].on && player.x+a==buttons[i].dx && player.y+b==buttons[i].dy)
                    return;
            }

            mapa.tiles[player.x][player.y]=1;
            mapa.updateTile(player.x, player.y);

            animateButtons(player.x, player.y, false);
            player.x+=a;
            player.y+=b;
            animateButtons(player.x, player.y, true);
            //player.updatePos();
            player.vx=a*8;
            player.vy=b*8;
            animation = 4;


            update();

            for (int i=0; i<buttons.size(); i++)
            {
                if (!buttons[i].on && player.x==buttons[i].dx && player.y==buttons[i].dy)
                {
                    dead();
                    return;
                }
            }

            for (int i=0; i<tunnels.size(); i++)
            {
                if (player.x==tunnels[i].x && player.y==tunnels[i].y)
                {
                    player.x = tunnels[i].dx;
                    player.y = tunnels[i].dy;
                    //player.updatePos();
                }
            }

            if (player.x==portal.x && player.y==portal.y)
            {
                //animation=0;
                //level++;
                won=true;
                //if (level<sources.size())
                  //  reload();
            }
        }
    }

    void animate()
    {
        if (animation==0)
        {
            if (won)
            {
                level++;
                if (level<sources.size())
                    reload();
                won=false;
            }
            player.vx=0;
            player.vy=0;
            if(!isded)
                player.updatePos();
            for (int i=0; i<enemies.size(); i++)
            {
                enemies[i].vx=0;
                enemies[i].vy=0;
                enemies[i].updatePos();
            }
            for (int i=0; i<crates.size(); i++)
            {
                crates[i].vx=0;
                crates[i].vy=0;
                crates[i].updatePos();
            }
        }
        else
        {
            animation--;
        }
    }

    void draw(sf::RenderWindow &app)
    {
        if (level<sources.size())
        {
            animate();

            mapa.draw(app);
            portal.draw(app);
            for (int i=0; i<buttons.size(); i++)
                buttons[i].draw(app);
            for (int i=0; i<tunnels.size(); i++)
                tunnels[i].draw(app);
            for (int i=0; i<crates.size(); i++)
                    crates[i].draw(app);

            for (int w=0; w<=ty; w++)
            {
                if (player.y==w)
                    player.draw(app);
                for (int i=0; i<enemies.size(); i++)
                    if (enemies[i].y==w)
                        enemies[i].draw(app);
            }

            if (isded)
            {
                youded.setPosition(player.sprite.getPosition().x-8, player.sprite.getPosition().y-20);
                app.draw(youded);
            }
        }
        else if (menu_on)
            app.draw(menu);
        else
            app.draw(win);
    }
};

int main()
{
    Game game;

    sf::Music music;
    music.openFromFile("music/2.0.wav");
    music.play();
    music.setLoop(true);

    sf::RenderWindow app(sf::VideoMode(640, 640), "Pathcloser"/*, sf::Style::None*/);
    app.setFramerateLimit(30);
    while (app.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            game.go(0,-1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            game.go(0,1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            game.go(-1,0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            game.go(1,0);

        sf::Event event;
        while (app.pollEvent(event))
        {
            if (event.type == sf::Event::Closed/* || event.type == sf::Event::MouseButtonPressed*/)
                app.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    if (game.menu_on)
                    {
                        game.level=0;
                        game.menu_on=false;
                    }
                    if (game.level>=game.sources.size())
                        game.menu_on=true;

                    game.reload();
                }

                if (event.key.code == sf::Keyboard::M)
                {
                    if(music.getStatus()!=sf::Music::Paused)
                        music.pause();
                    else
                        music.play();
                }
            }
        }

        app.clear();

        game.draw(app);

        app.display();
    }

    return EXIT_SUCCESS;
}