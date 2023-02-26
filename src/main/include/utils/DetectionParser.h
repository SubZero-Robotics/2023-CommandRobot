#ifndef DETECTION_PARSER_H
#define DETECTION_PARSER_H
#include <vector>
#include <utility>
#include <array>

namespace DetectionParser {

    enum class ObjectClasses {
        Cone = 0, 
        Cube = 1,
        Merge = 2
    };

    struct BoundingBox {
        // An ordered pair representing X and Y of the top left coordinate
        std::pair<float, float> topLeft;
        // An ordered pair representing X and Y of the bottom right coordinate
        std::pair<float, float> bottomRight;
        float width;
        float height;

        // [topLeftY, topLeftX, bottomRightY, bottomRightX]
        static BoundingBox parse(std::array<float, 4> boxes) {
            return {
                .topLeft = std::make_pair(boxes.at(1), boxes.at(0)),
                .bottomRight = std::make_pair(boxes.at(3), boxes.at(2)),
                .width = boxes.at(3) - boxes.at(1),
                .height = boxes.at(2) - boxes.at(0)
            };
        }
    };

    struct DetectedObject
    {
        ObjectClasses classId;
        float confidence;
        BoundingBox bbox;

        static std::vector<DetectedObject> parse(std::vector<float> flattenedOutputList) {

            auto detectedObjects = std::vector<DetectedObject>(10);

            int entryNum = flattenedOutputList.at(0);

            for (int i = 0; i < entryNum; i++) {
                int startNum = i * 6 + 1;
                auto bound = std::array<float, 4> ({
                    flattenedOutputList.at(startNum + 2),
                    flattenedOutputList.at(startNum + 3),
                    flattenedOutputList.at(startNum + 4),
                    flattenedOutputList.at(startNum + 5)
                });
                DetectedObject box = {
                    .classId = (ObjectClasses) flattenedOutputList.at(startNum),
                    .confidence = flattenedOutputList.at(startNum + 1),
                    .bbox = BoundingBox::parse(bound)
                };
                detectedObjects.push_back(box);
            }
        }
    };

}


#endif