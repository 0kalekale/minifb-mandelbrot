/*
  miniFB Mandelbrot set example by 0kalekale
*/

#include <MiniFB.h>
#include <stdio.h>
#include <stdint.h>
#include <complex.h>

static uint32_t g_width = 600;
static uint32_t g_height = 600;
static uint32_t *g_buffer = 0x0;

void 
resize(struct mfb_window *window, int width, int height) {
  (void) window;
  g_width  = width;
  g_height = height;
  g_buffer = realloc(g_buffer, g_width * g_height * 4);
}

int 
iter(double complex z) {
  int it = 0;
  double complex c = z;
  while (cabs(z)<=4&&(it<30)) {
    z = (z*z)+c;
    it++;
  }
  return 30-it;
}

int 
main()
{
  
  struct mfb_window *window = mfb_open_ex("Mandelbrot Set", g_width, g_height, NULL);
  if (!window)
    return 0;
  
  g_buffer = (uint32_t *) malloc(g_width * g_height * 4);
  
  mfb_set_viewport(window, 50, 50, g_width - 50 - 50, g_height - 50 - 50);
  resize(window, g_width - 100, g_height - 100);  // to resize buffer
  
  mfb_update_state state;
  do {
    int i = 0;
    for (int x = 0; x< g_width; x++) {
      for (int y =  0; y< g_height; y++) {
	double re = (double)(2*(y-300))/(double)300;
	double im = (double)(2*(x-300))/(double)300;
	double complex z = CMPLX(re, im);
	int it = iter(z);
	if (it == 0) {
	  g_buffer[i] = MFB_RGB(0x00, 0x00, 0x00);
	}
	else {
	  switch(it%6) {
	  case 0:
	    g_buffer[i] = MFB_RGB(0xFF, 0x00, 0x00);
	    break;
	  case 1:
	    g_buffer[i] = MFB_RGB(0x80, 0x80, 0x00);
	    break;
	  case 2:
	    g_buffer[i] = MFB_RGB(0x00, 0xFF, 0x00);
	    break;
	  case 3:
	    g_buffer[i] = MFB_RGB(0, 0x80, 0x80);
	    break;
	  case 4:
	    g_buffer[i] = MFB_RGB(0, 0, 0xFF);
	    break;
	  case 5:
	    g_buffer[i] = MFB_RGB(0, 0, 0xFF);
	  }	
	}
	++i;	
      }
    }
    
    state = mfb_update_ex(window, g_buffer, g_width, g_height);
    if (state != STATE_OK) {
        window = 0x0;
        break;
    }
  } while(mfb_wait_sync(window));
  
  return 0;
}
