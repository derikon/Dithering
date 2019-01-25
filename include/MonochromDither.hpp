#ifndef MONOCHROM_DITHER_H
#define MONOCHROM_DITHER_H

#include <vector>

#include "Dither.hpp"


namespace dither
{
    class MonochromDither : public Dither
    {
    public:
        /*  Fixed Threshol
            A good place to start is with the example of performing a simple (or fixed)
            thresholding operation on our grayscale image in order to display it on our
            black and white device.  This is accomplished by establishing a demarcation
            point, or threshold, at the 50% gray level.  Each dot of the source image is
            compared against this threshold value: if it is darker than the value, the
            device plots it black, and if it's lighter, the device plots it white.

            What happens to the image during this operation?  Well, some detail
            survives, but our perception of gray levels is completely gone.  This means
            that a lot of the image content is obliterated.  Take an area of the image
            which is made up of various gray shades in the range of 60-90%.  After fixed
            thresholding, all of those shades (being darker than the 50% gray threshold)
            will be mapped to solid black.  So much for variations of intensity.

            Another portion of the image might show an object with an increasing,
            diffused shadow across one of its surfaces, with gray shades in the range of
            20-70%.  This gradual variation in intensity will be lost in fixed
            thresholding, giving way to two separate areas (one white, one black) and a
            distinct, visible boundary between them.  The situation where a transition
            from one intensity or shade to another is very conspicuous is known as
            contouring.
        */
        cv::Mat fixedTreshold(const cv::Mat& srcImg, const uint8_t threshold = 128) override;
        cv::Mat noiseTreshold(const cv::Mat& srcImg, const uint8_t noiseThreshold = 64, const uint8_t threshold = 128) override;


        /*  Random dither

            Random dithering could be termed the "bubblesort" of digital halftoning
            algorithms.  It was the first attempt (documented as far back as 1951) to
            correct the contouring produced by fixed thresholding, and it has
            traditionally been referenced for comparison in most studies of digital
            halftoning.  In fact, the name "ordered dither" (which will be discussed
            later) was chosen to contrast random dither.

            While it is not really acceptable as a production method, it is very simple
            to describe and implement.  For each dot in our grayscale image, we generate
            a random number in the range 0 - 255: if the random number is greater than
            the image value at that dot, the display device plots the dot white;
            otherwise, it plots it black.  That's it.

            This generates a picture with a lot of "white noise", which looks like TV
            picture "snow".  Although inaccurate and grainy, the image is free from
            artifacts.  Interestingly enough, this digital halftoning method is useful
            in reproducing very low-frequency images, where the absence of artifacts is
            more important than noise.  For example, a whole screen containing a
            gradient of all levels from black to white would actually look best with a
            random dither.  With this image, other digital halftoning algorithms would
            produce significant artifacts like diagonal patterns (in ordered dithering)
            and clustering (in error diffusion halftones).

            I should mention, of course, that unless your computer has a hardware-based
            random number generator (and most don't), there may be some artifacts from
            the random number generation algorithm itself.  For efficiency, you can take
            the random number generator "out of the loop" by generating a list of random
            numbers beforehand for use in the dither.  Make sure that the list is larger
            than the number of dots in the image or you may get artifacts from the reuse
            of numbers.  The worst case would be if the size of your list of random
            numbers is a multiple or near-multiple of the horizontal size of the image;
            in this case, unwanted vertical or diagonal lines will appear.

            As unattractive as it is, random dithering can actually be related to a
            pleasing, centuries-old art know as mezzotinting (the name itself is an
            Italianized derivative of the English "halftone").  In a mezzotint, the
            skilled craftsman worked a soft metal (usually copper) printing plate, and
            roughened or ground the dark regions of the image by hand and in a seemingly
            random fashion.  Analyzing it in scientific terms (which would surely insult
            any mezzotinting artisan who might read this!) the pattern created is not
            very regular or periodic at all, but the absence of low frequency noise
            leads to a very attractive image without much graininess.  A similar process
            is still in use today, in the form of modern gravure printing.
        */
        cv::Mat random(const cv::Mat& srcImg) override;


