#include"shapes.h"
#include"point.h"
#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED
class line:public Point{
	protected:
		float sx1,sy1,sx2,sy2;
	public:
		line();
        void drawLine(int sx,int sy,int ex,int ey);

};
#endif


