/**
 * Copyright (c) 2012 the Massachusetts Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#include "./graphic_stuff.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "./line.h"
#include "./line_demo.h"

static LineDemo *gLineDemo = NULL;
XSegment *segments = NULL;
XSegment *gray_segments = NULL;


Display *display;

Window window;
Window root;
Window parent;

int screen;
int depth;
int visibility;

int windowwidth;
int windowheight;

static void drawLineSegments(Display *display, Drawable drawable) {
  Line *line;
  unsigned int nsegments;
  window_dimension px1;
  window_dimension py1;
  window_dimension px2;
  window_dimension py2;

  Colormap cmap;
  XGCValues gcval;
  XColor color;
  XColor ignore;
  int64_t fgcolor;
  GC gray;
  GC red;

  cmap = DefaultColormap(display, screen);
  XAllocNamedColor(display, cmap, "gray", &color, &ignore);
  fgcolor = color.pixel;
  gcval.foreground = fgcolor;
  gray = XCreateGC(display, window, GCForeground, &gcval);

  XAllocNamedColor(display, cmap, "dark red", &color, &ignore);
  fgcolor = color.pixel;
  gcval.foreground = fgcolor;
  red = XCreateGC(display, window, GCForeground, &gcval);

  nsegments = LineDemo_getNumOfLines(gLineDemo);
  if (segments == NULL || gray_segments == NULL) {
    segments = malloc(nsegments * sizeof(XSegment));
    gray_segments = malloc(nsegments * sizeof(XSegment));
  }
  XClearWindow(display, window);
  int red_segments_count = 0;
  int gray_segments_count = 0;
  for (unsigned int i = 0; i < nsegments; i++) {
    line = LineDemo_getLine(gLineDemo, i);

    // Convert box coordinates to window coordinates.
    boxToWindow(&px1, &py1, line->p1.x, line->p1.y);
    boxToWindow(&px2, &py2, line->p2.x, line->p2.y);
    // Set line color.
    switch (line->color) {
      case RED:
        // Convert doubles to short ints and store into segments.
        segments[red_segments_count].x1 = (int16_t) px1;
        segments[red_segments_count].y1 = (int16_t) py1;
        segments[red_segments_count].x2 = (int16_t) px2;
        segments[red_segments_count].y2 = (int16_t) py2;
        red_segments_count++;
        break;
      case GRAY:
        gray_segments[gray_segments_count].x1 = (int16_t) px1;
        gray_segments[gray_segments_count].y1 = (int16_t) py1;
        gray_segments[gray_segments_count].x2 = (int16_t) px2;
        gray_segments[gray_segments_count].y2 = (int16_t) py2;
        gray_segments_count++;
        break;
    }
  }
  XDrawSegments(display, drawable, red, segments, red_segments_count);
  XDrawSegments(display, drawable, gray, gray_segments, gray_segments_count);
  XSync(display, 0);
}

static void checkEvent() {
  XEvent event;
  bool block = false;

  while ((XPending(display) > 0) || (block == true)) {
    XNextEvent(display, &event);
    switch (event.type) {
      case ReparentNotify:
        if (event.xreparent.window != window) {
          break;
        }
        XSelectInput(display, event.xreparent.parent, StructureNotifyMask);
        XSelectInput(display, parent, 0);
        parent = event.xreparent.parent;
        break;

      case UnmapNotify:
        if ((event.xunmap.window != window)
            && (event.xunmap.window != parent)) {
          break;
        }
        block = true;
        break;

      case VisibilityNotify:
        if (event.xvisibility.window != window) {
          break;
        }
        if (event.xvisibility.state == VisibilityFullyObscured) {
          block = true;
          break;
        }
        if ((event.xvisibility.state == VisibilityUnobscured)
            && (visibility == 1)) {
          visibility = 0;
          block = false;
          break;
        }
        if (event.xvisibility.state == VisibilityPartiallyObscured) {
          visibility = 1;
          block = false;
        }
        break;

      case Expose:
        block = false;
        break;

      case MapNotify:
        if ((event.xmap.window != window) && (event.xmap.window != parent)) {
          break;
        }
        block = false;
        break;

      case ConfigureNotify:
        if (event.xconfigure.window != window) {
          break;
        }
        if ((windowwidth == event.xconfigure.width)
            && (windowheight == event.xconfigure.height)) {
          break;
        }
        windowwidth = event.xconfigure.width;
        windowheight = event.xconfigure.height;
        XClearWindow(display, window);
        block = false;
        break;

      default:
        break;
    }
  }
}

static void graphicMainLoop(bool imageOnlyFlag) {
  while (true) {
    checkEvent();
    drawLineSegments(display, window);
    if (!imageOnlyFlag && !LineDemo_update(gLineDemo)) {
      return;
    }
  }
}

static void graphicInit(int *argc, char *argv[]) {
  // Initialization
  int64_t fgcolor;
  int64_t bgcolor;
  int64_t eventmask;
  char *host;

  if ((host = ((char *) getenv("DISPLAY"))) == NULL) {
    perror("Error: No environment variable DISPLAY\n");
    exit(1);
  }

  if (!(display = XOpenDisplay(host))) {
    perror("XOpenDisplay");
    exit(1);
  }

  screen = DefaultScreen(display);
  root = RootWindow(display, screen);
  parent = root;
  bgcolor = BlackPixel(display, screen);
  fgcolor = WhitePixel(display, screen);
  depth = DefaultDepth(display, screen);
  windowwidth = WINDOW_WIDTH;
  windowheight = WINDOW_HEIGHT;
  window = XCreateSimpleWindow(display, root, 0, 0, windowwidth, windowheight,
                               2, fgcolor, bgcolor);

  eventmask = SubstructureNotifyMask;
  XSelectInput(display, window, eventmask);

  XMapWindow(display, window);

  XClearWindow(display, window);
  XSync(display, 0);
}

void graphicMain(int argc, char *argv[], LineDemo *lineDemo, bool imageOnlyFlag) {
  gLineDemo = lineDemo;

  // Initialization
  graphicInit(&argc, argv);

  // Entering the rendering loop
  graphicMainLoop(imageOnlyFlag);

  if (segments != NULL) {
    free(segments);
  }
  if (gray_segments != NULL) {
    free(gray_segments);
  }
}
