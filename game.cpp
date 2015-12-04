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
    {   public:
        sf::Texture planeTexture;
        sf::Sprite planeSprite;
        void setImage (std::String filename) 
            {
                if(!planeTexture.loadFromFile(filename))
                    {
                        std::cout<<"Error opening plane icon\n";
                    }
                planeSprite.setTexture(planeTexture);
                planeSprite.setOrigin(24,24);

            }

        sf::Vector2f getPosition()
            {
                return planeSprite.getPosition();
            } 



    };




bool isPlaneClicked ( sf::Sprite *spr, sf::RenderWindow *render , sf::Vector2i mouse_pos )
{
    if   ( mouse_pos.x > spr->getPosition().x - 24
        && mouse_pos.x < spr->getPosition().x + 24
        && mouse_pos.y > spr->getPosition().y - 24
        && mouse_pos.y < spr->getPosition().y + 24 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

int distance (long index , sf::Vector2i mouse_pos)
{
    return fabs(mouse_pos.y- mouse_coords[index].y) + fabs(mouse_pos.x - mouse_coords[index].x);
}

void drawLine(long ctr , sf::RenderWindow *window)
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
}

void rotate (sf::Sprite *planeSprite, sf::Vertex vertex)
{   sf::Vector2f point = vertex.position;
    sf::Vector2f plane_pos;
    double rise, run, angle;
    
    plane_pos=planeSprite->getPosition();

    //std::cout<<"Mouse: "<<mouse_pos.x<<" "<<mouse_pos.y<<std::endl;
    //std::cout<<"Plane: "<<plane_pos.x<<" "<<plane_pos.y<<std::endl;
    rise= point.y - (plane_pos.y);
    run = point.x - (plane_pos.x);
    angle = 57.296*atan(double(rise/run));
    //std::cout<<"Rise: "<<rise<<"Run: "<<run<<"Angle: "<<angle<<std::endl;
    if (run<0)
        planeSprite->setRotation(135+angle+180);
    else
        planeSprite->setRotation(135+angle);

}

void movePlane(sf::Sprite *planeSprite , sf::RenderWindow *window , long ctr)
{
    if(planeIndex<=ctr && ctr>=1)
    {
        sf::Vector2f point = line_points[planeIndex].position;
        planeSprite->setPosition(point);
        planeIndex++;
    }
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
    

    sf::Texture texture;
    if (!texture.loadFromFile("airport.png"))
        {
        std::cout<<"Texture not found";
        }
    
    sf::Sprite bg;
    bg.setTexture(texture);


    
    sf::Vector2f plane_pos;


    sf::Texture planeTexture;
    sf::Sprite planeSprite;
    if(!planeTexture.loadFromFile("airplane.png"))
    {
        std::cout<<"Error opening plane icon\n";
    }
    planeSprite.setTexture(planeTexture);
    planeSprite.setOrigin(24,24);
    planeSprite.setPosition(100,100);


 
    while (window.isOpen())
    {   
        mouse_pos=sf::Mouse::getPosition(window);
        
        rotate(&planeSprite , sf::Vertex(sf::Vector2f(100,0)));


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

        if(sf::Mouse::isButtonReleased)
        {
            planeClicked=false;
            click_start=false;
        }


        window.draw(bg);

        movePlane(&planeSprite , &window , ctr-1);
        window.draw(planeSprite);
        drawLine(ctr-1 , &window);
        window.display();
    }

    return 0;

}