        /*  Patterning

            This was the first digital technique to pay homage to the classical
            halftone.  It takes advantage of the fact that the spatial resolution of
            display devices had improved to the point where one could trade some of it
            for better intensity resolution.  Like random dither, it is also a simple
            concept, but is much more effective.

            For each possible value in the image, we create and display a pattern of
            pixels (which can be either video pixels or printer "spots") that
            approximates that value.  Remembering the concept of spatial integration, if
            we choose the appropriate patterns we can simulate the appearance of various
            intensity levels -- even though our display can only generate a limited set
            of intensities.

            For example, consider a 3 x 3 pattern. It can have one of 512 different
            arrangements of pixels:  however, in terms of intensity, not all of them are
            unique.  Since the number of black pixels in the pattern determines the
            darkness of the pattern, we really have only 10 discrete intensity patterns
            (including the all-white pattern), each one having one more black pixel than
            the previous one.

            But which 10 patterns?  Well, we can eliminate, right off the bat, patterns
            like:

                 ---        X--        --X        X--
                 XXX   or   -X-   or   -X-   or   X--
                 ---        --X        X--        X--


            because if they were repeated over a large area (a common occurrence in many
            images [1]) they would create vertical, horizontal, or diagonal lines.
            Also, studies [1] have shown that the patterns should form a "growth
            sequence:"  once a pixel is intensified for a particular value, it should
            remain intensified for all subsequent values.  In this fashion, each pattern
            is a superset of the previous one; this similarity between adjacent
            intensity patterns minimizes any contouring artifacts.

            Here is a good pattern for a 3-by-3 matrix which subscribes to the rules set
            forth above:


                 ---   ---   ---   -X-   -XX   -XX   -XX   -XX   XXX   XXX
                 ---   -X-   -XX   -XX   -XX   -XX   XXX   XXX   XXX   XXX
                 ---   ---   ---   ---   ---   -X-   -X-   XX-   XX-   XXX


            This pattern matrix effectively simulates a screened halftone with dots of
            various sizes.  In large areas of constant value, the repetitive pattern
            formed will be mostly artifact-free.

            No doubt, the reader will realize that applying this patterning process to
            our image will triple its size in each direction.  Because of this,
            patterning can only be used where the display's spatial resolution is much
            greater than that of the image.

            Another limitation of patterning is that the effective spatial resolution is
            decreased, since a multiple-pixel "cell" is used to simulate the single,
            larger halftone dot.  The more intensity resolution we want, the larger the
            halftone cell used and, by extension, the lower the spatial resolution.

            In the above example, using 3 x 3 patterning, we are able to simulate 10
            intensity levels (not a very good rendering) but we must reduce the spatial
            resolution to 1/3 of the original figure.  To get 64 intensity levels (a
            very acceptable rendering), we would have to go to an 8 x 8 pattern and an
            eight-fold decrease in spatial resolution.  And to get the full 256 levels
            of intensity in our source image, we would need a 16 x 16 pattern and would
            incur a 16-fold reduction in spatial resolution.  Because of this size
            distortion of the image, and with the development of more effective digital
            halftoning methods, patterning is only infrequently used today.

            To extend this method to color images, we would use patterns of colored
            pixels to represent shades not directly printable by the hardware.  For
            example, if your hardware is capable of printing only red, green, blue, and
            black (the minimal case for color dithering), other colors can be
            represented with 2 x 2 patterns of these four:


                Yellow = R G    Cyan = G B      Magenta = R B       Gray = R G
                         G R           B G                B R              B K


            (B here represents blue, K is black).  In this particular example, there are
            a total of 31 such distinct patterns which can be used; their enumeration is
            left "as an exercise for the reader" (don't you hate books that do that?).

            Clustered vs. dispersed patterns

            The pattern diagrammed above is called a "clustered" pattern, so called
            because as new pixels are intensified in each pattern, they are placed
            adjacent to the already-intensified pixels.  Clustered-dot patterns were
            used on many of the early display devices which could not render individual
            pixels very distinctly, e.g. printing presses or other printers which smear
            the printed spots slightly (a condition known as dot gain), or video
            monitors which introduce some blurriness to the pixels.  Clustered-dot
            groupings tend to hide the effect of dot gain, but also produce a somewhat
            grainy image.

            As video and hardcopy display technology improved, newer devices (such as
            electrophotographic laser printers and high-res video displays) were better
            able to accurately place and size their pixels.  Further research showed
            that, especially with larger patterns, the dispersed (non-clustered) layout
            was more pleasing.  Here is one such pattern:


                 ---   X--   X--   X--   X-X   X-X   X-X   XXX   XXX   XXX
                 ---   ---   ---   --X   --X   X-X   X-X   X-X   XXX   XXX
                 ---   ---   -X-   -X-   -X-   -X-   XX-   XX-   XX-   XXX



            Since clustering is not used, dispersed-dot patterns produce less grainy
            images.
        */
        cv::Mat patterned(const cv::Mat& srcImg, const PATTERN_TYPE type) override;


