#include "Rubik.h"
#include <iostream>

Rubik::Rubik()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				cube[i][j][k] = 0;	
			}
		}
	}
	Clockwize = 1;
	angle = 90;
	for (int i = 0; i < 6; i++)
	{
		wallAngle[i] = 0;
		wallLock[i] = false;
	}
	
}


void Rubik::setCube(int x, int y, int z, int index)
{
	cube[x][y][z] = index;
	transformations.push_back(glm::mat4(1));
	
}

void Rubik::printCube()
{
	int original = 0;
	for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					std::cout <<original<<"- " << "cube index: " << cube[i][j][k] << ", [" << i << "," << j << "," << k << "]" << std::endl;
					original++;
				}
			}
		}
}

void Rubik::setClockwize()
{
	this->Clockwize = this->Clockwize == 1 ? 0 : 1;
}

void Rubik::setAngle(float angle)
{
	if (angle == 45 || angle == 90 || angle == 180)
		this->angle = angle;
	else
		std::cout << "Invalid angle" << std::endl;
}

float Rubik::getAngle()
{
	return this->angle;
}


glm::mat3 Rubik::rotateMatrixClockwise(glm::mat3 matrix) {

		int temp;

		// Transpose the matrix
		for (int i = 0; i < 3; ++i) {
			for (int j = i; j < 3; ++j) {
				temp = matrix[i][j];
				matrix[i][j] = matrix[j][i];
				matrix[j][i] = temp;
			}
		}

		// Reverse each row
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 1.5; ++j) { // Only iterate half of the row to avoid swapping twice
				temp = matrix[i][j];
				matrix[i][j] = matrix[i][2 - j];
				matrix[i][2 - j] = temp;
			}
		}
		return matrix;
}
glm::mat3 Rubik::rotateMatrixCounterClockwise(glm::mat3 matrix) {
    int temp;

    // Reverse each row
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 1.5; ++j) { // Only iterate half of the row to avoid swapping twice
            temp = matrix[i][j];
            matrix[i][j] = matrix[i][2 - j];
            matrix[i][2 - j] = temp;
        }
    }

    // Transpose the matrix
    for (int i = 0; i < 3; ++i) {
        for (int j = i; j < 3; ++j) {
            temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }

    return matrix;
}

void Rubik::lockWalls(int wall)
{
	//0 - front, 1 - back, 2 - left, 3 - right, 4 - up, 5 - down
	switch (wall) {
	case 0:
		wallLock[2] = true;
		wallLock[3] = true;
		wallLock[4] = true;
		wallLock[5] = true;
		break;
	
	case 1:
		wallLock[2] = true;
		wallLock[3] = true;
		wallLock[4] = true;
		wallLock[5] = true;
		break;
	case 2:
		wallLock[0] = true;
		wallLock[1] = true;
		wallLock[4] = true;
		wallLock[5] = true;
		break;
	case 3:
		wallLock[0] = true;
		wallLock[1] = true;
		wallLock[4] = true;
		wallLock[5] = true;
		break;
	case 4:
		wallLock[0] = true;
		wallLock[1] = true;
		wallLock[2] = true;
		wallLock[3] = true;
		break;
	case 5:
		wallLock[0] = true;
		wallLock[1] = true;
		wallLock[2] = true;
		wallLock[3] = true;
		break;
	}
}

void Rubik::unlockWalls(int wall) {
	//0 - front, 1 - back, 2 - left, 3 - right, 4 - up, 5 - down
	switch (wall) {
	case 0:
		if (wallAngle[1] % 90 == 0) {
			wallLock[2] = false;
			wallLock[3] = false;
			wallLock[4] = false;
			wallLock[5] = false;
		}
		break;
	case 1:
		if (wallAngle[0] % 90 == 0) {
			wallLock[2] = false;
			wallLock[3] = false;
			wallLock[4] = false;
			wallLock[5] = false;
		}
		break;
	case 2:
		if (wallAngle[3] % 90 == 0) {
			wallLock[0] = false;
			wallLock[1] = false;
			wallLock[4] = false;
			wallLock[5] = false;
		}
		break;
	case 3:
		if (wallAngle[2] % 90 == 0) {
			wallLock[0] = false;
			wallLock[1] = false;
			wallLock[4] = false;
			wallLock[5] = false;
		}
		break;
	case 4:
		if (wallAngle[5] % 90 == 0) {
			wallLock[0] = false;
			wallLock[1] = false;
			wallLock[2] = false;
			wallLock[3] = false;
		}
		break;
	case 5:
		if (wallAngle[4] % 90 == 0) {
			wallLock[0] = false;
			wallLock[1] = false;
			wallLock[2] = false;
			wallLock[3] = false;
		}
		break;
	}

}




