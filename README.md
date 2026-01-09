# 3D-Ray-Tracing

A lightweight ray-tracing project made in C, using the minimal OpenGL-based library MLX42 to render images on screen. Designed with physically based rendering principles.<br><br>

## How to use:

### 🖼️ Camera/View Control
**Move Camera:** `WASD` and `QZ`
    Press `W` or `S` to move forwards or backwards.
    Press `A` or `D` to move left or right.
    Press `Q` or `Z` to move up or down.

**Rotate Camera:** 
    Press `arrow keys` to rotate the camera view.

**Adjust Ray Depth:**
    Press `B` or `N` to add or reduce maximum bounces (range from 1 to 5).

**Adjust Ambient brightness:**
    Press `L` or `K` to increase or reduce ambient light brightness.

### 🔵 Object Control
**Automate Object Rotation:**
    Press `1` to automate rotation for all spheres with a texture.
    Press `2` to stop the autorotation.

**Translate Object:**
    Use `left mouse button` to drag the object to your preferred place.

### 🗔 Window Control
**Resize Window:**
    Stretch the window frame with mouse to change the size of image (horizontal FOV stays constant).

**Close Window:**
    Press `Esc` or click on the cross on the window frame to close the window and quit the program.<br><br>

## Examples

<img width="3832" height="2064" alt="necklace" src="https://github.com/user-attachments/assets/614105d3-85d5-4811-9795-21f33feed9c7" />

*Figure 1 Beaded neon necklace under colored spotlights* <br><br>

![room](https://github.com/user-attachments/assets/4f2155af-25c9-4b58-a963-3266521b81d1)
*Figure 2 Rotating metal ball in the room with reflections* <br><br>

<img width="3832" height="2064" alt="icelake1" src="https://github.com/user-attachments/assets/edae5fb2-ecb5-4a54-b22f-66956b5d01d0" />

*Figure 3 Snowballs stacked on an ice lake ready for a tournament. The scene shows materials with different reflectance properties - the glossy green pole has high albedo with strong specular (mirror-like) reflection; the snow objects have high albedo dominated by diffuse reflection with negligible specular components; and the rough ice surface has moderate albedo that increases sharply at low viewing angles, producing the observed glitter path*
