#include <cstddef>
#include <cstdlib>
#include <err.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
struct MMapMetaData {
    void* addr = NULL;
    size_t length;
    int prot;
    int flags;
    int fd;
    off_t offset;
};

char* mmapCustom(MMapMetaData metadata) {

    return static_cast<char*>(mmap(metadata.addr, metadata.length, metadata.prot, metadata.flags,
                                   metadata.fd, metadata.offset));
}

int main() {
    MMapMetaData metadata;
    struct stat file_stat;
    metadata.fd = open("./test.txt", O_RDONLY);
    fstat(metadata.fd, &file_stat);
    metadata.length = file_stat.st_size;
    metadata.prot = PROT_READ;
    metadata.flags = MAP_PRIVATE;
    metadata.offset = 0;

    char* addr = mmapCustom(metadata);

    if (addr == MAP_FAILED) {
        err(EXIT_FAILURE, "mmap");
    }

    write(1, addr, metadata.length);
    munmap(addr, 100);
    close(metadata.fd);
}
