// gcc -o dijkstra  dijkstra.c  -lGL -lglut -lGLU  -lm -std=c99
#include<GL/glut.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<time.h>

void dijkstra();

GLsizei wh = 1000, ww = 2000;
 

GLfloat size = 3.0;

float PI = 3.14285714286;

int radius = 30;

int nodeCount = 0;

int maxNodes = 26;

int adjMatrix[26][26];

int lineNodesCount = 0;

int sourceNodeCreated = 0;

int destinationNodeCreated = 0;

int dijkstraRun = 0;

int instruction = 1;

struct node{	
	int id;
	int xCoordinate;
	int yCoordinate;
}nodes[26], lineNodes[2], sourceNode, destinationNode;


int computeDistance(struct node a, struct node b){
	return (int) sqrt(pow(abs(a.xCoordinate - b.xCoordinate), 2) + pow(abs(a.yCoordinate - b.yCoordinate), 2));
}

int noRepeat(int x, int y) {	
	if(!nodeCount)
		return 1;

	struct node temp = {nodeCount, x, y};

	for(int i = 0; i < nodeCount; i++){	
		if(computeDistance(temp, nodes[i]) < 2*radius){	 
			lineNodes[lineNodesCount] = nodes[i];
			return 0;
		}
	}
	return 1;
}

void getPerpendicularCoordinates(int *x, int *y){
	int x1, x2, y1, y2;

	x1 = lineNodes[0].xCoordinate;
	x2 = lineNodes[1].xCoordinate;
	y1 = lineNodes[0].yCoordinate;
	y2 = lineNodes[1].yCoordinate;

	float diff = (x2 - x1)? 0.0001 : x2 - x1;

	float slope = ((float)(y2 - y1)) / (diff);

	float perpendicularSlope = -1*(1 / slope);


	*x = *x + 25; 	
	*y = *y + perpendicularSlope * (*x) + 7;
}

void writeDistance(char *text, int x, int y){	
	if(instruction)
		glColor3f(1.0, 1.0, 1.0);
	else
		glColor3f(1.0, 1.0, 1.0);
    glRasterPos2i(x,y);
    int len = strlen(text);
    for(int i = 0; i < len; i++){ 
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , text[i]);
    }
    glFlush();
}

void writeLabel(char text, int x, int y){	
	glColor3f(0.5, 0.2, 0.7);
    glRasterPos2i(x,y);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 , text);
}

void drawCircle(float cx, float cy, float r, float num_segments, char colorCharacter){
	float theta,x,y;

	if(colorCharacter == 'N')
		writeLabel((char)65 + nodeCount ,cx - 5, cy - 14);

	if(colorCharacter == 'N')
		glColor3f(0.0, 1.0, 0.0);

	if(colorCharacter == 'B')
		glColor3f(0.0, 0.0, 1.0);

	if(colorCharacter == 'R')
		glColor3f(1.0, 0.0, 0.0);

	glBegin(GL_LINE_LOOP);
 
	for(int i = 0; i < num_segments; i++){
		theta = 2.0f * PI * i / num_segments;
 		x = r * cosf(theta);
		y = r * sinf(theta);
 		
		glVertex2f(x + cx, y + cy);
	}
	glEnd();

	glFlush();
}

void drawSquare(int x, int y){

	glBegin(GL_POLYGON);
		glVertex2f(x+size, y+size);
		glVertex2f(x-size, y+size);
		glVertex2f(x-size, y-size);
		glVertex2f(x+size, y-size);
	glEnd();
	glFlush();
}

void getMidPoint(int *midX, int *midY){
	*midX = (lineNodes[0].xCoordinate + lineNodes[1].xCoordinate)/2;
	*midY = (lineNodes[0].yCoordinate + lineNodes[1].yCoordinate)/2;
}

void drawLine(char color) {
	if(color == 'N' || color == 'R')
		glColor3f(0.2, 0.2, 0.3);

	if(color == 'D')
		glColor3f(0.8, 0.8, 0.8);

	if(color == 'M')
		glColor3f(0.5, 0.0, 0.0);

	glBegin(GL_LINES);
		glVertex2f(lineNodes[0].xCoordinate, lineNodes[0].yCoordinate);
		glVertex2f(lineNodes[1].xCoordinate, lineNodes[1].yCoordinate);
	glEnd();
	glFlush();

	int midX, midY;
	char distanceString[10]; 
	getMidPoint(&midX, &midY);  


	int id1 = lineNodes[0].id;
	int id2 = lineNodes[1].id;
	int distance = computeDistance(lineNodes[0], lineNodes[1]);
	adjMatrix[id1][id2] = distance;
	adjMatrix[id2][id1] = distance;

	sprintf(distanceString, "%d",distance);
	getPerpendicularCoordinates(&midX, &midY);
	writeDistance(distanceString, midX, midY);

	glFlush();

	writeLabel('A' + lineNodes[0].id, lineNodes[0].xCoordinate - 5, lineNodes[0].yCoordinate - 14);
	writeLabel('A' + lineNodes[1].id, lineNodes[1].xCoordinate - 5, lineNodes[1].yCoordinate - 14);

	glFlush();
}


