#include <SFML/Graphics.hpp>
#include <iostream>
#include<queue>
#include<vector>
#include <cstdlib>
#include <ctime>
using namespace sf;
using namespace std;



enum GameState{ MENU, PLAYING, OPTIONS, CHOOSETRAFFIC, CHOOSECAR};
GameState currentState = MENU;

bool gameStarted = true;

string trafficStrength = "";
string chooseCar = "";


RenderWindow window;
Vector2u winSize;

Sprite* mapSprite = nullptr;
Texture mapTexture;

Sprite* mosqueSprite = nullptr;
Texture mosqueTexture;

Sprite* hospitalSprite = nullptr;
Texture hospitalTexture;

Texture carTexture;
float scaleX, scaleY;
Sprite* carSprite = nullptr;


Cursor* handCursor = nullptr;
Cursor* arrowCursor = nullptr;

Texture upRedSignalTexture;
Texture upGreenSignalTexture;
Texture downRedSignalTexture;
Texture downGreenSignalTexture;

Texture logo;
Sprite* logoSprite = nullptr;

Texture background;
Sprite* backgroundSprite = nullptr;

Texture start;
Sprite* startSprite = nullptr;

Texture back;
Sprite* backSprite = nullptr;

Texture options;
Sprite* optionsSprite = nullptr;

Texture quit;
Sprite* quitSprite = nullptr;

Texture qasim;
Sprite* qasimSprite = nullptr;

Texture bilal;
Sprite* bilalSprite = nullptr;

string personalDestination = "policestation";



class Button{
    public:
        Texture button;
        Sprite* buttonSprite = nullptr;
        string name;
        float originalx = 0;
        float originaly = 0;

    Button(string name){
        if(name == "choosetraffic"){
            button.loadFromFile("choosetraffic.png");
        }else if(name == "back"){
            button.loadFromFile("back.png");
        }else if(name == "high"){
            button.loadFromFile("high.png");
        }else if(name == "medium"){
            button.loadFromFile("medium.png");
        }else if(name == "low"){
            button.loadFromFile("low.png");
        }else if(name == "superCar"){
            button.loadFromFile("superCarButton.png");
        }else if(name == "normalCar"){
            button.loadFromFile("normalCarButton.png");
        }else if(name == "policeCar"){
            button.loadFromFile("policeCarButton.png");
        }else if(name == "taxi"){
            button.loadFromFile("taxiButton.png");
        }else if(name == "ambulance"){
            button.loadFromFile("ambulanceButton.png");
        }else if(name == "firetruck"){
            button.loadFromFile("firetruckButton.png");
        }else if(name == "truck"){
            button.loadFromFile("truckButton.png");
        }else if(name == "chooseCarLogo"){
            button.loadFromFile("chooseCarLogo.png");
        }

        this->name = name;



        buttonSprite = new Sprite(button);
        FloatRect bounds = buttonSprite->getLocalBounds();
        buttonSprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    }

    Vector2f getPosition(){
        return buttonSprite->getPosition();
    }

    void setPosition(float x,float y){
        buttonSprite->setPosition({x,y});
        originalx = x;
        originaly = y;
    }

    void setTempPosition(float x,float y){
        buttonSprite->setPosition({x,y});
    }

    void setScale(float x,float y){
        buttonSprite->setScale({x,y});
    }

    void show(){
        window.draw(*buttonSprite);
    }

};

Button traffic("choosetraffic");
Button chooseCarLogo("chooseCarLogo");
Button backButton("back");
Button highButton("high");
Button mediumButton("medium");
Button lowButton("low");
Button superCarButton("superCar");
Button normalCarButton("normalCar");
Button taxiButton("taxi");
Button policeCarButton("policeCar");
Button ambulanceButton("ambulance");
Button firetruckButton("firetruck");
Button truckButton("truck");

vector<Button> chooseCarButtons;

class Building{
    public:
    float x1,x2,y1,y2;
    string name;
    Building(float x1,float x2, float y1, float y2, string name){
        this->x1 = x1;
        this->x2 = x2;
        this->y1 = y1;
        this->y2 = y2;
        this->name = name;
    }
};


class Node{
    public:
        int id;
        float x,y;
        bool hasSignal;
        bool hasUp;
        bool hasDown;
        bool hasLeft;
        bool hasRight;

        float topLeftx;
        float topLefty;
        float topRightx;
        float topRighty;
        float downLeftx;
        float downLefty;
        float downRightx;
        float downRighty;

        int upNode;
        int downNode;
        int leftNode;
        int rightNode;
        int upEdge;
        int downEdge;
        int leftEdge;
        int rightEdge;

        bool ghostNode;
        char ghostStart;

        int occupied = 0;

        Node(){

        }

        Node(int id,float x,float y,bool hasSignal,bool hasUp, bool hasDown, bool hasLeft, bool hasRight,float topleftx,float toplefty,float topRightx,float topRighty, float downLeftx,float downLefty, float downRightx,float downRighty,int upNode,int downNode,int leftNode,int rightNode,int upEdge,int downEdge, int leftEdge, int rightEdge,bool ghostNode,char ghostStart){
            this->id = id;
            this->x = x;
            this->y = y;


            this->hasSignal = hasSignal;
            this->hasUp = hasUp;
            this->hasDown = hasDown;
            this->hasLeft = hasLeft;
            this->hasRight = hasRight;


            this->topLeftx = topleftx;
            this->topLefty = toplefty;
            this->topRightx = topRightx;
            this->topRighty = topRighty;
            this->downLeftx = downLeftx;
            this->downLefty = downLefty;
            this->downRightx = downRightx;
            this->downRighty = downRighty;


            this->upNode = upNode;
            this->downNode = downNode;
            this->leftNode = leftNode;
            this->rightNode = rightNode;
            this->upEdge = upEdge;
            this->downEdge = downEdge;
            this->leftEdge = leftEdge;
            this->rightEdge = rightEdge;

            this->ghostNode = ghostNode;
            this->ghostStart = ghostStart;
        }


};

class Edge{
    public:
        int id;
        float x1,x2,y1,y2;
        bool hasUp;
        bool hasDown;
        bool hasLeft;
        bool hasRight;

        int upNode;
        int downNode;
        int leftNode;
        int rightNode;


        Sprite* upRedSignalSprite = nullptr;

        Sprite* upGreenSignalSprite = nullptr;

        Sprite* downRedSignalSprite = nullptr;

        Sprite* downGreenSignalSprite = nullptr;

        float ux = 0;
        float uy = 0;
        float dx = 0;
        float dy = 0;

        Edge(){

        }

        Edge(int id,float x1,float x2,float y1,float y2,bool hasUp, bool hasDown, bool hasLeft, bool hasRight,int upNode,int downNode,int leftNode, int rightNode,float ux,float uy,float dx,float dy){
            this->id = id;
            this->x1 = x1;
            this->x2 = x2;
            this->y1 = y1;
            this->y2 = y2;
            this->hasUp = hasUp;
            this->hasDown = hasDown;
            this->hasLeft = hasLeft;
            this->hasRight = hasRight;
            this->upNode = upNode;
            this->downNode = downNode;
            this->leftNode = leftNode;
            this->rightNode = rightNode;


            if(ux == 0){
                ux = -50.0f;
            }
            if(uy == 0){
                uy = -50.0f;
            }
            if(dx == 0){
                dx = -50.0f;
            }
            if(dy == 0){
                dy = -50.0f;
            }

            this->ux = ux;
            this->uy = uy;
            this->dx = dx;
            this->dy = dy;




        }

        void init(){
            upRedSignalSprite = new Sprite(upRedSignalTexture);
            FloatRect bounds1 = upRedSignalSprite->getLocalBounds();
            upRedSignalSprite->setOrigin({bounds1.size.x / 2.0f, bounds1.size.y / 2.0f});
            upRedSignalSprite->setScale({0.5,0.5});
            upRedSignalSprite->setPosition({ux,uy});


            upGreenSignalSprite = new Sprite(upGreenSignalTexture);
            FloatRect bounds2 = upGreenSignalSprite->getLocalBounds();
            upGreenSignalSprite->setOrigin({bounds2.size.x / 2.0f, bounds2.size.y / 2.0f});
            upGreenSignalSprite->setScale({0.5,0.5});
            upGreenSignalSprite->setPosition({ux,uy});

            downRedSignalSprite = new Sprite(downRedSignalTexture);
            FloatRect bounds3 = downRedSignalSprite->getLocalBounds();
            downRedSignalSprite->setOrigin({bounds3.size.x / 2.0f, bounds3.size.y / 2.0f});
            downRedSignalSprite->setScale({0.5,0.5});
            downRedSignalSprite->setPosition({dx,dy});

            downGreenSignalSprite = new Sprite(downGreenSignalTexture);
            FloatRect bounds4 = downGreenSignalSprite->getLocalBounds();
            downGreenSignalSprite->setOrigin({bounds4.size.x / 2.0f, bounds4.size.y / 2.0f});
            downGreenSignalSprite->setScale({0.5,0.5});
            downGreenSignalSprite->setPosition({dx,dy});
        }

        void showAllSignals(){
            window.draw(*downGreenSignalSprite);
            window.draw(*upGreenSignalSprite);
            window.draw(*upRedSignalSprite);
            window.draw(*downRedSignalSprite);
        }

        void hideUpRedSignal(){
            upRedSignalSprite->setPosition({-10.0f,-10.0f});
        }

        void showUpRedSignal(){
            upRedSignalSprite->setPosition({ux,uy});
        }

        void hideDownRedSignal(){
            downRedSignalSprite->setPosition({-10.0f,-10.0f});
        }

        void showDownRedSignal(){
            downRedSignalSprite->setPosition({dx,dy});
        }

};


Building mosque(105.0f,250.0f,115.0f,236.0f,"mosque");
Building hospital(810.0f,1095.0f,94.0f,200.0f,"hospital");
Building policestation(873.0f,1031.0f,391.0f,540.0f,"policestation");
Building pump(1461.0f,1745.0f,416.0f,560.0f,"pump");
Building rightbottomhouse(1462.0f,1919.0f,674.0f,770.0f,"rightbottomhouse");
Building leftground(316.0f,603.0f,691.0f,913.0f,"leftground");
Building park(88.0f,450.0f,416.0f,670.0f,"park");
Building bank(1151.0f,1350.0f,406.0f,606.0f,"bank");
Building grave(1138.0f,1345.0f,126.0f,315.0f,"grave");
Building resturant(1461.0f,1751.0f,126.0f,318.0f,"resturant");
Building school(1136.0f,1360.0f,690.0f,913.0f,"school");
Building market(693.0f,1039.0f,680.0f,905.0f,"market");

vector<Building> buildings = {mosque,hospital,policestation,pump,rightbottomhouse,leftground,park,bank,grave,resturant,school,market};

