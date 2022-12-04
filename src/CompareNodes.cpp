#include "CompareNodes.h"

bool CompareNodes::operator()(RouteModel::Node* Node1, RouteModel::Node* Node2){
    return (Node1->g_value+Node1->h_value)>(Node2->g_value+Node2->h_value);
}