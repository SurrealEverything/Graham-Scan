#include<iostream>
#include<fstream>
#include<vector>
#include<stack>
#include<algorithm>
using namespace std;
ofstream out("data.out");
struct Point
{
	int x, y;
};
vector<Point> points, noncollinearPoints;
stack<Point> hull;
void printPoint(Point pt)
{
	out << "(" << pt.x << ", " << pt.y << ")" << endl;
}
void read()
{
	ifstream in("data.in");
	Point temp;
	while(in >> temp.x >> temp.y)
		points.push_back(temp);
}
int findBottomMost()
{
	int minIter = 0;
	for (int i = 1; i < points.size(); ++i)
	{
		if (points[i].y < points[minIter].y)//bottom most point
			minIter = i;
		else if ((points[i].y == points[minIter].y))
			if (points[i].x < points[minIter].x)//left most point
				minIter = i;
	}
	return minIter;
}
int distanceSquared(Point p1, Point p2)
{
	return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
}
int orientation(Point p, Point q, Point r)
{
	int val = (q.x - p.x) * (r.y - q.y) - (q.y - p.y) * (r.x - q.x);
	if (val == 0) return 0;  //colinear
	return (val > 0) ? -1 : 1; //left turn or right turn
}
bool comp(Point p1, Point p2)//A point p1 comes before p2 in sorted ouput if p2 has a larger polar angle
{
	int o = orientation(points[0], p1, p2);
	if (o == 0)
		return (distanceSquared(points[0], p2) >= distanceSquared(points[0], p1));//keep the farthest point at the end
	return (o == -1);
}
void sortByPolar()
{
	int pivot = findBottomMost();
	if (pivot != 0)
		iter_swap(points.begin() + pivot, points.begin());//Place the bottom-most point at first position
	sort(points.begin() + 1, points.end(), comp);//polar angle sort
}
void clearCollinear()
{
	//If two or more points make same angle with p0, remove all but the one that is farthest from p0
	vector<Point>::iterator i;
	noncollinearPoints.push_back(*points.begin());
	for(i = points.begin() + 1; i != points.end()-1; ++i)
		if (orientation(points[0], *i, *(i + 1)) != 0)//noncollinear
			noncollinearPoints.push_back(*i);
	noncollinearPoints.push_back(*i);
}
Point nextToTop(stack<Point> &S)
{
	Point p = S.top();
	S.pop();
	Point res = S.top();
	S.push(p);
	return res;
}
void createHull()
{
	hull.push(noncollinearPoints[0]);
	hull.push(noncollinearPoints[1]);
	hull.push(noncollinearPoints[2]);
}
void printHull()
{
	while (!hull.empty())
	{
		printPoint(hull.top());
		hull.pop();
	}
}
void GrahamScan()
{
	sortByPolar();
	clearCollinear();
	if (noncollinearPoints.size() < 3)//line
	{
		hull.push(noncollinearPoints[0]);
		hull.push(noncollinearPoints[1]);
		return;
	}
	else
	{
		createHull();
		for (int i = 3; i < noncollinearPoints.size(); ++i)
		{
			while (orientation(nextToTop(hull), hull.top(), noncollinearPoints[i]) != -1)//right turn or collinear points
				hull.pop();
			hull.push(noncollinearPoints[i]);
		}
	}
}
int main()
{
	read();
	GrahamScan();
	printHull();
	return 0;
}
