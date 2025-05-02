A detailed README file is in each of the main and outs for each data size folders located (ex. : "3way-pthread/mains_and_outs/mo_1k")
Resulting times are found in the times folder in each of the three way folders

If looking to rebuild when the executable is not there, you MUST change "LINES_TO_READ" to change how many lines to read from the wiki dump,
then follow the seperate README files to run on beocat


Each Main and Out folder has two shell files one shell will run the executable for all core size (1, 2, 4, 8, 16) (starts with cores in name)

NOTE:
When running the executable file LOCALLY the main takes in two arguments
You must define a text file in which you would like to have the times outputed to
ex: ./pthreads-1k hi.txt