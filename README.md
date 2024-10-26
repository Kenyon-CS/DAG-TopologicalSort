# Dag-TopologicalSort
C++Project to do a top

1. Ingests a DAG definition in the specified format.
2. Creates an internal graph representation using adjacency lists.
3. Performs a topological sort using a stack.

## Features:
  - **Flexible Parsing**: The program can parse DAG definitions where nodes may or may not have descriptive names.
  - **Graph Representation**: Uses an adjacency list to represent the graph, mapping each node label to its list of adjacent node labels.
  - **Topological Sort**: Implements a Depth-First Search (DFS) based topological sort using a stack to determine a valid ordering of tasks respecting their dependencies.
  - **Error Handling**: Includes basic error checking during parsing and cycle detection during topological sort.

## Graph Representation
  - **Adjacency List (```adj```)**:
    - Maps each source node label to a list of destination node labels, representing directed edges.
  - **Node Labels and Names (```label_to_name```)**:
    - Stores descriptive names for nodes that have them.
    - Nodes without names are handled gracefully by labeling them as [Unknown Task] in the output.

## Topological Sort Implementation
  - **DFS-Based Approach**:
    - The algorithm uses DFS to traverse the graph.
    - **Cycle Detection**: Utilizes a recursion_stack to detect cycles. If a node is revisited while still in the recursion stack, a cycle is present, and the sort is aborted.
  - **Stack** (```sorted_stack```):
    - Stores nodes in the order of their completion times during DFS.
    -  Popping from this stack yields the topological order.
    -  
## Input:
```
A[Project Planning] --> B[Design Architecture]
A --> C[Obtain Permits]
A --> D[Site Survey]
B --> E[Site Preparation]
C --> E
D --> E
E --> F[Foundation]
F --> G[Framing]
G --> H[Roofing]
G --> I[Electrical Installation]
G --> J[Plumbing Installation]
I --> K[HVAC Installation]
J --> K
K --> L[Interior Finishing]
K --> M[Exterior Finishing]
L --> O[Final Inspection]
M --> O
N[Landscaping] --> O

```
## Output
```
Topological Sort Order:
A [Project Planning]
N [Landscaping]
B [Design Architecture]
C [Obtain Permits]
D [Site Survey]
E [Site Preparation]
F [Foundation]
G [Framing]
J [Plumbing Installation]
I [Electrical Installation]
K [HVAC Installation]
M [Exterior Finishing]
L [Interior Finishing]
H [Roofing]
O [Final Inspection]

```

## Processing Steps:
1. Parsing Lines:
  - Each line is parsed using the revised regex.
  - Node labels and names are extracted where available.
  - Edges are added to the adjacency list.
2. Graph Representation:
```
A -> B, C, D
B -> E
C -> E
D -> E
E -> F
F -> G
G -> H, I, J
I -> K
J -> K
K -> L, M
L -> O
M -> O
N -> O
```
3. Topological Sort Execution:
  - Start with A:
    - Visit A, then B, then E, then F, then G, then H.
    - Backtrack to G, visit I, then K, then L, then O.
    - Backtrack to K, visit M, then O.
  - Process Remaining Nodes:
    - Visit N, then O (already visited).
4. Final Topological Order:
```
A [Project Planning]
N [Landscaping]
B [Design Architecture]
C [Obtain Permits]
D [Site Survey]
E [Site Preparation]
F [Foundation]
G [Framing]
J [Plumbing Installation]
I [Electrical Installation]
K [HVAC Installation]
M [Exterior Finishing]
L [Interior Finishing]
H [Roofing]
O [Final Inspection]
```

## Output:
```
Topological Sort Order:
A [Project Planning]
N [Landscaping]
B [Design Architecture]
C [Obtain Permits]
D [Site Survey]
E [Site Preparation]
F [Foundation]
G [Framing]
J [Plumbing Installation]
I [Electrical Installation]
K [HVAC Installation]
M [Exterior Finishing]
L [Interior Finishing]
H [Roofing]
O [Final Inspection]
```
