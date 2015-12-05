#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <math.h>
#include <SFML/Graphics/Image.hpp>

#define SPEED 1

sf::Vector2i mouse_pos;
sf::RenderWindow window;

int no_of_planes=0;


long planeIndex=0;


class Plane
    {   
        public:
        sf::Texture planeTexture;
        sf::Sprite planeSprite;
        sf::Vertex line_points[10000];
        sf::Vertex line_vertices[2];
        sf::Vector2f last_direction;
        long line_ctr;
        bool planeReached;
        bool line_drawn;
        long ctr;
        int dist_travelled;


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
                planeReached=line_drawn=false;
                ctr=line_ctr=dist_travelled=0;
                last_direction.x=1;
                last_direction.y=0;
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
                planeReached=false;
                ctr=line_ctr=dist_travelled=0;
            }
        
        sf::Vector2f getPosition()
            {
                return planeSprite.getPosition();
            } 

        void refreshPointsList()
        {
            for(long i=0 ; i<line_ctr ; i++)
            {
                line_points[i]=sf::Vertex(sf::Vector2f(0,0));
            }
            line_vertices[0]=sf::Vertex(sf::Vector2f(0,0));
            line_vertices[1]=sf::Vertex(sf::Vector2f(0,0));
            line_ctr=0;
            line_drawn=false;
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
            {   sf::Vector2f point = vertex.position;
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


                    //std::cout<<distance<<"\n";
                    //planeSprite.setPosition(initial_point.x + SPEED*timer*cosine , initial_point.y + SPEED*timer*sine);
                    planeSprite.move(SPEED*cosine , SPEED*sine);
                    std::cout<<distance<<" "<<dist_travelled<<"\n";
                    if (distance!=20000)
                    dist_travelled+=SPEED;
                    if (dist_travelled>=distance)
                    dist_travelled=0;
                }
            }

        void moveInLine()
            {   
                if (line_drawn && ctr==0)
                    {   
                        sf::Vector2f direction;
                        direction.y = line_points[ctr].position.y- getPosition().y;
                        direction.x = line_points[ctr].position.x- getPosition().x;
                        float distance =sqrt(direction.x*direction.x + direction.y*direction.y);
                        moveInDirection (   getPosition() , direction , int (distance+dist_travelled)); //distance+dist_travelled because every time distance decreases by 1 unit and dist_travelled increases by 1 but sum remains constant (varying getPosition())
                        rotate(line_points[ctr]);
                        std::cout<<"1\n";
                        if (dist_travelled==0)
                            ctr++;

                    }
                else if (line_drawn && ctr<line_ctr-1)
                    {   
                        sf::Vector2f direction;
                        direction.y = line_points[ctr].position.y- line_points[ctr-1].position.y;
                        direction.x = line_points[ctr].position.x- line_points[ctr-1].position.x;
                        float distance =sqrt(direction.x*direction.x + direction.y*direction.y);
                        moveInDirection ( line_points[ctr].position , direction , int (distance));
                        rotate(line_points[ctr]);
                        std::cout<<"2\n";
                        //std::cout<<line_points[ctr].position.x<<" "<<line_points[ctr].position.y<<" "<<line_points[ctr-1].position.x<<" "<<line_points[ctr-1].position.y<<"\n";
                        if (dist_travelled==0)
                            ctr++;

                    }
               
                else if(line_drawn && ctr==line_ctr-1 && ctr!=0)
                    {
                        last_direction.y = line_points[ctr].position.y- line_points[ctr-1].position.y;
                        last_direction.x = line_points[ctr].position.x- line_points[ctr-1].position.x;
                        moveInDirection ( line_points[ctr].position , last_direction);
                        rotate(line_points[ctr]);
                        std::cout<<"3\n";
                        //std::cout<<line_points[ctr].position.x<<" "<<line_points[ctr].position.y<<" "<<line_points[ctr-1].position.x<<" "<<line_points[ctr-1].position.y<<" "<<ctr<<" "<<last_direction.x<<" "<<last_direction.y<<"\n";
                        refreshPointsList();
                        ctr++;
                        
                    }
                else
                    {   std::cout<<"4";
                        ctr=0;
                        moveInDirection ( getPosition() , last_direction);
                    }
            }


    };


Plane planes[1];


int distance (long index , sf::Vector2i mouse_pos , long clickedPlaneIndex)
{
    return fabs(mouse_pos.y - planes[clickedPlaneIndex].line_points[index].position.y) + fabs(mouse_pos.x - planes[clickedPlaneIndex].line_points[index].position.x);
}

void drawLine(long line_ctr , long clickedPlaneIndex)
{
    if(line_ctr>=1 && clickedPlaneIndex>=0)
    {
        for(long i=1 ; i<line_ctr ; i++)
        {
            planes[clickedPlaneIndex].line_vertices[0]=planes[clickedPlaneIndex].line_points[i-1];
            planes[clickedPlaneIndex].line_vertices[1]=planes[clickedPlaneIndex].line_points[i];
            window.draw(planes[clickedPlaneIndex].line_vertices , 2 , sf::Lines);
        }
    }
}

void drawLinesForAll()
{
    for(int i=0 ; i<no_of_planes ; i++)
    {   planes[i].moveInLine();
        if(!planes[i].planeReached)
            drawLine(planes[i].line_ctr , i);
    }
}

int whichPlaneClicked(sf::Vector2i mouse_pos)
{
    for(int i=0 ; i<= no_of_planes ; i++)
    {
        if(planes[i].isPlaneClicked(mouse_pos))                          
            return i;
    }
    return -1;
}

int main()
{

    sf::Vector2i initial_point;

    long line_ctr=0;
    int dist;
    bool planeClicked = false;
    bool click_started = false;

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

    int clickedPlaneIndex=-1;

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



        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            mouse_pos=sf::Mouse::getPosition(window);
            if(!click_started) {
                clickedPlaneIndex = whichPlaneClicked(mouse_pos);
                planes[clickedPlaneIndex].refreshPointsList();
            }

            click_started=true;

            if(clickedPlaneIndex>=0 && planes[clickedPlaneIndex].line_ctr==0) {

                planes[clickedPlaneIndex].line_points[line_ctr]=sf::Vertex(sf::Vector2f(mouse_pos.x , mouse_pos.y));
                planes[clickedPlaneIndex].line_ctr++; 
            }
            else if(clickedPlaneIndex>=0)
            {
                dist = distance(planes[clickedPlaneIndex].line_ctr-1,mouse_pos,clickedPlaneIndex);
                if(dist>50) {
                    planes[clickedPlaneIndex].line_points[planes[clickedPlaneIndex].line_ctr]=sf::Vertex(sf::Vector2f(mouse_pos.x , mouse_pos.y));
                    std::cout<<planes[clickedPlaneIndex].line_points[planes[clickedPlaneIndex].line_ctr].position.x<<" "<<planes[clickedPlaneIndex].line_points[planes[clickedPlaneIndex].line_ctr].position.y<<std::endl;
                    planes[clickedPlaneIndex].line_ctr++;
                }
            }

            

        }
        else
        {
            if (click_started==true)
                planes[clickedPlaneIndex].line_drawn=true;
            click_started=false;
        }


        window.draw(bg);
        window.draw(planes[0].planeSprite);

        drawLinesForAll();
        window.display();
    }

    return 0;

}
