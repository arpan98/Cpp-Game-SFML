#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Graphics/Image.hpp>




int main()
{
    sf::RenderWindow window;
    window.create(sf::VideoMode (774,359), "Hello");
    window.setPosition(sf::Vector2i(100,100));
    window.setTitle("Changed Title");
    
   	sf::Texture texture;
	if (!texture.loadFromFile("airport.png"))
		{
    	std::cout<<"Texture not found";
    	}
	
	sf::Sprite bg;
	bg.setTexture(texture);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(bg);
        window.display();
    }

    return 0;

}

	