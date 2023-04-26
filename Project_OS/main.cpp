#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

sf::Image g_bgImage;

enum CAR_STATE { NONE, STOPPED, LEFT, UP, RIGHT, DOWN, TURNING_LEFT, TURNING_RIGHT };

class Car
{
public:
    Car();
    ~Car();
    void Update(sf::Time deltaTime);
    void Render(sf::RenderWindow& window);
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f velocity;
private:
    uint16_t CheckAround();
    void TurnLeft();
    void TurnRight();
    sf::RectangleShape checkRect;
    bool isOnMarkup;
    sf::Vector2f size;
    sf::Vector2f pos;
    uint16_t dir;
    uint16_t turnState;
    float rotateAngle;
    float startAngle;
};

Car::Car()
{
    texture.loadFromFile("car.png");
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
    velocity = sf::Vector2f(1, 0);
    checkRect = sf::RectangleShape(sf::Vector2f(1, 5));
    checkRect.setFillColor(sf::Color::Red);
    checkRect.setOrigin(-33.5, 5.5);
    //checkRect.setOrigin(-10, 5.5);
    //sprite.setRotation(180);
    //checkRect.setRotation(90);
    isOnMarkup = false;
    size = sf::Vector2f(texture.getSize());
    dir = RIGHT;
    turnState = NONE;
    rotateAngle = 0.0f;
    startAngle = 0.0f;
}

Car::~Car()
{
}

void Car::Update(sf::Time deltaTime)
{

    if (turnState == TURNING_RIGHT)
    {
        if (rotateAngle < 90)
        {
            rotateAngle += 1;
            sprite.setRotation(startAngle + rotateAngle);
            checkRect.setRotation(startAngle + rotateAngle);

            sf::Vector2f offset;
            offset.x = deltaTime.asSeconds() * (80 * std::cos(rotateAngle * M_PI / 180));
            offset.y = deltaTime.asSeconds() * (80 * std::sin(rotateAngle * M_PI / 180));
            sprite.setPosition(sprite.getPosition() + offset);
            checkRect.setPosition(checkRect.getPosition() + offset);
        }
        else
        {
            turnState = NONE;
            dir = DOWN;
        }
    }
    else
    {
        checkRect.setPosition(sprite.getPosition().x, sprite.getPosition().y);
        for (int i = 0; i < 11; i++)
        {
            sf::FloatRect rect = checkRect.getGlobalBounds();
            sf::Color color = g_bgImage.getPixel(rect.left + i, rect.top);

            if (color == sf::Color(255, 255, 255))
            {
                if (dir == RIGHT)
                {
                    /*
                    float temp = velocity.x;
                    velocity.x = velocity.y;
                    velocity.y = temp;
                    dir = DOWN;
                    sprite.setRotation(sprite.getRotation() + 90);
                    checkRect.setRotation(checkRect.getRotation() + 90);
                    */
                    startAngle = checkRect.getRotation();
                    turnState = TURNING_RIGHT;
                }
                else if (dir == DOWN)
                {
                    float temp = velocity.x;
                    velocity.x = velocity.y;
                    velocity.y = temp;
                    velocity.x *= -1;
                    dir = LEFT;
                    sprite.setRotation(sprite.getRotation() + 90);
                    checkRect.setRotation(checkRect.getRotation() + 90);
                }
                std::cout << "Check x: " << checkRect.getPosition().x + i << " y: " << checkRect.getPosition().y << "\n";
                break;
            }
            else if (color == sf::Color(248, 221, 205))
            {
                if (dir == LEFT)
                {
                    float temp = velocity.x;
                    velocity.x = velocity.y;
                    velocity.y = temp;
                    velocity.y *= -1;
                    dir = DOWN;
                    sprite.setRotation(sprite.getRotation() - 90);
                    checkRect.setRotation(checkRect.getRotation() - 90);
                }
                else if (dir == DOWN)
                {
                    float temp = velocity.x;
                    velocity.x = velocity.y;
                    velocity.y = temp;
                    dir = RIGHT;
                    sprite.setRotation(sprite.getRotation() - 90);
                    checkRect.setRotation(checkRect.getRotation() - 90);
                }
                std::cout << "Check x: " << checkRect.getPosition().x + i << " y: " << checkRect.getPosition().y << "\n";
                break;
            }
        }
        pos = sprite.getPosition();
        pos += velocity;
        sprite.setPosition(pos);


        //checkRect.move(velocity);
    }
    //checkRect.setPosition(sprite.getPosition().x + 33, sprite.getPosition().y);
    
}

void Car::Render(sf::RenderWindow& window)
{
    window.draw(sprite);
    window.draw(checkRect);
}

uint16_t Car::CheckAround()
{
    return uint16_t();
}

void Car::TurnLeft()
{
}

void Car::TurnRight()
{
}


class Train
{
public:
    Train();
    ~Train();
    void Update();
    void Render(sf::RenderWindow& window);
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f velocity;
private:

};

Train::Train()
{
    texture.loadFromFile("train.png");
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}

Train::~Train()
{
}

void Train::Update()
{
}

void Train::Render(sf::RenderWindow& window)
{
}


int main()
{
    sf::Clock clock;
    sf::Time delta_time;
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Cars");
    
    window.setFramerateLimit(60);
    

    /* Bg */
    sf::Texture map_texture;
    if (!map_texture.loadFromFile("mapa_v2.png"))
        return -1;
    sf::Sprite map_sprite;
    map_sprite.setTexture(map_texture);

    g_bgImage = map_texture.copyToImage();
    

    /* Car */
    Car car;
    car.sprite.setPosition(600, 252);
    
    
    Car car1;
    car1.sprite.setPosition(400, 252);
    car1.velocity.x = 3;


    //std::cout << "w: " << r.getSize().x << " h: " << r.getSize().y << "\n";


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        delta_time = clock.restart();
        
        
        

        car.Update(delta_time);
        car1.Update(delta_time);

        window.clear();

        window.draw(map_sprite);
        
        car.Render(window);
        car1.Render(window);


        window.display();
    }

    return 0;
}