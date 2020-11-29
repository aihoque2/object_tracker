# object_tracker

This is an object tracker I developed to automate data collection for experiments in the Biological Machines research group at Saif lab at UIUC. The purpose of this program is to track the discplacement of moving muscle pillars from video datasets. More can be read about it in the links below: 

-  https://aip.scitation.org/doi/full/10.1063/1.5134477
-  https://www.pnas.org/content/116/40/19841

## Method
The dataset of images looked roughly like the following:

![raw image of the muscle strips](https://github.com/aihoque2/object_tracker/blob/master/readme_images/test21_1_wells.png?raw=true)

I would like to detect the squares in the image, as visualized below: 

[highlighted image of the muscle strips](/readme_images/highlighted_wells.png)

the objects are detected with a Hough Circle Transform in openCV. They are then processed to bring out the square shape of the muscle pillars. 

## Demo


## Requirements
- C++
- g++ >=7.0.0 
- OpenCV >=3.2.0

## TODO
If I ever come back to this project, I would like to implement some other features:

- Parallel tracking of multiple detected objects with OpenMP
- Utilize my GPU for better performance in tracking
- ability switch between object detectors (rather than use my circle transfrom, use a Single Shot Detector for other objects, etc.) 
