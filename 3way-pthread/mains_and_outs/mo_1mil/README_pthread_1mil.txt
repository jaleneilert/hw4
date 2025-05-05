(to run when executable is there skip to step 5)

HOW TO REBUILD:
CHANGE VARIABLE IN MAIN "LINES_TO_READ": 

1. (in folder 3way-pthread) Make executable:
    make

2. Move executable to different folder
    mv pthread-exc /homes/jjeilert/proj4/hw4/3way-pthread/mains_and_outs/mo_1mil/pthreads-1mil

3. Navigate to folder with the shell scripts
    cd mains_and_outs/

4. continue
    cd mo_1mil/

//CHANGE "jjeilert/proj4" to own directory in cores shell script
5. check script:
    chmod +x pthread-1mil-batch.sh

6. check other script:
    chmod +x cores_1mil_pthread.sh

7. submit jobs:
    ./cores_1mil_pthread.sh

use kstat --me to check if jobs submitted
should output txt file and time file

