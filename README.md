# polygons
It's another one of my school projects. It recognizes the shape from given points:
 - nothing (when no points are inputted)
 - point
 - segment
 - triangle
 - quadrangle
 - rhombus (as a specific case of a quadrangle)
 - other

Unfortunately, it is in Polish, because it was forced by the task specification, but you can change the language just by changing defined macros for English words.
It seems that it works for every case, and I got an A for it, but there probably is some way to break it. You can treat it as a challange.

## Input
You input the points into the **punkty.txt** file in the **build** directory.
In the first line the number of the points should be written, in the following lines the points should be inputted as their x and y coordinates seperated by a space character.

## Make
The project uses GNU Make to support the development process. The Make commands are:
  - ```make / make build``` - builds the program
  - ```make debug``` - builds the program in a debug mode
  - ```make run``` - runs the program
