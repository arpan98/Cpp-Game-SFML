#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <math.h>
#include <SFML/Graphics/Image.hpp>
#include <time.h>
#include <string>
#include <stdlib.h>


#define SPEED 1
#define line_resolution 10

sf::Vector2i mouse_pos;
sf::RenderWindow window;
sf::Vector2f landingZone1(249,138);
sf::Vector2f landingZone1Direction(1,0);


int no_of_planes=0;
int score=0;
int clickedPlaneIndex=-1;

void shiftOneDown(long index);

class Plane
    {   
        public:
        sf::Texture planeTexture;
        sf::Sprite planeSprite;
        sf::Vertex line_points[10000];
        sf::Vertex line_vertices[2];
        sf::Vector2f last_direction;
        long line_ctr;
        long line_start;
        bool planeLanded;
        bool line_drawn;
        bool transition;
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
                no_of_planes++;
                planeLanded=line_drawn=transition=false;
                ctr=line_ctr=dist_travelled=line_start=0;
                sendInPlane();

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
                planeLanded=line_drawn=transition=false;
                ctr=line_ctr=dist_travelled=line_start=0;
                last_direction.x=1;
                last_direction.y=0;
                planeSprite.setRotation(135);
            }
        
        sf::Vector2f getPosition()
            {	
                return planeSprite.getPosition();
            } 

        sf::Sprite getPlaneSprite()
            {	
            	if(!planeTexture.loadFromFile("airplane.png"))
                    {
                        std::cout<<"Error opening plane icon\n";
                    }
                planeSprite.setTexture(planeTexture);
                planeSprite.setOrigin(24,24);
                return planeSprite;
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
            line_start=0;
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

        void rotate (sf::Vector2f direction)
            {  
                double rise, run, angle;
                
                
                rise= direction.y;
                run = direction.x;
                angle = 57.296*atan(double(rise/run));
                
                if (run<0)
                    planeSprite.setRotation(135+angle+180);
                else
                    planeSprite.setRotation(135+angle);
            }

        void drawLine()
			{
			    if(line_ctr>=1)
			    {
			        for(long i=line_start+1 ; i<line_ctr ; i++)
			        {
			            line_vertices[0]=line_points[i-1];
			            line_vertices[1]=line_points[i];
			            window.draw(line_vertices , 2 , sf::Lines);
			        }
			    }
			}

        void moveInDirection(sf::Vector2f initial_point , sf::Vector2f direction , int distance = 20000)
            {
                if(dist_travelled < distance) {
                    
                    float hyp = sqrt(direction.x*direction.x + direction.y*direction.y);
                    float sine = (direction.y)/hyp;
                    float cosine = (direction.x)/hyp;


                    
                    //planeSprite.setPosition(initial_point.x + SPEED*timer*cosine , initial_point.y + SPEED*timer*sine);
                    planeSprite.move(SPEED*cosine , SPEED*sine);
                    
                    if (distance!=20000)
                    dist_travelled+=SPEED;
                    if (dist_travelled>=distance)
                    dist_travelled=0;
                }
            }

        void landing (sf::Vector2f landingZoneDirection, sf::Vector2f landingZone, int planeIndex)
        	{	

        		rotate(landingZone1Direction);      		
        		moveInDirection(landingZone,landingZoneDirection);
        		sf::Vector2f scale = planeSprite.getScale();
        		if(scale.x>0.1)
        			planeSprite.setScale(scale.x - 0.01f,scale.y -0.01f);
        		else
        		{	
        			transition=false;
        			shiftOneDown(planeIndex);
                	no_of_planes-=1;
                    score+=1;        			
        		}
        	}

	    void moveInLine()
	    	{	
	    		if (line_drawn && ctr==0)
	    			{	
	    				
	    				last_direction.y = line_points[ctr].position.y- getPosition().y;
	    				last_direction.x = line_points[ctr].position.x- getPosition().x;
	    				float distance =sqrt(last_direction.x*last_direction.x + last_direction.y*last_direction.y);
	    				moveInDirection (getPosition() , last_direction , int (distance+dist_travelled)); //distance+dist_travelled because every time distance decreases by 1 unit and dist_travelled increases by 1 but sum remains constant (varying getPosition())
	    				rotate(last_direction);
	    			
	    				if (dist_travelled==0)
	    					{
	    						ctr++;
	    						line_start++;
	    					}

	    			}
	    		else if (line_drawn && ctr<line_ctr-1)
	    			{	
	    				
	    				last_direction.y = line_points[ctr].position.y- line_points[ctr-1].position.y;
	    				last_direction.x = line_points[ctr].position.x- line_points[ctr-1].position.x;
	    				float distance =sqrt(last_direction.x*last_direction.x + last_direction.y*last_direction.y);
	    				moveInDirection ( line_points[ctr].position , last_direction , int (distance));
	    				rotate(last_direction);
	    				
	    				
	    				if (dist_travelled==0)
	    					{
	    						ctr++;
	    						line_start++;
	    						
	    					}

	    			}
               
                else if(line_drawn && ctr==line_ctr-1 && ctr!=0)
                    {
                        last_direction.y = line_points[ctr].position.y- line_points[ctr-1].position.y;
                        last_direction.x = line_points[ctr].position.x- line_points[ctr-1].position.x;
                        moveInDirection ( line_points[ctr].position , last_direction);
                        rotate(last_direction);
                        
                        
                        refreshPointsList();
                        ctr++;
                        line_start++;
                        
                    }

	    	    else
	    	    	{	
                        ctr=0;
                        moveInDirection ( getPosition() , last_direction);
                    }
            }

            void sendInPlane()
            {	
            	srand(time(NULL));
            	switch(rand()%4)
            	{	
            		/*
            		0 : -40< x < -20
            			  0 < y < 359
            		1 :  790< x < 810
            			  0 < y < 359
					2 :  -40 < x< 810
						 -40 < y <-20
            		3 :  -40 < x< 810
						 380 < y < 400
					*/ 
            		case 0: planeSprite.setPosition(-40 + rand()%20,rand()%359);
            				break;
            		case 1:	planeSprite.setPosition(790 + rand()%20,rand()%359);
            				break;
            		case 2:	planeSprite.setPosition(-40 + rand()%850,-40 +rand()%20);
            				break;
            		case 3:	planeSprite.setPosition(-40 + rand()%850,380 +rand()%20);
            				break;
            	}
            	last_direction.x=(774/2) - getPosition().x;
                last_direction.y=(360/2) - getPosition().y;
                rotate(last_direction);                
            }



    };

