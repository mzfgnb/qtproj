
# To compile project and run

### For Qt6:

  **On Arch/Manjaro**

    ```
      qmake6
      make
      g++ main.o labrecordsapp.o moc_labrecordsapp.o -lQt6Widgets -lQt6Gui -lQt6Core
      ./a.out
    ```

### For Qt5:

  **On Arch/Manjaro**

    ```
      qmake5
      make
      g++ main.o labrecordsapp.o moc_labrecordsapp.o -lQt5Widgets -lQt5Gui -lQt5Core
      ./a.out
    ```

You also may find an issue that doesn't allow you to run file with the `Permission denied` error, run `chmod 777 a.out`
