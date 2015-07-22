#ifdef _MSC_VER
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <queue>
#include <vector>
#include <utility>

using namespace std;

class Tuple {
public:
	int x, y, n;
};


void getInfo(int***, int*, int*, char*, int*, int*, int*, int*);
void generate_weight(int***, int, int, int***, int, int, int, int);
void find_shortest_path(int**, int, int, int, int, vector<int>&, vector<int>&);
void show_result(int, int, int, int, vector<int>&, vector<int>&, int**, int**, int, int);

int main()
{
	int** table;
	int width;
	int height;
	int** weight;
	int sx, sy;
	int dx, dy;
	vector<int> recordx, recordy;
	char filename[1024];
	cout << "Please enter the input file: ";
	cin >> filename;
	//filename = "benchmark2.txt";
	getInfo(&table, &width, &height, filename, &sx, &sy, &dx, &dy);
	/*
	for(int i = 0; i < width; i++)
	{
	for(int j = 0; j < height; j++)
	{
	printf("%d ", table[j][i]);
	}
	cout << endl;
	}
	*/
	generate_weight(&weight, width, height, &table, sx, sy, dx, dy);
	find_shortest_path(weight, width, height, dx, dy, recordx, recordy);
	/*
	for(int j = height - 1; j >= 0; j--)
	{
	for(int i = 0; i < width; i++)
	{
	if(i == dx && j == dy)
	printf(" x ");
	else
	printf("%2d ", weight[i][j]);
	}
	cout << endl;
	}
	*/
	show_result(sx, sy, dx, dy, recordx, recordy, weight, table, width, height);
	cout << "The result has outputed as the file: output.m" << endl;
	cout << "Please Enter Any Key to Left...";
	cin.ignore(1000, '\n');
	cin.get();
	return 0;
}

void getInfo(int*** table, int* width, int* height, char* filename, int* sx, int* sy, int* dx, int* dy)
{
	fstream fp;
	fp.open(filename, ios::in);
	if (!fp)
	{
		cout << "Fail to open file: " << filename << endl;
		exit(0);
	}
	char* buffer = new char[10240];
	int i = 0;
	while (fp.getline(buffer, 10240))
	{
		if (strlen(buffer) > 6)
		{
			if (i == 0)
			{
				sscanf(buffer, "%*s %dx%d", width, height);
				*table = new int*[*width];
				for (int j = 0; j < *width; j++)
					(*table)[j] = new int[*height];
				for (int j = 0; j < *width; j++)
				for (int k = 0; k < *height; k++)
					(*table)[j][k] = 0;
			}
			else if (i == 1)
			{
				int x, y, len;
				sscanf(buffer, "%*s%n", &len);
				for (char* p = buffer + len; sscanf(p, " (%d , %d)%n", &x, &y, &len) != EOF; p += len)
				{
					(*table)[x][y] = 1;
				}
			}
			else if (i == 2)
			{
				int x, y;
				sscanf(buffer, "%*s (%d , %d)", &x, &y);
				(*table)[x][y] = 2;
				*sx = x;
				*sy = y;
			}
			else if (i == 3)
			{
				int x, y;
				sscanf(buffer, "%*s (%d , %d)", &x, &y);
				(*table)[x][y] = 3;
				*dx = x;
				*dy = y;
			}
			i++;
		}
	}
	fp.close();
}

void generate_weight(int*** weight, int width, int height, int*** table, int sx, int sy, int dx, int dy)
{
	int dir[4][2] = { { 0, 1 }, { 1, 0 }, { -1, 0 }, { 0, -1 } };
	int** processed;
	processed = new int*[width];
	for (int i = 0; i < width; i++)
		processed[i] = new int[height];
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			if ((*table)[i][j] == 1)
				processed[i][j] = 1;
			else
				processed[i][j] = 0;
	*weight = new int*[width];
	for (int i = 0; i < width; i++)
		(*weight)[i] = new int[height];
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			(*weight)[i][j] = -1;
	int x = sx;
	int y = sy;
	queue<Tuple> que;
	Tuple tul;
	tul.x = sx;
	tul.y = sy;
	tul.n = 0;
	que.push(tul);
	while (que.size() != 0 && processed[dx][dy] == 0 /*&& que1.size() != 0*/)
	{
		tul = que.front();
		que.pop();
		x = tul.x;
		y = tul.y;
		if (processed[x][y])
			continue;
		processed[x][y] = 1;
		(*weight)[x][y] = tul.n;
		if (x == dx && y == dy)
			break;
		for (int i = 0; i < 4; i++)
			if (dir[i][0] + x >= 0 && dir[i][1] + y >= 0 && dir[i][0] + x < width && dir[i][1] + y < height)
				if (!processed[x + dir[i][0]][y + dir[i][1]])
				{
					Tuple temp;
					temp.x = x + dir[i][0];
					temp.y = y + dir[i][1];
					temp.n = tul.n + 1;
					que.push(temp);
				}
	}
}

