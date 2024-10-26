#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <regex>
#include <algorithm>

// Function to perform topological sort using DFS and a stack
bool topological_sort_util(const std::string& node,
                           const std::unordered_map<std::string, std::vector<std::string>>& adj,
                           std::unordered_set<std::string>& visited,
                           std::unordered_set<std::string>& recursion_stack,
                           std::stack<std::string>& sorted_stack) {
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
                std::cerr << "Cycle detected involving node: " << neighbor << std::endl;
                return false;
            }
        }
    }

    // Push the current node to the stack and remove from recursion stack
    sorted_stack.push(node);
    recursion_stack.erase(node);
    return true;
}

bool topological_sort(const std::unordered_map<std::string, std::vector<std::string>>& adj,
                     const std::unordered_set<std::string>& all_nodes,
                     std::stack<std::string>& sorted_stack) {
    std::unordered_set<std::string> visited;
    std::unordered_set<std::string> recursion_stack;

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
    std::unordered_map<std::string, std::string> label_to_name; // Maps label to node name
    std::unordered_map<std::string, std::vector<std::string>> adj; // Adjacency list
    std::unordered_set<std::string> all_nodes; // Set of all node labels

    // Regex to parse lines like:
    // A[Project Planning] --> B[Design Architecture]
    // A --> C[Obtain Permits]
    // J --> K
    // K --> L[Interior Finishing]
    std::regex edge_regex(R"((\w+)(?:\[(.*?)\])?\s*-->\s*(\w+)(?:\[(.*?)\])?)");
    std::smatch matches;
    std::string line;

    std::cout << "Enter DAG definitions (e.g., A[Task A] --> B[Task B], A --> C[Task C], J --> K, K --> L[Task L]). Enter EOF (Ctrl+D/Ctrl+Z) to finish:\n";

    // Read lines until EOF
    while (std::getline(std::cin, line)) {
        // Trim the line (optional)
        // Remove leading and trailing whitespace
        size_t start = line.find_first_not_of(" \t\r\n");
        size_t end = line.find_last_not_of(" \t\r\n");
        if (start == std::string::npos || end == std::string::npos) {
            continue; // Skip empty or whitespace-only lines
        }
        line = line.substr(start, end - start + 1);

        // Apply regex to the line
        if (std::regex_match(line, matches, edge_regex)) {
            if (matches.size() >= 4) { // matches[0] is the whole string
                std::string src_label = matches[1];
                std::string src_name = matches[2];
                std::string dst_label = matches[3];
                std::string dst_name = matches[4];

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
                std::cerr << "Invalid format in line: " << line << std::endl;
            }
        } else {
            std::cerr << "Line did not match expected format and will be ignored: " << line << std::endl;
        }
    }

    // Perform topological sort
    std::stack<std::string> sorted_stack;
    bool success = topological_sort(adj, all_nodes, sorted_stack);

    if (!success) {
        std::cerr << "Topological sort failed due to a cycle in the graph." << std::endl;
        return 1;
    }

    // Collect sorted nodes
    std::vector<std::string> sorted_order;
    while (!sorted_stack.empty()) {
        sorted_order.push_back(sorted_stack.top());
        sorted_stack.pop();
    }

    // Display the sorted order with names
    std::cout << "\nTopological Sort Order:\n";
    for (const auto& label : sorted_order) {
        // Check if the label has a corresponding name
        if (label_to_name.find(label) != label_to_name.end()) {
            std::cout << label << " [" << label_to_name[label] << "]\n";
        } else {
            std::cout << label << " [Unknown Task]\n";
        }
    }

    return 0;
}
