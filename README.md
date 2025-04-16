
# To compile project run

# For Qt6:

  # On Arch/Manjaro

    ```
    qmake6
    make
    g++ main.o labrecordsapp.o moc_labrecordsapp.o -lQt6Widgets -lQt6Gui -lQt6Core
    ```

# For Qt5:

  # On Arch/Manjaro

    ```
    qmake5
    make
    g++ main.o labrecordsapp.o moc_labrecordsapp.o -lQt5Widgets -lQt5Gui -lQt5Core
    ```
