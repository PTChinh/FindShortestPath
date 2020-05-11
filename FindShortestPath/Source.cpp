#include <iostream>
#include <fstream>
#include <string>
#include <vector> 
#include <queue>
#include <set>

using namespace std;

struct VisitOldFuel {
	bool isVisit = false;
	int pos;
};

class QItem {
public:
	int row;
	int col;
	int dist;
	QItem() {};
	QItem(int x, int y, int w)
		: row(x), col(y), dist(w)
	{
	}
};

void printMap(vector<vector<int>> arr)
{
	for (int i = 0; i < arr.size(); ++i)
	{
		for (int j = 0; j < arr[0].size(); ++j)
		{
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}
}

int findPath(vector<vector<int>> &maze, int si, int sj, QItem &currPos, int findPoint, vector<QItem> &currPath)
{
	QItem source(si, sj, 0);

	int N = maze.size(); // Rows
	int	M = maze[0].size(); // Cols

	queue<vector<QItem>> Q;
	vector<QItem> path;
	path.push_back(source);

	Q.push(path);

	// To keep track of visited QItems. Marking 
	// blocked cells as visited. 
	vector<vector<bool>> visited;
	for (int i = 0; i < N; i++)
	{
		vector<bool> row;
		for (int j = 0; j < M; j++)
		{
			if (maze[i][j] == 0)
				row.push_back(true);
			else
				row.push_back(false);
		}
		visited.push_back(row);
	}
	visited[source.row][source.col] = true;

	while (!Q.empty())
	{
		path = Q.front();
		Q.pop();

		QItem last = path[path.size() - 1];

		QItem up, down, left, right;

		// Destination found; 
		if ((last.row != source.row || last.col != source.col) && maze[last.row][last.col] == findPoint)
		{
			currPath = path;
			currPos = last;
			return last.dist;
		}

		// moving up 
		if (last.row - 1 >= 0 && visited[last.row - 1][last.col] == false)
		{
			up.row = last.row - 1;
			up.col = last.col;
			up.dist = last.dist + 1;
			vector<QItem> newPath(path);
			newPath.push_back(up);
			Q.push(newPath);
			visited[last.row - 1][last.col] = true;
		}

		// moving down 
		if (last.row + 1 < N && visited[last.row + 1][last.col] == false)
		{
			down.row = last.row + 1;
			down.col = last.col;
			down.dist = last.dist + 1;
			vector<QItem> newPath(path);
			newPath.push_back(down);
			Q.push(newPath);
			visited[last.row + 1][last.col] = true;
		}

		// moving left 
		if (last.col - 1 >= 0 && visited[last.row][last.col - 1] == false)
		{
			left.row = last.row;
			left.col = last.col - 1;
			left.dist = last.dist + 1;
			vector<QItem> newPath(path);
			newPath.push_back(left);
			Q.push(newPath);
			visited[last.row][last.col - 1] = true;
		}

		// moving right 
		if (last.col + 1 < M && visited[last.row][last.col + 1] == false)
		{
			right.row = last.row;
			right.col = last.col + 1;
			right.dist = last.dist + 1;
			vector<QItem> newPath(path);
			newPath.push_back(right);
			Q.push(newPath);
			visited[last.row][last.col + 1] = true;
		}
	}

	return -1;
}

int main()
{
	fstream mapInput, filePath;

	mapInput.open("map.txt", ios::in); // File map input
	filePath.open("path.txt", ios::out); // File path output

	int rows, cols; // Map size rows x cols
	
	int y, x; // Start position
	int gasUnits; // Gas units
	QItem currPos; // Current position
	int totalStep = 0; // Total steps
	
	
	vector<pair<int,int>> impediment; // impediment - 0
	vector<pair<int, int>> lane; // lane - 1
	vector<pair<int, int>> gas; // gas - 2
	vector<pair<int, int>> star; // star - 3
	
	mapInput >> y >> x;
	mapInput >> gasUnits;
	mapInput >> rows >> cols;

	currPos.row = y - 1;
	currPos.col = x - 1;
	currPos.dist = 0;
	int maxGasUnits = gasUnits;

	vector<vector<int>> maze; // Map matrix

	for (int i = 0; i < rows; i++)
	{
		vector<int> row;
		for (int j = 0; j < cols; j++)
		{
			int pos;
			mapInput >> pos;
			row.push_back(pos);
			
			switch (pos)
			{
			case 0:
				impediment.push_back(make_pair(i, j));
				break;
			case 1:
				lane.push_back(make_pair(i, j));
				break;
			case 2:
				gas.push_back(make_pair(i, j));
				break;
			default:
				star.push_back(make_pair(i, j));
				break;
			}
		}
		maze.push_back(row);
	}

	cout << y << " " << x << endl;
	cout << gasUnits << endl;
	cout << rows << " " << cols << endl;

	//printMap(maze);
	
	cout << "Total impediment: " << impediment.size() << endl;
	cout << "Total lane: " << lane.size() << endl;
	cout << "Total gas: " << gas.size() << endl;
	cout << "Total star: " << star.size() << endl;

	cout << "--------------------------------------------------------" << endl << endl;

	//Find path
	while (gasUnits > 0)
	{
		VisitOldFuel visitOldFuel;
		visitOldFuel.isVisit = false;
		visitOldFuel.pos = 0;

		QItem starPos(0, 0, 0);
		QItem fuelPos(0, 0, 0);

		vector<QItem> starPath; // Current path from source to star
		vector<QItem> fuelPath; // Current path from source to fuel

		if (gasUnits == maxGasUnits)
		{
			int dStar = findPath(maze, currPos.row, currPos.col, starPos, 3, starPath);
			if (dStar == -1)
			{
				cout << "Can find path, current gas units: " << gasUnits << endl;
				break;
			}

			if (dStar > gasUnits)
			{
				int dFuel = findPath(maze, currPos.row, currPos.col, fuelPos, 2, fuelPath);
				if (dStar == -1)
				{
					cout << "Can find path, current gas units: " << gasUnits << endl;
					break;
				}
				if (dFuel > gasUnits)
				{
					cout << "Can find path, current gas units: " << gasUnits << endl;
					break;
				}
				if (dFuel <= gasUnits)
				{
					gasUnits = maxGasUnits; // Refill gas units
					totalStep += dFuel;
					currPos = fuelPos;
					for (int i = 1; i < fuelPath.size(); i++)
					{
						filePath << fuelPath[i].row + 1 << " " << fuelPath[i].col + 1 << endl;
						cout << fuelPath[i].row + 1 << " " << fuelPath[i].col + 1 << endl;
					}
					cout << "Current gas units when collect gas: " << gasUnits << endl;
				}
			}
			else
			{
				gasUnits -= dStar;
				totalStep += dStar;
				currPos = starPos;
				maze[starPos.row][starPos.col] = 1;
				for (int i = 1; i < starPath.size(); i++)
				{
					filePath << starPath[i].row + 1 << " " << starPath[i].col + 1 << endl;
					cout << starPath[i].row + 1 << " " << starPath[i].col + 1 << endl;
				}
				cout << "Current gas units when collect star: " << gasUnits << endl;
			}
		}
		else
		{
			int dFuel = findPath(maze, currPos.row, currPos.col, fuelPos, 2, fuelPath);
			if (dFuel == -1)
			{
				cout << "Can find path, current gas units: " << gasUnits << endl;
				break;
			}
			if (dFuel > gasUnits)
			{
				cout << "Can find path, current gas units: " << gasUnits << endl;
				break;
			}
			if (dFuel <= gasUnits)
			{
				gasUnits = maxGasUnits; // Refill gas units
				totalStep += dFuel;
				currPos = fuelPos;
				for (int i = 1; i < fuelPath.size(); i++)
				{
					filePath << fuelPath[i].row + 1 << " " << fuelPath[i].col + 1 << endl;
					cout << fuelPath[i].row + 1 << " " << fuelPath[i].col + 1 << endl;
				}
				cout << "Current gas units when collect gas: " << gasUnits << endl;
			}
		}		
	}

	mapInput.close();
	filePath.close();
	

	cout << "Done!" << endl;
	cout << "Total Steps is: " << totalStep << endl;

	system("pause");
	return 0;
}