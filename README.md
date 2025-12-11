## My attempt to Advent of code 2025

In 2023 I attempted do this for learning ocaml, I had fun, a lot, but I didn't finished
both becasue I started on the middle of december,
but mostly because vacation means family time, and working far from home makes desire family time more that anything.

The approach for this year is "use the best tool for the job"
where with "best tool for the job" is usually a thing I am familiar with,
since it is on of the less objective and flexible definition you can give to anything.
So mostly arrows already in my quiver. And maybe something else if it is funnier.



## Puzzle 1

The dial that it is evil when tourned counterclockwise.
Count __when you stop on 0__, and then count when __you pass on 0, and whan you stop on 0__.

I wanted to flex thata I read the manual for `std::tie` with `std::div` at least for turning clockwise,
and I ended up in bruteforcing everithing: if it works and does not need to go blazingly fast it is ok.
For the second part: the evilness was that if you go backward starting from 0 you need to apply the pbc once,
but you do not need to count the first pbc passage as a click on 0 for that turn!

## Puzzle 2

That was surprisingly easier that I tough: becasue I am used to the evil world that are regexes.
I discovered that some engine can autorefer to the capture groups in the matching string.

The puzzle is about finding number that in base 10 are represented by repetition(s) of the same combination of digits from start to end (line 1212 or 343434) in a sequence.

Initially I wanted to do this in bash. The test string passed, then the real one took more time that my patience would allow.
So cpp.

I wanted to try `format`, but of course nothing on my pc is compatible with `<format>`. So I opted for the classic `sstream`.
The evilness here it is that some of the numbers are bigget that the maximun integer representable with 32 bits (even unsigned!!!).
This is why I used also `sstream` instead of the only `fscanf`, my compiler refused long integer as input of fscanf,
even if the documentation was ok with it.

In the end the second part was just a matter of knowlege of how the regex work.
Maybe you can solve it by looking at x and x+half to see if they are always equal
(and change the half in something else for the secon part) might be more efficient,
but the low hanging fruit here was the regex engine that someone already built for me

## Puzzle 3

This looks easy at firt sight: get the highest number by combining two digits in order in the same row.
The bruteforce approach might be to take the nth digit, and sort the array from n+1 by getting the highest number, rinse and repeat for the others, then take the highest nth + other  couple.
I am sure that there is an algorithm that does this at command, but I cannot think about it now
In the end I did not bruteforced part1 in the way I explained above: i walked the array backward, storing the biggest digit found until that step and I summed it to 10* the current digit, and stored the bigged sum found

I followed a similar strategy for part2:

 - store the 12 digit of the final number in an array
 - if the new digit is bigger than the most significant one swap it with that in that case
   - then repeat this with the next significant digit using the swapped out digit
   - repeat this until there are no more digits in the result array or if the swap did not happened
 - turns out that to work the "bigger" should be and "bigger or equal", luckily the authors of aoc give us a very generous test set that caught that hiccup

## Puzzle 4

For part1 is matter of counting neigbors, it looks easy, so that I am sure that will be 
a "we did it not because it is easy, but becasue we tought it was easy" situation.

I want to try to store the data using less memory possible

So I used the std::biset, that was a good idea becasue part 2 felt like a breeze thanks to the bitwise logic :3


## Puzzle 5

This seems easy, after puzzle 1 now I will use 64 bit integer for life.
First part is count numbers within interval once.

Second part is counting all the number in the intervals, but avoiding double counting.
I think I will go for a sane "merging the intervals, then sum all the ranges". It worked, but I had to go to reddit and found that the key was to sort.

## Puzzle 6

This was a parser problem, the "difficult" part in part 1 was to understand how many entries per line per file.
I hardcoded the number of lines for my sanity.

And I took the apporach of reaading again and again the file to have a easier and more straighforward logic that trying with a smart solution and failing spectacularly.

## Puzzle 7

I am solving a simplified wave equation?
Maybe not, just counting the splits was easier that read, I had to count the number of splitter that have been hit.

Let's see what is the evilness in the second part

It it a mathematical evilness, I know there is a formula, but I have to remember it (or I can bruteforce this in the most recursive way possible)

Basically the solution is walking the tree in all the possible combinations. With the twist that this is not a binary tree, but a node can be tranversed in some different paths (all the paths have the same lenght, at least)

I'll try by creating a second array with the number of each possible path that bringed there, and then sum

__PRO TIP:__
changing 
```c++
  splits2 = std::accumulate(paths.back().begin(), paths.back().end(), 0);
```
into
```c++
  splits2 = std::accumulate(paths.back().begin(), paths.back().end(), 0ull);
```
will solve the puzzle

## Puzzle 8

Day 8 part 1 looks familiar to me (I may steal something like the "makewhole" functionality from PLUMED)

I searched what a DSU is, and I sorta implemented one, then I implemented a proper one for the part 1 and succeed.

Second part was easier than I hoped, basically is the first one, but "heavier" on the machine (a ~1000x1000 array is not that heavy), I had to remove the heap I used in part1 becasue I needed all the distances

##Puzzle 9

Part 1 is suspiciosly too simple.

Part 2 is evil at first read, now I have to check that the rectangle is completely encased within the area that is traced by the given points
