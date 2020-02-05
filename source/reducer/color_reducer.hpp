//
// Created by Aleksey on 30.04.2015.
//

#ifndef IMAGE_TO_SKETCH_COLOR_REDUCER_H
#define IMAGE_TO_SKETCH_COLOR_REDUCER_H

#include <opencv2/core.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <map>

using namespace std;

struct Edge;

class _ColorReducer;
typedef boost::shared_ptr<_ColorReducer> ColorReducer;

class _ColorReducer {
public:
    _ColorReducer(string mapping_file_name, string _distance_file_name_pattern);
    void reduce(string input_file_name, string output_file_name, int count);

private:
    string distance_file_name_pattern;
    cv::Mat mapping;

    std::map<int, int> weights;
    std::map<int, int> road;
    std::vector<Edge*> edges;

    string readFile(const std::string &file_name) const;
};

#endif //IMAGE_TO_SKETCH_COLOR_REDUCER_H