        /*  Ordered dither

            While patterning was an important step toward the digital reproduction of
            the classic halftone, its main shortcoming was the spatial enlargement (and
            corresponding reduction in resolution) of the image.  Ordered dither
            represents a major improvement in digital halftoning where this spatial
            distortion was eliminated and the image could then be rendered in its
            original size.

            Obviously, in order to accomplish this, each dot in the source image must be
            mapped to a pixel on the display device on a one-to-one basis.  Accordingly,
            the patterning concept was redefined so that instead of plotting the whole
            pattern for each image dot, THE IMAGE DOT IS MAPPED ONLY TO ONE PIXEL IN THE
            PATTERN.  Returning to our example of a 3 x 3 pattern, this means that we
            would be mapping NINE image dots into this pattern.

            The simplest way to do this in programming is to map the X and Y coordinates
            of each image dot into the pixel (X mod 3, Y mod 3) in the pattern.

            Returning to our two patterns (clustered and dispersed) as defined earlier,
            we can derive an effective mathematical algorithm that can be used to plot
            the correct pixel patterns.  Because each of the patterns above is a
            superset of the previous, we can express the patterns in a compact array
            form as the order of pixels added:


                    8  3  4                       1  7  4
                    6  1  2          and          5  8  3
                    7  5  9                       6  2  9


            Then we can simply use the value in the array as a threshold.  If the value
            of the original image dot (scaled into the 0-9 range) is less than the
            number in the corresponding cell of the matrix, we plot that pixel black;
            otherwise, we plot it white.  Note that in large areas of constant value, we
            will get repetitions of the pattern just as we did with patterning.

            As before, clustered patterns should be used for those display devices which
            blur the pixels.  In fact, the clustered-dot ordered dither is the process
            used by most newspapers, and in the computer imaging world the term
            "halftoning" has come to refer to this method if not otherwise qualified.


            As noted earlier, the dispersed-dot method (where the display hardware
            allows) is preferred in order to decrease the graininess of the displayed
            images.  Bayer [2] has shown that for matrices of orders which are powers of
            two there is an optimal pattern of dispersed dots which results in the
            pattern noise being as high-frequency as possible.  The pattern for a 2x2
            and 4x4 matrices are as follows:


            1  3        1  9  3 11        These patterns (and their rotations
            4  2       13  5 15  7        and reflections) are optimal for a
                        4 12  2 10        dispersed-dot ordered dither.
                       16  8 14  6


            Ulichney [3] shows a recursive technique can be used to generate the larger
            patterns.  (To fully reproduce our 256-level image, we would need to use an
            8x8 pattern.)

            The Bayer ordered dither is in very common use and is easily identified by
            the cross-hatch pattern artifacts it produces in the resulting display.
            This artifacting is the major drawback of an otherwise powerful and very
            fast technique.
        */
        cv::Mat ordered(const cv::Mat& srcImg, const MAP_TYPE type = MAP_TYPE::bayer_4x4) override;


        cv::Mat simpleErrorDiffusion(const cv::Mat& srcImg) override;


        /*  The Floyd-Steinberg filter

            This is where it all began, with Floyd and Steinberg's [4] pioneering
            research in 1975.  The filter can be diagrammed thus:


                      *   7
                  3   5   1     (1/16)


            In this (and all subsequent) filter diagrams, the "*" represents the pixel
            currently being scanning, and the neighboring numbers (called weights)
            represent the portion of the error distributed to the pixel in that
            position.  The expression in parentheses is the divisor used to break up the
            error weights.  In the Floyd-Steinberg filter, each pixel "communicates"
            with 4 "neighbors."  The pixel immediately to the right gets 7/16 of the
            error value, the pixel directly below gets 5/16 of the error, and the
            diagonally adjacent pixels get 3/16 and 1/16.

            The weighting shown is for the traditional left-to-right scanning of the
            image.  If the line were scanned right-to-left (more about this later), this
            pattern would be reversed.  In either case, the weights calculated for the
            subsequent line must be held by the program, usually in an array of some
            sort, until that line is visited later.

            Floyd and Steinberg carefully chose this filter so that it would produce a
            checkerboard pattern in areas with intensity of 1/2 (or 128, in our sample
            image).  It is also fairly easy to execute in programming code, since the
            division by 16 is accomplished by simple, fast bit-shifting instructions
            (this is the case whenever the divisor is a power of 2).


            =====================================
            The "false" Floyd-Steinberg filter

            Occasionally, you will see the following filter erroneously called the
            Floyd-Steinberg filter:


                      *   3
                      3   2     (1/8)


            The output from this filter is nowhere near as good as that from the real
            Floyd-Steinberg filter.  There aren't enough weights to the dispersion,
            which means that the error value isn't distributed finely enough.  With the
            entire image scanned left-to-right, the artifacting produced would be
            totally unacceptable.

            Much better results would be obtained by using an alternating, or
            serpentine, raster scan: processing the first line left-to-right, the next
            line right-to-left, and so on (reversing the filter pattern appropriately).
            Serpentine scanning -- which can be used with any of the error-diffusion
            filters detailed here -- introduces an additional perturbation which
            contributes more randomness to the resultant halftone.  Even with serpentine
            scanning, however, this filter would need additional perturbations (see
            below) to give acceptable results.
        */
        cv::Mat floydSteinberg(const cv::Mat &srcImg) override;
    };
}


#endif
