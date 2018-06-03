: ' CPSC 457
Spring 2018
Assignment 2 - Q5
File: scan.sh
Usage: ./scan.sh "suffix" "Number of Files (N)"
To sort N files, names ending with the given suffix, in bytes and printing out sum -
- of file sizes.
Name: Siddharth Kataria
UCID: 30000880 '

: 'References:
https://sites.google.com/site/pfederlcpsc457s18/assignments/assignment-2
https://stackoverflow.com/questions/43158140/way-to-create-multiline-comments-in-bash
https://stackoverflow.com/questions/11223977/printf-in-c-pointer-variable-for-p-c-s
https://stackoverflow.com/questions/6438896/sorting-data-based-on-second-column-of-a-file
https://likegeeks.com/awk-command/
'

#!/bin/bash

find -type f -name "*$1" -printf "%p	" -printf "%s \n" |   #finiding files with suffix
 sort -k2 -n -r |	#sorting file based on their sizes (2nd coloumn)
 head -$2 |		#Choosing only 'N' files out of all files found
 awk '{print $1 " " $2; sum += $2}  END { print "Total size: " sum }'	#Using end function for recursion and printing out total sizes
 
