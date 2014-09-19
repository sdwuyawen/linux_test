#!/bin/bash  
# testset.sh  
echo 'disable exit on non-zero return status and pipefail track'  
set +e  
set +o pipefail  
a=$[1/0]|b=2  
echo 'return status = '$?  
  
echo 'disable exit on non-zero return status but enable pipefail track'  
set +e  
set -o pipefail  
a=$[1/0]|b=2  
echo 'return status = '$?  
  
echo 'enable exit on non-zero return status and pipefail track'  
set -e  
set -o pipefail  
a=$[1/0]|b=2  
echo 'return status = '$?  
