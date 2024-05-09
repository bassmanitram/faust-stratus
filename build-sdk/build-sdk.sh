##
## A shell script to build the Stratus installation for an easier life when constructing
## Faust-based effects. It's effectively just two docker commands, so it shouldn't be har to 
## translate into other, non-UNIX-shell contexts
##
cd $(realpath $(dirname $0))
cp ../faust2stratus ../stratus.cpp .
cp ../stratus.py ./stratus-python/stratus/
docker build -f Dockerfile.build_faust_stratus -t faust:stratus .
#docker run -v "$PWD:/mnt/build" faust:stratus cp /root/faust-stratus.tgz /mnt/build/