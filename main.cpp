#include "point.h"
#include "line.h"
#include "polygon.h"
#include "shapes.h"
#include "curve.h"
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <list>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#endif

using namespace std;
#define pi 3.142857

//Value of RGB while Drawing
float red = 1.0, green = 0.0, blue = 0.0;

int ptsize=3;
int brushSize = 4;
int edge=3;
int eraserSize = 1;
int tmpx, tmpy; // store the first point when shape is line, rectangle or circle
bool isSecond = false;
bool isfilled = false;
/*
When shape is line , rect , circle after storing the first point we make isSecond =true ......
so as to take new values of x , y and give call to line , or rect , or circle with 4 parameters
,old points stores in tmpx ,tmpy and new points stored in x, y
*/
bool isRandom = false;
bool isEraser = false;
bool isRadial = false;
float window_w = 600;
float window_h = 500;
//Erase means simply drawing white dot over dot.so that it will be rubbed. 
//Initially we have kept shape as point
int shape = 1; // 1:point, 2:line, 3:rectangle, 4:circle, 5:brush
//Erase means simply drawing white dot over dot.so that it will be rubbed. 
vector<Point> points;		// store all the points until clear
std::list<int> undoHistory; // record for undo, maximum 20 shapes in history
std::list<int> redoHistory; // record for redo, maximum 20 shapes in history
std::vector<Point> redoDots;  // store the dots after undo temporaly

line L;//global objects defined for class line ,point,rectangle circle.
Rectangle R;
circle c;
Polygon P1;
void printGuide();
//EVENT ZONE
//clear  ,quit
/* Double buffering provides two complete color buffers for use in drawing. One buffer is displayed while the other buffer is being drawn into. When the drawing is complete, the two buffers are swapped so that the one that was being viewed is now used for drawing. The swap is almost instantaneous, and thus provides a means of performing animation.
GLUT provides commands for setting up and using double buffers on a per window basis. The initial display mode for the window must be set using GLUT_DOUBLE mode (glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE)). The function glutSwapBuffers(), which is typically the last function called in the windows display callback, performs the actual buffer swap. */
void clear()          //delets all entries in vector defined for objects of class point.
{
	points.clear();
	undoHistory.clear();
	redoDots.clear();
	redoHistory.clear();
	glClear(GL_COLOR_BUFFER_BIT);//clears screen with current colour i.e. white colour.
	glutSwapBuffers();
	cout<< "[Info] The window is cleared successfully.\n";
}

void quit()
{
	/* cout<<"this os total width  X of pixel."<<glutGet(GLUT_WINDOW_X);
	 cout<<"this os total width  Y of pixel."<<glutGet(GLUT_WINDOW_Y);
	 cout<<"this os total width of pixel."<<glutGet(GLUT_WINDOW_WIDTH);
	 cout<<"this os total width of pixel."<<glutGet(GLUT_WINDOW_HEIGHT);*/
	cout << "Thank you for using this Paint tool! Goodbye!" << endl;
	exit(0);
}
/*
 * Undo implementation:
 * I am storing dot size each time i encounter a new shape into undo history vector.
 *So suppose 20 dots are drawn ,they will be pushed ,after clicking on shape first 20 dots are pushed in do array and corresponding size will be pushed in undo.Now u drawn shape.so those dots will also be stored on dots vector.so dots size will increase.Now new dot size push into undo.Afte rundo command u will find differnece between dots size- undo history back which is equal total dots contributing for that shape.SO u wil pop those dots and push them into redo dots.
 *and finally in redo history push that shape s and pop shpae from undo history.
 *
 *
 */
void undo()
{
	if (undoHistory.size() > 0)
	{
		if (undoHistory.back() != points.size() && redoHistory.back() != points.size())
		{
			redoHistory.push_back(points.size());
		}
		int numRemove = points.size() - undoHistory.back();
		for (int i = 0; i < numRemove; i++)
		{
			redoDots.push_back(points.back());
			points.pop_back();
		}
		redoHistory.push_back(undoHistory.back());
		undoHistory.pop_back();
	}
	else
	{
		cout<< "[Warning] Cannot undo. This is the first record in the history.\n";
	}
}

