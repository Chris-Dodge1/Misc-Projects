"""
find_route.py

CSE 5120 Programming Project 1

As needed by the assignment, a tree-search route finder utilizing Uniform-Cost Search (UCS) 
is implemented to determine the shortest way between two cities using only the fringe 
queue (no explored set).
"""

import sys
import heapq
from dataclasses import dataclass, field
from typing import Dict, List, Tuple, Optional

# Data structures

AdjacencyList = Dict[str, List[Tuple[str, float]]]

@dataclass
class Node:
    """A node in the search tree."""
    city: str
    parent: Optional["Node"]
    step_distance: float        # distance from parent to city
    path_cost: float            # total distance from start to city
    
@dataclass(order=True)
class PrioritizedItem:
    """An item in the priority queue."""
    priority: float
    count: int
    node: Node = field(compare=False)
    
# Functions

def format_distance(d: float) -> str:
    """Format distance to 3 decimal places."""
    if float(int(d)) == d:
        return str(int(d))
    return str(d)

def reconstruct_path(goal_node: Node) -> List[Node]:
    """Reconstruct the path from start to goal."""
    path: List[Node] = []
    current = goal_node
    while current is not None:
        path.append(current)
        current = current.parent
    path.reverse()
    return path

# Graph Building

def load_map(filename: str) -> AdjacencyList:
    """Load the map from a file into an adjacency list."""
    graph: AdjacencyList = {}
    
    try:
        with open(filename, "r") as f:
            for raw_line in f:
                line = raw_line.strip()
                if not line:
                    continue
                if line == "END OF INPUT":
                    break
                
                parts = line.split()
                if len(parts) != 3:
                    # Ignore malformed lines
                    continue
                
                city1, city2, distance_str = parts
                try:
                    distance = float(distance_str)
                except ValueError:
                    # Ignore lines with invalid distance
                    continue
                
                # Add edges to the graph (undirected)
                graph.setdefault(city1, []).append((city2, distance))
                graph.setdefault(city2, []).append((city1, distance))
    except FileNotFoundError:
        print(f"Error: File '{filename}'.", file=sys.stderr)
        sys.exit(1)
        
    return graph

# Tree search: Uniform-Cost Search (UCS)

def on_current_path(node: Node, city: str) -> bool:
    """Check if the city is already on the current path to avoid cycles."""
    current = node
    while current is not None:
        if current.city == city:
            return True
        current = current.parent
    return False

def uniform_cost_search(graph: AdjacencyList, start: str, goal: str) -> Optional[Node]:
    """Perform Uniform-Cost Search to find the shortest path from start to goal."""
    root = Node(city=start, parent=None, step_distance=0.0, path_cost=0.0)
    
    frontier: List[PrioritizedItem] = []
    counter = 0  # To break ties in priority queue
    
    heapq.heappush(frontier, PrioritizedItem(priority=0.0, count=counter, node=root))
    
    while frontier:
        item = heapq.heappop(frontier)
        node = item.node
        
        # Goal test
        if node.city == goal:
            return node
        
        # Expand node
        for neighbor, dist in graph.get(node.city, []):
            # Avoid cycles by checking if neighbor is on the current path
            if on_current_path(node, neighbor):
                continue
            
            child = Node(
                city=neighbor,
                parent=node,
                step_distance=dist,
                path_cost=node.path_cost + dist
            )
            counter += 1
            heapq.heappush(
                frontier, 
                PrioritizedItem(priority=child.path_cost, count=counter, node=child)
            )
            
    return None  # No path found

# Main function

def print_no_route():
    """Print message when no route is found."""
    print("distance: infinity")
    print("route:")
    print("none")
    
def print_zero_route(city: str):
    """Print message when start and goal are the same."""
    print("distance: 0 km")
    print("route:")
    print(f"{city} to {city}, 0 km")
    
def print_route(goal_node: Node):
    """Print the found route."""
    path = reconstruct_path(goal_node)
    
    total_distance = goal_node.path_cost
    print(f"distance: {format_distance(total_distance)} km")
    print("route:")
    
    # Print each step in the path
    for i in range(len(path) - 1):
        from_city = path[i].city
        to_city = path[i + 1].city
        step_distance = path[i + 1].step_distance   # distance from from_city to to_city
        print(f"{from_city} to {to_city}, {format_distance(step_distance)} km")
    
def main(): 
    # Check command-line arguments
    # find_route inpute_file start_city goal_city
    if len(sys.argv) != 4:
        print("Usage: python find_route.py input_file start_city goal_city", file=sys.stderr)
        sys.exit(1)
        
    input_file = sys.argv[1]
    origin = sys.argv[2]
    destination = sys.argv[3]
    
    # Handle case where origin and destination are the same
    if origin == destination:
        print_zero_route(origin)
        return
    
    # Load the map
    graph = load_map(input_file)
    
    # Run UCS
    goal_node = uniform_cost_search(graph, origin, destination)
    
    # Print results
    if goal_node is None:
        print_no_route()
    else:
        print_route(goal_node)
        
if __name__ == "__main__":
    main()