#include "ofApp.h"


/*
 f(n) = g(n) + h(n)
 
 G cost is the distance from the starting node
 H cost is how far away from the end node
 F cost is H cost plus G cost
 
 */


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(0);
    
    //Grid Dimensions:
    spacingX = ofGetWidth()/ numX;
    spacingY = ofGetHeight()/ numY;
    
    startingX = spacingX /2;
    startingY = spacingY /2;
    
    
    stepSize = 20;

    box_size_w = 25;
    box_size_h = 25;
    
    /*
     Populating the grid point location coords (x,y) and color value (default 0):
     0 = White
     1 = Red : Block
     2 = Green : Goal
     3 = Blue : Start
     4 = Reset
     */
    for ( int x = 0; x < numX; x++){
        for (int y = 0; y < numY; y++){
            
            int tmp_loc_x = startingX + spacingX * x ;
            int tmp_loc_y = startingY + spacingY * y ;

            vector<int> x_y_c_init;
            x_y_c_init.push_back(tmp_loc_x);
            x_y_c_init.push_back(tmp_loc_y);
            x_y_c_init.push_back(0);
            
            grid_points_x_y_c.push_back(x_y_c_init);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    

    for ( int i = 0; i < grid_points_x_y_c.size(); i++){
            
        if(grid_points_x_y_c[i][2] == 0){
            //white
            ofSetColor(255);
        }else if(grid_points_x_y_c[i][2] == 1){
            //red - blocked
            ofSetColor(255, 0 , 0);
        }else if(grid_points_x_y_c[i][2] == 2){
            //blue - start
            ofSetColor(0, 0 , 255);
        }else if(grid_points_x_y_c[i][2] == 3){
            //green - finish
            ofSetColor(0, 255 , 0);
        }else if(grid_points_x_y_c[i][2] == 4){
            grid_points_x_y_c[i][2] = 0;
            ofSetColor(255);
        }else if(grid_points_x_y_c[i][2] == 5){
            ofSetColor(245, 236, 66);
        }
           
            ofDrawRectangle(grid_points_x_y_c[i][0], grid_points_x_y_c[i][1],box_size_w, box_size_h);
    }
    
    if(final_path.size() > 0){
        for(int j = 0; j < final_path.size()-2; j+=2){
            ofSetColor(0, 255, 0);
            ofSetLineWidth(box_size_w);
            ofDrawLine(final_path[j], final_path[j+1], final_path[j+2], final_path[j+3]);
        }
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    //CLEAR ALL VALUES:
    if(key == 'c' || key == 'C'){
        grid_points_x_y_c.clear();
        for ( int x = 0; x < numX; x++){
            for (int y = 0; y < numY; y++){

                int tmp_loc_x = startingX + spacingX * x ;
                int tmp_loc_y = startingY + spacingY * y ;

                vector<int> x_y_c_init;
                x_y_c_init.push_back(tmp_loc_x);
                x_y_c_init.push_back(tmp_loc_y);
                x_y_c_init.push_back(0);

                grid_points_x_y_c.push_back(x_y_c_init);

            }
        }
        final_path.clear();
        walls.clear();
        start.clear();
        last_checked_node.clear();
        open_set.clear();
        closed_set.clear();
        
    }
    
    
    //RUN a*
    if(key == 'r' || key == 'R'){
        a_star();
    }
    
}

//--------------------------------------------------------------
void ofApp::reconstruct_path(vector<vector<int>> cameFrom, vector<int>end){

    
    
    
    final_path.push_back(end[0]);//current goal_x
    final_path.push_back(end[1]);//current goal_y
   
    vector<vector<int>> temp_vals;
    temp_vals.push_back(end);
    
    for(int i = cameFrom.size()-1; i >= 0; i--){
        
        if(
           cameFrom[i][0] == temp_vals[0][4] &&
           cameFrom[i][1] == temp_vals[0][5]){
            
            final_path.push_back(cameFrom[i][0]);
            final_path.push_back(cameFrom[i][1]);
            temp_vals.clear();
            temp_vals.push_back(cameFrom[i]);
            
        }
    }
    
    

}

//--------------------------------------------------------------
bool ofApp::check_for_walls(int x, int y, vector<int>vec_to_check){

    bool response = false;
    for(int i = 0; i < vec_to_check.size(); i+=2){
        if(x == vec_to_check[i] && y == vec_to_check[i+1]){
            response = true;
        }
    }
    
    return response;
}

//--------------------------------------------------------------
int ofApp::heuristic(int a_x, int a_y, int b_x, int b_y){

    int tmp_x = abs(a_x - b_x);
    int tmp_y = abs(a_y - b_y);
    int distance = tmp_x + tmp_y;
    
    return distance;
}
//--------------------------------------------------------------
vector<vector<int>> ofApp::remove_vector_element(vector<vector<int>> vec, vector<int>elm){
    
    for(int i = vec.size() - 1; i >= 0; i-- ){
        if(vec[i] == elm){

            vec.erase(vec.begin()+i);
        }
    }
    return vec;
}
//--------------------------------------------------------------
void ofApp::color_check(int x, int y, int sx, int sy, int gx, int gy){
   
    for (int i = 0; i < grid_points_x_y_c.size(); i++){
        
        //if the point to check is the start or goal; skip:
        if (grid_points_x_y_c[i][0] == sx && grid_points_x_y_c[i][1] == sy || grid_points_x_y_c[i][0] == gx && grid_points_x_y_c[i][1] == gy){
            continue;
        }
        
        if(grid_points_x_y_c[i][0] == x && grid_points_x_y_c[i][1] == y ){
            
            grid_points_x_y_c[i][2] = 5;
            ofSetColor(245, 242, 66);
            ofDrawRectangle(x, y,box_size_w, box_size_h);

            break;
        }
    }
   
}

//--------------------------------------------------------------
vector<int> ofApp::get_all_neighbors(vector<int> vec){
    int tmp_x = vec[0];
    int tmp_y = vec[1];
    vector<int> neighbors;
    int tmpf, tmpg, tmph;
    bool is_wall = false;
    
    //LEFT:
    if(tmp_x >= (startingX + spacingX)){
        
        is_wall = check_for_walls(tmp_x - ( spacingX), tmp_y, walls);
        
        if(is_wall == false){
            neighbors.push_back(tmp_x - ( spacingX));
            neighbors.push_back(tmp_y);
            
            tmpg = heuristic(tmp_x - ( spacingX), tmp_y, start_x, start_y);
            tmph = heuristic(tmp_x - ( spacingX), tmp_y, goal_x, goal_y);
            tmpf = tmph + tmpg;
            
            neighbors.push_back(tmpf);
            neighbors.push_back(tmpg);
        }
    }
    //RIGHT:
    if(tmp_x <= (ofGetWidth() - (startingX + spacingX))){
        
        is_wall = check_for_walls(tmp_x + (spacingX),tmp_y, walls);
        
        if(is_wall == false){
            neighbors.push_back(tmp_x + (spacingX));
            neighbors.push_back(tmp_y);
            
            tmpg = heuristic(tmp_x + (spacingX), tmp_y, start_x, start_y);
            tmph = heuristic(tmp_x + (spacingX), tmp_y, goal_x, goal_y);
            tmpf = tmph + tmpg;
            neighbors.push_back(tmpf);
            neighbors.push_back(tmpg);
        }
        
    }
    //ABOVE:
    if(tmp_y >= (startingY + spacingY)){
            
        is_wall = check_for_walls(tmp_x,tmp_y - ( spacingY),walls);
         
        if(is_wall == false){
            neighbors.push_back(tmp_x);
            neighbors.push_back(tmp_y - ( spacingY));
            
            tmpg = heuristic(tmp_x, tmp_y - ( spacingY), start_x, start_y);
            tmph = heuristic(tmp_x, tmp_y - ( spacingY), goal_x, goal_y);
            tmpf = tmph + tmpg;
            neighbors.push_back(tmpf);
            neighbors.push_back(tmpg);
        }
        
    }
    //BELOW:
    if(tmp_y <= (ofGetHeight() - (startingY + spacingY))){
            
            
        is_wall = check_for_walls(tmp_x,tmp_y + ( spacingY),walls);
          
        if(is_wall == false){
            neighbors.push_back(tmp_x);
            neighbors.push_back(tmp_y + ( spacingY));
            
            tmpg = heuristic(tmp_x, tmp_y + ( spacingY), start_x, start_y);
            tmph = heuristic(tmp_x, tmp_y + ( spacingY), goal_x, goal_y);
            tmpf = tmph + tmpg;
            neighbors.push_back(tmpf);
            neighbors.push_back(tmpg);
        }
    }
    

    return neighbors;
}

//--------------------------------------------------------------
void ofApp::a_star(){
    
    
    /*
     Node structure will be:
     [0],[1],[2      ],[3      ],[4         ],[5         ]
     [x],[y],[f_score],[g_score],[previous x],[previous y]
     */
    
    int x = 0;
    int y = 1;
    int f = 2;
    int g = 3;
    
    
    //Get all walls first:
    for(int j = 0; j < grid_points_x_y_c.size(); j++){
        if(grid_points_x_y_c[j][2] == 1){
            walls.push_back(grid_points_x_y_c[j][0]);
            walls.push_back(grid_points_x_y_c[j][1]);
        }
    }
   
    
    //Gets the start and goal coordinates:
    bool start_set = false;
    bool goal_set = false;
    
    for (int i = 0; i < grid_points_x_y_c.size(); i++){
        
        if(grid_points_x_y_c[i][2] == 2){
            start_x = grid_points_x_y_c[i][0];
            start_y = grid_points_x_y_c[i][1];
            start_set = true;
        }else if(grid_points_x_y_c[i][2] == 3){
            goal_x = grid_points_x_y_c[i][0];
            goal_y = grid_points_x_y_c[i][1];
            goal_set = true;
        }
    }
    
    if(!start_set || !goal_set){
        /*
         If one parameter is missing (start or goal), defaults are used.
         These will be the first an last square (based on a 10x10 20spc grid).
        */
        start_x = startingX;
        start_y = startingY;
        goal_x = numX * spacingX;
        goal_y = numY * spacingY;
        
    }
    
    
    start.push_back(start_x); //start x postion
    start.push_back(start_y); //start y position
    
    int initg = 0; //we're at starting node
    int inith = heuristic(start_x, start_y, goal_x, goal_y);
    int initf = inith + initg;
    
    start.push_back(initf);
    start.push_back(initg);
    start.push_back(0); //start "previous" x
    start.push_back(0);//start "previous " y
    
    
    open_set.push_back(start);
    
    while (open_set.size() > 0) {
        
        
        int winner = 0;
        
        for (int i = 0; i < open_set.size(); i++){
            
            if(open_set[i][f] < open_set[winner][f]){
                winner = i;
            }
            if(open_set[i][f] == open_set[winner][f]){
                if(open_set[i][g] > open_set[winner][g]){
                    winner = i;
                }
            }
        }
            vector<int> current;
            current = open_set[winner];
            
            last_checked_node = current;
        
        color_check(current[x], current[y], start_x, start_y, goal_x, goal_y);
            
        //Check if finished:
            if(current[x] == goal_x && current[y] == goal_y){
                std::cout<< "DONE!" << endl;
                reconstruct_path(closed_set, current);
                break;
            }
            
            //Move best option to closed set:
            open_set = remove_vector_element(open_set, current);
            closed_set.push_back(current);
            
            //Check all neighbors of the current node:
            vector<int> neighbors = get_all_neighbors(current);
            
            for(int i = 0; i < neighbors.size(); i+=4){
                //neighbors are stored in one vector in x ,y, x, y
                
                in_closed = false;
                for(int j = 0; j < closed_set.size(); j ++){
                    //check if closed set DOESNT include neighbor already
                    if(closed_set[j][x] == neighbors[i] && closed_set[j][y] == neighbors[i + 1]){
                        in_closed = true;
                        break;
                    }
                }
                if(in_closed){
                    continue;
                }

                if(!in_closed){
                    int temp_gscore = current[g] + heuristic(neighbors[i], neighbors[i+1], current[x], current[y]);
                    
                    //IS this a better path than before?
                    in_open = false;
                    for(int k = 0; k < open_set.size(); k ++){
                        if(open_set[k][x] == neighbors[i] && open_set[k][y] == neighbors[i + 1]){
                            in_open = true;
                            break;
                        }
                    }
                    
                    vector<int> tmp_nbor;
                    //Add x,y coords to temporary vector:
                    tmp_nbor.push_back(neighbors[i]);
                    tmp_nbor.push_back(neighbors[i+1]);
                    //Add fscore and G score:
                    tmp_nbor.push_back(neighbors[i+2]);
                    tmp_nbor.push_back(neighbors[i+3]);
                    
                    //Add previous step
                    tmp_nbor.push_back(current[x]);
                    tmp_nbor.push_back(current[y]);
                    
                                      
                    if(!in_open){
                        open_set.push_back(tmp_nbor);
                    }else if(temp_gscore >= tmp_nbor[g]){
                        continue;
                    }
                }
            }
        }
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    //Rotate through colors when clicked:
    for(int i = 0; i < grid_points_x_y_c.size(); i++){
            if(
               x > grid_points_x_y_c[i][0] &&
               x < grid_points_x_y_c[i][0]+ box_size_w &&
               y > grid_points_x_y_c[i][1] &&
               y < grid_points_x_y_c[i][1]+ box_size_h){
        
                grid_points_x_y_c[i][2] +=1;
               
        
                std::cout << "x: " << grid_points_x_y_c[i][0] << " y: " << grid_points_x_y_c[i][1] << endl;
            }
    }
    
    
}






