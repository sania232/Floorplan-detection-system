<h1> Object Detection </h1>
This floorplan detection system is used to take a floorplan like the one below
![floorplan](https://user-images.githubusercontent.com/38992928/154818509-2136ac3a-81aa-46de-83d7-3a12983891eb.png)

and then uses the YOLO v3 algorithm to draw bounding boxes around the its basic elements(doors, windows, walls)
![image](https://user-images.githubusercontent.com/38992928/154818577-174428a0-4181-43f0-865f-85a00a2bbce3.png)

However, due to the bounding boxes not being 100% accurate, a post processing algorithm is used make sure coordinates are 10px of each other (like walls). Lastly, the JSON file is written in the following format.
![image](https://user-images.githubusercontent.com/38992928/154818709-984e4f4a-64b5-4457-9ad0-54a202494f82.png)