void find_shortest_path(int** weight, int width, int height, int dx, int dy, vector<int>& recordx, vector<int>& recordy)
{
	queue<Tuple> que;
	int dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	int** processed;
	processed = new int*[width];
	for (int i = 0; i < width; i++)
		processed[i] = new int[height];
	for (int i = 0; i < width; i++)
	for (int j = 0; j < height; j++)
		processed[i][j] = 0;
	Tuple tul;
	tul.x = dx;
	tul.y = dy;
	tul.n = weight[dx][dy];
	que.push(tul);
	recordx.push_back(tul.x);
	recordy.push_back(tul.y);
	int x;
	int y;
	while (que.size() != 0)
	{
		tul = que.front();
		que.pop();
		x = tul.x;
		y = tul.y;
		if (weight[x][y] == 0)
			break;
		if (processed[x][y])
			continue;
		processed[x][y] = 1;
		for (int i = 0; i < 4; i++)
			if (x + dir[i][0] >= 0 && x + dir[i][0] < width && y + dir[i][1] >= 0 && y + dir[i][1] < height)
				if (weight[x + dir[i][0]][y + dir[i][1]] == tul.n - 1)
				{
					Tuple temp;
					temp.x = x + dir[i][0];
					temp.y = y + dir[i][1];
					temp.n = weight[temp.x][temp.y];
					que.push(temp);
					recordx.push_back(temp.x);
					recordy.push_back(temp.y);
					break;
				}
	}
}

void show_result(int sx, int sy, int dx, int dy, vector<int>& recordx, vector<int>& recordy, int** weight, int**table, int width, int height)
{
	fstream fp;
	char* filename = "output.m";
	fp.open(filename, ios::out);
	if (!fp)
	{
		cout << "Fail to open file: " << filename << endl;
		exit(0);
	}
	fp << "\tfigure('units', 'normalized', 'outerposition', [0 0 1 1 ]);\n\tclear;\n\tclf;\n\taxis equal;\n\thold on;\n\tgrid on;\n";
	int x, y;
	int w, h;
	string offsetx, offsety;
	w = 4;
	h = 4;
	offsetx = "2";
	offsety = "2";
	for(int i = 0; i < width; i++)
		for(int j = 0; j < height; j++)
		{
			if(table[i][j] == 0)
			{
				fp << "rectangle('Position',[" << 4 * i << "," << 4 * j << "," << w << "," << h << "],'FaceColor','w')" << endl;
				if(weight[i][j] != -1)
					fp << "text(" << 4 * i << "+" << offsetx << "," << 4 * j << "+" << offsety << ",'" << weight[i][j] << "', 'HorizontalAlignment', 'center', 'VerticalAlignment', 'middle')" << endl;
			}
			else
			{
				fp << "rectangle('Position',[" << 4 * i << "," << 4 * j << "," << w << "," << h << "],'FaceColor','k')" << endl;
			}
		}
	while (recordx.size() != 0 || recordy.size() != 0)
	{
		x = recordx.back();
		y = recordy.back();
		recordx.pop_back();
		recordy.pop_back();
		if ((x == sx && y == sy) || (x == dx && y == dy))
		{
			fp << "rectangle('Position',[" << 4 * x << "," << 4 * y << "," << w << "," << h << "],'FaceColor','y')" << endl;
			if (x == sx && y == sy)
				fp << "text(" << 4 * x << "+" << offsetx << "," << 4 * y << "+" << offsety << ",'S', 'HorizontalAlignment', 'center', 'VerticalAlignment', 'middle')" << endl;
			else if (x == dx && y == dy)
				fp << "text(" << 4 * x << "+" << offsetx << "," << 4 * y << "+" << offsety << ",'T', 'HorizontalAlignment', 'center', 'VerticalAlignment', 'middle')" << endl;
		}
		else if (table[x][y] == 1)
		{
			fp << "rectangle('Position',[" << 4 * x << "," << 4 * y << "," << w << "," << h << "],'FaceColor','k')" << endl;
		}
		else // others
		{
			fp << "rectangle('Position',[" << 4 * x << "," << 4 * y << "," << w << "," << h << "],'FaceColor','r')" << endl;
			fp << "text(" << 4 * x << "+" << offsetx << "," << 4 * y << "+" << offsety << ",'" << weight[x][y] << "', 'HorizontalAlignment', 'center', 'VerticalAlignment', 'middle')" << endl;
		}
	}
	fp.close();

}

