#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cctype>
#include <algorithm>

using std::vector;
using std::cout;
using std::string;
using std::endl;

float position[3] = {0,0,0};//xyz
vector<vector<float>> balls = {};
float lastBallDist = -1;
float indexenbr = -1; 
//the shape of area is a sphere

//Make it so the closest ball is stored, if no movement, use it
//After that, the functions checking position need changing
//Then you fix movement in the 3rd dimension
//Lastly, autoGetBall needs to support 3 dimensions
//After that you can add numbers and command codes
//Maybe gravity, time and saving
//Turn it into an actual game?

float radius = 30.0;

int xydirection = 0;
int xzdirection = 0;
float normalMovement = 1.0;
const double PI = 3.1416;

float placeBall(){
    vector<float> ballPos = {position[0],position[1],position[2]};
    balls.push_back(ballPos);
    lastBallDist = 0;
    indexenbr = balls.size();
    return 1;
}

float distToBall(string code);

float getBall(){
    float output = distToBall("");//Direct distance/hypotenuse
    float indexen = distToBall("ind");//Which ball
    cout <<"Distance to ball: " <<output<<endl;
    if (output<=5){
        cout << " Got Ball"<<endl;
        balls[indexen] = balls.back();
        balls.pop_back();
    }
    return output;
}

float autoGetBall(){
    string line;
    cout << "How many times to move to get to ball: ";
    std::getline(std::cin,line);
    int steps = stoi(line);
    
    //float hypotenuse = distToBall("");
    int dy = distToBall("dy");
    int dx = distToBall("dx");
    int dz = distToBall("dz");
    float hypotenuse = sqrt(dy*dy+dz*dz+dx*dx);
    
    float speedReq = hypotenuse/steps;
    int neededTurn;
    
    cout << endl << "Dy, Dx, Dz " << dy<< ", "<<dx<< ", "<<dz;
    
    if (dy<0){//
        int degree = std::asin(-dy/hypotenuse)*180/PI;
        cout << endl <<"degree: "<<degree<<"  ";
        //int gegree = std::asin();
        if (dx<0){//-dx -dy
            int gegree = 180+degree;
            neededTurn = gegree-xydirection;
        }else{//dx -dy
            int gegree = 350-degree;
            neededTurn = gegree-xydirection;
        }
    }else{//positive player is below
        int degree = std::asin(dy/hypotenuse)*180/PI;
        //int degree1 = std::acos(dx/hypotenuse);
        if (dx<0){//-dx dy
            int gegree = 180-std::asin(dy/hypotenuse)*180/PI;
            neededTurn = gegree-xydirection;
        }else{//dx dy
            neededTurn = degree-xydirection;
        }
    }   
    if (hypotenuse !=-1){
        cout << endl << "To get to the closest ball, set speed to " << speedReq << " and turn " << neededTurn<<" 'it's "<<neededTurn/19<<endl;
    }
    return neededTurn;
}

float distToBall(string code){
    cout << lastBallDist<< endl;
    if (lastBallDist != -1){
        if (code == ""){
            return lastBallDist;    
        }if(code == "ind"){
            return indexenbr;
        }
    }
    float px = position[0];
    float py = position[1];
    float pz = position[2];
    float minDist = -1;
    float index;
    float dinbex;
    int act_index;
    for (size_t i=0;i<balls.size();i++){
        float bx = balls[i][0];
        float by = balls[i][1];
        float bz = balls[i][2];
        float dx = (bx-px);
        float dy = (by-py);
        float dz = (bz-pz);
        float distance = sqrt(dx*dx+dy*dy+dz*dz);
        if (distance<minDist){//How does this work, mindist -1
            minDist = distance;
            index = dy;
            dinbex = dx;
            act_index = i;
        }
    }//make an array and return the code'th value
    if (code == "dy"){
        return index;
    }if(code =="dx"){
        return dinbex;
    }if(code=="ind"){
        return act_index;
    }else{
        return minDist;
    }
}
//Es gibt eine probleme mit der Bewegung. Es macht kein unterschied, ob der Winkel neunzig oder zweihundertsiebzig ist. Ich glaube, das liegt daran, dass wir den Winkel negativ gemacht haben.
float move(){
    lastBallDist = -1;
    indexenbr = -1;
    float radi = -xydirection*PI/180;
    float gadi = -xzdirection*PI/180;
    float tBSqr = 1+(std::tan(radi))*(std::tan(radi));
    tBSqr += (std::tan(gadi))*(std::tan(gadi));
    float xM= normalMovement/(std::sqrt(tBSqr));
    //xMovement += nM*std::cos(gadi);
    float yM=xM*std::tan(radi);//nM*std::sin(radi);
    float zM=xM*std::tan(gadi);//nM*std::sin(gadi);
    float t1 = position[0]+round(100.0*xM)/100.0;
    float t2 = position[1]+round(100.0*yM)/100.0;
    float t3 = position[2]+round(100.0*zM)/100.0;
    if (std::abs(t1)>= radius){
        t1 = radius*t1/std::abs(t1)-0.01;
        cout <<endl<< "Caution: Hit Wall X"<<endl<<endl;
    }
    if (std::abs(t2)>=radius){
        t2 = radius*t2/std::abs(t2)-0.01;
        cout <<endl<<"Caution: Hit Wall Y"<<endl<<endl;
    }
    if (std::abs(t3)>=radius){
        t3 = radius*t3/std::abs(t3)-0.01;
        cout <<endl<<"Caution: Hit Wall Z"<<endl<<endl;
        
    }
    position[0] = t1;
    position[1] = t2;
    position[2] = t3;
    //cout <<endl<< position[0]<<", "<<position[1]<<endl<<endl;
    return 0;
}

