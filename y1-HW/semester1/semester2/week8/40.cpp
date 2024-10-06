#include <bits/stdc++.h>
using namespace std;

class Point{
public:
	double x, y;
	Point(double nx=0.0, double ny=0.0): x(nx), y(ny){}	
};

class Mountain{
private:
	vector<Point> p;
public:
	void addPoint(double x, double y){
		p.push_back(Point(x, y));
	}
	double calLength(){
		if(p.empty()) return 0.0;
		sort(p.begin(), p.end(), [](const Point& n, const Point& m){
			return n.x>m.x;
		});
		double highest=0.0, line=0.0;
		for(size_t i=1; i<p.size(); i++){
			if(p[i].y>highest){
				double a=(p[i].y-p[i-1].y)/(p[i].x-p[i-1].x);
				double b=p[i].y-a*p[i].x;
				double temp=(highest-b)/a;
				line+=sqrt(pow(p[i].x-temp, 2)+pow(p[i].y-highest, 2));
				highest=p[i].y;
			}
		}
		return line;
	}
};

int main(){
    int c;
    cin>>c;
    for(int i=0; i<c; i++){
        int n;
        cin>>n;
        Mountain mountain;
        for(int j=0; j<n; j++){
            double x, y;
            cin>>x>>y;
            mountain.addPoint(x, y);
        }
        cout<<fixed<<setprecision(2)<<mountain.calLength()<<endl;
    }
    return 0;
}

