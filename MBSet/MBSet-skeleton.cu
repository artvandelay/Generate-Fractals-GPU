/* 
 * File:   MBSet.cu
 * 
 * Created on June 24, 2012
 * 
 * Purpose:  This program displays Mandelbrot set using the GPU via CUDA and
 * OpenGL immediate mode.
 * 
 */

#include <iostream>
#include <stack>
#include <cuda_runtime_api.h>
#include <stdio.h>
#include "Complex.cu"

#include <GL/freeglut.h>

// Size of window in pixels, both width and height
#define WINDOW_DIM            (512)
// Maximum iterations for the MBSet calculations
#define MAX_IT                (2000)

using namespace std;

// Initial screen coordinates, both host and device.
Complex minC(-2.0, -1.2);
Complex maxC(1.0, 1.8);
Complex* dev_minC;
Complex* dev_maxC;

// Block and thread counts for CUDA
dim3 blocks(WINDOW_DIM/8, WINDOW_DIM/8);
dim3 threads(8, 8);



int main(int argc, char** argv)
{
  // Initialize OPENGL here
  // Set up necessary host and device buffers
  // set up the opengl callbacks for display, mouse and keyboard

  // Calculate the interation counts
  // Grad students, pick the colors for the 0 .. 1999 iteration count pixels

  glutMainLoop(); // THis will callback the display, keyboard and mouse
  return 0;
  
}