void Rubik::rotateWallInDS(int wall) 
{
	if (wallLock[wall])
		return;
	std::cout << "wall: " << wall << std::endl;
	std::cout << "lock: " << wallLock[wall] << std::endl;
	std::cout << "angle: " << angle << std::endl;
	//setting the wall angle
	if (Clockwize)
		wallAngle[wall] += angle; 
	else
		wallAngle[wall] -= angle;

	wallAngle[wall] = wallAngle[wall] % 360;
	std::cout << "wall angle after mod: " << wallAngle[wall] << std::endl;

	if (wallAngle[wall] % 90 != 0) {
		lockWalls(wall);
		return;
	}	

	//counting the number of 90 degrees rotates
	int rotates = wallAngle[wall] / 90; 
	rotates < 0 ? rotates = rotates * (-1) : rotates = rotates;

	std::cout << "rotates: " << rotates << std::endl;
	glm::mat3 tempWall;

	switch (wall)
	{
	case 0: //front

		tempWall[0][0] = cube[0][2][2];
		tempWall[0][1] = cube[1][2][2];
		tempWall[0][2] = cube[2][2][2];	
		tempWall[1][0] = cube[0][1][2];
		tempWall[1][1] = cube[1][1][2];
		tempWall[1][2] = cube[2][1][2];
		tempWall[2][0] = cube[0][0][2];
		tempWall[2][1] = cube[1][0][2];
		tempWall[2][2] = cube[2][0][2];

		for (int i = 0; i < rotates; i++)
			wallAngle[wall] > 0 ? tempWall = rotateMatrixClockwise(tempWall) : tempWall = rotateMatrixCounterClockwise(tempWall);

		cube[0][2][2] = tempWall[0][0];
		cube[1][2][2] = tempWall[0][1];
		cube[2][2][2] = tempWall[0][2];
		cube[0][1][2] = tempWall[1][0];
		cube[1][1][2] = tempWall[1][1];
		cube[2][1][2] = tempWall[1][2];
		cube[0][0][2] = tempWall[2][0];
		cube[1][0][2] = tempWall[2][1];
		cube[2][0][2] = tempWall[2][2];

		break;

	case 1: //back
 		tempWall[0][0] = cube[2][2][0];
		tempWall[0][1] = cube[1][2][0];
		tempWall[0][2] = cube[0][2][0];
		tempWall[1][0] = cube[2][1][0];
		tempWall[1][1] = cube[1][1][0];
		tempWall[1][2] = cube[0][1][0];
		tempWall[2][0] = cube[2][0][0];
		tempWall[2][1] = cube[1][0][0];
		tempWall[2][2] = cube[0][0][0];

		for (int i = 0; i < rotates; i++)
			wallAngle[wall] > 0 ? tempWall = rotateMatrixClockwise(tempWall) : tempWall = rotateMatrixCounterClockwise(tempWall);

		cube[2][2][0] = tempWall[0][0];
		cube[1][2][0] = tempWall[0][1];
		cube[0][2][0] = tempWall[0][2];
		cube[2][1][0] = tempWall[1][0];
		cube[1][1][0] = tempWall[1][1];
		cube[0][1][0] = tempWall[1][2];
		cube[2][0][0] = tempWall[2][0];
		cube[1][0][0] = tempWall[2][1];
		cube[0][0][0] = tempWall[2][2];

		break;
	case 2: //left

		tempWall[0][0] = cube[0][2][0];
		tempWall[0][1] = cube[0][2][1];
		tempWall[0][2] = cube[0][2][2];
		tempWall[1][0] = cube[0][1][0];
		tempWall[1][1] = cube[0][1][1];
		tempWall[1][2] = cube[0][1][2];
		tempWall[2][0] = cube[0][0][0];
		tempWall[2][1] = cube[0][0][1];
		tempWall[2][2] = cube[0][0][2];

		for (int i = 0; i < rotates; i++)
			wallAngle[wall] > 0 ? tempWall = rotateMatrixClockwise(tempWall) : tempWall = rotateMatrixCounterClockwise(tempWall);

		cube[0][2][0] = tempWall[0][0];
		cube[0][2][1] = tempWall[0][1];
		cube[0][2][2] = tempWall[0][2];
		cube[0][1][0] = tempWall[1][0];
		cube[0][1][1] = tempWall[1][1];
		cube[0][1][2] = tempWall[1][2];
		cube[0][0][0] = tempWall[2][0];
		cube[0][0][1] = tempWall[2][1];
		cube[0][0][2] = tempWall[2][2];
		break;

	case 3: //right

		tempWall[0][0] = cube[2][2][2];
		tempWall[0][1] = cube[2][2][1];
		tempWall[0][2] = cube[2][2][0];
		tempWall[1][0] = cube[2][1][2];
		tempWall[1][1] = cube[2][1][1];
		tempWall[1][2] = cube[2][1][0];
		tempWall[2][0] = cube[2][0][2];
		tempWall[2][1] = cube[2][0][1];
		tempWall[2][2] = cube[2][0][0];

		for (int i = 0; i < rotates; i++)
			wallAngle[wall] > 0 ? tempWall = rotateMatrixClockwise(tempWall) : tempWall = rotateMatrixCounterClockwise(tempWall);

		cube[2][2][2] = tempWall[0][0];
		cube[2][2][1] = tempWall[0][1];
		cube[2][2][0] = tempWall[0][2];
		cube[2][1][2] = tempWall[1][0];
		cube[2][1][1] = tempWall[1][1];
		cube[2][1][0] = tempWall[1][2];
		cube[2][0][2] = tempWall[2][0];
		cube[2][0][1] = tempWall[2][1];
		cube[2][0][0] = tempWall[2][2];
		break;

	case 4: //up

		tempWall[0][0] = cube[0][2][0];
		tempWall[0][1] = cube[1][2][0];
		tempWall[0][2] = cube[2][2][0];
		tempWall[1][0] = cube[0][2][1];
		tempWall[1][1] = cube[1][2][1];
		tempWall[1][2] = cube[2][2][1];
		tempWall[2][0] = cube[0][2][2];
		tempWall[2][1] = cube[1][2][2];
		tempWall[2][2] = cube[2][2][2];

		for (int i = 0; i < rotates; i++)
			wallAngle[wall] > 0 ? tempWall = rotateMatrixClockwise(tempWall) : tempWall = rotateMatrixCounterClockwise(tempWall);

		cube[0][2][0] = tempWall[0][0];
		cube[1][2][0] = tempWall[0][1];
		cube[2][2][0] = tempWall[0][2];
		cube[0][2][1] = tempWall[1][0];
		cube[1][2][1] = tempWall[1][1];
		cube[2][2][1] = tempWall[1][2];
		cube[0][2][2] = tempWall[2][0];
		cube[1][2][2] = tempWall[2][1];
		cube[2][2][2] = tempWall[2][2];

		break;
	case 5: //down

		tempWall[0][0] = cube[0][0][2];
		tempWall[0][1] = cube[1][0][2];
		tempWall[0][2] = cube[2][0][2];
		tempWall[1][0] = cube[0][0][1];
		tempWall[1][1] = cube[1][0][1];
		tempWall[1][2] = cube[2][0][1];
		tempWall[2][0] = cube[0][0][0];
		tempWall[2][1] = cube[1][0][0];
		tempWall[2][2] = cube[2][0][0];

		for (int i = 0; i < rotates; i++)
			wallAngle[wall] > 0 ? tempWall = rotateMatrixClockwise(tempWall) : tempWall = rotateMatrixCounterClockwise(tempWall);

		cube[0][0][2] = tempWall[0][0];
		cube[1][0][2] = tempWall[0][1];
		cube[2][0][2] = tempWall[0][2];
		cube[0][0][1] = tempWall[1][0];
		cube[1][0][1] = tempWall[1][1];
		cube[2][0][1] = tempWall[1][2];
		cube[0][0][0] = tempWall[2][0];
		cube[1][0][0] = tempWall[2][1];
		cube[2][0][0] = tempWall[2][2];

		
		break;

		default:
			break;
	}
	unlockWalls(wall);
		
	wallAngle[wall] = 0;
	printCube();
}

