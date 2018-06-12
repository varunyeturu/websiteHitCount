**************************COMPILING THE SOURCE***************************
gcc hitCount.c sort.c -o hitCount
**************************RUNNING THE EXECUTABLE*************************
./hitCount sample.txt

***********************************
Time Complexity of the Program:

readAndStoreData --> Reads the entire file in a byte stream one time --> O(1)
storeDate	 --> Called for all the lines in the file to store the data --> O(number of lines in the file) --> O(1)
storeWebsiteData --> Called for all the lines in the file to store the data --> O(number of lines in the file) --> O(1)
displayHits	 --> O(Total Number of Unique Dates * displayWebsiteLists())
		     = O(Total Number of Unique Dates * nLogn) (where n = unique websites visited per day)
nLogn is the time required to sort the entries based on the hitCount.
freeWebsiteList  --> O(n), where n is the unique websites list per day.

Total time complexity = O(1) + O(1) + O(1) + O(nLogn) + O(n), where n = unique websites visited per day.
 
***********************************
Space Complexity if the program:
Constant space for storing unique websites per day with respective hits --> O(1)

***********************************
