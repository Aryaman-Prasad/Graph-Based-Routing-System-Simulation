#include "../nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
/*
    Add other includes that you require, only write code wherever indicated
*/
#include "graphProcess.hpp"
#include "queryProcess.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <graph.json> <queries.json> <output.json>" << std::endl;
        return 1;
    }

    // Read graph from first file
    /*
        Add your graph reading and processing code here
        Initialize any classes and data structures needed for query processing
    */
    std::ifstream graph_file(argv[1]);
    if (!graph_file.is_open()) {
        std::cerr << "Failed to open " << argv[1] << std::endl;
        return 1;
    }

    // Reading json file
    json graph_json;
    graph_file >> graph_json;

    // Processing json file to construct graph
    Graph G(graph_json["meta"]["nodes"]);
    process_graph(graph_json, G);

    // // Preprocessing for approx_dist
    std::vector<std::vector<double>> ref_sssp;
    double e = 0.05;
    error_bound_dist(G, e, ref_sssp);


    // Read queries from second file
    std::ifstream queries_file(argv[2]);
    if (!queries_file.is_open()) {
        std::cerr << "Failed to open " << argv[2] << std::endl;
        return 1;
    }
    json queries_json;
    queries_file >> queries_json;

    // Reading events and meta separately from queries_json
    json meta = queries_json["meta"];
    queries_json = queries_json["events"];

    std::vector<json> results;
    
    for (const auto& query : queries_json) {
        auto start_time = std::chrono::high_resolution_clock::now();

        /*
            Add your query processing code here
            Each query should return a json object which should be printed to sample.json
        */

        // Answer each query replacing the function process_query using 
        // whatever function or class methods that you have implemented
        json result = process_query(query, G,ref_sssp);

        auto end_time = std::chrono::high_resolution_clock::now();
        result["processing_time"] = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        results.push_back(result);
    }

    std::ofstream output_file(argv[3]);
    if (!output_file.is_open()) {
        std::cerr << "Failed to open output.json for writing" << std::endl;
        return 1;
    }

    // json output = results;

    // Create output json in given format
    json output;
    output["meta"] = meta;
    output["results"] = results;

    output_file << output.dump(4) << std::endl;

    output_file.close();
    return 0;
}