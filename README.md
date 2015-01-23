# CS-214-Search
Fall 2014 with Brian Russell


For the indexer, a hash map was used as our data structure. The hash map has 36 keys going
from a – z and 0-9, and can be filled with linked lists of structs for each key. The tokenizer runs in O(n) time
and the print method uses two nested loops. One loop is used to print out the tokens from the initial
linked lists and the second loop is used to obtain the linked list of records connected to the token linked
list. The indexer uses recursion to loop through directories and find the proper path names to be printed
out to the inverted-index file.

The search program then stores the inverted-index file into a linked list in ascending order. In
the linked list, the nodes contain the value of the token and an array of the path names that they are
found in. Linear search is used for both types of search commands. Thus, the resulting run time for SO is
O(m * n) since it runs through the user inputs as an array and through the array of path names.
To implement to SO query, all of the token names and the path names from the inverted-index file was stored into a linked list. From there we compared the inputs from the query against our linked list and printed out any matches.

The SA query applied a similar method in addition to using an extra array. First, we had a count
that would equal the number of input queries. Then, for every match, we stored the result into an array.
Going through that array at the end, we kept another count that would keep track of the number of
occurrences for a specific path name. If that count equaled the initial count, then the SA method should
print out the result accordingly. This method was run in O(n^2) time since it had to run through the
same array in two for loops.

All use of this code must comply with the [Rutgers University Code of Student Conduct](http://eden.rutgers.edu/%7Epmj34/media/AcademicIntegrity.pdf).