void redo()
{
	if (redoHistory.size() > 1)
	{
		undoHistory.push_back(redoHistory.back());
		redoHistory.pop_back();
		int numRemove = redoHistory.back() - points.size();
		for (int i = 0; i < numRemove; i++)
		{
			points.push_back(redoDots.back());
			redoDots.pop_back();
		}
	}
	else
	{
		cout<< "[Warning] Cannot redo. This is the last record in the history.\n";
	}
}
void Polygon::draw_polygon(int x1, int y1, int x2, int y2){
		float x,y,m,n,i;
		x=x1;
		y=y1;
       	float a = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
        float interior=(pi*(edge-2)/edge);
        float k=pi-interior;
        //Writing in general function{}
        for(int j=0;j<edge;j++){
                m=x;
                n=y;
                L.drawLine(m,n,m+a*cos(k*j),n+a*sin(k*j));
                x=m+a*cos(k*j);
                y=n+a*sin(k*j);
       }
}
//Drawing Zone
/*
FOR GLUT : mouse_x and mouse_y
Top left corner is the origin
As we move on right x increases
As we move down y increases


Bottom Left Corner is the origin
As we move on right x increases
As we move up y increases

*/
void drawPoint(int mousex, int mousey)
{
	Point newPoint(mousex, window_h - mousey, isEraser ? 1.0 : red, isEraser ? 1.0 : green, isEraser ? 1.0 : blue,ptsize);
	points.push_back(newPoint);

/*Its taking in 2 parameters a)mouse_x, mouse_y....
//red ,green ,blue are global variables.........
//isEraser tell weather we have selected eraser or not......
//Because if eraser is selected then value of rgb at that point must be(1,1,1)..

//So it simply sees the point and color of the point
//Makes a new object of class POINT........Initializes the object by seeing the values........ and pushes the POINT object in dots vector.............
*/
}

void draw_polygon();
//Drawline method for object of class line. using standard mathematical algorithm.
void line::drawLine(int x1,int y1,int x2,int y2)
{
        if(x1==x2)
        {
                for(float i=(y1<y2?y1:y2);i<=(y1>y2?y1:y2);i+=0.01)
                {
                        drawPoint(x1,i);
                }
                return;
        }
        else if(y1==y2)
        {
                for(float i=(x1<x2?x1:x2);i<=(x1>x2?x1:x2);i+=0.01)
                {
                        drawPoint(i ,y1);
                }
                return;
        }
        else
        {
                for(float t=0 ;t<=1 ;t+=.001)
                {
                    float x = x1 + (x2-x1)*t;
                    float y = y1 + (y2-y1)*t;
                    drawPoint(x,y);
                }
        }
        return;
}
//Draw Rectangle method for object of class Rectangle.
//x1,y1 and x2,y2 must be any 2 diagonal points of rectangle
void Rectangle::drawRectangle(int x1, int y1, int x2, int y2)
{
		drawLine(x1, y1, x2, y1);
		drawLine(x2, y1, x2, y2);
		drawLine(x2, y2, x1, y2);
		drawLine(x1, y2, x1, y1);
}
void drawRectangle_filled(int x1, int y1, int x2, int y2)
{
    if(y1>y2)
    {
        for(float i=y2 ; i<=y1 ;i++)
        {
            L.drawLine(x1, i, x2, i);
        }
    }
    else
    {
        for(float i=y1 ; i<=y2 ;i++)
        {
            L.drawLine(x1, i, x2, i);
        }
    }
}
//Drawcircle method for circle object.
void circle::drawCircle(int x1, int y1, int x2, int y2)
{
	float r = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	float x,y;

	for (float i = 0; i < (2 * pi); i += 0.001)
	{
	    //Using Parametric Equation of circle
		// circle is defined as x=r*cos(i) and y=r*sin(i)
		x = x1 + r * cos(i);
		y = y1 + r * sin(i);
		drawPoint(x, y);
	}
}

/*
void drawCircle_Filled(int x1, int y1, int x2, int y2)
{
    float r = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    float x,y;


    for(float j=0 ; j<=r ; j++)
    {
        for (float i = 0; i < (2 * pi); i += 0.001)
        {
            //Using Parametric Equation of circle
            // circle is defined as x=r*cos(i) and y=r*sin(i)
            x = x1 + j * cos(i);
            y = y1 + j * sin(i);
            drawPoint(x, y);
        }
    }
}
*/


