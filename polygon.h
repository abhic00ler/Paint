#include"shapes.h"
#include"line.h"
#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED
class Polygon : public shape{    //Base Class
    protected :
        int edges,length;
        float peri, interior, exterior;
    public:
        Polygon();
        Polygon(int s, int m);
        void draw_polygon(int x1, int y1, int x2, int y2);
        void perimeter();
        void int_ext();
};
/*class Triangle : public Polygon{
    private:
        float a, b, c;
    public:
        Triangle(){   //By default equilateral triangle
            edges=3;
            length=0;
        }
        void Area(int m){
            length=m;
            area=(sqrt(3)*length*length)/4;
            display_area();
        }
        void Area(float a1, float b1, float c1){   //Function Overloading
            a=a1;
            b=b1;
            c=c1;
            Display_dim();
            float k;
            float s=(a+b+c)/2;
            if((k=(s*(s-a)*(s-b)*(s-c)))>0){
                area=sqrt(k);
                display_area();
            }
            else
                cout<<"Invalid inputs"<<"\n";
        }
        void Perimeter(){
            peri=a+b+c;
            display_peri();
        }
        void Display_dim(){
            cout<<"Given triangle with dimensions:(3 sides)"<<a<<", "<<b<<", "<<c<<"\n";
        }

};*/
class Rectangle : public Polygon, public line{
    protected:
        int dx1,dy1,dx2,dy2;
    public:
        Rectangle();
	void setp(int x1,int y1,int x2,int y2);
    void drawRectangle(int x1,int y1,int x2,int y2);
};
#endif

