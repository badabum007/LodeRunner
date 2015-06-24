#include "../include/Personagem.h"
#include "../include/ObjEnum.h"
//#include "../include/collision.h"

extern bool collides(float,float);

Personagem::Personagem()
{

}

Personagem::~Personagem()
{
    //dtor
}

Personagem::Personagem(int x, int y, float sX, float sY)
{
    this->matrixPosition = std::make_pair(x,y);
    this->screenPosition = std::make_pair(sX,sY);
}

Personagem::Personagem(std::pair<int, int> matrixPosition, std::pair<float,float> screenPosition)
{
    this->matrixPosition = matrixPosition;
    this->screenPosition = screenPosition;
}

void Personagem::move_me(ObjEnum *matrix, int height, int width)
{
    if (this->moving) { this->keep_moving(matrix); return; };
    std::vector<std::pair<int,int>> neighbours;


    const int myRow = matrixPosition.first;
    const int myCol = matrixPosition.second;
    std::pair<int,int> move_to;




    for (int i = -1; i <= 1; ++i)
    {
        for (int j = ((i != 0) ? 0 : -1) ; j <= ((i != 0) ? 0 : 1); ++j)
        {

            int k = myRow + i;
            if (k < 0 || k >= height) continue;
            int l = myCol + j;
            if (l < 0 || l >= width) continue;
            if (k == myRow && l == myCol)
                continue;

            if (matrix[k*24+l] == ObjEnum::PRINCIPAL)
            {
                extern bool pengoDead;
                move_to = std::make_pair(k,l);
                //pengoDead = true;
                this->targetPosition = std::make_pair((float) move_to.first - 12.0 + 0.5, (float)move_to.second - 12.0 + 0.5);
                this->moving = true;
                return;
            }
            if (matrix[k*24+l] == ObjEnum::VAZIO)
                neighbours.push_back(std::make_pair(k,l));
        }
    }
    if (neighbours.empty()) return;
    std::shuffle(neighbours.begin(), neighbours.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
    move_to = neighbours[0];
    this->targetPosition = std::make_pair((float)move_to.first - 12.0 + 0.5, (float)move_to.second - 12.0 + 0.5);
    this->moving = true;
    /*
    matrix[this->matrixPosition.first*24 + this->matrixPosition.second] = NOTHING;
    if (this->valid){
        matrix[move_to.first*24 + move_to.second] = ENEMY;
        this->matrixPosition = move_to;
        this->targetPosition = make_pair((float)move_to.first - 12.0 + 0.5, (float)move_to.second - 12.0 + 0.5);
        this->moving = true;
    }
    */
}

bool Personagem::isValid() {
    return valid;
}



void Personagem::keep_moving(ObjEnum *matrix)
{
    /*
    const int myRow = matrixPosition.first;
    const int myCol = matrixPosition.second;
                if ((matrix[myRow*24+myCol] == ICECUBE || matrix[(myRow+1)*24+myCol] == ICECUBE || matrix[(myRow-1)*24+myCol] == ICECUBE || matrix[myRow*24+myCol+1] == ICECUBE || matrix[myRow*24+myCol-1] == ICECUBE)){ //COLLISION
                this->moving = false;
                this->valid = false;
            }
    */
    if (std::fabs(this->screenPosition.first - this->targetPosition.first) <= 0.1)
    {
        this->screenPosition.first = this->targetPosition.first;
    }
    if (std::fabs(this->screenPosition.second - this->targetPosition.second) <= 0.1)
    {
        this->screenPosition.second = this->targetPosition.second;
    }
    if (this->screenPosition.first != this->targetPosition.first)
    {

        if (matrix[((int)std::round(this->targetPosition.first -0.5)+12)*24 + ((int)std::round(this->targetPosition.second - 0.5)+12) ] != ObjEnum::VAZIO)
        {
            this->moving = false;

            this->targetPosition = std::make_pair(float(this->matrixPosition.first) -12 + 0.5, float(this->matrixPosition.second) -12.0 + 0.5);
            return;
        }

        this->screenPosition.first += (this->screenPosition.first < this->targetPosition.first ? 0.05 : -0.05);
        return;
    }
    if (this->screenPosition.second != this->targetPosition.second)
    {
        if (matrix[this->matrixPosition.first*24 + ((int)std::round(this->targetPosition.second - 0.5) + 12)] != ObjEnum::VAZIO)
        {
            this->moving = false;
            this->targetPosition = std::make_pair(float(this->matrixPosition.first) -12 + 0.5, float(this->matrixPosition.second) -12.0 + 0.5);
            return;
        }
        this->screenPosition.second += (this->screenPosition.second < this->targetPosition.second ? 0.05 : -0.05);
    }

    if (this->screenPosition == this->targetPosition)
    {
        this->moving = false;
    }
    this->matrixPosition =
            std::make_pair((int)std::round(this->screenPosition.first - 0.5) + 12, (int)std::round(this->screenPosition.first - 0.5) + 12);
    extern bool pengoDead;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = (i != 0 ? 0 : -1); j <= ( i != 0 ? 0 : 1); j += (i == 0 ? 2 : 1))
        {
            if (matrix[(this->matrixPosition.first+i)*24 + this->matrixPosition.second + j] == ObjEnum::PRINCIPAL)
            {
//                pengoDead = true;
                this->moving = false;
                return;
            }
        }
    }
    if (matrix[this->matrixPosition.first*24 + this->matrixPosition.second] != ObjEnum::VAZIO)
    {
        this->moving = false;
    }
}

std::pair<int,int> Personagem::get_matrix_pos()
{
    return this->matrixPosition;
}

std::pair<float,float> Personagem::get_screen_pos()
{
    return this->screenPosition;
}

void Personagem::set_valid(bool b)
{
    this->valid = b;
}

bool Personagem::is_moving()
{
    return this->moving;
}

void Personagem::set_moving(bool b)
{
    this->moving = b;
}
