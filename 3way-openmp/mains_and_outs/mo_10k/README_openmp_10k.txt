(to run when executable is there skip to step 5)

HOW TO REBUILD:
CHANGE VARIABLE IN MAIN "LINES_TO_READ": 
1. Make executable:
    make

2. Move executable to different folder
    mv openmp-exc /homes/jjeilert/proj4/hw4/3way-openmp/mains_and_outs/mo_10k/openmp-10k

3. Navigate to folder with the shell scripts
    cd mains_and_outs/

4. continue
    cd mo_10k/

//CHANGE "jjeilert/proj4" to own directory in cores shell script
5. check script:
    chmod +x openmp_10k_batch.sh

6. check other script:
    chmod +x cores_10k_openmp.sh

7. submit jobs:
    ./cores_10k_openmp.sh

use kstat --me to check if jobs submitted
should output txt file and time file

