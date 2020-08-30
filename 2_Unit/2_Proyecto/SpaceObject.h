#include <iostream>
#include <vector>

using namespace std;

class SpaceObject
{
	private:
	
	public:
		int x;
		int y;
		int dx;
		int dy;
		int nSize;
		float angle;
		vector<pair<int, int>> vecModelAsteroid;
	
		SpaceObject(int = 0, int = 0, int = 0, int = 0, int = 0, float = 0.0f);
};
