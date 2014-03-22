# Build Note - 2

## Basic Idea
Randomized terrain-like pattern is hard to generate. To make things easier, I came up with an idea of using "indexed color" for representing each pixel's height.
A pixel will have a color value associated with its height represented. So it is required to have a color-height conversion map. Many years ago, this type of image was very popular. Because of simplicity, 256-color images were used to in games and web pages.

## Indexed Color and Height Relation Map
Each pixel has 8 bit integer. 512 x 512 byte buffer would be big enough to show a map and small enough to guarantee smooth processing speed.
![Example Image](../project_images/IndexedColor.jpg?raw=true "Index Color")

## Putting them all togather
To build a working sample, a PC, an Android device, java SE SDK, Python, Android SDK, Android NDK, Cocos2d-X engine, Eclipse and Visual Studio 2010 Express. Image editing S/W such as PhotoShop would be needed all the time. After long waited installing S/W and bug fixing, the very first working sample was finally came to alive.
![Example Image](../project_images/build.jpg?raw=true "cocos2d-x")