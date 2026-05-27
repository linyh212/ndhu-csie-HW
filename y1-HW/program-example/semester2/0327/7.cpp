#include <iostream>
using namespace std;
class Cube {
private:
  int height, length, width;
public:
  int getHeight() {
    return height;
  }
  int getWidth() {
    return width;
  }
  int getLength() {
    return length;
  }
  void setHeight(int h) {
    if(h > 0)
      height = h;
  }
  void setWidth(int w) {
    if(w > 0)
      width = w;
  }
  void setLength(int l) {
    if(l > 0)
      length = l;
  }
};

class CubeVol : public Cube {
public:
  void read_input() {
    int h, w, l;
    cin >> h >> w >> l;
    setHeight(h);
    setWidth(w);
    setLength(l);
  }
  void display() {
    cout << getHeight() * getLength() * getWidth() << endl;
  }
};

int main()
{
  CubeVol cv;
  cv.read_input();
  cv.display();
}