Node n1(1,334.0f,90.0f,false,true,true,true,true,324.0f,82.0f,349.0f,82.0f,324.0f,99.0f,349.0f,99.0f,21,5,25,2,1,9,5,6,false,'n');
Node n2(2,653.0f,89.0f,true,true,true,true,true,636.0f,81.0f,670.0f,81.0f,637.0f,100.0f,669.0f,100.0f,22,6,1,3,2,10,6,7,false,'n');
Node n3(3,1409.0f,89.0f,true,true,true,true,true,1393.0f,82.0f,1426.0f,82.0f,1392.0f,99.0f,1426.0f,99.0f,23,8,2,4,3,11,7,8,false,'n');
Node n4(4,1797.0f,89.0f,false,true,true,true,false,1781.0f,83.0f,1814.0f,82.0f,1781.0f,99.0f,1813.0f,99.0f,24,9,3,0,4,12,8,0,false,'n');
Node n5(5,335.0f,366.0f,false,true,false,true,true,325.0f,350.0f,350.0f,353.0f,324.0f,379.0f,348.0f,379.0f,1,0,27,6,9,0,13,14,false,'n');
Node n6(6,653.0f,365.0f,true,true,true,true,true,637.0f,349.0f,669.0f,349.0f,637.0f,381.0f,669.0f,381.0f,2,10,5,7,10,19,14,15,false,'n');
Node n7(7,1088.0f,366.0f,false,false,true,true,true,1071.0f,350.0f,1104.0f,350.0f,1071.0f,378,1104.0f,378.0f,0,11,6,8,0,20,15,16,false,'n');
Node n8(8,1409.0f,365.0f,true,true,true,true,true,1393.0f,351.0f,1425.0f,351.0f,1393.0f,381.0f,1425.0f,381.0f,3,12,7,9,11,21,16,17,false,'n');
Node n9(9,1798.0f,366.0f,false,true,true,true,true,1781.0f,351.0f,1813.0f,351.0f,1781.0f,381.0f,1814.0f,378.0f,4,13,8,26,12,22,17,18,false,'n');
Node n10(10,653.0f,641.0f,true,true,true,true,true,637.0f,624.0f,669.0f,624.0f,637.0f,657,669.0f,656.0f,6,16,36,11,19,28,23,24,false,'n');
Node n11(11,1089.0f,641.0f,true,true,true,true,true,1071.0f,624.0f,1103.0f,624.0f,1072.0f,657.0f,1104.0f,657.0f,7,17,10,12,20,29,24,25,false,'n');
Node n12(12,1408.0f,640.0f,true,true,true,true,true,1393.0f,624.0f,1426.0f,624.0f,1393.0f,656.0f,1425.0f,657.0f,8,18,11,13,21,30,25,26,false,'n');
Node n13(13,1798.0f,640.0f,false,true,false,true,true,1782.0f,625.0f,1814.0f,625.0f,1781.0f,656.0f,1814.0f,657.0f,9,0,12,28,22,0,26,27,false,'n');
Node n14(14,273.0f,723.0f,false,false,true,true,false,262.0f,705.0f,287.0f,705.0f,262.0f,737.0f,286.0f,737.0f,0,15,29,0,0,32,31,0,false,'n');
Node n15(15,274.0f,963.0f,false,true,true,true,true,262.0f,946.0f,285.0f,947.0f,262.0f,978.0f,285.0f,978.0f,14,31,30,16,32,40,33,34,false,'n');
Node n16(16,653.0f,962.0f,true,true,true,true,true,637.0f,946.0f,670.0f,947.0f,636.0f,978.0f,670.0f,978.0f,10,32,15,17,28,41,34,35,false,'n');
Node n17(17,1087.0f,963.0f,true,true,true,true,true,1072.0f,946.0f,1103.0f,947.0f,1072.0f,978.0f,1104.0f,978.0f,11,33,16,18,29,42,35,36,false,'n');
Node n18(18,1409.0f,963.0f,true,true,true,true,true,1394.0f,953.0f,1425.0f,953.0f,1397.0f,972.0f,1426.0f,972.0f,12,34,17,19,30,43,36,37,false,'n');
Node n19(19,1798.0f,962.0f,false,true,false,true,false,1790.0f,955.0f,1806.0f,955.0f,1788.0f,970.0f,1805.0f,970.0f,20,0,18,0,38,0,37,0,false,'n');
Node n20(20,1798.0f,801.0f,false,false,true,true,false,1789.0f,792.0f,1805.0f,792.0f,1789.0f,809.0f,1805.0f,809.0f,0,19,35,0,0,38,39,0,false,'n');
Node n35(35,1675.0f,801.0f,false,false,false,false,true,1684.0f,793.0f,1708.0f,793.0f,1676.0f,810.0f,1706.0f,810.0f,0,0,0,20,0,0,0,39,false,'n');
Node n36(36,450.0f,641.0f,false,false,false,false,true,460.0f,625.0f,460.0f,625.0f,460.0f,655.0f,460.0f,655.0f,0,0,0,10,0,0,0,23,false,'n');

Node g21(21,349.0f,-50.0f,false,false,true,false,false,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,true,'d');
Node g22(22,668.0f,-50.0f,false,false,true,false,false,0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,0,true,'d');
Node g23(23,1425.0f,-50.0f,false,false,true,false,false,0,0,0,0,0,0,0,0,0,3,0,0,0,3,0,0,true,'d');
Node g24(24,1814.0f,-50.0f,false,false,true,false,false,0,0,0,0,0,0,0,0,0,4,0,0,0,4,0,0,true,'d');

Node g26(26,1972.0f,377.0f,false,false,false,true,false,0,0,0,0,0,0,0,0,0,0,9,0,0,0,18,0,true,'l');
Node g28(28,1972.0f,656.0f,false,false,false,true,false,0,0,0,0,0,0,0,0,0,0,13,0,0,0,27,0,true,'l');

Node g25(25,-50.0f,84.0f,false,false,false,false,true,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,5,true,'r');
Node g27(27,-50.0f,350.0f,false,false,false,false,true,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,13,true,'r');
Node g29(29,-50.0f,706.0f,false,false,false,false,true,0,0,0,0,0,0,0,0,0,0,0,14,0,0,0,31,true,'r');
Node g30(30,-50.0f,948.0f,false,false,false,false,true,0,0,0,0,0,0,0,0,0,0,0,15,0,0,0,33,true,'r');

Node g31(31,261.0f,1112.0f,false,true,false,false,false,0,0,0,0,0,0,0,0,15,0,0,0,40,0,0,0,true,'u');
Node g32(32,638.0f,1112.0f,false,true,false,false,false,0,0,0,0,0,0,0,0,16,0,0,0,41,0,0,0,true,'u');
Node g33(33,1072.0f,1112.0f,false,true,false,false,false,0,0,0,0,0,0,0,0,17,0,0,0,42,0,0,0,true,'u');
Node g34(34,1392.0f,1112.0f,false,true,false,false,false,0,0,0,0,0,0,0,0,18,0,0,0,43,0,0,0,true,'u');

Edge e1(1,315.0f,362.0f,1.0f,75.0f,true,true,false,false,21,1,0,0  ,0,0,0,0);
Edge e2(2,622.0f,685.0f,1.0f,75.0f,true,true,false,false,22,2,0,0   ,0,0,694.0f,47.0f);
Edge e3(3,1379.0f,1442.0f,1.0f,75.0f,true,true,false,false,23,3,0,0  ,0,0,1452.0f,47.0f);
Edge e4(4,1767.0f,1829.0f,1.0f,75.0f,true,true,false,false,24,4,0,0  ,0,0,0,0);
Edge e5(5,1.0f,313.0f,75.0f,108.0f,false,false,true,true,0,0,25,1  ,0,0,0,0);
Edge e6(6,363.0f,622.0f,75.0f,108.0f,false,false,true,true,0,0,1,2  ,614,45,0,0);
Edge e7(7,684.0f,1381.0f,76.0f,108.0f,false,false,true,true,0,0,2,3  ,1370,44,693,96);
Edge e8(8,1441.0f,1767.0f,76.0f,108.0f,false,false,true,true,0,0,3,4  ,0,0,1450,96);
Edge e9(9,314.0f,363.0f,108.0f,336.0f,true,true,false,false,1,5,0,0  ,0,0,0,0);
Edge e10(10,621.0f,682.0f,109.0f,336.0f,true,true,false,false,2,6,0,0  ,614,96,694,303);
Edge e11(11,1377.0f,1442.0f,108.0f,335.0f,true,true,false,false,3,8,0,0  ,1371,97,1451,303);
Edge e12(12,1767.0f,1831.0f,108.0f,335.0f,true,true,false,false,4,9,0,0  ,0,0,0,0);
Edge e13(13,1.0f,312.0f,338.0f,388.0f,false,false,true,true,0,0,27,5  ,0,0,0,0);
Edge e14(14,364.0f,623.0f,335.0f,397.0f,false,false,true,true,0,0,5,6  ,613,302,0,0);
Edge e15(15,685.0f,1058.0f,335.0f,389.0f,false,false,true,true,0,0,6,7  ,0,0,692,386);
Edge e16(16,1118.0f,1378.0f,335.0f,389.0f,false,false,true,true,0,0,7,8  ,1369,303,0,0);
Edge e17(17,1441.0f,1767.0f,337.0f,395.0f,false,false,true,true,0,0,8,9  ,0,0,1449,386);
Edge e18(18,1831.0f,1919.0f,336.0f,396.0f,false,false,true,true,0,0,9,26  ,0,0,0,0);
Edge e19(19,622.0f,685.0f,398.0f,608.0f,true,true,false,false,6,10,0,0  ,614,386,693,578);
Edge e20(20,1056.0f,1121.0f,396.0f,608.0f,true,true,false,false,7,11,0,0  ,0,0,1129,577);
Edge e21(21,1378.0f,1441.0f,397.0f,609.0f,true,true,false,false,8,12,0,0  ,1371,386,1450,577);
Edge e22(22,1767.0f,1829.0f,397.0f,609.0f,true,true,false,false,9,13,0,0  ,0,0,0,0);
Edge e23(23,460.0f,620.0f,610.0f,672.0f,false,false,true,true,0,0,36,10  ,613,578,0,0);
Edge e24(24,685.0f,1058.0f,612.0f,672.0f,false,false,true,true,0,0,10,11  ,1048,576,693,661);
Edge e25(25,1121.0f,1378.0f,609.0f,672.0f,false,false,true,true,0,0,11,12  ,1370,577,1128,661);
Edge e26(26,1443.0f,1768.0f,609.0f,672.0f,false,false,true,true,0,0,12,13  ,0,0,1450,661);
Edge e27(27,1831.0f,1919.0f,609.0f,672.0f,false,false,true,true,0,0,13,28  ,0,0,0,0);
Edge e28(28,622.0f,686.0f,673.0f,931.0f,true,true,false,false,10,16,0,0  ,614,661,693,898);
Edge e29(29,1058.0f,1121.0f,673.0f,931.0f,true,true,false,false,11,17,0,0  ,1048,662,1128,899);
Edge e30(30,1384.0f,1442.0f,673.0f,931.0f,true,true,false,false,12,18,0,0  ,1369,661,1451,921);
Edge e31(31,1.0f,250.0f,690.0f,752.0f,false,false,true,true,0,0,29,14  ,0,0,0,0);
Edge e32(32,251.0f,298.0f,752.0f,931.0f,true,true,false,false,14,15,0,0  ,0,0,0,0);
Edge e33(33,1.0f,250.0f,932.0f,993.0f,false,false,true,true,0,0,30,15  ,0,0,0,0);
Edge e34(34,298.0f,620.0f,933.0f,993.0f,false,false,true,true,0,0,15,16  ,613,900,0,0);
Edge e35(35,686.0f,1058.0f,933.0f,993.0f,false,false,true,true,0,0,16,17  ,1048,900,692,985);
Edge e36(36,1121.0f,1376.0f,933.0f,993.0f,false,false,true,true,0,0,17,18  ,1370,899,1128,986);
Edge e37(37,1444.0f,1781.0f,947.0f,978.0f,false,false,true,true,0,0,18,19  ,0,0,1450,987);
Edge e38(38,1781.0f,1814.0f,817.0f,946.0f,true,true,false,false,20,19,0,0  ,0,0,0,0);
Edge e39(39,1691.0f,1780.0f,788.0f,817.0f,false,false,true,true,0,0,35,20  ,0,0,0,0);
Edge e40(40,251.0f,298.0f,994.0f,1079.0f,true,true,false,false,15,31,0,0  ,0,0,0,0);
Edge e41(41,622.0f,685.0f,993.0f,1079.0f,true,true,false,false,16,32,0,0  ,613,984,0,0);
Edge e42(42,1057.0f,1120.0f,993.0f,1079.0f,true,true,false,false,17,33,0,0  ,1048,984,0,0);
Edge e43(43,1380.0f,1442.0f,993.0f,1079.0f,true,true,false,false,18,34,0,0  ,1369,985,0,0);