/*Bak adam bize diyecek ki 009A donmek istiyorum
Biz bunu 0x19+0 xz yonunde 0,   9x19+10 181 xy yonunde donmek
istiyourm diye anliyacagiz*/


int giveNona(string binput){
    //binput here is B9 or JJ type 2 digits
    char rep[9]= {'A','B','C','D','E','F','G','H','I'};
    int nums[9] = {10 ,11 ,12 ,13 ,14 ,15 ,16 ,17 ,18};
    int convertedDegrees = 0;
    if (std::isdigit(binput[0])){
        convertedDegrees += (binput[0]-'0')*19;
    }if(std::isdigit(binput[1])){
        convertedDegrees += (binput[1]-'0');
    }
    for (int i=0;i<9;i++){
        if (binput[0] == rep[i]){
            convertedDegrees += nums[i]*19;
        }
        if (binput[1] == rep[i]){
            convertedDegrees += nums[i];
        }
    }
    return convertedDegrees;
}

int cnvrtFrmNonadeci(string input){
    //here we are giving a 4 ncit input
    //and looking to give an 6 digit output
    /*in the form of: XXXZZZ
    string xzx = input[0]+input[1];
    string XZX = std::to_string(giveNona(xzx));//
    string xyx = input[2]+input[3];
    string XYX = std::to_string(giveNona(xyx));//
    if (XZX.length()<3){
        XZX+="0";
        if (XZX.length()<3){
            XZX += "0";
        }
    }
    if (XYX.length()<3){
        XYX+="0";
        if (XYX.length()<3){
            XYX+="0";
        }
    }
    string code = std::to_string(XZX)+std::to_string(XYX);
    */
    return 0;
}

int turn(){
    string degres;
    cout << "90 degrees is 4E, 180 is 99, 270 is E4, 360 is II(max 19^2-1)"<<endl;
    cout << "Enter amount in degrees in nonadecimal to turn: ";
    std::getline(std::cin,degres);
    int xzdegre = 0;
    int xydegre = 0;
    //Now 
    std::transform(degres.begin(),degres.end(),degres.begin(),[](unsigned char c){return std::toupper(c);});
    if (degres.length()>=2){
        string xyx = degres.substr(0, 2);
        xydegre = giveNona(xyx);
        if (degres.length()>=4){
            xzdegre = xydegre;
            string xzx = degres.substr(2, 2);
            xydegre = giveNona(xzx);
        }
    }
    
    xydirection += xydegre;
    xydirection %= 360;
    
    xzdirection +=xzdegre;
    xzdirection %= 360;
    return 0;
}

float setSpeed(){
    string newSpeed;
    cout << "Enter new speed: ";
    std::getline(std::cin, newSpeed);
    float num = normalMovement;
    try {
        num = stof(newSpeed);
    }catch(std::invalid_argument){
        cout << endl<< endl<<"Invalid Input"<<endl<<endl;
    }
    normalMovement = num;
    return 0;
}

float getDistance();

int giveInfo(){
    cout << endl << endl;
    cout << "You are at: "<<position[0]<<", "<<position[1]<<", "<<position[2];
    cout << endl<< "xydirection: "<< xydirection<< " Degrees";
    cout << endl<< "xzdirection: "<< xzdirection<< " Degrees";
    cout << endl << "Speed: " << normalMovement;
    cout << endl << "Distance from origin: " << 0.01*round(100*getDistance());
    cout << endl << "Distance to closest Ball: "<< distToBall(""); 
    cout<<endl<<"Number of balls: "<<balls.size()<<endl<< endl;
    return 0;
}
string checkCommand(string cmd);
float getDistance(){
    float n1 = position[0];
    float n2 = position[1];
    float n3 = position[2];
    float sumOfSquares = n1*n1+n2*n2+n3*n3;
    return std::sqrt(sumOfSquares);
}
string nextCommand()
{//To run this type of single line script 
//you need to have the number written
//No wait you can just ignore the rest of the command after 
//set speed
    string input;
    std::getline(std::cin, input);
    //input.pop_back();
    for (int ii=0;ii<input.length()/3;ii++){
        string commaned;
        commaned += input[ii*3];
        commaned += input[ii*3+1];
        commaned += input[ii*3+2];
        //cout <<"code: "<<commaned<<endl;
        if (checkCommand(commaned)=="break"){
            break;
        }
    }
    return input;
}
string checkCommand(string cmd){
    string input = "";
    if (cmd == "000"){
        move();    
    }
    if (cmd == "001"){
        turn();
    }
    if (cmd == "010"){
        setSpeed();
        //input = "break";
    }
    if (cmd == "011"){
        giveInfo();
    }
    if (cmd == "100"){
        getBall();
    }
    if (cmd=="101"){
        placeBall();    
    }
    if (cmd == "110"){
        autoGetBall();
    }
    //cout << endl<< cmd<<endl;
    return input;
}
//00 JJ==360 Degrees
//A B C D E F G H I J 
//0 1 2 3 4 5 6 7 8 9
int main()
{
    //vector<int> balls = {9};balls.push_back(1);//for (int num : balls){
    cout << "Instructions:"<<endl<< "000 to move"<< endl;
    cout <<"001 to turn"<<endl<<"010 to set speed"<<endl;
    cout <<"011 to display current status"<< endl;
    cout << "100 to get ball"<<endl<<"101 to place ball"<<endl;
    cout << "110 to auto get ball"<<endl;
    cout << "Turning is 4 bit nonadecimal or base-19. The first 2 digits are turning in the xz plane, the second 2 digits are for turning in the xy plane A(10) B C D E F G H I(18)";
    giveInfo();
    string line;
    while (true){
        cout << "Enter next command: ";
        line = nextCommand();
    }
    return 0;
}