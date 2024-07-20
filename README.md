# Meeting Data Visualizer

## CO222 Programming Methodology | Project 2

### Project Description

Developed a program to analyze data files of online meetings and visually represent the data based on the user's requirements. The program can display the number of meetings, number of participants, or duration of meetings in a horizontal bar chart format. 

### Features

- Analyze multiple CSV files containing meeting data.
- Display data in various modes: number of meetings, number of participants, and meeting duration.
- Support for scaling the bar chart to fit the screen width.
- Configurable number of rows in the bar chart.
- Error handling for command-line arguments.

### Technologies Used

- C
- Linux

### Project Type

- Individual

### Usage

1. **Compile the Program:**
    ```bash
    gcc -std=c99 -o meeting_data_visualizer 18115Project2.c
    ```

2. **Run the Program:**
    ```bash
    ./meeting_data_visualizer [options] file1.csv file2.csv ...
    ```
   - `[options]` can include:
     - `-l <number>`: Specifies the number of rows in the bar chart.
     - `--scaled`: Scales the bar chart to fit the screen width.
     - `-m`: Display the number of meetings.
     - `-p`: Display the number of participants.
     - `-t`: Display the meeting duration.

### Example

- Running the program to display the number of meetings with scaling and a limit of 10 rows:
    ```bash
    ./meeting_data_visualizer -m --scaled -l 5 meetings1.csv meetings2.csv
    ```

- Expected output:
    ```bash
    +----------------------------+
    | Meeting Data Visualizer    |
    +----------------------------+
    | Name        | Meetings     |
    |-------------|--------------|
    | John Doe    | |||||||||    |
    | Jane Smith  | |||||||      |
    | ...         | ...          |
    +----------------------------+
    ```

### Author

- Gowsigan A

