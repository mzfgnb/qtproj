
# To compile project and run the instruction

### For Qt6:

  **On Arch/Manjaro**

    
    qmake6 
    make 
    g++ main.o labrecordsapp.o moc_labrecordsapp.o moc_umldiagramwindow.o umldiagramwindow.o moc_chartwindow.o chartwindow.o -lQt6Widgets -lQt6Gui -lQt6Core -lQt6Charts
    ./a.out  

   **or use this one-click command:**
   ```qmake6;make;g++ main.o labrecordsapp.o moc_labrecordsapp.o moc_umldiagramwindow.o umldiagramwindow.o moc_chartwindow.o chartwindow.o -lQt6Widgets -lQt6Gui -lQt6Core -lQt6Charts```

### For Qt5:

  **On Arch/Manjaro**

    
    qmake5 
    make 
    g++ main.o labrecordsapp.o moc_labrecordsapp.o moc_umldiagramwindow.o umldiagramwindow.o moc_chartwindow.o chartwindow.o -lQt5Widgets -lQt5Gui -lQt5Core -lQt5Charts
    ./a.out   

   **or use this one-click command:**
   ```qmake5;make;g++ main.o labrecordsapp.o moc_labrecordsapp.o moc_umldiagramwindow.o umldiagramwindow.o moc_chartwindow.o chartwindow.o -lQt5Widgets -lQt5Gui -lQt5Core -lQt5Charts```

You also may find an issue that doesn't allow you to run file with the `Permission denied` error, run `chmod 777 a.out`
