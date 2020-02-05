//
// Created by Aleksey on 30.04.2015.
//

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <helper/color.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <rapidjson/document.h>
#include <iostream>
#include <fstream>
#include "color_reducer.hpp"

struct Edge {
    Edge(int _from, int _to, double _distance, double _weight) : from(_from), to(_to), distance(_distance),
                                                                 weight(_weight) { };
    int from;
    int to;
    double distance;
    double weight;
};

_ColorReducer::_ColorReducer(string mapping_file_name, string _distance_file_name_pattern) :
        distance_file_name_pattern(_distance_file_name_pattern) {
    mapping = cv::imread(mapping_file_name);
}

void _ColorReducer::reduce(string input_file_name, string output_file_name, int count) {
    cv::Mat input = cv::imread(input_file_name);
    cv::Mat output(input.rows, input.cols, input.type());
    for (int row = 0; row < input.rows; ++row) {
        for (int col = 0; col < input.cols; ++col) {
            int fromIndex = color::colorToIndex(input.at<cv::Vec3b>(row, col));
            int toIndex = color::colorToIndex(mapping.at<cv::Vec3b>(0, fromIndex));
            weights[toIndex]++;
            road[fromIndex] = toIndex;
//            output.at<cv::Vec3b>(row, col) = mapping.at<cv::Vec3b>(0, fromIndex);
        }
    }
    for (auto from: weights) {
        string content = readFile(boost::str(boost::format(distance_file_name_pattern) % from.first));
        rapidjson::Document document;
        document.Parse(content.c_str());
        for (auto to: weights) {
            if (from == to)
                continue;
            string key = boost::lexical_cast<std::string>(to.first);
            rapidjson::Value &json = document[key.c_str()];
            Edge *edge = new Edge(from.first, to.first, json.GetDouble(), (from.second * json.GetDouble()) ); // / (to.second / 1000));
            edges.push_back(edge);
        }
    }

    while (weights.size() > count) {
        auto min_iter = std::min_element(edges.begin(), edges.end(), [](Edge const *left, Edge const *right) {
            return left->weight < right->weight;
        });
        Edge *min = (*min_iter);
        int min_index = min->from;

        cout << min->from << "(" << weights[min->from] << ") -> " << min->to << "(" << weights[min->to] << "): " <<
        min->distance << "(" << min->weight << ")\n";

        weights[min->to] += weights[min_index];
        weights.erase(min_index);

        auto rm = std::remove_if(edges.begin(), edges.end(),
                                 [min_index](Edge const *n) { return (n->to == min_index) || (n->from == min_index); });
        edges.erase(rm, edges.end());

        for (std::vector<Edge *>::iterator iterator = edges.begin(); iterator != edges.end(); ++iterator) {
            Edge *&item = *iterator;
            if ((item->to == min->to) || (item->from == min->to)) {
                item->weight = (weights[item->from] * item->distance);// / (weights[item->to] / 1000);
            }
        }
        for (auto r: road) {
            if (r.second == min_index)
                road[r.first] = min->to;
        }
    }
    cout << weights.size();

    for (int row = 0; row < input.rows; ++row) {
        for (int col = 0; col < input.cols; ++col) {
            int fromIndex = color::colorToIndex(input.at<cv::Vec3b>(row, col));
            output.at<cv::Vec3b>(row, col) = mapping.at<cv::Vec3b>(0, road[fromIndex]);
        }
    }

    cv::imwrite(output_file_name, output);
}

string _ColorReducer::readFile(const std::string &file_name) const {
    ifstream t(file_name);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
}
