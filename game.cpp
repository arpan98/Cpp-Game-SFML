#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <math.h>
#include <SFML/Graphics/Image.hpp>

sf::Vector2i mouse_pos;

std::vector<sf::Vector2i> mouse_coords(500);

sf::Vertex line_points[10000];
sf::Vertex line_vertices[2];

long planeIndex=0;
class Plane
	{	public:
		sf::Texture planeTexture;
    	sf::Sprite planeSprite;
    	
    	Plane () 
    		{
    		    if(!planeTexture.loadFromFile("airplane.png"))
    				{
        				std::cout<<"Error opening plane icon\n";
    				}
    			planeSprite.setTexture(planeTexture);
    		    planeSprite.setOrigin(24,24);
    		    planeSprite.setPosition(100,100);

    		}

    	Plane (char* filename) 
    		{
    		    if(!planeTexture.loadFromFile(filename))
    				{
        				std::cout<<"Error opening plane icon\n";
    				}
    			planeSprite.setTexture(planeTexture);
    		    planeSprite.setOrigin(24,24);
    		    planeSprite.setPosition(100,100);

    		}
		
    	sf::Vector2f getPosition()
    		{
    			return planeSprite.getPosition();
    		} 

    	void rotate (sf::Vertex vertex)
			{	sf::Vector2f point = vertex.position;
			    sf::Vector2f plane_pos;
			    double rise, run, angle;
				
				plane_pos=planeSprite.getPosition();

			    //std::cout<<"Mouse: "<<mouse_pos.x<<" "<<mouse_pos.y<<std::endl;
			    //std::cout<<"Plane: "<<plane_pos.x<<" "<<plane_pos.y<<std::endl;
			    rise= point.y - (plane_pos.y);
			    run = point.x - (plane_pos.x);
			    angle = 57.296*atan(double(rise/run));
			    //std::cout<<"Rise: "<<rise<<"Run: "<<run<<"Angle: "<<angle<<std::endl;
			    if (run<0)
			      	planeSprite.setRotation(135+angle+180);
			    else
			    	planeSprite.setRotation(135+angle);

			}


	};

Plane p("airplane.png");



int distance (long index , sf::Vector2i mouse_pos)
{
    return fabs(mouse_pos.y- mouse_coords[index].y) + fabs(mouse_pos.x - mouse_coords[index].x);
}





int main()
{
    sf::RenderWindow window;
    sf::Vector2i initial_point;

    long ctr=0;
    int dist;
    bool planeClicked = false;
    bool click_start = false;

    window.create(sf::VideoMode (774,359), "Game");
    window.setPosition(sf::Vector2i(100,100));
    window.setTitle("Changed Title");
    
					    //Setting background
					   	sf::Texture texture;
						if (!texture.loadFromFile("airport.png"))
							{
					    	std::cout<<"Texture not found";
					    	}
						
						sf::Sprite bg;
						bg.setTexture(texture);

      
    sf::Vector2f plane_pos;

 
    while (window.isOpen())
    {	
        mouse_pos=sf::Mouse::getPosition(window);
    	
    	p.rotate(sf::Vertex(sf::Vector2f(100,0)));


        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed :
                    window.close();
                    break;
                case sf::Event::KeyPressed :
                    p.planeSprite.move(sf::Vector2f(5,5));
                    break;
                default :
                    break;
            }

        }

        sf::Vector2f point = p.getPosition();
 		std::cout<<point.x<<" "<<point.y<<std::endl;    	    
        window.draw(bg);
        window.draw(p.planeSprite);
        window.display();
    }

    return 0;

}

	
