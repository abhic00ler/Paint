#include"point.h"
#ifndef SHAPE_H_INCLUDED 
#define SHAPE_H_INCLUDED


class shape : public Point{
	protected:
		int flag;               //Flag 1 for curve and 0 for Polygons.
	public:
		shape();
		void setflag(int f);


};
#endif

