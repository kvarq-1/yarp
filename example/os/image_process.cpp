// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
#include <ace/config.h>

#include <yarp/os/all.h>
#include <yarp/sig/all.h>

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::sig::draw;

/*
  This example adds a moving circle to an image stream.

  Suppose we have an image source on port /source such as:
    yarpdev --device test_grabber --name /source --mode line --framerate 10

  And suppose we have an image viewer on port /view:
    yarpview --name /view

  Then we can hook this program up in between as follows:
    ./image_process --name /worker
    yarp connect /source /worker
    yarp connect /worker /view

  You should see the normal scrolling line of test_grabber, with a moving
  circle overlaid.

 */

int main(int argc, char *argv[]) {

    // Initialize network
    Network::init();

    // Make a port for reading and writing images
    BufferedPort<ImageOf<PixelRgb> > port;

    // Get command line options
    Property options;
    options.fromCommand(argc,argv);

    // Set the name of the port (use "/worker" if there is no --name option)
    ConstString portName = options.check("name",Value("/worker")).asString();
    port.open(portName);
  
    int ct = 0;
    while (true) {
        // read an image from the port
        ImageOf<PixelRgb> *img = port.read();
        if (img==NULL) continue;

        // add a blue circle
        PixelRgb blue(0,0,255);
        addCircle(*img,blue,ct,50,10);
        ct = (ct+5)%img->width();

        // output the image
        port.prepare() = *img;
        port.write();
    }
  
    Network::fini();
    return 0;
}
