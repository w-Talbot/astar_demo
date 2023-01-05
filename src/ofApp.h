#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
    
        vector<int> reconstruct_path(vector<int> cameFrom, vector<int>current);
        void a_star();
        int heuristic(int a_x, int a_y, int b_x, int b_y);
        vector<vector<int>> remove_vector_element(vector<vector<int>> vec, vector<int>elm);
        vector<int> get_all_neighbors(vector<int> vec);
        void color_check(int x, int y);
    
        
    
    //a*

    vector<int>start, last_checked_node;
    vector<vector<int>> open_set, closed_set;
    int start_x,start_y,goal_x,goal_y;
    bool in_closed, in_open;
    
    
    
    
    static const int numX=5;
    static const int numY=5;

  
    vector<vector<int>>grid_points_x_y_c;
    
    int spacingX, spacingY;
    
    
    float box_size_w, box_size_h;
    
    //needs to move from origin...
    int startingX;
    int startingY;
    
    
    //stores the amount of movement
    float stepSize;
		
};
