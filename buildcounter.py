# Open a file in read mode
get = open("buildnumber.txt", "r")
print("buildnumber: ", get.name)

buildnumber = "0"
get.read( buildnumber)

print(buildnumber)

#set

set = open("buildnumber.txt", "w")
print("Name of the file: ", set.name)

# Assuming file has following 5 lines
# This is 1st line
# This is 2nd line
# This is 3rd line
# This is 4th line
# This is 5th line

buildnumber = buildnumber + 1
# Write a line at the end of the file.
set.write(buildnumber)

# Close opened file
set.close()