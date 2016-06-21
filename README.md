# Aldebaran

## How to get the code

`git clone --recursive https://github.com/davidsiaw/aldebaran`

## Raspberry PI

The Raspberry PI does not have all the GUI goodies, so we run on OpenGLES directly on VideoCore. Input is handled via udev. This means you need to set the FOR variable when you build the code.

Be sure to install the appropriate libraries before running the make command

`sudo apt-get install libudev-dev`

`make run FOR=RASPBERRYPI`
