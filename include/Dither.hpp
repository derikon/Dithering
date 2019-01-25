#ifndef DITHER_HPP
#define DITHER_HPP

#include <vector>

#include "opencv2/core.hpp"
#include "Types.hpp"


namespace dither
{
    class Dither
    {
    public:
        Dither();
        ~Dither();

    public:
        virtual cv::Mat fixedTreshold(const cv::Mat& srcImg, const uint8_t threshold) = 0;
        virtual cv::Mat noiseTreshold(const cv::Mat& srcImg, const uint8_t noiseThreshold, const uint8_t threshold) = 0;
        virtual cv::Mat random(const cv::Mat& srcImg) = 0;
        virtual cv::Mat patterned(const cv::Mat& srcImg, const PATTERN_TYPE type) = 0;
        virtual cv::Mat ordered(const cv::Mat& srcImg, const MAP_TYPE type = MAP_TYPE::bayer_4x4) = 0;
        virtual cv::Mat simpleErrorDiffusion(const cv::Mat& srcImg) = 0;
        virtual cv::Mat floydSteinberg(const cv::Mat &srcImg) = 0;

    protected:
        std::vector<const cv::Mat> clusteredPatterns;
        std::vector<const cv::Mat> dispersedPatterns;
        void createClusteredPatterns();
        void createDispersedPatterns();

    protected:
        uint8_t saturated_add(uint8_t val1, int8_t val2);
    };
}


#endif //DITHER_HPP