vector<Edge> edges = {e1,e2,e3,e4,e5,e6,e7,e8,e9,e10,e11,e12,e13,e14,e15,e16,e17,e18,e19,e20,e21,e22,e23,e24,e25,e26,e27,e28,e29,e30,e31,e32,e33,e34,e35,e36,e37,e38,e39,e40,e41,e42,e43};
vector<Node> nodes = {n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,g21,g22,g23,g24,g25,g26,g27,g28,g29,g30,g31,g32,g33,g34,n35,n36};
vector<Node> ghosts = {g21,g22,g23,g24,g25,g26,g27,g28,g29,g30,g31,g32,g33,g34};


Node findNode(int id){
    for(int i = 0;i<nodes.size();i++){
        if(nodes[i].id == id){
            return nodes[i];
        }
    }
}

Edge* findEdge(int id){
    for(int i = 0;i<edges.size();i++){
        if(edges[i].id == id){
            return &edges[i];
        }
    }
    return nullptr;
}


class Vehicle{
    public:
        int id;
        Texture vehicleTexture;
        Sprite* vehicleSprite = nullptr;
        Edge edge;
        Node node;
        float rotation = 0.0f;
        float movingx = 0.0f;
        float movingy = 0.0f;
        float speed = 0.0f;
        queue<int> path;
        int prevNode = -1;
        bool uturnfirst = false;
        bool started = false;
        string type;
        bool personallyControlled = false;
        string currentDestination = "";
        bool bfsdone = false;

        Vehicle(string type,int id){
            this->id = id;
            this->type = type;
            if(type == "car"){
                vehicleTexture.loadFromFile("car.png");
                vehicleSprite = new Sprite(vehicleTexture);
                FloatRect bounds = vehicleSprite->getLocalBounds();
                vehicleSprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
                vehicleSprite->setScale({scaleX*1.1,scaleY*1.1});
                speed = 3;
            }else if(type == "truck"){
                vehicleTexture.loadFromFile("truck.png");
                vehicleSprite = new Sprite(vehicleTexture);
                FloatRect bounds = vehicleSprite->getLocalBounds();
                vehicleSprite->setOrigin({(bounds.size.x / 2.0f)-5.0f, bounds.size.y / 2.0f});
                vehicleSprite->setScale({1,1});
                speed = 4;
            }else if(type == "ambulance"){
                vehicleTexture.loadFromFile("ambulance.png");
                vehicleSprite = new Sprite(vehicleTexture);
                FloatRect bounds = vehicleSprite->getLocalBounds();
                vehicleSprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
                vehicleSprite->setScale({0.7,0.7});
                speed = 4;
            }else if(type == "taxi"){
                vehicleTexture.loadFromFile("taxi.png");
                vehicleSprite = new Sprite(vehicleTexture);
                FloatRect bounds = vehicleSprite->getLocalBounds();
                vehicleSprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
                vehicleSprite->setScale({0.7,0.7});
                speed = 3;
            }else if(type == "bus"){
                vehicleTexture.loadFromFile("bus.png");
                vehicleSprite = new Sprite(vehicleTexture);
                FloatRect bounds = vehicleSprite->getLocalBounds();
                vehicleSprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
                vehicleSprite->setScale({0.5,0.5});
                speed = 2;
            }else if(type == "policecar"){
                vehicleTexture.loadFromFile("policecar.png");
                vehicleSprite = new Sprite(vehicleTexture);
                FloatRect bounds = vehicleSprite->getLocalBounds();
                vehicleSprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
                vehicleSprite->setScale({0.7,0.7});
                speed = 3;
            }else if(type == "firetruck"){
                vehicleTexture.loadFromFile("firetruck.png");
                vehicleSprite = new Sprite(vehicleTexture);
                FloatRect bounds = vehicleSprite->getLocalBounds();
                vehicleSprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
                vehicleSprite->setScale({0.7,0.7});
                speed = 4;
            }
            else if(type == "supercar"){
                vehicleTexture.loadFromFile("supercar.png");
                vehicleSprite = new Sprite(vehicleTexture);
                FloatRect bounds = vehicleSprite->getLocalBounds();
                vehicleSprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
                vehicleSprite->setScale({0.7,0.7});
                speed = 4;
            }
        }

        void rotate(float pos){
            vehicleSprite->setRotation(degrees(pos));
            rotation = pos;
        }

        void rotateup(){
            vehicleSprite->setRotation(degrees(90.0f));
            rotation = 90.0f;
        }

        void rotatedown(){
            vehicleSprite->setRotation(degrees(-90.0f));
            rotation = -90.0f;
        }

        void rotateleft(){
            vehicleSprite->setRotation(degrees(0.0f));
            rotation = 0.0f;
        }

        void rotateright(){
            vehicleSprite->setRotation(degrees(180.0f));
            rotation = 180.0f;
        }

        void rotator(){
            vehicleSprite->setRotation(degrees(rotation + 90.0f));
            rotation = rotation + 90.0f;
        }

        void setPosition(float x,float y){
            vehicleSprite->setPosition({x,y});
        }

        void move(float speedx,float speedy){
            vehicleSprite->move({speedx, speedy});
            movingx = speedx;
            movingy = speedy;
            window.draw(*vehicleSprite);
        }

        void moveup(){
            movingx = 0.0f;
            movingy = -speed;
            vehicleSprite->move({movingx, movingy});
            window.draw(*vehicleSprite);
        }

        void movedown(){
            movingx = 0.0f;
            movingy = speed;
            vehicleSprite->move({movingx, movingy});
            window.draw(*vehicleSprite);
        }

        void moveleft(){
            movingx = -speed;
            movingy = 0.0f;
            vehicleSprite->move({movingx, movingy});
            window.draw(*vehicleSprite);
        }

        void moveright(){
            movingx = speed;
            movingy = 0.0f;
            vehicleSprite->move({movingx, movingy});
            window.draw(*vehicleSprite);
        }



        void move(){
            vehicleSprite->move({movingx,movingy});
            window.draw(*vehicleSprite);
        }

        Vector2f getPosition(){
            return vehicleSprite->getPosition();
        }

        void setEdge(Edge edge){
            this->edge = edge;
        }

        void setNode(Node node){
            this->node = node;
        }

        void setGhostPosition(){
            for(int i = 0;i<nodes.size();i++){
                if(path.front() == nodes[i].id){
                    setPosition(nodes[i].x,nodes[i].y);
                }
            }
        }

        void stop(){

            window.draw(*vehicleSprite);
        }

        void giveRandomPath(){

            int startingGhost = 0;
            int currentNode = 0;
            int j = 0;
            bool firstStep = false;

            while(true){

                if(startingGhost == 0){
                    j = rand() % ghosts.size();
                    startingGhost = ghosts[j].id;
                    path.push(startingGhost);
                    currentNode = ghosts[j].id;
                }

                j = rand() % 4 + 1;

                if(j == 1 && findNode(currentNode).hasUp){

                    currentNode = findNode(currentNode).upNode;
                    path.push(currentNode);
                    firstStep = true;

                }else if(j == 2 && findNode(currentNode).hasDown){

                    currentNode = findNode(currentNode).downNode;
                    path.push(currentNode);
                    firstStep = true;

                }else if(j == 3 && findNode(currentNode).hasLeft){

                    currentNode = findNode(currentNode).leftNode;
                    path.push(currentNode);
                    firstStep = true;

                }else if(j == 4 && findNode(currentNode).hasRight){

                    currentNode = findNode(currentNode).rightNode;
                    path.push(currentNode);
                    firstStep = true;

                }


                if(findNode(currentNode).ghostNode && firstStep ){
                        break;
                    }



            }

            prevNode = -1;
            started = false;
            uturnfirst = false;
            setGhostPosition();
        }
};


vector<Vehicle> vehicles;



class Signal{
    public:

    int nodeID = 0;
    int frames = 0;
    int turn = 0;
    int edge = 0;
    bool initialized = false;

    Signal(int id){
        this->nodeID = id;
    }

    bool canPass(int edge){

        return (getCurrentEdge() == edge && getCurrentEdge() != 0);

    }

    int getCurrentEdge(){
        if(turn == 0){
            return findNode(nodeID).upEdge;
        }else if(turn == 1){
            return findNode(nodeID).downEdge;
        }else if(turn == 2){
            return findNode(nodeID).leftEdge;
        }else if(turn == 3){
            return findNode(nodeID).rightEdge;
        }
    }

    void update(){

        int currentEdge = getCurrentEdge();
        if(!initialized){
            Edge* e = findEdge(getCurrentEdge());
            if(e) e->hideDownRedSignal();
            initialized = true;
        }
        frames++;
        if(frames >= 180){
            frames = 0;

            Edge* expiringEdge = findEdge(currentEdge);
            if(expiringEdge != nullptr){
                if(turn == 0){
                    expiringEdge->showDownRedSignal();
                }else if(turn == 1){
                    expiringEdge->showUpRedSignal();
                }else if(turn == 2){
                    expiringEdge->showUpRedSignal();
                }else if(turn == 3){
                    expiringEdge->showDownRedSignal();
                }
            }


            if(turn == 0){
                turn = 1;
                Edge* e = findEdge(getCurrentEdge());
                if(e) e->hideUpRedSignal();

            }else if(turn == 1){
                turn = 2;
                Edge* e = findEdge(getCurrentEdge());
                if(e) e->hideUpRedSignal();

            }else if(turn == 2){
                turn = 3;
                Edge* e = findEdge(getCurrentEdge());
                if(e) e->hideDownRedSignal();

            }else if(turn == 3){
                turn = 0;
                Edge* e = findEdge(getCurrentEdge());
                if(e) e->hideDownRedSignal();
            }
        }
    }

};



Signal s1(1);
Signal s2(2);
Signal s3(3);
Signal s4(6);
Signal s5(8);
Signal s6(9);
Signal s7(10);
Signal s8(11);
Signal s9(12);
Signal s10(15);
Signal s11(16);
Signal s12(17);
Signal s13(18);

vector<Signal> signals = {s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13};


Signal* findSignal(int id){
    for(int i = 0;i<signals.size();i++){
        if(signals[i].nodeID == id){
            return &signals[i];
        }
    }
    return nullptr;
}


