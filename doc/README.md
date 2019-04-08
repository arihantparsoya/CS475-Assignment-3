# Assignment 3

## Members
* Arihant Parsoya - 15D260012
* Dylan Woods - 18v051002


## Youtube Video
Link: https://www.youtube.com/watch?v=t50SEqlE-Rw&feature=youtu.be

## Keyboard Shortcuts

### Moving the Parts
Switch between the 2 models by pressing `M`.
Keys to pick the body parts to control:

1 - torso 
2 - neck
3 - head
4 - left tigh 
5 - right thigh leg 
6 - left lower leg 
7 - right lower leg 
8 - left upper arm 
9 - right upper arm 
R - left lower arm
L - right lower arm

Once a body part is selected, use the arrow keys as well as page-up/page-down to control them. 

Select the box by pressing B.
Open and close the lid of the box by using the keys `O` and `C`.

## keyboard Shortcuts for Moving the Camera

Changing the angle of the Cameras: 
A - Around the Y axis by -10 degrees.
D - Around the Y axis by +10 degrees.
W - Around the X axis by -10 degrees.
S - Around the X axis by +10 degrees. 
Q - Around the Z axis by -10 degrees.
E - Around the Z axis by +10 degrees.

Changing the position of the camera:
H - Left
K - Right 
U - Up 
J - Down 
I - Forward(Zoom in)
Y - Backwards(Zoom out)

Each move changes the position of the camera by 3 units. The Camera lookat stays the same as the camera moves. It is always directed at the center of the room.

### Keyframe the Camera

The camera keyframes are saved in the `camera_positions.txt` file. To save keyframes, first, the camera positions are recorded by the program using the `Z` key. The user can check all the positions he has recorded by pressing `X` key which shows only the saved keyframes, then, the user can press `N` key to switch between the recorded keyframes. After all the positions are recordrd, the user can press `V` key to export all the recorded positions to the `camera_posiitons.txt` file. 

`Z` - Record the current camera position and orientation as a keyframe.
`X` - Switch to a mode where only the recorded camera keyframes will be shown. Same key is used to swith back to the normal viewing mode.
`N` - When the user is in the mode where only camera keyframes are shown (i.e. by pressing `X` key), he can press `N` to swith between different keyframes.
`V` - Export the camera keyframes in the `camera_positions.txt` file.
`T` - Import and load all the keyframes saved in `camera_positions.txt` file into the program.
`G` - Erase all the keyframes saved in `camera_positions.txt` and `keyframes.txt` file.

### Keyframe the Model Positions

The model keyframes are saved in the `keyframes.txt` file. By pressing the key `F`, the current model positions are directly exported to the `keyframes.txt` file. When pressing the key `F`, the user is asked to enter the time of that keyframe in the console. This time value is used to interpolate the model positions in the final animation.

`F` - Save the current positions as a keyframe in the `keyframes.txt` file. After pressing the `F` key, you will be asked to enter the time in the console. This time is used to interpolate the keyfarme positions during animation.
`.` - Import the model keyframes from the `keyframes.txt` file.
`G` - Erase all the keyframes saved in `camera_positions.txt` and `keyframes.txt` file.

### Control the Lights

`[` - Toggle between ON/OFF state of light 1.
`]` - Toggle between ON/OFF state of light 2.

### Animating

To start the animation, press `T` (to import the camera keyframes) and `.` (to import the model keyframes) into the program. Then press `SPACE` to start the animation. The time variable for the animation is saved in the `hierarchial_modelling.cpp` file.


`SPACE` - To start the animation from the beginning. This key can also be used to pause the animation.


The following variables control the time duration of the animation. 
`animation_start_time` - The timestamp value when the animation begins when the user presses `SPACE`. 
`camera_animation_duration` - The duration until which the camera will be animated. The camera positions are interpolated using a bezier curve within this time. 
`animation_duration` - The total duration until which the animation runs. The model positions are linearly interpolated during this time.
