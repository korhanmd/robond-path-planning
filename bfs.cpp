#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

// Map class
class Map{
public:
  const static int mapWidth = 6;
  const static int mapHeight = 5;
  vector<vector<int>> grid = {
    {0, 1, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0}
  };
};

// Planner class
class Planner : Map{
public:
  int start[2] = {0, 0};
  int goal[2] = {mapHeight - 1, mapWidth - 1};
  int cost = 1;

  vector<vector<int>> movements = {
    {-1, 0},
    {0, -1},
    {1, 0},
    {0, 1}
  };

  string movements_arrows[4] = {"^", "<", "v", ">"};
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
void search(Map map, Planner planner)
{
  int x = planner.start[0];
  int y = planner.start[1];
  int g = 0;

  // Status flags
  bool found = false;
  bool no_way = false;

  // Create a closed 2 array filled with 0s and first element 1
  vector<vector<int>> closed(map.mapHeight, vector<int>(map.mapWidth));
  closed[planner.start[0]][planner.start[1]] = 1;

  // Store the expansions
  vector<vector<int>> open;
  open.push_back({ g, x, y });

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

      g = next[0];
      x = next[1];
      y = next[2];

      // Fill the expand array
      expand[x][y] = count++;

      if (x == planner.goal[0] && y == planner.goal[1]){
        found = true;
        cout << "Goal found: [" << g << ", " << x << ", " << y << "]" << endl;
      }
      else {
        for (int i = 0; i < planner.movements.size(); i++){
          x_n = x + planner.movements[i][0];
          y_n = y + planner.movements[i][1];

          if (x_n >= 0 && x_n < map.mapHeight && y_n >= 0 && y_n < map.mapWidth){
            if (closed[x_n][y_n] == 0 and map.grid[x_n][y_n] == 0) {
              open.push_back({g + planner.cost, x_n, y_n});
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
      policy[x_n][y_n] = planner.movements_arrows[action[x][y]];
      x = x_n;
      y = y_n;
    }

    cout << "Path:" << endl;
    print2DVector(policy);
  }
}

int main()
{
  // Instantiate map and planner objects
  Map map;
  Planner planner;

  // Search for the expansions
  search(map, planner);

  return 0;
}