void TrafficManagement(Vehicle& vehicle){
    bool foundEdge = false;
    bool foundNode = false;
    float tolerance = 3.0f;
    if (vehicle.personallyControlled){
        return;
    }

    if(vehicle.path.empty()){
        vehicle.giveRandomPath();
        return;
    }

    int path = vehicle.path.front();

    //checking if vehicles don't collide on one another and stop if there is a vehicle ahead
    int space = 0;
    if(vehicle.type == "car"){
        space = 50;
    }else if(vehicle.type == "truck"){
        space = 80;
    }else if(vehicle.type == "firetruck"){
        space = 80;
    }else if(vehicle.type == "taxi"){
        space = 60;
    }else if(vehicle.type == "policecar"){
        space = 60;
    }else if(vehicle.type == "ambulance"){
        space = 60;
    }else if(vehicle.type == "bus"){
        space = 80;
    }else if(vehicle.type == "supercar"){
        space = 50;
    }
    for(int i = 0;i<vehicles.size();i++){
        if(vehicle.id == vehicles[i].id){
            continue;
        }
        if(vehicle.rotation == 180.0f){
            if(vehicles[i].getPosition().x > vehicle.getPosition().x && vehicles[i].getPosition().x < vehicle.getPosition().x+space && vehicles[i].getPosition().y < vehicle.getPosition().y + 10 && vehicles[i].getPosition().y > vehicle.getPosition().y - 10){
                vehicle.stop();
                return;
            }
        }else if(vehicle.rotation == -90.0f){
            if(vehicles[i].getPosition().y > vehicle.getPosition().y && vehicles[i].getPosition().y < vehicle.getPosition().y+space && vehicles[i].getPosition().x < vehicle.getPosition().x + 10 && vehicles[i].getPosition().x > vehicle.getPosition().x - 10){
                vehicle.stop();
                return;
            }
        }else if(vehicle.rotation == 90.0f){
            if(vehicles[i].getPosition().y < vehicle.getPosition().y && vehicles[i].getPosition().y > vehicle.getPosition().y-space && vehicles[i].getPosition().x < vehicle.getPosition().x + 10 && vehicles[i].getPosition().x > vehicle.getPosition().x - 10){
                vehicle.stop();
                return;
            }
        }
        else if(vehicle.rotation == 0.0f){
            if(vehicles[i].getPosition().x < vehicle.getPosition().x && vehicles[i].getPosition().x > vehicle.getPosition().x-space && vehicles[i].getPosition().y < vehicle.getPosition().y + 10 && vehicles[i].getPosition().y > vehicle.getPosition().y - 10){
                vehicle.stop();
                return;
            }
        }
    }


    for(int i = 0; i < nodes.size(); i++){

        // skip nodes vehicle isn't near
        if(abs(vehicle.getPosition().x - nodes[i].x) > 50.0f || abs(vehicle.getPosition().y - nodes[i].y) > 50.0f){
            continue;
        }
        if(nodes[i].hasSignal){

            // already inside this node → just process path, keep moving
            if(nodes[i].occupied == vehicle.id){
                if(nodes[i].id == path){
                    vehicle.prevNode = (vehicle.prevNode == -1) ? nodes[i].id : vehicle.node.id;
                    vehicle.setNode(nodes[i]);
                    vehicle.path.pop();
                    if(!vehicle.path.empty()) path = vehicle.path.front();
                }
                foundNode = true;
                break;
            }

            // new vehicle approaching → check signal
            Signal* s = findSignal(nodes[i].id);
            bool greenLight = (s != nullptr && vehicle.edge.id != 0 && s->canPass(vehicle.edge.id));
            bool nodeFree   = (nodes[i].occupied == 0);
            
          
                
            if((greenLight && nodeFree )||( nodeFree && !greenLight && vehicle.type == "ambulance")){
                // GREEN + node free → enter
                nodes[i].occupied = vehicle.id;
                if(nodes[i].id == path){
                    vehicle.prevNode = (vehicle.prevNode == -1) ? nodes[i].id : vehicle.node.id;
                    vehicle.setNode(nodes[i]);
                    vehicle.path.pop();
                    if(!vehicle.path.empty()) path = vehicle.path.front();
                }
                foundNode = true;
                break;
            }else{
                // RED or node occupied → wait
                vehicle.stop();
                return;
            }
        }

        // ── NORMAL NODE ───────────────────────────────────────────
        if(nodes[i].occupied == 0 || nodes[i].occupied == vehicle.id){
            if(!nodes[i].ghostNode) nodes[i].occupied = vehicle.id;
            if(nodes[i].id == path){
                vehicle.prevNode = (vehicle.prevNode == -1) ? nodes[i].id : vehicle.node.id;
                vehicle.setNode(nodes[i]);
                vehicle.path.pop();
                if(!vehicle.path.empty()) path = vehicle.path.front();
            }
            foundNode = true;
            break;
        }else{
            // node occupied by someone else → wait
            vehicle.stop();
            return;
        }
    }

    //checking if car left the node, then node should no unoccupied
    for(int i = 0;i<nodes.size();i++){
        if(nodes[i].occupied == vehicle.id){
            if(!(abs(vehicle.getPosition().x - nodes[i].x) <= 70.0f && abs(vehicle.getPosition().y - nodes[i].y) <= 70.0f )){
                nodes[i].occupied = 0;

            }
        }
    }

    if(!foundNode){
        for(int i = 0;i<edges.size();i++){
            if(vehicle.getPosition().x >= edges[i].x1 && vehicle.getPosition().x <= edges[i].x2 && vehicle.getPosition().y >= edges[i].y1 && vehicle.getPosition().y <= edges[i].y2){

                    vehicle.setEdge(edges[i]);
                    foundEdge = true;
                    break;
                }

            }
    }


    if(foundNode){

        if(!vehicle.started){
            if(vehicle.node.ghostNode){
                if(vehicle.node.ghostStart == 'r'){
                    vehicle.rotateright();
                    vehicle.moveright();
                    vehicle.started = true;

                }else if(vehicle.node.ghostStart == 'd'){
                    vehicle.rotatedown();
                    vehicle.movedown();
                    vehicle.started = true;

                }else if(vehicle.node.ghostStart == 'u'){
                    vehicle.rotateup();
                    vehicle.moveup();
                    vehicle.started = true;

                }else if(vehicle.node.ghostStart == 'l'){
                    vehicle.rotateleft();
                    vehicle.moveleft();
                    vehicle.started = true;

                }
            }
        }

        else if(vehicle.prevNode == path){
            if(vehicle.node.downNode == path){
                if(abs(vehicle.getPosition().y - vehicle.node.downLeftx) <= tolerance || vehicle.uturnfirst == false){
                    vehicle.rotateright();
                    vehicle.moveright();
                    vehicle.uturnfirst = true;

                }else if(abs(vehicle.getPosition().x - vehicle.node.downRightx) <= tolerance){

                    vehicle.rotatedown();
                    vehicle.movedown();
                    vehicle.uturnfirst = false;
                }
                else{
                    vehicle.move();
                }
            }else if(vehicle.node.rightNode == path){
                if(abs(vehicle.getPosition().x - vehicle.node.downRightx) <= tolerance || vehicle.uturnfirst == false){

                    vehicle.rotateup();
                    vehicle.moveup();
                    vehicle.uturnfirst = true;

                }else if(abs(vehicle.getPosition().y - vehicle.node.topRighty) <= tolerance){

                    vehicle.rotateright();
                    vehicle.moveright();
                    vehicle.uturnfirst = false;
                }
                else{
                    vehicle.move();
                }
            }else if(vehicle.node.upNode == path){
                if(abs(vehicle.getPosition().y - vehicle.node.topRighty) <= tolerance || vehicle.uturnfirst == false){

                    vehicle.rotateleft();
                    vehicle.moveleft();
                    vehicle.uturnfirst = true;

                }else if(abs(vehicle.getPosition().x - vehicle.node.topLeftx) <= tolerance){

                    vehicle.rotateup();
                    vehicle.moveup();
                    vehicle.uturnfirst = false;
                }
                else{
                    vehicle.move();
                }
            }
            else if(vehicle.node.leftNode == path){
                if(abs(vehicle.getPosition().x - vehicle.node.topLeftx) <= tolerance || vehicle.uturnfirst == false){

                    vehicle.rotatedown();
                    vehicle.movedown();
                    vehicle.uturnfirst = true;
                }else if(abs(vehicle.getPosition().y - vehicle.node.downLefty) <= tolerance){

                    vehicle.rotateleft();
                    vehicle.moveleft();
                    vehicle.uturnfirst = false;
                }
                else{
                    vehicle.move();
                }
            }



        }else{
            if(vehicle.node.downNode == path){
                    if(abs(vehicle.getPosition().x - vehicle.node.topRightx) <= tolerance){
                        vehicle.rotatedown();
                        vehicle.movedown();
                    }else{
                        vehicle.move();
                    }

            }else if(vehicle.node.rightNode == path){
                if((abs(vehicle.getPosition().x - vehicle.node.topRightx) <= tolerance && abs(vehicle.getPosition().y - vehicle.node.topRighty) <= tolerance) || abs(vehicle.getPosition().y - vehicle.node.topLefty) <= tolerance){

                    vehicle.rotateright();
                    vehicle.moveright();
                }else{
                    vehicle.move();
                }
            }else if(vehicle.node.upNode == path){
                if(abs(vehicle.getPosition().x - vehicle.node.topLeftx) <= tolerance){
                    vehicle.rotateup();
                    vehicle.moveup();
                }else{
                    vehicle.move();
                }
            }
            else if(vehicle.node.leftNode == path){
                if(abs(vehicle.getPosition().y - vehicle.node.downLefty) <= tolerance){
                    vehicle.rotateleft();
                    vehicle.moveleft();
                }else{
                    vehicle.move();
                }
            }
        }

    }else if(foundEdge){
        if(vehicle.edge.rightNode == path){
            vehicle.rotateright();
            vehicle.moveright();
        }else if(vehicle.edge.downNode == path){
            vehicle.rotatedown();
            vehicle.movedown();
        }else if(vehicle.edge.upNode == path){
            vehicle.rotateup();
            vehicle.moveup();
        }else if(vehicle.edge.leftNode == path){
            vehicle.rotateleft();
            vehicle.moveleft();
        }


    }
    else{
        vehicle.move();
    }


}

void menuBackgroundTraffic(){
    vehicles.clear();
    vehicles.reserve(80);
    int id = 1;
    vehicles.emplace_back("car", id++).giveRandomPath();
    vehicles.emplace_back("car", id++).giveRandomPath();
    vehicles.emplace_back("car", id++).giveRandomPath();
    vehicles.emplace_back("car", id++).giveRandomPath();
    vehicles.emplace_back("car", id++).giveRandomPath();


    // taxis - second most common
    vehicles.emplace_back("taxi", id++).giveRandomPath();
    vehicles.emplace_back("taxi", id++).giveRandomPath();
    vehicles.emplace_back("taxi", id++).giveRandomPath();
    vehicles.emplace_back("taxi", id++).giveRandomPath();
    vehicles.emplace_back("taxi", id++).giveRandomPath();


    // police cars
    vehicles.emplace_back("policecar", id++).giveRandomPath();
    vehicles.emplace_back("policecar", id++).giveRandomPath();
    vehicles.emplace_back("policecar", id++).giveRandomPath();
    vehicles.emplace_back("policecar", id++).giveRandomPath();


    // ambulances - rare
    vehicles.emplace_back("ambulance", id++).giveRandomPath();
    vehicles.emplace_back("ambulance", id++).giveRandomPath();


    // firetrucks - rare

    vehicles.emplace_back("firetruck", id++).giveRandomPath();
    vehicles.emplace_back("firetruck", id++).giveRandomPath();

    // trucks
    // trucks
    vehicles.emplace_back("truck", id++).giveRandomPath();
    vehicles.emplace_back("truck", id++).giveRandomPath();

    vehicles.emplace_back("supercar", id++).giveRandomPath();
    vehicles.emplace_back("supercar", id++).giveRandomPath();
}

void screenSetup(){
    VideoMode desktop = VideoMode::getDesktopMode();
    window.create(desktop, "City Simulation", State::Fullscreen);
    window.setFramerateLimit(60);
    winSize = window.getSize(); // assign to global winSize
}