void myMouse(int btn, int state, int x, int y) {
	if(instruction){
		instruction = 0;
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if(dijkstraRun) {
		return;
	}

	y = wh-y;

	if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN){	

		if (noRepeat(x, y)){
			if(nodeCount >= 26){
				return;
			}

			drawCircle(x, y, radius, 200, 'N');
			lineNodesCount = 0;

			nodes[nodeCount].xCoordinate = x;
			nodes[nodeCount].yCoordinate = y;
			nodes[nodeCount].id = nodeCount;

			nodeCount++;
		} 
		else{
			if(lineNodesCount == 1){
				drawLine('N');
				lineNodesCount = 0;
			}
			else{
				lineNodesCount += 1;
			}
		}
	}

	if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN && (!sourceNodeCreated || !destinationNodeCreated)){
		if(nodeCount){	
			if(!noRepeat(x, y)){
				if(!sourceNodeCreated){
					sourceNode = lineNodes[0];
					sourceNodeCreated = 1;
					drawCircle(sourceNode.xCoordinate, sourceNode.yCoordinate, radius, 200, 'R');
				}
				else {
					if(((sourceNode.xCoordinate - lineNodes[0].xCoordinate) != 0) || ((sourceNode.yCoordinate - lineNodes[0].yCoordinate) != 0 )){
						destinationNode = lineNodes[0];
						destinationNodeCreated = 1;
						drawCircle(destinationNode.xCoordinate, destinationNode.yCoordinate, radius, 200, 'B');
					} 
				}
			}
		}
	}

}

void myReshape(GLsizei w, GLsizei h){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
	ww = w;
	wh = h;
}

void myInit(){
	glViewport(0,0,ww,wh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, ww, 0, wh);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 1);
	glColor3f(1.0, 1.0, 1.0);
	 glLineWidth(5);
}

void display_hello(){
	writeDistance("*  Use Left click to Create Nodes.", 250, 800);
	writeDistance("*  Select two Nodes to create a Path.", 250, 770);
	writeDistance("*  Use Right click to select Source and Destrination.", 250, 740);
	writeDistance("*  Press ENTER key to begin.", 250, 710);
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		display_hello();

		glFlush();
}

void fixAdjMatrix(){
	for(int i = 0; i < maxNodes; i++){
		for(int j = 0; j < maxNodes; j++){
			adjMatrix[i][j] = 99999;
		}
	}
}

void beginDijkstra(unsigned char key, int x, int y){
	if((int)key == 13){	
		dijkstraRun = 1;
		dijkstra();
	}
}

int main(int argc, char **argv) {
	fixAdjMatrix();
	glutInit(&argc, argv);
	glutInitWindowSize(ww, wh);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Dijksta's Algorithm Mini Project");
	myInit();
	glutReshapeFunc(myReshape);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(beginDijkstra);
	glutDisplayFunc(myDisplay);
	glutMainLoop();

	return 0;
}

int getMinimumDistanceNode(int *distanceFromSource, int *selected){
	int minimumNode = -1;
	int minimumDistance = 99999;

	for(int i = 0; i < nodeCount; i++){
		if(selected[i] != 1 && distanceFromSource[i] < minimumDistance){
			minimumNode = i;
			minimumDistance = distanceFromSource[i];
		}
	}
	return minimumNode;
}

void routeNodes(int n1, int n2){
	lineNodes[0] = nodes[n1];
	lineNodes[1] = nodes[n2];

	drawLine('M');
}

void delay(int number_of_seconds){
    int milli_seconds = 1000 * number_of_seconds;
 
    clock_t start_time = clock();
 
    while (clock() < start_time + milli_seconds);
}

void dijkstra(){

	int distanceFromSource[26];

	int prev[26];

	int selected[26];

	int current;

	for(int i = 0; i < nodeCount; i++){
		distanceFromSource[i] = 99999;
		prev[i] = -1;
		selected[i] = -1;
	}

	distanceFromSource[sourceNode.id] = 0;

	for(int i = 0; i < nodeCount; i++){
		current = getMinimumDistanceNode(distanceFromSource, selected);

		if(current == -1)
			break;

		selected[current] = 1;

		for(int j = 0; j < nodeCount; j++){

			if(selected[j] != 1){	
				if((distanceFromSource[current] + adjMatrix[current][j]) < distanceFromSource[j]){	
					distanceFromSource[j] = distanceFromSource[current] + adjMatrix[current][j];

					if(prev[j] != -1){	
						lineNodes[0] = nodes[prev[j]];
						lineNodes[1] = nodes[j];
						delay(700);
						drawLine('R');

						delay(700);
						lineNodes[0] = nodes[current];
						drawLine('D');
					}
					else{	
						delay(700);
						lineNodes[1] = nodes[j];
						lineNodes[0] = nodes[current];
						drawLine('D');
					}
					prev[j] = current;
				}
			}
		}

	}
	while(1){
		if(prev[destinationNode.id] == -1)
			break;
		delay(500);
		routeNodes(destinationNode.id, prev[destinationNode.id]);
		destinationNode = nodes[prev[destinationNode.id]];
	}
}