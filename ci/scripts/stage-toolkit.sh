#
# build the install package
#
OLDDIR=$PWD
WORKDIR=build
mkdir ${WORKDIR}

cd ${WORKDIR}
cp -pr $OLDDIR/srcs/stratus-python .
cp -pr $OLDDIR/srcs/faust-stratus .
cp -pr $OLDDIR//tests .
cp -pr $OLDDIR/srcs/install/* .