void loadImages(){
    mapTexture.loadFromFile("map.png");
    upRedSignalTexture.loadFromFile("RedSignal.png");
    upGreenSignalTexture.loadFromFile("GreenSignal.png");
    downRedSignalTexture.loadFromFile("RedSignal.png");
    downGreenSignalTexture.loadFromFile("GreenSignal.png");
    mosqueTexture.loadFromFile("mosque.png");
    hospitalTexture.loadFromFile("hospital.png");
    logo.loadFromFile("logo.png");
    background.loadFromFile("background.png");
    start.loadFromFile("start.png");
    quit.loadFromFile("quit.png");
    options.loadFromFile("options.png");
    back.loadFromFile("back.png");
    qasim.loadFromFile("qasim.png");
    bilal.loadFromFile("bilal.png");

}

bool loadingMap(){

    loadImages();
    handCursor = new Cursor(Cursor::createFromSystem(Cursor::Type::Hand).value());
    arrowCursor = new Cursor(Cursor::createFromSystem(Cursor::Type::Arrow).value());



    mosqueSprite = new Sprite(mosqueTexture);
    FloatRect bounds = mosqueSprite->getLocalBounds();
    mosqueSprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    mosqueSprite->setScale({1,1});
    mosqueSprite->setPosition({184.0f,155.0f});


    hospitalSprite = new Sprite(hospitalTexture);
    FloatRect bounds2 = hospitalSprite->getLocalBounds();
    hospitalSprite->setOrigin({bounds2.size.x / 2.0f, bounds2.size.y / 2.0f});
    hospitalSprite->setScale({1,1});
    hospitalSprite->setPosition({959.0f,141.0f});

    for(int i = 0; i < edges.size(); i++){
        edges[i].init();
    }



    Vector2u texSize = mapTexture.getSize();
    scaleX = (float)winSize.x / (float)texSize.x; // assign to globals
    scaleY = (float)winSize.y / (float)texSize.y;

    mapSprite = new Sprite(mapTexture); // correct syntax
    mapSprite->setScale({scaleX, scaleY});
    return true;
}

bool mainMenuBackgrounds(RenderWindow& window){
    if(gameStarted){
        menuBackgroundTraffic();
        gameStarted = false;

    }
    RectangleShape overlay(Vector2f(winSize.x,winSize.y));
    overlay.setFillColor(Color(0,0,0,150));

    backgroundSprite = new Sprite(background);
    FloatRect bound9 = backgroundSprite->getLocalBounds();
    backgroundSprite->setOrigin({bound9.size.x / 2.0f, bound9.size.y / 2.0f});
    backgroundSprite->setScale({0.9,1});
    backgroundSprite->setPosition({950,599});

    logoSprite = new Sprite(logo);
    FloatRect bound = logoSprite->getLocalBounds();
    logoSprite->setOrigin({bound.size.x / 2.0f, bound.size.y / 2.0f});
    logoSprite->setScale({0.4,0.4});
    logoSprite->setPosition({950,150});

    window.draw(overlay);
    window.draw(*backgroundSprite);
    window.draw(*logoSprite);

    return true;
}

bool startMenuButtons(RenderWindow& window){

    startSprite = new Sprite(start);
    FloatRect bound3 = startSprite->getLocalBounds();
    startSprite->setOrigin({bound3.size.x / 2.0f, bound3.size.y / 2.0f});
    startSprite->setScale({0.4,0.4});
    startSprite->setPosition({950,400});

    optionsSprite = new Sprite(options);
    FloatRect bound8 = optionsSprite->getLocalBounds();
    optionsSprite->setOrigin({bound8.size.x / 2.0f, bound8.size.y / 2.0f});
    optionsSprite->setScale({0.4,0.4});
    optionsSprite->setPosition({950,530});

    backSprite = new Sprite(back);
    FloatRect bound4 = backSprite->getLocalBounds();
    backSprite->setOrigin({bound4.size.x / 2.0f, bound4.size.y / 2.0f});
    backSprite->setScale({0.4,0.4});
    backSprite->setPosition({950,800});

    quitSprite = new Sprite(quit);
    FloatRect bound5 = quitSprite->getLocalBounds();
    quitSprite->setOrigin({bound5.size.x / 2.0f, bound5.size.y / 2.0f});
    quitSprite->setScale({0.4,0.4});
    quitSprite->setPosition({950,640});

    qasimSprite = new Sprite(qasim);
    FloatRect bound6 = qasimSprite->getLocalBounds();
    qasimSprite->setOrigin({bound6.size.x / 2.0f, bound6.size.y / 2.0f});
    qasimSprite->setScale({0.4,0.4});
    qasimSprite->setPosition({950,900});

    bilalSprite = new Sprite(bilal);
    FloatRect bound7 = bilalSprite->getLocalBounds();
    bilalSprite->setOrigin({bound7.size.x / 2.0f, bound7.size.y / 2.0f});
    bilalSprite->setScale({0.4,0.4});
    bilalSprite->setPosition({950,1020});





    Vector2i mousepos = Mouse::getPosition(window);
    if(abs(startSprite->getPosition().x - mousepos.x) <= 100 && abs(startSprite->getPosition().y - mousepos.y) <= 50){
        startSprite->setPosition({950,395});
        window.setMouseCursor(*handCursor);
    }else{
        startSprite->setPosition({950,400});
        window.setMouseCursor(*arrowCursor);
    }

    if(abs(optionsSprite->getPosition().x - mousepos.x) <= 100 && abs(optionsSprite->getPosition().y - mousepos.y) <= 50){
        optionsSprite->setPosition({950,525});
        window.setMouseCursor(*handCursor);
    }else{
        optionsSprite->setPosition({950,530});
        window.setMouseCursor(*arrowCursor);
    }

    if(abs(quitSprite->getPosition().x - mousepos.x) <= 100 && abs(quitSprite->getPosition().y - mousepos.y) <= 50){
        quitSprite->setPosition({950,635});
        window.setMouseCursor(*handCursor);
    }else{
        quitSprite->setPosition({950,640});
        window.setMouseCursor(*arrowCursor);
    }




    window.draw(*startSprite);
    window.draw(*optionsSprite);
    window.draw(*quitSprite);
    window.draw(*qasimSprite);
    window.draw(*bilalSprite);
    return true;
}

void chooseTrafficButtons(RenderWindow& window){

    traffic.setPosition(950,150);
    traffic.setScale(0.4,0.4);


    backButton.setPosition(950,950);
    backButton.setScale(0.5,0.5);


    highButton.setPosition(950,400);
    highButton.setScale(0.17,0.17);


    mediumButton.setPosition(950,550);
    mediumButton.setScale(0.17,0.17);


    lowButton.setPosition(950,700);
    lowButton.setScale(0.17,0.17);



    Vector2i mousepos = Mouse::getPosition(window);
    if(abs(backButton.getPosition().x - mousepos.x) <= 100 && abs(backButton.getPosition().y - mousepos.y) <= 50){
        backButton.setPosition(950,945);
    }else{
        backButton.setPosition(950,950);
    }

    if(abs(highButton.getPosition().x - mousepos.x) <= 100 && abs(highButton.getPosition().y - mousepos.y) <= 50){
        highButton.setPosition(950,395);
    }else{
        highButton.setPosition(950,400);
    }

    if(abs(mediumButton.getPosition().x - mousepos.x) <= 100 && abs(mediumButton.getPosition().y - mousepos.y) <= 50){
        mediumButton.setPosition(950,545);
    }else{
        mediumButton.setPosition(950,550);
    }

    if(abs(lowButton.getPosition().x - mousepos.x) <= 100 && abs(lowButton.getPosition().y - mousepos.y) <= 50){
        lowButton.setPosition(950,695);
    }else{
        lowButton.setPosition(950,700);
    }





    mediumButton.show();
    highButton.show();
    lowButton.show();
    backButton.show();
    traffic.show();

}

void chooseCarMenuButtons(RenderWindow& window){

    chooseCarButtons.clear();

    chooseCarLogo.setPosition(950,150);
    chooseCarLogo.setScale(0.4,0.4);


    backButton.setPosition(950,1030);
    backButton.setScale(0.3,0.3);
    chooseCarButtons.push_back(backButton);

    float xsize = 0.12;
    float ysize = 0.12;

    superCarButton.setPosition(950,320);
    superCarButton.setScale(xsize,ysize);
    chooseCarButtons.push_back(superCarButton);

    normalCarButton.setPosition(950,420);
    normalCarButton.setScale(xsize,ysize);
    chooseCarButtons.push_back(normalCarButton);

    taxiButton.setPosition(950,520);
    taxiButton.setScale(xsize,ysize);
    chooseCarButtons.push_back(taxiButton);

    policeCarButton.setPosition(950,620);
    policeCarButton.setScale(xsize,ysize);
    chooseCarButtons.push_back(policeCarButton);

    ambulanceButton.setPosition(950,720);
    ambulanceButton.setScale(xsize,ysize);
    chooseCarButtons.push_back(ambulanceButton);

    firetruckButton.setPosition(950,820);
    firetruckButton.setScale(xsize,ysize);
    chooseCarButtons.push_back(firetruckButton);

    truckButton.setPosition(950,920);
    truckButton.setScale(xsize,ysize);
    chooseCarButtons.push_back(truckButton);


    Vector2i mousepos = Mouse::getPosition(window);
    for(int i = 0;i<chooseCarButtons.size();i++){
        if(abs(chooseCarButtons[i].getPosition().x - mousepos.x) <= 40 && abs(chooseCarButtons[i].getPosition().y - mousepos.y) <= 20){
            chooseCarButtons[i].setTempPosition(chooseCarButtons[i].originalx,chooseCarButtons[i].originaly - 5);
        }else{
            chooseCarButtons[i].setPosition(chooseCarButtons[i].originalx,chooseCarButtons[i].originaly);
        }
    }




    superCarButton.show();
    normalCarButton.show();
    policeCarButton.show();
    firetruckButton.show();
    ambulanceButton.show();
    taxiButton.show();
    truckButton.show();
    backButton.show();
    chooseCarLogo.show();
}


void startMenuClickHandle(Vector2i clickpos,RenderWindow& window){
    if(abs(startSprite->getPosition().x - clickpos.x) <= 100 && abs(startSprite->getPosition().y - clickpos.y) <= 50){
        currentState = CHOOSETRAFFIC;
        cout<<"Its working";
    }else if(abs(quitSprite->getPosition().x - clickpos.x) <= 100 && abs(quitSprite->getPosition().y - clickpos.y) <= 50){
        window.close();
    }
}

void trafficClickHandle(Vector2i clickpos,RenderWindow& window){
    if(abs(highButton.getPosition().x - clickpos.x) <= 100 && abs(highButton.getPosition().y - clickpos.y) <= 50){
        trafficStrength = "high";
        currentState = CHOOSECAR;
    }else if(abs(mediumButton.getPosition().x - clickpos.x) <= 100 && abs(mediumButton.getPosition().y - clickpos.y) <= 50){
        trafficStrength = "medium";
        currentState = CHOOSECAR;
    }else if(abs(lowButton.getPosition().x - clickpos.x) <= 100 && abs(lowButton.getPosition().y - clickpos.y) <= 50){
        trafficStrength = "low";
        currentState = CHOOSECAR;
    }
    else if(abs(backButton.getPosition().x - clickpos.x) <= 100 && abs(backButton.getPosition().y - clickpos.y) <= 50){
        currentState = MENU;
    }
}

void chooseCarClickHandle(Vector2i clickpos, RenderWindow& window){
    for(int i = 0;i<chooseCarButtons.size();i++){
        if(abs(chooseCarButtons[i].getPosition().x - clickpos.x) <= 100 && abs(chooseCarButtons[i].getPosition().y - clickpos.y) <= 50){
            if(chooseCarButtons[i].name == "back"){
                currentState = CHOOSETRAFFIC;
                return;
            }
            chooseCar = chooseCarButtons[i].name;
            currentState = PLAYING;
        }
    }
}



