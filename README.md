# Dag-TopologicalSort
C++Project to do a top

1. Ingests a DAG definition in the specified format.
2. Creates an internal graph representation using adjacency lists.
3. Performs a topological sort using a stack.

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
