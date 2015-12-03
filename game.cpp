#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Graphics/Image.hpp>

int main()
{
    sf::RenderWindow window;
    window.create(sf::VideoMode (800,600), "Hello");
    window.setPosition(sf::Vector2i(100,100));
    window.setTitle("Changed Title");
    sf::Image background;
	if (!background.loadFromFile("airport.png"))
    std::cout<<1;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        
        window.display();
    }

    return 0;

}

	