# C Project Rulles

File structure:

  - "ProjectName" // Project Dir
  
    -  cproject.json { 
      
             "Type": "ProjectType", 
             "Libs": [], 
             "InclPaths": [],
             "LibPaths": []
             "FileDeps": []
             ...
       }
       
    -   README.MD // Describtion of the project
    -   public
          - file.h
          - ...
    -   srs 
          -  file.cpp
          -  subfolder 
          - ...
