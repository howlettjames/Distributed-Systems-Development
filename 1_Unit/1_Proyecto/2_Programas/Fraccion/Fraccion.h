#ifndef FRACCION_H_
#define FRACCION_H_

class Fraccion
{
	private:
		int num;
		int den;

 	public:
 		Fraccion(int numm = 0, int denn = 0);
 		void setNum(int numm);
 		void setDen(int denn);
 		double getDouble();
 		void minimized();
};

#endif