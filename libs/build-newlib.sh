#Get the compiler's binary path
export COMPILER_BINS=`pwd`/cross-compiler/compiler/bin
#Add the compiler to path
echo 'Adding compiler to PATH environment'
export PATH=`pwd`/cross-compiler/compiler:$PATH
#Add it's binaries to the path environment
echo 'Adding cross compiler binaries to PATH environment'
export PATH=`pwd`/cross-compiler/compiler/bin:$PATH
MACHINE=`gcc -dumpmachine`
cd newlib-build
../newlib-src/configure --host="$MACHINE" --build="$MACHINE" --target='i686-tinaos' --with-sysroot="$COMPILER_BINS" --disable-maintainer-mode --enable-newlib-elix-level=4 --disable-werror --disable-newlib-io-float --disable--libgloss
make
make DESTDIR="$COMPILER_BINS" install