<h1>Image Augmentation</h1>

This was used in training to quickly generate a versatile variety of data mostly focusing on the usage of noisy images and different sides, angles of
rotation and translations (geometric augmentations).


<h1> Object Detection </h1>
This floorplan detection system is used to take a floorplan like the one below

![floorplan](https://user-images.githubusercontent.com/38992928/154818769-90f9f141-93fb-4b94-8c7e-540968dd1551.png)


and then uses the YOLO v3 algorithm to draw bounding boxes around the its basic elements(doors, windows, walls)
![image](https://user-images.githubusercontent.com/38992928/154818577-174428a0-4181-43f0-865f-85a00a2bbce3.png)

However, due to the bounding boxes not being 100% accurate, a post processing algorithm is used make sure coordinates are 10px of each other (like walls). Lastly, the JSON file is written in the following format.
![image](https://user-images.githubusercontent.com/38992928/154818743-8e9c97f9-ed29-4207-a9fc-e25a7d35d41f.png)

