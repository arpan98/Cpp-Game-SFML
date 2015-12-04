#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <math.h>
#include <SFML/Graphics/Image.hpp>

#define SPEED 0.05

sf::Vector2i mouse_pos;
sf::Clock clk;
std::vector<sf::Vector2i> mouse_coords(500);

int no_of_planes=0;
    int dist_travelled=0;




long planeIndex=0;
class Plane
    {   
        public:
        sf::Texture planeTexture;
        sf::Sprite planeSprite;
        sf::Vertex line_points[10000];
        sf::Vertex line_vertices[2];

    	Plane () 
    		{
    		    if(!planeTexture.loadFromFile("airplane.png"))
    				{
        				std::cout<<"Error opening plane icon\n";
    				}
    			planeSprite.setTexture(planeTexture);
    		    planeSprite.setOrigin(24,24);
    		    planeSprite.setPosition(100,100);
                no_of_planes++;

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
                no_of_planes++;
    		}
		
    	sf::Vector2f getPosition()
    		{
    			return planeSprite.getPosition();
    		} 

        bool isPlaneClicked ( sf::Vector2i mouse_pos )
            {
                if   ( mouse_pos.x > planeSprite.getPosition().x - 24
                      && mouse_pos.x < planeSprite.getPosition().x + 24
                      && mouse_pos.y > planeSprite.getPosition().y - 24
                      && mouse_pos.y < planeSprite.getPosition().y + 24 )
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
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

        void moveInDirection(sf::Vector2f initial_point , sf::Vector2f direction , int distance = 20000)
        {
            if(dist_travelled < distance) {
                //std::cout<<position.x<<" "<<position.y<<"\n";
                float hyp = sqrt(direction.x*direction.x + direction.y*direction.y);
                float sine = (direction.y)/hyp;
                float cosine = (direction.x)/hyp;

                sf::Time tmr = clk.getElapsedTime();
                float timer = tmr.asMilliseconds();
                std::cout<<dist_travelled<<"\n";
                planeSprite.setPosition(initial_point.x + SPEED*timer*cosine , initial_point.y + SPEED*timer*sine);
                dist_travelled=SPEED*timer;        
            }
        }

	};


Plane planes[5];




int distance (long index , sf::Vector2i mouse_pos)
{
    return fabs(mouse_pos.y- mouse_coords[index].y) + fabs(mouse_pos.x - mouse_coords[index].x);
}

/*void drawLine(long ctr , sf::RenderWindow *window)
{
    if(ctr>=1)
    {
        for(long i=1 ; i<ctr ; i++)
        {
            line_vertices[0]=line_points[i-1];
            line_vertices[1]=line_points[i];
            window->draw(line_vertices , 2 , sf::Lines);
        }
    }
}*/

int whichPlaneClicked(sf::Vector2i mouse_pos)
{
    for(int i=0 ; i<= no_of_planes ; i++)
    {
        if(planes[i].isPlaneClicked(mouse_pos))                               //TODO : plane[i]
            return i;
    }
    return -1;
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
            	
    	//planes[0].rotate(sf::Vertex(sf::Vector2f(100,0)));


        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed :
                    window.close();
                    break;
                case sf::Event::KeyPressed :
                    window.close();
                    break;
                default :
                    break;
            }

        }



        /*if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {

            mouse_pos=sf::Mouse::getPosition(window);
            int clickedPlaneIndex = whichPlaneClicked();
            if(click_start==false && planeClicked==false)
                planeClicked=isPlaneClicked(&planeSprite,&window,mouse_pos);

            click_start=true;

            if(planeClicked && ctr==0) {
                mouse_coords[ctr].x=mouse_pos.x;
                mouse_coords[ctr].y=mouse_pos.y;
                line_points[ctr]=sf::Vertex(sf::Vector2f(mouse_pos.x , mouse_pos.y));
                ctr++; 
            }
            else if(planeClicked)
            {
                //std::cout<<mouse_pos.x<<" "<<mouse_pos.y<<'\n';
                dist = distance(ctr-1,mouse_pos);
                if(dist>20) {
                    //mouse_coords.push_back(std::make_pair(mouse_pos.x , mouse_pos.y));
                    mouse_coords[ctr].x=mouse_pos.x;
                    mouse_coords[ctr].y=mouse_pos.y;

                    line_points[ctr]=sf::Vertex(sf::Vector2f(mouse_pos.x , mouse_pos.y));
                    
                    ctr++;
                }
            }

            

        }
        else
        {
            click_start=false;
            planeIndex=false;
        }*/


        window.draw(bg);

        planes[0].moveInDirection(sf::Vector2f(100,100) , sf::Vector2f(100,0));
        window.draw(planes[0].planeSprite);
        
        window.display();
    }

    return 0;

}
