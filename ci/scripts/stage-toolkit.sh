#
# build the install package
#
OLDDIR=$PWD
WORKDIR=$(mktemp -d)

##
## Install cleanup routine
##
for sig in SIGTERM ERR EXIT; do trap "_term ${sig}" ${sig}; done
_term() {
  trap "" SIGTERM ERR EXIT
  rm -rf ${WORKDIR}
}

cd ${WORKDIR}
cp -p srcs/stratus-python .
cp -p srcs/faust-stratus .
cp -p ./tests ./local
cp srcs/install/* .

cd ${OLDDIR}
zip -r faust-stratus.zip ${WORKDIR}/*