/*
//It draws a dot of white color , wherever we want to erase
//if eraser size is 1 , then it will erase  square of 3*3
//if eraser size is 2 , then it will erase  square of 5*5
//if eraser size is x , then it will erase  square of (2x+1)*(2x+1)
//In drop down i paint application ,3 sizes: small medium large.
*/
void erase(int x, int y)// o(n^2) complexity with n^2 dots drawn near points just like n*n square matrix with point at middle of diagobal elements.
{
	for (int i = -eraserSize; i <= eraserSize; i++)
	{
		for (int j = -eraserSize; j <= eraserSize; j++)
		{
			drawPoint(x + i, y + j);
		}
	}
}
/*
//The difference is , drawBrush is used for AirBrush

//So when size of AirBrush is 4px , it puts 4 points very near to our selected (x,y)
//So when size of AirBrush is 8px , it puts 8 points very near to our selected (x,y)
//So when size of AirBrush is 12px , it puts 12 points very near to our selected (x,y)
//So when size of AirBrush is 16px , it puts 16 points very near to our selected (x,y)
//Its using rand() to get a random point very near to our point
*/

void drawBrush(int x, int y)
{
	for (int i = 0; i < brushSize; i++)
	{
		int randX = rand() % (brushSize + 1) - brushSize / 2 + x;
		int randY = rand() % (brushSize + 1) - brushSize / 2 + y;
		drawPoint(randX, randY);
	}
}
void drawRadialBrush(int x, int y)
{
	int xc = glutGet(GLUT_WINDOW_WIDTH) / 2;
	int yc = glutGet(GLUT_WINDOW_HEIGHT) / 2;
	int dx, dy;

	dx = xc - x;
	dy = yc - y;

	drawPoint(xc + dx, yc + dy);
	drawPoint(xc - dx, yc + dy);
	drawPoint(xc + dx, yc - dy);
	drawPoint(xc - dx, yc - dy);
	drawPoint(xc + dy, yc + dx);
	drawPoint(xc - dy, yc + dx);
	drawPoint(xc + dy, yc - dx);
	drawPoint(xc - dy, yc - dx);
}
void motion(int x, int y)//When mouse button is in pressed mode.It is either due to dragging points or using air brush.
{
	if (isEraser)
		erase(x, y);
	else
	{
		if (shape == 1)
		{
           if (isRadial)
				drawRadialBrush(x, y);
			else
				drawPoint(x, y);
		}
		if (shape == 5)
			drawBrush(x, y);
	}
}
void mouse(int bin, int state, int x, int y){
	if (bin == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (isRandom)	{
            srand(time(NULL)); //Standard practice is to use the result of a call to srand(time(0)) as the seed. However, time() returns a time_t value which vary everytime and hence the pseudo-random number vary for every program call. 
			red = float(rand()) / float(RAND_MAX);
			green = float(rand()) / float(RAND_MAX);
			blue = float(rand()) / float(RAND_MAX);
		}
		if (isEraser)
		{
		    undoHistory.push_back(points.size());
			erase(x, y);
		}
		else
		{

			if (shape == 1)
			{
				undoHistory.push_back(points.size());
				if (isRadial)
					drawRadialBrush(x, y);
				else
					drawPoint(x, y);
			}
			else if (shape == 5)
			{
			    undoHistory.push_back(points.size());
				drawBrush(x, y);
			}
			else
			{
				if (!isSecond)
				{
					tmpx = x;
					tmpy = y;
					isSecond = true;
				}
				else
				{
				    if (undoHistory.back() != points.size())
						undoHistory.push_back(points.size());
					if (shape == 2){
						L.drawLine(tmpx, tmpy, x, y);
					}
					else if (shape == 3){
						if(isfilled)
							drawRectangle_filled(tmpx,tmpy,x,y);
						else
							R.drawRectangle(tmpx, tmpy, x, y);
						isfilled=false;
					}
					else if (shape == 4){
						c.drawCircle(tmpx, tmpy, x, y);
					}
					else if(shape == 6)
				 		P1.draw_polygon(tmpx,tmpy,x,y);
					isSecond = false;
				}
			}
		}
		if (undoHistory.size() > 20)
		{
			undoHistory.pop_front();
		}
	}
}
void processColourMenu(int value)
{
	isSecond = false;
	isEraser = false;
	isRandom = false;

	switch (value)
	{
	case 1: // red
		red = 1.0;
		green = 0.0;
		blue = 0.0;
		break;
	case 2: // green
		red = 0.0;
		green = 1.0;
		blue = 0.0;
		break;
	case 3: // blue
		red = 0.0;
		green = 0.0;
		blue = 1.0;
		break;
	case 4: // purple
		red = 0.5;
		green = 0.0;
		blue = 0.5;
		break;
	case 5: // yellow
		red = 1.0;
		green = 1.0;
		blue = 0.0;
		break;
    case 6: // black
		red = 0.0;
		green = 0.0;
		blue = 0.0;
		break;
	case 7: // random
		isRandom = true;
		break;
	}
}
void processBrushSizeMenu(int value){
	shape = 5;
	isEraser = false;
	brushSize = value;
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);//For brush it sets mouse cursor as +.
}
void processEdgeMenu(int value){
	shape = 6;
	isEraser = false;
	isSecond = false;
	edge=value;
}
void processShapeMenu(int value)
{
	shape = value;
	isEraser = false;
	isSecond = false;
	isRadial = false;
	switch (shape)
	{
	case 1:
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		break;
	case 2:
	case 3:
	case 6:
	case 4:
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		break;
	}
}
void processFilledMenu(int value){
	shape=3;
	isEraser = false;
	isSecond = false;
	if(value)
		isfilled=true;
	else
		isfilled=false;
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}
void processBorderMenu(int value){
	isSecond = false;
	isEraser = false;
	ptsize=value;
}
void processEraserSizeMenu(int value)
{
	glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	eraserSize = value;
	isEraser = true;
}
void processMainMenu(int value)
{
	switch (value)
	{
	case 0:
		quit();
		break;
	case 1:
		clear();
		break;

	case 2:
		undo();
		break;
	case 3:
		redo();
		break;
	}
}
void processRadicalBrushMenu(int value)
{
	isRadial = value == 1 ? true : false;
}
void createOurMenu()
{
    /*
    int glutCreateMenu(void (*func)(int value));
    //glutCreateMenu creates a new pop-up menu function.The callback function for the menu is called when a menu entry from the menu is selected.The value passed to the callback is determined by the value for the selected menu entry.
    */
	int colourMenu = glutCreateMenu(processColourMenu);
	glutAddMenuEntry("Red", 1);
	/*
	  glutAddMenuEntry adds a menu entry to the bottom of the current menu.

        void glutAddMenuEntry(char *name, int value);
        name    :      ASCII character string to display in the menu entry.
        value   :      Value to return to the menu's callback function if the menu entry is selected.
        Description     :       glutAddMenuEntry adds a menu entry to the bottom of the current menu. The string name will be displayed for the newly added menu entry
	*/
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Purple", 4);
	glutAddMenuEntry("Yellow", 5);
	glutAddMenuEntry("Black", 6);
	glutAddMenuEntry("Random", 7);

	int sizeMenu = glutCreateMenu(processBrushSizeMenu);
	glutAddMenuEntry("4px", 4);
	glutAddMenuEntry("8px", 8);
	glutAddMenuEntry("12px", 12);
	glutAddMenuEntry("16px", 16);
	
	int sideMenu = glutCreateMenu(processEdgeMenu);
	glutAddMenuEntry("3", 3);
	glutAddMenuEntry("4", 4);
	glutAddMenuEntry("5", 5);
	glutAddMenuEntry("6", 6);
	glutAddMenuEntry("7", 7);
	glutAddMenuEntry("8", 8);
	glutAddMenuEntry("9", 9);
	glutAddMenuEntry("10", 10);
	glutAddMenuEntry("11", 11);
	glutAddMenuEntry("12", 12);
	glutAddMenuEntry("15", 15);
	glutAddMenuEntry("16", 16);
	glutAddMenuEntry("20", 20);
	
	int filledMenu = glutCreateMenu(processFilledMenu);
	glutAddMenuEntry("Fill_Area", 1);
	glutAddMenuEntry("Unfill_Area",0);
	int shapeMenu = glutCreateMenu(processShapeMenu);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddSubMenu("Rectangle",filledMenu);
	glutAddMenuEntry("Circle", 4);
	glutAddSubMenu("Airbrush", sizeMenu);
	glutAddSubMenu("Polygon",sideMenu);
	int radicalBrushMenu = glutCreateMenu(processRadicalBrushMenu);
	glutAddMenuEntry("True", 1);
	glutAddMenuEntry("False", 2);

	int borderMenu = glutCreateMenu(processBorderMenu);
	glutAddMenuEntry("2px",2);
	glutAddMenuEntry("4px",4);
	glutAddMenuEntry("5px",5);
	glutAddMenuEntry("6px",6);

	int eraserSizeMenu = glutCreateMenu(processEraserSizeMenu);
	glutAddMenuEntry("Small", 2);
	glutAddMenuEntry("Medium", 6);
	glutAddMenuEntry("Large", 10);

	glutCreateMenu(processMainMenu);
	glutAddSubMenu("Colour", colourMenu);
	glutAddSubMenu("Shapes", shapeMenu);
	glutAddSubMenu("Radical Paint Brush", radicalBrushMenu);
	glutAddSubMenu("Border", borderMenu);
	glutAddSubMenu("Eraser", eraserSizeMenu);
	glutAddMenuEntry("Undo", 2);
	glutAddMenuEntry("Redo", 3);
	glutAddMenuEntry("Clear", 1);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
	/*
	glutAttachMenu attaches a mouse button for the current window to the identifier of the current menu;
	glutDetachMenu detaches an attached mouse button from the current window.
	By attaching a menu identifier to a button, the named menu will be popped up when the user presses the specified button.
	button should be one of GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, and GLUT_RIGHT_BUTTON.

void glutAddSubMenu(char *name, int menu);
name    :   ASCII character string to display in the menu item from which to cascade the sub-menu.
menu    :   Identifier of the menu to cascade from this sub-menu.

*/
//Event HandLing Zone

void display(void)
{
    /*
    After we made some changes in our window ,they must be immediately displayed  ,hence this function would be called
    Before calling it , it displayed the last time window , but now since we have made changes , many points have been drawn and many are removed ...
    Hence to reflect those changes .....we first clear whatever is shown on the window and then again print all the dots in the entire vector
    */
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);// specifies the red, green, blue, and alpha values used by glClear to clear the color buffers.,RGB=111 specifies white.
	glClear(GL_COLOR_BUFFER_BIT);   //clear buffers to preset values.
	glPointSize(ptsize);//Setting the size of the point

	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < points.size(); i++)
	{
		glColor3f(points[i].getR(), points[i].getG(), points[i].getB());    //set the current color
		glVertex2i(points[i].getX(), points[i].getY());                     //To draw the point
	}
	glEnd();

	glutSwapBuffers();
