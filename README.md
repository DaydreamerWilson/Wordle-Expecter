# Wordle-Expecter
- A side project to determine the best word to enter Wordle to narrow down the vocabulary list. (Same method as 3Blue1Brown, I stole his vocabulary list, and also his idea)

- This is only tested on Windows 10 and 11, it is not recommended to run on any other platform due to dynamic linking
- However, in theory it is possible to compile on other platform with compiler version newer or equal to C++19, since it only used standard libraries

- My slogan is < 0% optimization with 100% brute force >

- How to use the program:
  - 1) Wait for list of recommend words to appear (Might takes up to minutes, depends on the number of words remaining, and your CPU spec)
  - 2) Enter word with top entropy ranking on Wordle
  - 3) Repeat the word in the program, and enter a five digit code representing the correctness (0: miss, 1: grey, 2: green)
    - Example:
    -   aisle
    -   00102
    -   ![image](https://github.com/DaydreamerWilson/Wordle-Expecter/assets/20011545/4f8ae40c-c570-4049-af1d-beb66337cb13)
  - 4) repeat 1 and 2 until finished the Wordle riddle
  - 5) enter -r to reset the program and start fresh
