#include "route_planner.h"
#include <algorithm>

using std::sort;

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node=&m_Model.FindClosestNode(start_x,  start_y);
    end_node=&m_Model.FindClosestNode(end_x,  end_y);


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
    RouteModel::Node* neighbor_nodes = nullptr;
    current_node->FindNeighbors();
    for(RouteModel::Node* neighbor_nodes: current_node->neighbors){
        neighbor_nodes->parent=current_node;
        neighbor_nodes->h_value=RoutePlanner::CalculateHValue(neighbor_nodes);
        neighbor_nodes->g_value=current_node->g_value+neighbor_nodes->distance(*current_node);
        open_list.push_back(neighbor_nodes);
        neighbor_nodes->visited=true;
    }
}

// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

RouteModel::Node *RoutePlanner::NextNode() {
    sort(open_list.begin(),open_list.end(),[](const RouteModel::Node *Node1, const RouteModel::Node *Node2)
        {return  (Node1->h_value+Node1->g_value)> (Node2->h_value+Node2->g_value);});
    RouteModel::Node *next_node = open_list.back();
    open_list.pop_back();
    return next_node;
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
    path_found.push_back(*current_node);
    RouteModel::Node *front_node = nullptr;
    // TODO: Implement your solution here.
    while (current_node->x!=start_node->x && current_node->y!=start_node->y)
    {      
        front_node = current_node->parent;
        distance += front_node->distance(*current_node);
        path_found.push_back(*front_node);
        current_node=front_node;
    }
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    std::reverse(path_found.begin(),path_found.end());
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
    RouteModel::Node *next_node = nullptr;
    std::vector<RouteModel::Node> final_path;
    // TODO: Implement your solution here.
    current_node = start_node;
    current_node->h_value=CalculateHValue(current_node);
    current_node->g_value=0;
    current_node->visited=true;
    AddNeighbors(current_node);
    while (open_list.size()>0)
    {
        next_node=NextNode();
        current_node=next_node;
        AddNeighbors(current_node);
        if(current_node->x==end_node->x && current_node->y==end_node->y){
            final_path = RoutePlanner::ConstructFinalPath(current_node);
            std::cout<<"path size= "<<final_path.size()<<std::endl;
            break;
        }
    }
    m_Model.path=final_path;

}