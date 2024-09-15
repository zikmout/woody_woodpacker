#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void encrypt_section(void *section, size_t size, unsigned char key) {
    for (size_t i = 0; i < size; i++) {
        ((unsigned char *)section)[i] ^= key;  // XOR encryption
    }
}

void generate_random_key(unsigned char *key, size_t size) {
    FILE *urandom = fopen("/dev/urandom", "r");
    if (urandom == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fread(key, sizeof(unsigned char), size, urandom);
    fclose(urandom);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <64-bit ELF binary>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        perror("lseek");
        close(fd);
        return EXIT_FAILURE;
    }

    void *map = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return EXIT_FAILURE;
    }

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)map;
    if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Not a valid ELF file\n");
        munmap(map, file_size);
        close(fd);
        return EXIT_FAILURE;
    }

    // Locate the program's .text section (or relevant executable section)
    Elf64_Shdr *shdr = (Elf64_Shdr *)(map + ehdr->e_shoff);
    Elf64_Shdr *strtab = &shdr[ehdr->e_shstrndx];
    char *shstrtab = (char *)(map + strtab->sh_offset);

    for (int i = 0; i < ehdr->e_shnum; i++) {
        if (strcmp(&shstrtab[shdr[i].sh_name], ".text") == 0) {
            printf("Found .text section, offset: 0x%lx, size: 0x%lx\n", shdr[i].sh_offset, shdr[i].sh_size);
            
            // Generate a random encryption key
            unsigned char key[1];
            generate_random_key(key, 1);
            printf("key_value: %02X\n", key[0]);

            // Encrypt the .text section
            encrypt_section(map + shdr[i].sh_offset, shdr[i].sh_size, key[0]);

            // Write the modified binary to a new file named "woody"
            int woody_fd = open("woody", O_CREAT | O_RDWR, 0755);
            if (woody_fd < 0) {
                perror("woody");
                munmap(map, file_size);
                close(fd);
                return EXIT_FAILURE;
            }

            if (write(woody_fd, map, file_size) != file_size) {
                perror("write");
                munmap(map, file_size);
                close(fd);
                close(woody_fd);
                return EXIT_FAILURE;
            }

            close(woody_fd);
            break;
        }
    }

    munmap(map, file_size);
    close(fd);
    return EXIT_SUCCESS;
}
