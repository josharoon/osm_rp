#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    
    this->start_node = &m_Model.FindClosestNode(start_x,start_y);
    this->end_node = &m_Model.FindClosestNode(end_x,end_y);
    



}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    

     


    

    return node->distance(*end_node);
    
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    //std::cout <<"Add Neighbours";
    current_node->FindNeighbors();
    auto neighbors = start_node->neighbors;
    for (int i = 0; i < neighbors.size(); i++)
    {
       
        //RouteModel::Node neighbour = *current_node->neighbors[i];
        //neighbour.parent=
        //neighbour.h_value=CalculateHValue(&neighbour);
        //neighbour.g_value=neighbour.distance(*start_node);
        //open_list.push_back(neighbour);
        //neighbour.visited=true;


        auto neighbors = start_node->neighbors;
        neighbors[i]->parent=current_node;
        neighbors[i]->h_value=CalculateHValue(neighbors[i]);
        neighbors[i]->g_value=neighbors[i]->distance(*start_node);
        open_list.push_back(neighbors[i]);
        neighbors[i]->visited=true;


    }
    

}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

bool wayToSort(RouteModel::Node *Nodei,RouteModel::Node *Nodej)
    {
        //std::cout  << Nodei->g_value <<Nodei->h_value;
        float i = Nodei->h_value+Nodei->g_value;
        float j = Nodej->h_value+Nodej->g_value;
        //std::cout << "i=" << i << " , j=" <<j <<"\n";
        
       return  i<j; 
    }
  

RouteModel::Node *RoutePlanner::NextNode() {

    //std::cout <<"NextNode";
    sort(open_list.begin(),open_list.end(), wayToSort);
    RouteModel::Node *lowest_sum = *open_list.begin();
    // for (size_t i = 0; i < open_list.size(); i++)
    // {
    //     std::cout << open_list.at(i)->h_value+open_list.at(i)->g_value;
    //     std::cout << " , ";
        
    // }
    //std::cout << "lowest =" << open_list.at(0)->h_value+open_list.at(0)->g_value << "\n";
    std::cout << "lowest =" << open_list.at(0)->h_value +open_list.at(0)->g_value << "\n";
    

    open_list.erase(open_list.begin());
    //std::cout << "new lowest =" << open_list.at(0)->h_value+open_list.at(0)->g_value;
    std::cout << "lowest sum =" << lowest_sum->h_value+lowest_sum->g_value;

    return lowest_sum;

}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.

    // while starting node not found
    std::cout <<"ConstructFinalPath";
    while (current_node->x != start_node->x  & current_node->y !=start_node->y)
    {
        distance += current_node->distance(*(current_node->parent));
        
    
        path_found.insert(path_found.begin(), *current_node);
        current_node=current_node->parent;

    }
    path_found.insert(path_found.begin(), *current_node);
    
        




    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.
    
    std::cout <<" Running A* search";
    
    start_node->visited=true;
    open_list.push_back(start_node);
    //this->AddNeighbors(current_node);
    AddNeighbors(start_node);
    current_node=start_node;
    
    
    
    while (open_list.size()>0)
       
    {
        
        current_node=NextNode();
        
        
        
        std::cout <<" distance "<< current_node->distance(*end_node);
              
        
        if (current_node->distance(*end_node)==0)

        {
            
             m_Model.path = ConstructFinalPath(current_node);
             return;
            
        }
        AddNeighbors(current_node);
        
        
        
    }
    
    //nEXTNode
    // while (current_node->x != end_node->x  & current_node->y !=end_node->y)
    // {
    //     /* code */
    //     AddNeighbors(current_node);
    //     current_node =  NextNode();
    
    // }

    //this->m_Model.path=(ConstructFinalPath(current_node));
    //m_Model.path = ConstructFinalPath(end_node);
    
    
    //if End node

            //ConstructFinalPath

            //m_Model.path store final path.
}