#include"polygon.h"
Polygon::Polygon(){
         flag=0;
         edges=3;
	 length=0;
}
Polygon::Polygon(int s, int m){
        flag=0;
        edges=s;
	length=m;
}
void Polygon::perimeter(){
        peri=edges*length;
}
void Polygon::int_ext(){
       interior=180*(edges-2)/edges;
       exterior=180-interior;
}
Rectangle::Rectangle(){
    flag=0;
	edges=4;
}
void Rectangle::setp(int x1,int y1,int x2,int y2){
	dx1=x1;
	dy1=y1;
	dx2=x2;
	dy2=y2;
}

