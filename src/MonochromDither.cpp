#include "MonochromDither.hpp"

#include "opencv2/imgproc/imgproc.hpp"

#include <random>


namespace dither
{
    cv::Mat MonochromDither::fixedTreshold(const cv::Mat& srcImg, const uint8_t threshold)
    {
        const auto imgWidth = srcImg.cols;
        const auto imgHeight = srcImg.rows;
        cv::Mat dithImg;
        cv::cvtColor(srcImg, dithImg, cv::COLOR_BGR2GRAY);
        for (int y = 0; y < imgHeight; ++y)
        {
            for (int x = 0; x < imgWidth; ++x)
            {
                auto pxlVal = dithImg.at<uint8_t>(y,x);
                dithImg.at<uint8_t>(y,x) = (pxlVal < threshold) ? 0 : 255;
            }
        }
        return dithImg;
    }


    cv::Mat MonochromDither::noiseTreshold(const cv::Mat& srcImg, const uint8_t noiseThreshold, const uint8_t threshold)
    {
        const auto imgWidth = srcImg.cols;
        const auto imgHeight = srcImg.rows;
        cv::Mat dithImg;
        cv::cvtColor(srcImg, dithImg, cv::COLOR_BGR2GRAY);
        const auto offset = (int)(noiseThreshold/2);
        const auto noise = cv::Mat(srcImg.size(), CV_8SC1);
        cv::randu(noise, cv::Scalar::all(-offset), cv::Scalar::all(offset+1));
        noise += dithImg;
        for (int y = 0; y < imgHeight; ++y)
        {
            for (int x = 0; x < imgWidth; ++x)
            {
                auto pxlVal = noise.at<int8_t>(y,x);
                dithImg.at<uint8_t>(y,x) = (pxlVal < threshold) ? 0 : 255;
            }
        }
        return dithImg;
    }


    cv::Mat MonochromDither::random(const cv::Mat& srcImg)
    {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<uint8_t> random(0, 255);
        const auto imgWidth = srcImg.cols;
        const auto imgHeight = srcImg.rows;
        cv::Mat dithImg;
        cv::cvtColor(srcImg, dithImg, cv::COLOR_BGR2GRAY);
        for (int y = 0; y < imgHeight; ++y)
        {
            for (int x = 0; x < imgWidth; ++x)
            {
                auto pxlVal = dithImg.at<uint8_t>(y,x);
                dithImg.at<uint8_t>(y,x) = (pxlVal < random(generator)) ? 0 : 255;
            }
        }
        return dithImg;
    }


    cv::Mat MonochromDither::patterned(const cv::Mat& srcImg, const dither::PATTERN_TYPE type)
    {
        auto patterns = &this->dispersedPatterns;
        switch (type)
        {
            case dither::PATTERN_TYPE::clustered:
                // ---   ---   ---   -X-   -XX   -XX   -XX   -XX   XXX   XXX
                // ---   -X-   -XX   -XX   -XX   -XX   XXX   XXX   XXX   XXX
                // ---   ---   ---   ---   ---   -X-   -X-   XX-   XX-   XXX
                patterns = &this->clusteredPatterns;
                break;
            case dither::PATTERN_TYPE::dispersed:
                // ---   X--   X--   X--   X-X   X-X   X-X   XXX   XXX   XXX
                // ---   ---   ---   --X   --X   X-X   X-X   X-X   XXX   XXX
                // ---   ---   -X-   -X-   -X-   -X-   XX-   XX-   XX-   XXX
                patterns = &this->dispersedPatterns;
                break;
        }
        const auto imgWidth = srcImg.cols - 1;
        const auto imgHeight = srcImg.rows - 1;
        cv::Mat dithImg;
        cv::cvtColor(srcImg, dithImg, cv::COLOR_BGR2GRAY);
        for (int y = 1; y < imgHeight; y+=2)
        {
            for (int x = 1; x < imgWidth; x+=2)
            {
                cv::Mat aux = dithImg.colRange(x-1,x+2).rowRange(y-1,y+2);
                auto mean = (uint8_t)cv::mean(aux)[0];
                if      (mean <= 25)  { patterns->at(0).copyTo(aux); }
                else if (mean <= 50)  { patterns->at(1).copyTo(aux); }
                else if (mean <= 75)  { patterns->at(2).copyTo(aux); }
                else if (mean <= 100) { patterns->at(3).copyTo(aux); }
                else if (mean <= 125) { patterns->at(4).copyTo(aux); }
                else if (mean <= 150) { patterns->at(5).copyTo(aux); }
                else if (mean <= 175) { patterns->at(6).copyTo(aux); }
                else if (mean <= 200) { patterns->at(7).copyTo(aux); }
                else if (mean <= 225) { patterns->at(8).copyTo(aux); }
                else                  { patterns->at(9).copyTo(aux); }
            }
        }
        return dithImg;
    }


