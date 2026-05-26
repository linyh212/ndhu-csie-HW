/*class Point {
public:
  Point(double _x, double _y);
  double getX();
  double getY();
};

class Segment {
public:
  Segment(Point p1, Point p2);
  Point getEnd(int n);
  double length()
}*/

class Path{	
struct dot{
	double x;
	double y;
};
public:
	Path(Segment *seg, int n){
		dot arr[100];
		double sum=0;
		bool con=0;
		for(int i=0; i<n; i++){
			sum+=arr[i].length();
			for(int j=0; j<i; j++){
				if((seg[i].getEnd(1).getX()==arr[j].x&&seg[i].getEnd(1).getY()==arr[j].y)||(seg[i].getEnd(0).getX()!=seg[i-1].getEnd(1).getX()&&seg[i].getEnd(0).getY()!=seg[i-1].getEnd(1).getY()&&i>0)){
					cout<<-1<<endl;
					con=1;
					break;	
				}
			}
			arr[i].x=seg[i].getEnd(0).getX();
			arr[i].y=seg[i].getEnd(0).getY();
		}
		if(!con) cout<<sum<<endl;
	}	
};
