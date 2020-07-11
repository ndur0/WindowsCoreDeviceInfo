# WindowsCoreDeviceInfo

Based on Clement Labro's (itm4n) awesome research and project (UsoDllLoader & WindowsCoreDeviceInfo) 

This DLL project was designed to trigger a payload (ie: smb beacon) whenever the QueryDeviceInformation() function is called by Windows usoclient.exe (my other repo has C# version of the client SharpUsoDllLoader to use with CS execute-assembly).  

Must add payload to the RawData.h file
    // choose .bin output from CS ---> hint: run through Donut
    // open in HxD editor, select all, edit, copy as -> C
    // paste between curly braces * dont forget to add the size ie: ENTER SIZE HERE
    

All credit to Clement Labro (itm4n)  
For more information:  
Part 1 - https://itm4n.github.io/usodllloader-part1/  
Part 2 - https://itm4n.github.io/usodllloader-part2/   
Project - https://github.com/itm4n/UsoDllLoader.git
    