/*
With double-buffering, two such buffers are used, the front buffer and the back buffer. The front buffer is the one that contains the frame you are currently seeing. The back buffer is the one in which the computer is currently busy rendering the next frame. When that rendering is done, the two buffers are swapped, instantly updating the image you see to be the next-now-current frame.

This technique ensures that you only ever see fully rendered frames rather than "work in progress".

glutSwapBuffers() is the GLUT function that instructs the computer that you are done with the current frame and the buffers should be swapped so that that frame be displayed and so that you can begin working on the next.
*/
}
/*
Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(int w, int h)
{
	window_w = w;
	window_h = h;

	//Before Calling gluOrtho2D we need to have the below 2 lines of code
	glMatrixMode(GL_PROJECTION);//Select projection matrix 
	glLoadIdentity();//Reset projection matrix.
	//Rectangular Region in World to be displayed.
	gluOrtho2D(0, w, 0, h); //(Left,Right,Bottom,Top )

	//These Functions can be Used Later if we Want to implement Functionalities like ZoomIn and ZoomOut

	glMatrixMode(GL_MODELVIEW);
	glViewport(0 , 0, w, h);   // Area on Screen to Display Objects ( width_start , height start ,width end , height end )

}

void keyboard(unsigned char key, int xIn, int yIn)
{
	isSecond = false;
	switch (key)
	{
    //Come Out From the Paint Application when we press (q or escape)
	case 'q':
	case 27: // 27 is the esc key
		quit();
		break;
	case 'c':
		clear();
		break;
	case '+':
		if (shape == 5 && !isEraser)
		{
			if (brushSize < 16)
				brushSize += 4;
			else
			{
				cout<< "[Warning] Airbrush's size cannot be larger. It is already the largest.\n";
			}
		}
		else if (isEraser)
		{
			if (eraserSize < 10)
				eraserSize += 4;
			else
			{
				cout<< "[Warning] Eraser's size cannot be larger. It is already the largest.\n";
			}
		}
		break;
	case '-':
		if (shape == 5 && !isEraser)
		{
			if (brushSize > 4)
				brushSize -= 4;
			else
			{
				cout<< "[Warning] Airbrush's size cannot be smaller. It is already the smallest.\n";
			}
		}
		else if (isEraser)
		{
			if (eraserSize > 2)
				eraserSize -= 4;
			else
			{
				cout<< "[Warning] Eraser's size cannot be smaller. It is already the smallest.\n";
			}
		}
		break;
    case 'u':
		undo();
		break;
	case 'r':
		redo();
		break;
	}
}
void Timer(int val){
	glutPostRedisplay();//To give window paint request which in turn activate display function.
	glutTimerFunc(0, Timer, 0);//Timer function will be called back after every specific interval.
}
void callbackInit(){			
	glutDisplayFunc(display);//glutDisplayFunc is called whenever your window must be redrawn.
	glutReshapeFunc(reshape);//Reshape will be called whenver you changes size i.e minimize,maximize.
	glutKeyboardFunc(keyboard);
	/*Sets the Keyboard callback for the current window.glutKeyboardFunc(void( * callback )( unsigned char key, int x, int y ));callback Client function for keyboard event.*/
	glutMouseFunc(mouse);
	/*
	glutMouseFunc sets the mouse callback for the current window. When a user presses and releases mouse buttons in the window, each press and each release generates a mouse callback. The state parameter is either GLUT_UP or GLUT_DOWN indicating whether the callback was due to a release or press respectively. The x and y callback parameters indicate the window relative coordinates when the mouse button state changed. If a GLUT_DOWN callback for a specific button is triggered, the program can assume a GLUT_UP callback for the same button will be generated (assuming the window still has a mouse callback registered) when the mouse button is released even if the mouse has moved outside the window.

If a menu is attached to a button for a window, mouse callbacks will not be generated for that button.

During a mouse callback, glutGetModifiers may be called to determine the state of modifier keys when the mouse event generating the callback occurred.
	*/
	glutMotionFunc(motion);