std::vector<Plane> planes;


void shiftOneDown(long index)
{
    /*for(long i=index ; i<no_of_planes-1 ; i++)
    {
        planes[i]=planes[i+1];
    }
    */
    planes.erase(planes.begin()+index);
    //no_of_planes--;
}

std::string convertInt(int number)
{
    if (number == 0)
        return "0";
    std::string temp="";
    std::string returnvalue="";
    while (number>0)
    {
        temp+=number%10+48;
        number/=10;
    }
    for (int i=0;i<temp.length();i++)
        returnvalue+=temp[temp.length()-i-1];
    return returnvalue;
}

int manhattan_distance (long index , sf::Vector2i mouse_pos , long clickedPlaneIndex)
{
    return fabs(mouse_pos.y - planes[clickedPlaneIndex].line_points[index].position.y) + fabs(mouse_pos.x - planes[clickedPlaneIndex].line_points[index].position.x);
}

float distance (sf::Vector2f p1 , sf::Vector2f p2)
{
    return sqrt((p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y));
}

float slope (sf::Vector2f p1 , sf::Vector2f p2)
{
    if(p2.x!=p1.x)
        return ((float(p2.y)-float(p1.y))/(float(p2.x)-float(p1.x)));
    else
        return -1;
}

void drawLinesForAll()
{
    for(int i=0 ; i<no_of_planes ; i++)
    {   if (!planes[i].planeLanded)
    		{	
    			planes[i].moveInLine();
        		planes[i].drawLine();
        	}
        if (planes[i].getPosition().y>120 && planes[i].getPosition().y<150 && planes[i].getPosition().x>240 && planes[i].getPosition().x<350 && planes[i].last_direction.x >0)
        	{	
                if(fabs(planes[i].last_direction.y/planes[i].last_direction.x)<0.3)
                {
				    planes[i].transition=true;
				    planes[i].planeLanded=true;    
                }
			}
    }
}

int whichPlaneClicked(sf::Vector2i mouse_pos)
{
    for(int i=0 ; i<no_of_planes ; i++)
    {
        if(planes[i].isPlaneClicked(mouse_pos))                          
            return i;
    }
    return -1;
}

