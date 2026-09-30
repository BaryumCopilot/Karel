#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <string>
#include <cctype>
#include <algorithm>

using std::vector;
using std::array;
using std::cout;
using std::string;
using std::endl;

// A position always has exactly three coordinates. std::array stores its
// elements directly (unlike vector, it cannot grow or shrink), so it suits
// fixed-size data such as one XYZ position.
using Position = array<float, 3>;

// getClosestBallInfo returns one compact snapshot with this fixed layout:
// [distance, delta X, delta Y, delta Z, index in balls].
// Keeping related values together means callers can find the nearest ball
// once and reuse all of its data instead of rescanning the vector per value.
using ClosestBallInfo = array<float, 5>;

// Named offsets make the array's layout readable at each use site and avoid
// scattering unexplained numeric subscripts such as closestBall[3].
constexpr size_t DISTANCE_INDEX = 0;
constexpr size_t DELTA_X_INDEX = 1;
constexpr size_t DELTA_Y_INDEX = 2;
constexpr size_t DELTA_Z_INDEX = 3;
constexpr size_t BALL_INDEX = 4;

Position position = {0, 0, 0};
vector<Position> balls = {};
//the shape of area is a sphere

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
    // Both position and each entry in balls use the same three-coordinate
    // type, so this copies the current XYZ position into the dynamic vector.
    balls.push_back(position);
    return 1;
}

// A declaration lets callers above the function use it before its definition.
ClosestBallInfo getClosestBallInfo();

float getBall(){
    // Store the returned array locally: all values below refer to the same
    // closest-ball search, and the vector is not searched again for its index.
    const ClosestBallInfo closestBall = getClosestBallInfo();
    const float distance = closestBall[DISTANCE_INDEX];
    cout << "Distance to ball: " << distance << endl;
    // A negative distance is the "no ball" sentinel. Check it before using
    // the returned index, because an empty vector has no valid element to erase.
    if (distance >= 0 && distance <= 5){
        cout << " Got Ball"<<endl;
        // The lookup stores the vector index as a float to fit the all-float
        // result array; convert it back before using it to index the vector.
        const size_t ballIndex = static_cast<size_t>(closestBall[BALL_INDEX]);
        balls[ballIndex] = balls.back();
        balls.pop_back();
    }
    return distance;
}

float autoGetBall(){
    const ClosestBallInfo closestBall = getClosestBallInfo();
    // The lookup returns a negative distance when balls is empty. Returning
    // here prevents using missing offsets or dividing by an invalid distance.
    if (closestBall[DISTANCE_INDEX] < 0){
        cout << "No balls available to collect." << endl;
        return -1;
    }

    string line;
    cout << "How many times to move to get to ball: ";
    std::getline(std::cin,line);
    const int steps = stoi(line);
    // steps is the divisor used to calculate speed; zero or a negative value
    // cannot describe a valid number of movements.
    if (steps <= 0){
        cout << "The number of steps must be greater than zero." << endl;
        return -1;
    }
    
    const float deltaY = closestBall[DELTA_Y_INDEX];
    const float deltaX = closestBall[DELTA_X_INDEX];
    const float deltaZ = closestBall[DELTA_Z_INDEX];
    const float hypotenuse = sqrt(deltaY*deltaY+deltaZ*deltaZ+deltaX*deltaX);
    // If all offsets are zero, the player is already at the target. Avoid
    // dividing by this zero length while calculating the turn angle.
    if (hypotenuse == 0){
        cout << "Already at the closest ball." << endl;
        return 0;
    }
    
    const float requiredSpeed = hypotenuse/steps;
    int requiredTurn;
    
    cout << endl << "Dy, Dx, Dz " << deltaY << ", " << deltaX << ", " << deltaZ;
    
    if (deltaY<0){//
        int degree = std::asin(-deltaY/hypotenuse)*180/PI;
        cout << endl <<"degree: "<<degree<<"  ";
        if (deltaX<0){//-dx -dy
            int targetDirection = 180+degree;
            requiredTurn = targetDirection-xydirection;
        }else{//dx -dy
            int targetDirection = 350-degree;
            requiredTurn = targetDirection-xydirection;
        }
    }else{//positive player is below
        int degree = std::asin(deltaY/hypotenuse)*180/PI;
        if (deltaX<0){//-dx dy
            int targetDirection = 180-std::asin(deltaY/hypotenuse)*180/PI;
            requiredTurn = targetDirection-xydirection;
        }else{//dx dy
            requiredTurn = degree-xydirection;
        }
    }   
    if (hypotenuse !=-1){
        cout << endl << "To get to the closest ball, set speed to " << requiredSpeed << " and turn " << requiredTurn<<" 'it's "<<requiredTurn/19<<endl;
    }
    return requiredTurn;
}

ClosestBallInfo getClosestBallInfo(){
    // A distance of -1 signals that no ball was found. The other sentinel
    // values keep the whole result initialized until a candidate is selected.
    ClosestBallInfo closestBall = {-1, -1, -1, -1, -1};
    // INFINITY ensures the first real distance is smaller, including when
    // there is exactly one ball or its distance is zero.
    float shortestDistance = INFINITY;

    // Calculate distance and offsets together for each ball. When a nearer
    // ball is found, save all its values in one result; this is one O(n) scan.
    for (size_t ballIndex = 0; ballIndex < balls.size(); ++ballIndex){
        const float deltaX = balls[ballIndex][0] - position[0];
        const float deltaY = balls[ballIndex][1] - position[1];
        const float deltaZ = balls[ballIndex][2] - position[2];
        const float distance = sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);

        if (distance < shortestDistance){
            shortestDistance = distance;
            // Aggregate initialization fills the result in the documented
            // order above, with an explicit conversion for the vector index.
            closestBall = {
                distance,
                deltaX,
                deltaY,
                deltaZ,
                static_cast<float>(ballIndex)
            };
        }
    }

    // Returning std::array by value gives the caller an independent,
    // fixed-size copy of the result (small arrays like this are inexpensive).
    return closestBall;
}
//Es gibt eine probleme mit der Bewegung. Es macht kein unterschied, ob der Winkel neunzig oder zweihundertsiebzig ist. Ich glaube, das liegt daran, dass wir den Winkel negativ gemacht haben.
float move(){
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
    // This display only needs the distance, so it reads that field from the
    // same result type used by getBall and autoGetBall.
    cout << endl << "Distance to closest Ball: " << getClosestBallInfo()[DISTANCE_INDEX];
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