void BFS(Vehicle& car){

    Edge e = car.edge;
    vector<int> reverse;
    vector<int> reverse2;
    vector<int> reverse3;
    vector<int> reverse4;
    queue<int> todo;
    int parent[100];
    int discovered[100];

    int nearfirstnode = 0;
    int nearsecondnode = 0;
    bool firstFound = false;

    int parentparent = 0;

    int prev = 0;






    if(personalDestination == "hospital"){
        nearfirstnode = 6;
        nearsecondnode = 7;
    }else if(personalDestination == "mosque"){
        nearfirstnode = 5;
        nearsecondnode = 27;
    }else if(personalDestination == "policestation"){
        nearfirstnode = 10;
        nearsecondnode = 11;
    }else if(personalDestination == "pump"){
        nearfirstnode = 12;
        nearsecondnode = 8;
    }else if(personalDestination == "rightbottomhouse"){
        nearfirstnode = 35;
        nearsecondnode = 20;
    }else if(personalDestination == "leftground"){
        nearfirstnode = 36;
        nearsecondnode = 10;
    }else if(personalDestination == "park"){
        nearfirstnode = 5;
        nearsecondnode = 27;
    }else if(personalDestination == "bank"){
        nearfirstnode = 11;
        nearsecondnode = 12;
    }else if(personalDestination == "grave"){
        nearfirstnode = 7;
        nearsecondnode = 8;
    }else if(personalDestination == "resturant"){
        nearfirstnode = 9;
        nearsecondnode = 8;
    }else if(personalDestination == "school"){
        nearfirstnode = 12;
        nearsecondnode = 18;
    }else if(personalDestination == "market"){
        nearfirstnode = 16;
        nearsecondnode = 17;
    }


        if(e.hasUp && !findNode(e.upNode).ghostNode){

             for(int i = 0;i<100;i++){
                parent[i] = 0;
                discovered[i] = false;
            }

            todo.push(e.upNode);
            discovered[e.upNode] = true;
            parent[e.upNode] = -1;

            prev = 0;

            firstFound = false;

            parentparent = 0;

            while(!todo.empty()){

                prev = todo.front();


                if(prev == nearfirstnode){
                    todo.push(nearsecondnode);
                    parent[nearsecondnode] = prev;
                    discovered[nearsecondnode] = true;
                    firstFound = true;
                    break;
                }else if(prev == nearsecondnode){
                    todo.push(nearfirstnode);
                    parent[nearfirstnode] = prev;
                    discovered[nearfirstnode] = true;

                    break;
                }

                Node n = findNode(prev);
                todo.pop();


                if(n.hasUp && !discovered[n.upNode]){
                    todo.push(n.upNode);
                    parent[n.upNode] = prev;
                    discovered[n.upNode] = true;
                }
                if(n.hasDown && !discovered[n.downNode]){
                    todo.push(n.downNode);
                    parent[n.downNode] = prev;
                    discovered[n.downNode] = true;
                }
                if(n.hasLeft && !discovered[n.leftNode]){
                    todo.push(n.leftNode);
                    parent[n.leftNode] = prev;
                    discovered[n.leftNode] = true;
                }
                if(n.hasRight && !discovered[n.rightNode]){
                    todo.push(n.rightNode);
                    parent[n.rightNode] = prev;
                    discovered[n.rightNode] = true;
                }
            }



            if(firstFound){
                parentparent = nearsecondnode;
            }else{
                parentparent = nearfirstnode;
            }
            reverse.push_back(parentparent);

            while(true){
                parentparent = parent[parentparent];
                if(parentparent == -1){
                    break;
                }

                reverse.push_back(parentparent);
            }
            while(!todo.empty()){
                todo.pop();
            }

        }if(e.hasDown && !findNode(e.downNode).ghostNode){

             for(int i = 0;i<100;i++){
                parent[i] = 0;
                discovered[i] = false;
            }

            todo.push(e.downNode);
            discovered[e.downNode] = true;
            parent[e.downNode] = -1;


            prev = 0;

            firstFound = false;

            parentparent = 0;

            while(!todo.empty()){

                prev = todo.front();


                if(prev == nearfirstnode){
                    todo.push(nearsecondnode);
                    parent[nearsecondnode] = prev;
                    discovered[nearsecondnode] = true;
                    firstFound = true;
                    break;
                }else if(prev == nearsecondnode){
                    todo.push(nearfirstnode);
                    parent[nearfirstnode] = prev;
                    discovered[nearfirstnode] = true;

                    break;
                }

                Node n = findNode(prev);
                todo.pop();


                if(n.hasUp && !discovered[n.upNode]){
                    todo.push(n.upNode);
                    parent[n.upNode] = prev;
                    discovered[n.upNode] = true;
                }
                if(n.hasDown && !discovered[n.downNode]){
                    todo.push(n.downNode);
                    parent[n.downNode] = prev;
                    discovered[n.downNode] = true;
                }
                if(n.hasLeft && !discovered[n.leftNode]){
                    todo.push(n.leftNode);
                    parent[n.leftNode] = prev;
                    discovered[n.leftNode] = true;
                }
                if(n.hasRight && !discovered[n.rightNode]){
                    todo.push(n.rightNode);
                    parent[n.rightNode] = prev;
                    discovered[n.rightNode] = true;
                }
            }



            if(firstFound){
                parentparent = nearsecondnode;
            }else{
                parentparent = nearfirstnode;
            }
            reverse2.push_back(parentparent);

            while(true){
                parentparent = parent[parentparent];
                if(parentparent == -1){
                    break;
                }

                reverse2.push_back(parentparent);
            }
            while(!todo.empty()){
                todo.pop();
            }
        }if(e.hasLeft && !findNode(e.leftNode).ghostNode){

             for(int i = 0;i<100;i++){
                parent[i] = 0;
                discovered[i] = false;
            }

            todo.push(e.leftNode);
            discovered[e.leftNode] = true;
            parent[e.leftNode] = -1;


            prev = 0;

            firstFound = false;

            parentparent = 0;

            while(!todo.empty()){

                prev = todo.front();


                if(prev == nearfirstnode){
                    todo.push(nearsecondnode);
                    parent[nearsecondnode] = prev;
                    discovered[nearsecondnode] = true;
                    firstFound = true;
                    break;
                }else if(prev == nearsecondnode){
                    todo.push(nearfirstnode);
                    parent[nearfirstnode] = prev;
                    discovered[nearfirstnode] = true;

                    break;
                }

                Node n = findNode(prev);
                todo.pop();


                if(n.hasUp && !discovered[n.upNode]){
                    todo.push(n.upNode);
                    parent[n.upNode] = prev;
                    discovered[n.upNode] = true;
                }
                if(n.hasDown && !discovered[n.downNode]){
                    todo.push(n.downNode);
                    parent[n.downNode] = prev;
                    discovered[n.downNode] = true;
                }
                if(n.hasLeft && !discovered[n.leftNode]){
                    todo.push(n.leftNode);
                    parent[n.leftNode] = prev;
                    discovered[n.leftNode] = true;
                }
                if(n.hasRight && !discovered[n.rightNode]){
                    todo.push(n.rightNode);
                    parent[n.rightNode] = prev;
                    discovered[n.rightNode] = true;
                }
            }



            if(firstFound){
                parentparent = nearsecondnode;
            }else{
                parentparent = nearfirstnode;
            }
            reverse3.push_back(parentparent);

            while(true){
                parentparent = parent[parentparent];
                if(parentparent == -1){
                    break;
                }

                reverse3.push_back(parentparent);
            }
            while(!todo.empty()){
                todo.pop();
            }
        }
        if(e.hasRight && !findNode(e.rightNode).ghostNode){

             for(int i = 0;i<100;i++){
                parent[i] = 0;
                discovered[i] = false;
            }

            todo.push(e.rightNode);
            discovered[e.rightNode] = true;
            parent[e.rightNode] = -1;


            prev = 0;

            firstFound = false;

            parentparent = 0;

            while(!todo.empty()){

                prev = todo.front();


                if(prev == nearfirstnode){
                    todo.push(nearsecondnode);
                    parent[nearsecondnode] = prev;
                    discovered[nearsecondnode] = true;
                    firstFound = true;
                    break;
                }else if(prev == nearsecondnode){
                    todo.push(nearfirstnode);
                    parent[nearfirstnode] = prev;
                    discovered[nearfirstnode] = true;

                    break;
                }

                Node n = findNode(prev);
                todo.pop();


                if(n.hasUp && !discovered[n.upNode]){
                    todo.push(n.upNode);
                    parent[n.upNode] = prev;
                    discovered[n.upNode] = true;
                }
                if(n.hasDown && !discovered[n.downNode]){
                    todo.push(n.downNode);
                    parent[n.downNode] = prev;
                    discovered[n.downNode] = true;
                }
                if(n.hasLeft && !discovered[n.leftNode]){
                    todo.push(n.leftNode);
                    parent[n.leftNode] = prev;
                    discovered[n.leftNode] = true;
                }
                if(n.hasRight && !discovered[n.rightNode]){
                    todo.push(n.rightNode);
                    parent[n.rightNode] = prev;
                    discovered[n.rightNode] = true;
                }
            }



            if(firstFound){
                parentparent = nearsecondnode;
            }else{
                parentparent = nearfirstnode;
            }
            reverse4.push_back(parentparent);

            while(true){
                parentparent = parent[parentparent];
                if(parentparent == -1){
                    break;
                }

                reverse4.push_back(parentparent);
            }
            while(!todo.empty()){
                todo.pop();
            }
        }


    if(reverse.size() != 0 && (reverse2.size() == 0 || reverse.size() <= reverse2.size())){
        for(int i = reverse.size()-1;i>=0;i--){
            car.path.push(reverse[i]);
            cout<<reverse[i]<<" ";
        }
    }else if(reverse2.size() != 0 && (reverse.size() == 0 || reverse2.size() <= reverse.size())){
        for(int i = reverse2.size()-1;i>=0;i--){
            car.path.push(reverse2[i]);
            cout<<reverse2[i]<<" ";
        }
    }else if(reverse3.size() != 0 && (reverse4.size() == 0 || reverse3.size() <= reverse4.size())){
        for(int i = reverse3.size()-1;i>=0;i--){
            car.path.push(reverse3[i]);
            cout<<reverse3[i]<<" ";
        }
    }else if(reverse4.size() != 0 && (reverse3.size() == 0 || reverse4.size() <= reverse3.size())){
        for(int i = reverse4.size()-1;i>=0;i--){
            car.path.push(reverse4[i]);
            cout<<reverse4[i]<<" ";
        }
    }



}

