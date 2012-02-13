#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

class myRange {
	public:
		int start;
		int end;
		Scalar rgb;
		myRange();
		myRange(int x, int y);
		bool within_range(int x);		
};

myRange::myRange() {}

myRange::myRange(int x, int y) {
	start = x;
	end = y;
}

bool myRange::within_range(int x) {
	if(x >= start && x <= end) {
		return true;
	} else return false;
}

int main(int argc, char* argv[]) {
	ifstream input;
	if(argc < 2) {
		cout << "usage: ./main test_case_input image_input\n";
		return 1;
	}

	input.open(argv[1]);
	
	char* holder = new char;
	char* start = new char;
	char* end = new char;
	char* r = new char;
	char* g = new char;
	char* b = new char;

	vector<myRange> l;
	myRange* l_ptr;
	myRange range;
	Scalar rgb;

	while(input.peek() != EOF) {
		input.getline(holder, 255);
		
		sscanf(holder, "%s %s %s %s %s\n", start, end, r, g, b);
		printf("%s->%s %s,%s,%s\n", start, end, r, g, b);
		range = myRange(atoi(start), atoi(end));
		rgb.val[0] = atoi(r);
		rgb.val[1] = atoi(g);
		rgb.val[2] = atoi(b);
		range.rgb = rgb;
		
		l.push_back(range);
	}
	cout << "input processed\n";

	Scalar intensity;
	Mat orig_img = imread(argv[2]);
	IplImage* new_img = new IplImage(); //check constructor
	int rgb_buf[3];
	char* buf = new char;
	int i, x, y;
	for( i = 0; i < l.size(); i++) {
		cout << "scanning image\n";
		*new_img = orig_img;
		l_ptr = &l.at(i);
		rgb_buf[0] = l_ptr->rgb.val[0];
		rgb_buf[1] = l_ptr->rgb.val[1];
		rgb_buf[2] = l_ptr->rgb.val[2];

		for( x = 0; x < orig_img.rows; x++) {
			for( y = 0; y < orig_img.cols; y++) {
				intensity = cvGet2D(new_img, x, y);
				if(l.at(i).within_range(intensity.val[0])) {
					intensity = cvGet2D(new_img, x, y);
					intensity.val[0] = rgb_buf[0];
					intensity.val[1] = rgb_buf[1];
					intensity.val[2] = rgb_buf[2];
					cvSet2D(new_img, x, y, intensity);
				}
			}
		}
		cout << "image scanned\n";
		sprintf(buf, "%d %d %d.tiff", rgb_buf[0], rgb_buf[1], rgb_buf[2]);
		cvSaveImage(buf, new_img);
		cout << "new image saved\n";
	}

	input.close();
	delete holder;
	delete start;
	delete end;
	delete r;
	delete g;
	delete b;
	delete new_img;
	delete buf;
	l_ptr = NULL;
	
	return 0;
}
