#pragma once
#include <vector>
#include "shape.h"

class Rubik
{
public:
	Rubik();
	void setCube(int x, int y, int z, int index);
	void printCube();
	void setClockwize();
	void setAngle(float angle);
	float getAngle();
	glm::mat3 rotateMatrixClockwise(glm::mat3 matrix);
	glm::mat3 rotateMatrixCounterClockwise(glm::mat3 matrix);
	void lockWalls(int wall);
	void Rubik::unlockWalls(int wall);
	void rotateWallInDS(int wall);
	~Rubik(void);
	void rotateWall(int wall, std::vector<Shape*> shapes);
	void setCubeTrans(int index, glm::mat4 trans);

protected:

private:
	int cube[3][3][3];
	int Clockwize;
	float angle;
	std::vector<glm::mat4> transformations;
	int wallAngle[6]; //0 - front, 1 - back, 2 - left, 3 - right, 4 - up, 5 - down
	bool wallLock[6];
};