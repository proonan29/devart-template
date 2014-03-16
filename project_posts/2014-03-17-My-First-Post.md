# Introduction to a random world - Map Maker

## How it started in 20 years ago

As a programmer, it requires to have own imagination to make something nice out of code and APIs. Random number generation itself is quite interesting but it only ends as what it is in most cases. I have had great interest on visualising numbers and making them into something meaningful.

My attempt was not working always. I kept working on bringing some of my idea into reality however. About 20 years ago, I came up with an algorithm that could generate map like image continously. The result was very interesting.

## Let's make a map

My idea was quite simple. Let's start with an image buffer filled with pure random numbers. It won't look good because they only look like noisy TV screen without proper signal. To make this random filled image into map like image, I needed to give each pixels depth. The color notation had index value instead of true color. So the color index also represented 8 bit altitude(or height) of a certain pixel. The idea was similar to Voxel 3D notation. From 0 to 127, the numbers were representing sea level. From 128 to 255, the numbers were terrains(land and mountains).

## Reverse quantization and adding modulation

Now I can call the algorithm "Reverse quantiziation with modulation" but I didn't know what I was doing. I was going to magnify pixels in center area and double the size horizontally and vertically. Then the new image would be enlarged to x4 but dull. Then I added some modulation on entire pixels. The modulation have only small difference from all the adjacent pixels. So after each iteration, newer image would have enlarged, median, randomly modulated pixels. Since each pixel had index value of geographical height, the newer image would more look like real map.

## Requirement for experiment

Bringing the idea into real world is very exciting. But something must be done to do that. A fast computer with advanced graphic card would do the job. It was not easy as today. VGA board only showed 256 colors on 320x200 resolution. That was the best graphics colors provided by industry standard by the time.

To be continued.

![Example Image](../project_images/cover.jpg?raw=true "Example Image")
