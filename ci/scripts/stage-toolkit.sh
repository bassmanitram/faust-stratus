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
cp -pr $OLDDIR/srcs/stratus-python .
cp -pr $OLDDIR/srcs/faust-stratus .
cp -pr $OLDDIR//tests .
cp -pr $OLDDIR/srcs/install/* .

cd ${OLDDIR}
zip -r faust-stratus.zip ${WORKDIR}/*
