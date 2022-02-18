Steps to run exe:
1) Open object_detection.exe
2) run the exe and provide a floorplan image in shared folder
3) view dataset generated in shared folder



Steps to build object_detection on VS:
1) Download opencv 4.1.2
2) Go to Advanced System Settings > Environment Variables >System Variables > Path.
3) Click on ‘Edit’. Now, click ‘New’ to add new environment variable.
4) Copy and paste the path of bin folder inside OpenCV package. The path will look similar to C:\opencv\build\x64\vc14\bin.
5) Open object_detection.vxproj on visual studio
6) Go to properties of the project, then C/C++ > General. Copy the path to include folder of opencv and paste it inside Additional Include Directories. The path will look similar to C:\opencv\build\include. 
7) Go to linker > General. Copy the path to folder containing opencv lib files and paste it inside Additional Library Directories. The path will look similar to C:\opencv\build\x64\vc14\lib. 
8) Go to C++>Language an select C++17.
8) Go to Input. Edit Additional Dependencies and paste the .lib file’s name. Choose the .lib file according to your configuration.Then, click Apply.
9) Build Project