Rubik::~Rubik(void)
{
}


void Rubik::rotateWall(int wall, std::vector<Shape*> shapes)
{
	if (wallLock[wall])
		return;

	float angle = getAngle();
	switch (wall)
	{
		case 0: //front
			
			for (int i = 0;i < 3;i++) {
				for (int j = 0;j < 3;j++) {
					Shape* s = shapes[cube[i][j][2]];
					if (s != nullptr) {
						glm::mat4 rot_t = glm::transpose(s->getRotate());
						!Clockwize? s->MyRotate(angle, glm::vec3(rot_t * glm::vec4(0, 0, 1, 1)), 0) : s->MyRotate(-angle, glm::vec3(rot_t * glm::vec4(0, 0, 1, 1)), 0);

					}
				}
			}
			break;
		case 1: //back
			for (int i = 0;i < 3;i++) {
				for (int j = 0;j < 3;j++) {
					Shape* s = shapes[cube[i][j][0]];
					if (s != nullptr) {
						glm::mat4 rot_t = glm::transpose(s->getRotate());
						Clockwize ? s->MyRotate(angle, glm::vec3(rot_t * glm::vec4(0, 0, 1, 1)), 0) : s->MyRotate(-angle, glm::vec3(rot_t * glm::vec4(0, 0, 1, 1)), 0);
					}
				}
			}
			break;
		case 2: //left
			for (int i = 0;i < 3;i++) {
				for (int j = 0;j < 3;j++) {
					Shape* s = shapes[cube[0][i][j]];
					if (s != nullptr) {
						glm::mat4 rot_t = glm::transpose(s->getRotate());
						Clockwize ? s->MyRotate(angle, glm::vec3(rot_t * glm::vec4(1, 0, 0, 1)), 0) : s->MyRotate(-angle, glm::vec3(rot_t * glm::vec4(1, 0, 0, 1)), 0);
					}
				}
			}
			break;
		case 3: //right
			for (int i = 0;i < 3;i++) {
				for (int j = 0;j < 3;j++) {
					Shape* s = shapes[cube[2][i][j]];
					if (s != nullptr) {
						glm::mat4 rot_t = glm::transpose(s->getRotate());
						!Clockwize ? s->MyRotate(angle, glm::vec3(rot_t * glm::vec4(1, 0, 0, 1)), 0) : s->MyRotate(-angle, glm::vec3(rot_t * glm::vec4(1, 0, 0, 1)), 0);
					}
				}
			}
			break;
		case 4: //up
			for (int i = 0;i < 3;i++) {
				for (int j = 0;j < 3;j++) {
					Shape* s = shapes[cube[i][2][j]];
					if (s != nullptr) {
						glm::mat4 rot_t = glm::transpose(s->getRotate());
						!Clockwize ? s->MyRotate(angle, glm::vec3(rot_t * glm::vec4(0, 1, 0, 1)), 0) : s->MyRotate(-angle, glm::vec3(rot_t * glm::vec4(0, 1, 0, 1)), 0);
					}
				}
			}
			break;
		case 5: //down
			for (int i = 0;i < 3;i++) {
				for (int j = 0;j < 3;j++) {
					Shape* s = shapes[cube[i][0][j]];
					if (s != nullptr) {
						glm::mat4 rot_t = glm::transpose(s->getRotate());
						Clockwize ? s->MyRotate(angle, glm::vec3(rot_t * glm::vec4(0, 1, 0, 1)), 0) : s->MyRotate(-angle, glm::vec3(rot_t * glm::vec4(0, 1, 0, 1)), 0);

					}
				}
			}
			break;

			default:
				break;
			
	}

	
}

void Rubik::setCubeTrans(int index, glm::mat4 trans)
{
	transformations[index] = trans;
}