void handlePersonalVehicle(Vehicle& car){

    


    if(personalDestination == "hospital"){//going to hospital
        if(abs(car.getPosition().x - 1000) < 20.0f && abs(car.getPosition().y - 260) < 20.0f){
            car.stop();
            car.currentDestination = "hospital";
            while(!car.path.empty()){
                car.path.pop();
            }
            for(int i = 0; i < nodes.size(); i++) {
                if (nodes[i].occupied == car.id) {
                    nodes[i].occupied = 0;
                }
            }

        }
        else if(abs(car.getPosition().x - 1000) < 10.0f && abs(car.getPosition().y - 349) < 120.0f){
            car.personallyControlled = true;
            car.rotateup();
            car.moveup();
        }

    }else if(personalDestination == "mosque"){//going to mosque
        if(abs(car.getPosition().x - 190) < 20.0f && abs(car.getPosition().y - 270) < 20.0f){
            car.stop();
            car.currentDestination = "mosque";
            while(!car.path.empty()){
                car.path.pop();
            }
            for(int i = 0; i < nodes.size(); i++) {
                if (nodes[i].occupied == car.id) {
                    nodes[i].occupied = 0;
                }
            }
        }
        else if(abs(car.getPosition().x - 190) < 10.0f && abs(car.getPosition().y - 337) < 100.0f){
            car.personallyControlled = true;
            car.rotateup();
            car.moveup();
        }

    }else if(personalDestination == "pump"){//going to pump
        if(abs(car.getPosition().x - 1495) < 20.0f && abs(car.getPosition().y - 495) < 20.0f){
            car.stop();
            car.currentDestination = "pump";
            while(!car.path.empty()){
                car.path.pop();
            }
            for(int i = 0; i < nodes.size(); i++) {
                if (nodes[i].occupied == car.id) {
                    nodes[i].occupied = 0;
                }
            }

        }
        else if(abs(car.getPosition().x - 1431) < 70.0f && abs(car.getPosition().y - 495) < 10.0f){
            car.personallyControlled = true;
            car.rotateright();
            car.moveright();
        }

    }else if(personalDestination == "policestation"){//going to POLICE STATION
        if(abs(car.getPosition().x - 957) < 20.0f && abs(car.getPosition().y - 560) < 20.0f){
            car.stop();
            car.currentDestination = "policestation";
            while(!car.path.empty()){
                car.path.pop();
            }
            for(int i = 0; i < nodes.size(); i++) {
                if (nodes[i].occupied == car.id) {
                    nodes[i].occupied = 0;
                }
            }
        }
        else if(abs(car.getPosition().x - 957) < 10.0f && abs(car.getPosition().y - 620) < 70.0f){
            car.personallyControlled = true;
            car.rotateup();
            car.moveup();
        }

    }else if(personalDestination == "rightbottomhouse"){
        if(abs(car.getPosition().x - 1725) < 20.0f && abs(car.getPosition().y - 730) < 20.0f){
            car.stop();
            car.currentDestination = "rightbottomhouse";
            while(!car.path.empty()){
                car.path.pop();
            }
            for(int i = 0; i < nodes.size(); i++) {
                if (nodes[i].occupied == car.id) {
                    nodes[i].occupied = 0;
                }
            }
        }
        else if(abs(car.getPosition().x - 1720) < 10.0f && abs(car.getPosition().y - 792) < 70.0f){
            car.personallyControlled = true;
            car.rotateup();
            car.moveup();
        }

    }else if(personalDestination == "leftground"){
        if(abs(car.getPosition().x - 571) < 20.0f && abs(car.getPosition().y - 741) < 20.0f){
            car.stop();
            car.currentDestination = "leftground";
            while(!car.path.empty()){
                car.path.pop();
            }
            for(int i = 0; i < nodes.size(); i++) {
                if (nodes[i].occupied == car.id) {
                    nodes[i].occupied = 0;
                }
            }
        }
        else if(abs(car.getPosition().x - 571) < 10.0f && abs(car.getPosition().y - 660) < 70.0f){
            car.personallyControlled = true;
            car.rotatedown();
            car.movedown();
        }

    }else if(personalDestination == "bank"){
        if(abs(car.getPosition().x - 1249) < 20.0f && abs(car.getPosition().y - 562) < 20.0f){
            car.stop();
            car.currentDestination = "bank";
            while(!car.path.empty()){
                car.path.pop();
            }
            for(int i = 0; i < nodes.size(); i++) {
                if (nodes[i].occupied == car.id) {
                    nodes[i].occupied = 0;
                }
            }
        }
        else if(abs(car.getPosition().x - 1249) < 10.0f && abs(car.getPosition().y - 624) < 70.0f){
            car.personallyControlled = true;
            car.rotateup();
            car.moveup();
        }

    }
    else if(personalDestination == "park"){
        if(abs(car.getPosition().x - 290) < 20.0f && abs(car.getPosition().y - 500) < 20.0f){
            car.stop();
            car.currentDestination = "park";
            while(!car.path.empty()){
                car.path.pop();
            }
            for(int i = 0; i < nodes.size(); i++) {
                if (nodes[i].occupied == car.id) {
                    nodes[i].occupied = 0;
                }
            }
            
        }
        else if(abs(car.getPosition().x - 290) < 10.0f && abs(car.getPosition().y - 382) < 120.0f){
            car.personallyControlled = true;
            car.rotatedown();
            car.movedown();
        }

    }else if(personalDestination == "grave"){
        if(abs(car.getPosition().x - 1249) < 20.0f && abs(car.getPosition().y - 280) < 20.0f){
            car.stop();
            car.currentDestination = "grave";
            while(!car.path.empty()){
                car.path.pop();
            }
            for(int i = 0; i < nodes.size(); i++) {
                if (nodes[i].occupied == car.id) {
                    nodes[i].occupied = 0;
                }
            }
        }
        else if(abs(car.getPosition().x - 1249) < 10.0f && abs(car.getPosition().y - 348) < 70.0f){
            car.personallyControlled = true;
            car.rotateup();
            car.moveup();
        }

    }else if(personalDestination == "resturant"){
        if(abs(car.getPosition().x - 1500) < 20.0f && abs(car.getPosition().y - 240) < 20.0f){
            car.stop();
            car.currentDestination = "resturant";
            while(!car.path.empty()){
                car.path.pop();
            }
            for(int i = 0; i < nodes.size(); i++) {
                if (nodes[i].occupied == car.id) {
                    nodes[i].occupied = 0;
                }
            }
        }
        else if(abs(car.getPosition().x - 1500) < 10.0f && abs(car.getPosition().y - 348) < 120.0f){
            car.personallyControlled = true;
            car.rotateup();
            car.moveup();
        }

    }else if(personalDestination == "school"){
        if(abs(car.getPosition().x - 1315) < 20.0f && abs(car.getPosition().y - 810) < 20.0f){
            car.stop();
            car.currentDestination = "school";
            while(!car.path.empty()){
                car.path.pop();
            }
            for(int i = 0; i < nodes.size(); i++) {
                if (nodes[i].occupied == car.id) {
                    nodes[i].occupied = 0;
                }
            }
        }
        else if(abs(car.getPosition().x - 1427) < 120.0f && abs(car.getPosition().y - 810) < 10.0f){
            car.personallyControlled = true;
            car.rotateleft();
            car.moveleft();
        }

    }else if(personalDestination == "market"){
        if(abs(car.getPosition().x - 815) < 20.0f && abs(car.getPosition().y - 714) < 20.0f){
            car.stop();
            car.currentDestination = "market";
            while(!car.path.empty()){
                car.path.pop();
            }
            for(int i = 0; i < nodes.size(); i++) {
                if (nodes[i].occupied == car.id) {
                    nodes[i].occupied = 0;
                }
            }
        }
        else if(abs(car.getPosition().x - 815) < 5.0f && abs(car.getPosition().y - 947) < 220.0f){
            car.personallyControlled = true;
            car.rotateup();
            car.moveup();
        }

    }

    






    if(personalDestination != car.currentDestination){
        if(car.currentDestination == "pump"){

            if(abs(car.getPosition().x - 1425) < 5.0f){
               if(car.bfsdone == false){
                    BFS(car);
               }

               car.bfsdone = true;
               if(car.path.front() == 12){
                    car.personallyControlled = false;
                    car.currentDestination = personalDestination;
                    car.bfsdone = false;
               }else{
                car.rotateleft();
                car.moveleft();
               }

            }else{
                car.rotateleft();
                car.moveleft();
                if(abs(car.getPosition().x - 1393) < 5.0f && car.bfsdone){
                        car.personallyControlled = false;
                        car.currentDestination = personalDestination;
                        car.bfsdone = false;
                }

            }

        }else if(car.currentDestination == "hospital"){

            if(abs(car.getPosition().y - 350) < 5.0f){
               if(car.bfsdone == false){
                    BFS(car);
               }

               car.bfsdone = true;
               if(car.path.front() == 7){
                    car.personallyControlled = false;
                    car.currentDestination = personalDestination;
                    car.bfsdone = false;
               }else{
                car.rotatedown();
                car.movedown();
               }

            }else{
                car.rotatedown();
                car.movedown();
                if(abs(car.getPosition().y - 380) < 5.0f && car.bfsdone){
                        car.personallyControlled = false;
                        car.currentDestination = personalDestination;
                        car.bfsdone = false;
                }

            }

        }else if(car.currentDestination == "mosque"){
            if(abs(car.getPosition().y - 349) < 5.0f){
               BFS(car);
               car.personallyControlled = false;
               car.currentDestination = personalDestination;
            }else{
                car.rotatedown();
                car.movedown();
            }
        }else if(car.currentDestination == "rightbottomhouse"){
            if(abs(car.getPosition().y - 792) < 5.0f){
               BFS(car);
               car.personallyControlled = false;
               car.currentDestination = personalDestination;
            }else{
                car.rotatedown();
                car.movedown();
            }
        }else if(car.currentDestination == "park"){

            if(abs(car.getPosition().y - 381) < 5.0f){
               if(car.bfsdone == false){
                car.setEdge(*findEdge(13));
                    BFS(car);
                    if (!car.path.empty() && car.path.front() == 5 && personalDestination == "mosque") {
                        car.path.pop();
                    }
               }

               car.bfsdone = true;
               if(car.path.front() == 27){
                    car.personallyControlled = false;
                    car.currentDestination = personalDestination;
                    car.bfsdone = false;
               }else{
                car.rotateup();
                car.moveup();
               }

            }else{
                car.rotateup();
                car.moveup();
                if(abs(car.getPosition().y - 350) < 5.0f && car.bfsdone){
                        car.personallyControlled = false;
                        car.currentDestination = personalDestination;
                        car.bfsdone = false;
                }

            }

        }
        else if(car.currentDestination == "policestation"){

            if(abs(car.getPosition().y - 626) < 5.0f){
               if(car.bfsdone == false){
                    BFS(car);
               }

               car.bfsdone = true;
               if(car.path.front() == 11){
                    car.personallyControlled = false;
                    car.currentDestination = personalDestination;
                    car.bfsdone = false;
               }else{
                car.rotatedown();
                car.movedown();
               }

            }else{
                car.rotatedown();
                car.movedown();
                if(abs(car.getPosition().y - 655) < 5.0f && car.bfsdone){
                        car.personallyControlled = false;
                        car.currentDestination = personalDestination;
                        car.bfsdone = false;
                }

            }
        }else if(car.currentDestination == "leftground"){

            if(abs(car.getPosition().y - 658) < 5.0f){
               if(car.bfsdone == false){
                    BFS(car);
               }

               car.bfsdone = true;
               if(car.path.front() == 36){
                    car.personallyControlled = false;
                    car.currentDestination = personalDestination;
                    car.bfsdone = false;
               }else{
                car.rotateup();
                car.moveup();
               }

            }else{
                car.rotateup();
                car.moveup();
                if(abs(car.getPosition().y - 627) < 5.0f && car.bfsdone){
                        car.personallyControlled = false;
                        car.currentDestination = personalDestination;
                        car.bfsdone = false;
                }

            }
        }else if(car.currentDestination == "bank"){

            if(abs(car.getPosition().y - 626) < 5.0f){
               if(car.bfsdone == false){
                    BFS(car);
               }

               car.bfsdone = true;
               if(car.path.front() == 12){
                    car.personallyControlled = false;
                    car.currentDestination = personalDestination;
                    car.bfsdone = false;
               }else{
                car.rotatedown();
                car.movedown();
               }

            }else{
                car.rotatedown();
                car.movedown();
                if(abs(car.getPosition().y - 657) < 5.0f && car.bfsdone){
                        car.personallyControlled = false;
                        car.currentDestination = personalDestination;
                        car.bfsdone = false;
                }

            }
        }else if(car.currentDestination == "grave"){

            if(abs(car.getPosition().y - 352) < 5.0f){
               if(car.bfsdone == false){
                    BFS(car);
               }

               car.bfsdone = true;
               if(car.path.front() == 8){
                    car.personallyControlled = false;
                    car.currentDestination = personalDestination;
                    car.bfsdone = false;
               }else{
                car.rotatedown();
                car.movedown();
               }

            }else{
                car.rotatedown();
                car.movedown();
                if(abs(car.getPosition().y - 382) < 5.0f && car.bfsdone){
                        car.personallyControlled = false;
                        car.currentDestination = personalDestination;
                        car.bfsdone = false;
                }

            }
        }else if(car.currentDestination == "resturant"){

            if(abs(car.getPosition().y - 352) < 5.0f){
               if(car.bfsdone == false){
                    BFS(car);
               }

               car.bfsdone = true;
               if(car.path.front() == 9){
                    car.personallyControlled = false;
                    car.currentDestination = personalDestination;
                    car.bfsdone = false;
               }else{
                car.rotatedown();
                car.movedown();
               }

            }else{
                car.rotatedown();
                car.movedown();
                if(abs(car.getPosition().y - 384) < 5.0f && car.bfsdone){
                        car.personallyControlled = false;
                        car.currentDestination = personalDestination;
                        car.bfsdone = false;
                }

            }
        }else if(car.currentDestination == "school"){

            if(abs(car.getPosition().x - 1394) < 5.0f){
               if(car.bfsdone == false){
                    BFS(car);
               }

               car.bfsdone = true;
               if(car.path.front() == 12){
                    car.personallyControlled = false;
                    car.currentDestination = personalDestination;
                    car.bfsdone = false;
               }else{
                car.rotateright();
                car.moveright();
               }

            }else{
                car.rotateright();
                car.moveright();
                if(abs(car.getPosition().x - 1427) < 5.0f && car.bfsdone){
                        car.personallyControlled = false;
                        car.currentDestination = personalDestination;
                        car.bfsdone = false;
                }

            }
        }else if(car.currentDestination == "market"){

            if(abs(car.getPosition().y - 948) < 5.0f){
               if(car.bfsdone == false){
                    BFS(car);
               }

               car.bfsdone = true;
               if(car.path.front() == 17){
                    car.personallyControlled = false;
                    car.currentDestination = personalDestination;
                    car.bfsdone = false;
               }else{
                car.rotatedown();
                car.movedown();
               }

            }else{
                car.rotatedown();
                car.movedown();
                if(abs(car.getPosition().y - 980) < 5.0f && car.bfsdone){
                        car.personallyControlled = false;
                        car.currentDestination = personalDestination;
                        car.bfsdone = false;
                }

            }
        }

    //checking if there is vehicle ahead of personal vehicle then we will not drive it instead stop
    float space = 80;
    for(int i = 0;i<vehicles.size();i++){
        if(car.id == vehicles[i].id){
            continue;
        }
        if(car.rotation == 180.0f){
            if(vehicles[i].getPosition().x > car.getPosition().x && vehicles[i].getPosition().x < car.getPosition().x+space && vehicles[i].getPosition().y < car.getPosition().y + 10 && vehicles[i].getPosition().y > car.getPosition().y - 10){
                car.stop();
                return;
            }
        }else if(car.rotation == -90.0f){
            if(vehicles[i].getPosition().y > car.getPosition().y && vehicles[i].getPosition().y < car.getPosition().y+space && vehicles[i].getPosition().x < car.getPosition().x + 10 && vehicles[i].getPosition().x > car.getPosition().x - 10){
                car.stop();
                return;
            }
        }else if(car.rotation == 90.0f){
            if(vehicles[i].getPosition().y < car.getPosition().y && vehicles[i].getPosition().y > car.getPosition().y-space && vehicles[i].getPosition().x < car.getPosition().x + 10 && vehicles[i].getPosition().x > car.getPosition().x - 10){
                car.stop();
                return;
            }
        }
        else if(car.rotation == 0.0f){
            if(vehicles[i].getPosition().x < car.getPosition().x && vehicles[i].getPosition().x > car.getPosition().x-space && vehicles[i].getPosition().y < car.getPosition().y + 10 && vehicles[i].getPosition().y > car.getPosition().y - 10){
                car.stop();
                return;
            }
        }
    }
    }


}

