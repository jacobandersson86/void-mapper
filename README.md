# Void Mapper
## Background
In most cases a frame buffer might be used when working with graphics and a full screen is clocked out in the next frame. But sometimes in the world of embedded, this situation might be a bit different. It could be a slow refresh rate or lack of memory. Perhaps there are hardware restriction not allowing for the full frame to be refreshed at once without noticing the scan lines. In these cases it is common to work with smaller areas of the screen and update just that particular part that has changed. For example one might be using sprites, and when animating a movement, the same sprite is drawn again but a new location.

Since the entire screen is not updated in each frame, there is a need to clear the parts of the screen that is out of date. It might be the whole screen but quite often it is just that part that contains the residue from the previous frame i.e. the border of the sprite that was moving.

When clearing the entire screen is not an option one might go for a solution where a frame update is divided into 1. drawing the assets that has changed. 2. clearing the areas that are outdated.

In some particular cases, the screen might have a controller that can clear areas fast, but only in the shape of rectangles.
This allows for an update where step 1. draws the all the sprites and step 2. clears all the areas that is unused.

This is of course a very particular case but it is useful when
1. Updating a full frame is slow.
2. A frame buffer is not used.
3. Less memory can be used.

## Description

Void Mapper is a stand-alone c-library that takes a list of rectangles and a screen size as argument, returning a list of rectangles that covers all the areas not part of the sprites. Basically it maps all the void.

It is designed to be used on an embedded device such as a micro controller and does not make use of dynamic allocation of the memory. It uses integers, and the integer size is defined. By default it is working in 16 bits, but it can be altered to 32 bits with ease.

It uses Make for building the example, designed to be compiled with GCC and it thoroughly unit tested with the Check framework.

## Dependencies

Check needs to be installed in order to build. It can be found in many package managers such as `apt` or `brew`. If the example is
built, the Makefile uses `pkg-config` to locate check.

For Linux:
```
sudo apt-get install pkg-config check
```

For Mac:
```
brew install pkg-config check
```
