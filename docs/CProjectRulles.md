# C Project Rulles (Outdated)

File structure:

  - "ProjectName" // Folder
  
    -  cproject.json { 
      
             "Type": "ProjectType", 
             "InternLibs" : [ "Types" ] ,
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