#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "MonochromDither.hpp"


using namespace std;
using namespace cv;
using namespace dither;


int main(int argc, const char** argv)
{
    CommandLineParser parser(argc, argv,
                             "{h help      |        | }"
                             "{i image     | <none> | image to dither}"
                             "{t threshold | 100    | greyscale threshold}"
                             "{n noise     | 120    | noise threshold}");

    parser.about("simple tool to demonstrate image dithering");

    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    const auto img = parser.get<string>("i");
    const auto threshold = (uint8_t)parser.get<int>("t");
    const auto noiseThreshold = (uint8_t)parser.get<int>("n");

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    const auto rawImg = imread(img);

    if (rawImg.data == NULL)
    {
        cerr << "image not found\n";
        parser.printMessage();
        return -1;
    }

    MonochromDither monochromDither;

    imshow("Simple Error Diffusion", monochromDither.simpleErrorDiffusion(rawImg));
    imshow("Floyd-Steinberg", monochromDither.floydSteinberg(rawImg));
    imshow("Fixed Threshold", monochromDither.fixedTreshold(rawImg, threshold));
    imshow("Fixed Treshold With Noise", monochromDither.noiseTreshold(rawImg, noiseThreshold, threshold));
    imshow("Ordered", monochromDither.ordered(rawImg));
    imshow("Random", monochromDither.random(rawImg));
    imshow("Dispersed Pattern", monochromDither.patterned(rawImg, PATTERN_TYPE::dispersed));
    imshow("Clustered Pattern", monochromDither.patterned(rawImg, PATTERN_TYPE::clustered));
    imshow("Raw Image", rawImg);

    cout << "Press any key to quit.\n";

    waitKey(0);

    return 0;
}