    cv::Mat MonochromDither::ordered(const cv::Mat& srcImg, const dither::MAP_TYPE type)
    {
        const auto imgWidth = srcImg.cols;
        const auto imgHeight = srcImg.rows;
        cv::Mat dithImg;
        cv::cvtColor(srcImg, dithImg, cv::COLOR_BGR2GRAY);

        cv::Mat thMap;
        uint8_t thMapW;

        switch (type)
        {
            case dither::MAP_TYPE::bayer_2x2:
                thMapW = 2;
                thMap = (cv::Mat_<uint8_t>(thMapW, thMapW)
                    <<  1, 3,
                        4, 2);
            break;
            case dither::MAP_TYPE::bayer_4x4:
                thMapW = 4;
                thMap = (cv::Mat_<uint8_t>(thMapW, thMapW)
                    <<   1,  9,  3, 11,
                        13,  5, 15,  7,
                         4, 12,  2, 10,
                        16,  8, 14,  6);
            break;
            case dither::MAP_TYPE::bayer_8x8:
                thMapW = 8;
                thMap = (cv::Mat_<uint8_t>(thMapW, thMapW)
                    <<   0, 32,  8, 40,  2, 34, 10, 42,
                        48, 16, 56, 24, 50, 18, 58, 26,
                        12, 44,  4, 36, 14, 46,  6, 38,
                        60, 28, 52, 20, 62, 30, 54, 22,
                         3, 35, 11, 43,  1, 33,  9, 41,
                        51, 19, 59, 27, 49, 17, 57, 25,
                        15, 47,  7, 39, 13, 45,  5, 37,
                        63, 31, 55, 23, 61, 29, 53, 21);
            case dither::MAP_TYPE::clustered_3x3_1:
                thMapW = 3;
                thMap = (cv::Mat_<uint8_t>(thMapW, thMapW)
                    <<  8, 3, 4,
                        6, 1, 2,
                        7, 5, 9);
            break;
            case dither::MAP_TYPE::clustered_3x3_2:
                thMapW = 3;
                thMap = (cv::Mat_<uint8_t>(thMapW, thMapW)
                    <<  1, 7, 4,
                        5, 8, 3,
                        6, 2, 9);
            break;
        }
        const auto thMapW2 = thMapW * thMapW;
        for (int y = 0; y < imgHeight; ++y)
        {
            for (int x = 0; x < imgWidth; ++x)
            {
                auto thMapY = y % thMapW;
                auto thMapX = x % thMapW;
                auto pxlVal = (uint8_t)((dithImg.at<uint8_t>(y,x)/255.f) * thMapW2);
                dithImg.at<uint8_t>(y,x) = (pxlVal < thMap.at<uint8_t>(thMapY,thMapX)) ? 0 : 255;
            }
        }
        return dithImg;
    }


    cv::Mat MonochromDither::simpleErrorDiffusion(const cv::Mat& srcImg)
    {
        const auto imgWidth = srcImg.cols;
        const auto imgHeight = srcImg.rows;
        cv::Mat dithImg;
        cv::cvtColor(srcImg, dithImg, cv::COLOR_BGR2GRAY);
        for (int y = 0; y < imgHeight; ++y)
        {
            int8_t err = 0;
            for (int x = 0; x < imgWidth; ++x)
            {
                auto pxlVal = dithImg.at<uint8_t>(y,x);
                if (x != imgWidth-1)
                {
                    err = (pxlVal < 127) ? pxlVal : pxlVal-255;
                    dithImg.at<uint8_t>(y,x+1) = saturated_add(dithImg.at<uint8_t>(y,x+1), err);
                }
                dithImg.at<uint8_t>(y,x) = (pxlVal < 127) ? 0 : 255;
            }
        }
        return dithImg;
    }


    cv::Mat MonochromDither::floydSteinberg(const cv::Mat &srcImg)
    {
        const auto imgWidth = srcImg.cols;
        const auto imgHeight = srcImg.rows;
        cv::Mat dithImg;
        cv::cvtColor(srcImg, dithImg, cv::COLOR_BGR2GRAY);  
        for (int y = 0; y < imgHeight; ++y)
        {      
            for (int x = 0; x < imgWidth; ++x)
            {
                uint8_t oldPxlVal = dithImg.at<uint8_t>(y, x);
                uint8_t newPxlVal = (oldPxlVal < 127) ? 0 : 255;
                dithImg.at<uint8_t>(y, x) = newPxlVal;
                int8_t err = oldPxlVal - newPxlVal;
                if ((y != (imgHeight-1)) && (x != 0) && (x != (imgWidth-1)))
                {
                    dithImg.at<uint8_t>(y+0, x+1) = saturated_add(dithImg.at<uint8_t>(y+0, x+1), (err * 7) / 16);
                    dithImg.at<uint8_t>(y+1, x+1) = saturated_add(dithImg.at<uint8_t>(y+1, x+1), (err * 1) / 16);
                    dithImg.at<uint8_t>(y+1, x+0) = saturated_add(dithImg.at<uint8_t>(y+1, x+0), (err * 5) / 16);
                    dithImg.at<uint8_t>(y+1, x-1) = saturated_add(dithImg.at<uint8_t>(y+1, x-1), (err * 3) / 16);
                }
            } 
        }
        return dithImg;
    }
}
