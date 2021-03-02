# C Project Rulles

File structure:

  - "ProjectName" // Folder
  
    -  cproject.json { 
      
             "Type": "ProjectType", 
             "Files": [], // cpp files
             "Libraries": [], 
             "IncludePaths": [],
             "LibPaths": []
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
