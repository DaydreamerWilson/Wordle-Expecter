# Wordle-Expecter
- A side project to determine the best word to enter Wordle to narrow down the vocabulary list. (Same method as 3Blue1Brown, I stole his vocabulary list, and also his idea)

- This is only tested on Windows 10 and 11, it is not recommended to compile or run on any other platform due to dynamic linking

- My slogan is < 0% optimization with 100% brute force >

- How to use the program:
  - 1) Wait for list of recommend words to appear (Might takes up to minutes, depends on the number of words remaining, and your CPU spec)
  - 2) Enter word with top entropy ranking
  - 3) Repeat the word in the program, and enter a five digit code representing (0: miss, 1: grey, 2: green)
    - Example:
    -   penis
    -   00102
  - 4) repeat 1 and 2 until finished the Wordle riddle
  - 5) enter -r to reset the word list 
