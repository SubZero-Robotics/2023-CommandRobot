#ifndef DETECTION_PARSER_H
#define DETECTION_PARSER_H
#include <array>
#include <utility>
#include <vector>

namespace DetectionParser {

enum class ObjectClasses { Cone = 0, Cube = 1, Merge = 2 };

struct BoundingBox {
    // An ordered pair representing X and Y of the top left coordinate
    std::pair<double, double> topLeft;
    // An ordered pair representing X and Y of the bottom right coordinate
    std::pair<double, double> bottomRight;
    double width;
    double height;

    // [topLeftY, topLeftX, bottomRightY, bottomRightX]
    static BoundingBox parse(std::array<double, 4> boxes) {
        return {
            std::make_pair(boxes.at(1), boxes.at(0)),  // top-left (X, Y)
            std::make_pair(boxes.at(3), boxes.at(2)),  // bottom-right (X, y)
            boxes.at(3) - boxes.at(1),                 // width
            boxes.at(2) - boxes.at(0)};                // height
    }
};

struct DetectedObject {
    ObjectClasses classId;
    double confidence;
    BoundingBox bbox;

    static std::vector<DetectedObject> parse(
        std::vector<double> flattenedOutputList) {
        std::vector<DetectedObject> detectedObjects;
        detectedObjects.reserve(10);

        int entryNum = flattenedOutputList.at(0);

        for (int i = 0; i < entryNum; i++) {
            int startNum = i * 6 + 1;
            auto bound =
                std::array<double, 4>({flattenedOutputList.at(startNum + 2),
                                      flattenedOutputList.at(startNum + 3),
                                      flattenedOutputList.at(startNum + 4),
                                      flattenedOutputList.at(startNum + 5)});
            DetectedObject box = {
                (ObjectClasses)flattenedOutputList.at(startNum),  // classId
                flattenedOutputList.at(startNum + 1),  // confidence score
                BoundingBox::parse(bound)};            // bbox
            detectedObjects.push_back(box);
        }
    }
};

}  // namespace DetectionParser

#endif