#include "Dither.hpp"


namespace dither
{
    Dither::Dither()
    {
        createClusteredPatterns();
        createDispersedPatterns();
    }


    Dither::~Dither() {}


    void Dither::createClusteredPatterns()
    {
        this->clusteredPatterns.clear();
        this->clusteredPatterns.reserve(10);
        this->clusteredPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<    0,   0,   0,
                0,   0,   0,
                0,   0,   0));
        this->clusteredPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<    0,   0,   0,
                0, 255,   0,
                0,   0,   0));
        this->clusteredPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<    0,   0,   0,
                0, 255, 255,
                0,   0,   0));
        this->clusteredPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<    0, 255,   0,
                0, 255, 255,
                0,   0,   0));
        this->clusteredPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<    0, 255, 255,
                0, 255, 255,
                0,   0,   0));
        this->clusteredPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<    0, 255, 255,
                0, 255, 255,
                0, 255,   0));
        this->clusteredPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<    0, 255, 255,
                255, 255, 255,
                0, 255,   0));
        this->clusteredPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<    0, 255, 255,
                255, 255, 255,
                255, 255,   0));
        this->clusteredPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<  255, 255, 255,
                255, 255, 255,
                255, 255,   0));
        this->clusteredPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<  255, 255, 255,
                255, 255, 255,
                255, 255, 255));
    }


    void Dither::createDispersedPatterns()
    {
        this->dispersedPatterns.clear();
        this->dispersedPatterns.reserve(10);
        this->dispersedPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<    0,   0,   0,
                0,   0,   0,
                0,   0,   0));
        this->dispersedPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<  255,   0,   0,
                0,   0,   0,
                0,   0,   0));
        this->dispersedPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<  255,   0,   0,
                0,   0,   0,
                0, 255,   0));
        this->dispersedPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<  255,   0,   0,
                0,   0, 255,
                0, 255,   0));
        this->dispersedPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<  255,   0, 255,
                0,   0, 255,
                0, 255,   0));
        this->dispersedPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<  255,   0, 255,
                255,   0, 255,
                0, 255,   0));
        this->dispersedPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<  255,   0, 255,
                255,   0, 255,
                255, 255,   0));
        this->dispersedPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<  255, 255, 255,
                255,   0, 255,
                255, 255,   0));
        this->dispersedPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<  255, 255, 255,
                255, 255, 255,
                255, 255,   0));
        this->dispersedPatterns.push_back((cv::Mat_<uint8_t>(3,3)
                <<  255, 255, 255,
                255, 255, 255,
                255, 255, 255));
    }


    uint8_t Dither::saturated_add(uint8_t val1, int8_t val2)
    {
        int16_t val1_int = val1;
        int16_t val2_int = val2;
        int16_t tmp = val1_int + val2_int;
        if (tmp > 255)
        {
            return 255;
        }
        else if (tmp < 0)
        {
            return 0;
        }
        else
        {
            return tmp;
        }
    }
}
