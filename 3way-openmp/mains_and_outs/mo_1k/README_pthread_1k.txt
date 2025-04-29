(go to folder 3way-openmp and change lines to read)
1. Make executable:
    make

2. Move executable to different folder
    mv openmp-exc /homes/jjeilert/proj4/hw4/3way-openmp/mains_and_outs/mo_1k/openmp-1k

3. Navigate to folder with the shell scripts
    cd mains_and_outs/

4. continue
    cd mo_1k/

5. check script:
    chmod +x openmp_1k_batch.sh

6. check other script:
    chmod +x cores_1k_openmp.sh

7. submit jobs:
    ./cores_1k_openmp.sh

use kstat --me to check if jobs submitted
should output txt file and time file

