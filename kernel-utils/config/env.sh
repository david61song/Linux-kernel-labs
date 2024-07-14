# Tweak the compiler flags
# To use clang: compiler_flags="CC=clang HOSTCC=clang"
#compiler_flags="CC=clang HOSTCC=clang"

ARCH="x86-64"

YOCTO_URL="http://downloads.yoctoproject.org/releases/yocto/yocto-2.3/machines/qemu/qemu${ARCH}/"
#YOCTO_IMAGE="core-image-minimal-qemu${ARCH}.ext4"
#YOCTO_IMAGE="core-image-minimal-dev-qemu${ARCH}.ext4"
#YOCTO_IMAGE="core-image-sato-dev-qemu${ARCH}.ext4"
#YOCTO_IMAGE="core-image-sato-qemu${ARCH}.ext4"
YOCTO_IMAGE="core-image-sato-sdk-qemu${ARCH}.ext4"

# Number of processor threads
procs=$(nproc)

# Directory locations
img=${YOCTO_IMAGE}
confdir=${basedir}/config
initrd=${basedir}/initrd
srcdir=${basedir}/src
rootfs=${basedir}/rootfs
rootimg=${basedir}/rootfs.img
depsdir=${basedir}/deps
samplesdir=${basedir}/samples
busyboxdir=${depsdir}/busybox
labdir=labs
projdir=project

hostname=kernel-lab
rootfs_size=2048m
memory=512m

# Option to compile and copy kernel modules to rootfs
copy_modules_to_rootfs=y
copy_samples_to_rootfs=n

# Option to compile and copy lab sources to rootfs
copy_labs_to_rootfs=y
copy_proj_to_rootfs=y

debootstrap_arch=amd64
qemu_arch=x86_64
kernel_arch=x86_64

# Boot into initramfs shell
boot_into_initrd_shell=n

# Set this to yes to stop the CPU at boot and wait for debugger
wait_for_gdb_at_boot=n
qemu_debug_args="-s -S"

# Packages to install on rootfs
packages_to_install="vim sudo bpftrace bpfcc-tools gdb net-tools gcc make"