int main()
{	
	sf::Clock clock, totalClock;
	sf::Time time, totalTime;
	int dist;
    bool planeClicked = false;
    bool click_started = false;
    float last_gap =5;

    window.create(sf::VideoMode (774,359), "Game");
    window.setPosition(sf::Vector2i(100,100));
    window.setTitle("Arphal :P");
    
    

     //Setting background
    sf::Texture texture;
    if (!texture.loadFromFile("airport.png"))
    {
        std::cout<<"Texture not found";
    }
    
    sf::Sprite bg;
    bg.setTexture(texture);

    sf::Font font;
    if(!font.loadFromFile("Roboto-Medium.ttf"))
    {
        std::cout<<"Error! Font not found";
    }

    sf::Text scoretext;
    scoretext.setFont(font);
    scoretext.setColor(sf::Color::Red);
    scoretext.setPosition(600,0);
    sf::Text timetext;
    timetext.setFont(font);
    timetext.setColor(sf::Color::Red);
    timetext.setPosition(10,0);


    sf::RectangleShape landingRectangle1(sf::Vector2f(250,32));
    landingRectangle1.setFillColor(sf::Color(0,255,0,150));
    landingRectangle1.setPosition(sf::Vector2f(249,120));

    

    while (window.isOpen())
    {   
        mouse_pos=sf::Mouse::getPosition(window);
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed :
                    window.close();
                    break;
                default :
                    break;
            }

        }

        time = clock.getElapsedTime();
        totalTime = totalClock.getElapsedTime();
        float gap = 5*exp(-0.01*totalTime.asSeconds());
	    std::cout<<time.asSeconds()<<" Gap= "<<last_gap<<" "<<no_of_planes<<std::endl;

	   
	    if(time.asSeconds()>last_gap)
	    {
	    	planes.push_back(Plane());
	    	last_gap=gap;
	    	clock.restart();
	    	
	    }
	    

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
	        {
	            mouse_pos=sf::Mouse::getPosition(window);
	            if(!click_started) {
	                clickedPlaneIndex = whichPlaneClicked(mouse_pos);
	                if (clickedPlaneIndex>=0) {
	                	planes[clickedPlaneIndex].refreshPointsList();

                    }
	                
	            }

	            click_started=true;

	            if(clickedPlaneIndex>=0 && planes[clickedPlaneIndex].line_ctr==0) {

	                planes[clickedPlaneIndex].line_points[planes[clickedPlaneIndex].line_ctr]=sf::Vertex(sf::Vector2f(mouse_pos.x , mouse_pos.y));
	                planes[clickedPlaneIndex].line_ctr++; 
	                
	            }
	            else if(clickedPlaneIndex>=0)
	            {
	                dist = manhattan_distance(planes[clickedPlaneIndex].line_ctr-1,mouse_pos,clickedPlaneIndex);
                    //std::cout<<slope(planes[clickedPlaneIndex].line_points[(planes[clickedPlaneIndex].line_ctr)-1].position , planes[clickedPlaneIndex].line_points[(planes[clickedPlaneIndex].line_ctr)-2].position)<<std::endl;
                    //std::cout<<planes[clickedPlaneIndex].line_points[planes[clickedPlaneIndex].line_ctr-1].position.x<<" "<<planes[clickedPlaneIndex].line_points[planes[clickedPlaneIndex].line_ctr-1].position.y<<" "<<planes[clickedPlaneIndex].line_points[planes[clickedPlaneIndex].line_ctr-2].position.x<<" "<<planes[clickedPlaneIndex].line_points[planes[clickedPlaneIndex].line_ctr-2].position.y<<std::endl;

	                if (planes[clickedPlaneIndex].line_points[planes[clickedPlaneIndex].line_ctr-1].position.x>240 &&
	                	planes[clickedPlaneIndex].line_points[planes[clickedPlaneIndex].line_ctr-1].position.x<350 &&
	                	planes[clickedPlaneIndex].line_points[planes[clickedPlaneIndex].line_ctr-1].position.y>125 && 
	                	planes[clickedPlaneIndex].line_points[planes[clickedPlaneIndex].line_ctr-1].position.y<150 && 
	                	fabs(slope(planes[clickedPlaneIndex].line_points[(planes[clickedPlaneIndex].line_ctr)-1].position , planes[clickedPlaneIndex].line_points[(planes[clickedPlaneIndex].line_ctr)-2].position)) < 0.3

                        && planes[clickedPlaneIndex].line_points[(planes[clickedPlaneIndex].line_ctr)-1].position.x > planes[clickedPlaneIndex].line_points[(planes[clickedPlaneIndex].line_ctr)-2].position.x)
	                {
	                	planes[clickedPlaneIndex].line_drawn=true;
	                    planes[clickedPlaneIndex].line_points[planes[clickedPlaneIndex].line_ctr]=sf::Vertex(sf::Vector2f(mouse_pos.x , mouse_pos.y));
	                    planes[clickedPlaneIndex].line_ctr++;
	                    clickedPlaneIndex=-1;
	            	}
	                else if(dist>line_resolution) 
	                {
	                	planes[clickedPlaneIndex].line_drawn=true;
	                    planes[clickedPlaneIndex].line_points[planes[clickedPlaneIndex].line_ctr]=sf::Vertex(sf::Vector2f(mouse_pos.x , mouse_pos.y));
	                    planes[clickedPlaneIndex].line_ctr++;
	            	}
	            
	            }
	        }
        else
	        {
	            clickedPlaneIndex=-1;
	            click_started=false;
	        }



        window.draw(bg);
        if(clickedPlaneIndex>=0)
        {
            window.draw(landingRectangle1);
        }

        
        
        timetext.setString("Time: "+convertInt(totalTime.asSeconds()));
        scoretext.setString("Score : " + convertInt(score));
        window.draw(scoretext);
        window.draw(timetext);


        for(int i=0; i<no_of_planes;i++)
        	{	
        		if (!planes[i].planeLanded)
        			window.draw(planes[i].getPlaneSprite());
	        	if (planes[i].transition==true)
	        	{
	        		planes[i].landing(landingZone1Direction,landingZone1,i);
	        		window.draw(planes[i].getPlaneSprite());
	        	}
        	}

        

        drawLinesForAll();

        
        window.display();
    }

    return 0;

}