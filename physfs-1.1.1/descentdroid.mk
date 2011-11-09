CFLAGS += -Iphysfs-1.1.1/ -D_SZ_ONE_DIRECTORY -Dunix -DPHYSFS_NO_CDROM_SUPPORT -DPHYSFS_SUPPORTS_HOG

PHYSFS_OBJS := physfs-1.1.1/physfs.o \
physfs-1.1.1/physfs_byteorder.o \
physfs-1.1.1/physfs_unicode.o \
physfs-1.1.1/platform/os2.o \
physfs-1.1.1/platform/pocketpc.o \
physfs-1.1.1/platform/posix.o \
physfs-1.1.1/platform/unix.o \
physfs-1.1.1/platform/macosx.o \
physfs-1.1.1/platform/windows.o \
physfs-1.1.1/archivers/dir.o \
physfs-1.1.1/archivers/grp.o \
physfs-1.1.1/archivers/hog.o \
physfs-1.1.1/archivers/lzma.o \
physfs-1.1.1/archivers/mvl.o \
physfs-1.1.1/archivers/qpak.o \
physfs-1.1.1/archivers/wad.o \
physfs-1.1.1/archivers/zip.o \
physfs-1.1.1/lzma/7zBuffer.o \
physfs-1.1.1/lzma/7zCrc.o \
physfs-1.1.1/lzma/7zDecode.o \
physfs-1.1.1/lzma/7zExtract.o \
physfs-1.1.1/lzma/7zHeader.o \
physfs-1.1.1/lzma/7zIn.o \
physfs-1.1.1/lzma/7zItem.o \
physfs-1.1.1/lzma/7zMethodID.o \
physfs-1.1.1/lzma/LzmaDecode.o \
physfs-1.1.1/lzma/LzmaStateDecode.o

OBJS += libphysfs.a

cleanphysfs:
	@rm -f $(PHYSFS_OBJS)

libphysfs.a: $(PHYSFS_OBJS)
	@echo "AR     $@"
	@$(AR) cr $@ $^

.PHONY:	cleanphysfs
