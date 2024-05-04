#!/bin/bash
echo "File Sizes of .ll Files" > file_sizes.txt
echo "-----------------------" >> file_sizes.txt
for file in tests/*.ll
do
  echo "$(du -sh "${file}")" >> file_sizes.txt
done


