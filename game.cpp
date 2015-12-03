#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Graphics/Image.hpp>


void isPlaneClicked ( sf::Sprite *spr, sf::RenderWindow *render , sf::Vector2i mouse_pos )
{
    if   ( mouse_pos.x > spr->getPosition().x
        && mouse_pos.x < spr->getPosition().x + 48
        && mouse_pos.y > spr->getPosition().y
        && mouse_pos.y < spr->getPosition().y + 48 )
    {
        std::cout<<"Click\n";
    }
    else
    {
        std::cout<<"NO\n";
    }
}


int main()
{
    sf::RenderWindow window;

    window.create(sf::VideoMode (774,359), "Game");
    window.setPosition(sf::Vector2i(100,100));
    window.setTitle("Changed Title");
    
   	sf::Texture texture;
	if (!texture.loadFromFile("airport.png"))
		{
    	std::cout<<"Texture not found";
    	}
	
	sf::Sprite bg;
	bg.setTexture(texture);


    sf::Vector2i mouse_pos;

    sf::Texture planeTexture;
    sf::Sprite planeSprite;
    if(!planeTexture.loadFromFile("airplane.png"))
    {
        std::cout<<"Error opening plane icon\n";
    }
    planeSprite.setTexture(planeTexture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed :
                    window.close();
                    break;
                case sf::Event::KeyPressed :
                    planeSprite.move(sf::Vector2f(5,5));
                    break;
                default :
                    break;
            }
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            mouse_pos=sf::Mouse::getPosition(window);
            isPlaneClicked(&planeSprite,&window,mouse_pos);
        }

        window.clear();

        window.draw(bg);

        window.draw(planeSprite);
        

        window.display();
    }

    return 0;

}

	