void chooseBuilding(RenderWindow& window){



    Vector2i mousepos = Mouse::getPosition(window);
    bool onBuilding = false;
    for(int i = 0;i<buildings.size();i++){
        if(mousepos.x >= buildings[i].x1 && mousepos.x <= buildings[i].x2 && mousepos.y >= buildings[i].y1 && mousepos.y <= buildings[i].y2){
            onBuilding = true;
            break;
        }
    }
    if(onBuilding){
        window.setMouseCursor(*handCursor);
    }else{
        window.setMouseCursor(*arrowCursor);
    }



    //adjusting back button to show on map here
    backSprite->setPosition({1830,1015});
    backSprite->setScale({0.35,0.3});

    if(abs(backSprite->getPosition().x - mousepos.x) <= 70 && abs(backSprite->getPosition().y - mousepos.y) <= 30){
        backSprite->setPosition({1830,1010});
    }else{
        backSprite->setPosition({1830,1015});
    }

    window.draw(*backSprite);
}

void handleClick(Vector2i clickpos){



    for(int i = 0;i<buildings.size();i++){
        if(clickpos.x >= buildings[i].x1 && clickpos.x <= buildings[i].x2 && clickpos.y >= buildings[i].y1 && clickpos.y <= buildings[i].y2){
            personalDestination = buildings[i].name;
            break;
        }
    }

    if(abs(backSprite->getPosition().x - clickpos.x) <= 70 && abs(backSprite->getPosition().y - clickpos.y) <= 30){
        currentState = MENU;
    }

}




void startGame(){
    vehicles.clear();
    vehicles.reserve(80);
    int id = 1;

    if(chooseCar == "superCar"){
        vehicles.emplace_back("supercar", 51);
    }else if(chooseCar == "normalCar"){
        vehicles.emplace_back("car", 51);
    }else if(chooseCar == "policeCar"){
        vehicles.emplace_back("policecar", 51);
    }else{
        vehicles.emplace_back(chooseCar, 51);
    }

    auto& personal = vehicles.back();
    personal.setPosition(957, 560);
    personal.rotatedown();
    personal.currentDestination = "policestation";
    personalDestination = "policestation";
    personal.personallyControlled = true;
    personal.setEdge(*findEdge(24));
    personal.prevNode = 11;
    personal.started = true;

    while(!personal.path.empty()){
        personal.path.pop();
    }

    vehicles.emplace_back("car", id++).giveRandomPath();
    vehicles.emplace_back("car", id++).giveRandomPath();
    vehicles.emplace_back("car", id++).giveRandomPath();



    vehicles.emplace_back("taxi", id++).giveRandomPath();
    vehicles.emplace_back("taxi", id++).giveRandomPath();
    vehicles.emplace_back("taxi", id++).giveRandomPath();



    vehicles.emplace_back("policecar", id++).giveRandomPath();
    vehicles.emplace_back("policecar", id++).giveRandomPath();



    vehicles.emplace_back("ambulance", id++).giveRandomPath();




    vehicles.emplace_back("firetruck", id++).giveRandomPath();


    vehicles.emplace_back("truck", id++).giveRandomPath();

    vehicles.emplace_back("supercar", id++).giveRandomPath();
    vehicles.emplace_back("supercar", id++).giveRandomPath();



    if(trafficStrength == "medium" || trafficStrength == "high"){

        vehicles.emplace_back("car", id++).giveRandomPath();
        vehicles.emplace_back("car", id++).giveRandomPath();
        vehicles.emplace_back("car", id++).giveRandomPath();
        vehicles.emplace_back("car", id++).giveRandomPath();



        vehicles.emplace_back("taxi", id++).giveRandomPath();
        vehicles.emplace_back("taxi", id++).giveRandomPath();


        vehicles.emplace_back("policecar", id++).giveRandomPath();
        vehicles.emplace_back("policecar", id++).giveRandomPath();


        vehicles.emplace_back("truck", id++).giveRandomPath();


        vehicles.emplace_back("ambulance", id++).giveRandomPath();

        vehicles.emplace_back("supercar", id++).giveRandomPath();
        vehicles.emplace_back("supercar", id++).giveRandomPath();


        vehicles.emplace_back("bus", id++).giveRandomPath();
    }

    // ── HIGH TRAFFIC (adds on top of medium) ───────────────────────
    if(trafficStrength == "high"){

        vehicles.emplace_back("car", id++).giveRandomPath();
        vehicles.emplace_back("car", id++).giveRandomPath();
        vehicles.emplace_back("car", id++).giveRandomPath();


        vehicles.emplace_back("taxi", id++).giveRandomPath();
        vehicles.emplace_back("taxi", id++).giveRandomPath();


        vehicles.emplace_back("policecar", id++).giveRandomPath();
        vehicles.emplace_back("policecar", id++).giveRandomPath();


        vehicles.emplace_back("truck", id++).giveRandomPath();


        vehicles.emplace_back("ambulance", id++).giveRandomPath();

        vehicles.emplace_back("firetruck", id++).giveRandomPath();

        vehicles.emplace_back("supercar", id++).giveRandomPath();
        vehicles.emplace_back("supercar", id++).giveRandomPath();

        vehicles.emplace_back("bus", id++).giveRandomPath();

    }
}
int main()
{
    screenSetup();
    loadingMap();

    srand(time(0));






    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
            if (auto* keyPressed = event->getIf<Event::KeyPressed>())
                if (keyPressed->code == Keyboard::Key::Escape)
                    window.close();
            if(auto* mouseClick = event->getIf<Event::MouseButtonPressed>()){
                if(mouseClick->button == Mouse::Button::Left){
                    Vector2i clickpos = Mouse::getPosition(window);
                    if(currentState == PLAYING){
                        handleClick(clickpos);
                    }else if(currentState == MENU){
                        startMenuClickHandle(clickpos,window);
                    }else if(currentState == CHOOSETRAFFIC){
                        trafficClickHandle(clickpos,window);
                    }else if(currentState == CHOOSECAR){
                        chooseCarClickHandle(clickpos,window);
                    }
                }
            }
        }


        window.clear();
        window.draw(*mapSprite);
        for(int i = 0;i<vehicles.size();i++){
            if(vehicles[i].id == 51){
                handlePersonalVehicle(vehicles[i]);
            }
            TrafficManagement(vehicles[i]);
        }
        for(int i = 0;i<signals.size();i++){
            signals[i].update();
        }
        for(int i = 0;i<edges.size();i++){
            edges[i].showAllSignals();
        }
        window.draw(*mosqueSprite);
        window.draw(*hospitalSprite);


        if(currentState == MENU){
            mainMenuBackgrounds(window);
            startMenuButtons(window);
        }else if(currentState == CHOOSETRAFFIC){
            mainMenuBackgrounds(window);
            chooseTrafficButtons(window);
        }else if(currentState == CHOOSECAR){
            mainMenuBackgrounds(window);
            chooseCarMenuButtons(window);
        }
        else if(currentState == PLAYING){
            if(gameStarted == false){
                startGame();
                gameStarted = true;
            }
            chooseBuilding(window);
        }

        window.display();
    }

    return 0;
}
