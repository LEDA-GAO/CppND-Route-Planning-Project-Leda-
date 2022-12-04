#include "route_planner.h"
#include <algorithm>
#include <queue>

using std::sort;

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node=&m_Model.FindClosestNode(start_x,  start_y);
    end_node=&m_Model.FindClosestNode(end_x,  end_y);


}


// Implement the CalculateHValue method.
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance(*end_node);

}



void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    RouteModel::Node* neighbor_nodes = nullptr;
    current_node->FindNeighbors();
    for(RouteModel::Node* neighbor_nodes: current_node->neighbors){
        neighbor_nodes->parent=current_node;
        neighbor_nodes->h_value=RoutePlanner::CalculateHValue(neighbor_nodes);
        neighbor_nodes->g_value=current_node->g_value+neighbor_nodes->distance(*current_node);
        open_queue.emplace(neighbor_nodes);
        //open_list.push_back(neighbor_nodes);
        neighbor_nodes->visited=true;
    }
}


RouteModel::Node *RoutePlanner::NextNode() {
    //This is the sorting method to find the next node. Sorting the node will
    // take up to O(nlog(n)).  
    //sort(open_list.begin(),open_list.end(),[](const RouteModel::Node *Node1, const RouteModel::Node *Node2)
    //    {return  (Node1->h_value+Node1->g_value)> (Node2->h_value+Node2->g_value);});
    //RouteModel::Node *next_node = open_list.back();
    //open_list.pop_back();

    //Try to use priority queue to improve the efficiency.  The time complexity will be O(1) here. 
    RouteModel::Node *next_node = open_queue.top();
    open_queue.pop();
    return next_node;
}


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
    //while (open_list.size()>0)
    while (open_queue.size()>0)
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