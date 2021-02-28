#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

// Map class
class Map{
public:
  const static int mapWidth = 150;
  const static int mapHeight = 300;

  vector<vector<double>> map = GetMap();
  vector<vector<int>> grid = MaptoGrid();
  vector<vector<int>> heuristic = GenerateHeuristic();

private:
  // Read the file and get the map
  vector<vector<double> > GetMap(){
    vector<vector<double> > mymap(mapHeight, vector<double>(mapWidth));
    ifstream myReadFile;
    myReadFile.open("map.txt");

    while (!myReadFile.eof()) {
      for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
          myReadFile >> mymap[i][j];
        }
      }
    }
    return mymap;
  }

  // Convert the map to 1's and 0's
  vector<vector<int> > MaptoGrid(){
    vector<vector<int>> grid(mapHeight, vector<int>(mapWidth));
    
    for (int i = 0; i < mapHeight; i++){
    	for (int j = 0; j < mapWidth; j++){
    		if (map[i][j] < 0)
    			grid[i][j] = 0;
    		else
    			grid[i][j] = 1;
    	}
    }
    return grid;
  }

  vector<vector<int>> GenerateHeuristic(){
  	vector<vector<int>> heuristic(mapHeight, vector<int>(mapWidth));
  	int goal[2] = {60, 50};

  	for (int i = 0; i < mapHeight; i++){
  		for (int j = 0; j < mapWidth; j++){
  			int x_d = abs(i - goal[0]);
  			int y_d = abs(j - goal[1]);

  			heuristic[i][j] = x_d + y_d;
  		}
  	}

  	return heuristic;
  }
};

// Planner class
class Planner : Map{
public:
  int start[2] = {230, 145};
  int goal[2] = {60, 50};
  int cost = 1;

  vector<vector<int>> movements = {
    {-1, 0},
    {0, -1},
    {1, 0},
    {0, 1}
  };

  string movements_arrows[4] = {"^", "<", "v", ">"};
  vector<vector<int>> path;
};

// print2DVector function prints 2D vectors of any data type
template <typename T>
void print2DVector(T Vec){
  for (int i = 0; i < Vec.size(); i++){
    for (int j = 0; j < Vec[0].size(); j++){
      cout << Vec[i][j] << ' ';
    }
    cout << endl;
  }
}

// search function generates the expansion list
Planner search(Map map, Planner planner)
{
  int x = planner.start[0];
  int y = planner.start[1];
  int g = 0;
  int f = g + map.heuristic[x][y];

  // Status flags
  bool found = false;
  bool no_way = false;

  // Create a closed 2 array filled with 0s and first element 1
  vector<vector<int>> closed(map.mapHeight, vector<int>(map.mapWidth));
  closed[planner.start[0]][planner.start[1]] = 1;

  // Store the expansions
  vector<vector<int>> open;
  open.push_back({f, g, x, y});

  int x_n, y_n;

  // Create expand vector
  vector<vector<int>> expand(map.mapHeight, vector<int>(map.mapWidth, -1));
  int count = 0;

  // Create a vector for actions
  vector<vector<int>> action(map.mapHeight, vector<int>(map.mapWidth, -1));

  while (!found && !no_way){
    // Check if there is no way
    if (open.size() == 0){
      no_way = true;
      cout << "Failed to reach a goal" << endl;
    }
    // If still there is a way to continue
    else {
      // Place next node at the end of the open list
      sort(open.begin(), open.end());
      reverse(open.begin(), open.end());

      // Create a vector for next node
      vector<int> next;
      next = open.back();
      open.pop_back();

      g = next[1];
      x = next[2];
      y = next[3];
      f = g + map.heuristic[x][y];

      // Fill the expand array
      expand[x][y] = count++;

      if (x == planner.goal[0] && y == planner.goal[1]){
        found = true;
        cout << "Goal found: [" << f << ", " << g << ", " << x << ", " << y << "]" << endl;
      }
      else {
        for (int i = 0; i < planner.movements.size(); i++){
          x_n = x + planner.movements[i][0];
          y_n = y + planner.movements[i][1];

          if (x_n >= 0 && x_n < map.mapHeight && y_n >= 0 && y_n < map.mapWidth){
            if (closed[x_n][y_n] == 0 and map.grid[x_n][y_n] == 0) {
              int g_n = g + planner.cost;
              open.push_back({g_n + map.heuristic[x_n][y_n], g_n, x_n, y_n});
              closed[x_n][y_n] = 1;
              action[x_n][y_n] = i;
            }
          }
        }
      }
    }
  }

  cout << "Expansion Vector:" << endl; 
  print2DVector(expand);

  if (found){
    // Create a vector to save path
    vector<vector<string> > policy(map.mapHeight, vector<string>(map.mapWidth, "-"));

    x = planner.goal[0];
    y = planner.goal[1];
    policy[x][y] = '*';

    while (x != planner.start[0] || y != planner.start[1]){
      x_n = x - planner.movements[action[x][y]][0];
      y_n = y - planner.movements[action[x][y]][1];

      planner.path.push_back({x_n, y_n});
      policy[x_n][y_n] = planner.movements_arrows[action[x][y]];
      
      x = x_n;
      y = y_n;
    }

    cout << "Path:" << endl;
    print2DVector(policy);
  }

  return planner;
}

void visualization(Map map, Planner planner)
{
    //Graph Format
    plt::title("Path");
    plt::xlim(0, map.mapHeight);
    plt::ylim(0, map.mapWidth);

    // Draw every grid of the map:
    for (double x = 0; x < map.mapHeight; x++) {
        cout << "Remaining Rows= " << map.mapHeight - x << endl;
        for (double y = 0; y < map.mapWidth; y++) {
            if (map.map[x][y] == 0) { //Green unkown state
                plt::plot({ x }, { y }, "g.");
            }
            else if (map.map[x][y] > 0) { //Black occupied state
                plt::plot({ x }, { y }, "k.");
            }
            else { //Red free state
                plt::plot({ x }, { y }, "r.");
            }
        }
    }

    // TODO: Plot start and end states in blue colors using o and * respectively

    
    // TODO: Plot the robot path in blue color using a .

    
    //Save the image and close the plot
    plt::save("./Images/Path.png");
    plt::clf();
}

int main()
{
  // Instantiate map and planner objects
  Map map;
  Planner planner;

  // Generate the shortest Path using the Astar algorithm
  planner = search(map, planner);

  // Plot the Map and the path generated
  visualization(map, planner);

  return 0;
}
