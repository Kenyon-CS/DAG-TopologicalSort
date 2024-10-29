#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <regex>
#include <algorithm>

using namespace std;

// Function to perform topological sort using DFS and a stack
bool topological_sort_util(const string& node,
                           const unordered_map<string, vector<string>>& adj,
                           unordered_set<string>& visited,
                           unordered_set<string>& recursion_stack,
                           stack<string>& sorted_stack) {
    // Mark the current node as visited and part of recursion stack
    visited.insert(node);
    recursion_stack.insert(node);

    // Iterate through all the neighbors
    auto it = adj.find(node);
    if (it != adj.end()) {
        for (const auto& neighbor : it->second) {
            // If neighbor hasn't been visited, recurse on it
            if (visited.find(neighbor) == visited.end()) {
                if (!topological_sort_util(neighbor, adj, visited, recursion_stack, sorted_stack))
                    return false;
            }
            // If neighbor is in recursion stack, a cycle is detected
            else if (recursion_stack.find(neighbor) != recursion_stack.end()) {
                cerr << "Cycle detected involving node: " << neighbor << endl;
                return false;
            }
        }
    }

    // Push the current node to the stack and remove from recursion stack
    sorted_stack.push(node);
    recursion_stack.erase(node);
    return true;
}

bool topological_sort(const unordered_map<string, vector<string>>& adj,
                     const unordered_set<string>& all_nodes,
                     stack<string>& sorted_stack) {
    unordered_set<string> visited;
    unordered_set<string> recursion_stack;

    // Visit all nodes
    for (const auto& node : all_nodes) {
        if (visited.find(node) == visited.end()) {
            if (!topological_sort_util(node, adj, visited, recursion_stack, sorted_stack))
                return false; // Cycle detected
        }
    }
    return true;
}

int main() {
    unordered_map<string, string> label_to_name; // Maps label to node name
    unordered_map<string, vector<string>> adj; // Adjacency list
    unordered_set<string> all_nodes; // Set of all node labels

    // Regex to parse lines like:
    // A[Project Planning] --> B[Design Architecture]
    // A --> C[Obtain Permits]
    // J --> K
    // K[Interior Finishing] --> L
    regex edge_regex(R"((\w+)(?:\[(.*?)\])?\s*-->\s*(\w+)(?:\[(.*?)\])?)");
    smatch matches;
    string line;

    cout << "Enter DAG definitions (e.g., A[Task A] --> B[Task B], A --> C[Task C], J --> K, K --> L[Task L]). Enter EOF (Ctrl+D/Ctrl+Z) to finish:\n";

    // Read lines until EOF
    while (getline(cin, line)) {
        // Trim the line (optional)
        // Remove leading and trailing whitespace
        size_t start = line.find_first_not_of(" \t\r\n");
        size_t end = line.find_last_not_of(" \t\r\n");
        if (start == string::npos || end == string::npos) {
            continue; // Skip empty or whitespace-only lines
        }
        line = line.substr(start, end - start + 1);

        // Apply regex to the line
        if (regex_match(line, matches, edge_regex)) {
            if (matches.size() >= 4) { // matches[0] is the whole string
                string src_label = matches[1];
                string src_name = matches[2];
                string dst_label = matches[3];
                string dst_name = matches[4];

                // Map labels to names if names are provided
                if (!src_name.empty()) {
                    label_to_name[src_label] = src_name;
                }
                if (!dst_name.empty()) {
                    label_to_name[dst_label] = dst_name;
                }

                // Add edge to adjacency list
                adj[src_label].push_back(dst_label);

                // Add nodes to the set
                all_nodes.insert(src_label);
                all_nodes.insert(dst_label);
            } else {
                cerr << "Invalid format in line: " << line << endl;
            }
        } else {
            cerr << "Line did not match expected format and will be ignored: " << line << endl;
        }
    }

    // Perform topological sort
    stack<string> sorted_stack;
    bool success = topological_sort(adj, all_nodes, sorted_stack);

    if (!success) {
        cerr << "Topological sort failed due to a cycle in the graph." << endl;
        return 1;
    }

    // Collect sorted nodes
    vector<string> sorted_order;
    while (!sorted_stack.empty()) {
        sorted_order.push_back(sorted_stack.top());
        sorted_stack.pop();
    }

    // Display the sorted order with names
    cout << "\nTopological Sort Order:\n";
    for (const auto& label : sorted_order) {
        // Check if the label has a corresponding name
        if (label_to_name.find(label) != label_to_name.end()) {
            cout << label << " [" << label_to_name[label] << "]\n";
        } else {
            cout << label << " [Unknown Task]\n";
        }
    }

    return 0;
}
