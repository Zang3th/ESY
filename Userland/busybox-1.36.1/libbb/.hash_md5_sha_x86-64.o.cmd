cmd_libbb/hash_md5_sha_x86-64.o := gcc -Wp,-MD,libbb/.hash_md5_sha_x86-64.o.d  -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG  -DBB_VER='"1.36.1"' -malign-data=abi -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wno-format-security -Wdeclaration-after-statement -Wold-style-definition -finline-limit=0 -fno-builtin-strlen -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-builtin-printf -Os       -c -o libbb/hash_md5_sha_x86-64.o libbb/hash_md5_sha_x86-64.S

deps_libbb/hash_md5_sha_x86-64.o := \
  libbb/hash_md5_sha_x86-64.S \
    $(wildcard include/config/sha1/small.h) \
  /usr/include/stdc-predef.h \

libbb/hash_md5_sha_x86-64.o: $(deps_libbb/hash_md5_sha_x86-64.o)

$(deps_libbb/hash_md5_sha_x86-64.o):
