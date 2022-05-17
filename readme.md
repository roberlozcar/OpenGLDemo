# OpenGL Demo

## Introduction

This is a demo for OpenGL. It draws a set of cube with different trajectories and rotations. After that, it applies different post-processes: depth of field and motion blur effects, some convolutional mask (Gaussian, Laplacian and unsharp filters), a distortion and a desaturation.

It used two set of shaders, the first one draw the geometry, create a buffer with the information of position, colors, depth and normal,  and the second one finish the image adding lights, with the information of the calculated buffer, and post-processes. It is called deferred shading and it is used to reduce the cost of lighting in complex scenes with many fragments and lights.

## Options

- The user can change the position of the camera with "wasd" and it orientation with "q", "e" and the mouse.
- The focal distance and max distance factor of the depth of field effect can be changed with "+" and "-" and "m" and "l", respectively.
- The motion blur can be enabled and disabled with "h" and its alpha is changed using "b" and "v" (more of less effect).
- Also, the user can change the type of light (spotlight, point light or directional light) of the scene.
- The type of convolutional mask is changed with "c".

## Questions

Write an email to r.lozano93@gmail.com.