/*
The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
The x and y callback parameters indicate the mouse location in window relative coordinates.
*/
	glutTimerFunc(0, Timer, 0);

}




int main(int argc, char **argv)
{
    //glutInit will initialize the GLUT library and negotiate a session with the window system.
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	/*Itequests a display with the specified mode, such as color mode GLUT_RGBA or  double buffering (GLUT_SINGLE, GLUT_DOUBLE)*/

	//Setting Size and position of the Window
	glutInitWindowSize(window_w, window_h);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Paint");//Giving name to our Window


	callbackInit();

	//To print all the instructions
	printGuide();

	//creating Our Graphical User Interface
	createOurMenu();

	//Goes in an infinite loop till we give call to exit(0) and in this loop gives call to display function
	glutMainLoop();

	return (0);
}
void printGuide(){
	cout << "#########################################################################\n"
			  << "#                    Welcome to use this Paint tool!                    #\n"
			  << "#########################################################################\n"
			  << "A list of commands:\n"
			  << "Right click\t"
			  << "-> show menu\n"
			  << "Left click\t"
			  << "-> choose option\n"
			  << "Menu \"Color\"\t"
			  << "-> You can choose Red, Green, Blue, Yellow or Random, the default color is Red.\n"
			  << "Menu \"Shapes\"\t"
			  << "-> The default shape is Point.\n"
			  << "\tPoint\t\t"
			  << "-> draw a dot at the point clicked with the mouse. Clicking and dragging will draw points constantly like free-form drawing.\n"
			  << "\tLine\t\t"
			  << "-> draw a line between two subsequently clicked points.\n"
			  << "\tRectangle\t"
			  << "-> draw a rectangle.\n"
			  << "\tCircle\t\t"
			  << "-> draw a circle centered at the position of the first click, with its radius set by a second click.\n"
			  << "\tAirbrush\t"
			  << "-> draw multiple points as brush around the clicked point. There are four options of size.\n"
			  << "Menu \"Eraser\"\t"
			  << "-> erase the points by clicking and dragging.\n"
			  << "Menu \"Clear\"\t"
			  << "-> clear all the points\n"
			  << "Menu \"Quit\"\t"
			  << "-> close the window.\n"
			  << "Keyboard 'q'\t"
			  << "-> close the window.\n"
			  << "Keyboard 'esc'\t"
			  << "-> close the window.\n"
			  << "Keyboard 'c'\t"
			  << "-> clear all the points and clear the history.\n"
			  << "Keyboard '+'\t"
			  << "-> larger size of eraser or brush.\n"
			  << "Keyboard '-'\t"
			  << "-> smaller size of eraser or brush.\n"
			  << "################################# Paint #################################" << endl;
}

