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
